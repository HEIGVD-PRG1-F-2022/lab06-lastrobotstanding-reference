#include "robot_1/RobotPA.cpp"
#include "robot_2/MyRobot1.cpp"
#include "robot_3/MapInfo.cpp"
#include "robot_3/MeinRobot.cpp"
#include "robot_3/Point2D.cpp"
#include "robot_4/action.cpp"
#include "robot_4/point.cpp"
#include "robot_4/sonny_robot.cpp"
#include "robot_5/MonRobot.cpp"
#include "robot_6/RobotFB.cpp"
#include "robot_6/RobotFB_diag.cpp"
#include "robot_7/BorisGuillaumator.cpp"
#include "robot_8/MonRobotGPKD_v2.cpp"
#include "robot_9/MyRobot.cpp"
#include "robot_10/RobotTeamR.cpp"
#include "robot_11/RandomRoboto.cpp"
#include "robot_11/Roboto.cpp"
#include "robot_12/SoftIntersRobot.cpp"
#include "robot_13/Pipou.cpp"
#include <librobots/Robot.h>
#include <vector>
  
std::vector<Robot*> students(){
  std::vector<Robot*> robots;

  robots.push_back(new RobotPA());
  robots.push_back(new MyRobot1());
  robots.push_back(new MeinRobot());
  robots.push_back(new SonnyRobot());
  robots.push_back(new MonRobot());
  robots.push_back(new RobotFB_diag());
  robots.push_back(new BorisGuillaumator());
  robots.push_back(new MonRobotGPKD_v2());
  robots.push_back(new MyRobot());
  robots.push_back(new RobotTeamR());
  robots.push_back(new Roboto());
  robots.push_back(new SoftIntersRobot());
  robots.push_back(new Pipou());
  return robots;
}

