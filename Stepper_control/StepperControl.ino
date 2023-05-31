#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

// XStep 2, XDir 5
// YStep 3, YDir 6
// ZStep 4, ZDir 7
// AStep 12, ADir 13 // you loose SpnEn and SpnDir
// https://forum.arduino.cc/t/does-anybody-know-how-to-contol-the-cnc-shield/572940/2
// https://forum.arduino.cc/t/using-cnc-shield-v3-directly-with-arduino-ide/1022999/3


#define enable 8
#define fanPin 11
#define XStep 2
#define XDir 5
#define YStep 3
#define YDir 6
#define ZStep 4
#define ZDir 7
#define AStep 12
#define ADir 13


const int microSteps = 16;              // 1/16 step enabled
const int stepper = 200;                // stepper has 200 steps/rotation
const int stepsPerRevolution = stepper * microSteps;
static bool waitingScreenToggle = false;
unsigned long long previousMillis = 0;
static int fanAlwaysOn = false;

// Receive Data init
static int whichPump;                   // defines the controlled pump
static int stepPin;                     // Z-axis is default
static int dirPin;                      // Z-axis is default
static long stepsToGo = 0;               // 3200 == 360degrees
static long retractSteps = 0;            // steps to retract
static int RPM = 0;                     // Rounds per minute
static bool direction = HIGH;           // true == CW, false == CCW
static long retractDelay = 200;          // delay between steps and retraction
static int fanSpeed = 0;                // fanspeed value from 0-100
static long fanTime = 0;                // after-run time of fan in [ms]


// calculating delaytime from initial given RPM
// 60 for seconds/min, 1000000 from microseconds to seconds, 2 because of 2x delayTime/step
int delayTime =  ((60. * 1000. * 1000.) / (stepper * microSteps * 2. * float(RPM)));


void setup() 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(500000);         // Set the baud rate to match the C++ program
  Serial.setTimeout(1);
  
  pinMode(enable, OUTPUT);      // default HIGH (stepper off)
  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0);    // disable fan

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Waiting:");
  lcd.setCursor(3, 1);
  lcd.print("for signal");


  pinMode(XStep, OUTPUT);
  pinMode(YStep, OUTPUT);
  pinMode(ZStep, OUTPUT);
  pinMode(AStep, OUTPUT);

  digitalWrite(enable, HIGH);   // disable stepper (this is also default)
}



void loop()
{  
  // when data is received
  if(Serial.available())
  {
    receive();                  // call the receive function
    
    move();                     // move the given steps
    
    // if retraction is enabled
    if (retractSteps > 0)
    {
      delay(retractDelay);        // delay between steps and retraction
      retract();  // run retraction movement
    }

    previousMillis = millis(); 
  }

  checkFanTime();

}


void checkFanTime()
{
  unsigned long long currentMillis = millis();

  if (currentMillis - previousMillis >= fanTime)
  {
    if (!fanAlwaysOn)
    {// turn of fan after fanTime
      previousMillis = currentMillis;
      analogWrite(fanPin, 0);
    }

    if (stepsToGo == 0)
    {
      digitalWrite(enable, HIGH);
    }

    // Toggle this if you either want or dont want to disable steppers after movement
    //digitalWrite(enable, HIGH);   // disable stepper
    
    if (waitingScreenToggle)
    {
      waitingScreen();
    }
  }
}


// function to mange the given movement command
void move()
{
  fanSpeed = map(fanSpeed, 0, 100, 0, 255);
  analogWrite(fanPin, fanSpeed);   // set fanSpeed
  
  digitalWrite(dirPin, direction);  // set direction
  
  digitalWrite(enable, LOW);        // enable stepper

  // check if retraction is enabled (if so add the retraction steps to stepsToGo)
  if (retractSteps > 0)
  {
    stepsToGo += retractSteps;
  }

  for (int step = 0; step < stepsToGo; step++)
  {
    // step the motor one step:
    makeStep();
  }
}


// function that steps the motor one step
void makeStep()
{
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
}


// function to manage retraction
void retract()
{
  changeDir();

  for (int step = 0; step < retractSteps; step++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }

  changeDir();
}


// function to manage dirction change
void changeDir()
{
  direction = !direction;
  digitalWrite(dirPin, direction);
  delayMicroseconds(200);
}


// function to manage received data
void receive()
{
  // Get Data
  String receivedData = Serial.readStringUntil('\n');

  // Find the positions of the commas
  int firstIndex =    receivedData.indexOf(';');
  int secondIndex =   receivedData.indexOf(';', firstIndex + 1);
  int thirdIndex =    receivedData.indexOf(';', secondIndex + 1);
  int fourthIndex =   receivedData.indexOf(';', thirdIndex + 1);
  int fifthIndex =    receivedData.indexOf(';', fourthIndex + 1);
  int sixthIndex =    receivedData.indexOf(';', fifthIndex + 1);
  int seventhIndex =  receivedData.indexOf(';', sixthIndex + 1);

  // Get substrings, convert and refresh global variables
  whichPump =     receivedData.substring(0, firstIndex).toInt();
  stepsToGo =     atol(receivedData.substring(firstIndex + 1, secondIndex).c_str());
  retractSteps =  atol(receivedData.substring(secondIndex + 1, thirdIndex).c_str());
  RPM =           receivedData.substring(thirdIndex + 1, fourthIndex).toInt();
  direction =     receivedData.substring(fourthIndex + 1, fifthIndex).toInt();
  retractDelay =  atol(receivedData.substring(fifthIndex + 1, sixthIndex).c_str());
  fanSpeed =      receivedData.substring(sixthIndex + 1, seventhIndex).toInt);
  fanTime =       atol(receivedData.substring(seventhIndex + 1)..c_str();
  fanTime *= 1000; // convert from given [s] to [ms]

  if (fanTime == -1000)
  {
    fanAlwaysOn = true;
  }

  // show transmission on LCD
  infoScreen();
  waitingScreenToggle = true;

  // calculating delaytime from given RPM   ( 60 for seconds/min, 1000000 from microseconds to seconds, 2 because of 2x delayTime/step)
  delayTime =  ((60. * 1000. * 1000.) / (stepper * microSteps * 2. * RPM));

  switch (whichPump)
  {
    case 1:
      stepPin = XStep;
      dirPin =  XDir;
      break;
    case 2:
      stepPin = YStep;
      dirPin =  YDir;
      break;
    case 3:
      stepPin = ZStep;
      dirPin =  ZDir;
      break;
    case 4:
      stepPin = AStep;
      dirPin =  ADir;
      break;
    default:
      break;
  }
}


// function to show "waiting" on LCD
void waitingScreen()
{
    waitingScreenToggle = false;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Waiting:");
    lcd.setCursor(3, 1);
    lcd.print("for signal");
}


// function to show transmitted data on LCD
void infoScreen()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S: ");
  lcd.print(stepsToGo);
  lcd.print(" RS: ");
  lcd.print(retractSteps);
  lcd.setCursor(0, 1);
  lcd.print("RPM: ");
  lcd.print(RPM);
  lcd.print(" FS:");
  lcd.print(fanSpeed);
  delay(20);
}