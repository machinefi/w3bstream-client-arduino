#ifndef __WSIOTSDK_H__
#define __WSIOTSDK_H__

#include "psa/crypto.h"
#include "application/devnet/iotex_dev_access.h"

namespace iotex
{
class Devnet
{
  private:


  public:

    void begin(void);
    void begin(iotex_gettime time_f, iotex_mqtt_pub pub_f, iotex_mqtt_sub sub_f);
    void setTimeFunction(iotex_gettime time_f);
    void setMQTTFunctions(iotex_mqtt_pub pub_f, iotex_mqtt_sub sub_f);
    void setDevnetToken(const char token[], int len);
    void setDevnetMQTTTopic(const char topic[], int len, int location = 0);
    void send(void *buf, unsigned int len, enum UserData_Type type);

    char* getMQTTConnectAddr(void);
    int getMQTTConnectPort(void);    

};
} // namespace iotex

extern iotex::Devnet devnet_client;

#endif
