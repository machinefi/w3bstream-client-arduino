#ifndef __WSIOTSDK_H__
#define __WSIOTSDK_H__

#include "include/svc/crypto.h"
#include "include/entropy.h"
#include "include/application/devnet/iotex_dev_access.h"

namespace iotex
{
class Devnet
{
  private:

  public:

    void begin(void);
    uint8_t * begin(iotex_gettime time_f, iotex_mqtt_pub pub_f, iotex_mqtt_sub sub_f);
    void setTimeFunction(iotex_gettime time_f);
    void setMQTTFunctions(iotex_mqtt_pub pub_f, iotex_mqtt_sub sub_f);
    void setDevnetToken(const char token[], int len);
    void setDevnetMQTTTopic(const char topic[], int len, int location = 0);
    void send(void *buf, unsigned int len, enum UserData_Type type);

    char* getMQTTConnectAddr(void);
    int getMQTTConnectPort(void);    

};
} // namespace iotex

#ifdef __cplusplus
extern "C" {
#endif
uint8_t * iotex_wsiotsdk_init(iotex_gettime get_time_func, iotex_mqtt_pub mqtt_pub, iotex_mqtt_sub mqtt_sub);
#ifdef __cplusplus
}
#endif

extern iotex::Devnet devnet_client;

#endif
