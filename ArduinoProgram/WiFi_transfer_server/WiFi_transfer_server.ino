#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Set AP redentials
#define AP_SSID "arfiligol"
#define AP_PASS "dragon25468"

#define pin_aa D3
#define pin_ab D4
#define pin_ba D5
#define pin_bb D6
// UDP
WiFiUDP UDP;
IPAddress local_IP(192,168,4,1); // Set the IP of the server
IPAddress gateway(192,168,4,1); // Set up Gateway
IPAddress subnet(255,255,255,0); // Set up subnet
#define UDP_PORT 4210 // The port we are using udp to communicate

// UDP Buffer // A variable we use to contain the UDP packet
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // UDP_TX_PACKET_MAX_SIZE is a number, 


// Functions for controlling car
void _control(int l,int r){//控制左右馬達功率(-255最大逆轉,0停止,255最大正轉)
  l=constrain(l,-255,255);r=constrain(r,-255,255);
  if(l<=0){digitalWrite(pin_ba,LOW);analogWrite(pin_bb,-l);}
  else{digitalWrite(pin_ba,HIGH);analogWrite(pin_bb,255-l);}
  if(r<=0){digitalWrite(pin_aa,LOW);analogWrite(pin_ab,-r);}
  else{digitalWrite(pin_aa,HIGH);analogWrite(pin_ab,255-r);}
  return;
}
// Different conditions of functions
void _stop(){return _control(0,0);}//停止
void forward(int power=255){return _control(power,power);}//往前(0停止,預設255最大功率)
void backward(int power=255){return _control(-power,-power);}//倒退(0停止,預設255最大功率)
void turn_right(int power=255){return _control(power,0);}//右轉(0停止,預設255最大功率)
void turn_left(int power=255){return _control(0,power);}//左轉(0停止,預設255最大功率)
void right_front(int power=255){return _control(power,power/2);}//淺右轉(0停止,預設255最大功率)
void left_front(int power=255){return _control(power/2,power);}//淺左轉(0停止,預設255最大功率)
void sharp_right(int power=255){return _control(power,-power/2);}//右急轉(0停止,預設255最大功率)
void sharp_left(int power=255){return _control(-power/2,power);}//左急轉(0停止,預設255最大功率)
void spot_right(int power=255){return _control(power,-power);}//原地右轉(0停止,預設255最大功率)
void spot_left(int power=255){return _control(-power,power);}//原地左轉(0停止,預設255最大功率)


// Arduino setup
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

  // Setup pinMode
  pinMode(pin_ba,OUTPUT);//接B-1A
  pinMode(pin_bb,OUTPUT);//接B-1B
  pinMode(pin_ab,OUTPUT);//接A-1B
  pinMode(pin_aa,OUTPUT);//接A-1A
  
  digitalWrite(pin_ba,LOW);
  analogWrite(pin_bb,0);
  analogWrite(pin_ab,0);
  digitalWrite(pin_aa,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Clear packetBuffer
  packetBuffer[0] = 0;

  // Receive packet
  int packetSize = UDP.parsePacket(); // parsePacket() function return the size of packet
  if (packetSize) {
    // Print the size of packet
    //Serial.print("Received packet! Size: ");
    //Serial.println(packetSize);

    // Read the UDP packet into the packetBuffer (a container) with length UDP_TX_PACKET_MAX_SIZE.
    int len = UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    //Serial.println(packetBuffer);

    Serial.println(packetBuffer[0]);
  }
  

  // Control the car
  if(packetBuffer[0]){
    switch(char(packetBuffer[0])){
      case '1':case 1:sharp_left();Serial.println("read!");break;
      case '2':backward();break;
      case '3':sharp_right();break;
      case '4':turn_left();break;
      case '5':_stop();break;
      case '6':turn_right();break;
      case '7':left_front();break;
      case '8':forward();break;
      case '9':right_front();break;
      case '0':spot_left();break;
      case '.':spot_right();break;
      default:break;
    }
  }
}
