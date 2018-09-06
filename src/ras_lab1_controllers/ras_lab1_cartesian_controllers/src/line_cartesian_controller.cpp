#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "line_controller");

  ros::NodeHandle n;

  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/motor_controller/twist", 10);

  ros::Rate loop_rate(10);


  while (ros::ok())
  {
    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x = 0.2;
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = 0;

    vel_pub.publish(vel_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
