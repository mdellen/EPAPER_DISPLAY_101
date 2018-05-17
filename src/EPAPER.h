
void epaperSetup();
void epaperUpdate();

#include <GxEPD.h>

#include <GxGDEW042T2/GxGDEW042T2.h> // 4.2" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <fontconvert/SF-Pro-Display-Thin.h>
#include <fontconvert/SF-Pro-Display-Regular.h>
#include <fontconvert/fontello.h>
//#include <fontconvert/Climacons.h>
#include <fontconvert/epaper.h>

extern GxEPD_Class display;
