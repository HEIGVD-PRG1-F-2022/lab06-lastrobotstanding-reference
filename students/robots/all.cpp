#include "robot_1/MyRobot1.cpp"
#include "robot_2/BonusState.cpp"
#include "robot_2/MapInfo.cpp"
#include "robot_2/MeinRobot.cpp"
#include "robot_2/Point.cpp"
#include "robot_3/action.cpp"
#include "robot_3/point.cpp"
#include "robot_3/sonny_robot.cpp"
#include "robot_4/MonRobot.cpp"
#include "robot_5/RobotFB.cpp"
#include "robot_5/RobotFB_diag.cpp"
#include "robot_6/BorisGuillaumator.cpp"
//#include "robot_7/MonRobotGPKD.cpp"
#include "robot_8/RobotTeamR.cpp"
#include "robot_9/RandomRoboto.cpp"
#include "robot_9/Roboto.cpp"
#include "robot_10/SoftIntersRobot.cpp"
#include <librobots/Robot.h>
#include <vector>
  
std::vector<Robot*> students(){
  std::vector<Robot*> robots;

  robots.push_back(new MyRobot1());
  robots.push_back(new MeinRobot());
  robots.push_back(new SonnyRobot());
  robots.push_back(new MonRobot());
  robots.push_back(new RobotFB());
  robots.push_back(new RobotFB_diag());
  robots.push_back(new BorisGuillaumator());
//  robots.push_back(new MonRobotGPKD());
//  robots.push_back(new RobotTeamR());
  robots.push_back(new RandomRoboto());
  robots.push_back(new Roboto());
  robots.push_back(new SoftIntersRobot());
  return robots;
}

