#include <Servo.h>

int TRIGGER_DISTANCE = 15;  //小於幾公分距離時,開蓋子
int JudgeTimes = 25;  //要判斷幾次距離才能決定要丟垃圾
long duration;
boolean DOOR_OPENED = 0; //目前蓋子是打開的嗎?
#define trigPin   6
#define echoPin   7

Servo myservo;

void setup()
{
  Serial.begin(9600);
  
  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  
  myservo.attach(9); // 連接數位腳位9，伺服馬達的訊號線
}

int distance(String deviceType) {
  long cm;
  if(deviceType=="Hand") {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    pinMode(echoPin, INPUT);             // 讀取 echo 的電位
     duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間
     cm = (duration/2) / 29.1; 
    Serial.print(cm); Serial.println(" cm");    
  }/*else{
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    cm = pulseIn(echoPin2, HIGH)/58;   
    
  }*/
  
  return cm;
}

//設定垃圾桶的垃圾量顯示燈號
/*void setLEDStatus() {
  if(TrashRed()==1) {
    Serial.println("LED Red.");
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledBluePin, LOW);
    digitalWrite(ledYellowPin, LOW);
  }else if(TrashYellow()==1) {
    Serial.println("LED Yellow.");
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledBluePin, LOW);
    digitalWrite(ledYellowPin, HIGH);    
  }else if(TrashBlue()==1) {
    Serial.println("LED Blue.");
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledBluePin, HIGH);
    digitalWrite(ledYellowPin, LOW);    
  }  
}*/

//判斷垃圾量是否快滿
/*char TrashRed() {
 int numTimer = 0;
  for(int i = 0; i < JudgeTimes; i+=1){
    if(distance("Trash")<=3) numTimer++;
  }
  if(numTimer>(JudgeTimes-2)) {
    return 1;
  }else{
    return 0;
  }  
}*/

//判斷垃圾量是否半滿
/*char TrashYellow() {
 int numTimer = 0;
  for(int i = 0; i < JudgeTimes; i+=1){
    if(distance("Trash")<=8 and distance("Trash")>3) numTimer++;
  }
  if(numTimer>(JudgeTimes-2)) {
    return 1;
  }else{
    return 0;
  }  
}*/

//判斷垃圾量是否不多
/*char TrashBlue() {
 int numTimer = 0;
  for(int i = 0; i < JudgeTimes; i+=1){
    if(distance("Trash")>8) numTimer++;
  }
  if(numTimer>(JudgeTimes-2)) {
    return 1;
  }else{
    return 0;
  }  
}*/

//處理手接近時的動作
boolean handNear() {
  int numTimer = 0;
  for(int i = 0; i < JudgeTimes; i++){
    if(distance("Hand")<TRIGGER_DISTANCE) numTimer++;
  }
  if(numTimer>(JudgeTimes-8)) {
    return 1;
  }else{
    return 0;
  }
}

//處理手移開時的動作
boolean handLeave() {
  int numTimer = 0;
  for(int i = 0; i < JudgeTimes; i++){
    if(distance("Hand")>TRIGGER_DISTANCE) numTimer++;
  }
  if(numTimer>(JudgeTimes-8)) {
    return 1;
  }else{
    return 0;
  }
}

//關閉桶蓋的動作
void closeDoor(int numSpeed, int numDelay) {
  for(int i = 50; i <= 180; i+=numSpeed){
    myservo.write(i); // 使用write，傳入角度，從0度轉到180度
    DOOR_OPENED = 0;
    delay(numDelay);
  }
}

//打開桶蓋的動作
void openDoor(int numSpeed, int numDelay) {
  for(int i = 180; i >= 50; i-=numSpeed){
    myservo.write(i);// 使用write，傳入角度，從180度轉到0度
    DOOR_OPENED = 1;
    delay(numDelay);
  }
}

void loop()
{   
  //如果手還在垃圾筒上 (小於定義距離)
  if(handNear()==1) {
        openDoor(3, 10);
      do{
        delay(500);
        //如果不在垃圾筒上了 (大於定義距離)
        if(handLeave()==1)  closeDoor(2,15);
        
      } while (DOOR_OPENED==1);
  }
  //setLEDStatus();
  delay(500);
}
