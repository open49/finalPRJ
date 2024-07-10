#define BLYNK_TEMPLATE_ID "TMPL6ZRK0mIGD"
#define BLYNK_TEMPLATE_NAME "sunlight"
#define BLYNK_AUTH_TOKEN "atgHd4B9IDIOFBYY1o67d5q75tUKQWFP"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

Servo servo1;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7*3600); // Khởi tạo NTPClient với máy chủ NTP và offset múi giờ GMT+7

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "TuanNA";
char pass[] = "88888888";


bool autoMode = false; // Chế độ mặc định là manual

BLYNK_WRITE(V0)
{
  int s0 = param.asInt(); 
  servo1.write(s0);
  Blynk.virtualWrite(V2, s0);
}

BLYNK_WRITE(V1) // Nút chức năng trên Blynk để chọn chế độ tự động hoặc thủ công
{
  autoMode = param.asInt(); // Nếu giá trị của nút là 1, chế độ tự động, nếu là 0, chế độ thủ công
}

void setServoAngle() {
  if(autoMode){

  int currentHour = timeClient.getHours();
  int servoAngle;

  if (currentHour >= 6 && currentHour < 7) {
    servoAngle = 0;
  } else if (currentHour >= 7 && currentHour < 8) {
    servoAngle = 15;
  } else if (currentHour >= 8 && currentHour < 9) {
    servoAngle = 30;
  } else if (currentHour >= 9 && currentHour < 10) {
    servoAngle = 45;
  } else if (currentHour >= 10 && currentHour < 11) {
    servoAngle = 60;
  } else if (currentHour >= 11 && currentHour < 12) {
    servoAngle = 75;
  } else if (currentHour >= 12 && currentHour < 13) {
    servoAngle = 90;
  } else if (currentHour >= 13 && currentHour < 14) {
    servoAngle = 105;
  } else if (currentHour >= 14 && currentHour < 15) {
    servoAngle = 120;
  } else if (currentHour >= 15 && currentHour < 16) {
    servoAngle = 135;
  } else if (currentHour >= 16 && currentHour < 17) {
    servoAngle = 150;
  } else if (currentHour >= 17 && currentHour < 18) {
    servoAngle = 165;
  } else {
    servoAngle = 180;
  }

  servo1.write(servoAngle);
  }
}

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  timeClient.begin();
  servo1.attach(D2); //Connect to Pin D2 in NodeMCU
  Blynk.begin(auth, ssid, pass);
}

void loop() 
{
  Blynk.run();
  
  timeClient.update();

  if (timeClient.getMinutes() == 0 && timeClient.getSeconds() == 0) {
    Serial.println("Synchronizing time...");
    timeClient.forceUpdate();
  }

  setServoAngle();

  // In ra thời gian hiện tại lên Serial Monitor
  Serial.print("Current time: ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  if (timeClient.getMinutes() < 10) {
    Serial.print("0");
  }
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  if (timeClient.getSeconds() < 10) {
    Serial.print("0");
  }
  Serial.println(timeClient.getSeconds());
  delay(1000); // Đợi 1 giây trước khi kiểm tra lại giờ
}


