//Sensor1����SSI
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

unsigned int UDPPort = 2302;                  // local port to listen on
char cmd;  // ��摨��靘��誘
char packetBuffer[255];                       //buffer to hold incoming packet
char ReplyBuffer[] = "FYBR";         // a string to send back
String teststr = "";
char myssid[] = "I-Trash";
char mypass[] = "rootcsie"; // your network SSID (name)

WiFiUDP Udp;

void setup() {
    Serial.begin(115200);
    Serial.println("Start");
    }

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    connectnetwork();
    }
    String replay = "";
    long rssi = WiFi.RSSI();
   // String localip = WiFi.localIP().toString();
   // int sensor_num = loaclip[10].toInt();
    Serial.print("Send RSSI:");
    Serial.println(rssi);
    teststr = rssi;
    //teststr = sensor_num + "." + rssi;
    replay = senddata(teststr);
    teststr = "";
    if(replay == "error"){
      WiFi.disconnect();
      Serial.println("WiFi disconnected...");
      }
      delay(4000);
}
String senddata(String str){
  String ans = "";
  char Buffer[255];
  str.toCharArray(Buffer, 255);
  Udp.begin(UDPPort);
  Udp.beginPacket("192.168.4.1", UDPPort);//send ip to server
  Udp.write(Buffer); 
  Udp.endPacket();
  Serial.println("Send RSSI...");
  
  /*Serial.print("PacketSize: ");
  Serial.println(packetSize);
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.print("Reply: ");
    Serial.println(packetBuffer);
    */
    // send a reply, to the IP address and port that sent us the packet we received
    /*Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(Buffer);
    Udp.endPacket();
    Serial.println("Send successful");
    ans = "OK";
  /*}else{
    ans = "error";
    }*/
  //delay(100);
  return ans;
  }
  void connectnetwork(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(myssid,mypass);
    Serial.println();
    Serial.print("Wait for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());

    Udp.begin(UDPPort);
    Udp.beginPacket("192.168.4.1", UDPPort);//send ip to server
    char ipBuffer[255];
    WiFi.localIP().toString().toCharArray(ipBuffer, 255);
    Udp.write(ipBuffer);
    Udp.endPacket();
    Serial.println("Sent ip adress to server");
    delay(100);
    }

