#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSWIFI_SSIDID = "FIESC_IOT_EDU";
const String WIFI_PASS = "8120gv08";
const String BoardID = "s4";

//constantes p/ broker
const char* URL    = "698831f5fdc44389a1d45db0951f0e8d.s1.eu.hivemq.cloud";
const int PORT      = 8883;
const char* USR    = "";
const char* broker_user   = "allyson_brinkerhoff" ;
const char* broker_PASS   = "AllysonGo1";

const char* MyTopic  = "Allyson Schaedler Brinkerhoff";
const char* OtherTopic  = "Allyson Schaedler Brinkerhoff";
const byte LED_G = 18;
const byte LED_R = 21;

// ====== TÓPICOS MQTT ======
const char* topico_velocidade = "velocidade";  // receber

void setup() {
  Serial.begin(115200);
  client.setInsecure();
  pinMode(LED_G,OUTPUT);
  pinMode(LED_R,OUTPUT);
  Serial.println("Conectando ao WiFi"); //apresenta a mensagem na tela
  WiFi.begin(SSWIFI_SSIDID,WIFI_PASS); //tenta conectar na rede
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); // mostra "....."
    delay(200);
    
  // O código transforma o ESP32 em um controlador para o trem do Ferrorama, conectando-o ao Wi-Fi e a um servidor MQTT. 
  // O ESP32 se inscreve no tópico "velocidade" para receber comandos de movimento. 
  // A função callback() processa os dados, e o programa é dividido em configuração inicial e um loop para comunicação contínua.
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
  mqtt.setCallback(callback);
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
  
  Serial.println(velocidade);
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

      // Se a velocidade for positiva, o robô vai para frente, acendendo o LED no pino 21 e apagando o no pino 18. 
      // Se a velocidade for negativa, o robô vai para trás, acendendo o LED no pino 18. Se a velocidade for zero, ambos os LEDs são apagados. 
      // Há comentários em português para indicar onde controlar os motores do robô.
    }
  }
}


