cmake_minimum_required(VERSION 2.8.3)
project(rosplan_interface_turtlebot2)

add_compile_options(-std=c++11)
add_definitions(-Wno-deprecated-declarations)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  std_srvs
  diagnostic_msgs
  geometry_msgs
  rosplan_dispatch_msgs
  rosplan_knowledge_msgs
  rosplan_planning_system
  actionlib
  kobuki_msgs
  tf
)

###################################
## catkin specific configuration ##
###################################

## Declare things to be passed to dependent projects
catkin_package(
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp rospy std_msgs std_srvs diagnostic_msgs geometry_msgs rosplan_dispatch_msgs rosplan_knowledge_msgs actionlib kobuki_msgs tf
  DEPENDS
)

###########
## Build ##
###########

## include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

## Declare cpp executables
add_executable(rpdocker src/RPDocker.cpp)
add_dependencies(rpdocker ${catkin_EXPORTED_TARGETS})

add_executable(rplocaliser src/RPLocaliser.cpp)
add_dependencies(rplocaliser ${catkin_EXPORTED_TARGETS})

add_executable(rptalker src/RPTalker.cpp)
add_dependencies(rptalker ${catkin_EXPORTED_TARGETS})

add_executable(rpasker src/RPAsker.cpp)
add_dependencies(rpasker ${catkin_EXPORTED_TARGETS})

add_executable(rpwait src/RPWaitPapers.cpp)
add_dependencies(rpwait ${catkin_EXPORTED_TARGETS})

## Specify libraries against which to link a library or executable target
target_link_libraries(rpdocker ${catkin_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(rplocaliser ${catkin_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(rptalker ${catkin_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(rpasker ${catkin_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(rpwait ${catkin_LIBRARIES} ${Boost_LIBRARIES})
