
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            6 // define the LED Strips Signal pin
#define SOUND         A0// sound sensor pin
#define NUMPIXELS      40// maximum led number


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int pushbutton1 = 2;
int pushbutton2 = 4;
int delayval = 10; // delay for 10ms
int Signal = 0;// catch a ball signal
int sound = 0;
int a = 0;
int b = 60;
int c = 240;//(0,60,240)blue
int b2g = 0 ; //0: unable it
int g2b = 0;
int g2r = 0;
int r2b = 0;
void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(SOUND, INPUT);  
}

void loop() 
{
  if (digitalRead(pushbutton1)==LOW) {Signal = 1;}
  if (digitalRead(pushbutton2)==LOW) {Signal = 0;}
  sound = analogRead(SOUND); 
  Serial.println(sound);
  Serial.println(Signal);
  
//Signal = 0: not catch anything
  if (Signal == 0 ){
    //red to blue:(240,0,0)->(0,60,240)
    if(r2b==1){
      RedtoBlue();
      r2b = 0;
      b2g = 1;
    }
    //green to blue:(0,240,0)->(0,60,240)
    else if(g2b == 1){
      GreentoBlue();
      g2r = 0;
      g2b = 0;
      b2g = 1;
     }
    //state = blue;
    else{         
      Stayblue();
      b2g = 1;  
    }
  }
  
//Signal=1: catched it!
  else if(Signal == 1)
  { 
    //GREEN: when ball is catched and no noise disturbing
    //blue tp green:(0,60,240)->(0,240,0)
    if(sound<=60 && b2g == 1){
      BluetoGreen();
      b2g = 0;
      g2r = 1;
      g2b = 1;
    }

    //RED: when noise is detected, free the ball and set signal = 0
    //green to red:(0,240,0)->(240,0,0)
    else if(sound>60 && g2r==1){
      RedLightShine();
      a = 240;
      b = 0;
      c = 0;
      g2b=0;
      g2r=0;
      r2b=1;
      Signal = 0;
    }
  }
}


void RedtoBlue( ){
//red to blue: (240, 0, 0)->(0,60,240)
a = 240;
b = 0;
c = 0;
for(int i = 0; i<60;i++){
  Serial.println("r2b");
  a -= 4;
  b += 1;
  c += 4;
  for (int j = 0; j<NUMPIXELS; j++) { 
      pixels.setPixelColor(j, pixels.Color(a,b,c));  
      pixels.show();
  }
}}

void GreentoBlue( ){
//green to blue:(0,240,0)->(0,60,240)
a = 0;
b = 240;
c = 0;
for(int i = 0; i<30;i++){
   Serial.println("g2b"); 
   b -= 6;
   c += 8;
   for (int j = 0; j<NUMPIXELS; j++) { 
      pixels.setPixelColor(j, pixels.Color(a,b,c));  
      pixels.show();
   }
  }
}


int glow = 1;
int green = 0;
int blue = 0;

void Stayblue () {
  for(int i = 0; i < 30; i++){
    green += 2  * glow;
    blue += 7 * glow;
    for (int i=0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0,green,blue)); // Moderately bright blue color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
    }
  }
  glow = -glow;
}

void BluetoGreen(){
a = 0;
b = 60;
c = 240;
for(int i = 0; i<30;i++){
   Serial.println("b2g");
   b += 6;
   c -= 8;
   for (int j = 0; j<NUMPIXELS; j++) { 
      pixels.setPixelColor(j, pixels.Color(a,b,c));  
      pixels.show();}
   }
}

void RedLightShine(){
for(int i = 0; i<3;i++){// shine for three times 
  for (int j = 0; j<NUMPIXELS; j++) { 
    pixels.setPixelColor(j, pixels.Color(255,0,0));  
    pixels.show();} 
  delay(500);// interval between each shine
  for (int j = 0; j<NUMPIXELS; j++){
    pixels.setPixelColor(j, pixels.Color(0,0,0));  
    pixels.show();}
  }
}
