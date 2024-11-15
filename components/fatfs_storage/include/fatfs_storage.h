#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H

#include "sdkconfig.h"


#ifdef __cplusplus
extern "C" {
#endif

    void inti_fatfs_storage(void);
    void read_fatfs_storage(void);
    void write_fatfs_storage(void);

#ifdef __cplusplus
}
#endif

#endif
