// (c) 2021 Copyright, Real-Time Innovations, Inc.  All rights reserved.
//
// RTI grants Licensee a license to use, modify, compile, and create derivative
// works of the Software.  Licensee has the right to distribute object form
// only for use with RTI products.  The Software is provided "as is", with no
// warranty of any type, including any warranty for fitness for any purpose.
// RTI is under no obligation to maintain or support the Software.  RTI shall
// not be liable for any incidental or consequential damages arising out of the
// use or inability to use the software.

#include <string>

#include <rti/ros2/ping/publisher.hpp>
#include <rti/ros2/ping/subscriber.hpp>
#include <rti/ros2/data/memory.hpp>

#include "std_msgs/msg/String.hpp"

#include "rclcpp_components/register_node_macro.hpp"

#include "connext_node_helpers/visibility_control.h"

/******************************************************************************
 * PingPongPublisher implementation for std_msgs::msg::String
 ******************************************************************************/
class StringPingPublisher :
  public rti::ros2::ping::PingPongPublisher<std_msgs::msg::String>
{
public:
  CONNEXT_NODE_HELPERS_PUBLIC
  StringPingPublisher(const rclcpp::NodeOptions & options)
  : PingPongPublisher("string_pub", options)
  {
    this->init_test();
  }

protected:
  virtual std_msgs::msg::String * alloc_sample()
  {
    return &msg_;
  }

  virtual void prepare_ping(std_msgs::msg::String & ping, const bool final)
  {
    if (final) {
      ping.data(std::to_string(0));
      return;
    }

    ping.data(std::to_string(this->participant_->current_time().to_microsecs()));
  }

  // Process received pong sample and return the timestamp
  virtual void process_pong(
    dds::sub::LoanedSamples<std_msgs::msg::String> & pong_samples,
    uint64_t & pong_timestamp)
  {
    pong_timestamp = std::stoull(pong_samples[0].data().data(), nullptr, 0);
  }

  std_msgs::msg::String msg_;
};

RCLCPP_COMPONENTS_REGISTER_NODE(StringPingPublisher)

/******************************************************************************
 * PingPongSubscriber implementation for std_msgs::msg::String
 ******************************************************************************/
class StringPingSubscriber :
  public rti::ros2::ping::PingPongSubscriber<std_msgs::msg::String>
{
public:
  CONNEXT_NODE_HELPERS_PUBLIC
  StringPingSubscriber(const rclcpp::NodeOptions & options)
  : PingPongSubscriber("string_sub", options)
  {
    this->init_test();
  }

protected:
  virtual std_msgs::msg::String * alloc_sample()
  {
    return &msg_;
  }

  virtual void prepare_pong(
    std_msgs::msg::String * const pong, const uint64_t ping_ts)
  {
    pong->data(std::to_string(ping_ts));
  }

  virtual void process_ping(
    dds::sub::LoanedSamples<std_msgs::msg::String> & ping_samples,
    uint64_t & ping_timestamp)
  {
    ping_timestamp = std::stoull(ping_samples[0].data().data(), nullptr, 0);
  }

  virtual void dump_ping(
    dds::sub::LoanedSamples<std_msgs::msg::String> & ping_samples,
    std::ostringstream & msg)
  {
    msg << ping_samples[0].data().data();
  }

  std_msgs::msg::String msg_;
};

RCLCPP_COMPONENTS_REGISTER_NODE(StringPingSubscriber)
