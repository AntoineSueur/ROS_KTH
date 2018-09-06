#include "ros/ros.h"
#include "ras_lab1_msgs/PWM.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");

  ros::NodeHandle n;

  ros::Publisher controller_pub = n.advertise<ras_lab1_msgs::PWM>("/kobuki/pwm", 10);

  ros::Rate loop_rate(10);


  while (ros::ok())
  {
    ras_lab1_msgs::PWM msg;
    msg.header.stamp = ros::Time::now();
    msg.header.frame_id = "test";
    msg.PWM1 = 255;
    msg.PWM2 = 255;

    controller_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
