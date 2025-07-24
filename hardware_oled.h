// hardware_oled.h

#ifndef HARDWARE_OLED_H
#define HARDWARE_OLED_H

#include <stdbool.h> // Necessário para usar os tipos 'true' e 'false'

/**
 * @brief Inicializa todo o hardware necessário (as duas portas I2C, GPIOs, Sensores, Display).
 * Deve ser chamada uma vez no início do programa.
 */
void hardware_init();

/**
 * @brief Exibe duas linhas de texto no display OLED na porta I2C1.
 *
 * @param linha1 Ponteiro para a string da primeira linha.
 * @param linha2 Ponteiro para a string da segunda linha.
 */
void hardware_oled_exibir(const char* linha1, const char* linha2);

/**
 * @brief Limpa completamente a tela do display OLED.
 */
void hardware_oled_limpar();

#endif // HARDWARE_OLED_H