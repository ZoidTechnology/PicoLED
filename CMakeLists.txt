cmake_minimum_required(VERSION 3.16)

include(sdk/pico_sdk_init.cmake)

project(picoled)

pico_sdk_init()

add_executable(picoled
	source/main.c
	source/led.c
	source/descriptors.c
)

target_include_directories(picoled PRIVATE source)

target_link_libraries(picoled
	pico_stdlib
	hardware_pio
	tinyusb_device
)

target_compile_options(picoled PRIVATE -Wall -Wextra)

pico_add_extra_outputs(picoled)