#include  <SoftwareSerial.h>
#include  <ESP8266WiFi.h>
#include  <WiFiClient.h>
#include  <ESP8266WebServer.h>

const char* ssid = "imessWLAN"; //Wifi SSID
const char* password = "imessgmbhwifi2018"; //Wifi Password

IPAddress ip(192,168,11,98); //Set static IP for ESP8266
IPAddress gateway(192,168,11,241); //Router IP Address 
IPAddress subnet(255,255,255,0); //Subnet router

ESP8266WebServer server(80); //Port to connect ESP8266
SoftwareSerial rs(14,12);// ESP8266 RX/TX connect to RX/TX in MAX232 interface

void handleNotFound(){
  Serial.println("404. No connection");
}

void handleRoot(){
  char read_data; 
  double value = 0;
  String str_value = "";
  String all_data = "";

  while(rs.available()){
     read_data = rs.read();
     if(isDigit(read_data)){
       all_data = all_data + read_data;
     }
  }
  if(all_data != ""){
    //Serial.print("All data ");
    //Serial.println(all_data);
    value = all_data.toDouble()/100;
    //Serial.print("Value ");
    //Serial.println(value);
    str_value = String(value, 2);
    //Serial.print("Send value ");
    //Serial.println(str_value);
    if(str_value != ""){
      str_value.replace(".", ",");
      Serial.print("Saber per 1m: ");
      Serial.print(str_value);
      Serial.println();
      server.send(200,"text/plain", str_value); 
    } 
  } 
}

void setup(){
  Serial.begin(9600);
  rs.begin(9600);
  
  Serial.println();
  Serial.println("MAC Address: "+WiFi.macAddress());
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  long rssi = WiFi.RSSI();
  Serial.print("Wifi signal ");
  Serial.print(rssi);
  Serial.println("dBm");
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop(){
  server.handleClient();
}
