/*
 * Copyright (c) 2015-2025 IoT.bzh Company
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zafb-helpers.h"
#include <stdio.h>
#include <zephyr/kernel.h>

#include <sys/rp-verbose.h>
#include <afb-v4.h>
#include <afb-apis.h>

/******************************************/
/* AFB ZEPHYR INTEGRATION SECTION - START */
/******************************************/
static struct afb_apiset *__zafb_all_apis__;

struct afb_apiset *zafb_apiset()
{
	if (__zafb_all_apis__ == NULL) {
		__zafb_all_apis__ = afb_apiset_create(NULL, 0);
	}
	return __zafb_all_apis__;
}	

static int __zafb_binding_add_preinit__(afb_api_t api, void *closure)
{
	int rc;
	const afb_binding_t *binding = closure;
	union afb_ctlarg ctlarg;

	/* record the description */
	afb_api_v4_set_userdata(api, binding->userdata);
	afb_api_v4_set_mainctl(api, binding->mainctl);
	afb_api_v4_set_verbs(api, binding->verbs);
	rc = 0;
	if (binding->provide_class)
		rc =  afb_api_v4_class_provide(api, binding->provide_class);
	if (!rc && binding->require_class)
		rc =  afb_api_v4_class_require(api, binding->require_class);
	if (!rc && binding->require_api)
		rc =  afb_api_v4_require_api(api, binding->require_api, 0);
	if (rc >= 0 && binding->mainctl) {
		/* call the pre init routine safely */
		memset(&ctlarg, 0, sizeof ctlarg);
		ctlarg.pre_init.path = NULL;
		ctlarg.pre_init.uid = binding->api;
		ctlarg.pre_init.config = NULL;
		rc = afb_api_v4_safe_ctlproc(api, binding->mainctl, afb_ctlid_Pre_Init, &ctlarg);
	}

	/* seal after init allows the pre init to add things */
	if (rc >= 0)
		afb_api_v4_seal(api);
	return rc;
}

int zafb_binding_add(afb_api_t *api, const afb_binding_t *binding)
{
	int rc = afb_api_v4_create(
		api,
		zafb_apiset(), zafb_apiset(),
		binding->api, Afb_String_Const,
		binding->info, Afb_String_Const,
		binding->noconcurrency,
		__zafb_binding_add_preinit__, (void*)binding,
		NULL, Afb_String_Const);
	return rc;
}

int zafb_start_all_api()
{
	return afb_apiset_start_all_services(zafb_apiset());
}

afb_api_t zafb_root_api()
{
	static afb_api_t root = NULL;
	if (root == NULL) {
		afb_api_v4_create(
			&root,
			zafb_apiset(), zafb_apiset(),
			"ROOT", Afb_String_Const,
			NULL, Afb_String_Const,
			0,
			NULL, NULL,
			NULL, Afb_String_Const);
	}
	return root;
}

static void __start__(int signum, void *arg)
{
	void (*start)() = arg;
	zafb_root_api();
	start();
	zafb_start_all_api();
}

int zafb_add_rpc_client(const char *uri)
{
	return afb_api_rpc_add_client_strong(
			uri,
			zafb_apiset(),
			zafb_apiset());
}

int zafb_add_rpc_server(const char *uri)
{
	return afb_api_rpc_add_server(
			uri,
			zafb_apiset(),
			zafb_apiset());
}

int zafb_start(void (*start)(), int maxjobs, int maxsessions, int maxthreads)
{
	afb_session_init(maxsessions, 0);

	/* start AFB scheduler */
	return afb_sched_start(
		1 /* started count of threads */,
		maxthreads /* maximum count of threads */,
		maxjobs /* maximum count of jobs */,
		__start__ /* start callback */,
		start /* argument to the callback */);
}

void zafb_exit(int code)
{
	afb_sched_exit(1, NULL, 0, code);
}

#if CONFIG_AFB_ZEPHYR_MBEDTLS
#include <tls/tls.h>
int zafb_tls_add_trust_list(const void *data, size_t size)
{
	return tls_add_trust(data, size);
}
int zafb_tls_set_certificate(const void *data, size_t size)
{
	return tls_set_cert(data, size);
}
int zafb_tls_set_private_key(const void *data, size_t size)
{
	return tls_set_key(data, size);
}
#endif

#if WITH_ZEPHYR_LLEXT
#include <zephyr/llext/llext.h>
#include <zephyr/llext/buf_loader.h>
#include <afb-apis.h>
#if CONFIG_USERSPACE
#include <zephyr/app_memory/mem_domain.h>
#endif /* CONFIG_USERSPACE */
#if CONFIG_USERSPACE
int zafb_load_llext_binding(const char *name, struct llext **ext, const void *buf,
                            size_t len, k_mem_domain domain)
#else /* CONFIG_USERSPACE */
int zafb_load_llext_binding(const char *name, struct llext **ext, const void *buf,
                            size_t len)
#endif /* CONFIG_USERSPACE */
{
	int rc;
	struct llext_buf_loader buf_loader = LLEXT_BUF_LOADER(buf, len);
	struct llext_loader *loader = &buf_loader.loader;
	struct llext_load_param ldr_parm = LLEXT_LOAD_PARAM_DEFAULT;
	struct afb_apiset *set = zafb_apiset();

	rc = llext_load(loader, name, ext, &ldr_parm);
#if CONFIG_USERSPACE
	if (rc >= 0)
		rc = llext_add_domain(*ext, &domain);
	if (rc >= 0)
		k_mem_domain_add_thread(&domain, k_current_get());
#endif /* CONFIG_USERSPACE */
	if (rc >= 0)
		rc = afb_api_so_v4_add(name, *ext, set, set);
	return rc;
}
#endif /* WITH_ZEPHYR_LLEXT */
