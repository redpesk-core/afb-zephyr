# afb-zephyr

This repository provides `afb-zephyr`, a zephyr external module providing AFB libraries.

## Zephyr version compatibility

The `afb-zephyr` Zephyr external module has been tested with the following zephyr versions:

| Zephyr version | SDK version | Status       |
| -------------- | ----------- | ------------ |
| 4.0.0          | 0.17.0      | To be tested |
| 4.0.0          | 0.16.x      | OK           |
| 3.7.0          | 0.16.x      | OK           |

## How to use `afb-zephyr`

### Clone needed ressources

To use afb-binder libraries in your zephyr environment please follow the following steps:

- Deploy your Zephyr environment following your preferred version official guide:
  - [Zephyr 4.0.0 getting started guide](https://docs.zephyrproject.org/4.0.0/develop/getting_started/index.html)
  - [Zephyr 3.7.0 getting started guide](https://docs.zephyrproject.org/3.7.0/develop/getting_started/index.html)

- Clone `afb-zephyr` repo and it's submodules into your zephyr project environment (or in any other path)

```bash
cd ~/zephyrproject/
git clone --recurse-submodules http://git.ovh.iot/redpesk/redpesk-core/afb-zephyr.git
```

- Clone your zephyr application into your zephyr project environment

```bash
cd ~/zephyrproject/
mkdir applications && cd applications
git clone http://git.ovh.iot/aymeric/zephyr-binding-test.git
```

### Add `afb-zephyr` external module to your build environment

Multiple possibilities exist to add afb-zephyr path to your build environnement, you only need to apply one of the following:

- **Option 1** - Add `afb-zephyr` path to your zephyr application CmakeLists.txt file

```bash
# Add this line to ~/zephyrproject/applications/<your-app>/CmakeLists.txt
list(APPEND EXTRA_ZEPHYR_MODULES "$ENV{HOME}/zephyrproject/afb-zephyr")

# Build your app
cd ~/zephyrproject/applications/<your-app>
west build -p always -b <your_board>
```

- **Option 2** - Give the `afb-zephyr` path to west at build time

```bash
# Build your app
cd ~/zephyrproject/applications/<your-app>
west build -p always -b <your_board> . -DEXTRA_ZEPHYR_MODULES="~/zephyrproject/afb-zephyr"
```

- **Option 3** - Add `afb-zephyr` path to your ~/.zephyrrc file

```bash
# Populate ~/.zephyrrc file
echo "export EXTRA_ZEPHYR_MODULES=~/zephyrproject/afb-zephyr" >> ~/.zephyrrc

# Source ~/.zephyrrc using zephyr-env.sh
source ~/zephyrproject/zephyr/zephyr-env.sh

# Build your app
cd ~/zephyrproject/applications/<your-app>
west build -p always -b <your_board>
```

### Use AFB libraries capabilities in your zephyr application

To enable `afb-zephyr` zephyr external module, add `AFB_BINDER=y` to your zephyr application prj.conf file.

For now, several `afb-zephyr` module capabilities needs other Zephyr configuration to be enabled to work, please see the following examples:

- http://git.ovh.iot/aymeric/zephyr-binding-test