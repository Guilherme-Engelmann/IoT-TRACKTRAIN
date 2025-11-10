<<<<<<< HEAD
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// ====== CONFIG WIFI ======
const char* SSID = "FIESC_IOT_EDU";
const char* PASS = "8120gv08";

// ====== CONFIG MQTT (HiveMQ) ======
const char* MQTT_SERVER = "your-hivemq-broker.hivemq.cloud"; // troque pelo seu domínio HiveMQ
const int   MQTT_PORT   = 8883;
const char* MQTT_USER   = "seuUsuario"; // insira seu user HiveMQ
const char* MQTT_PASS   = "suaSenha";   // insira sua senha HiveMQ

WiFiClientSecure espClient;
PubSubClient mqtt(espClient);

// ====== TÓPICOS MQTT ======
const char* TOPICO_PRESENCA1 = "ferrorama/bloco1/presenca1";
const char* TOPICO_PRESENCA2 = "ferrorama/bloco1/presenca2";
const char* TOPICO_ILUMINACAO = "ferrorama/iluminacao";  // receber
=======
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSID = "";
const String PASS = "8120gv08";

//constantes p/ broker
const String URL    = "570f551f5406454bb50ec90a421ce6ae.s1.eu.hivemq.cloud";
const int PORT      = "8883";
const String USR    = "";
const String broker_user   = "Placa_2_Allyson" ;
const String broker_PASS   = "Placa_2_Allyson";

const String MyTopic  = "Allyson Schaedler Brinkerhoff";
const String OtherTopic  = "Allyson Schaedler Brinkerhoff";

const byte PONTE_H = 2;

>>>>>>> c12e100322b0d347412989500f955093cd2dfdee

// ====== PINOS ======
#define LED 2
#define TRIG1 5
#define ECHO1 18
#define TRIG2 19
#define ECHO2 21

// ====== VARIÁVEIS ======
long duracao;
int distancia;
String mensagem;

// ====== FUNÇÃO CALLBACK ======
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == TOPICO_ILUMINACAO) {
    if (msg == "ON") {
      digitalWrite(LED, HIGH);
      Serial.println(" Iluminação ligada!");
    } else {
      digitalWrite(LED, LOW);
      Serial.println(" Iluminação desligada!");
    }
  }
}

// ====== CONFIGURAÇÃO INICIAL ======
void setup() {
  Serial.begin(115200);
<<<<<<< HEAD
  pinMode(LED, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  // Conexão Wi-Fi
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
=======
  pinMode(2, OUTPUT);
  Serial.println("Conectando ao WiFi");
  WiFi.begin(SSID,PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso!");
  Client.setinsecure();
  Serial.println("Conectando ao Broker");
  mqtt.setServer(URL.c_str(),PORT);
  while(!mqtt.connected()){
    String ID = "S2_";
    ID += String(random(0xffff),HEX); //cria a parte aleat.
    mqtt.connect(ID.c_str(),USR.c_str(),broker_PASS.c_str());
>>>>>>> c12e100322b0d347412989500f955093cd2dfdee
    Serial.print(".");
  }
  Serial.println("\n Conectado ao Wi-Fi!");

  // Configura TLS (HiveMQ usa SSL)
  espClient.setInsecure();

  // Conexão ao Broker
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);

  while (!mqtt.connected()) {
    Serial.println("Conectando ao HiveMQ...");
    if (mqtt.connect("Bloco1Client", MQTT_USER, MQTT_PASS)) {
      Serial.println(" Conectado ao broker!");
      mqtt.subscribe(TOPICO_ILUMINACAO);
    } else {
      Serial.print("Falha na conexão. Estado: ");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

// ====== FUNÇÃO PARA MEDIR DISTÂNCIA ======
int lerDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duracao = pulseIn(echo, HIGH);
  int dist = duracao * 0.034 / 2;
  return dist;
}

// ====== LOOP PRINCIPAL ======
void loop() {
  if (!mqtt.connected()) return;

  mqtt.loop();
<<<<<<< HEAD
=======
  delay(2000);
}
>>>>>>> c12e100322b0d347412989500f955093cd2dfdee

  int dist1 = lerDistancia(TRIG1, ECHO1);
  int dist2 = lerDistancia(TRIG2, ECHO2);

<<<<<<< HEAD
  // Publica presença 1
  if (dist1 < 10) {
    mqtt.publish(TOPICO_PRESENCA1, "Trem detectado em S1");
  } else {
    mqtt.publish(TOPICO_PRESENCA1, "Sem trem em S1");
  }

  // Publica presença 2
  if (dist2 < 10) {
    mqtt.publish(TOPICO_PRESENCA2, "Trem detectado em S2");
  } else {
    mqtt.publish(TOPICO_PRESENCA2, "Sem trem em S2");
  }

  delay(1000);
}
=======
void setup() {
  pinMode(PONTE_H, OUTPUT);
  ledcWrite(PONTE_H, 255, 0);
}

void loop() {

  for (int brilho = 0; brilho <= 255; brilho++) {
    ledcWrite(PONTE_H, frente);
    delay(10);
  }
  
  // Diminui brilho gradualmente
  for (int brilho = 255; brilho >= 0; brilho--) {
    ledcWrite(PONTE_H, trás);
    delay(10);
  }
}

}


//---------------------------------------------
const byte PONTE_H = 2;

void setup() {
  pinMode(PONTE_H, OUTPUT);
  ledcWrite(PONTE_H, 255, 0);
}

void loop() {

  for (int brilho = 0; brilho <= 255; brilho++) {
    ledcWrite(PONTE_H, frente);
    delay(10);
  }
  
  // Diminui brilho gradualmente
  for (int brilho = 255; brilho >= 0; brilho--) {
    ledcWrite(PONTE_H, trás);
    delay(10);
  }
}



  // put your main code here, to run repeatedly:
  // S4 (TREM) minha parte - Allyson
  //DOIT ESP32 DEVKIT
  //PubSubClient

>>>>>>> c12e100322b0d347412989500f955093cd2dfdee
