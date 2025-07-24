// mqtt_config.c - VERSÃO FINAL

#include "config.h"
#include "mqtt_config.h"
#include "lwip/apps/mqtt.h"

static mqtt_client_t *client;
static volatile bool g_comando_ack_recebido = false; // A "bandeira"

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    char payload[20];
    strncpy(payload, (const char *)data, len);
    payload[len] = '\0';

    if (strcmp(payload, "ACK") == 0) {
        g_comando_ack_recebido = true; // Levanta a bandeira
    }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    // Apenas para debug, se necessário
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("[MQTT] Conectado ao broker!\n");
        mqtt_subscribe(client, TOPICO_CONEXAO, 0, NULL, NULL);
    } else {
        printf("[MQTT] Falha na conexao: %d\n", status);
    }
}

void mqtt_iniciar() {
    ip_addr_t broker_ip;
    ip4addr_aton(MQTT_BROKER_IP, &broker_ip);

    client = mqtt_client_new();
    struct mqtt_connect_client_info_t ci = {0};
    ci.client_id = MQTT_CLIENT_ID;

    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);
    mqtt_client_connect(client, &broker_ip, MQTT_BROKER_PORT, mqtt_connection_cb, NULL, &ci);
}

bool mqtt_comando_ack_recebido() {
    if (g_comando_ack_recebido) {
        g_comando_ack_recebido = false; // Abaixa a bandeira depois de verificar
        return true;
    }
    return false;
}

void mqtt_publicar(const char *topico, const char *mensagem) {
    if (!client || !mqtt_client_is_connected(client)) return;
    mqtt_publish(client, topico, mensagem, strlen(mensagem), 1, 0, NULL, NULL);
}

bool mqtt_esta_conectado() { return client && mqtt_client_is_connected(client); }