#include "MyRobot1.cpp"
//#include "BonusState.cpp"
//#include "MapInfo.cpp"
//#include "MeinRobot.cpp"
//#include "Point.cpp"
#include "BorisGuillaumator.cpp"
#include "MonRobot.cpp"
//#include "MonRobotGPKD.cpp"
#include "RandomRoboto.cpp"
#include "RobotFB.cpp"
#include "RobotFB_diag.cpp"
#include "RobotTeamR.cpp"
#include "Roboto.cpp"
#include "SoftIntersRobot.cpp"
#include "action.cpp"
#include "point.cpp"
#include "sonny_robot.cpp"
#include <librobots/Robot.h>

vector<Robot *> students() {
    vector<Robot *> robots;

  robots.push_back(new MyRobot1());
//  robots.push_back(new MeinRobot());
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
