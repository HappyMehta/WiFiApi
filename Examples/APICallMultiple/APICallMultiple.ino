#include <WiFiApi.h>

String hostName = "ESP1"; 
String ssid="SMARTLAB";
String password="C@M_SERVER@2019";

ServerReference s1;
ServerReference s2;
ServerReference s3;
ServerReference p4;
json gate;
json response1,response2,response3,response4;
void setup() {
     Serial.begin(115200);
  
  //establish connection with Wifi
  Connect(ssid,password,hostName);

  //gives local IP Address of network
  Serial.println(getAddress());
  
  //Add Key-Value Pair in JSON
  gate.addUnit("State", "ONN");
  //set host by connecting with API of server
  //parameter_1 - Ip address of server(local_IP of server)
  //parameter_2 - Server Api's name where you want to sent message  eg. if Link is "http://192.168.1.115/get" then "get" is Api's name 
  String server1HostName = "ESP2";
  s1.setHost(server1HostName,"gets1");
  s2.setHost(server1HostName,"gets2");
  s3.setHost(server1HostName,"gets3");
  p4.setHost(server1HostName,"getpu");
 
}
json response;
void loop() {
  //sends json object which contains message and reurns json response recieved from server
  response1 = s1.send(gate);
  Serial.println(response1.getString());
 // delay(50);
  response2 = s2.send(gate);
  Serial.println(response2.getString());
  //delay(50);
  response3 = s3.send(gate);
  Serial.println(response3.getString());
  //delay(50);
  response4 = p4.send(gate);
  Serial.println(response4.getString());
  //delay(50);
  //Serial.println(ESP.getFreeHeap());
}
