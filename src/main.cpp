#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SSD1306.h"
#include "htmlPages.h"

#ifndef STASSID
#define STASSID "YOUR SSID"
#define STAPSK "YOUR PASSWD"
#endif


const char *ssid = STASSID;
const char *password = STAPSK;

//--------------------------------------------> SEU IP FIXO E SUAS CONFIGURAÇÕES DE REDE

IPAddress ip(192, 168, 0, 50);
IPAddress gateaway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

//--------------------------------------------> DECLARANDO PINOS DE SAÍDA DE ENERGIA

#define led LED_BUILTIN
#define plug1 D0
#define plug2 D5
#define plug3 D6
#define plug4 D7

//--------------------------------------------> INICIALIZANDO O DISPLAY OLED

SSD1306Wire display(0x3c, SDA, SCL);

//--------------------------------------------> MACROS -> FUNÇÕES QUE FAZEM A ANIMAÇÃO DO DISPLAY OLED 


#define X0 64    // px
#define Y0 40    // px
#define raio 18  // px
#define alpha0  // Fase Inicial
#define period 3 * PI
#define passo 3 * PI / 120
#define X(t, phase) X0 + raio *cos(pow(t, 2) + phase)
#define Y(t, phase) Y0 + raio *sin(pow(t, 2) + phase)
#define P(t, phase) X0 + raio *cos(18 * pow(PI, 2) - pow((t - 6 * PI), 2) - PI / 2 + phase)
#define Q(t, phase) Y0 + raio *sin(18 * pow(PI, 2) - pow((t - 6 * PI), 2) - PI / 2 + phase)

//-------------------------------------------->  DECLARAÇÂO DAS FUNCÔES

//Wifi
void wifiStatus();

//Rotas
void routes();

// Funções Controladoras
void handleRoot();
void verifyStates();
void setState();
void update();

// Display Oled

void oledConnecting(String text);
void oledSpinning(String text, String message);
void oledWifiConfigurations();
void ledBlink();
void oledStatePlugs(byte state1, byte state2, byte state3, byte state4);
void saveState(int address, byte state);

//------------------------------------------------------------>  SETUP E LOOP (Principal)
void setup(void)
{

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(plug1, OUTPUT);
  pinMode(plug2, OUTPUT);
  pinMode(plug3, OUTPUT);
  pinMode(plug4, OUTPUT);

  EEPROM.begin(4);
  
  digitalWrite(plug1, EEPROM.read(0));
  digitalWrite(plug2, EEPROM.read(1));
  digitalWrite(plug3, EEPROM.read(2));
  digitalWrite(plug4, EEPROM.read(3));

  display.init();
  display.flipScreenVertically();

  //Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateaway, subnet);

  oledConnecting("CONNECTING");
  wifiStatus();
  
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  routes();

  server.begin();
  //Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
  if ( WiFi.status() != WL_CONNECTED ){
    wifiStatus();
  };
}

// ------------------------------------------------> ROTAS

void routes()
{

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_page);
  });

  server.on("/states", HTTP_GET, [] {
    verifyStates();
  });

  server.on("/toogle", HTTP_GET, [] {
    setState();
  });

  server.on("/firmware", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", update_page);
  });

  server.on(
      "/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart(); }, []() { update(); });
}

//----------------------------------------------------> FUNÇÔES CONTROLLERS

String oledStateVerify(byte state)
{
  if (state == 0)
  {
    return "ON";
  }
  else
  {
    return "OFF";
  }
}

void verifyStates()
{
  StaticJsonDocument<200> doc;
  JsonArray state = doc.createNestedArray("statePlugs");

  byte statePlug1 =  digitalRead(plug1),
       statePlug2 =  digitalRead(plug2),
       statePlug3 =  digitalRead(plug3),
       statePlug4 =  digitalRead(plug4);

  state.add(statePlug1);
  state.add(statePlug2);
  state.add(statePlug3);
  state.add(statePlug4);

  String response;
  serializeJson(doc, response);

  server.send(200, "text/plain", response);

  oledStatePlugs(statePlug1, statePlug2, statePlug3, statePlug4);
  
  saveState(0,statePlug1);
  saveState(1,statePlug2);
  saveState(2,statePlug3);
  saveState(3,statePlug4);

};

