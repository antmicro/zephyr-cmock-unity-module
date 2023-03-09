# Creating custom tests

Example tests are provided by this repository in the `tests/` directory. This
document goes into more detail on how to prepare custom testing scenarios using
the Zephyr CMock/Unity integration module.

## File structure

A test case consists of the following files and directories:

```
your_test_case
├── CMakeLists.txt
├── Kconfig
├── prj.conf
├── src
└── testcase.yaml
```

### CmakeLists.txt

The project recipe is located in the `CMakeLists.txt` file. It uses a standard
format expected by the Zephyr RTOS. Two CMake functions are provided by this
module that have to be used in this file:

* `test_runner_generate(<src_file>.c)`
* `cmock_handle(<path/to/zephyr/header>.h <header_category>)`

The `test_runner_generate` function points to a C source file that invokes the
`unity_main` function, and from which test cases are collected. This is
necessary for the Unity assertion engine to locate your test case asserts.

The `cmock_handle` function takes care of the mock/stub generation. The
`<header_category>` parameter helps organize generated mocks and stubs. If
unsure, use `zephyr` here.

### Kconfig

This imports Kconfig options from your sample. If you're not using any other
external modules, the following should suffice:

```
source "Kconfig.zephyr"
```

Otherwise source your additional external Kconfigs here.


### prj.conf

This module defines two Kconfig symbols:

* `CONFIG_UNITY`, to enable the usage of the Unity assertion engine
* `CONFIG_CMOCK`, to enable the usage of the CMock mock/stub generator

Both of them are needed for this module to work properly, so the minimal
`prj.conf` should look as follows:
```
CONFIG_UNITY=y
CONFIG_CMOCK=y
```

If a more specific configuration is needed, put it in this file as well.

### src

This directory hosts your test case source code. At least one file here, selected by
the `test_runner_generate` function, should invoke the `unity_main` function here, e.g.:

```
void main(void)
{
	(void)unity_main();
}
```

Test cases are collected automatically from this file as well. Each test case
should use at least one assertion macro. An example test case would look as
follows:

```
void test_example(void) {
	TEST_ASSERT_TRUE(2 == 2);
}
```

### testcase.yml

This file provides Twister integration. Twister is the Zephyr tool for running
tests. A simple `testcase.yml` should look as follows:

```
tests:
  testing.<test_name>:
    tags: testing <tags>
```

You can also allow-list and block-list platforms with `platform_allow` and
`platform_exclude`, respectively. For more details, visit the Zephyr documentation
[Test Runner (Twister) chapter](https://docs.zephyrproject.org/3.3.0/develop/test/twister.html#test-cases).

## Running tests

You can either run your test case manually with the `west` tool, or use
`twister` to automatically select platforms, test cases and analyze test
results and run them in one test suite.

### Manually

To run your test case manually with `west`, use the following command:

```
west build -p -b <platform_name> your_test_case -t run
```

This command assumes that `your_test_case` is located inside your current
working directory. For more details on how to use the `west` tool, please refer
to the Zephyr documentation [West (Zephyr's meta-tool) chapter](https://docs.zephyrproject.org/latest/develop/west/index.html).

### Automatically

To run tests automatically, use the following command:
```
$ZEPHYR_BASE/scripts/twister --testsuite-root test_suite
```

This command assumes that:

* The `ZEPHYR_BASE` environment variable is pointing to the Zephyr RTOS
  repository on your local drive
* The `test_suite` directory contains one or more test cases with the directory
  structure described earlier in this document

For more details on how to use `twister` test suite runner, please refer to the
Zephyr documentation [Test Runner (Twister) chapter](https://docs.zephyrproject.org/3.3.0/develop/test/twister.html#test-cases).

## Configuring Unity and CMock

Both Unity and CMock are configured using YAML files provided by this module.
These YAMLs are located in:
* `unity/unity_cfg.yaml`, for Unity
* `cmock/cmock_cfg.yaml`, for CMock

For a more detailed configuration guide, please refer to these documents
provided by Unity and CMock:
* [Unity documentation](https://github.com/ThrowTheSwitch/Unity/tree/master/docs)
* [CMock documentation](https://github.com/ThrowTheSwitch/CMock/blob/master/docs)
