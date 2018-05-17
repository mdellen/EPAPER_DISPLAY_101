#include <Ticker.h>
#include <AsyncMqttClient.h>
#define MQTT_HOST IPAddress(77,174,82,246) // www.mennovandellen.nl
#define MQTT_PORT 1883
#define MQTT_USER "mdel"
#define MQTT_PW "Yamaha01v"

#include <WiFiManager.h>
extern WiFiManager wifiManager;
extern AsyncMqttClient mqttClient;
extern Ticker mqttReconnectTimer;
//extern char* mqttMessage;
extern char* mqttTemp[];
extern String weatherIcon;
extern String weatherSummary;
extern String icyTemperature;

void mqttSetup();
void mqttKeepAlive();
void connectToMqtt();
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);

void connectToWifi();
void onWifiConnect(const WiFiEventStationModeGotIP& event);
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
