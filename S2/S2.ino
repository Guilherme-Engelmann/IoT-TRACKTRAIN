#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// ====== WIFI ======
const char* SSID = "FIESC_IOT_EDU";
const char* PASS = "8120gv08";

// ====== MQTT (HiveMQ Cloud) ======
const char* MQTT_SERVER = "698831f5fdc44389a1d45db0951f0e8d.s1.eu.hivemq.cloud";
const int   MQTT_PORT   = 8883;

const char* MQTT_USER   = "Placa_2_Guilherme";
const char* MQTT_PASS   = "Placa_2_Guilherme";

// ====== TÓPICOS ======
const char* TOPICO_S1 = "ferrorama/bloco1/sensor1";
const char* TOPICO_S2 = "ferrorama/bloco1/sensor2";
const char* TOPICO_ILUMINACAO = "ferrorama/iluminacao";  // recebe comando

// ====== CLIENTES ======
WiFiClientSecure espClient;
PubSubClient mqtt(espClient);

// ====== PINOS ======
#define LED   2      // LED na placa
#define TRIG1 5
#define ECHO1 18
#define TRIG2 19
#define ECHO2 21

// ====== FUNÇÃO CALLBACK ======
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == TOPICO_ILUMINACAO) {
    if (msg == "ON") {
      digitalWrite(LED, HIGH);
      Serial.println("💡 Iluminação LIGADA!");
    } else {
      digitalWrite(LED, LOW);
      Serial.println("💡 Iluminação DESLIGADA!");
    }
  }
}

// ====== CONECTAR WIFI ======
void conectaWiFi() {
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi conectado!");
}

// ====== CONECTAR MQTT ======
void conectaMQTT() {
  Serial.println("Conectando ao HiveMQ...");

  while (!mqtt.connected()) {
    if (mqtt.connect("Bloco1Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("MQTT conectado!");
      mqtt.subscribe(TOPICO_ILUMINACAO);
    } else {
      Serial.print("Falha, estado = ");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

// ====== FUNÇÃO DE DISTÂNCIA ======
int lerDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH);
  int distancia = duracao * 0.034 / 2;

  return distancia;
}

// ====== SETUP ======
void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  conectaWiFi();

  espClient.setInsecure();  // TLS sem certificado
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);

  conectaMQTT();
}

// ====== LOOP ======
void loop() {
  if (!mqtt.connected()) conectaMQTT();
  mqtt.loop();

  int d1 = lerDistancia(TRIG1, ECHO1);
  int d2 = lerDistancia(TRIG2, ECHO2);

  // Publica sensor 1
  if (d1 < 10)
    mqtt.publish(TOPICO_S1, "Trem detectado S1");
  else
    mqtt.publish(TOPICO_S1, "Sem trem S1");

  // Publica sensor 2
  if (d2 < 10)
    mqtt.publish(TOPICO_S2, "Trem detectado S2");
  else
    mqtt.publish(TOPICO_S2, "Sem trem S2");

  delay(600);
}
