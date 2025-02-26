# Copyright (C) 2015-2025 IoT.bzh Company
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(afb-binding)
add_subdirectory(rp-lib-utils)
add_subdirectory(afb-libafb)

zephyr_include_directories(src)
add_subdirectory(src)
