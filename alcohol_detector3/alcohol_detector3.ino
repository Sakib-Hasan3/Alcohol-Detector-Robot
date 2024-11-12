#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows 

int mq3_pin1 = A0; // First MQ3 sensor analog pin // Second MQ3 sensor analog pin 
int buzzer_pin = 11; // Buzzer digital pin 
int relay_pin = 6; // Relay module pin to control the motor 
int threshold = 370; // Adjust this threshold value based on your sensors

void setup() 
{ 
  // Set pin modes 
  pinMode(mq3_pin1, INPUT); // Set first MQ3 pin as inpu // Set second MQ3 pin as input 
  pinMode(buzzer_pin, OUTPUT); // Set buzzer pin as output 
  pinMode(relay_pin, OUTPUT); // Set relay pin as output (for motor control) // Initialize Serial communication 
  Serial.begin(9600); 
  Serial.println("Alcohol Sensor and Motor Control Test Starting..."); // Initialize the LCD 
  lcd.init(); // Initialize the LCD 
  lcd.backlight(); // Turn on the LCD backlight 
  lcd.clear(); // Clear any previous data on the LCD 
  lcd.setCursor(0, 0); // Set the cursor to the first row 
  lcd.print("System Starting"); // Display message on the LCD 
  delay(2000); // Wait for 2 seconds 
  lcd.clear(); // Clear the LCD for further use 
}

void loop() 
{ 
  int sensor_value1 = analogRead(mq3_pin1); // Read value from first MQ3 sens 
  // Read value from second MQ3 sensor 
  // Print the sensor readings to the Serial Monitor for debugging 
  Serial.print("MQ3 Sensor 1 Value: "); 
  Serial.println(sensor_value1); 
  // Check if either sensor value exceeds the threshold 
  if (sensor_value1 > threshold) { 
    digitalWrite(buzzer_pin, HIGH); // Turn buzzer on 
    digitalWrite(relay_pin, HIGH); // Turn motor off (assuming relay is active HIGH)
// Print to Serial Monitor 
Serial.println("Buzzer ON - Engine OFF - Alcohol detected!"); 
// Display "Alcohol Detected" on the LCD 
lcd.setCursor(0, 0); // Set cursor to the first row, first column 
lcd.print("Alcohol Detected"); // Display "Alcohol Detected" 
lcd.setCursor(0, 1); // Set cursor to the second row, first column 
lcd.print("Motor OFF "); // Display "Motor OFF" 
}

else 
{ 
  digitalWrite(buzzer_pin, LOW); // Turn buzzer off 
  digitalWrite(relay_pin, LOW); // Turn motor on (assuming relay is active HIGH) 
  // Print to Serial Monitor 
  Serial.println("Buzzer OFF - Engine ON - No alcohol detected."); 
  // Display "No Alcohol" on the LCD 
  lcd.setCursor(0, 0); // Set cursor to the first row, first column 
  lcd.print("No Alcohol "); // Display "No Alcohol" 
  lcd.setCursor(0, 1); // Set cursor to the second row, first column 
  lcd.print("Engine ON "); // Display "Motor ON" 
  }
   delay(500); // Add a delay for better stability 
}


#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h> 
#include <ESP8266HTTPClient.h> 

#define MQ3_PIN A0 // Analog pin for MQ3 alcohol sensor 
const char* ssid = "sakib"; // Wi-Fi SSID 
const char* password = "12345678"; // Wi-Fi password 

// Telegram bot credentials 
const char* botToken = "7684402346:AAGWObm7273kvUFtzlPspJTCL9EnKiBxop4"; // Bot token 
const char* chatID = "5069903325"; // Your chat ID

void setup() 
{ 
  Serial.begin(115200); 
  
  // Initialize MQ3 sensor pin 
  pinMode(MQ3_PIN, INPUT);
// Connect to Wi-Fi 
  Serial.println("Connecting to Wi-Fi..."); 
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.print("."); 
    } 
    
    Serial.println("\nWi-Fi connected!"); 
    Serial.print("IP Address: "); 
    Serial.println(WiFi.localIP()); 
    }


    void loop() 
    { 
      // Read MQ3 sensor value 
      int sensorValue = analogRead(MQ3_PIN); 
      Serial.print("MQ3 Sensor Value: "); 
      Serial.println(sensorValue); 
      
      // Determine message based on alcohol level 
      String message; 
      if (sensorValue > 180) 
      {
         message = "Alert!%20Alcohol%20Detected:%20" + String(sensorValue); 
         Serial.println("Alert! Alcohol Detected"); 
         } else { 
          //message = "Safe!%20Alcohol%20Level:%20" + String(sensorValue); 
          Serial.println("Safe!"); 
          } 
          
          // Prepare to send Telegram message 
          WiFiClientSecure clientSecure; 
          clientSecure.setInsecure(); 
          HTTPClient telegramHttp; 
          
          // Construct Telegram API URL 
          String telegramURL = "https://api.telegram.org/bot"; 
          telegramURL += botToken; 
          telegramURL += "/sendMessage?chat_id="; 
          telegramURL += chatID; 
          telegramURL += "&text=" + message;

          // Send data to Telegram 
          
          if (telegramHttp.begin(clientSecure, telegramURL)) 
          {
            int telegramResponseCode = telegramHttp.GET(); 
            
            if (telegramResponseCode > 0) 
            { 
              Serial.print("Telegram Response code: "); 
              Serial.println(telegramResponseCode); 
            } 
            else 
            { 
              Serial.print("Error code: "); 
              Serial.println(telegramResponseCode); 
              } 
              telegramHttp.end(); 
              } 
              else 
              { 
                Serial.println("Unable to connect to Telegram."); 
                } 
                delay(2000); // Wait before the next loop 
                }


