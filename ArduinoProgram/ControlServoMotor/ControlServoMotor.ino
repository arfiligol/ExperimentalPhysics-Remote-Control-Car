#include<Servo.h>

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

void setup()
{
  hor_servo.attach(hor_servo_pin); 
  ver_servo.attach(ver_servo_pin); 
  cam_pos_reset();
}

void loop()
{   
  
  
}
