#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

unsigned int UDPPort1 = 2302; 
unsigned int UDPPort2 = 2303;  
unsigned int UDPPort3 = 2304;  
unsigned int UDPPort4 = 2305;  // local port to listen on
char cmd;
char packetBuffer[255];                       //buffer to hold incoming packet
char ReplyBuffer[] = "OK";         // a string to send back
String teststr = "";
char myssid[] = "I-Trash";
char mypass[] = "rootcsie";
char ssid[] = "frank-home";
char pass[] = "BR";
// your network SSID (name) 

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  WiFi.mode(WIFI_STA);
  WiFi.softAP(myssid,mypass);
  Udp.begin(UDPPort1);
  Udp.begin(UDPPort2);
  Udp.begin(UDPPort3); 
  Udp.begin(UDPPort4);
  Serial.println();
  Serial.println("Started ap. Local ip: " + WiFi.localIP().toString());
}

void loop() {
  /*if(WiFi.status() != WL_CONNECTED) {
    connectnetwork();
    }*/
  String Sensor_data1="";
  String Sensor_data2="";
  String Sensor_data3="";
  String Sensor_data4="";
  String Sensor_num = "";
  String Sensor_RSSI= "";
  //int Sensor_array[]={0,0,0,0};
  int Sensor_array[]={-25,-30,-59,-10};  //test
  
  String S1 = "S1=";
  String S2 = "S2=";
  String S3 = "S3=";
  String S4 = "S4=";
  
  Sensor_data1= receivedata1();//port 2302
  Sensor_data2= receivedata2();//port 2303
  Sensor_data3= receivedata3();//port 2304
  Sensor_data4= receivedata4();//port 2305//Sensor_data="2,-80"
  //Serial.write(Sensor_data);
  /*有4個port要寫*/
  //------------------------------------------------
  Sensor_num1 = Sensor_data1[0];
  Sensor_num2 = Sensor_data2[0];
  Sensor_num3 = Sensor_data3[0];
  Sensor_num4 = Sensor_data4[0];
  
  for(int i=2 ; i<5 ; i++){
    Sensor1_RSSI= Sensor1_RSSI + Sensor_data1[i];
  }
  for(int i=2 ; i<5 ; i++){
    Sensor2_RSSI= Sensor2_RSSI + Sensor_data2[i];
  }
  for(int i=2 ; i<5 ; i++){
    Sensor3_RSSI= Sensor3_RSSI + Sensor_data3[i];
  }
  for(int i=2 ; i<5 ; i++){
    Sensor4_RSSI= Sensor4_RSSI + Sensor_data4[i];
  }

  //---------------------
  if(Sensor_num != ""){
    Serial.print("outside_Sensor_data: ");
    Serial.println(Sensor_data);
    Serial.print("outside_Sensor_num: ");
    Serial.println(Sensor_num);
    Serial.print("outside_Sensor_RSSI: ");
    Serial.println(Sensor_RSSI);
  }
  byte senddata;
  String Sensor_location = "";
//  Sensor_location = Sensor_location + Sensor_num + "," + Sensor_RSSI;
//  for(int i=0 ; i<Sensor_location.length() ; i++){
//    senddata = Sensor_location[i];
//    //Serial.println(senddata);
//    Serial.write(senddata);
//    Serial.println();
//    }
  if(Sensor_num == "1"){
   // S1 = S1 + Sensor1_RSSI;
    Sensor_array[0] = Sensor1_RSSI.toInt();
    }
  if (Sensor_num == "2"){
    //  S2 = S2 + Sensor2_RSSI;
      Sensor_array[1] = Sensor2_RSSI.toInt();
    }
  if(Sensor_num == "3"){
      //  S3 = S3 + Sensor3_RSSI;
        Sensor_array[2] = Sensor3_RSSI.toInt();
     }
  if(Sensor_num == "4"){
        //  S4 = S4 + Sensor4_RSSI;
          Sensor_array[3] = Sensor4_RSSI.toInt();
      }

      for(int i=0;i<4;i++)
      {
        Sensor_location = Sensor_location + Sensor_array[i] + ",";
      }
     delay(2000);
     
     Serial.write(Sensor_location);//-83,-91,-54,-86
    //Serial.println(S1);
    //Serial.println(Sensor_array[0]);
    //Serial.println(S2);
    //Serial.println(S3);
    //Serial.println(S4);
    
    
  //http://140.127.218.56/dashboard/QQ.php?S1=XX&&S2=XX&&S3=XX&&S4=XX update url
  
  
  //delay(1000);
}
String receivedata1(){
  // if there's data available, read a packet
  String Sensor=" ";
  String txt_RSSI="";
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    //Serial.print("Which Sensor: ");
    Sensor = remoteIp[3]-1;//****
    txt_RSSI = Sensor + ",";
    //Serial.println(Sensor);
    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    //Serial.print("Contents Sensor");
    //Serial.print(Sensor);
    //Serial.print(" :");
    //Serial.println(packetBuffer);
    //Serial.println("Receive successful");
    txt_RSSI += packetBuffer ;//txt_RSSI="2,-80"
    //Serial.print("Total txt:");
    //Serial.println(txt_RSSI);
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  else{
    Serial.println("Waiting for Receive...");
    }
    return txt_RSSI;
  delay(1000);
  }
  void connectnetwork(){
    WiFi.begin(myssid);
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
    }


