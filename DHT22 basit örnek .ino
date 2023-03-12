#include <DHT.h>

#define DHTPIN 9     // DHT22 sensörünün sinyal kablosunun bağlı olduğu pin
#define DHTTYPE DHT22   // DHT22 sensörünün tipi

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();    // DHT22 sensörü başlatılır
}

void loop() {
  delay(2000);    // 2 saniyede bir ölçüm yapılır

  float temperature = dht.readTemperature();  // Sıcaklık ölçümü yapılır
  float humidity = dht.readHumidity();       // Nem ölçümü yapılır

  if (isnan(temperature) || isnan(humidity)) {  // Veriler geçersizse hata mesajı verilir
    Serial.println("DHT22 sensöründen veri alınamadı!");
    return;
  }

  Serial.print("Sicaklik: ");
  Serial.print(temperature);
  Serial.print(" C  ");
  Serial.print("Nem: ");
  Serial.print(humidity);
  Serial.println(" %");
}
