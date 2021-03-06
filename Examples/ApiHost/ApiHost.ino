#include <WiFiApi.h>

String hostName = "ESP2";
String ssid="your WiFi Name";
String password="your password";


//function that recieves data
json handleCall(json data){
  //Getting Elements from JSON(i.e data)
  Serial.println("Name : " + String(data.getValue("Name")));
  Serial.println("Age : " + String(data.getNumberValue("Age")));
  Serial.println("CPI : " + String(data.getDoubleValue("CPI")));
   //Clear Entire JSON(i.e data)
  data.clear();
  //add acknowledgement to JSON(i.e data) 
  data.addUnit("ACK",200.66);
  delay(20);
  return data;
}

void setup() {
  Serial.begin(115200);
  
  //establish connection with Wifi
  Connect(ssid,password,hostName);

  //gives local IP Address of network
  Serial.println(getAddress());

  //register API and recieves data from it
  //parameter_1 - name of Api eg. if Link is "http://127.0.0.1/get" then "get" is Api's name
  //parameter_2 - fuction which recieves data
  registerApi("get",handleCall);

  //starts server
  serverBegin();
}

void loop() {
  
}
