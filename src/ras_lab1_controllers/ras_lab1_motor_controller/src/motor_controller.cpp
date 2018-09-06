#include "ros/ros.h"
#include "ras_lab1_msgs/PWM.h"
#include "ras_lab1_msgs/Encoders.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"

/* DEBUGGING CODE
ros::Publisher debug = n.advertise<std_msgs::Float32>("/debug", 1000);
std_msgs::Float32 debug_msg;
debug_msg.data = pwm2;
debug.publish(debug_msg);*/

geometry_msgs::Twist twist_data = geometry_msgs::Twist();
ras_lab1_msgs::Encoders encoder_data = ras_lab1_msgs::Encoders();

void chatterCallback1(const geometry_msgs::Twist::ConstPtr& msg)
{
twist_data = *msg;
ROS_INFO("Required X-velocity: [%f]", msg->linear.x);
}

void chatterCallback2(const ras_lab1_msgs::Encoders::ConstPtr& msg)
{
  encoder_data = *msg;
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "motor_controller");

  ros::NodeHandle n;

  ros::Publisher controller_pub = n.advertise<ras_lab1_msgs::PWM>("/kobuki/pwm", 1000);
  ros::Subscriber sub_twist = n.subscribe("/motor_controller/twist", 10, chatterCallback1);
  ros::Subscriber sub_encoder = n.subscribe("/kobuki/encoders", 10, chatterCallback2);
  ros::Rate loop_rate(10);

  double control_frequency = 10.0;
  double ticks_per_rev = 360.0;
  double alpha = 10.0;
  double beta = 10.0;
  double dt = 0.1;
  double r = 0.0352;
  double b = 0.23;



  while (ros::ok())
  {

    double desired_v_x = twist_data.linear.x;
    double desired_w1 = (desired_v_x - b*twist_data.angular.z/2)/r;
    double desired_w2 = (desired_v_x + b*twist_data.angular.z/2)/r ;
    double delta_encoder1 = (double) encoder_data.delta_encoder1;
    double delta_encoder2 = (double) encoder_data.delta_encoder2;
    double estimated_w1 = (delta_encoder1*2*M_PI*control_frequency)/(ticks_per_rev);
    double estimated_w2 = (delta_encoder2*2*M_PI*control_frequency)/(ticks_per_rev);



    double error1 = desired_w1 - estimated_w1;
    double int_error1 = int_error1 + error1*dt;
    int pwm1 = (int) alpha*error1 + beta*int_error1;

    double error2 = desired_w2 - estimated_w2;
    double int_error2 = int_error2 + error2*dt;
    int pwm2 = (int) alpha*error2 + beta*int_error2;

    ras_lab1_msgs::PWM msg;
    msg.header.stamp = ros::Time::now();
    msg.header.frame_id = "test";
    msg.PWM1 = pwm1;
    msg.PWM2 = pwm2;

    controller_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
