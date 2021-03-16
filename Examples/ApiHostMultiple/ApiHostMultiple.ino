#include <WiFiApi.h>

String hostName = "ESP2";
String ssid="SMARTLAB";
String password="C@M_SERVER@2019";


//function that recieves data
json Servo1Handle(json data){
  //Getting Elements from JSON(i.e data)
  Serial.println("State of servo1  "+String(data.getValue("State")));
  data.clear();
  //add acknowledgement to JSON(i.e data) 
  data.addUnit("servo1's state recieved",200);
  delay(20);
  return data;
}
json Servo2Handle(json data){
  //Getting Elements from JSON(i.e data)
  Serial.println("State of servo2"+String(data.getValue("State")));
  data.clear();
  //add acknowledgement to JSON(i.e data) 
  data.addUnit("servo2's state recieved",200);
  delay(20);
  return data;
}
json Servo3Handle(json data){
  //Getting Elements from JSON(i.e data)
  Serial.println("State of servo3"+String(data.getValue("State")));
  data.clear();
  //add acknowledgement to JSON(i.e data) 
  data.addUnit("servo3's state recieved",200);
  delay(20);
  return data;
}
json PneumaticHandle(json data){
  //Getting Elements from JSON(i.e data)
  Serial.println("State of Pneumatic  "+String(data.getValue("State")));
  data.clear();
  //add acknowledgement to JSON(i.e data) 
  data.addUnit("Pneumatic's state recieved",200);
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
  registerApi("gets1",Servo1Handle);
  registerApi("gets2",Servo2Handle);
  registerApi("gets3",Servo3Handle);
  registerApi("getpu",PneumaticHandle);
 
  //starts server
  serverBegin();
}

void loop() {
  
}
