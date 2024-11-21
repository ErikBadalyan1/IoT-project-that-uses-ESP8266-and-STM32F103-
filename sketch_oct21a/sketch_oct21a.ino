#include <DHT.h>

#define DHT_PIN PA1  // GPIO for DHT11 on STM32
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  dht.begin();         // Initialize DHT11
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    Serial.print("T:");  // Sending temperature
    Serial.print(temp);
    Serial.print(";");
    Serial.print("H:");  // Sending humidity
    Serial.println(hum);
  }

  delay(2000);  // Send data every 2 seconds
}
