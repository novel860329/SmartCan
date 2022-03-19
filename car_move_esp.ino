#include <SoftwareSerial.h>
SoftwareSerial Esp(4, 13);//Rx,Tx

int const JudgeTimes = 10;
int often_dist[JudgeTimes] ={0};
#define FTrigPin   2
#define FEchoPin   3
#define RTrigPin   7
#define REchoPin   8
#define LTrigPin   11
#define LEchoPin   12
const int motorIn2 = 10;
const int motorIn1 = 9;
const int motorIn3 = 6;     
const int motorIn4 = 5; 

int state = 0;//紀錄是不是在走y軸
double car_x = 0,car_y = 0;
double des_x = 0,des_y = 0;
String backtostart = "";
String control_direct ="";//前後左右控制
char dir = ' ';
int pos = 0;
boolean back = false;

void motorstop()
{
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, LOW);
  //delay(15);
  
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
}
 
void forward() {     // 馬達轉向：前進（兩個馬達都正轉）
  backtostart += 'f';
  //for(int i=150;i<256;i+=10)
  //{   
    analogWrite(motorIn1,180);//*右邊
    analogWrite(motorIn2,0);//輪胎*/
    
    analogWrite(motorIn3,179);//*左邊
    analogWrite(motorIn4,0);//輪胎*/
    //Serial.println('f');
  //}
}
 
void backward() {// 馬達轉向：後退（兩個馬達都反轉）
  backtostart += 'b';    
  //for(int i=150;i<256;i+=10)
  //{    
    analogWrite(motorIn1,0);//*右邊
    analogWrite(motorIn2,179);//輪胎*/
    
    analogWrite(motorIn3,0);//*左邊
    analogWrite(motorIn4,180);//輪胎*/
    //Serial.println('b');
  //}
}
 
void turnLeft() {    // 馬達轉向：左轉（馬達A反轉、馬達B正轉）
  backtostart += 'l'; 
  //for(int i=230;i<240;i+=10)
  {
    analogWrite(motorIn1,140);//*右邊
    analogWrite(motorIn2,0);//輪胎*/
    
    analogWrite(motorIn3,0);//*左邊
    analogWrite(motorIn4,230);//輪胎*/
  }
  //Serial.println('l');
}
 
void turnRight() {     // 馬達轉向：右轉（馬達A正轉、馬達B反轉）
  backtostart += 'r'; 
  //for(int i=230;i<240;i+=10)
  //{
    analogWrite(motorIn1,0);//*右邊
    analogWrite(motorIn2,140);//輪胎*/
    
    analogWrite(motorIn3,230);//*左邊
    analogWrite(motorIn4,0);//輪胎*/
  //}
  //Serial.println('r');
}

int distance(int trigPin,int echoPin) {
    long duration;
    long cm;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    pinMode(echoPin, INPUT);             // 讀取 echo 的電位
    duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間
    cm = (duration/2) / 29.1; 
    //Serial.print(cm); Serial.println(" cm");
    return cm;    
}

void bubble_sort(int *a)
{
  int i,j,temp; 
  for(i = 0; i < JudgeTimes - 1; i++)
  {  
      for(j = 0; j < JudgeTimes - 1 - i; j++)
      {    
          if(a[j + 1] < a[j])        
           {              
                  temp = a[j];               
                  a[j] = a[j+1];               
                  a[j+1] = temp;           
          }
      }
   }
}

long Fdistance()
{
    for(int i = 0; i < JudgeTimes; i++)
    {     
      often_dist[i] = distance(FTrigPin,FEchoPin);
      delay(2);
    }
    bubble_sort(often_dist);
    return often_dist[JudgeTimes/2];
    /*long duration;
    long cm;
    
    digitalWrite(FTrigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(FTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(FTrigPin, LOW);
    
    pinMode(FEchoPin, INPUT);             // 讀取 echo 的電位
     duration = pulseIn(FEchoPin, HIGH);   // 收到高電位時的時間
     cm = (duration/2) / 29.1; 
    //Serial.print(cm); Serial.println(" cm");
    return cm;*/
}
long Rdistance()
{
    for(int i = 0; i < JudgeTimes; i++)
    {
      often_dist[i] = distance(RTrigPin,REchoPin);
      delay(2);
    }
    bubble_sort(often_dist);
    return often_dist[JudgeTimes/2];
    /*long duration;
    long cm;
    
    digitalWrite(RTrigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(RTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(RTrigPin, LOW);
    
    pinMode(REchoPin, INPUT);             // 讀取 echo 的電位
     duration = pulseIn(REchoPin, HIGH);   // 收到高電位時的時間
     cm = (duration/2) / 29.1; 
    //Serial.print(cm); Serial.println(" cm");
    return cm;*/
}
long Ldistance()
{
    for(int i = 0; i < JudgeTimes; i++)
    {
      often_dist[i] = distance(LTrigPin,LEchoPin);
      delay(2);
    }
    bubble_sort(often_dist);
    return often_dist[JudgeTimes/2];
    /*long duration;
    long cm;
    
    digitalWrite(LTrigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(LTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(LTrigPin, LOW);
    
    pinMode(LEchoPin, INPUT);             // 讀取 echo 的電位
     duration = pulseIn(LEchoPin, HIGH);   // 收到高電位時的時間
     cm = (duration/2) / 29.1; 
    //Serial.print(cm); Serial.println(" cm");
    return cm;*/
}
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(FTrigPin, OUTPUT); 
   pinMode(FEchoPin, INPUT); 
   pinMode(RTrigPin, OUTPUT); 
   pinMode(REchoPin, INPUT); 
   pinMode(LTrigPin, OUTPUT); 
   pinMode(LEchoPin, INPUT); 

