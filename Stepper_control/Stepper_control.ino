#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 


#define stepPin 4
#define dirPin 7
#define enable 8

int index = 0;

const int microSteps = 16;              // 1/16 step enabled
const int stepper = 200;   // stepper has 200 steps/rotation
const int stepsPerRevolution = stepper * microSteps;


static int stepsToGo = 0;         // 3200 == 360degrees
static int retractSteps = 0;        // steps to retract
static int RPM = 0;               // Rounds per minute
static bool direction = HIGH;        // true == CW, false == CCW
static bool retraction = false;       // toggle for retraction
static bool doMove = false;          // trigger variable

// calculating delaytime from initial given RPM
// 60 for seconds/min, 1000000 from microseconds to seconds, 2 because of 2x delayTime/step
int delayTime =  ((60. * 1000. * 1000.) / (stepper * microSteps * 2. * float(RPM)));


void setup() 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(500000);       // Set the baud rate to match the C++ program
  Serial.setTimeout(1);
  pinMode(enable, OUTPUT);  // default HIGH (stepper off)
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, direction);

  waitingScreen();
}



void loop()
{  

  if(Serial.available())
  {
    receive();
    
    digitalWrite(enable, LOW);    // enable stepper
    
    move();
    
    delay(100);

    if (retraction)
    {
      retract();
    }

    delay(3000);
    waitingScreen();
    digitalWrite(enable, HIGH);   // disable stepper    
  }  
}





void move()
{
  if (retraction)
  {
    stepsToGo += retractSteps;
  }

  for (int step = 0; step < stepsToGo; step++)
  {
    // step the motor one step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
}

  
  

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

void changeDir()
{
  direction = !direction;
  digitalWrite(dirPin, direction);
  delayMicroseconds(200);
}



void receive()
{
  // Get Data
  String receivedData = Serial.readStringUntil('\n');

  // Find the positions of the commas
  int firstIndex = receivedData.indexOf(';');
  int secondIndex = receivedData.indexOf(';', firstIndex + 1);
  int thirdIndex = receivedData.indexOf(';', secondIndex + 1);
  int fourthIndex = receivedData.indexOf(';', thirdIndex + 1);

  // Get substrings, convert and refresh global variables
  stepsToGo = receivedData.substring(0, firstIndex).toInt();
  retractSteps = receivedData.substring(firstIndex + 1, secondIndex).toInt();
  RPM = receivedData.substring(secondIndex + 1, thirdIndex).toInt();
  
  if (receivedData.substring(thirdIndex + 1, fourthIndex).toInt() == 1)
  {
    direction = HIGH;
  }
  else 
  {
    direction = LOW;
  }

  if (receivedData.substring(fourthIndex + 1).toInt() == 1)
  {
    retraction = true;
  }
  else if (receivedData.substring(fourthIndex + 1).toInt() == 0)
  {
    retraction = false;
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STP: ");
  lcd.print(stepsToGo);
  lcd.print(" RS: ");
  lcd.print(retractSteps);
  lcd.setCursor(0, 1);
  lcd.print("RPM: ");
  lcd.print(RPM);
  lcd.print(" D:");
  lcd.print(direction);
  lcd.print(" R:");
  lcd.print(retraction);
  delay(200);

  // calculating delaytime from given RPM
  // 60 for seconds/min, 1000000 from microseconds to seconds, 2 because of 2x delayTime/step
  delayTime =  ((60. * 1000. * 1000.) / (stepper * microSteps * 2. * RPM));

}

void waitingScreen()
{
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Waiting:");
    lcd.setCursor(3, 1);
    lcd.print("for signal");
}