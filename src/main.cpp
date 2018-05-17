// To enable Deep-sleep, we need to tie the RST pin to GPIO 16 on the ESP8266. On the NodeMCU, GPIO 16 is represented as D0.

#include <Arduino.h>

#include <EPAPER.h>

#include <WIFI.h>
#include <NTP.h>
#include <MQTT.h>

Ticker epaperTimer;
bool update = true;

void doUpdate()
{
  update = true;
}

const int sleepTimeS = 10;

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting...");
  Serial.print("Reset Reason:  ");
  Serial.println(ESP.getResetReason());

  epaperSetup();
  // partial update to full screen to preset for partial update of box window
  // (this avoids strange background effects)
  delay(2000);
  display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);

  display.setCursor(30, 185);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&epaper32pt7b);
  display.print("l "); //settings icon
  //display.updateWindow(0,180,400,50,false);
  display.updateWindow(0, 120, 400, 80, false);
  //delay(1000);

  wifiSetup();         // setup WIFI
  display.print("* "); //web icon
  display.updateWindow(0, 120, 400, 80, false);
  //delay(1000);

  ntpSetup();          // setup NTP
  display.print("a "); //time icon
  display.updateWindow(0, 120, 400, 80, false);
  //delay(1000);

  mqttSetup();        // setup MQTT client
  display.print("o"); //mosquitto icon
  display.updateWindow(0, 120, 400, 80, false);

  epaperTimer.attach(10, doUpdate);
}

void loop()
{
  //
  //static int last = 0;

  ArduinoOTA.handle();

  if (update)
  {
    update = false;
    ntpSync();
    mqttKeepAlive();
    epaperUpdate();


    static int i = 0;

    /*//last = millis();
    Serial.print(i);
    Serial.print(" ");
    Serial.print(NTP.getTimeDateString());
    Serial.print(" ");
    Serial.print(NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ");
    Serial.print("WiFi is ");
    Serial.print(WiFi.isConnected() ? "connected" : "not connected");
    Serial.print(". ");
    Serial.print("Uptime: ");
    Serial.print(NTP.getUptimeString());
    Serial.print(" since ");
    Serial.println(NTP.getTimeDateString(NTP.getFirstSync()).c_str());

    i++; */
   }


}
