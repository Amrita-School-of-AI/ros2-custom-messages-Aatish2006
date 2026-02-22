#include "rclcpp/rclcpp.hpp"
#include "ros2_custom_msgs/msg/robot_status.hpp"
#include <chrono>

using namespace std::chrono_literals;

class StatusPublisher : public rclcpp::Node
{
public:
  StatusPublisher()
  : Node("status_publisher"),
    battery_level_(100.0),
    mission_count_(0)
  {
    publisher_ = this->create_publisher<ros2_custom_msgs::msg::RobotStatus>(
      "/robot_status", 10);

    timer_ = this->create_wall_timer(
      1000ms,
      std::bind(&StatusPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = ros2_custom_msgs::msg::RobotStatus();

    // Required fixed robot name
    message.robot_name = "Explorer1";

    // Decrement battery first (as required)
    battery_level_ -= 0.5;

    // Increment mission count first
    mission_count_++;

    message.battery_level = battery_level_;
    message.is_active = true;
    message.mission_count = mission_count_;

    publisher_->publish(message);

    // Exact required logging format
    RCLCPP_INFO(
      this->get_logger(),
      "Robot: %s, Battery: %.2f, Active: %s, Missions: %d",
      message.robot_name.c_str(),
      message.battery_level,
      message.is_active ? "true" : "false",
      message.mission_count
    );
  }

  rclcpp::Publisher<ros2_custom_msgs::msg::RobotStatus>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  double battery_level_;
  int mission_count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusPublisher>());
  rclcpp::shutdown();
  return 0;
}
