#include <LoRa.h>
#include <SPI.h>

String lastPacket;
bool isFar = false;
bool toStop = false;

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(115200); 
  while (!Serial);

  initLoRa();

  xTaskCreate(Led, "Led", 2048, NULL, 1, NULL);
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

void heartbeat() {
  static int counter = 1;

  Serial.printf("Sent beat %i\n", counter);
  LoRa.beginPacket();
  LoRa.printf("H%i", counter);
  LoRa.endPacket();

  counter++;
}

bool waitPacket() {
  int size = LoRa.parsePacket();
  if (!size) return false;

  if (!LoRa.available()) return false;

  // Read packet string
  String data = LoRa.readString();
  lastPacket = data;
  return true;
}

void loop() {
  const int LISTENING_TIME_MS = 2000;

  heartbeat();

  // Listen for a whole second
  int time = millis();
  while (millis() - time < LISTENING_TIME_MS) {
    if (!waitPacket()) continue;

    // Print packet
    Serial.printf("- %s\n", lastPacket);

    if (lastPacket.equals("HLD")) {
      toStop = !toStop;
      break;
    }
    if (lastPacket.equals("F")) {
      isFar = true;
    }
    if (lastPacket.equals("NF")) {
      isFar = false;
    }
  }
}

void Led(void *pvParameters) {
  while (true) {
    if(isFar || toStop){
      analogWrite(13, 40);
    } if(toStop){
      
    }else{
      vTaskDelay(500 / portTICK_PERIOD_MS); 
      analogWrite(13, 0); 
    } 
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

