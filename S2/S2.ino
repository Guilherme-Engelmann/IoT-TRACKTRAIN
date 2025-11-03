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




void setup() {
  Serial.begin(115200);
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
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(MyTopic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectando com sucesso ao broker !");
}


void loop() {
  String mensagem = "Allyson Schaedler Brinkerhoff: "; //mudar p/ nome de vcs
  if (Serial.available()>0){
  mensagem += Serial.readStringUntil('\n');
  mqtt.publish(OtherTopic.c_str(),mensagem.c_str());
  }
  mqtt.loop();
  delay(2000);
}

void callback(char * topic, byte* payload, unsigned int length){
  String mensagem = "";
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(mensagem);

   if(mensagem == "HIGH") {
    Serial.println("Ligado e Conectado");
    digitalWrite(2, HIGH);
  } else {
    Serial.println("Desligado e Fracasso");
    digitalWrite(2, LOW);
  }
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}



  // put your main code here, to run repeatedly:
  // S4 (TREM) minha parte - Allyson
  //DOIT ESP32 DEVKIT
  //PubSubClient


