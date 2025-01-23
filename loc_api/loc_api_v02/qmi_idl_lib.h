#ifndef QMI_IDL_LIB_H
#define QMI_IDL_LIB_H

typedef enum {
    QMI_IDL_REQUEST = 0,
    QMI_IDL_RESPONSE,
    QMI_IDL_INDICATION,
    QMI_IDL_NUM_MSG_TYPES
} qmi_idl_type_of_message_type;

typedef struct qmi_idl_service_object *qmi_idl_service_object_type;
typedef struct qmi_idl_type_table_object qmi_idl_type_table_object;

#endif /* QMI_IDL_LIB_H */
