// Copyright (c) 2019 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NAV2_TASKS__GOAL_REACHED_CONDITION_HPP_
#define NAV2_TASKS__GOAL_REACHED_CONDITION_HPP_

#include <string>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "behaviortree_cpp/condition_node.h"
#include "nav2_robot/robot.hpp"
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
//#include "nav2_tasks/compute_path_to_pose_task.hpp"

namespace nav2_tasks
{

class GoalReachedCondition : public BT::ConditionNode
{
public:
  explicit GoalReachedCondition(const std::string & condition_name)
  : BT::ConditionNode(condition_name), initialized_(false)
  {
  }

  GoalReachedCondition() = delete;

  ~GoalReachedCondition()
  {
  }

  BT::NodeStatus tick() override
  {
    if (!initialized_) {
      initialize();
    }

    if (goalReached()) {
      return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::FAILURE;
  }

  void initialize()
  {
    node_ = blackboard()->template get<rclcpp::Node::SharedPtr>("node");
    node_->get_parameter_or<double>("goal_reached_tol", goal_reached_tol_, 0.25);
    //robot_ = std::make_unique<nav2_robot::Robot>(node_);
    initialized_ = true;
  }

  bool
  goalReached()
  {
#if 0
    auto current_pose = std::make_shared<geometry_msgs::msg::PoseWithCovarianceStamped>();

    if (!robot_->getCurrentPose(current_pose)) {
      RCLCPP_DEBUG(node_->get_logger(), "Current robot pose is not available.");
      return false;
    }
    // TODO(mhpanah): replace this with a function
    blackboard()->get<nav2_tasks::ComputePathToPoseCommand::SharedPtr>("goal", command_);
    double dx = command_->pose.position.x - current_pose->pose.pose.position.x;
    double dy = command_->pose.position.y - current_pose->pose.pose.position.y;

    if ( (dx * dx + dy * dy) <= (goal_reached_tol_ * goal_reached_tol_) ) {
      return true;
    } else {
      return false;
    }
#else
  return false;
#endif
  }

private:
  rclcpp::Node::SharedPtr node_;
  std::unique_ptr<nav2_robot::Robot> robot_;
  geometry_msgs::msg::PoseStamped::SharedPtr goal_;
  geometry_msgs::msg::PoseStamped::SharedPtr command_;

  bool initialized_;
  double goal_reached_tol_;
};

}  // namespace nav2_tasks

#endif  // NAV2_TASKS__GOAL_REACHED_CONDITION_HPP_
