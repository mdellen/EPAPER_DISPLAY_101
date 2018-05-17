#include <MQTT.h>
AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

String mqttMessage    = "No MQTT message...";
String weatherIcon    = "K";
String weatherSummary = "";
String icyTemperature = "-";

void mqttSetup()
{
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCredentials(MQTT_USER, MQTT_PW);
    connectToMqtt();
}

void connectToMqtt()
{
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

void onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    uint16_t packetIdSub1 = mqttClient.subscribe("forecast/icon", 2);
    uint16_t packetIdSub2 = mqttClient.subscribe("forecast/now", 2);
    mqttClient.subscribe("icy/temp", 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.println(packetIdSub1);
    Serial.println(packetIdSub2);
    mqttClient.publish("test/lol", 0, true, "test 1");
    Serial.println("Publishing at QoS 0");
    uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
    Serial.print("Publishing at QoS 1, packetId: ");
    Serial.println(packetIdPub1);
    uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
    Serial.print("Publishing at QoS 2, packetId: ");
    Serial.println(packetIdPub2);
}

void mqttKeepAlive(){
  if (!mqttClient.connected())  {
      Serial.print("Disconnected from MQTT.");
    if
        (WiFi.isConnected())
        {
            mqttReconnectTimer.once(2, connectToMqtt);
        }
  }   

}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.print("Disconnected from MQTT.");
    if
        (WiFi.isConnected())
        {
            mqttReconnectTimer.once(2, connectToMqtt);
        }
}


void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    Serial.println("Publish received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  payload: ");
    Serial.println(payload);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);
   // char* mqttMessage[sizeof(payload)+1] = payload + '\0';
    String mqttTopic = String(topic);


    char mqttMessage[sizeof(payload)+1]; // size for temporary array with an extra spot for a NULL (if needed)
    strlcpy(mqttMessage, payload, len+1); // copy the array, up to len limit max + 1
     mqttMessage[len] = '\0'; // null terminate final char of the new array, strlcpy does this, but you should get a good habbit of doing it as well, especially if your new at char arrays
    
//    char mqttTopic[sizeof(topic)+1]; // size for temporary array with an extra spot for a NULL (if needed)
//    strlcpy(mqttTopic, topic, len+1); // copy the array, up to len limit max + 1
//    mqttTopic[len] = '\0'; // null terminate final char of the new array, strlcpy does this, but you should get a good habbit of doing it as well, especially if your new at char arrays
    
    Serial.print("Topic: ");
    Serial.println(mqttTopic);
     Serial.print("Message: ");
    Serial.println(mqttMessage);

    if (mqttTopic == "forecast/icon")   weatherIcon      = mqttMessage;
    if (mqttTopic == "forecast/now")    weatherSummary   = mqttMessage;
    if (mqttTopic == "icy/temp")        icyTemperature   = mqttMessage;
}

void onMqttPublish(uint16_t packetId)
{
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}