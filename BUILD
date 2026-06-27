load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "stage0_hello",
    srcs = [
        "src/main.cc",
        "src/cpu.cc",
        "src/firmware.cc",
        "src/uart.cc",
    ],
    hdrs = [
        "src/cpu.h",
        "src/firmware.h",
        "src/uart.h",
    ],
    copts = [
        "-std=c++20",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
    ],
)