/*
 * Copyright (c) 2015-2025 IoT.bzh Company
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZAFB_HELPERS_INCLUDED
#define ZAFB_HELPERS_INCLUDED

#define afbBindingRoot (zafb_root_api())

#include <afb-v4.h>
#include <afb-apis.h>

/* get the virtual root api */
extern afb_api_t zafb_root_api();

/* add a binding and get it's api */
extern int zafb_binding_add(afb_api_t *api, const afb_binding_t *binding);

/* add a RPC client of the serveur given by the URI */
extern int zafb_add_rpc_client(const char *uri);

/* add a RPC server for the URI */
extern int zafb_add_rpc_server(const char *uri);

/* start all recorded apis, can be called several times */
extern int zafb_start_all_api();

/* start the frame work scheduler
 * start is a callback invoked for setting up framework items
 * maxjobs is the maximum count of jobs
 * maxsessions is the maximal count of sessions
 * maxthreads is the maxinal count of threads
 */
extern int zafb_start(void (*start)(), int maxjobs, int maxsessions, int maxthreads);

/* exit the framework scheduler with the given code */
extern void zafb_exit(int code);

#if CONFIG_AFB_ZEPHYR_MBEDTLS
extern int zafb_tls_add_trust_list(const void *data, size_t size);
extern int zafb_tls_set_certificate(const void *data, size_t size);
extern int zafb_tls_set_private_key(const void *data, size_t size);
#endif /* CONFIG_AFB_ZEPHYR_MBEDTLS */

#if WITH_ZEPHYR_LLEXT
#include <zephyr/llext/llext.h>
#if CONFIG_USERSPACE
extern int zafb_load_llext_binding(const char *name, struct llext **ext, const void *buf,
                            size_t len, k_mem_domain domain);
#else /* CONFIG_USERSPACE*/
extern int zafb_load_llext_binding(const char *name, struct llext **ext, const void *buf,
                            size_t len);
#endif /* CONFIG_USERSPACE*/
#endif /* WITH_ZEPHYR_LLEXT */

#endif /* ZAFB_HELPERS_INCLUDED */
