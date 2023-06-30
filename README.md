# Web3 Component for Arduino



## Developing in VSCode

Open the folder in VSCode. You whould see a pop up on the bottom left asking to install the recommended extensions. Install them.  
If the popup doesn't appear, use the following guide to [install recommended extensions](https://code.visualstudio.com/docs/editor/extension-marketplace#_recommended-extensions).  

### Building with VSCode CMake extension

1. Open the Command Palette in VSCode by pressing Ctrl+Shift+P.  
2. Search for "CMake: Build" and select the "CMake: Build" command.  

### Running/debugging the tests

1. Ensure you have selected the Debug CMake variant when building the project. Use Ctrl+Shift+P and "CMake: Select variant" to do so.  
2. Build the project using CMake.  
3. On the left side panel, click on the Testing extension icon. It should open a list of tests.  
4. Beside each of the tests, you can see an icon to run the test or debug it.  

## Configure Your Project

### Dependency:

When you connect your device to web3 devnet, you need the appropriate mqtt library, such as the `PubSubClient`  library.



## Get Devnet Configuration Parameters

##### You need to create the project and device from the w3bstream studio page and get the token and mqtt topic, for more detailed instructions, please see the following documentation:

[How to create a w3bstream studio project](./doc/How_to_create_a_w3bstream_studio_project.md)



## Use IOTEX WSIoTSDK

##### 1. Add header file.

```c++
#include "wsiotsdk.h"
```

##### 2. Add SDK initialization code.

```c
time_t iotex_time_set_func(void)
{
    return time(nullptr);
}

int iotex_mqtt_pubscription(unsigned char *topic, unsigned char *buf, unsigned int buflen, int qos)
{
	return client.publish((const char *)topic, (const uint8_t *)buf, buflen, false);
}

int iotex_mqtt_subscription(unsigned char *topic)
{
    return client.subscribe((const char *)topic);
}

void setup() {

    .......
    iotex_wsiotsdk_init(iotex_time_set_func, iotex_mqtt_pubscription, iotex_mqtt_subscription);
    .......
}

```

##### 3. project configuration

Fill the **token** and **mqtt topic** obtained through **w3bstream studio** into the **iotex_dev_access_config.h** file:

```c
#define IOTEX_TOKEN_DEFAULT		        "eyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define IOTEX_MQTT_TOPIC_DEFAULT		"eth_xxxxxxxxxxxxxxxxxxx_esp32_hello"
```

##### 4. The project examples provide several examples for uploading user data to w3bstream:

```c
void iotex_devnet_upload_data_example_json(void);
void iotex_devnet_upload_data_example_pb(void);
void iotex_devnet_upload_data_example_raw(void);
```

##### 5. The user does not need to care about the details of the device's data interaction with w3bstream.

##### 6. All you need to do is pass the data you want to report into the SDK as shown in the example in step 5.



## IOTEX WSIoTSDK Reference

##### For more information about Iotex WSIoTSDK, please refer to the following document:

[WSIoTSDK User Manual](./doc/WSIoTSDK_User_Manual.md)
