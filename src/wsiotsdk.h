#ifndef __WSIOTSDK_H__
#define __WSIOTSDK_H__

#include "psa/crypto.h"
#include "application/devnet/iotex_dev_access.h"

#ifdef __cplusplus
extern "C" {
#endif

int iotex_wsiotsdk_init(iotex_gettime get_time_func, iotex_mqtt_pub mqtt_pub, iotex_mqtt_sub mqtt_sub);

#ifdef __cplusplus
}
#endif

#endif