#ifndef QMI_CCI_TARGET_EXT_H
#define QMI_CCI_TARGET_EXT_H

#include <stdint.h>
#include <pthread.h>

typedef struct {
    uint32_t sig_set;
    uint32_t timed_out;
    uint32_t clock;
    pthread_cond_t cond;
    pthread_condattr_t attr;
    pthread_mutex_t mutex;
} qmi_cci_os_signal_type;

typedef qmi_cci_os_signal_type qmi_client_os_params;

#define QMI_CCI_OS_SIGNAL qmi_cci_os_signal_type
#define QMI_CCI_OS_SIGNAL_CLEAR(ptr) (ptr)->sig_set = 0

typedef void (*qmi_cci_os_signal_wait_t)(
    QMI_CCI_OS_SIGNAL *ptr,
    unsigned int timeout_ms);

static qmi_cci_os_signal_wait_t qmi_cci_os_signal_wait;

#define QMI_CCI_OS_SIGNAL_WAIT qmi_cci_os_signal_wait

#endif /* QMI_CCI_TARGET_EXT_H */
