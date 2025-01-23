#ifndef COMMON_V01_H
#define COMMON_V01_H

#include "qmi_idl_lib_internal.h"

typedef enum {
    QMI_RESULT_SUCCESS_V01 = 0,
    QMI_RESULT_FAILURE_V01 = 1,
} qmi_result_type_v01;

typedef enum {
    QMI_ERR_MALFORMED_MSG_V01 = 0x0001,
    QMI_ERR_DEVICE_IN_USE_V01 = 0x0017,
    QMI_ERR_INVALID_ARG_V01 = 0x0030,
    QMI_ERR_INVALID_MESSAGE_ID_V01 = 0x0039,
    QMI_ERR_SESSION_OWNERSHIP_V01 = 0x0043,
    QMI_ERR_NOT_SUPPORTED_V01 = 0x005E,
} qmi_error_type_v01;

typedef struct {
    qmi_result_type_v01 result;
    qmi_error_type_v01 error;
} qmi_response_type_v01;

typedef struct {
    qmi_response_type_v01 resp;
    uint8_t supported_msgs_valid;
    uint32_t supported_msgs_len;
    uint8_t supported_msgs[8192];
} qmi_get_supported_msgs_resp_v01;

extern qmi_idl_type_table_object common_qmi_idl_type_table_object_v01;

#endif /* COMMON_V01_H */
