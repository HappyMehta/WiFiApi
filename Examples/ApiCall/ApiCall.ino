#include <WiFiApi.h>

String hostName = "ESP1"; 
String ssid="your WiFi Name";
String password="your password";

ServerReference sv1;
json myObj;
void setup() {
     Serial.begin(115200);
  
  //establish connection with Wifi
  Connect(ssid,password,hostName);

  //gives local IP Address of network
  Serial.println(getAddress());
  
  //Add Key-Value Pair in JSON
  myObj.addUnit("Name", "Kalhar");
  myObj.addUnit("Age", 21);
  myObj.addUnit("CPI", 9.15);

  //set host by connecting with API of server
  //parameter_1 - Ip address of server(local_IP of server)
  //parameter_2 - Server Api's name where you want to sent message  eg. if Link is "http://192.168.1.115/get" then "get" is Api's name 
  String server1HostName = "ESP2";
  sv1.setHost(server1HostName,"get");
 
}
json response;
void loop() {
  //sends json object which contains message and reurns json response recieved from server
  response = sv1.send(myObj);
  Serial.println(response.getString());
}
