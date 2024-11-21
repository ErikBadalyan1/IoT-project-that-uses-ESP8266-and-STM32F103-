#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Ucom0222";
const char* password = "Bmw45545";

// Web server on port 80
WiFiServer server(80);

// Variables to store temperature and humidity
String temperature = "N/A";
String humidity = "N/A";

void setup() {
  Serial.begin(9600);  // Initialize serial for communication with STM32F103
  Serial.setTimeout(2000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // Check for data from STM32
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if (data.startsWith("T:") && data.indexOf("H:") != -1) {
      // Parse temperature and humidity
      int tempIndex = data.indexOf("T:") + 2;
      int humIndex = data.indexOf("H:") + 2;

      temperature = data.substring(tempIndex, data.indexOf(';'));
      humidity = data.substring(humIndex);
    }
  }

  // Handle client requests
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected.");
    String req = client.readStringUntil('\r');
    client.flush();

    // Webpage content
    String html = "<html><head><title>STM32 DHT11</title></head>";
    html += "<body style='background-color:white;'>";
    html += "<h1>Temperature: " + temperature + " °C</h1>";
    html += "<h1>Humidity: " + humidity + " %</h1>";
    html += "<div id='map' style='width: 600px; height: 400px;'></div>";
    html += "<script src='https://unpkg.com/leaflet@1.7.1/dist/leaflet.js'></script>";
    html += "<script>";
    html += "var map = L.map('map').setView([51.505, -0.09], 13);";
    html += "L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);";
    html += "</script></body></html>";

    // Send the webpage
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println(html);

    client.stop();
    Serial.println("Client disconnected.");
  }
}
