cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
include(pico-sdk/pico_sdk_init.cmake)

set (PROJECT sbus-hid-bridge)
project(${PROJECT})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
file(GLOB LIB
        "lib/*.h"
        "lib/*.cpp"
)

file(GLOB SRC
        "src/*.h"
        "src/*.cpp"
)

file(GLOB PIOS
        "lib/*.pio"
#         "src/*.pio"
)

add_executable(${PROJECT}
        ${SRC}
        ${LIB}
)
target_include_directories(${PROJECT}
        PRIVATE "."
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${PROJECT} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include)

# make sure TinyUSB sources link against our callbacks
target_sources(${PROJECT} PUBLIC
        src/usb_descriptors.c
)

# enable USB serial output
pico_enable_stdio_usb(${PROJECT} 0)
pico_enable_stdio_uart(${PROJECT} 1)

# compile pio file
pico_generate_pio_header(${PROJECT}
        ${PIOS}
        OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated
)

target_link_libraries(${PROJECT}
        pico_stdlib
        hardware_irq
        pico_multicore
        pico_unique_id
        tinyusb_device
        tinyusb_board
        )

pico_set_program_name(${PROJECT} "${PROJECT}")
pico_set_program_version(${PROJECT} "0.2")

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT})