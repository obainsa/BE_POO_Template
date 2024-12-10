#include "Classes.hpp"

String Serveur::Message = "vide";


class error{
public: int index;
error(int i): index(i) {}
};

//Device
Device::Device(){CompteurDevice++;}
int Device::CompteurDevice = 0;

//Capteur Temp

CapteurTemperatureHumidite::CapteurTemperatureHumidite():Device(),CapteurTempHum(),Temperature(),Humidite(){;}


bool CapteurTemperatureHumidite::StartUp(){
    return CapteurTempHum.begin();
}


float CapteurTemperatureHumidite::GetTemperature(){
    this->Temperature=CapteurTempHum.getTemperature();
    // if(Temperature==NAN) throw error(1);
    return this->Temperature;
}

float CapteurTemperatureHumidite::GetHumidite(){
    this->Humidite=CapteurTempHum.getHumidity();
    //if(Temperature==NAN) throw error(2);
    return this->Humidite;
}

//BOUTON POUSSOIRE 

BoutonPoussoire::BoutonPoussoire(int pin):Device(),pin(pin),State(){;}



bool BoutonPoussoire::StartUp(){
  pinMode(pin, INPUT);
  return 0;
}

bool BoutonPoussoire::CheckRisingEdge(){
  if(State^digitalRead(pin) and State==0){this->State=digitalRead(pin);return 1;}
  this->State=digitalRead(pin);
  return 0;
}

//Capteur Lumiere

CapteurLumiere::CapteurLumiere(int pin):Device(),pin(pin),luminosite(){;}

bool CapteurLumiere::StartUp(){
  pinMode(pin, INPUT);
  return 1;
}

float CapteurLumiere::GetLuminosite(){
  luminosite=analogRead(pin)/(2^10);
  luminosite=luminosite*100;
  luminosite=luminosite/128;
  return luminosite;
}


//Servo

ServoMoteur::ServoMoteur(int pin):Device(),servo(),pin(pin),angle(){;}

bool ServoMoteur::StartUp(){
  servo.attach(pin);
  return 1;
}
void ServoMoteur::SetAngle(int angle){
    this->angle = angle;
    servo.write(this->angle);
}
float ServoMoteur::GetAngle(){
    return this->angle;
}
void ServoMoteur::resetAngle(){
    this->angle = 0;
    servo.write(this->angle);
}


// Serveur

Serveur::Serveur(int Port):server(Port),ssid(),password(),IP(),Port(Port){;}

void Serveur::reset(){;}

bool Serveur::StartUp(const String& ssid, const String& password) {
    constexpr unsigned long WIFI_TIMEOUT_MS = 20000;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
        delay(500); // Avoid busy-waiting if possible
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        this->IP = WiFi.localIP();

        if (!MDNS.begin("Station-Meteo")) {
            Serial.println("Error starting mDNS");
        }

        server.on("/", [this]() {
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Live Sensor Data</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 0;
                    background-color: #f4f4f9;
                    color: #333;
                }
                header {
                    background: #007bff;
                    color: white;
                    padding: 10px 20px;
                    text-align: center;
                    font-size: 24px;
                    font-weight: bold;
                }
                main {
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 80vh;
                }
                .card {
                    background: white;
                    padding: 20px;
                    border-radius: 8px;
                    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
                    text-align: center;
                    max-width: 400px;
                    width: 100%;
                }
                .card h1 {
                    font-size: 48px;
                    color: #007bff;
                    margin: 0 0 10px;
                }
                .card p {
                    font-size: 18px;
                    margin: 10px 0;
                }
                .status {
                    font-weight: bold;
                    color: #28a745;
                }
                .error {
                    color: #dc3545;
                }
            </style>
            <script>
                function fetchData() {
                    fetch('/update')
                        .then(response => response.text())
                        .then(data => {
                            const lines = data.split('\\n');
                            document.getElementById('compteur').innerText = lines[0];
                            document.getElementById('time').innerText = lines[1];
                            document.getElementById('temperature').innerText = lines[2];
                            document.getElementById('humidity').innerText = lines[3];
                            document.getElementById('light').innerText = lines[4];
                        })
                        .catch(err => {
                            console.error(err);
                            document.getElementById('status').innerText = "Error fetching data";
                            document.getElementById('status').className = 'error';
                        });
                }
                setInterval(fetchData, 1000); // Fetch updates every second
            </script>
        </head>
        <body>
            <header>Station Météo</header>
            <main>
                <div class="card">
                    <p id="status" class="status">Live Data Connected</p>
                    <h1 id="compteur">0</h1>
                    <p id="time">Time: Loading...</p>
                    <p id="temperature">Temperature: Loading...</p>
                    <p id="humidity">Humidity: Loading...</p>
                    <p id="light">Light: Loading...</p>
                </div>
            </main>
        </body>
        </html>
    )rawliteral";
    server.send(200, "text/html", html);
});

        Update("/Meteo");

        server.begin();
        Serial.println("HTTP server started");
        return true;
    } else {
        Serial.println("\nWi-Fi connection FAILED");
        return false;
    }
}


void Serveur::Write(const String& Message){
  server.send(200,"text/html",Message);
}

void Serveur::Update(const String& URL) {
    server.on(URL, [this]() {
      
        this->Write(Serveur::Message);
    });
}

void Serveur::HandleClient(){
  server.handleClient();
}

// EcranLCD::EcranLCD:lcd(),colonne(),ligne(),message(){;}

void EcranLCD::StartUp(){
    lcd.begin(16, 2); //Initialisation du lcd de 16 colonnes et 2 lignes
    lcd.setRGB(255,0,0);
    lcd.home(); // positionnez le cursuer dans le coin supérieur gauche de l'écran 
    lcd.clear(); //effacer l'affichage
}

void EcranLCD::Afficher(String m, int c, int l){
    this->colonne = c;
    this->ligne = l; 
    lcd.setCursor(c, l);  // déplacer le curseur à (0, 0)
    lcd.print(m);
}
void EcranLCD::Reset(){
    lcd.clear(); //effacer l'affichage
}
