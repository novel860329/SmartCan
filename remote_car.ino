#include "ESP8266WiFi.h"

char ssid[] = "I-Trash";          // your network SSID (name) 
char pass[] = "rootcsie";          // your network password
int Sensor_port = 8001;
IPAddress server(10,250,163,158);
WiFiClient client;
boolean isconnect = false;

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.println("Start");
  connectwifi();
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //WiFi.mode(WIFI_STA);
  //WiFi.disconnect();
  delay(100);
  isconnect = client.connect(server, Sensor_port);
  Serial.println("Setup done");
}

void loop() {
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Reconnect wifi");
    connectwifi();
    }
    
  if (!isconnect) {
    Serial.println("Reconnect Server");
    client.connect(server, Sensor_port);
  }
  else
  {
    
 
    Serial.print(".");
  
  while(client.available()>0) {
    //Serial.println(client.available());
    char c = client.read();
    
    Serial.print("Write to car: ");
    Serial.write(c);
    Serial.println("");
  }
  delay(1000);
 }
 /*WiFiClientSecure client;
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.begin(myssid, mypass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println(".");  
    Serial.print("Connect to");
    Serial.println(myssid);
  Serial.println("closing connection");
  Serial.println();
  client.stop();*/

  
  ip = WiFi.localIP();

  while(1){
  delay(1000);
  long rssi = WiFi.RSSI();
  Serial.print("RSSI = ");
  Serial.println(rssi);
  Serial.print("IP = ");
  Serial.println(ip);
  }

  
  // Wait for ever...
  
  
}
void connectwifi(){
  Serial.print("Wait for WiFi");
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to wifi!");
  }
void connectServer(){
  if (client.connect(server, Sensor_port)) {
    Serial.println("connected");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    } 
}
