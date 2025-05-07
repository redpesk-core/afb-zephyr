# Copyright (C) 2015-2025 IoT.bzh Company
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(zafb-helpers.c)

if (CONFIG_AFB_ZEPHYR_LLEXT_LOADER)
	zephyr_compile_definitions(WITH_ZEPHYR_LLEXT=1)
else()
	zephyr_compile_definitions(WITH_ZEPHYR_LLEXT=0)
endif()
