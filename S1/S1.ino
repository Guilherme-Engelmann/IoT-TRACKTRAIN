#include <Wifi.h>

const String SSID = "nome_da_rede";
const String PASS = "senha_da_rede";

void setup() {
  Serial.begin(115200);
  Serial.println("Conectado ao Wifi");
  Wifi.begin(SSID,PASS);
  while(Wifi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
