# Copyright (c) 2023 Antmicro <www.antmicro.com>
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)
set(ENV{MODULE_CMOCK_DIR} ../../cmock)

test_runner_generate(src/main.c)
cmock_handle(${ZEPHYR_BASE}/include/zephyr/drivers/gpio.h drivers)

target_sources(app PRIVATE src/main.c)
