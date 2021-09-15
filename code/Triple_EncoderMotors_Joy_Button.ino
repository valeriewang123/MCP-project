#include <Encoder.h>
#define POWER 500
// motor X
#define enA 3
#define in1 17//A3
#define in2 18//A4
//motor Y
#define enB 5
#define in3 2
#define in4 4
//dc motor Z
#define enC 6
#define in5 7
#define in6 A5

#define delaytime 200
#define Limitdis  30000
Encoder myEncX(8,9);
Encoder myEncY(10,11);
int xPin = A0; //joystick X
int yPin = A1; // joystick Y
//int button = 4;//joystick sw button
int valuex = 0;
int valuey = 0;

long oldPositionX = 0;
long oldPositionY = 0;
long distX = 0;
long distY = 0;
int disXL = 0;
int disXR = 0;
int disYL = 0;
int disYR = 0;
int button1Pin = 12;
int button2Pin = 16; //A2
int button1State = HIGH;
int button2State = HIGH;

//---------------VARIABLES DEFINE FINISH --------------------------------//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  pinMode(button2Pin, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);

  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
  digitalWrite(enC, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);

  // read the orignal position once
  oldPositionX = myEncX.read();
  oldPositionY = myEncY.read();
}
 
void loop() {
// READ joystick input: (x|y|buttonstate)
//  valuex = analogRead(xPin);
//  Serial.print("X:"); Serial.print(valuex, DEC);
//  valuey = analogRead(yPin);
//  Serial.print(" | Y:"); Serial.print(valuey, DEC);

// READ BUTTONS
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);

  Serial.print("Button1: ");
  Serial.println(button1State, DEC);
  Serial.print("Button2: ");
  Serial.println(button2State, DEC);
   
// update Encoded distance
  long newPositionX = myEncX.read();
  long newPositionY = myEncY.read();  
  distX = newPositionX - oldPositionX;
  distY = newPositionY - oldPositionY;
//  Serial.println(distX);
//  Serial.println(distY);
//  
// Z condition
  if(button2State == LOW){
     turnRight();  
//     Serial.print("turn right");
  }else if (button1State == LOW) { //button is pressed
    turnLeft();
//    Serial.print("turn left ");
  }
  else{
    stopMotor();
  }
  

// X&Y conditions
  if(distX>Limitdis){disXR = 1;} //disable X axis to right rotate
  if(distX<-Limitdis){disXL = 1;} //disable X axis to left rotate
  if(distY>Limitdis){disYR = 1;} //disable Y axis to right rotate
  if(distY>Limitdis){disYL = 1;}//disable Y axis to left rotate

  if(disXR)
  {
    if (disYR)     {LL();}// when both X and Y axis can't Right rotate, then only left left is possible
    else if(disYL) {LR();}
    else           {L(); }
  }

  if(disXL)
  {
    if(disYR)      {RL();}
    else if(disYL) {RR();}
    else           {R();}
  }

  else  {enable();}
    
  Serial.println();
  delay(delaytime); 
}

//-----------functions------------------//
void LL()
{
  if (valuex < 100){
    MotorXL(); 
    MotorYStop();}
  else if(valuey < 100){
    MotorYL();
    MotorXStop();}
  else{
    MotorXStop();
    MotorYStop();
    }
}

void LR(){
  if (valuex < 100){
    MotorXL(); 
    MotorYStop();}
  else if(valuey > 900){
    MotorYR();
    MotorXStop();}
  else{
    MotorXStop();
    MotorYStop();
    }  
  }
  
void L(){
  if (valuex < 100){
    MotorXL(); 
    MotorYStop();}
  else if(valuey < 100){
    MotorYL();
    MotorXStop();}
  else if(valuey > 900){
    MotorYR();
    MotorXStop();}
  else{
    MotorXStop();
    MotorYStop();
    }  
  }
  
void RL(){
   if (valuex > 900){
    MotorXR(); 
    MotorYStop();}
  else if(valuey < 100){
    MotorYL();
    MotorXStop();}
  else{
    MotorXStop();
    MotorYStop();
    }
  }
  
void RR(){
   if (valuex>900){
    MotorXL(); 
    MotorYStop();}
  else if(valuey >900){
    MotorYL();
    MotorXStop();}
  else{
    MotorXStop();
    MotorYStop();
    }
  }
  
void R(){
  if (valuex > 900){
    MotorXR(); 
    MotorYStop();}
  else if(valuey < 100){
    MotorYL();
    MotorXStop();}
  else if(valuey > 900){
    MotorYR();
    MotorXStop();}
  else{
    MotorXStop();
    MotorYStop();
    } 
  }
  
void enable(){
  if (valuex < 100){
    MotorXL();
    MotorYStop();
    }
  else if(valuex > 900){
    MotorXR(); 
    MotorYStop();
    }
  else if(valuey < 100){
    MotorYL();
    MotorXStop();
    }
  else if(valuey > 900){
    MotorYR();
    MotorXStop();
    }
  else{
    MotorXStop();
    MotorYStop();
    }  
  }
  

void MotorXL(){
//  Serial.print(" ->MotorXLeft");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, POWER);
}
  
void MotorXR(){
//  Serial.print(" ->MotorXRight");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, POWER);
}

void MotorXStop(){
//  Serial.print(" ->XNotRotate");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW); 
}
void MotorYL(){
//  Serial.print(" ->MotorYLeft");
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, POWER);
}
  
void MotorYR(){
//  Serial.print(" ->MotorYRight");
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, POWER);
}

void MotorYStop(){
//  Serial.print(" ->YNotRotate");
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}

//DC Motor
void turnLeft()   
{
  

  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  // set speed to 150 out 255
  analogWrite(enC, POWER);
  //delay(2000);
  
}

void turnRight()   
{
  
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  // set speed to 150 out 255
  analogWrite(enC, POWER);
 
  
}

void stopMotor(){
  // now turn off motors 
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
 }
