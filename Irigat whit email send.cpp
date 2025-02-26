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
#define SENZOR 34
#define SENZOR2 25
#define SENZOR3 35
#define SENZOR4 39
#define STARTER 14
#define STARTER2 26
#define STARTER3 32
#define STARTER4 27
#define POMPA 33
#define POMPA2 13

const char *ssid = "wifiName";
const char *password = "wifiPassword";

const char *ntpServer = "ntp.lonelybinary.com";
const long gmtOffset_sec = 3600L * 3;
const int daylightOffset_sec = 0;
void conectWifi();
int indexvechi = 0;
bool fls1(int min);
long interval;
long interval2;
long interval3;
long interval4;
bool stoptime = false;
bool stoptime2 = false;
bool stoptime3 = false;
bool stoptime4 = false;
bool email = true;
int sensorValue;
int sensorValue2;
int sensorValue3;
int sensorValue4;
String textEmail = "";
bool umiditate();
bool umiditate2();
bool umiditate3();
bool umiditate4();
void startValva();
void startValva2();
void startValva3();
void startValva4();
void durata(long interval);
void durata2(long interval2);
void durata3(long interval3);
void durata4(long interval4);
void pompa1();
void pompa2();
void sendEmail();
String genText();

SMTPSession smtp;

void setup(){
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SENZOR, INPUT);
    pinMode(SENZOR2, INPUT);
    pinMode(SENZOR3, INPUT);
    pinMode(SENZOR4, INPUT);
    pinMode(STARTER, OUTPUT);
    pinMode(STARTER2, OUTPUT);
    pinMode(STARTER3, OUTPUT);
    pinMode(STARTER4, OUTPUT);
    pinMode(POMPA, OUTPUT);
    pinMode(POMPA2, OUTPUT);
    digitalWrite(STARTER, HIGH);
    digitalWrite(STARTER2, HIGH);
    digitalWrite(STARTER3, HIGH);
    digitalWrite(STARTER4, HIGH);
    digitalWrite(POMPA, HIGH);
    digitalWrite(POMPA2, HIGH);
    conectWifi();
  
}

void loop()
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    int hour = timeinfo.tm_hour;
    int min = timeinfo.tm_hour;
    bool scazut = umiditate();
    bool scazut2 = umiditate2();
    bool scazut3 = umiditate3();
    bool scazut4 = umiditate4();
    textEmail = genText();
    bool ceas = fls1(min);
    if((ceas) && (scazut)){
        interval = millis() + 120000;
        stoptime = true;
    }
    if((ceas) && (scazut2)){
        interval2 = millis() + 120000;
        stoptime2 = true;
    }
     if((ceas) && (scazut3)){
        interval3 = millis() + 180000;
        stoptime3 = true;
    }
     if((ceas) && (scazut4)){
        interval4 = millis() + 270000;
        stoptime4 = true;
    }
    startValva();
    startValva2();
    startValva3();
    startValva4();
    pompa1();
    pompa2();
    if(email){
     sendEmail();
    }
}

bool fls1(int min){
    bool flag1 = false;
    int index = 15;
    int index2 = 22;
    if(index == min && indexvechi == index -1){
        flag1 = true;
        indexvechi = min;
    }
    else if(index2 == min && indexvechi == index2 -1){
        flag1 = true;
        indexvechi = min;
    }
    else{flag1 = false;
         indexvechi = min;
    }
    return flag1;
}

bool umiditate(){
    bool scazut = false;
    int sensor1UpperLimit = 2590;
    int sensor1LowerLimit = 1100;
    int sensor1Value = analogRead(SENZOR);
    if (sensor1Value >= sensor1UpperLimit) sensor1Value =sensor1UpperLimit;
  else if (sensor1Value <= sensor1LowerLimit) sensor1Value = sensor1LowerLimit;

  sensorValue = map(sensor1Value, sensor1LowerLimit, sensor1UpperLimit, 100, 0);
  if(sensorValue < 86){
       scazut = true;
  }
  return scazut;
}

bool umiditate2(){
    bool scazut2 = false;
    int sensor2UpperLimit = 2550;
    int sensor2LowerLimit = 1040;
    int sensor2Value = analogRead(SENZOR2);
    if (sensor2Value >= sensor2UpperLimit) sensor2Value =sensor2UpperLimit;
  else if (sensor2Value <= sensor2LowerLimit) sensor2Value = sensor2LowerLimit;
  sensorValue2 = map(sensor2Value, sensor2LowerLimit, sensor2UpperLimit, 100, 0);
  if(sensorValue2 < 71){
       scazut2 = true;
  }
  return scazut2;
}

void startValva(){
    if(stoptime){
       durata(interval); 
    }
}

void startValva2(){
    if(stoptime2){ 
       durata2(interval2); 
    }
}

void pompa1(){
    if((stoptime4) || (stoptime2)){
        digitalWrite(POMPA, LOW); 
    }
    else if ((!stoptime4) && (!stoptime2)){
        digitalWrite(POMPA, HIGH);
    }
}

