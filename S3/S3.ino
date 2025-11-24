#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// -------------------------
// CONFIGURAÇÃO DE REDE
// -------------------------
const char* ssid     = "SEU_WIFI";
const char* password = "SENHA_WIFI";

const char* mqttServer = "test.mosquitto.org";
int mqttPort = 1883;

// -------------------------
// DEFINIÇÃO DE PINOS
// -------------------------
#define DHTPIN 4
#define DHTTYPE DHT11

#define TRIG_PIN 22
#define ECHO_PIN 23

#define LED_ILUMINACAO 19

#define LDR_PIN 34

#define RGB_R 14
#define RGB_G 26
#define RGB_B 25

// -------------------------
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// -------------------------
// FUNÇÃO RGB
// -------------------------
void rgb(int r, int g, int b){
  analogWrite(RGB_R, r);
  analogWrite(RGB_G, g);
  analogWrite(RGB_B, b);
}

// -------------------------
// CONECTAR WIFI
// -------------------------
void conectarWiFi() {
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  rgb(0,50,0); // Verde: conectado
}

// -------------------------
// CONECTAR MQTT
// -------------------------
void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("S1_Station")) {
      Serial.println("Conectado!");
      rgb(0,0,50); // Azul: conectado ao broker
    } else {
      Serial.print("Falha. Erro: ");
      Serial.println(client.state());
      rgb(50,0,0); // vermelho: erro
      delay(2000);
    }
  }
}

// -------------------------
// LEITURA DO ULTRASSOM
// -------------------------
int lerPresenca(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  if(distance > 0 && distance < 20){
    return 1;   // TEM ALGO PERTO
  } else {
    return 0;
  }
}

// -------------------------
// SETUP
// -------------------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_ILUMINACAO, OUTPUT);

  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);

  dht.begin();

  conectarWiFi();
  client.setServer(mqttServer, mqttPort);

  conectarMQTT();
}

// -------------------------
// LOOP PRINCIPAL
// -------------------------
void loop() {

  // Reconectar MQTT se cair
  if (!client.connected()) conectarMQTT();
  client.loop();

  // -------- TEMPERATURA e UMIDADE --------
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (!isnan(temperatura)){
    String tempStr = String(temperatura);
    client.publish("no1/temperatura", tempStr.c_str());
  }

  if (!isnan(umidade)){
    String umiStr = String(umidade);
    client.publish("no1/umidade", umiStr.c_str());
  }

  // -------- LUMINOSIDADE (LDR) --------
  int ldr = analogRead(LDR_PIN);
  String estadoLuz;

  if(ldr < 2000){
    estadoLuz = "escuro";
    digitalWrite(LED_ILUMINACAO, HIGH);   // acende LED
  } else {
    estadoLuz = "claro";
    digitalWrite(LED_ILUMINACAO, LOW);    // apaga LED
  }

  client.publish("no1/luminosidade", estadoLuz.c_str());

  // -------- PRESENÇA (ULTRASSOM) --------
  int presenca = lerPresenca();
  client.publish("no1/presenca", String(presenca).c_str());

  // Status visual simples no LED RGB
  if(presenca == 1){
    rgb(50,0,0); // vermelho = presença
  } else {
    rgb(0,0,20); // azul fraco = normal
  }

  delay(1500); // intervalo entre leituras
}