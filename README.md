# afb-zephyr

This repository provides `afb-zephyr`, a zephyr external module providing
AFB libraries. The module is also providing some zephyr specific sources
intended to make the glue between the original libraries and their use in
a zephyr application.

## Zephyr version compatibility

The `afb-zephyr` Zephyr external module has been tested with the following
zephyr versions:

| Zephyr version | SDK version | Status       |
| -------------- | ----------- | ------------ |
| 4.0.0          | 0.17.0      | To be tested |
| 4.0.0          | 0.16.x      | OK           |
| 3.7.0          | 0.16.x      | OK           |

## How to use `afb-zephyr`

### Clone needed ressources

To use afb-binder libraries in your zephyr environment please follow
the following steps:

- Deploy your Zephyr environment following your preferred version official guide:
  - [Zephyr 4.0.0 getting started guide](https://docs.zephyrproject.org/4.0.0/develop/getting_started/index.html)
  - [Zephyr 3.7.0 getting started guide](https://docs.zephyrproject.org/3.7.0/develop/getting_started/index.html)

- Clone `afb-zephyr` repo and it's submodules into your zephyr project
  environment (or in any other path)

```bash
cd ~/zephyrproject/
git clone --recurse-submodules --remote https://github.com/redpesk-core/afb-zephyr.git

# You may want to update your module afterward
cd ~/zephyrproject/afb-zephyr
git pull origin master
git submodule update --recursive --remote
```

- Clone your zephyr application into your zephyr project environment.
  Here we clone the directory giving some examples of how to use afb-zephyr.

```bash
cd ~/zephyrproject/
mkdir applications && cd applications
git clone https://github.com/redpesk-samples/afb-zephyr-samples.git
```

### Add `afb-zephyr` external module to your build environment

Multiple possibilities exist to add afb-zephyr path to your build environnement,
you only need to apply one of the following:

- **Option 1** - Add `afb-zephyr` path to your zephyr application CmakeLists.txt file

```cmake
# Add this line to ~/zephyrproject/applications/<your-app>/CmakeLists.txt
list(APPEND EXTRA_ZEPHYR_MODULES "$ENV{HOME}/zephyrproject/afb-zephyr")
```

```bash
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

- **Option 4** - Add use of AFB\_ZEPHYR\_DIR in CMakeLists.txt

```cmake
# Add this lines to ~/zephyrproject/applications/<your-app>/CmakeLists.txt
if("" STREQUAL "$ENV{AFB_ZEPHYR_DIR}")
   set(ENV{AFB_ZEPHYR_DIR} "$ENV{HOME}/zephyrproject/afb-zephyr")
endif()
list(APPEND EXTRA_ZEPHYR_MODULES "$ENV{AFB_ZEPHYR_DIR}")
```

```bash
# Build your app
cd ~/zephyrproject/applications/<your-app>
west build -p always -b <your_board>
```

### Use AFB libraries capabilities in your zephyr application

To enable `afb-zephyr` zephyr external module, please use the following
configs in your application prj.conf file (by prefixing them with `CONFIG_`)
or enable them through menuconfig (path: `(Top) -> Modules -> afb-zephyr`).

| CONFIG Name   | Default value | Comment                              |
| ------------- | ------------- | ------------------------------------ |
| AFB\_ZEPHYR   | n             | Enables `afb-zephyr` module          |
| AFB\_RPC\_NET | n             | Enables `afb-zephyr` RPC over TCP/IP |


## Examples

Several examples using `afb-zephyr` module are available
[here](https://github.com/redpesk-samples/afb-zephyr-samples).

These examples are also showing the the use of the helpers given
in the directory `src`.
