// config.h - VERSÃO COMPLETA E CORRETA

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/cyw43_arch.h"


// --- CONSTANTES DE CONFIGURAÇÃO ---
#define DISTANCIA_LIMIAR_CM   15

// --- CONFIGURAÇÕES ---
#define RELER_PIN             8

// --- CONFIGURAÇÕES DE LEDS ---
#define LED_VERDE_PIN         11
#define LED_VERMELHO_PIN      13

// --- CONFIGURAÇÃO DAS PORTAS I2C ---
// Porta I2C 1 (VL53L0X e OLED)

// Porta I2C 1 (OLED)
#define I2C1_PORT          i2c1
#define I2C1_SDA_PIN       14
#define I2C1_SCL_PIN       15

// Porta I2C 0 (Sensor de Distancia)
#define I2C0_PORT             i2c0
#define I2C0_SDA_PIN          0
#define I2C0_SCL_PIN          1
#define VL53L0X_ADDR          0x29


// --- CONFIGURAÇÃO DE REDE E MQTT 
#define WIFI_SSID             "EmbarcaTech"
#define WIFI_PASSWORD         "iot123456"
#define MQTT_BROKER_IP        "52.28.242.147"
#define MQTT_BROKER_PORT      1883 
#define MQTT_CLIENT_ID        "PicoWMonitorAlagamentos"

// --- TÓPICOS MQTT ---
#define TOPICO_CONEXAO       "monitor/boia/conexao"
#define TOPICO_MEDICOES       "monitor/boia/medicoes"
#define TOPICO_ALERTA         "monitor/boia/alerta"



#endif // CONFIG_H