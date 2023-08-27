#include <stdlib.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error Platform not supported
#endif

#include <WiFiClient.h>
#include <PubSubClient.h>           // https://github.com/knolleary/pubsubclient/releases/tag/v2.8
#include <wsiotsdk.h>

#include <time.h>
#define emptyString String()

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMillis = 0;
time_t now;
time_t nowish = 1510592825;

int8_t TIME_ZONE = -5;          //NYC(USA): -5 UTC

static const char ssid[] = "XXX";
static const char pass[] = "YYYY";

#define THINGNAME   "IoTeX_Demo"

#define IOTEX_DEBUG_SHOW_PUBLIC_KEY

int iotex_mqtt_pubscription(unsigned char *topic, unsigned char *buf, unsigned int buflen, int qos) {
    return client.publish((const char *)topic, (const uint8_t *)buf, buflen, false);
}

int iotex_mqtt_subscription(unsigned char *topic) {
    return client.subscribe((const char *)topic);
}

time_t iotex_time_set_func(void) {
    return time(nullptr);
}

void NTPConnect(void)
{
    Serial.print("Setting time using SNTP");

    configTime(TIME_ZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
    
    now = time(nullptr);
    while (now < nowish) {
      delay(500);
      Serial.print(".");
      now = time(nullptr);
    }
    Serial.println("done!");

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void messageReceived(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Received [");
    Serial.print(topic);
    Serial.print("]: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void pubSubErr(int8_t MQTTErr)
{
  if (MQTTErr == MQTT_CONNECTION_TIMEOUT)
    Serial.print("Connection tiemout");
  else if (MQTTErr == MQTT_CONNECTION_LOST)
    Serial.print("Connection lost");
  else if (MQTTErr == MQTT_CONNECT_FAILED)
    Serial.print("Connect failed");
  else if (MQTTErr == MQTT_DISCONNECTED)
    Serial.print("Disconnected");
  else if (MQTTErr == MQTT_CONNECTED)
    Serial.print("Connected");
  else if (MQTTErr == MQTT_CONNECT_BAD_PROTOCOL)
    Serial.print("Connect bad protocol");
  else if (MQTTErr == MQTT_CONNECT_BAD_CLIENT_ID)
    Serial.print("Connect bad Client-ID");
  else if (MQTTErr == MQTT_CONNECT_UNAVAILABLE)
    Serial.print("Connect unavailable");
  else if (MQTTErr == MQTT_CONNECT_BAD_CREDENTIALS)
    Serial.print("Connect bad credentials");
  else if (MQTTErr == MQTT_CONNECT_UNAUTHORIZED)
    Serial.print("Connect unauthorized");
}

void connectToMqtt(bool nonBlocking = false) {
  Serial.print("MQTT connecting ");

  while (!client.connected()) {
    if (client.connect(THINGNAME)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, reason -> ");
      pubSubErr(client.state());
      if (!nonBlocking) {
        Serial.println(" < try again in 5 seconds");
        delay(5000);
      } else {
        Serial.println(" <");
      }
    }
    
    if (nonBlocking)
      break;
  }
}

void connectToWiFi(String init_str) {
    
    if (init_str != emptyString)
      Serial.print(init_str);
    
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }

    if (init_str != emptyString)
      Serial.println("ok!");
}

void checkWiFiThenMQTT(void) {
    connectToWiFi("Checking WiFi");
    connectToMqtt();
}

typedef struct __packed user_data {
    int i;
    float f;
    bool b;
}user_data_t;

void iotex_devnet_upload_data_example_raw(void) {

	user_data_t user_data;
	unsigned int len = sizeof(user_data);

	user_data.i = 64;
	user_data.f = 128.128;
	user_data.b = true;

  devnet_client.send((void *)&user_data, len, IOTEX_USER_DATA_TYPE_RAW);
}

void setup()
{
    uint8_t *public_key = NULL;

    Serial.begin(115200);
    delay(1000);

#ifdef ESP32
    WiFi.setHostname(THINGNAME);
#else
    WiFi.hostname(THINGNAME);
#endif
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    connectToWiFi(String("Attempting to connect to SSID: ") + String(ssid));

    NTPConnect();

    client.setServer(devnet_client.getMQTTConnectAddr(), devnet_client.getMQTTConnectPort());
    client.setCallback(messageReceived);
    client.setBufferSize(384);

    connectToMqtt();

    public_key = devnet_client.begin(iotex_time_set_func, iotex_mqtt_pubscription, iotex_mqtt_subscription);

#ifdef IOTEX_DEBUG_SHOW_PUBLIC_KEY
    Serial.println("Public Key :");
    for (int i = 0; i < 64; i++) {
         Serial.printf("%02x ", public_key[i]); 
    }
    Serial.println();
#endif
    
}

void loop()
{
    now = time(nullptr);
  
    if (!client.connected()) {
        checkWiFiThenMQTT();

    } else {
        client.loop();
        if (millis() - lastMillis > 5000) {
            lastMillis = millis();
            Serial.println("Raw Data uploading");
            iotex_devnet_upload_data_example_raw();
        }
    }
}








