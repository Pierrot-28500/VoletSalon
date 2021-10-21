
#include "CLS_WebServer.h"
#include "CLS_Shutter.h"
#include "CLS_Aht10.h"

//#define DEBUG_HTTP_RESPONSE_HEADER
//#define DEBUG_HTTP_RESPONSE
//#define DEBUG_HTTP_REQUEST
//#define DEBUG_WEB_SERVER
//#define DEBUG_BAD_REQUEST

class CLS_HTTP_Header
{
public:
    String headerLine = "";
    String uri = "";
    String parameters = "";
    HTTP_VERB verb = UNKNOWN;

    CLS_HTTP_Header(String request)
    {
        if (request.length() > 0)
        {
            // On recherche la ligne utile contenant GET ou POST
            int iVerb = request.indexOf("GET");
            if (iVerb < 0)
            {
                iVerb = request.indexOf("POST");
                if (iVerb >= 0)
                    verb = POST;
            }
            else
                verb = GET;

            // Si GET ou POST est trouvé, on récupère cette ligne
            // toout le reste du header est ignoré
            if (verb != UNKNOWN)
            {
                request = request.substring(iVerb);
                int iEnd = request.indexOf('\n');
                if (iEnd > 0)
                    request = request.substring(0, iEnd);
                request.trim();
                headerLine = request;
                int iUri = headerLine.indexOf(' ');
                if (iUri > 0)
                {
                    uri = headerLine.substring(iUri);
                    uri.trim();
                    int iUriEnd = uri.indexOf(' ');
                    if (iUriEnd > 0)
                        uri = uri.substring(0, iUriEnd);
                    uri.trim();
                    int iParams = uri.indexOf('?');
                    if (iParams > 0)
                    {
                        parameters = uri.substring(iParams);
                        uri = uri.substring(0, iParams);
                        uri.trim();
                    }
                }
                else
                    uri = "/";
            }
            // else --> la requète http est ignorée
#ifdef DEBUG_HTTP_HEADER_REQUEST
            Serial.println("headerline : " + headerLine);
            Serial.println("verb : " + String(verb));
            Serial.println("uri : " + uri);
            Serial.println("parameters : " + parameters);
#endif
        }
#ifdef DEBUG_HTTP_HEADER_REQUEST
        else
            Serial.println("----------------------------------->Empty request");
#endif
    }

    /*
    bool isEmpty()
    {
        bool empty = headerLine.length() == 0;
        return empty;
    }
*/

    bool isValid()
    {
        bool valid = true;
        valid &= headerLine.length() > 0;
        valid &= verb != UNKNOWN;
        valid &= uri.length() > 0;
        return valid;
    }
};
class CLS_HTTP_Request
{
public:
    CLS_HTTP_Header *header = nullptr;

    String readRequest(WiFiClient client)
    {
        client.setTimeout(250);
        String request = "";
        do
        {
            String read = client.readString();
            request.concat(read);
        } while (client.available());
        request.trim();
        header = new CLS_HTTP_Header(request);
        return request;
    }

    bool isValid()
    {
        bool valid = true;
        valid &= header != nullptr;
        valid &= header->isValid();
        return valid;
    }

    CLS_HTTP_Request(WiFiClient client)
    {
#ifdef DEBUG_HTTP_REQUEST
        Serial.println("CLS_HTTP_Request(String strRequest)");
#endif
        String request = readRequest(client);
    }
};

extern CLS_Shutter shutter;
extern CLS_Aht10 aht10;

String getHttpHeadResponse(int code,
                           String strCode,
                           String contentType,
                           String cacheControl,
                           String responseContent)
{
    int contentLength = sizeof(responseContent);

    String value = "HTTP/1.1 " + String(code) + " " + strCode + "\n";

    value += "Allow: GET, POST\n";
    value += "Connection: close\n";
    value += "Content-type:" + contentType + "; charset=utf-8\n";
    if (cacheControl == "")
        value += "Cache-Control: no-cache\n";
    else
        value += "Cache-Control: " + cacheControl + "\n";
    value += "Content-Language: fr-FR\n";
    value += "Server: ESP8266_server\n";
    value += "X-Content-Type-Options: nosniff\n";
    //value += "Content-Length: " + String(contentLength) + "\n";
    value += "\n"; // Blank line before content

#ifdef DEBUG_HTTP_RESPONSE_HEADER
    Serial.println("___________________________ HTTP RESPONSE HEADER ___________________");
    Serial.println(value);
    Serial.println("____________________________________________________________________");
#endif

    value += responseContent;
    value += "\n";
    return value;
}

void sendHttpResponse(WiFiClient client, String response)
{
#ifdef DEBUG_HTTP_RESPONSE
    Serial.println("____________________________ HTTP RESPONSE __________________");
    Serial.println(response);
    Serial.println("____________________________________________________________________");
#endif
    client.println(response);
    client.flush();
}

