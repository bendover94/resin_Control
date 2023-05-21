// Resin exchange controller V1.0 by Lucas Schmirl, FH Technikum-Wien
/*
  This script controls an arduino Nano

  Inputs:       2 Buttons wired to D2 (Button 1) and D3 (Button 2) each to GND
  Outputs:      2 Relais signals wired to D6 (Relais 1) and D7 (Realais 2)
  OLED screen:  I2C connection SDA to A4 and SCK to A5

  The push of a button triggers the corresponding Relais circuit.
  Additionaly the Relais states are visible at the screen.

  Arduino gets powerd either with mini-USB (5V) or VIN and GND (7V-12V), DCDC-Buck is included for 2nd option (currently set to 5V)
  The Relais get powered from 
      control:  from arduino (with jumper) from external source (5V DC) (without jumper)
      power:    external source (max: 10A 250VAC, 30VDC)
*/

// BUTTON
#define RELAIS_1 6
#define RELAIS_2 7
#define BUTTON_1 2
#define BUTTON_2 3

bool button_1_val;
bool button_2_val;



// DISPLAY
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() 
{
// Serial Monitor
  Serial.begin(9600);

// RELAIS
  pinMode(RELAIS_1, OUTPUT);
  pinMode(RELAIS_2, OUTPUT);
  digitalWrite(RELAIS_1, HIGH); // RELAIS OFF, reversed ciruit for safety
  digitalWrite(RELAIS_2, HIGH); // RELAIS OFF, reversed ciruit for safety
  
// BUTTONS
  pinMode(BUTTON_1, INPUT_PULLUP); // To use buttons without circuit
  pinMode(BUTTON_2, INPUT_PULLUP); // To use buttons without circuit


// DISPLAY
  initDisplay();  // initialize the Display and print the start screen 
}


void initDisplay()
{
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(33,0);
  display.print("Resin");
  display.setCursor(22,15);
  display.print("Control");
  display.display();
  delay(3000);
  display.clearDisplay();
}

void handle_Display()
{
  display.clearDisplay();

  display.setCursor(10,0);
  display.print("Ch 1:");
  display.setCursor(10,18);
  display.print("Ch 2:");

  if (button_1_val == LOW) // is pressed
  {
    display.setCursor(90,0);
    display.print("ON");
  }
  else
  {
    display.setCursor(85,0);
    display.print("OFF");
  }

  if (button_2_val == LOW) // is pressed
  {
    display.setCursor(90,18);
    display.print("ON");
  }
  else
  {
    display.setCursor(85,18);
    display.print("OFF");
  }

  display.display();  
}

void handle_Buttons_and_Relais()
{
  if (button_1_val == LOW) // is pressed
  {
    digitalWrite(RELAIS_1, LOW); // turn on Relais 1
  }
  else
  {
    digitalWrite(RELAIS_1, HIGH); // turn off Relais 1
  }

  if (button_2_val == LOW) // is pressed
  {
    digitalWrite(RELAIS_2, LOW); // turn on Relais 2
  }
  else
  {
    digitalWrite(RELAIS_2, HIGH); // turn off Relais 2
  }
}



void loop() 
{
  button_1_val = digitalRead(BUTTON_1);
  button_2_val = digitalRead(BUTTON_2);

  handle_Display();
  handle_Buttons_and_Relais();           
               
}


