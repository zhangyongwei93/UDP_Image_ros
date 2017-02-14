cmake_minimum_required(VERSION 2.8.3)
project(udp)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   genmsg
  # message_generation
   sensor_msgs
   cv_bridge
   image_transport  
)
find_package(OpenCV REQUIRED)

## Declare ROS messages and services
#add_message_files (DIRECTORY msg FILES ActuatorControl.msg)

## Declare a catkin package
catkin_package(
  INCLUDE_DIRS include
  DEPENDS system_lib opencv
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(client src/Client.cpp src/PracticalSocket.cpp)
target_link_libraries(client ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(server src/Server.cpp src/PracticalSocket.cpp)
target_link_libraries(server ${catkin_LIBRARIES} ${OpenCV_LIBS})


#cmake_minimum_required(VERSION 2.8)
#project( lan_vid_pseudostream )
#find_package( OpenCV REQUIRED )
#add_executable( server Server.cpp PracticalSocket.cpp )
#target_link_libraries( server ${OpenCV_LIBS} )
#add_executable( client Client.cpp PracticalSocket.cpp )
#target_link_libraries( client ${OpenCV_LIBS} )
