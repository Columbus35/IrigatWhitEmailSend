#include <WiFi.h>
#include "time.h"
#include <Arduino.h>
#include <ESP_Mail_Client.h>
#include <string>
#include <iostream>
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587
#define AUTHOR_EMAIL "emailAddress"
#define AUTHOR_PASSWORD "emailPassword"
#define SENSOR1 34
#define SENSOR2 25
#define SENSOR3 35
#define SENSOR4 39
#define VALVE1 14
#define VALVE2 26
#define VALVE3 32
#define VALVE4 27
#define PUMP1 33
#define PUMP2 13

const char *ssid = "wifiName";
const char *password = "wifiPassword";

const char *ntpServer = "ntp.lonelybinary.com";
const long gmtOffset_sec = 3600L * 3;
const int daylightOffset_sec = 0;
void connectWiFi();
int previousIndex = 0;
bool checkTime(int minute);
long interval1, interval2, interval3, interval4;
bool stopTime1 = false, stopTime2 = false, stopTime3 = false, stopTime4 = false;
bool emailSent = true;
int sensorValue1, sensorValue2, sensorValue3, sensorValue4;
String emailText = "";
bool checkMoisture1(), checkMoisture2(), checkMoisture3(), checkMoisture4();
void activateValve1(), activateValve2(), activateValve3(), activateValve4();
void runDuration1(long interval), runDuration2(long interval), runDuration3(long interval), runDuration4(long interval);
void controlPump1(), controlPump2();
void sendEmail();
String generateEmailText();

SMTPSession smtp;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SENSOR1, INPUT);
    pinMode(SENSOR2, INPUT);
    pinMode(SENSOR3, INPUT);
    pinMode(SENSOR4, INPUT);
    pinMode(VALVE1, OUTPUT);
    pinMode(VALVE2, OUTPUT);
    pinMode(VALVE3, OUTPUT);
    pinMode(VALVE4, OUTPUT);
    pinMode(PUMP1, OUTPUT);
    pinMode(PUMP2, OUTPUT);
    digitalWrite(VALVE1, HIGH);
    digitalWrite(VALVE2, HIGH);
    digitalWrite(VALVE3, HIGH);
    digitalWrite(VALVE4, HIGH);
    digitalWrite(PUMP1, HIGH);
    digitalWrite(PUMP2, HIGH);
    connectWiFi();
}

void loop() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    int minute = timeinfo.tm_min;
    bool lowMoisture1 = checkMoisture1();
    bool lowMoisture2 = checkMoisture2();
    bool lowMoisture3 = checkMoisture3();
    bool lowMoisture4 = checkMoisture4();
    emailText = generateEmailText();
    bool timeCheck = checkTime(minute);
    
    if (timeCheck && lowMoisture1) {
        interval1 = millis() + 120000;
        stopTime1 = true;
    }
    if (timeCheck && lowMoisture2) {
        interval2 = millis() + 120000;
        stopTime2 = true;
    }
    if (timeCheck && lowMoisture3) {
        interval3 = millis() + 180000;
        stopTime3 = true;
    }
    if (timeCheck && lowMoisture4) {
        interval4 = millis() + 270000;
        stopTime4 = true;
    }
    activateValve1();
    activateValve2();
    activateValve3();
    activateValve4();
    controlPump1();
    controlPump2();
    if (emailSent) {
        sendEmail();
    }
}

bool checkTime(int minute) {
    bool flag = false;
    int index1 = 15;
    int index2 = 22;
    if (index1 == minute && previousIndex == index1 - 1) {
        flag = true;
        previousIndex = minute;
    } else if (index2 == minute && previousIndex == index2 - 1) {
        flag = true;
        previousIndex = minute;
    } else {
        flag = false;
        previousIndex = minute;
    }
    return flag;
}

bool checkMoisture1() {
    bool low = false;
    int sensorUpperLimit = 2590;
    int sensorLowerLimit = 1100;
    int sensorValue = analogRead(SENSOR1);
    sensorValue = constrain(sensorValue, sensorLowerLimit, sensorUpperLimit);
    sensorValue1 = map(sensorValue, sensorLowerLimit, sensorUpperLimit, 100, 0);
    if (sensorValue1 < 86) {
        low = true;
    }
    return low;
}

void activateValve1() {
    if (stopTime1) {
        runDuration1(interval1);
    }
}

void controlPump1() {
    if (stopTime4 || stopTime2) {
        digitalWrite(PUMP1, LOW);
    } else if (!stopTime4 && !stopTime2) {
        digitalWrite(PUMP1, HIGH);
    }
}

void runDuration1(long interval) {
    if (millis() < interval) {
        digitalWrite(VALVE1, LOW);
    } else {
        digitalWrite(VALVE1, HIGH);
        stopTime1 = false;
    }
}

void connectWiFi() {
    boolean ledState = false;
    Serial.print("Connecting to WiFi network ");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }
    Serial.println("WiFi connected");
}

String generateEmailText() {
    String text = "Sensor 1 value: " + String(sensorValue1) +
                  "\nSensor 2 value: " + String(sensorValue2) +
                  "\nSensor 3 value: " + String(sensorValue3) +
                  "\nSensor 4 value: " + String(sensorValue4);
    return text;
}

void sendEmail() {
    Serial.print("Connecting to WiFi for email...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected.");
    SMTP_Message message;
    message.sender.name = "Irrigation System";
    message.sender.email = "your email address";
    message.subject = "Sensor Values";
    message.addRecipient("User", "user@example.com");
    message.text.content = emailText.c_str();
    message.text.charSet = "utf-8";
    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("Error sending Email");
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    emailSent = false;
}
