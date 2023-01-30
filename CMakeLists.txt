# Copyright (c) 2023 Antmicro <www.antmicro.com>
# SPDX-License-Identifier: Apache-2.0

set_property(GLOBAL PROPERTY CMOCK_UNITY_MODULE_BASE ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory_ifdef(CONFIG_UNITY unity)
add_subdirectory_ifdef(CONFIG_CMOCK cmock)
