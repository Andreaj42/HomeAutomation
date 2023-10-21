#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 600

#include <WiFi.h>
#include <InfluxDbClient.h>
#include <DHT.h>

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "********";
const char* password = "********";

#define INFLUXDB_URL "********"
#define INFLUXDB_ORG "********"
#define INFLUXDB_TOKEN "********"
#define INFLUXDB_BUCKET ""********""

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);


void send_data(float h, float t) {
  client.setInsecure();
  Point sensorData("DHT");
  sensorData.addTag("location", "salon");
  sensorData.addField("humidity", h);
  sensorData.addField("temperature", t);
  client.writePoint(sensorData);
}

void setup() {
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    send_data(humidity, temperature);
  }
  esp_deep_sleep_start();
}

void loop() {
}