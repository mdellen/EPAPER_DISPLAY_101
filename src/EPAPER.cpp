#include <EPAPER.h>
#include <WIFI.h>
#include <NTP.h>
#include <MQTT.h>

#include <GxGDEW042T2/GxGDEW042T2.cpp> // 4.2" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

#if defined(ESP8266)
GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class 
GxEPD_Class display(io /*RST=D4*/ /*BUSY=D2*/);              // default selection of D4(=2), D2(=4)
#endif





void epaperSetup(){
      display.init();

    if (ESP.getResetReason() != "Deep-Sleep Wake")
    {
      display.fillScreen(GxEPD_WHITE);
      display.update();
    }
      
      
     //display.init(115200); // enable diagnostic output on Serial
}

void epaperUpdate(){

    Serial.print("Update diplay...");    
    
    display.fillScreen(GxEPD_WHITE);

      //if ((millis() - last) > 10000)
 // {
    display.fillScreen(GxEPD_WHITE);
    display.setTextSize(1);
    //display.fillRoundRect(0, 0, 400, 290, 30, GxEPD_BLACK);
    //display.fillRoundRect(3, 3, 394, 284, 27, GxEPD_WHITE);
    display.setFont();
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(0, 293);
    display.print(WiFi.localIP());
    display.print("  ");
    display.print(WiFi.isConnected() ? "connected" : "not connected");
    //display.print(" ");
    //display.print(mqttMessage);

    display.setCursor(20,195);
    display.setFont(&epaper32pt7b);
    display.print(weatherIcon);

    if (weatherIcon == "reset") wifiManager.resetSettings();

    //display.setTextSize(2);
    //display.setCursor(90,180);
   // display.setFont(&SF_Pro_Display_Regular17pt7b);
    //display.setFont();
    //display.println(weatherSummary);
    //display.setTextSize(1);


    display.setFont(&epaper32pt7b);
    display.setCursor(10,280);
    display.print("b"); // degrees celsius
    display.setCursor(55,260);
    display.setFont(&SF_Pro_Display_Regular17pt7b);
    display.setFont();
    display.setTextSize(2);
    display.print(icyTemperature);
    display.setCursor(display.getCursorX(), 248);
    display.print((char)248);
    display.setCursor(display.getCursorX(), 254);
    display.println("C"  );
    display.setTextSize(1);


 
  //Serial.println(dec(mqttMessage));
   // Serial.println(hex(mqttMessage));
   // display.print((mqttTemp));


    display.setFont(&SF_Pro_Display_Thin79pt7b);
    //display.setFont(&DIEDIEDI72pt7b);

    int16_t x1, y1;
    uint16_t w1, h1;
    int str_len = NTP.getTimeStr().substring(0, 5).length() + 1;
    char char_array[str_len];
    NTP.getTimeStr().substring(0, 5).toCharArray(char_array, str_len);
    display.getTextBounds(char_array, 0, 0, &x1, &y1, &w1, &h1);

    //display.setCursor(200 - (w1 / 2) - 8, 200);
    display.setCursor(200 - (w1 / 2) - 8, 120);
    //display.setCursor(200, 200);
    //display.print("");

    display.print(NTP.getTimeStr().substring(0, 5)); 
    /*display.setCursor(200 - (w1 / 2) - 8, 120);
    display.print(NTP.getTimeStr().substring(0, 2)); 
    display.setCursor(display.getCursorX(), 110);
    display.print(NTP.getTimeStr().substring(3, 1)); 
    display.setCursor(display.getCursorX(), 120);
    display.print(NTP.getTimeStr().substring(4, 2)); 

*/
        mqttClient.publish("epaper/clock", 0, true, char_array);
    mqttClient.publish("test/lol", 0, true, "test 1");
    display.update();

    if (weatherIcon == "sleep") 
    {
        Serial.println("ESP8266 in sleep mode (10 seconds)");
        delay(10000);
        ESP.deepSleep(10 * 1000000);
    }
//display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
//display.updateWindow(0,0,GxEPD_WIDTH,125,false);
}