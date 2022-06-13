#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// Set AP redentials
#define AP_SSID "arfiligol2"
#define AP_PASS "dragon25468"

#define pin_aa D4
#define pin_ab D3
#define pin_ba D5
#define pin_bb D6

#define ser_hor D8

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


// Setup for servo motor

Servo ver_servo;
Servo hor_servo;

#define hor_servo_pin D9 //設定"控制相機水平移動的伺服馬達"的腳位
#define ver_servo_pin D8 //設定"控制相機垂直移動的伺服馬達"的腳位

#define cam_hor_def 90 //設定相機的水平原始角度
#define cam_ver_def 90 //設定相機的垂直原始角度

#define cam_hor_del 90 //設定相機的水平最大偏移角度
#define cam_ver_del 90 //設定相機的垂直最大偏移角度


const int cam_hor_min=cam_hor_def-cam_hor_del; //相機的水平最小角度
const int cam_hor_max=cam_hor_def+cam_hor_del; //相機的水平最大角度
const int cam_ver_min=cam_ver_def-cam_ver_del; //相機的垂直最小角度
const int cam_ver_max=cam_ver_def+cam_ver_del; //相機的垂直最大角度

void cam_pos_reset(){hor_servo.write(cam_hor_def);ver_servo.write(cam_ver_def);return;} //相機轉回原位

void cam_hor_write(int angle=0){hor_servo.write(constrain(angle+cam_hor_def,cam_hor_min,cam_hor_max));return;} //相機水平轉到angle度，angle預設為0 (0為原始角度、正值向左)
void cam_ver_write(int angle=0){ver_servo.write(constrain(angle+cam_ver_def,cam_ver_min,cam_ver_max));return;} //相機垂直轉到angle度，angle預設為0 (0為原始角度、正值向上)

void cam_hor_turn(int angle){hor_servo.write(constrain(hor_servo.read()+angle,cam_hor_min,cam_hor_max));return;} //相機水平轉angle度 (正值向左)
void cam_ver_turn(int angle){ver_servo.write(constrain(ver_servo.read()+angle,cam_ver_min,cam_ver_max));return;} //相機垂直轉angle度 (正值向上)

int cam_hor_read(){return hor_servo.read()-cam_hor_def;} //讀取相機水平偏移角度 (0為原始角度、正值向左)
int cam_ver_read(){return ver_servo.read()-cam_ver_def;} //讀取相機垂直偏移角度 (0為原始角度、正值向上)

void cam_turn_right(int angle=1){return cam_hor_turn(-angle);} //相機往右轉angle度，angle預設為1
void cam_turn_left (int angle=1){return cam_hor_turn( angle);} //相機往左轉angle度，angle預設為1
void cam_turn_down (int angle=1){return cam_ver_turn(-angle);} //相機往下轉angle度，angle預設為1
void cam_turn_up   (int angle=1){return cam_ver_turn( angle);} //相機往上轉angle度，angle預設為1



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

  // Setup for servo motor
  hor_servo.attach(hor_servo_pin); 
  ver_servo.attach(ver_servo_pin); 
  cam_pos_reset();

}

void loop() {
  // put your main code here, to run repeatedly:

  // Clear packetBuffer
  packetBuffer[0] = 0;
  for(auto i:packetBuffer){
    i = '\0';
  }
  // Receive packet
  int packetSize = UDP.parsePacket(); // parsePacket() function return the size of packet
  if (packetSize) {
    // Print the size of packet
    //Serial.print("Received packet! Size: ");
    //Serial.println(packetSize);

    // Read the UDP packet into the packetBuffer (a container) with length UDP_TX_PACKET_MAX_SIZE.
    int len = UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

    if (len > 0) {
      packetBuffer[len] = 0;
    }

    Serial.print("Contents: ");
    Serial.println(packetBuffer);
  }

  // Control the car
  if(packetBuffer[0]){
    switch(char(packetBuffer[0])){
      // Car movement
      case '1':case 1:sharp_left();break;
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
