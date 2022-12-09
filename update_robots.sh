#!/bin/bash

SUCCESS_FILE=robots/compiling.dir

check_robots() {
  rm -f $SUCCESS_FILE
  TMPDIR=$( mktemp -d )
  BASEDIR=$(PWD)
  for robot in students/*; do
    echo -e "\nTesting $robot"
    if (cd $TMPDIR; g++ -c -std=c++2a $BASEDIR/$robot/*.cpp -I $BASEDIR/librobots/include); then
      echo "SUCCESS: $robot compiles robotfully"
      if grep -e "class.*\: *public Robot" $robot/*.h; then
        echo $robot >>$SUCCESS_FILE
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
  mkdir -p robots
  rm -f robots/*{h,cpp}
  NAMES=""
  ALL=robots/all.cpp
  for robot in $(cat $SUCCESS_FILE); do
    for NAME in $(grep -h "class.*\: *public Robot" $robot/*.h | sed -e "s/class \(.*\):.*/\1/"); do
      echo "Got a good robot in $robot with name $NAME"
      NAMES="$NAMES $NAME"
    done
    cp $robot/*.h $robot/*.cpp robots
    for f in $robot/*.cpp; do
      echo "#include \"$(basename $f)\"" >>$ALL
    done
  done

  echo -e "#include <librobots/Robot.h>\n\nvector<Robot*> students(){\n  vector<Robot*> robots;\n" >>$ALL
  for name in $NAMES; do
    echo -e "  robots.push_back(new $name());" >>$ALL
  done
  echo -e "  return robots;\n}\n" >>$ALL
}

check_robots
add_robots
