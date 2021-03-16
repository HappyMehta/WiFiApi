#include <WiFiApi.h>




String ssid="your WiFi Name";
String password="your password";
String hostName = "ESP";

IPAddress local_IP(192, 168, 1, 115);//Mention IPAddress you want as WiFi's localIP Address 
IPAddress gateway(192, 168, 1, 1);//Mention gatewayAddress accordding to local_IP & Router


ServerReference sv1;
json myObj;

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
  Connect(ssid,password,local_IP,gateway);

  //gives local IP Address of network
  Serial.println(getAddress());

  //register API and recieves data from it
  //parameter_1 - name of Api eg. if Link is "http://127.0.0.1/get" then "get" is Api's name
  //parameter_2 - fuction which recieves data
  registerApi("get",handleCall);

  //starts server
  serverBegin();

  //Add Key-Value Pair in JSON
  myObj.addUnit("Name", "Kalhar");
  myObj.addUnit("Age", 21);
  myObj.addUnit("CPI", 9.15);

  //set host by connecting with API of server
  //parameter_1 - API name of server(local_IP of server)
  //parameter_2 - Server Api's name where you want to sent message  eg. if Link is "http://192.168.1.115/get" then "get" is Api's name 
  sv1.setHost("ESP","get");
 
}
json response;
void loop() {
    //sends json object which contains message and reurns json response recieved from server
    response = sv1.send(myObj);
    Serial.println(response.getString());
  
}
