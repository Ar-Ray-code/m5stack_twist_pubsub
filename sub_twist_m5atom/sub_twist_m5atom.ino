#include <ros2arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP udp;  

#include "M5Atom.h"

bool IMU6886Flag = false;

//-------------------
void up_icon();
void down_icon();
void right_icon();
void left_icon();

void read_msgs(geometry_msgs::Twist* msg, void* arg);
//-------------------

// Connect to RasPi(Micro-XRCE-DDS-Agent)
String ssid       = "ssid";
String pass       = "pass";
String agent_ip   = "agent_ip";
int port = 2019;


class StringPub : public ros2::Node
{
  public:
    StringPub(): Node("m5atom_m_ros2node")
    {
      this->createSubscriber<geometry_msgs::Twist>("cmd_vel", (ros2::CallbackFunc)read_msgs, nullptr);
    }
};

void read_msgs(geometry_msgs::Twist* msg, void* arg)
{
  (void)(arg);
  if(abs(msg->linear.x) > abs(msg->angular.z)){
    if(msg->linear.x > 0){
      right_icon();
    }
    else if(msg->linear.x < 0){
      left_icon();
    }
  }
  else if(msg->angular.z != 0){
    if(msg->angular.z > 0){
      up_icon();
    }
    else if(msg->angular.z < 0){
      down_icon();
    }
  }
  else
  {
    M5.dis.clear();
    M5.update();
  }
}

void up_icon()
{
  M5.dis.clear();
  M5.dis.drawpix(7, 0xf00000);
  
  M5.dis.drawpix(11, 0xf00000);
  M5.dis.drawpix(12, 0xf00000);
  M5.dis.drawpix(13, 0xf00000);

  M5.dis.drawpix(15, 0xf00000);
  M5.dis.drawpix(16, 0xf00000);
  M5.dis.drawpix(17, 0xf00000);
  M5.dis.drawpix(18, 0xf00000);
  M5.dis.drawpix(19, 0xf00000);
  M5.update();
}
void down_icon()
{
  M5.dis.clear();
  M5.dis.drawpix(17, 0xf00000);
  
  M5.dis.drawpix(11, 0xf00000);
  M5.dis.drawpix(12, 0xf00000);
  M5.dis.drawpix(13, 0xf00000);

  M5.dis.drawpix(5, 0xf00000);
  M5.dis.drawpix(6, 0xf00000);
  M5.dis.drawpix(7, 0xf00000);
  M5.dis.drawpix(8, 0xf00000);
  M5.dis.drawpix(9, 0xf00000);
  M5.update();
}
void left_icon()
{
  M5.dis.clear();
  M5.dis.drawpix(11, 0xf00000);
  
  M5.dis.drawpix(7, 0xf00000);
  M5.dis.drawpix(12, 0xf00000);
  M5.dis.drawpix(17, 0xf00000);

  M5.dis.drawpix(3, 0xf00000);
  M5.dis.drawpix(8, 0xf00000);
  M5.dis.drawpix(13, 0xf00000);
  M5.dis.drawpix(18, 0xf00000);
  M5.dis.drawpix(23, 0xf00000);
  M5.update();
}
void right_icon()
{
  M5.dis.clear();
  M5.dis.drawpix(13, 0xf00000);
  
  M5.dis.drawpix(7, 0xf00000);
  M5.dis.drawpix(12, 0xf00000);
  M5.dis.drawpix(17, 0xf00000);

  M5.dis.drawpix(1, 0xf00000);
  M5.dis.drawpix(6, 0xf00000);
  M5.dis.drawpix(11, 0xf00000);
  M5.dis.drawpix(16, 0xf00000);
  M5.dis.drawpix(21, 0xf00000);
  M5.update();
}

void setup()
{
  M5.begin(true, false, true);
  Serial.begin(115200);
  WiFi.begin(ssid.c_str(), pass.c_str());
  while (WiFi.status() != WL_CONNECTED);
  ros2::init(&udp, agent_ip.c_str(), port);
  delay(50);   
}

void loop()
{
  static StringPub StringNode;
  ros2::spin(&StringNode);
}