void setState()
{

  int plugNumber = server.arg("plug").toInt();

  if (plugNumber > 0 && plugNumber < 5)
  {

    int gpio;

    switch (plugNumber)
    {
    case 1:
      gpio = plug1;
      break;
    case 2:
      gpio = plug2;
      break;
    case 3:
      gpio = plug3;
      break;
    case 4:
      gpio = plug4;
      break;
    default:
      break;
    }
    Serial.println(gpio);

    digitalWrite(gpio, !digitalRead(gpio));

    verifyStates();
  }
  else
  {
    server.send(404, "text/plain", "GPIO NOT FOUND GUY!");
  };
};

void update()
{

  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    Serial.printf("Update: %s\n", upload.filename.c_str());
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace))
    { //start with max available size
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
    {
      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (Update.end(true))
    { //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
    }
    else
    {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  }
  yield();
}

//------------------------------------------------> ANIMAÇÕES DO DISPLAY OLED

void oledConnecting(String text)
{
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  for (int i = -64; i <= 64; i++)
  {
    display.drawString(i, 0, text);
    display.display();
    delay(10);
    display.clear();
  }
}

void oledSpinning(String simbol, String message)
{
  display.setFont(ArialMT_Plain_16);

  for (float t = 0; t <= period; t += passo)
  {
    display.drawString(X(t, 0), Y(t, 0), simbol);
    display.drawString(64, 0, message);
    display.drawString(X(t, -PI / 6), Y(t, -PI / 6), simbol);
    display.drawString(X(t, -PI / 3), Y(t, -PI / 3), simbol);
    display.display();
    delay(10);
    display.clear();
  }

  for (float t = period; t <= 2 * period; t += passo)
  {
    display.drawString(P(t, 0), Q(t, 0), simbol);
    display.drawString(64, 0, message);
    display.drawString(P(t, -PI / 6), Q(t, -PI / 6) + PI / 2, simbol);
    display.drawString(P(t, -PI / 3), Q(t, -PI / 3) + PI / 2, simbol);

    display.display();
    delay(10);

    display.clear();
  }
  ledBlink();
}

void oledWifiConfigurations()
{
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "CONNECTED!");
  display.display();
  delay(2000);
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 22, "SSID:");
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 40, String(ssid));
  display.display();
  delay(3000);
  display.setFont(ArialMT_Plain_16);
  display.clear();
  display.drawString(64, 22, "IP address:");
  display.drawString(64, 40, WiFi.localIP().toString());
  display.display();
  delay(5000);
};

void oledStatePlugs(byte state1, byte state2, byte state3, byte state4)
{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, "Status");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(6, 20, "1) " + oledStateVerify(state1));
  display.drawString(70, 20, "3) " + oledStateVerify(state3));
  display.drawString(6, 48, "2) " + oledStateVerify(state2));
  display.drawString(70, 48, "4) " + oledStateVerify(state4));
  display.display();
};

void ledBlink()
{
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(1000);
};
//------------------------------------------->> CONEXÃO WIFI

void wifiStatus()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    oledSpinning("*", "CONNECTING");
    oledSpinning("*", "CONNECTING");
    delay(500);
  }
  oledWifiConfigurations();
  oledStatePlugs( digitalRead(plug1), digitalRead(plug2), digitalRead(plug3), digitalRead(plug4) );
};

//------------------------------------------->> SALVA O STATUS DAS GPIOs NA EPROM

void saveState(int address, byte state){
  if (EEPROM.read(address) != state)
   {
      EEPROM.write(address, state);
      EEPROM.commit();
      //Serial.println(EEPROM.read(address));
   }
}
