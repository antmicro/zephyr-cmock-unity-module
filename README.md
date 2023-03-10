# Zephyr CMock Unity Module

Copyright (c) 2023 [Antmicro](https://www.antmicro.com)

This Zephyr module provides an integration with CMock mock generator and the
Unity Test framework. For a more in-depth test creation description using this
module refer to [this document](CREATING_TESTS.md).

## Installation

This instruction was tested on Debian Bookworm and Ubuntu 22.04. Install
dependencies:

<!-- name="install-dependencies" -->
```
apt update
apt install -y cmake git ninja-build gperf ccache dfu-util device-tree-compiler \
	wget python3-dev python3-pip python3-setuptools python3-tk python3-wheel \
	xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1 curl \
	ruby
```

Setup Zephyr SDK:

<!-- name="zephyr-sdk-setup" -->
```
mkdir -p zephyr-sdk
pushd zephyr-sdk
curl -kL https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.15.2/zephyr-sdk-0.15.2_linux-x86_64.tar.gz | tar xz --strip 1
./setup.sh -t all -h -c
export ZEPHYR_SDK_INSTALL_DIR=$(pwd)
popd
```

Clone this repository:
```
git clone https://github.com/antmicro/zephyr-cmock-unity-module.git
cd zephyr-cmock-unity-module
```

Setup Python dependencies:

<!-- name="python-requirements" -->
```
pip3 install -r requirements.txt
```

An example project manifest that uses this module is provided in
`project-example.yml`. To use it, type:

<!-- name="west-init" -->
```
west init --local --mf project-example.yml
```

Next, set Zephyr RTOS and its dependencies up:

<!-- name="west-update" -->
```
west update
west zephyr-export
pip3 install -r ../zephyr/scripts/requirements.txt
```

## Samples

You can build and run samples with:

<!-- name="west-build" -->
```
west build -p -b native_posix tests/unity -t run
west build -p -b native_posix tests/cmock -t run
```

You can also run samples in Renode with:
```
west build -p -b <platform_name> tests/unity -t run_renode
west build -p -b <platform_name> tests/cmock -t run_renode
```
Where `<platform_name>` is the Zephyr platform of your choice.

## Twister

Tests located under the `tests` directory support Zephyr's `twister` script. To
use it, run the following command from this module's main directory:
```
../zephyr/scripts/twister --testsuite-root tests
```

You can also select a specific platform you want to run these tests on with
Twister. To do that, use the `-p <platform_name>` switch:
```
../zephyr/scripts/twister -p <platform_name> --testsuite-root tests
```
