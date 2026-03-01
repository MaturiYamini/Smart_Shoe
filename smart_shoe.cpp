#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
const char* ssid = "smart shoe";
const char* password = "12345678";
#define BOT_TOKEN "8468014203:AAF8A_osBnl1ceiNAsu1sDD-mJXqH1kgV18"
#define CHAT_ID   "6686323461"
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
float flat = 0, flon = 0;
int buz = 33;
int led = 25;
#define TRIGPIN 5
#define ECHOPIN 18
int stepCount = 0;
unsigned long lastStepTime = 0;
bool stepDetected = false;
float lastMagnitude = 0;
const float STEP_THRESHOLD = 9.0; 
const unsigned long STEP_COOLDOWN = 300; 

void updateStepCount(float x, float y) {
  // Calculate magnitude of XY acceleration (ignoring Z-axis for step detection)
  float currentMagnitude = sqrt(x * x + y * y);
  Serial.println(currentMagnitude );
  // Get current time
  unsigned long currentTime = millis();
  
  // Step detection algorithm
  if (currentTime - lastStepTime > STEP_COOLDOWN) {
    // Detect peak in acceleration magnitude
    if (currentMagnitude > STEP_THRESHOLD && lastMagnitude <= STEP_THRESHOLD && !stepDetected) {
      stepCount++;
      stepDetected = true;
      lastStepTime = currentTime;
      Serial.println("Step detected! Total steps: " + String(stepCount));
    }
    
    // Reset step detection when magnitude drops below threshold
    if (currentMagnitude < STEP_THRESHOLD) {
      stepDetected = false;
    }
  }
  
  // Store current magnitude for next comparison
  lastMagnitude = currentMagnitude;
}

void resetStepCounter() {
  stepCount = 0;
  Serial.println("Step counter reset");
}

float getDistance() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  long duration = pulseIn(ECHOPIN, HIGH, 30000);  // 30 ms timeout
  float distance = duration * 0.0343 / 2.0;
  if (distance == 0 || distance > 400) distance = 400; // cap to 400 cm
  return distance;
}

void send_sms(int sts) {
  String msg;
  if (sts == 1)
    msg = "Step count is reached 20 ";
  else if (sts == 2)
    msg = "Object detected ";

  // Uncomment to add GPS coordinates when available
  // msg += "https://www.google.com/maps/search/?api=1&query=" +
  //       String(flat, 6) + "," + String(flon, 6);

  Serial.println("Sending Telegram Alert...");
  if (bot.sendMessage(CHAT_ID, msg, "Markdown")) {
    Serial.println("✅ Telegram message sent.");
  } else {
    Serial.println("❌ Failed to send Telegram message.");
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);  // GPS module typically connected to Serial2
  
  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }
  
  pinMode(led, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  
  digitalWrite(buz, 1);
  digitalWrite(led, LOW);
  
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  
  client.setInsecure(); // for HTTPS access to Telegram
  bot.sendMessage(CHAT_ID, "🟢 Device Started - Monitoring Active", "");
  delay(1000);
}

void loop() {
  delay(200);
  
  // Read GPS data
  read_gps();
  
  // Read ultrasonic distance
  float distance = getDistance();
  
  // Read accelerometer data
  sensors_event_t event;
  accel.getEvent(&event);
  float xval = event.acceleration.x;
  float yval = event.acceleration.y;
  float zval = event.acceleration.z;
  
  // Update step count
  updateStepCount(xval, yval);
  
  if (stepCount > 20) {
    digitalWrite(led, HIGH);
    send_sms(1);
    digitalWrite(led, LOW);
    resetStepCounter();  // Reset after sending alert
  }
  else if (distance < 30) {
    digitalWrite(buz, 0);
    send_sms(2);
    digitalWrite(buz, 1);
  }
  else {
    digitalWrite(buz, 1);
    digitalWrite(led, LOW);
  }
  
  // Optional: Print sensor data for debugging
  Serial.print("Steps: ");
  Serial.print(stepCount);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Accel: ");
  Serial.print(xval);
  Serial.print(", ");
  Serial.print(yval);
  Serial.print(", ");
  Serial.println(zval);
}
