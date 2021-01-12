#include <ros2arduino.h>

// M5 Stack include ---------------------------------
#include <M5Core2.h>

#include <WiFi.h>
#include <WiFiUdp.h>
#define PUBLISH_FREQUENCY 30 //hz

WiFiUDP udp;

#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3

// Connect to RasPi(Micro-XRCE-DDS-Agent)
String ssid       = "ssid";
String pass       = "pass";
String agent_ip   = "agent_ip";
int port = 2018;

float joy_x, joy_y = 0;
//===========================================================

void touchflush(){
  TouchPoint_t pos = M5.Touch.getPressPoint();
  bool touchStateNow = ( pos.x == -1 ) ? false : true;
  M5.Lcd.clear();
  
  if( touchStateNow ){
    M5.Lcd.drawCircle(pos.x, pos.y, 20, GREEN);
    
    joy_x = (float(pos.x) - 320/2)/50;
    joy_y = -1*(float(pos.y) - 240/2)/50;

    if(joy_x >= 1)        joy_x = 1.0;
    else if(joy_x <= -1)  joy_x = -1.0;
    if(joy_y >= 1)        joy_y = 1.0;
    else if(joy_y <= -1)  joy_y = -1.0;
  }else{
    M5.Lcd.drawCircle(320/2, 240/2, 20, GREEN);
    joy_x = 0;
    joy_y = 0;
  }
}

void publishTwist(geometry_msgs::Twist* msg, void* arg){
  (void)(arg);
  
  msg->linear.x = joy_x;
  msg->linear.y = 0;
  msg->linear.z = 0;
  msg->angular.x = 0;
  msg->angular.y = 0;
  msg->angular.z = joy_y;
}


class TwistPub : public ros2::Node
{
  public:
    TwistPub(): Node("m5core2_Twist"){
      ros2::Publisher<geometry_msgs::Twist>* publisher_ = this->createPublisher<geometry_msgs::Twist>("cmd_vel");
      this->createWallFreq(PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishTwist, nullptr, publisher_);
    }
};

// M5 Stack ===========================================================================
void setup()
{
  M5.begin(true, true, true, true);
  WiFi.begin(ssid.c_str(), pass.c_str());
  while (WiFi.status() != WL_CONNECTED);

  ros2::init(&udp, agent_ip.c_str(), port);
}

void loop()
{
  static TwistPub TwistNode;
  touchflush();
  ros2::spin(&TwistNode);
}
