#include <geometry_msgs/Twist.h>
#include <jetbot_teleop_msgs/cmd_raw.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

class JetbotTeleop
{
public:
  JetbotTeleop();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  ros::Publisher raw_pub_;
  ros::Subscriber joy_sub_;
};

JetbotTeleop::JetbotTeleop()
{
  raw_pub_ = nh_.advertise<jetbot_teleop_msgs::cmd_raw>("/jetbot_motors/cmd_raw", 1);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &JetbotTeleop::joyCallback, this);
}

void JetbotTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  // geometry_msgs::Twist twist;
  // twist.angular.z = a_scale_*joy->axes[angular_];
  // twist.linear.x = l_scale_*joy->axes[linear_];
  jetbot_teleop_msgs::cmd_raw msg_raw;

  if ((joy->axes[6] != 0) || (joy->axes[7] != 0))
  {
    msg_raw.left = joy->axes[7];
    msg_raw.right = joy->axes[7];
    if (joy->axes[6] == 1)
    {
      msg_raw.left = 0;
    }
    else if (joy->axes[6] == -1)
    {
      msg_raw.right = 0;
    }
    if ((joy->axes[6] != 1) && (joy->axes[7] == 0))
    {
      msg_raw.left = 1;
      msg_raw.right = -1;
    }
    else if ((joy->axes[6] != -1) && (joy->axes[7] == 0))
    {
      msg_raw.left = -1;
      msg_raw.right = 1;
    }
  }
  else
  {
    msg_raw.left = joy->axes[1];
    msg_raw.right = joy->axes[3];
  }
  raw_pub_.publish(msg_raw);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "jetbot_teleop");
  JetbotTeleop jetbot_teleop;

  ros::spin();
}