
#include "Classes.hpp"

// #ifndef STASSID
// #define STASSID "OnePlus 12R"
// #define STAPSK "aymen123"
// #endif
// constexpr unsigned long RefreshData = 1000;
// const char* ssid = STASSID;
// const char* password = STAPSK;


CapteurTemperatureHumidite monCapteur;
BoutonPoussoire monBouton(D5);
ServoMoteur myservo(D3); 
CapteurLumiere capteurLumiere(A0);
EcranLCD myScreen;

// Serveur MonServeur(80);
// //String Message;

// unsigned long Elapsed;
// unsigned long LastUpdate;
// int Compteur=0;


void setup() {
  
  Serial.begin(9600);
  while(!Serial);
 Serial.println("begin...");
  monCapteur.StartUp();
  monBouton.StartUp();
  myservo.StartUp();
  myScreen.StartUp();
  capteurLumiere.StartUp();

  //MonServeur.StartUp(ssid,password);

}

void loop() {
  capteurLumiere.GetLuminosite();

  String m="";
  m+= capteurLumiere.GetLuminosite();
  m+="%";
  myScreen.Afficher(m, 0, 0);
 Serial.println(m);

  if(MonBouton.CheckRisingEdge()){
    
  
  // Serial.print("Temp=");
  // Serial.print(MonCapteur.GetTemperature()); 
  // Serial.println(" °C"); //The unit for  Celsius because original arduino don't support speical symbols
  // Serial.print("Hum = "); 
  // Serial.print(MonCapteur.GetHumidite());
  // Serial.println("%"); 
  // Serial.println();
  // Serial.print("lum=");
  // Serial.print(analogRead(A0));
  // Serial.println();
  // Serveur::Message="J'ai cliqué sur le bouton";
  }
  
  myservo.SetAngle(0);
  // delay(1000);
  // myservo.SetAngle(90);
  // delay(1000);
  // myservo.SetAngle(180);
  // delay(1000);
  // myservo.resetAngle();





// MonServeur.HandleClient();
// MDNS.update();
// Elapsed=millis()-LastUpdate;


// if (Elapsed>RefreshData){
//   Elapsed=0;
//   Compteur++;
//   LastUpdate=millis();
//   String Message= "<html><head><meta http-equiv=\"refresh\" content=\"1\"></head>";
//   Message+=Compteur;
//   Message+="Nous sommes le ";
//   Message+=millis();  
//   Message+="\nla température est de ";
//   Message+=MonCapteur.GetTemperature();
//   Message+=" °C";
//   Message+="\nl'humidité' est de ";
//   Message+=MonCapteur.GetHumidite();
//   Message+=" %";
//   Message+="\nla luminosité est ";
//   if(analogRead(A0)<512) Message+="mauvaise";
//   else Message+="bonne";
//   Serveur::Message=Message;
// }



}


