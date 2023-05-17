# Copyright (c) 2023 Antmicro <www.antmicro.com>
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.22.1)

set_property(GLOBAL PROPERTY CMOCK_UNITY_MODULE_UNITY_BASE ${CMAKE_CURRENT_LIST_DIR})

get_property(CMOCK_UNITY_MODULE_BASE GLOBAL PROPERTY CMOCK_UNITY_MODULE_BASE)
set_property(GLOBAL PROPERTY UNITY_BASE ${CMOCK_UNITY_MODULE_BASE}/../test/cmock/vendor/unity)
get_property(UNITY_BASE GLOBAL PROPERTY UNITY_BASE)

# include custom `unity_config.h`
zephyr_compile_definitions(UNITY_INCLUDE_CONFIG_H)

# add Unity Test Runner sources as a part of this library
zephyr_library()
zephyr_include_directories(
    ${UNITY_BASE}/src
    ${CMAKE_CURRENT_LIST_DIR}
)
zephyr_library_sources(
    ${UNITY_BASE}/src/unity.c
    generic_teardown.c
)

function(test_runner_generate test_file)
    # check if ruby is present on the system
    find_program(ruby ruby REQUIRED)
    message(STATUS "Found ruby: ${ruby}")

    # get path to the Unity Test Runner generator script
    get_property(CMOCK_UNITY_MODULE_BASE GLOBAL PROPERTY CMOCK_UNITY_MODULE_BASE)
    set(cmock_unity_project_root ${CMOCK_UNITY_MODULE_BASE}/..)
    set(unity_generator ${cmock_unity_project_root}/test/cmock/vendor/unity/auto/generate_test_runner.rb)
    message(STATUS "Found Unity Test Runner generator script: ${unity_generator}")

    # get path to the Unity Test Runner generator YAML configuration file
    get_property(CMOCK_UNITY_MODULE_UNITY_BASE GLOBAL PROPERTY CMOCK_UNITY_MODULE_UNITY_BASE)
    set(UNITY_CONFIG ${CMOCK_UNITY_MODULE_UNITY_BASE}/unity_cfg.yaml CACHE FILEPATH "Path to custom unity config file")
    message(STATUS "Found Unity Test Runner generator YAML config file: ${UNITY_CONFIG}")

    # set Test Runner output directory to `build/unity`
    set(unity_output_base ${APPLICATION_BINARY_DIR}/unity)
    file(MAKE_DIRECTORY ${unity_output_base})
    message(STATUS "Created Unity Test Runner generator output directory: ${unity_output_base}")

    # prepend Test Runner generator output file name with `generated_`
    get_filename_component(test_file_basename ${test_file} NAME)
    set(unity_generated_runner_name "generated_${test_file_basename}")
    set(unity_generated_runner ${unity_output_base}/${unity_generated_runner_name})

    # run Unity Test Runner generator script
    message(STATUS "Running Unity Test Runner Generator for ${test_file}...")
    execute_process(COMMAND ${ruby} ${unity_generator} ${UNITY_CONFIG} ${test_file} ${unity_generated_runner}
        WORKING_DIRECTORY ${APPLICATION_SOURCE_DIR}
        RESULT_VARIABLE generator_result
        OUTPUT_VARIABLE generator_output)

    # check if the Test runner generator ran succesfully
    if(generator_result EQUAL "1")
        message(FATAL_ERROR "Unity Test Runner generator: bad exit status!")
    endif()

    # add Zephyr app sources
    target_sources(app PRIVATE ${unity_generated_runner})
    message(STATUS "Generated Unity Test Runner: ${unity_generated_runner}")
    message(STATUS "Unity Test Runner generation done")
endfunction()
