cc_binary(
    name = "baremetal",
    srcs = [
        "src/main.cpp",
        "src/cpu.cpp",
        "src/cpu.h",
        "src/uart.cpp",
        "src/uart.h",
        "src/firmware.cpp",
        "src/firmware.h",
        "src/memory_map.cpp",
        "src/memory_map.h",
        "src/memory_region.cpp",
        "src/memory_region.h",
        "src/st_link.h",
        "src/st_link.cpp",
        "src/gpio.cpp", 
        "src/gpio.h"
    ],
    copts = [
        "-std=c++20",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
    ],
)