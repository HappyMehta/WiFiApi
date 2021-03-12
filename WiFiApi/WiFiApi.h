#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ArduinoJSON.h>
String api_server_ssid = "";
String api_server_password = "";
String PARAM_MESSAGE = "message";
AsyncWebServer server(80);

IPAddress subnet(255, 255, 255, 0);
int Connect(String ssid, String password, IPAddress local_IP, IPAddress gateway)
{
    api_server_ssid = ssid;
    api_server_password = password;
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin(api_server_ssid.c_str(), api_server_password.c_str());
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            Serial.printf("WiFi Failed!\n");
            WiFi.begin(api_server_ssid.c_str(), api_server_password.c_str());
            return -1;
        }
    return 1;
}
String getAddress()
{
    Serial.println(WiFi.localIP());
}
json (*dataRecieve)(json);

json messageRecieved;
void registerApi(String name, json (*recieve)(json))
{
    messageRecieved.clear();
    int cnt = 0;
    dataRecieve = recieve;
    server.on(("/" + name).c_str(), HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam(PARAM_MESSAGE))
        {

            String message = request->getParam(PARAM_MESSAGE)->value();
            messageRecieved = parseJSON(message);
        }
        else
        {
            messageRecieved.addUnit("error","Message Not Present");
        }
        json response = dataRecieve(messageRecieved);
        // Serial.println(response.getString());
        request->send(200, "text/plain", response.getString());
    });
}
void serverBegin()
{
    server.begin();
}
class ServerReference
{
    public:
        String host = "";
        WiFiClient client;
        String apiName="";
        json response;
        String url;
        ServerReference()
        {}

        ServerReference(String server_host,String name)
        {
            setHost(server_host,name);
        }
        void setHost(String server_host,String name)
        {
            host=server_host;
            apiName=name;
        }
        void setMessage(String msg)
        {
            url= "/"+apiName+"?message="+msg;
        }
        int connectHost()
        {
            Serial.print("connecting to ");
            Serial.println(host);
            const int httpPort = 80;
            unsigned long timeout = millis();
            while (!client.connect(host.c_str(), httpPort)) 
            {
                if (millis() - timeout > 5000) 
                {
                Serial.println("connection failed");
                return -1;
                }
            }
            return 1;
        }
        json send(json mg)
        {   
            if(connectHost())
            {
                setMessage(mg.getString());
                client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
                return recieveResponse();
            }

            response.clear();
            response.addUnit("error","Connection Failed");
            return response;
        }
        json recieveResponse()
        {
            unsigned long timeout = millis();
            while (client.available() == 0) 
            {
                if (millis() - timeout > 1000) 
                {
                Serial.println(">>> Client Timeout !");
                client.stop();
                response.clear();
                response.addUnit("error","Client Timeout");
                return response;
                }
            }
            String line = "";
            while (client.available())
            {
                line += char(client.read());
            }
            Serial.println(line);
            response = parseJSON(line);
            return response;  
        }

};