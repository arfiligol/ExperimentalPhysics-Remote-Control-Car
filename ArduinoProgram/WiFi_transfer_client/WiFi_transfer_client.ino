#include <ESP8266WiFi.h> 
#include <WiFiUdp.h> // We are using UDP protocol to send message

// Set WiFi credentials
#define WIFI_SSID "arfiligol"
#define WIFI_PASS "dragon25468"

// UDP
WiFiUDP UDP;
IPAddress remote_IP(192,168,4,1); // remote IP is the IP where we are going to send message to.
#define UDP_PORT 4210

void setup() {
  // put your setup code here, to run once:

  // Setup serial port
  Serial.begin(115200);
  Serial.println();

  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);

  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  // When connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Opening UDP port ");
  Serial.println(UDP_PORT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  // Send Packet
  UDP.beginPacket(remote_IP, UDP_PORT); // Where the udp packet start
  UDP.write("hello"); // Write data or whatever to the packet
  UDP.endPacket(); // Where the udp packet end.
  delay(1000);
  
}
