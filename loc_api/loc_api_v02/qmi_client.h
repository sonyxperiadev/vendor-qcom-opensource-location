#ifndef QMI_CLIENT_H
#define QMI_CLIENT_H

#include "qmi_idl_lib.h"
#include "qmi_cci_target_ext.h"

#define QMI_NO_ERR         0
#define QMI_SERVICE_ERR  (-2)

typedef void *qmi_client_type;
typedef int qmi_client_error_type;

typedef struct {
    unsigned int info[5];
} qmi_service_info;

typedef void (*qmi_client_ind_cb)
(
    qmi_client_type user_handle,
    unsigned int msg_id,
    void *ind_buf,
    unsigned int ind_buf_len,
    void *ind_cb_data
);

typedef void (*qmi_client_error_cb)
(
    qmi_client_type user_handle,
    qmi_client_error_type error,
    void *err_cb_data
);

typedef qmi_client_error_type (*qmi_client_get_any_service_t)
(
    qmi_idl_service_object_type service_obj,
    qmi_service_info *service_info
);

typedef qmi_client_error_type (*qmi_client_get_service_instance_t)
(
    qmi_idl_service_object_type service_obj,
    unsigned int instance_id,
    qmi_service_info *service_info
);

typedef qmi_client_error_type (*qmi_client_init_t)
(
    qmi_service_info *service_info,
    qmi_idl_service_object_type service_obj,
    qmi_client_ind_cb ind_cb,
    void *ind_cb_data,
    qmi_client_os_params *os_params,
    qmi_client_type *user_handle
);

typedef qmi_client_error_type (*qmi_client_message_decode_t)
(
    qmi_client_type user_handle,
    qmi_idl_type_of_message_type req_resp_ind,
    unsigned int message_id,
    const void *p_src,
    unsigned int src_len,
    void *p_dst,
    unsigned int dst_len
);

typedef qmi_client_error_type (*qmi_client_notifier_init_t)
(
    qmi_idl_service_object_type service_obj,
    qmi_client_os_params *os_params,
    qmi_client_type *user_handle
);

typedef qmi_client_error_type (*qmi_client_register_error_cb_t)
(
    qmi_client_type user_handle,
    qmi_client_error_cb err_cb,
    void *err_cb_data
);

typedef qmi_client_error_type (*qmi_client_release_t)
(
    qmi_client_type user_handle
);

typedef qmi_client_error_type (*qmi_client_send_msg_sync_t)
(
    qmi_client_type user_handle,
    unsigned int msg_id,
    void *req_c_struct,
    unsigned int req_c_struct_len,
    void *resp_c_struct,
    unsigned int resp_c_struct_len,
    unsigned int timeout_msecs
);

extern qmi_client_get_any_service_t qmi_client_get_any_service;
extern qmi_client_get_service_instance_t qmi_client_get_service_instance;
extern qmi_client_init_t qmi_client_init;
extern qmi_client_message_decode_t qmi_client_message_decode;
extern qmi_client_notifier_init_t qmi_client_notifier_init;
extern qmi_client_register_error_cb_t qmi_client_register_error_cb;
extern qmi_client_release_t qmi_client_release;
extern qmi_client_send_msg_sync_t qmi_client_send_msg_sync;

#endif /* QMI_CLIENT_H */
