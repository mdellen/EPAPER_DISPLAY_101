#include <time.h>
#include <NtpClientLib.h>

extern int8_t timeZone;
extern int8_t minutesTimeZone;
extern bool wifiFirstConnected;
extern boolean syncEventTriggered; // True if a time even has been triggered
extern NTPSyncEvent_t ntpEvent;            // Last triggered event

void ntpSetup();
void ntpSync();
void processSyncEvent(NTPSyncEvent_t ntpEvent);
