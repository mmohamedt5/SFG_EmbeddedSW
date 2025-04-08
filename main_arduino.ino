#include <rgb_lcd.h>

// lcd
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

const int colorR = 0;
const int colorG = 0;
const int colorB = 0;

//For the Temperature Sensor
#define ADC_VREF 5.0  // in volt
#define ADC_RES 1024.0 //2^10
#define PIN_TMP36 A0
double tempC = 0.0; 
double targetTemp = 0.0;
double curr_temp = 0.0;

//moisture Sensor
int moistSensor = D7;
int analog_sensor = A5;
int data;
int moisture_reading;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  lcd.setRGB(colorR, colorG, colorB);
  
  // Print a message to the LCD.
  lcd.print("Temp:");
  
  lcd.setCursor(0, 1);
  
  lcd.print("Water:");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(6, 0);
  // print the number of seconds since reset:
  lcd.print(temp_reading());
  Serial.println(temp_reading());

  //Set the LCD and get the moisture reading
  lcd.setCursor(7, 1);
  lcd.print(soil_reading());
    

  delay(1000);
  
}
/*
* This function gets the temperature reading in celcius using a function
*/
int temp_reading(){
  double tempDAC = analogRead(PIN_TMP36);
  tempC = tempDAC * (ADC_VREF / ADC_RES);
  curr_temp = (tempC - .5) * 100;
  return curr_temp;
}
/*
* This function gets the soil moisture
*/
int soil_reading() {
    digitalWrite(moistSensor, HIGH);
    delay(10);
    moisture_reading = analogRead(analog_sensor);
    digitalWrite(moistSensor, LOW);
    return moisture_reading;
}


