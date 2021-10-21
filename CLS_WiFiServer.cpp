#include <ESP8266WiFi.h>
#include "CLS_WiFiServer.h"
#include "CLS_WebServer.h"

#define DEBUG_WIFI_SERVEUR

WiFiServer server(80);
extern CLS_WebServer *pWebServer;

CLS_WiFiServer::CLS_WiFiServer()
{
    connect();
    pWebServer = new CLS_WebServer();
#ifdef DEBUG_WIFI_SERVEUR
    Serial.println("CLS_ServeurWifi::CLS_ServeurWifi  ---- Instance created");
#endif
}

void CLS_WiFiServer::connect()
{
    // on demande la connexion au WiFi
#ifdef DEBUG_WIFI_SERVEUR
    Serial.println("CLS_ServeurWifi::connect  ---- begin ");
#endif
    WiFi.mode(WIFI_STA);
    IPAddress ip(192, 168, 1, SHUTTER_SUBNET);
    IPAddress dns(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, dns, gateway, subnet);
    WiFi.hostname(SHUTTER_HOSTNAME);
#ifdef DEBUG_WIFI_SERVEUR
    Serial.print("CLS_ServeurWifi::connect  ---- WiFi.begin ");
#endif
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // on attend d'etre connecte au WiFi avant de continuer
#ifdef DEBUG_WIFI_SERVEUR
    Serial.print("CLS_ServeurWifi::connect  ---- Waiting connection ");
#endif
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
#ifdef DEBUG_WIFI_SERVEUR
        Serial.print(".");
#endif
    }

    server.begin();

    //WIFI.begin();
#ifdef DEBUG_WIFI_SERVEUR
    Serial.println("CLS_ServeurWifi::connect  ---- WiFi Started");
    traceWifiServerInfos();
#endif
}

void CLS_WiFiServer::traceWifiServerInfos()
{
#ifdef DEBUG_WIFI_SERVEUR
    Serial.print("WiFisever mode = ");
    Serial.println(WiFi.getMode());

    Serial.print("WiFisever SSID = ");
    Serial.println(WiFi.SSID());

    Serial.print("WiFisever status = ");
    Serial.println(WiFi.status());

    Serial.print("WiFisever hostname = ");
    Serial.println(WiFi.hostname());

    Serial.print("WiFisever IP = ");
    Serial.println(WiFi.localIP());

    Serial.print("WiFisever macAddress = ");
    Serial.println(WiFi.macAddress());
#endif
}

void CLS_WiFiServer::traceWifiStatuses()
{
#ifdef DEBUG_WIFI_SERVEUR
    unsigned long span = millis() - lastTraceMillis;
    if (span >= 5000)
    {
        Serial.println("no connection PassCount:" + String(wifiWithoutConnectionCount));
        Serial.println("with client PassCount:" + String(wifiWithClientPassCount));
        Serial.println("without PassCount:" + String(wifiWithoutClientPassCount));

        traceWifiServerInfos();

        lastTraceMillis = millis();
    }
#endif
}

void CLS_WiFiServer::handleClients()
{ /* code */
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi Connexion problem");
        wifiWithoutConnectionCount++;
        traceWifiStatuses();
        connect();
    }
    else
    {
        WiFiClient client = server.available();
        if (client)
        {
            Serial.println("Wifi client available");
            wifiWithClientPassCount++;
            pWebServer->handleClient(client);
        }
        else
            wifiWithoutClientPassCount++;
    }
    //traceWifiStatuses();
}
