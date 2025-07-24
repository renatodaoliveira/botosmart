// mqtt_config.h - VERSÃO FINAL

#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <stdbool.h>

void mqtt_iniciar();
void mqtt_publicar(const char *topico, const char *mensagem);
bool mqtt_esta_conectado();

// A função correta que verifica se o comando 'ACK' foi recebido
bool mqtt_comando_ack_recebido();

#endif