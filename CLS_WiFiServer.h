
#ifndef _CLS_SERVER_WIFI_H
#define _CLS_SERVER_WIFI_H

#define WIFI_SSID "Pierrot"
#define WIFI_PASSWORD "iditem6ingibleolosag"

#define DEV_ENV

#ifdef DEV_ENV
#define SHUTTER_HOSTNAME "LvRoShDev"
#define SHUTTER_SUBNET 90
#else
#define SHUTTER_HOSTNAME "LvRoSh"
#define SHUTTER_SUBNET 80
#endif

class CLS_WiFiServer
{
public:
    CLS_WiFiServer();
    void handleClients();

protected:
    unsigned long lastTraceMillis = 0;
    unsigned long wifiWithoutClientPassCount = 0L;
    unsigned long wifiWithClientPassCount = 0L;
    unsigned long wifiWithoutConnectionCount = 0L;

    void connect();
    void traceWifiServerInfos();
    void traceWifiStatuses();
};

#endif
