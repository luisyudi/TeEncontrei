#include <LoRa.h>
#include <SPI.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
 
BluetoothSerial SerialBT; //Bluetooth serial
char LB;
float radius = 0.0;
const char* currentOrder = NULL;
bool isFar = false;

void setup() 
{
  //Initialize Serial
  Serial.begin(115200);
  while (!Serial);

  //Initialize led
  pinMode(13, OUTPUT);
  analogWrite(13, 0);

  initLoRa();

  //Initialize Bluetooth
  SerialBT.begin();

  // Create task
  xTaskCreate(BTRead, "BTRead", 2048, NULL, 1, NULL);
}

void initLoRa() {
  const int ss = 5, rst = 14, dio0 = 2;

  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }

  //LoRa.setSignalBandwidth(125E3);*/
  //LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(12);
  LoRa.setGain(7);
  LoRa.setTxPower(23);
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
}


void loop() {
  checkOrders();
  waitPacket();

  if(isFar){
    analogWrite(13, 40);
    delay(300);
    analogWrite(13, 0);  
  }
}

void doOrder(const char* order) {
  currentOrder = order;
}

void checkOrders() {
  if (!currentOrder) return;

  int time = millis();
  while (millis() - time < 1000) {
    sendPacket();
  }
  currentOrder = NULL;
}

void waitPacket() {
  if (!LoRa.parsePacket()) return;          
  
  String data = LoRa.readString();
  int rssi = LoRa.packetRssi();

  Serial.printf("Got beat %s [%i]\n", data, rssi); 

  radius = (float)rssi / 20.0 * -1 + 10;
  if(!isFar && rssi < -70){
    doOrder("F");
    isFar = true;
  }else if(isFar && rssi > -40){
    doOrder("NF");
    isFar = false;
  }
}

void sendPacket() {
  LoRa.beginPacket();
  LoRa.print(currentOrder);
  LoRa.endPacket();
}

// Função da tarefa para processar o RSSI
void BTRead(void *pvParameters) {
  int rssi = 0;
  while (true) {
    if (SerialBT.available()) { // Verifica se há dados disponíveis
      char LB = (char)SerialBT.read();
      Serial.print("Recebido via Bluetooth: ");
      Serial.println(LB);

      if (LB == 'S') {
        Serial.println(radius);
        String radiusStr = String(radius);
        SerialBT.print(radiusStr);
        Serial.print(radiusStr);
     
      } else if (LB == 'H') {
        doOrder("HLD");
      }
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS); // Delay para evitar sobrecarga da CPU
  }
}