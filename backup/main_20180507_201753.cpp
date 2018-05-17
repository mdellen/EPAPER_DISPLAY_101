#include <Arduino.h>

#include <time.h>
#include <ESP8266WiFi.h>
#include <NtpClientLib.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <WiFiUdp.h>
#include "ArduinoOTA.h" //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA

#include <GxEPD.h>
#include <GxGDEW042T2/GxGDEW042T2.cpp> // 4.2" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
//#include <Fonts/FreeMonoBold9pt7b.h> // FreeFonts from Adafruit_GFX
//#include GxEPD_BitmapExamples

#include <fontconvert/SF-Pro-Display-Thin.h>
//#include <fontconvert/SF-Pro-Display-Regular.h>
#include <fontconvert/fontello.h>
#include <fontconvert/DIEDIEDI.h>


#if defined(ESP8266)
GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io /*RST=D4*/ /*BUSY=D2*/);              // default selection of D4(=2), D2(=4)
#endif

int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
bool wifiFirstConnected = true;

void processSyncEvent(NTPSyncEvent_t ntpEvent)
{
  if (ntpEvent)
  {
    Serial.print("Time Sync error: ");
    if (ntpEvent == noResponse)
      Serial.println("NTP server not reachable");
    else if (ntpEvent == invalidAddress)
      Serial.println("Invalid NTP server address");
  }
  else
  {
    Serial.print("Got NTP time: ");
    Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
  }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent;            // Last triggered event

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFiManager wifiManager;

  wifiManager.autoConnect("epaper", "1983");
  Serial.println("connected...yeey :)");

  NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
    ntpEvent = event;
    syncEventTriggered = true;
  });
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("epaper");
  ArduinoOTA.setPassword("1983");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready OTA8");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //display.init(115200); // enable diagnostic output on Serial
  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setFont(&fontello79pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(100, 200);
  display.print("W");
  display.update();
  //delay(2000);
  //display.fillScreen(GxEPD_WHITE);
  //display.setCursor(20, 40);
  //display.drawBitmap(APPLE, 0,0,400,300,false);
  //display.setCursor(100, 200);
  //display.print("H");
  //display.update();
  delay(2000);

}

void loop()
{
  static int i = 0;
  static int last = 0;

  ArduinoOTA.handle();

  if (wifiFirstConnected)
  {
    wifiFirstConnected = false;
    NTP.begin("pool.ntp.org", timeZone, true, minutesTimeZone);
    NTP.setInterval(63);
  }

  if (syncEventTriggered)
  {
    processSyncEvent(ntpEvent);
    syncEventTriggered = false;
  }

  if ((millis() - last) > 10000)
  {
    display.fillScreen(GxEPD_WHITE);
    display.fillRoundRect(0, 0, 400, 290, 30, GxEPD_BLACK);
    display.fillRoundRect(3, 3, 394, 284, 27, GxEPD_WHITE);
    display.setFont();
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(30, 293);
    display.print(WiFi.localIP());
    display.print("  ");
    display.print(WiFi.isConnected() ? "connected" : "not connected");

    //display.setFont(&SF_Pro_Display_Thin79pt7b);
    display.setFont(&DIEDIEDI72pt7b);

    int16_t x1, y1;
    uint16_t w1, h1;
    int str_len = NTP.getTimeStr().substring(0, 5).length() + 1;
    char char_array[str_len];
    NTP.getTimeStr().substring(0, 5).toCharArray(char_array, str_len);
    display.getTextBounds(char_array, 0, 0, &x1, &y1, &w1, &h1);

    display.setCursor(200 - (w1 / 2) - 8, 200);

    //display.setCursor(200, 200);
    //display.print("");

    display.print(NTP.getTimeStr().substring(0, 5));
    display.update();

    last = millis();
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

    i++;
  }
  /*
   display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false); // (this avoids strange background effects)
   display.setCursor(200, 293);
   display.setFont();
   display.setTextColor(GxEPD_BLACK);
   display.print(NTP.getTimeStr());
   display.updateWindow(0,290,400,10,false); 
   delay(1000);*/
  // display.updateWindow(0, 100, 400, 100, true);

  //display.setFont(&SF_Pro_Display_Regular17pt7b);
  //display.setCursor(0, 240);
  //display.println("The quick brown fox jumps over the lazy dog...");
  // Serial.println("24:13");

  //delay(DEMO_DELAY * 1000);
}


