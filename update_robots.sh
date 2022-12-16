#!/bin/bash

STUDENTS_DIR=students/raw
ROBOTS_SUCCESS=$STUDENTS_DIR/compiling.dir
ROBOTS_DIR=students/robots
ROBOTS_ALL=$ROBOTS_DIR/all.cpp
BASEDIR=$(PWD)
mkdir -p $ROBOTS_DIR

check_robots() {
  rm -f $ROBOTS_SUCCESS
  TMPDIR=$( mktemp -d )
  for robot in $STUDENTS_DIR/*; do
    echo -e "\nTesting $robot"
    if (cd $TMPDIR; g++ -c -std=c++2a $BASEDIR/$robot/*.cpp -I $BASEDIR/librobots/include); then
      echo "SUCCESS: $robot compiles robotfully"
      if grep -e "class.*\: *public Robot" $robot/*.h; then
        echo $robot >>$ROBOTS_SUCCESS
        echo "SUCCESS: $robot implements 'Robot' virtual class"
      else
        echo "FAILURE: $robot doesn't implement 'Robot' virtual class"
      fi
    fi
  done
  rm -rf $TMPDIR
  echo
}

add_robots() {
  mkdir -p $ROBOTS_DIR
  rm -f $ROBOTS_DIR/*{h,cpp}
  NAMES=""
  ROBOT_NBR=1
  for robot in $(cat $ROBOTS_SUCCESS); do
    ROBOT_TEAM="robot_$ROBOT_NBR"
    for NAME in $(grep -h "class.*\: *public Robot" $robot/*.h | sed -e "s/class \(.*\):.*/\1/"); do
      echo "Got a good robot in $robot with name $NAME"
      NAMES="$NAMES $NAME"
#      NAMES="$NAMES $ROBOT_TEAM::$NAME"
    done
    ROBOT_DIR="$ROBOTS_DIR/$ROBOT_TEAM"
    mkdir -p $ROBOT_DIR
    cp $robot/*.h $robot/*.cpp $ROBOT_DIR
#    echo "namespace $ROBOT_TEAM {" >> $ROBOTS_ALL
    for f in $robot/*.cpp; do
      echo "#include \"$ROBOT_TEAM/$(basename $f)\"" >>$ROBOTS_ALL
    done
#    echo "}" >> $ROBOTS_ALL
    ROBOT_NBR=$(( ROBOT_NBR + 1 ))
  done

  echo -e "#include <librobots/Robot.h>\n#include <vector>
  \nstd::vector<Robot*> students(){\n  std::vector<Robot*> robots;\n" >> $ROBOTS_ALL
  for name in $NAMES; do
    echo -e "  robots.push_back(new $name());" >>$ROBOTS_ALL
  done
  echo -e "  return robots;\n}\n" >>$ROBOTS_ALL
}

check_robots
add_robots
