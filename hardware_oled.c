// hardware_handler.c - VERSÃO COMPLETA E FUNCIONAL

#include "config.h"
#include "hardware_oled.h"
#include "hardware/i2c.h"
#include "ssd1306_font.h" 

// Definições e Comandos do SSD1306
#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128
#define SSD1306_NUM_PAGES (SSD1306_HEIGHT / 8)
#define SSD1306_BUF_LEN (SSD1306_NUM_PAGES * SSD1306_WIDTH)

#define SSD1306_SET_DISPLAY_ON 0xAF
#define SSD1306_SET_DISPLAY_OFF 0xAE
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_SET_ENTIRE_ON 0xA4
#define SSD1306_SET_NORMAL_DISPLAY 0xA6
#define SSD1306_SET_MEMORY_MODE 0x20
#define SSD1306_SET_PAGE_ADDRESS 0x22
#define SSD1306_SET_COLUMN_ADDRESS 0x21

static uint8_t oled_buffer[SSD1306_BUF_LEN];

static void oled_send_cmd(uint8_t cmd) {
    uint8_t buf[2] = {0x80, cmd};
    i2c_write_blocking(i2c1, SSD1306_I2C_ADDR, buf, 2, false);
}

static void oled_render() {
    oled_send_cmd(SSD1306_SET_COLUMN_ADDRESS);
    oled_send_cmd(0); oled_send_cmd(SSD1306_WIDTH - 1);
    oled_send_cmd(SSD1306_SET_PAGE_ADDRESS);
    oled_send_cmd(0); oled_send_cmd(SSD1306_NUM_PAGES - 1);
    uint8_t buf[SSD1306_BUF_LEN + 1];
    buf[0] = 0x40;
    memcpy(buf + 1, oled_buffer, SSD1306_BUF_LEN);
    i2c_write_blocking(i2c1, SSD1306_I2C_ADDR, buf, sizeof(buf), false);
}

static void oled_draw_char(int16_t x, int16_t y, char c) {
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;
    int char_idx = (c >= 'A' && c <= 'Z') ? (c - 'A' + 1) : ((c >= 'a' && c <= 'z') ? (c - 'a' + 1) : ((c >= '0' && c <= '9') ? (c - '0' + 27) : 0));
    for (int i = 0; i < 8; i++) {
        uint8_t line = font[char_idx * 8 + i];
        for (int j = 0; j < 8; j++) {
            if ((line >> j) & 1) {
                int px = x + i; int py = y + j;
                if (px < SSD1306_WIDTH && py < SSD1306_HEIGHT) {
                    int byte_idx = (py / 8) * SSD1306_WIDTH + px;
                    if(byte_idx < sizeof(oled_buffer)) oled_buffer[byte_idx] |= (1 << (py % 8));
                }
            }
        }
    }
}

static void oled_draw_string(int16_t x, int16_t y, const char *s) {
    while (*s) { oled_draw_char(x, y, *s++); x += 8; }
}


void hardware_init() {

    i2c_init(i2c1, 400 * 1000); // I2C1 para Display
    gpio_set_function(I2C1_SDA_PIN, GPIO_FUNC_I2C); gpio_pull_up(I2C1_SDA_PIN);
    gpio_set_function(I2C1_SCL_PIN, GPIO_FUNC_I2C); gpio_pull_up(I2C1_SCL_PIN);

    i2c_init(i2c0, 100 * 1000); // 100kHz
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C); gpio_pull_up(I2C0_SDA_PIN);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C); gpio_pull_up(I2C0_SCL_PIN);

    gpio_init(LED_VERMELHO_PIN); gpio_set_dir(LED_VERMELHO_PIN, GPIO_OUT);
    gpio_init(LED_VERDE_PIN); gpio_set_dir(LED_VERDE_PIN, GPIO_OUT);
    gpio_init(RELER_PIN); gpio_set_dir(RELER_PIN, GPIO_OUT);

    oled_send_cmd(SSD1306_SET_DISPLAY_OFF);
    oled_send_cmd(0xD5); oled_send_cmd(0x80); oled_send_cmd(0xA8); oled_send_cmd(SSD1306_HEIGHT - 1);
    oled_send_cmd(0xD3); oled_send_cmd(0x0); oled_send_cmd(0x40); oled_send_cmd(0x8D); oled_send_cmd(0x14);
    oled_send_cmd(0x20); oled_send_cmd(0x00); oled_send_cmd(0xA1); oled_send_cmd(0xC8);
    oled_send_cmd(0xDA); oled_send_cmd(0x12); oled_send_cmd(SSD1306_SET_CONTRAST); oled_send_cmd(0xFF);
    oled_send_cmd(0xD9); oled_send_cmd(0xF1); oled_send_cmd(0xDB); oled_send_cmd(0x40);
    oled_send_cmd(SSD1306_SET_ENTIRE_ON); oled_send_cmd(SSD1306_SET_NORMAL_DISPLAY);
    oled_send_cmd(SSD1306_SET_DISPLAY_ON);
    sleep_ms(20);
    hardware_oled_exibir("Hardware OK!", "");
}

void hardware_oled_exibir(const char* linha1, const char* linha2) {
    memset(oled_buffer, 0, sizeof(oled_buffer));
    oled_draw_string(0, 8, linha1);
    oled_draw_string(0, 24, linha2);
    oled_render();
}

void hardware_oled_limpar() {
    memset(oled_buffer, 0, sizeof(oled_buffer));
    oled_render();
}

void hardware_led_set(bool on) { gpio_put(LED_VERMELHO_PIN, on); }