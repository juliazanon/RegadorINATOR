/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME   "Matak"
#define IO_KEY        "aio_GYNc111fRwZKSScogOFOpkr46hYx"

/******************************* WIFI **************************************/

#define WIFI_SSID   "VIRUS2G"
#define WIFI_PASS   "DanYa2018"

#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/****************************************************************************/
