
#ifndef _CLS_WEBSERVER_H
#define _CLS_WEBSERVER_H

#include <ESP8266WiFi.h>

enum HTTP_VERB
{
    UNKNOWN,
    GET,
    POST
};

class CLS_WebServer
{
public:
    CLS_WebServer();
    void handleClient(WiFiClient client);
    void handleHomePageRequest(WiFiClient client);
    void handleAht(WiFiClient client);
    void handleSetHomeShuttle(WiFiClient client);
    void handleOpenShuttle(WiFiClient client);
    void handleCloseShuttle(WiFiClient client);
    void handleStopShuttle(WiFiClient client);
    void handleSetRatioShuttlePos(WiFiClient client, int ratioPosition);
};

#endif