///馬達訊號區////////////////
   pinMode(motorIn1, OUTPUT);
   pinMode(motorIn2, OUTPUT);
   pinMode(motorIn3, OUTPUT);
   pinMode(motorIn4, OUTPUT); 

   Esp.begin(115200);
}

void loop() {
  long Fdist,Rdist,Ldist;
  Fdist = Fdistance();
  Ldist = Ldistance();
  Rdist = Rdistance();
  Serial.print(Fdist); Serial.print(" cm(前) ");
  Serial.print(Ldist); Serial.print(" cm(左) ");
  Serial.print(Rdist); Serial.println(" cm(右)");

if(Esp.available())
{
  char ch = Esp.read();
  //Serial.print(ch);
  Esp.flush();
  delay(2);
  if(ch!=',') 
  {
    control_direct = control_direct + ch;
    //Serial.println(control_direct);
  }
  else if(ch == ',')
  {
    for(int i=0;i<control_direct.length();i++)
    {
      if(control_direct[i] == 'b')
      {
        back = true;
      }
      else if(control_direct[i] == 'r')
      {
        dir = 'r';
        pos = i;
        break;
      }
      else if(control_direct[i] == 'l')
      {
        dir = 'l';
        pos = i;
        break;
      }
    }
  for(int i=0;i<control_direct.length();i++)
  {
    Fdist = Fdistance();
    Ldist = Ldistance();
    Rdist = Rdistance();
    Serial.print("direct: ");
    Serial.println(control_direct[i]);
    Serial.println(control_direct);
    Serial.print(" direct len: ");
    Serial.println(control_direct.length());
    if(control_direct[i]!='f' && control_direct[i]!='r'&& control_direct[i]!='l' && control_direct[i]!='b')
     {
        continue;
     }
    else if(control_direct[i]=='f' && Fdist>15)
    {
      forward();
      delay(300);
    }
    else if(control_direct[i] =='b')
    {
      turnRight();
      delay(650);
      turnRight();
      delay(650);
    }
    else if(control_direct[i]=='r' && Rdist>15 && !back)//往前左右不改變
    {
      turnRight();
      delay(650);
    }
    else if(control_direct[i]=='r' && Ldist>15 && back)//往後左右顛倒
    {
      turnLeft();
      delay(650);
    }
    else if(control_direct[i]=='l' && Ldist>15 && !back)
    {
      turnLeft();
      delay(650);
    }  
    else if(control_direct[i]=='l' && Rdist>15 && back)
    {
      turnRight();
      delay(650);
    }    
    //else if(Fdist <= 15){  //避障
      
    else if(dir == 'r' && Rdist > 15)
    {
      //Serial.println("front is wall");
      boolean xwall = false;
      turnRight();
      delay(650);
      while(Ldistance() <= 15)//左邊還是障礙物就一直往前
      {
        //Serial.println("left is wall");
        forward();
        delay(300);
        for(int k = control_direct.length() - 1; k >= 0;k--)
        {
          if(control_direct[k] == 'f' && i < pos)
          {
            control_direct[k] = 'n';
            break;
          }
          if(control_direct[k] == 'f' && i > pos)
          {
            if(xwall==false){
              control_direct = control_direct + 'l';
              xwall = true;
            }
            control_direct = control_direct + 'f';
            break;
          }
        }
      }
      turnLeft();//左邊已清空，恢復原本的方向
      delay(650);
    }
    else if(dir == 'l' && Ldist > 15)
    {
      boolean xwall = false;
      turnLeft();
      delay(650);
      while(Rdistance() <= 15)//右邊還是障礙物就一直往前
      {
        forward();
        delay(300);
        for(int k = control_direct.length() - 1; k >= 0;k--)
        {
          if(control_direct[k] == 'f' && i < pos)
          {
            control_direct[k] = 'n';
            break;
          }
          if(control_direct[k] == 'f' && i > pos)
          {
            if(xwall==false){
              control_direct = control_direct + 'r';
              xwall = true;
            }
            control_direct = control_direct + 'f';
            break;
          }
        }
      }
      turnRight();//右邊已清空，恢復原本的方向
      delay(650);
   }
   else if(Rdist>=Ldist && Rdist > 15)
    {
      boolean xwall = false;
      turnRight();
      delay(650);
      while(Ldistance() <= 15)//左邊還是障礙物就一直往前
      {
        forward();
        delay(300);
        if(i < pos)control_direct = control_direct + 'f';
        if(i > pos)
        {
          if(xwall==false){
            control_direct = control_direct + 'l';
            xwall = true;
          }
          control_direct = control_direct + 'f';           
        }
      }
      turnLeft();//左邊已清空，恢復原本的方向
      delay(650);
    }
    else if(Ldist>Rdist && Ldist > 15)
    {
      boolean xwall = false;
      turnLeft();
      delay(650);
      while(Rdistance() <= 15)//右邊還是障礙物就一直往前
      {
        forward();
        delay(300);
        if(i < pos)control_direct = control_direct + 'f';
        if(i > pos)
        {
          if(xwall==false){
            control_direct = control_direct + 'r';
            xwall = true;
          }
          control_direct = control_direct + 'f';           
        }
      }
      turnRight();//右邊已清空，恢復原本的方向
      delay(650);
    }
  }
  for(int i=0;i<10;i++)
  {
    Serial.println("Walking...................");
    delay(1000);
  }
  Serial.println("Arrive!!!!!");
  /*丟垃圾的時間*/
  while(1){
    
  int avail = 0;
  if(Serial.available() > 0)
  {
    motorstop();
    //Serial.println("motorstop");
    delay(10);
    
    String string1 = "";
    char c  = ' ';
    avail = 1;
    c = Serial.read();
    delay(10);
    while(c != ',')
    {
      motorstop(); 
      //Serial.println("motorstop");
      delay(10);
     
      if(avail == 1){
        string1 = string1 + c;
        //Serial.println(string1);
      }
      avail = 0;
      if(Serial.available() > 0){
        motorstop();
        delay(10);
        //Serial.println("motorstop");
        c = Serial.read();
        delay(10);
        avail = 1;
      }
    }
    if(string1 == "STOP")// 收到9代表不能動 ， 0代表車子要返回原點
    {                      // 其他為垃圾量
      motorstop();
      delay(10);
      Serial.println("Stop!!");
      //Serial.println("motorstop");
    }
    else if(string1 == "GO")
    {
      //返回原點
      motorstop();
      //Serial.println("motorstop");
      delay(10);
      Serial.println("GO!!");
      break;
    }
    else
    {
       Esp.write(string1[0]);
       delay(10);
       Serial.print("trash amount: ");
       Serial.println(string1[0]);
       motorstop();
       //Serial.println("motorstop");
       delay(10);
    }
  }
  motorstop();
  //Serial.println("motorstop");
  delay(10);
  }
  for(int i=0;i<10;i++)
  {
    Serial.println("back...................");
    delay(1000);
  }
  turnRight();//車頭轉到反方向朝出發點
  delay(650);
  turnRight();
  delay(650); 
  for(int i=control_direct.length();i >=0;i--)//走回出發點
    {
      if(control_direct[i]=='f')
      {
        forward();
        delay(300);
      }
      else if(control_direct[i]=='r')
      {
        turnLeft();
        delay(650);
      }
      else if(control_direct[i]=='l')
      {
        turnRight();
        delay(650);
      }
    }
    turnRight();//車頭轉到反方向朝出發點
    delay(350);
    turnRight();
    delay(350); 
    /*control_direct ="";//前後左右控制
    dir = ' ';
    pos = 0;
    back = false;*/
  }
}  
/*else
{
  motorstop();
  /*String string1 = "";
  char c = ' ';
  if(Serial.available() > 0)
  { 
    c = Serial.read();//????
    while(c != '\n')//c != ','
    {
      motorstop();
      string1 = string1 + c;
      c = Serial.read();
    }
  }
  Esp.write(string1[0]);*/
  //Serial.println("no data.....");
//} 
  /*String car_xbuf = "";
  String car_ybuf = "";
  String des_xbuf = "";
  String des_ybuf = "";
  for(int i =1; i<=5; i++)
  {
    car_xbuf += control_direct[i];
  }
  for(int i =6; i<=10; i++)
  {
    car_ybuf += control_direct[i];
  }
  for(int i =11; i<=15; i++)
  {
    des_xbuf += control_direct[i];
  }
  for(int i =16; i<=20; i++)
  {
    des_ybuf += control_direct[i];
  }
  car_x = car_xbuf.toFloat();
  car_y = car_ybuf.toFloat();
  des_x = des_xbuf.toFloat();
  des_y = des_ybuf.toFloat();

  Serial.println(control_direct);
  /*Serial.println(car_xbuf);Serial.println(car_ybuf);Serial.println(des_xbuf);Serial.println(des_ybuf);
  Serial.println(car_x);Serial.println(car_y);Serial.println(des_x);Serial.println(des_y);
  Serial.println(control_direct[0]);//f54.2626.5350.4751.25
  
  if(control_direct[0]=='s')
  {
    motorstop();
    state = 0;
    /*
      if(Serial.available())//停下來讓人丟垃圾的時間//蓋子關閉10秒後(代表人沒有要丟垃圾)
      {
        char a = Serial.read();
        while(a!=0)
        {
          motorstop();
          if(Serial.available())
          {
             a = Serial.read();
          }
        }
      }
    turnRight();//車頭轉到反方向朝出發點
    delay(350);
    turnRight();
    delay(350); 
    for(int i=0;i<backtostart.length();i++)//走回出發點
    {
      if(backtostart[i]=='f')
      {
        forward();
        delay(800);
      }
      else if(backtostart[i]=='b')
      {
        backward();
        delay(800);
      }
      else if(backtostart[i]=='r')
      {
        turnLeft();
        delay(550);
      }
      else if(backtostart[i]=='l')
      {
        turnRight();
        delay(350);
      }
    }
    turnRight();//車頭轉到反方向朝出發點
    delay(350);
    turnRight();
    delay(350); 
    backtostart = "";
  }
  else if(control_direct[0]=='f' && Fdist>15)
  {
    //Esp.write("f");
    forward();
    delay(300);
  }
  else if(control_direct[0]=='r' &&Rdist>15)
  {
    //motorstop();
    //delay(10);
    //Esp.write("r");
    state++;
    turnRight();
    delay(650);
  }
  else if(control_direct[0]=='l' &&Ldist>15)
  {
    //motorstop();
    //delay(10);
    //Esp.write("l");
    state--;
    turnLeft();
    delay(650);
  }
  else if(control_direct[0]=='b')
  {
    //motorstop();
    //delay(10);
    //Esp.write("b");
    backward();
    delay(300);
  }  
  else{  //避障
    if(des_x <= car_x && Rdist > 15)
    {
      //Esp.write("r");
      turnRight();
      delay(650);
      while(Ldistance() <= 15)//左邊還是障礙物就一直往前
      {
        forward();
        delay(800);
      }
      turnLeft();//左邊已清空，恢復原本的方向
      delay(550);
      forward();
      delay(800);
      if(state%4!=0)
      {      
        while(Ldistance() <= 15)//左邊還是障礙物就一直往前
        {
          forward();
          delay(800);
        }
      }
    }
    else if(des_x > car_x && Ldist > 15)
    {
      //Esp.write("l");
      turnLeft();
      delay(550);
      while(Rdistance() <= 15)//右邊還是障礙物就一直往前
      {
        forward();
        delay(800);
      }
      turnRight();//右邊已清空，恢復原本的方向
      delay(650);
      forward();
      delay(800);
      if(state%4!=0)
      {       
        while(Rdistance() <= 15)//右邊還是障礙物就一直往前
        {
          forward();
          delay(800);
        }
      }
    }
    else if(Rdist>=Ldist && Rdist > 15)
    {
      //Esp.write("r");
      turnRight();
      delay(650);
      while(Ldistance() <= 15)//左邊還是障礙物就一直往前
      {
        forward();
        delay(800);
      }
      turnLeft();//左邊已清空，恢復原本的方向
      delay(550);
      forward();
      delay(800);
      if(state%4!=0)
      {        
        while(Ldistance() <= 15)//左邊還是障礙物就一直往前
        {
          forward();
          delay(800);
        }
      }
    }
    else if(Ldist>Rdist && Ldist > 15)
    {
      //Esp.write("l");
      turnLeft();
      delay(550);
      while(Rdistance() <= 15)//右邊還是障礙物就一直往前
      {
        forward();
        delay(800);
      }
      turnRight();//右邊已清空，恢復原本的方向
      delay(650);
      forward();
      delay(800);
      if(state%4!=0)
      {       
        while(Rdistance() <= 15)//右邊還是障礙物就一直往前
        {
          forward();
          delay(800);
        }
      }
    }
    /*else if(Fdist<=15 && Rdist<=15 && Ldist<=15)//先暫時不考慮此情況
    {
      backward();
      delay(500);
    }
  }*/
Esp.flush();
}
