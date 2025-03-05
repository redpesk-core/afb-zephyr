# Copyright (C) 2015-2025 IoT.bzh Company
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_AFB_ZEPHYR)
	add_subdirectory(afb-binding)
	add_subdirectory(rp-lib-utils)
	add_subdirectory(afb-libafb)

	zephyr_include_directories(src)
	add_subdirectory(src)
endif()
if(CONFIG_AFB_LLEXT_BINDING)
	add_compile_definitions(AFB_BINDING_VERSION=4)
	add_subdirectory(afb-binding)
endif()
