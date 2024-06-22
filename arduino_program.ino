#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>
#include <TinyGPS++.h>

// MPU6050 object
MPU6050 mpu;

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // I2C address for the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// GPS settings
TinyGPSPlus gps;
HardwareSerial ss(1);  // UART1 on the ESP32
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17

// Pin definitions
const int buzzerPin = 2;
const int buttonPin = 15;  // Digital pin connected to the button

// Variables
bool accidentDetected = false;
unsigned long accidentTime = 0;
const unsigned long alertDelay = 20000;  // 20 seconds
unsigned long displayTime = 0;
unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 50;  // 50 milliseconds debounce delay

// WiFi credentials
const char* ssid = "your_ssid";
const char* password = "your_password";

// Twilio credentials
const char* twilioAccountSid = "your_twilio_account_sid";
const char* twilioAuthToken = "your_twilio_auth_token";
const char* fromNumber = "your_twilio_phone_number";
const char* toNumbers[] = {"number1", "number2", "number3"};  // Add multiple numbers here
const int numToNumbers = sizeof(toNumbers) / sizeof(toNumbers[0]);

// Firebase settings
const char* firebaseHost = "your_firebase_host";
const char* firebaseAuth = "your_firebase_auth_key";

// NTP server to get time
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;  // 5:30 hours ahead of UTC for IST
const int daylightOffset_sec = 0;  // No daylight saving time in IST

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // Initialize I2C with specified SDA and SCL pins
  ss.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);  // Initialize GPS serial communication

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);  // Loop forever if OLED initialization fails
  }
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the display buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System Initializing...");
  display.display();
  delay(2000);

  // Initialize MPU6050 with specified SDA and SCL pins
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("MPU6050 fail");
    display.display();
    while (1);  // Loop forever if MPU6050 initialization fails
  }
  Serial.println("MPU6050 initialized");

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Using internal pull-up resistor

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Connecting");
    display.setCursor(0, 20);
    display.println("to WiFi...");
    display.display();
  }
  initializeTime();
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float ax_g = ax / 16384.0; // Convert to g's
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  Serial.print("AccX: "); Serial.print(ax_g);
  Serial.print(" | AccY: "); Serial.print(ay_g);
  Serial.print(" | AccZ: "); Serial.println(az_g);

  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  if (accidentDetected) {
    unsigned long timeElapsed = millis() - accidentTime;
    unsigned long remainingTime = alertDelay - timeElapsed;

    // Check if the button is pressed to cancel the alert
    if (digitalRead(buttonPin) == LOW && (millis() - lastButtonPressTime) > debounceDelay) {  // Button is active LOW due to INPUT_PULLUP
      lastButtonPressTime = millis();
      accidentDetected = false;
      displayAlert("Alert     Canceled!");
      updateFirebase("Alert Canceled");
      Serial.println("Accident alert canceled");
      delay(1000); // Display cancel message for 1 second
    } else if (timeElapsed < alertDelay) {
      displayCountdown(remainingTime / 1000);
    } else {
      sendSMSAlerts();
      accidentDetected = false;
      displayAlert("Alert     Sent!");
      updateFirebase("Alert Sent to the mobile numbers: " + getToNumbersString());
      Serial.println("Accident alert sent");
      delay(1000); // Display sent message for 1 second
    }
  } else if (abs(ax_g) > 1.5 || abs(ay_g) > 1.5 || abs(az_g) > 1.5) {
    displayAlert("Accident  Detected!");
    triggerAlarm();
    accidentDetected = true;
    accidentTime = millis();
    Serial.println("Accident Detected");
  } else {
    // Display accelerometer values
    displayAccelerometerValues(ax_g, ay_g, az_g);
  }
}

void initializeTime() {
  Serial.println("Initializing time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  int retries = 10;
  while (retries-- > 0) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      Serial.println("Time obtained successfully");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Time obtained");
      display.display();
      delay(2000);
      return;
    }
    Serial.println("Failed to obtain time, retrying...");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Failed to obtain time");
    display.display();
    delay(2000);
  }

  Serial.println("Failed to obtain time after multiple attempts");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Failed to obtain time");
  display.display();
  delay(2000);
}

void triggerAlarm() {
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}

void sendSMSAlerts() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("https://api.twilio.com/2010-04-01/Accounts/") + twilioAccountSid + "/Messages.json";
    String auth = base64::encode(String(twilioAccountSid) + ":" + twilioAuthToken);

    for (int i = 0; i < numToNumbers; i++) {
      String payload = "To=" + String(toNumbers[i]) + "&From=" + String(fromNumber) + "&Body=" + getMessage();

      http.begin(url);
      http.addHeader("Authorization", "Basic " + auth);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      int httpCode = http.POST(payload);

      if (httpCode > 0) {
        String response = http.getString();
        Serial.println(httpCode);
        Serial.println(response);
      } else {
        Serial.println("Error on sending POST");
      }

      http.end();
      delay(1000);  // Delay to avoid hitting rate limits
    }
  }
}
void updateFirebase(String status) {
  // Get current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char timeStringBuff[50]; 
  strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d %H:%M:%S", &timeinfo);
  String currentTime = String(timeStringBuff);

  String path = String(firebaseHost) + "/accidents.json?auth=" + firebaseAuth;
  String payload = "{\"location_link\":\"" + getLocationLink() + "\",\"time\":\"" + currentTime + "\",\"status\":\"" + status + "\"}";

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(path);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.println("Error on sending POST");
    }

    http.end();
  }
}

String getLocationLink() {
  if (gps.location.isValid()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    String locationLink = "http://maps.google.com/?q=" + String(latitude, 6) + "," + String(longitude, 6);
    return locationLink;
  } else {
    return "Location not available";
  }
}

String getMessage() {
  String message = "Accident Detected! Location: " + getLocationLink();
  return message;
}

void displayAlert(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

void displayCountdown(int seconds) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Alert in:");
  display.setCursor(0, 20);
  display.print(seconds);
  display.println(" sec");
  display.display();
}

void displayAccelerometerValues(float ax, float ay, float az) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("AccX: "); display.println(ax);
  display.print("AccY: "); display.println(ay);
  display.print("AccZ: "); display.println(az);
  display.display();
}

String getToNumbersString() {
  String numbers = "";
  for (int i = 0; i < numToNumbers; i++) {
    if (i > 0) {
      numbers += ", ";
    }
    numbers += toNumbers[i];
  }
  return numbers;
}
