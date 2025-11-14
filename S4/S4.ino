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


// ====== TÓPICOS MQTT ======
const char* topico_presenca1 = "//presenca1";
const char* topico_presenca2 = "//presenca2";
const char* topico_luminacao = "/iluminacao";  // receber



<<<<<<< HEAD




=======
>>>>>>> c12e100322b0d347412989500f955093cd2dfdee
void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi"); //apresenta a mensagem na tela
  WiFi.begin(WIFI_SSID,WIFI_PASS); //tenta conectar na rede
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); // mostra "....."
    delay(200);
  }
  Serial.println("\nConectado com Sucesso!");


  Serial.println("Conectando ao Broker...");
<<<<<<< HEAD
  mqtt.setServer(URL,PORT);
  String BoardID = "s2";
  BoardID += String(random(0xffff),HEX);
  mqtt.connect(BoardID.c_str() , Broker_user, Broker_PASS);
=======
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String BoardID = "s2";
  BoardID += String(random(0xffff),HEX);
  mqtt.connect(BoardID.c_str() , Broker_user, Broker_PASS;
>>>>>>> c12e100322b0d347412989500f955093cd2dfdee
  while(!mqtt.connected()){
    Serial.print(".");
    delay(200);
  }
<<<<<<< HEAD
  mqtt.subscribe(topico_luminacao)
=======
  mqtt.subscribe("topico_luminacao")
>>>>>>> c12e100322b0d347412989500f955093cd2dfdee
  Serial.println("\nConectado ao Broker!");
}


void loop() {
String mensagem = "Nome: Allyson";
mensagem+= "oi";
mqtt.publish("topico_luminacao" , mensagem.c_str());
mqtt.loop();
delay(1000);
}



void callback(char* topic, byte* payload, usigned int length){
  string msg = "";
  for(int i = 0; i < length; i++){
    msg += (char) payload[i]
  }
<<<<<<< HEAD
  if(topic == "topico_luminacao" && msg == "Acender"){
    digitalWrite(2,HIGH);
    if(topic == "topico_luminacao" && msg == "Apagar"){
=======
  if(topic == topico_luminacao && msg == "Acender"){
    digitalWrite(2,HIGH);
    if(topic == topico_luminacao && msg == "Apagar"){
>>>>>>> c12e100322b0d347412989500f955093cd2dfdee
    digitalWrite(2,LOW);
        }
     }
  }
}
