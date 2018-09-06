#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ras_lab1_msgs/ADConverter.h"



ras_lab1_msgs::ADConverter adc_data = ras_lab1_msgs::ADConverter();


void callback(const ras_lab1_msgs::ADConverter::ConstPtr& msg)
{
adc_data = *msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "wall_folowing_controller");

  ros::NodeHandle n;
  ros::Subscriber adc = n.subscribe("/kobuki/adc", 10, callback);
  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/motor_controller/twist", 10);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {

    double front_dist = adc_data.ch1;
    double back_dist = adc_data.ch2;
    double alpha = 0.005;

    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x = 0.314;
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = alpha*( back_dist - front_dist);

    vel_pub.publish(vel_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
