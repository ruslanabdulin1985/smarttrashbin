int echoPin = 9; 
int trigPin = 8; 
int button = 2;

#include <Servo.h> //используем библиотеку для работы с сервоприводом
Servo servo; //объявляем переменную servo типа Servo

#include <Adafruit_NeoPixel.h>  //библиотека для работы с адресными светодиодами
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define LedDataPIN            6
#define NUMPIXELS      4

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);

int cyclesCounter = 0; // to count when it's time to extend the open range
int delayBetweenLightsSmile = 150; // delay for half a second

int TapCurrentPosition = servo.read();
boolean serviceMode = false;



boolean shortPress = false;
boolean longpPress = false;
boolean lastDistance = 0;

int distanceToOpen = 80;
//boolean isOpened = false;

void openTap(int Position){
//  TurnOffTheLights();
  servo.attach(4); //привязываем привод к портуs
  delay(30);
  for(int i=Position; i<=175; i=i+1)
  {
  servo.write(i);
  TapCurrentPosition = i;
  delay(15);
  }
}

//void Button(){
//  openTap(TapCurrentPosition);
//  
//  }

void closeTap(int Position)
{
  for(int i=TapCurrentPosition; i>=75; i=i-1)
  {
  servo.write(i);
  TapCurrentPosition = i;
  delay(15);
  }
 
  //servo.detach();
  delay(1000);
  setDistanceToOpen();
  
}

void initiate(){
  servo.attach(4);
  servo.write(60);
  int TapCurrentPosition = servo.read();
  delay(100);
}

int setDistanceToOpen(){
  int currentDistance = GetAverageDistance();
   Serial.println(currentDistance);
  if (currentDistance>150)
    distanceToOpen = 80;
//
//  else if (currentDistance>150)
//    distanceToOpen = 100;
  
  else
    distanceToOpen = currentDistance-30;
}

int GetDistance(){
  int duration, cm; 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  cm = duration / 58;
  return cm;
  }

int GetAverageDistance(){
  int times = 30;
  int counter = 0;
  for(int i=0; i<times; i++){
    counter+=GetDistance();
    delay(1);
  }
  return counter/times;
}

void setGreenLighs(){
  for (int j=0; j<NUMPIXELS; j++){
       pixels.setPixelColor(j, pixels.Color(0,150,0));
       pixels.show(); // This sends the updated pixel color to the hardware.
    }
}

void knightBlink(){
  for(int j=0; j<NUMPIXELS; j++){
   pixels.setPixelColor(j, pixels.Color(0,0,205));
   pixels.show(); // This sends the updated pixel color to the hardware.
   delay (100);
  }
  for(int j=0; j<NUMPIXELS; j++){
   pixels.setPixelColor(j, pixels.Color(0,0,0));
   pixels.show(); // This sends the updated pixel color to the hardware.
   delay (100);
  }
}

void blinkRedLighs(){
  for (int j=0; j<NUMPIXELS; j++){
       pixels.setPixelColor(j, pixels.Color(255,0,0));
       pixels.show(); // This sends the updated pixel color to the hardware.
    }
   delay(100);
   for (int j=0; j<NUMPIXELS; j++){
       pixels.setPixelColor(j, pixels.Color(0,0,0));
       pixels.show(); // This sends the updated pixel color to the hardware.
    }
   delay(100);
}

void redLigtsOn(){
   for (int j=0; j<NUMPIXELS; j++){
       pixels.setPixelColor(j, pixels.Color(255,0,0));
      
    }
     pixels.show(); // This sends the updated pixel color to the hardware.
}


void OpenThenClose(){
   // Turn on green lights
    setGreenLighs();
    delay(200);
    openTap(TapCurrentPosition);
//    Serial.println( "Opened");
    delay (6000);

    for(int i =0; i<20; i++){
      redLigtsOn();
      delay(100);
      TurnOffTheLights();
      delay(100);
      
    }
//    redLigtsOn();
    setGreenLighs();
    closeTap(TapCurrentPosition);
    delay (100);
    TurnOffTheLights();
}



void TurnOnTheLights(){
for (int j=0; j<NUMPIXELS; j++){
     pixels.setPixelColor(j, pixels.Color(255,255,255));
  }
pixels.show(); // This sends the updated pixel color to the hardware.
delay (1000);
}

void TurnOnYellowLights(){
for (int j=0; j<NUMPIXELS; j++){
     pixels.setPixelColor(j, pixels.Color(184,134, 11));
  }
pixels.show(); // This sends the updated pixel color to the hardware.
}


void TurnOnBlueLights(){
  for (int j=0; j<NUMPIXELS; j++){
     pixels.setPixelColor(j, pixels.Color(0, 0, 205));
  }
pixels.show(); // This sends the updated pixel color to the hardware.
}


void TurnOffTheLights(){
for (int j=0; j<NUMPIXELS; j++){
     pixels.setPixelColor(j, pixels.Color(0,0,0));
  }
  
pixels.show(); // This sends the updated pixel color to the hardware.
}


void setup() {
//  setDistanceToOpen(); 
  //Servo
  initiate();
  Serial.begin (9600); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(button, INPUT_PULLUP);
  //servo.attach(7); //привязываем привод к портуs
//  servo.write(25);
  //attachInterrupt (0, Button, LOW);

lastDistance = GetAverageDistance();
//led
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
pixels.begin(); // This initializes the NeoPixel library.
setDistanceToOpen(); 
}
 
void loop() {

//Serial.println (GetDistance());
//cyclesCounter++;


//servo.write(10);
checkButton();

if (checkDistance()){
  OpenThenClose();
}
//
//if (cyclesCounter = 500){
//  distanceToOpen = GetAverageDistance()-10;
//  cyclesCounter = 0;
//}

  
  

delay(10);   
}

boolean checkDistance(){  // Check if it's time to open a trashBin
  int i = 0;
  boolean toReturn = false;
  int recentDistance = GetAverageDistance();
  while (i<NUMPIXELS && recentDistance<distanceToOpen){
     
     Serial.print("DistToOpen=");
     Serial.print(distanceToOpen);
     Serial.print("  recDist=");
     Serial.println(recentDistance);
     pixels.setPixelColor(i, pixels.Color(0,150,0));
     pixels.show(); // This sends the updated pixel color to the hardware.
     delay (40);
     recentDistance = GetAverageDistance();
     if (i==NUMPIXELS-1){
       toReturn = true;
       break;
       }
     i++;
  }
  for (int j=0; j<NUMPIXELS; j++){
     pixels.setPixelColor(j, pixels.Color(0,0,0));
     pixels.show(); // This sends the updated pixel color to the hardware.
  }
  return toReturn;
  
}

void checkButton(){
  if (isButtonPressed()){
    TurnOnBlueLights();

    openTap(TapCurrentPosition);
    
    while(isButtonPressed())
      knightBlink();
    

    while(!isButtonPressed()){
      TurnOnBlueLights();
      delay(50);
    }
   
    closeTap(TapCurrentPosition);   
    TurnOffTheLights();
  }
}


boolean isButtonPressed(){
   return (digitalRead(button) == 0);
}
