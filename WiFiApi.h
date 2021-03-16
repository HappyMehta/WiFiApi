#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ArduinoJSON.h>
#define MAX_API 15
json hostAdress;
String api_server_ssid = "";
String api_server_password = "";
String PARAM_MESSAGE = "message";
AsyncWebServer server(80);
json (*dataRecieve[15])(json);
IPAddress subnet(255, 255, 255, 0);
bool isServer = false;

void browseService(const char * service = "http", const char * proto = "tcp"){
    Serial.printf("Browsing for service _%s._%s.local. ... ", service, proto);
    int n = MDNS.queryService(service, proto);
    if (n == 0) {
        Serial.println("no services found");
    } else {
        Serial.print(n);
        Serial.println(" service(s) found");
        hostAdress.clear();
        for (int i = 0; i < n; ++i) {
            // Print details for each service found
            
        hostAdress.addUnit(MDNS.hostname(i), MDNS.IP(i).toString());
        }
    }
    Serial.println();
}

int Connect(String ssid, String password, String hostName)
{
    api_server_ssid = ssid;
    api_server_password = password;
    WiFi.mode(WIFI_STA);
    WiFi.begin(api_server_ssid.c_str(), api_server_password.c_str());
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(500);
        }
    int cnt = 0;
    String offset = "";
    while (!MDNS.begin((hostName+offset).c_str())) {
        Serial.println("Error setting up MDNS responder!");
        delay(500);
        offset = String(++cnt);
        Serial.println("offset = "+offset);
    }
    Serial.println("Connected as "+ hostName+offset );
    return 1;

}
String getAddress()
{
    WiFi.localIP();
}

json messageRecieved;
int apiId = 0;
json idUrlMapping;
void registerApi(String name, json (*recieve)(json))
{
    idUrlMapping.addUnit("/"+name, apiId);
    isServer = true;
    messageRecieved.clear();
    int cnt = 0;
    dataRecieve[apiId] = recieve;
    server.on(("/" + name).c_str(), HTTP_GET, [](AsyncWebServerRequest *request) {
        String urlId;
        if (request->hasParam(PARAM_MESSAGE))
        {
            urlId=request->url();
            //Serial.println("message recieved from"+url);
            String message = request->getParam(PARAM_MESSAGE)->value();
            messageRecieved = parseJSON(message);
        }
        else
        {   
            messageRecieved.addUnit("error","Message Not Present");
        }
        //Serial.println("Api Index= "+String(idUrlMapping.getNumberValue(urlId)));
        json response = dataRecieve[idUrlMapping.getNumberValue(urlId)](messageRecieved);
        // Serial.println(response.getString());
        request->send(200, "text/plain", response.getString());
    });
    apiId++;
}
void serverBegin()
{
    server.begin();
    if(isServer){
        MDNS.addService("http", "tcp", 80);
    }
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
        void setHost(String hostName,String name)
        {
            while(hostAdress.getValue(hostName) == ""){
                browseService();
            }
            host=hostAdress.getValue(hostName);
            //Serial.println(host);   
            apiName=name;
        }
        void setMessage(String msg)
        {
            url= "/"+apiName+"?message="+msg;
        }
        int connectHost()
        {
            // Serial.print("connecting to ");
            // Serial.println(host);
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
            //Serial.println(line);
            response = parseJSON(line);
            return response;  
        }

};



