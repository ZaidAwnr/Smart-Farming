#include <SPI.h>
#include <LoRa.h>

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
// FREKUENSI LORA
// =====================================================
#define BAND 433E6

// =====================================================
// PIN SENSOR SOIL
// =====================================================
#define SOIL_PIN 34

void setup() {

  Serial.begin(115200);

  // =====================================================
  // SPI LORA
  // =====================================================
  SPI.begin(SCK, MISO, MOSI, SS);

  // =====================================================
  // SET PIN LORA
  // =====================================================
  LoRa.setPins(SS, RST, DIO0);

  // =====================================================
  // START LORA
  // =====================================================
  if (!LoRa.begin(BAND)) {

    Serial.println("LoRa gagal start!");

    while (1);
  }

  LoRa.setSyncWord(0xF3);

  Serial.println("NODE1 READY");
}

void loop() {

  // =====================================================
  // BACA SOIL MOISTURE
  // =====================================================
  int soilADC = analogRead(SOIL_PIN);

  // =====================================================
  // KONVERSI KE PERSEN
  // 4095 = kering
  // 1500 = basah
  // =====================================================
  int soilPercent = map(soilADC, 4095, 1500, 0, 100);

  soilPercent = constrain(soilPercent, 0, 100);

  // =====================================================
  // FORMAT DATA
  // NODE1|soil=65
  // =====================================================
  String dataSend = "NODE1|soil=" + String(soilPercent);

  // =====================================================
  // KIRIM LORA
  // =====================================================
  LoRa.beginPacket();
  LoRa.print(dataSend);
  LoRa.endPacket();

  // =====================================================
  // SERIAL MONITOR
  // =====================================================
  Serial.println("=================================");
  Serial.println("Mengirim Data NODE1");
  Serial.println(dataSend);
  Serial.println("=================================");

  delay(5000);
}
```

---

# Node 2 — Sensor NPK + DHT22

```cpp
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
* Adafruit Unified Sensor
* SPI
