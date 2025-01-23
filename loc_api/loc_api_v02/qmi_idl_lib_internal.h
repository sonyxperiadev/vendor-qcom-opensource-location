#ifndef QMI_IDL_LIB_INTERNAL_H
#define QMI_IDL_LIB_INTERNAL_H

#include <stdint.h>

#include "qmi_idl_lib.h"

#define QMI_IDL_TYPE16(table, type) (((table) << 12) | (type))
#define QMI_IDL_TYPE88(table, type) (type & 0xFF), (((type & 0xFF00) >> 4) | table)

#define QMI_IDL_FLAGS_OFFSET_IS_16 0x80
#define QMI_IDL_FLAGS_IS_ARRAY 0x40
#define QMI_IDL_FLAGS_SZ_IS_16 0x20
#define QMI_IDL_FLAGS_IS_VARIABLE_LEN 0x10

#define QMI_IDL_OFFSET8(t,f)          ((uint8_t) offsetof(t,f))
#define QMI_IDL_OFFSET16RELATIVE(t,f) offsetof(t,f)
#define QMI_IDL_OFFSET16ARRAY(t,f)    ((uint8_t) offsetof(t,f)), \
                                      ((uint8_t) (offsetof(t,f) >> 8))


#define QMI_IDL_FLAG_END_VALUE 0x20

#define QMI_IDL_TLV_FLAGS_LAST_TLV              0x80
#define QMI_IDL_TLV_FLAGS_OPTIONAL              0x40

#define QMI_IDL_GENERIC_1_BYTE     0
#define QMI_IDL_GENERIC_2_BYTE     1
#define QMI_IDL_GENERIC_4_BYTE     2
#define QMI_IDL_GENERIC_8_BYTE     3
#define QMI_IDL_STRING             6
#define QMI_IDL_AGGREGATE          7

typedef struct {
    uint32_t c_struct_sz;
    const uint8_t *p_encoded_type_data;
} qmi_idl_type_table_entry;

typedef struct {
    uint32_t c_struct_sz;
    const uint8_t *p_encoded_tlv_data;
} qmi_idl_message_table_entry;

typedef struct {
    int8_t (*range_check)(void *val);
} qmi_idl_range_table_entry;

typedef struct {
    uint16_t qmi_message_id;
    uint16_t message_table_message_id;
    uint16_t max_msg_len;
} qmi_idl_service_message_table_entry;

struct qmi_idl_type_table_object {
    uint16_t n_types;
    uint16_t n_messages;
    uint8_t n_referenced_tables;
    const qmi_idl_type_table_entry *p_types;
    const qmi_idl_message_table_entry *p_messages;
    const struct qmi_idl_type_table_object **p_referenced_tables;
    const qmi_idl_range_table_entry *p_ranges;
};

struct qmi_idl_service_object {
    uint32_t library_version;
    uint32_t idl_version;
    uint32_t service_id;
    uint32_t max_msg_len;
    uint16_t n_msgs[QMI_IDL_NUM_MSG_TYPES];
    const qmi_idl_service_message_table_entry *msgid_to_msg[QMI_IDL_NUM_MSG_TYPES];
    const qmi_idl_type_table_object *p_type_table;
    uint32_t idl_minor_version;
    struct qmi_idl_service_object *parent_service_obj;
};

#endif /* QMI_IDL_LIB_INTERNAL_H */
