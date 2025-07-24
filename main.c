#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware_oled.h"
#include "config.h"
#include "mqtt_config.h"
#include "vl53l0x.h"

typedef enum {
    ESTADO_ANALISANDO,
    ESTADO_ALERTA_ATIVO
} SystemState;

int main() {
    stdio_init_all();
    sleep_ms(2000); // Dá tempo para o host detectar o USB

   hardware_init();

    // Conexão Wi-Fi


    // Processo de conexão Wi-Fi
    printf("Conectando ao Wi-Fi...\n");
    hardware_oled_exibir("Wi-Fi", "Conectando");

    while (1) {
        if (cyw43_arch_init()) {
            printf("Falha ao inicializar Wi-Fi\n");
            hardware_oled_exibir("Wi-Fi", "Falha init");
            sleep_ms(2000); // Aguarda antes de tentar novamente
            continue;
        }
        cyw43_arch_enable_sta_mode();
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
            printf("Falha ao conectar ao Wi-Fi\n");
            hardware_oled_exibir("Wi-Fi", "Falha conexao");
            sleep_ms(2000); // Aguarda antes de tentar novamente
            continue;
        }
        break; // Conectou com sucesso
    }

    printf("Wi-Fi conectado!\n");
    hardware_oled_exibir("Wi-Fi", "Conectado");
    sleep_ms(1000);

    mqtt_iniciar(); // inicializa MQTT

    hardware_oled_exibir("", "   Analisando   ");
    SystemState estado_atual = ESTADO_ANALISANDO;

    // Inicializa sensor VL53L0X
    vl53l0x_init(I2C0_PORT);
    vl53l0x_start_ranging(I2C0_PORT);

    while (true) {
        cyw43_arch_poll(); // mantém rede viva

        // Variáveis para média das leituras
        uint32_t soma = 0;
        uint16_t leituras_validas = 0;

        // Leitura de distância
        uint16_t distancia_mm = 0;
        for (int i = 0; i < 50; i++) {
            vl53l0x_start_ranging(I2C0_PORT); // inicia nova medição
            sleep_ms(5); // tempo para o sensor medir
            uint16_t dist = 0;
            if (vl53l0x_read_distance(I2C0_PORT, &dist)) {
                soma += dist;
                leituras_validas++;
            }
            sleep_ms(6); // 50 leituras x 6ms = 300ms
        }

        if (leituras_validas > 0) {
            uint16_t media = soma / leituras_validas;
            uint16_t media_cm = media / 10;
            char msg_alerta[50];
            sprintf(msg_alerta, "%d", media);
            mqtt_publicar(TOPICO_MEDICOES, msg_alerta);
            printf("Distância média: %d cm (%d leituras)\n", media_cm, leituras_validas);

            if (media_cm < DISTANCIA_LIMIAR_CM) {
                if (estado_atual != ESTADO_ALERTA_ATIVO) {
                    mqtt_publicar(TOPICO_ALERTA, "Anomalia Detectada!");
                    estado_atual = ESTADO_ALERTA_ATIVO;
                }
                gpio_put(LED_VERDE_PIN, 0);
                gpio_put(LED_VERMELHO_PIN, 1);
                gpio_put(RELER_PIN, 1);
            } else{
                if (estado_atual != ESTADO_ANALISANDO) {
                mqtt_publicar(TOPICO_ALERTA, "Sem Anomalias");
                estado_atual = ESTADO_ANALISANDO;
                }
                gpio_put(LED_VERDE_PIN, 1);
                gpio_put(LED_VERMELHO_PIN, 0);
                gpio_put(RELER_PIN, 0);
                
            }
        }

        sleep_ms(500);
    }

    return 0;
}