#!/bin/bash

SUCCESS_FILE=robots/compiling.dir

check_robots() {
  rm -f $SUCCESS_FILE

  for robot in students/*; do
    echo -e "\nTesting $robot"
    if gcc -c -std=c++2a $robot/*.cpp -I librobots/include -o /dev/null; then
      echo "SUCCESS: $robot compiles robotfully"
      if grep -e "class.*\: *public Robot" $robot/*.h; then
        echo $robot >>$SUCCESS_FILE
        echo "SUCCESS: $robot implements 'Robot' virtual class"
      else
        echo "FAILURE: $robot doesn't implement 'Robot' virtual class"
      fi
    fi
  done
}

add_robots() {
  rm -rf robots
  mkdir robots
  NAMES=""
  ALL=robots/all.cpp
  for robot in $(cat $SUCCESS_FILE); do
    if [[ $(grep -c "class.*\: *public Robot" $robot/*.h) != 1 ]]; then
      echo "More than 1 robot - cannot handle this yet"
      continue
    fi
    NAME=$(grep "class.*\: *public Robot" $robot/*.h | sed -e "s/class \(.*\):.*/\1/")
    echo "Got a good robot in $robot with name $NAME"
    NAMES="$NAMES $NAME"
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

add_robots