void durata(long interval) {
    if (millis() < interval) {
       digitalWrite(STARTER, LOW); 
    }
    else{
        digitalWrite(STARTER, HIGH);
        stoptime = false;
    } 
}

void durata2(long interval2) {
    if (millis() < interval2) {
       digitalWrite(STARTER2, LOW); 
    }
    else{
        digitalWrite(STARTER2, HIGH);
        stoptime2 = false;
    }
}

bool umiditate3(){
    bool scazut3 = false;
    int sensor3UpperLimit = 2590;
    int sensor3LowerLimit = 1060;
    int sensor3Value = analogRead(SENZOR3);
    if (sensor3Value >= sensor3UpperLimit) sensor3Value =sensor3UpperLimit;
  else if (sensor3Value <= sensor3LowerLimit) sensor3Value = sensor3LowerLimit;
  sensorValue3 = map(sensor3Value, sensor3LowerLimit, sensor3UpperLimit, 100, 0);
  if(sensorValue3 < 76){
       scazut3 = true;
  }
  return scazut3;
}

bool umiditate4(){
    bool scazut4 = false;
    int sensor4UpperLimit = 2550;
    int sensor4LowerLimit = 1070;
    int sensor4Value = analogRead(SENZOR4);
    if (sensor4Value >= sensor4UpperLimit) sensor4Value =sensor4UpperLimit;
  else if (sensor4Value <= sensor4LowerLimit) sensor4Value = sensor4LowerLimit;

  sensorValue4 = map(sensor4Value, sensor4LowerLimit, sensor4UpperLimit, 100, 0);
  if(sensorValue4 < 86){
       scazut4 = true;
  }
  return scazut4;
}

void startValva3(){
    if(stoptime3){
       durata3(interval3); 
    }
}

void startValva4(){
    if(stoptime4){ 
       durata4(interval4); 
    }
}

void pompa2(){
    if((stoptime) || (stoptime3)){
        digitalWrite(POMPA2, LOW); 
    }
    else if ((!stoptime) && (!stoptime3)){
        digitalWrite(POMPA2, HIGH);
    }
}

void durata3(long interval3) {
    if (millis() < interval3) {
       digitalWrite(STARTER3, LOW); 
    }
    else{
        digitalWrite(STARTER3, HIGH);
        stoptime3 = false;
    } 
}

void durata4(long interval4) {
    if (millis() < interval4) {
       digitalWrite(STARTER4, LOW); 
    }
    else{
        digitalWrite(STARTER4, HIGH);
        stoptime4 = false;
    }
}
void conectWifi(){
      boolean ledState = false;

    Serial.print ("Connecting to WiFi network ");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }
    Serial.println("");

    /*
        Sync time with NTP server and update ESP32 RTC
        getLocalTime() return false if time is not set
    */
    Serial.print("Syncing time with NTP server ");
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo))
    {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        
        Serial.print(".");
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }

    Serial.println("");
    digitalWrite(LED_BUILTIN, true);
    // disconnect WiFi
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

String genText(){
    String text1 = "Valoarea senzorului 1 este de: ";
    String text2 = "\n Valoarea senzorului 2 este de: ";
    String text3 = "\n Valoarea senzorului 3 este de: ";
    String text4 = "\n Valoarea senzorului 4 este de: ";
    
    String text = text1 + sensorValue +text2 + sensorValue2 + text3 + sensorValue3 + text4 + sensorValue4;
      return text;

}

void sendEmail(){
    boolean ledState = false;

    Serial.print ("Connecting to WiFi network ");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }
    Serial.println("");
    SMTPSession smtp;
      //Debug Meldungen anzeigen
  smtp.debug(1);


  //Mail Session
  ESP_Mail_Session session;

  //Serverdaten setzen
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "smtp.gmail.com";

  //Wenn keine Verbindung aufgebaut werden konnte soll die Funktion verlassen werden.
  if (!smtp.connect(&session)){
    return;
  }

  //Aufbau der E-Mail
  SMTP_Message message;
  //Im Header kann man recht einfach dem Absender Faken
  message.sender.name = "Irigatie"; //steht bei "gesendet von"
  message.sender.email = "esp0683@gmail.com"; //der Absender (oder eine Fake E-Mail)
  message.subject = "Valorile senzorilor"; //der Betreff der E-Mail
  message.addRecipient("Levei Edmond", "levei_edmond@yahoo.com"); //der Empfänger

  //Aufbau des Contents der E-Mail
  String textMsg = textEmail;
  message.text.content = textMsg.c_str();
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  //Eine eindeutige ID welche die Mail kennzeichnet
  //zwischen den spitzen Klammern kann ein Wert xyz@domain.com eingetragen werden
  message.addHeader("Message-ID: <levei_edmond@yahoo.com>");

  //Absenden der E-Mail
  if (!MailClient.sendMail(&smtp, &message)){
    //Im Fehlerfall wird der Grund auf der seriellen Schnittstelle ausgegeben.
    Serial.println("Error sending Email, ");
  }

  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  email = false;
}


