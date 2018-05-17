#include <NTP.h>

int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
bool wifiFirstConnected = true;
boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; 

void ntpSetup()
{
    NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
    ntpEvent = event;
    syncEventTriggered = true;
    }); 
}

void ntpSync(){
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

}

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
