/** MIT License
Copyright (c) 2017 Miguel Maestre Trueba
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *@copyright Copyright 2017 Miguel Maestre Trueba
 *@file robot.cpp
 *@author Miguel Maestre Trueba
 *@brief Implementation of the methods of class robot.
 *@brief These methods are in charge of receiving messages and moving the robot depending on the information of the msg.
 */

#include <geometry_msgs/Twist.h>
#include <vector>
#include "ros/ros.h"
#include "ros/console.h"
#include "robot.hpp"
#include "traffic_sign_recognition/sign.h"

void robot::signCallback(traffic_sign_recognition::sign msg) {
    // Store the msg data in two different variables
    type = msg.sign_type;
    area = msg.area;

    // When traffic sign is close enough, start action
    if (area > 5200 && area < 6000) {
        area = 0;
        flag = true;
        count++;
        ROS_INFO_STREAM("Number of signs detected: " << count);
    } else {
        flag = false;
    }
}

void robot::command(geometry_msgs::Twist &velocity,
    ros::Publisher &pub, ros::Rate &loop_rate) {
    if (flag == true) {
        //////// MOVE FORWARD //////
        // Set of actions for the robot to do when forward sign is detected.
        if (type == 1) {
            ROS_INFO_STREAM("FORWARD SIGN DETECTED");
            // Reduce speed and go for 2 meters
            ros::Time start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(10.0)) {
                velocity.linear.x = 0.2;
                velocity.angular.z = 0.0;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            // Readjust position
            start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(1.0)) {
                velocity.linear.x = 0.0;
                velocity.angular.z = 0.17;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            // Stop for 3 seconds before moving again
            start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(3.0)) {
                velocity.linear.x = 0.0;
                velocity.angular.z = 0.0;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            type = 0;
            area = 0;
            flag = false;
        }


        ///////// TURN ////////
        // Set of actions for the robot to do when turn sign is detected.
        if (type == 2) {
            ROS_INFO_STREAM("TURN SIGN DETECTED");
            // Reduce speed and go for 2 meters
            ros::Time start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(10.0)) {
                velocity.linear.x = 0.2;
                velocity.angular.z = 0.0;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            // Stop for 3 seconds before moving
            start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(3.0)) {
                velocity.linear.x = 0.0;
                velocity.angular.z = 0.0;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            // Make a turn to the left
            start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(6)) {
                velocity.linear.x = 0.4;
                velocity.angular.z = 0.3;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            type = 0;
            area = 0;
            flag = false;
        }

        ///////// STOP ////////
        // Set of actions for the robot to do when stop sign is detected.
        if (type == 3) {
            ROS_INFO_STREAM("STOP SIGN DETECTED");
            // Reduce speed and move 1 meter ahead
            ros::Time start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(5.0)) {
                velocity.linear.x = 0.2;
                velocity.angular.z = 0.0;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            // Stop in front of stop sign
            start = ros::Time::now();
            while (ros::Time::now() - start < ros::Duration(3.0)) {
                velocity.linear.x = 0.0;
                velocity.angular.z = 0.0;
                pub.publish(velocity);
                loop_rate.sleep();
            }
            type = 0;
            area = 0;
            flag = false;

            ROS_INFO_STREAM("DESTINATION REACHED");
            return;
        }
    }

    if (flag == false) {
        velocity.linear.x = 0.3;
        velocity.angular.z = 0.0;
        pub.publish(velocity);
    }
}
