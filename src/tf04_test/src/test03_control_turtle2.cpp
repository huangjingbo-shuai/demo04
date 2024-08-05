/*

需求:
    现有坐标系统，父级坐标系统 world,下有两子级系统 son1，son2，
    son1 相对于 world，以及 son2 相对于 world 的关系是已知的，
    求 son1 与 son2中的坐标关系，又已知在 son1中一点的坐标，要求求出该点在 son2 中的坐标
实现流程:
    1.包含头文件
    2.初始化 ros 节点
    3.创建 ros 句柄
    4.创建 TF 订阅对象
    5.解析订阅信息中获取 son1 坐标系原点在 son2 中的坐标
      解析 son1 中的点相对于 son2 的坐标
    6.spin

*/
//1.包含头文件
#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/Twist.h"


int main(int argc, char *argv[])
{   setlocale(LC_ALL,"");
    // 2.初始化 ros 节点
    ros::init(argc,argv,"sub_frames");
    // 3.创建 ros 句柄
    ros::NodeHandle nh;
    // 4.创建 TF 订阅对象
    tf2_ros::Buffer buffer; 
    tf2_ros::TransformListener listener(buffer);
    // 5.解析订阅信息中获取 son1 坐标系原点在 son2 中的坐标
    ros::Publisher pub=nh.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel",100);
    ros::Rate r(10);
    while (ros::ok())
    {
        try
        {
        //   解析 son1 中的点相对于 son2 的坐标
            geometry_msgs::TransformStamped tfs = buffer.lookupTransform("turtle2","turtle1",ros::Time(0));
            // ROS_INFO("turtle1 相对于 turtle2 的坐标关系:父坐标系ID=%s",tfs.header.frame_id.c_str());
            // ROS_INFO("turtle1 相对于 turtle2 的坐标关系:子坐标系ID=%s",tfs.child_frame_id.c_str());
            // ROS_INFO("turtle1 相对于 turtle2 的坐标关系:x=%.2f,y=%.2f,z=%.2f",
            //         tfs.transform.translation.x,
            //         tfs.transform.translation.y,
            //         tfs.transform.translation.z
            //         );
            geometry_msgs::Twist twist;

            twist.linear.x=0.5*sqrt(pow(tfs.transform.translation.x,2)+pow(tfs.transform.translation.y,2));
            twist.angular.z=4*atan2(tfs.transform.translation.y,tfs.transform.translation.x);
            pub.publish(twist);
        }
        catch(const std::exception& e)
        {
            // std::cerr << e.what() << '\n';
            ROS_INFO("异常信息:%s",e.what());
        }


        r.sleep();
        // 6.spin
        ros::spinOnce();
    }
    return 0;
}