# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define o tipo de placa
set(PICO_BOARD pico_w CACHE STRING "Tipo de placa")

# Inclui o Raspberry Pi Pico SDK (deve ser antes do projeto)
include(pico_sdk_import.cmake)

# Declara o projeto CMake
project(Monitor C CXX ASM)

# Inicializa o SDK do Raspberry Pi Pico
pico_sdk_init()

# Adiciona o executável com todos os arquivos fonte (.c)
add_executable(Monitor
    main.c
    hardware_oled.c
    mqtt_config.c
    vl53l0x.c
)

target_link_libraries(Monitor
    pico_stdlib
)

# Habilita a saída de terminal via USB
pico_enable_stdio_usb(Monitor 1)
pico_enable_stdio_uart(Monitor 0)


# Vincula todas as bibliotecas necessárias em um só lugar
# <<< MELHORIA: Tudo em um único comando para maior clareza
target_link_libraries(Monitor
    hardware_i2c
    hardware_timer
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
)

# Adiciona os diretórios de inclusão para os arquivos de cabeçalho (.h)
target_include_directories( Monitor PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib/ssd1306 
)

# Adiciona a geração de outros formatos de binário (uf2, etc.)
pico_add_extra_outputs(Monitor)