void CLS_WebServer::handleHomePageRequest(WiFiClient client)
{
#ifdef DEBUG_WEB_SERVER
    Serial.println("________________________ HomePageRequest _____________________________");
#endif
    /*
    CLS_PageHTML oPage;
    String page = oPage.CreerHomePage();
    String response = getHttpHeadResponse(200, "OK", "text/html", "", page);
    sendHttpResponse(client, response);
    */
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", "Hello");
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleAht(WiFiClient client)
{
    String athValues = aht10.toTextPlain();
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", athValues);
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleSetHomeShuttle(WiFiClient client)
{
    shutter.setHomePosition();
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", "Done");
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleOpenShuttle(WiFiClient client)
{
    shutter.setPosition(0);
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", "Done");
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleCloseShuttle(WiFiClient client)
{
    shutter.setPosition(SHUTTER_FULL_ACTION_TIME);
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", "Done");
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleStopShuttle(WiFiClient client)
{
    shutter.stop();
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", "Done");
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleSetRatioShuttlePos(WiFiClient client, int ratioPosition)
{
    shutter.setRatioPosition(ratioPosition);
    String response = getHttpHeadResponse(200, "OK", "text/plain", "", "Done");
    sendHttpResponse(client, response);
}

void handleStyleCSS(WiFiClient client)
{
#ifdef DEBUG_WEB_SERVER
    Serial.println("________________________ StyleCSSRequest _____________________________");
#endif
    /*
    CLS_PageHTML oPage;
    String styleCSS = oPage.getStyleCSS();
    String response = getHttpHeadResponse(200, "OK", "text/css", "max-age=31536000, immutable", styleCSS);
    sendHttpResponse(client, response);
    */
    String response = getHttpHeadResponse(204, "No Content", "text/plain", "max-age=31536000, immutable", "style n'est pas géré");
    sendHttpResponse(client, response);
}

void handleFaviconRequest(WiFiClient client)
{
    String response = getHttpHeadResponse(204, "No Content", "text/plain", "max-age=31536000, immutable", "Favicon n'est pas géré");
    sendHttpResponse(client, response);
}

void handleEmptyRequest(WiFiClient client, CLS_HTTP_Request request)
{
#ifdef DEBUG_BAD_REQUEST
    Serial.println("________________________ BAD Request _____________________________");
#endif

    String response = getHttpHeadResponse(400, "Bad request", "text/plain", "", "\nInvalid URL\nURL requested=[" + request.header->headerLine + "]\n");
    sendHttpResponse(client, response);
}

void CLS_WebServer::handleClient(WiFiClient client)
{
    bool restartRequest = false;

#ifdef DEBUG_HTTP_REQUEST
    Serial.println(" ++++++++ HTTP CLIENT ++++++++");
#endif
    CLS_HTTP_Request request = CLS_HTTP_Request(client);
#ifdef DEBUG_HTTP_REQUEST
    Serial.println(" ++++++++ request analysis done ++++++++");
#endif
    if (request.isValid())
    {
        String uri = request.header->uri;
        uri.toUpperCase();
#ifdef DEBUG_HTTP_REQUEST
        Serial.println(" _____________________ HTTP REQUEST __________________________");
        Serial.println(" _____________________ " + uri + " __________________________");
#endif
        if (uri == "/") // réponse HTML des informations sur la cuve
            handleHomePageRequest(client);
        else if (uri == "/AHT")
            handleAht(client);
        else if (uri == "/HOME")
            handleSetHomeShuttle(client);
        else if (uri == "/UP")
            handleOpenShuttle(client);
        else if (uri == "/DOWN")
            handleCloseShuttle(client);
        else if (uri == "/STOP")
            handleStopShuttle(client);
        else if (uri == "/POS_1")
            handleSetRatioShuttlePos(client, 1);
        else if (uri == "/POS_2")
            handleSetRatioShuttlePos(client, 2);
        else if (uri == "/POS_3")
            handleSetRatioShuttlePos(client, 3);
        else if (uri == "/POS_4")
            handleSetRatioShuttlePos(client, 4);
        else if (uri == "/POS_5")
            handleSetRatioShuttlePos(client, 5);
        else if (uri == "/POS_6")
            handleSetRatioShuttlePos(client, 6);
        else if (uri == "/POS_7")
            handleSetRatioShuttlePos(client, 7);
        else if (uri == "/POS_8")
            handleSetRatioShuttlePos(client, 8);
        else if (uri == "/POS_9")
            handleSetRatioShuttlePos(client, 9);
        else if (uri == "/STYLE.CSS")
            handleStyleCSS(client);
        else if (uri == "/FAVICON.ICO")
            handleFaviconRequest(client);
    }
    else if (request.header != nullptr)
    {
        if (request.header->headerLine.length() > 0)
            handleEmptyRequest(client, request);
    }
    client.stop();
    if (restartRequest)
    {
#ifdef DEBUG_WEB_SERVER
        Serial.println("<-----------------------Restarting");
#endif
        ESP.restart();
    }
}

CLS_WebServer::CLS_WebServer()
{
#ifdef DEBUG_WEB_SERVER
    Serial.println("CLS_WebServer::CLS_WebServer()  ---- Instance created");
#endif
}
