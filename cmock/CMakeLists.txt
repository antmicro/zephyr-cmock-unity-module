# Copyright (c) 2023 Antmicro <www.antmicro.com>
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.22.1)

set_property(GLOBAL PROPERTY CMOCK_UNITY_MODULE_CMOCK_BASE ${CMAKE_CURRENT_LIST_DIR})

get_property(CMOCK_UNITY_MODULE_BASE GLOBAL PROPERTY CMOCK_UNITY_MODULE_BASE)
set_property(GLOBAL PROPERTY CMOCK_BASE ${CMOCK_UNITY_MODULE_BASE}/../test/cmock)
get_property(CMOCK_BASE GLOBAL PROPERTY CMOCK_BASE)

# add CMock mock/stub generator sources as a part of this library
zephyr_library()
zephyr_include_directories(
    ${CMOCK_BASE}/src
    ${CMAKE_CURRENT_LIST_DIR}
)
zephyr_library_sources(
    ${CMOCK_BASE}/src/cmock.c
)

function(cmock_handle header_file category)
    # get header_file basename
    get_filename_component(header_file_basename_we ${header_file} NAME_WE)

    # set CMock output directory to `build/cmock/<category>`
    set(cmock_base ${APPLICATION_BINARY_DIR}/cmock)
    set(cmock_output_base ${cmock_base}/${category})
    file(MAKE_DIRECTORY ${cmock_output_base})
    message(STATUS "Created CMock output directory: ${cmock_output_base}")

    # check if ruby is present on the system
    find_program(ruby ruby REQUIRED)
    message(STATUS "Found ruby: ${ruby}")

    # find the `cmock.rb` script from CMock
    get_property(CMOCK_BASE GLOBAL PROPERTY CMOCK_BASE)
    set(cmock ${CMOCK_BASE}/lib/cmock.rb)
    message(STATUS "Found cmock.rb script: ${cmock}")

    # find CMock YAML configuration file
    get_property(CMOCK_UNITY_MODULE_CMOCK_BASE GLOBAL PROPERTY CMOCK_UNITY_MODULE_CMOCK_BASE)
    set(cmock_config ${CMOCK_UNITY_MODULE_CMOCK_BASE}/cmock_cfg.yaml)
    message(STATUS "Found CMock YAML configuration file: ${cmock_config}")

    # run the `cmock.rb` script
    message(STATUS "Running cmock.rb script for ${header_file}...")
    execute_process(COMMAND ${ruby} ${cmock}
        --mock_path=${cmock_output_base}
        -o${cmock_config}
	${header_file}
        WORKING_DIRECTORY ${APPLICATION_SOURCE_DIR}
        RESULT_VARIABLE cmock_result
        OUTPUT_VARIABLE cmock_output)

    if(cmock_result EQUAL "1")
        message(FATAL_ERROR "CMock mock/stub generator: bad exit status!")
    endif()

    target_include_directories(app BEFORE PRIVATE ${cmock_base})
    target_sources(app PRIVATE ${cmock_output_base}/mock_${header_file_basename_we}.c)
endfunction()
