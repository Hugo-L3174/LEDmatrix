cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(LEDmatrix)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_library(MatrixDriver STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/lib/Driver_IS31FL3731.c
)

target_include_directories(MatrixDriver PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lib/
)

target_link_libraries(MatrixDriver
    hardware_i2c
    pico_stdlib
)

add_executable(LEDmatrix
    LEDmatrix.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(LEDmatrix 
    MatrixDriver
    pico_stdlib
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(LEDmatrix)