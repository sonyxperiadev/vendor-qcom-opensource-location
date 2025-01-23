#include <string>
#include <vector>
#include <dlfcn.h>

#include "loc_util_log.h"
#include "common_v01.h"
#include "qmi_client.h"

#include "libqmi_loader.h"

qmi_client_get_any_service_t qmi_client_get_any_service;
qmi_client_get_service_instance_t qmi_client_get_service_instance;
qmi_client_init_t qmi_client_init;
qmi_client_message_decode_t qmi_client_message_decode;
qmi_client_notifier_init_t qmi_client_notifier_init;
qmi_client_register_error_cb_t qmi_client_register_error_cb;
qmi_client_release_t qmi_client_release;
qmi_client_send_msg_sync_t qmi_client_send_msg_sync;

qmi_idl_type_table_object common_qmi_idl_type_table_object_v01;

static void *load_library(const char *lib_name) {
    void *handle = dlopen(lib_name, RTLD_NOW);
    if (!handle) {
        LOC_LOGE("%s: ERROR: Failed to load library %s: %s\n", __func__,
                lib_name, dlerror());
    }
    return handle;
}

static int load_symbols(void *handle,
            const std::vector<std::pair<std::string, void **>> &symbols) {
    for (const auto &symbol : symbols) {
        *symbol.second = dlsym(handle, symbol.first.c_str());
        if (!*symbol.second) {
            LOC_LOGE("%s: ERROR: Failed to load symbol %s: %s\n", __func__,
                    symbol.first.c_str(), dlerror());
            dlclose(handle);
            return -1;
        }
    }

    return 0;
}

static int load_libqmi_cci_symbols() {
    void *handle = load_library("libqmi_cci.so");
    if (!handle) {
        return -1;
    }

    std::vector<std::pair<std::string, void **>> symbols = {
        {"qmi_client_get_any_service", reinterpret_cast<void **>(&qmi_client_get_any_service)},
        {"qmi_client_get_service_instance", reinterpret_cast<void **>(&qmi_client_get_service_instance)},
        {"qmi_client_init", reinterpret_cast<void **>(&qmi_client_init)},
        {"qmi_client_message_decode", reinterpret_cast<void **>(&qmi_client_message_decode)},
        {"qmi_client_notifier_init", reinterpret_cast<void **>(&qmi_client_notifier_init)},
        {"qmi_client_register_error_cb", reinterpret_cast<void **>(&qmi_client_register_error_cb)},
        {"qmi_client_release", reinterpret_cast<void **>(&qmi_client_release)},
        {"qmi_client_send_msg_sync", reinterpret_cast<void **>(&qmi_client_send_msg_sync)},
    };

    return load_symbols(handle, symbols);
}

static int load_libqmi_common_so_symbols() {
    void *handle = load_library("libqmi_common_so.so");
    if (!handle) {
        return -1;
    }

    common_qmi_idl_type_table_object_v01 = *reinterpret_cast<qmi_idl_type_table_object*>(
        dlsym(handle, "common_qmi_idl_type_table_object_v01"));

    return 0;
}

int load_qmi_symbols() {
    if (load_libqmi_cci_symbols()) {
        LOC_LOGE("%s: ERROR: One or more symbols failed to load from libqmi_cci.so\n", __func__);
        return -1;
    }

    if (load_libqmi_common_so_symbols()) {
        LOC_LOGE("%s: ERROR: One or more symbols failed to load from libqmi_common_so.so\n", __func__);
        return -1;
    }

    return 0;
}
