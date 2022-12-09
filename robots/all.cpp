#include "MonRobot.cpp"
//#include "RobotFB.cpp"
//#include "RobotFB_diag.cpp"
#include "MonRobotGPKD.cpp"
#include "RobotTeamR.cpp"
#include "SoftIntersRobot.cpp"
#include <librobots/Robot.h>

vector<Robot*> students(){
  vector<Robot*> robots;

  robots.push_back(new MonRobot());
//  robots.push_back(new RobotFB());
//  robots.push_back(new MonRobotGPKD());
  robots.push_back(new RobotTeamR());
  robots.push_back(new SoftIntersRobot());
  return robots;
}

