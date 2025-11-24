#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>


/*WiFi.h → permite conectar o ESP32 à rede Wi-Fi.

WiFiClientSecure.h → permite conexões seguras (TLS/SSL), necessárias para o HiveMQ Cloud.

PubSubClient.h → implementa o protocolo MQTT (publicar e assinar tópicos).
  */
// ------------------ PINOS ------------------
#define PINO_LED 2
#define TRIG_PIN 26
#define ECHO_PIN 25
#define SERVO1_PIN 19
#define SERVO2_PIN 18
#define SENSOR_PRESENCA 14


/*
Pino	Função
2	LED
26	Trigger do ultrassom (envia pulso)
25	Echo do ultrassom (recebe pulso)
19	Servo motor 1
18	Servo motor 2
14	Sensor PIR de presença
*/

// ------------------ OBJETOS ------------------
WiFiClientSecure client;
PubSubClient mqtt(client);
Servo servoA;
Servo servoB;

/*
client → responsável pela comunicação segura.

mqtt → cliente MQTT que usa o cliente seguro.

servo3/servo4 → instâncias que controlam servos.
*/

// ------------------ WIFI ------------------
const char* SSID = "FIESC_IOT_EDU";
const char* PASS = "8120gv08";

/*
const char* SSID = "FIESC_IOT_EDU";
const char* PASS = "8120gv08";
São usados mais tarde para conectar à rede.
*/

// ------------------ MQTT ------------------
const char* BROKER_URL  = "698831f5fdc44389a1d45db0951f0e8d.s1.eu.hivemq.cloud";
const int   BROKER_PORT = 8883;
const char* BROKER_USER = "Polzin_s3";
const char* BROKER_PASS = "polzin2020";

const char* TOPIC_PUBLISH_PRESENCA = "Projeto_S3/Presenca3";
const char* TOPIC_PUBLISH_ULTRA1   = "Projeto_S3/Ultrassom1";
const char* TOPIC_PUBLISH_ULTRA2   = "Projeto_S3/Ultrassom2";

const char* TOPIC_LED_CONTROL = "S1/iluminacao";
const char* TOPIC_SERVO1 = "Projeto_S2/Distancia1";
const char* TOPIC_SERVO2 = "Projeto_S2/Distancia2";

// Timer para publicação
unsigned long lastPublish = 0;
const int publishInterval = 3000;



// ------------------ FUNÇÃO: ULTRASSOM ------------------
long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duracao == 0) return -1; // leitura inválida

  long distancia = (duracao * 0.0343) / 2;
  return distancia;
}

// ------------------ FUNÇÃO: CALLBACK MQTT ------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Mensagem recebida em ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // Controle do LED
  if (String(topic) == TOPIC_LED_CONTROL) {
    if (msg == "acender") {
      digitalWrite(PINO_LED, HIGH);
    } else if (msg == "apagar") {
      digitalWrite(PINO_LED, LOW);
    }
  }

  // Controle servo 1
  if (String(topic) == TOPIC_SERVO1) {
    servoA.write(msg == "objeto_proximo" ? 90 : 45);
  }

  // Controle servo 2
  if (String(topic) == TOPIC_SERVO2) {
    servoB.write(msg == "objeto_proximo" ? 90 : 45);
  }
}

// ------------------ WIFI ------------------
void conectarWiFi() {
  WiFi.begin(SSID, PASS);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(350);
  }

  Serial.println("\nWi-Fi conectado!");
}

// ------------------ MQTT ------------------
void conectarMQTT() {
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  client.setInsecure();
  mqtt.setCallback(callback);

  Serial.println("Conectando ao MQTT...");

  while (!mqtt.connected()) {
    String clientId = "S3_" + String(random(0xffff), HEX);

    if (mqtt.connect(clientId.c_str(), BROKER_USER, BROKER_PASS)) {
      Serial.println("Conectado ao broker!");

      mqtt.subscribe(TOPIC_LED_CONTROL);
      mqtt.subscribe(TOPIC_SERVO1);
      mqtt.subscribe(TOPIC_SERVO2);
    } 
    else {
      Serial.println("Falha ao conectar. Tentando novamente...");
      delay(1200);
    }
  }
}

// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);

  pinMode(PINO_LED, OUTPUT);
  pinMode(SENSOR_PRESENCA, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoA.attach(SERVO1_PIN);
  servoB.attach(SERVO2_PIN);

  servoA.write(0);
  servoB.write(0);

  conectarWiFi();
  conectarMQTT();
}

// ------------------ LOOP PRINCIPAL ------------------
void loop() {
  if (!mqtt.connected()) conectarMQTT();
  mqtt.loop();

  // ULTRASSOM
  long distancia = medirDistancia();
  Serial.print("Distância: ");
  Serial.println(distancia);

  if (distancia > 0 && distancia < 10) {
    mqtt.publish(TOPIC_PUBLISH_ULTRA1, "objeto_proximo");
  } else if (distancia >= 10) {
    mqtt.publish(TOPIC_PUBLISH_ULTRA2, "objeto_longe");
  }

  // PRESENÇA (a cada 3 segundos)
  unsigned long agora = millis();
  if (agora - lastPublish >= publishInterval) {
    lastPublish = agora;
    int presenca = digitalRead(SENSOR_PRESENCA);

    mqtt.publish(TOPIC_PUBLISH_PRESENCA, String(presenca).c_str());
  }

  delay(20);
}
