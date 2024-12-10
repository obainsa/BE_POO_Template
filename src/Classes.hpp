#ifndef CLASSES_H
#define CLASSES_H

#include <string.h>
#include <Arduino.h>
#include <Servo.h>
#include "SHT31.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "rgb_lcd.h" 

using namespace std;
#endif 

class Device{
  private:
    static int CompteurDevice;
  public :
    Device();
    virtual bool StartUp()=0;
    static int GetCpt(){return CompteurDevice;};
    
};

class CapteurTemperatureHumidite : public Device{
  private: 
    SHT31 CapteurTempHum;
    float Temperature;
    float Humidite;
  public : 
    CapteurTemperatureHumidite();
    virtual bool StartUp(); // Initialise le capteur
    float GetTemperature();
    float GetHumidite();
};



class CapteurLumiere : public Device {
  private:
    int pin;
    float luminosite;
  public:
    CapteurLumiere(int pin);
    virtual bool StartUp();
    float GetLuminosite();
};

class EcranLCD {
  private:
    rgb_lcd lcd;
    int colonne;
    int ligne;
    String message;
  public : 
    void StartUp();
    void Reset();
    void Afficher(String message, int colonne, int ligne);
};

class BoutonPoussoire : public Device {
  private:
    int pin;
    bool State;
  public:
    BoutonPoussoire(int pin);
    virtual bool StartUp();  //Set the pinmode for object as input
    bool CheckRisingEdge(); // Return 1 for rising edge, 0 otherwise
};


class ServoMoteur: public Device {
  private:
    Servo servo;
    int pin;
    float angle;
  public : 
    ServoMoteur(int pin);
    virtual bool StartUp();
    void resetAngle();
    void SetAngle(int alpha);
    float GetAngle();
};



class Serveur {
  private:
    ESP8266WebServer server;
    String ssid;
    String password;
    IPAddress IP;
    int Port;
  public:
    static String Message;
  
    Serveur(int Port);
    void reset();
    bool StartUp(const String& ssid,const String& password);
    void Write(const String& Message);
    void HandleClient();
    void Update(const String& URL);
};




