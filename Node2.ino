
#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

// =====================================================
// PIN LORA TTGO T3 V1.6.1
// =====================================================
#define SCK     5
#define MISO    19
#define MOSI    27
#define SS      18
#define RST     14
#define DIO0    26

// =====================================================
// FREKUENSI
// =====================================================
#define BAND 433E6

// =====================================================
// DHT22
// =====================================================
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  dht.begin();

  // =====================================================
  // SPI
  // =====================================================
  SPI.begin(SCK, MISO, MOSI, SS);

  // =====================================================
  // LORA
  // =====================================================
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {

    Serial.println("LoRa gagal start!");

    while (1);
  }

  LoRa.setSyncWord(0xF3);

  Serial.println("NODE2 READY");
}

void loop() {

  // =====================================================
  // CONTOH DATA NPK
  // GANTI DENGAN SENSOR ASLI JIKA ADA
  // =====================================================
  int nitrogen = random(80, 150);
  int phosphorus = random(20, 80);
  int potassium = random(40, 120);

  // =====================================================
  // BACA DHT22
  // =====================================================
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // =====================================================
  // CEK ERROR DHT
  // =====================================================
  if (isnan(temperature) || isnan(humidity)) {

    Serial.println("Gagal membaca DHT22!");

    return;
  }

  // =====================================================
  // FORMAT DATA
  // NODE2|n=120,p=40,k=80,temp=29.4,hum=78
  // =====================================================
  String dataSend = "NODE2|";

  dataSend += "n=" + String(nitrogen);
  dataSend += ",p=" + String(phosphorus);
  dataSend += ",k=" + String(potassium);
  dataSend += ",temp=" + String(temperature);
  dataSend += ",hum=" + String(humidity);

  // =====================================================
  // KIRIM LORA
  // =====================================================
  LoRa.beginPacket();
  LoRa.print(dataSend);
  LoRa.endPacket();

  // =====================================================
  // SERIAL
  // =====================================================
  Serial.println("=================================");
  Serial.println("Mengirim Data NODE2");
  Serial.println(dataSend);
  Serial.println("=================================");

  delay(5000);
}
