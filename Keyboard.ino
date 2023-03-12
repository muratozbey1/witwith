#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#define MQ3 D8         // MQ-3 alkol sensörü analog pin A0'a bağlıdır
#define THRESHOLD 50   // Alkol eşiği değeri

char ssid[] = "didim1";    // WiFi ağının SSID'si
char password[] = "Elaemir1906";   // WiFi ağının şifresi

// MySQL veritabanı bilgileri
IPAddress server_addr(213,146,138,70);  // MySQL veritabanı adresi
char user[] = "alkometre";              // MySQL kullanıcı adı
char password_db[] = "654123";          // MySQL şifresi
char db_name[] = "Alkol";               // MySQL veritabanı adı

WiFiClient client;
MySQL_Connection conn((Client *)&client);
char INSERT_SQL[100];

void setup() {
  Serial.begin(9600);
  pinMode(MQ3, INPUT);
  
  // WiFi bağlantısı yap
  WiFi.begin(ssid, password);
  Serial.println("WiFi Baglaniliyor...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi Baglanti Bekleniyor...");
  }
  Serial.println("WiFi Baglandi.");
}

void loop() {
  float alkolDegeri = analogRead(MQ3) / 1024.0 * 5.0;  // Sensörden alkol değerini oku
  String durum = ""; // Durum sütunu
  if (alkolDegeri > THRESHOLD) {   // Eşik değerini kontrol et
    durum = "Yuksek Alkollu"; // Yüksek alkol seviyesi tespit edildi
  } else {
    durum = "Ysal sinirda alkollu"; // Yüksek alkol seviyesi tespit edilmedi
  }
  
  // MySQL bağlantısı yap
  if (conn.connect(server_addr, 3306, user, password_db)) {
    Serial.println("MySQL Baglandi.");
    MySQL_Cursor *cursor = new MySQL_Cursor(&conn);
    sprintf(INSERT_SQL, "INSERT INTO Alkol(olcumdegeri, olcumzamani, durum) VALUES(%.2f, NOW(), '%s')", alkolDegeri, durum.c_str());
    cursor->execute(INSERT_SQL);
    delete cursor;
    conn.close();
  }
  
  delay(1000); // 1 saniye bekle ve yeniden oku
}
