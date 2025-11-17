#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSWIFI_SSIDID = "";
const String WIFI_PASS = "8120gv08";
const String BoardID = "s4";

//constantes p/ broker
const char* URL    = "570f551f5406454bb50ec90a421ce6ae.s1.eu.hivemq.cloud";
const int PORT      = 8883;
const char* USR    = "";
const char* broker_user   = "Placa_4_Allyson" ;
const char* broker_PASS   = "Placa_4_Allyson";

const char* MyTopic  = "Allyson Schaedler Brinkerhoff";
const char* OtherTopic  = "Allyson Schaedler Brinkerhoff";
const byte LED_G = 18;
const byte LED_R = 21;

// ====== TÓPICOS MQTT ======
const char* topico_velocidade = "velocidade";  // receber

void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi"); //apresenta a mensagem na tela
  WiFi.begin(SSWIFI_SSIDID,WIFI_PASS); //tenta conectar na rede
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); // mostra "....."
    delay(200);
  }
  Serial.println("\nConectado com Sucesso!");


  Serial.println("Conectando ao Broker...");
  mqtt.setServer(URL,PORT);
  String BoardID = "s4";
  BoardID += String(random(0xffff),HEX);
  mqtt.connect(BoardID.c_str() , broker_user, broker_PASS);
  mqtt.setServer(URL, PORT);
  
  BoardID += String(random(0xffff),HEX);

  while(!mqtt.connected()){
    Serial.print(".");
    delay(200);
  }

  mqtt.subscribe(topico_velocidade);
  Serial.println("\nConectado ao Broker!");
}


void loop() {
  mqtt.loop();
  delay(10);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  int velocidade = msg.toInt();
  
  if (strcmp(topic, topico_velocidade) == 0) {
    if (velocidade > 0) {
      digitalWrite(21, HIGH);  // Liga LED
      delay(1000);
      digitalWrite(18, LOW);  // Liga LED
      delay(1000);
      // Controlar os motores frente
    }
    else if (velocidade < 0) {
      digitalWrite(21, LOW);  // Liga LED
      delay(1000);
      digitalWrite(18, HIGH);  // Liga LED
      delay(1000);
      // Controlar os motores tras
    }
    else if (velocidade == 0) {
      digitalWrite(21, LOW);  // Liga LED
      delay(1000);
      digitalWrite(18, LOW);  // Liga LED
      delay(1000);
      // Controlar os motores parar
    }
  }
}
