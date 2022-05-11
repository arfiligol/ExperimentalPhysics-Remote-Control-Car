#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Set AP redentials
#define AP_SSID "arfiligol"
#define AP_PASS "dragon25468"


// UDP
WiFiUDP UDP;
IPAddress local_IP(192,168,4,1); // Set the IP of the server
IPAddress gateway(192,168,4,1); // Set up Gateway
IPAddress subnet(255,255,255,0); // Set up subnet
#define UDP_PORT 4210 // The port we are using udp to communicate

// UDP Buffer // A variable we use to contain the UDP packet
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // UDP_TX_PACKET_MAX_SIZE is a number, 

void setup() {
  // put your setup code here, to run once:

  // Setup serial port
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial port start.");

  // Begin AP
  Serial.println("Starting access point...");
  WiFi.softAPConfig(local_IP, gateway, subnet); // Config the access point
  WiFi.softAP(AP_SSID, AP_PASS); // Config name and password
  Serial.print("The IP of server is: ");
  Serial.println(WiFi.softAPIP());

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Receive packet
  
  int packetSize = UDP.parsePacket(); // parsePacket() function return the size of packet
  if (packetSize) {
    // Print the size of packet
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize);

    // Read the UDP packet into the packetBuffer (a container) with length UDP_TX_PACKET_MAX_SIZE.
    int len = UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println(packetBuffer);

  }
}
