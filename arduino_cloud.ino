// lcd
#include <Wire.h>
#include "rgb_lcd.h"
#include <WiFiS3.h>

//WIFI Setup
const char ssid[] = "Galaxy-S24";  // change your network SSID (name)
const char pass[] = "awab20121";   // change your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

//LCD Setup
rgb_lcd lcd;
const int colorR = 0;
const int colorG = 0;
const int colorB = 0;

//Temperature Sensor Setup
#define ADC_VREF 5.0  // in volt
#define ADC_RES 1024.0 //2^10
#define PIN_TMP36 A0
double tempC = 0.0; 
double targetTemp = 0.0;
double curr_temp = 0.0;

//moisture Sensor Setup
// int moistSensor = D7;
// int analog_sensor = A5;
// int data;
// int moisture_reading;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  Serial.println("Please upgrade the firmware");

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();

  //LCD Setup
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  // Print the message to the LCD.
  lcd.print("Temp:");
  lcd.setCursor(0, 1); 
  lcd.print("Water:");


  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // read the HTTP request header line by line
    while (client.connected()) {
      if (client.available()) {
        String HTTP_header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (HTTP_header.equals("\r"))  // the end of HTTP request
          break;

        Serial.print("<< ");
        Serial.println(HTTP_header);  // print HTTP request to Serial Monitor
      }
    }
    
    //LCD Display
    lcd.setCursor(6, 0);
    lcd.print(temp_reading());
    // lcd.setCursor(7, 1);
    // lcd.print(soil_reading());
    

    delay(1000);

    // send the HTTP response
    // send the HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body
    // send the HTTP response body
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    client.println("</head>");

    client.println("<p>");

    client.print("Temperature: <span style=\"color: red;\">");

    //Get the temperature
    int temperature = temp_reading();
    //print to the serial monitor
    Serial.println(temperature);


    client.print(temperature, 10); //10 for decimal format
    client.println("&deg;C</span>");

    client.println("</p>");
    client.println("</html>");
    client.flush();


    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
  }
}
int temp_reading(){
    double tempDAC = analogRead(PIN_TMP36);
    tempC = tempDAC * (ADC_VREF / ADC_RES);
    curr_temp = (tempC - .5) * 100;
    return curr_temp;
}
// int soil_reading() {
//     digitalWrite(moistSensor, HIGH);
//     delay(10);
//     moisture_reading = analogRead(analog_sensor);
//     digitalWrite(moistSensor, LOW);
//     return moisture_reading;
// }

void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
