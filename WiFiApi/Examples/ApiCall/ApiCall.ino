#include <WiFiApi.h>


String ssid="your WiFi Name";
String password="your password";

IPAddress local_IP(192, 168, 1, 125);//Mention IPAddress you want as WiFi's localIP Address 
IPAddress gateway(192, 168, 1, 1);//Mention gatewayAddress accordding to local_IP mentioned above


ServerReference sv1;
json myObj;
void setup() {
     Serial.begin(115200);
  
  //establish connection with Wifi
  Connect(ssid,password,local_IP,gateway);

  //gives local IP Address of network
  Serial.println(getAddress());
  
  //Add Key-Value Pair in JSON
  myObj.addUnit("Name", "Kalhar");
  myObj.addUnit("Age", 21);
  myObj.addUnit("CPI", 9.15);

  //set host by connecting with API of server
  //parameter_1 - API name of server(local_IP of server)
  //parameter_2 - Server Api's name where you want to sent message  eg. if Link is "http://192.168.1.115/get" then "get" is Api's name 
  sv1.setHost("192.168.1.115","get");
 
}
json response;
void loop() {
  //sends json object which contains message and reurns json response recieved from server
  response = sv1.send(myObj);
  Serial.println(response.getString());
}
