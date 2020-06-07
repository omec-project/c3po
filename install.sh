#! /bin/bash
# Copyright 2019-present Open Networking Foundation

#Copyright (c) 2017 Sprint
#
# SPDX-License-Identifier: Apache-2.0

cd $(dirname ${BASH_SOURCE[0]})
export C3PO_DIR=$PWD
echo "------------------------------------------------------------------------------"
echo " C3PO_DIR exported as $C3PO_DIR"
echo "------------------------------------------------------------------------------"

OSDIST=`lsb_release -is`
OSVER=`lsb_release -rs`

#
# Sets QUIT variable so script will finish.
#
quit()
{
  QUIT=$1
}

# Shortcut for quit.
q()
{
  quit
}

################################################################################

step_1()
{
    TITLE="Environment setup."
    CONFIG_NUM=1
    TEXT[1]="Check OS and network connection"
    FUNC[1]="setup_env"
    CPLT[1]=$SETUP_ENV_COMPLETE
}

setup_env()
{
  # a. Check for OS and version dependencies
  case "$OSDIST" in
    Ubuntu)
      case "$OSVER" in
        14.04|16.04) echo "$OSDIST version $OSVER is supported." ;;
        *) echo "$OSDIST version $OSVER is unsupported." ; exit;;
      esac
      ;;
    *) echo "ERROR: Unsupported operating system distribution: $OSDIST"; exit;;
  esac

  echo
  echo "Checking network connectivity..."
  # b. Check for internet connections
  wget -T 3 -t 3 --spider http://www.google.com 2>/dev/null
  if [ "$?" != 0 ]; then
    while true; do
      read -p "No Internet connection. Are you behind a proxy (y/n)? " yn
      case $yn in
        [Yy]* ) $SETUP_PROXY ; return;;
        [Nn]* ) echo "Please check your internet connection..." ; exit;;
        * ) "Please answer yes or no.";;
      esac
    done
  fi

  SETUP_ENV_COMPLETE="- COMPLETE"
}

################################################################################

step_2()
{
  TITLE="Download and Install"
  CONFIG_NUM=1
  TEXT[1]="Agree to download"
  FUNC[1]="get_agreement_download"
  CPLT[1]=$GET_AGREEMENT_DOWNLOAD_COMPLETE

  case $OSDIST in
    Ubuntu)
      case "$OSVER" in
        14.04)
          TEXT[2]="Download and install Cmake 3.5.1"
          FUNC[2]="install_cmake"
          CPLT[2]=$INSTALL_CMAKE_COMPLETE

          TEXT[3]="Download and install libtool 2.4.6"
          FUNC[3]="install_libtool"
          CPLT[3]=$INSTALL_LIBTOOL_COMPLETE

          TEXT[4]="Download packages"
          FUNC[4]="install_libs"
          CPLT[4]=$INSTALL_LIBS_COMPLETE
          ;;
        16.04)
          TEXT[2]="Download packages"
          FUNC[2]="install_libs"
          CPLT[2]=$INSTALL_LIBS_COMPLETE
          ;;
        *) echo "$OSDIST version $OSVER is unsupported." ; exit;;
      esac
      ;;
    *) echo "ERROR: Unsupported operating system distribution: $OSDIST"; exit;;
  esac
}

get_agreement_download()
{
  echo
  echo "List of packages needed for C3PO build and installation:"
  echo "-------------------------------------------------------"

  case $OSDIST in
    Ubuntu)
      case $OSVER in
        14.04)
          echo "1. Cmake version 3.5.1"
          echo "2. libtool version 2.4.6"
          echo "3. Other libraries including :"
          echo "     gcc g++ libuv-dev libssl-dev automake libmemcached-dev memcached bison"
          echo "     flex libsctp-dev libgnutls-dev libgcrypt-dev libidn11-dev nettle-dev"
          ;;
        16.04)
          echo "gcc g++ make cmake libuv-dev libssl-dev autotools-dev libtool-bin"
          echo "m4 automake libmemcached-dev memcached cmake-curses-gui gcc bison"
          echo "flex libsctp-dev libgnutls-dev libgcrypt-dev libidn11-dev nettle-dev"
          ;;
        *) echo "$OSDIST version $OSVER is unsupported." ; exit;;
      esac
      ;;
    *) echo "ERROR: Unsupported operating system distribution: $OSDIST"; exit;;
  esac

  while true; do
    echo
    read -p "We need to download and install the above mentioned packages. Press (y/n) to continue? " yn
    case $yn in
      [Yy]* )
        touch .agree
        break;;
      [Nn]* ) exit;;
      * ) "Please answer yes or no.";;
    esac
  done

  GET_AGREEMENT_DOWNLOAD_COMPLETE="- COMPLETE"
}

install_cmake()
{
  file_name=".agree"
  if [ ! -e "$file_name" ]; then
    echo "Please choose option '2.Agree to download' first"
    return
  fi
  sudo apt-get purge cmake cmake-curses-gui
  sudo apt-get install g++ make autotools-dev m4 libncurses-dev
  pushd modules
  rm -rf cmake-3.5.1
  rm -rf cmake-3.5.1.tar.gz
  wget https://cmake.org/files/v3.5/cmake-3.5.1.tar.gz
  tar -xvf cmake-3.5.1.tar.gz
  cd cmake-3.5.1
  ./bootstrap && make -j4 && sudo make install
  popd

  INSTALL_CMAKE_COMPLETE="-COMPLETE"
}

install_libtool()
{
  file_name=".agree"
  if [ ! -e "$file_name" ]; then
    echo "Please choose option '2.Agree to download' first"
    return
  fi
  pushd modules
  rm -rf libtool-2.4.6
  rm -rf libtool-2.4.6.tar.gz
  wget http://ftp.gnu.org/gnu/libtool/libtool-2.4.6.tar.gz
  tar -xvf libtool-2.4.6.tar.gz
  cd libtool-2.4.6
  ./configure && make && sudo make install
  popd

  INSTALL_LIBTOOL_COMPLETE="- COMPLETE"
}

install_libs()
{
  echo "Install packages needed to build and run the C3PO components..."
  file_name=".agree"
  if [ ! -e "$file_name" ]; then
    echo "Please choose option '2.Agree to download' first"
    return
  fi
  file_name=".download"
  if [ -e "$file_name" ]; then
    clear
    INSTALL_LIBS_COMPLETE="- COMPLETE"
    return
  fi
  sudo apt-get update > /dev/null
  case $OSDIST in
    Ubuntu)
      case "$OSVER" in
        14.04) sudo apt-get install libuv-dev libssl-dev automake libmemcached-dev memcached gcc bison flex libsctp-dev libgnutls-dev libgcrypt-dev libidn11-dev nettle-dev ;;
        16.04) sudo apt-get install g++ make cmake libuv-dev libssl-dev autotools-dev libtool-bin m4 automake libmemcached-dev memcached cmake-curses-gui gcc bison flex libsctp-dev libgnutls-dev libgcrypt-dev libidn11-dev nettle-dev ;;
        *) echo "$OSDIST version $OSVER is unsupported." ; exit;;
      esac
      ;;
    *) echo "ERROR: Unsupported operating system distribution: $OSDIST"; exit;;
  esac
  touch .download

  INSTALL_LIBS_COMPLETE="- COMPLETE"
}

################################################################################

step_3()
{
  TITLE="Build and Install Project Submodules"
  CONFIG_NUM=1
  TEXT[1]="Build and install the submodules"
  FUNC[1]="init_submodules"
  CPLT[1]=$INIT_SUBMODULES_COMPLETE
#  TEXT[2]="Build and install freeDiameter"
#  FUNC[2]="build_freeDiameter"
#  CPLT[2]=$BUILD_FREEDIAMETER_COMPLETE
#  TEXT[3]="Build and install c-ares"
#  FUNC[3]="build_c_ares"
#  CPLT[3]=$BUILD_C_ARES_COMPLETE
#  TEXT[4]="Build and install cpp-driver"
#  FUNC[4]="build_cpp_driver"
#  CPLT[4]=$BUILD_CPP_DRIVER_COMPLETE
#  TEXT[5]="Build and install pistache"
#  FUNC[5]="build_pistache"
#  CPLT[5]=$BUILD_PISTACHE_COMPLETE
}

init_submodules()
{
  git submodule init
  git submodule update

  build_freeDiameter
  build_c_ares
  build_cpp_driver
  build_pistache
  build_rapidjson
  sudo ldconfig

  INIT_SUBMODULES_COMPLETE="- COMPLETE"
}

build_freeDiameter()
{
  pushd modules/freeDiameter
  rm -rf build
  mkdir -p build
  cd build
  cmake ..
  awk '{if (/^DISABLE_SCTP/) gsub(/OFF/, "ON"); print}' CMakeCache.txt > tmp && mv tmp CMakeCache.txt
  make
  sudo make install
  popd

  BUILD_FREEDIAMETER_COMPLETE="- COMPLETE"
}

build_c_ares()
{
  pushd modules/c-ares
  ./buildconf
  ./configure
  make
  sudo make install
  popd

  BUILD_C_ARES_COMPLETE="- COMPLETE"
}

build_cpp_driver()
{
  pushd modules/cpp-driver
  rm -rf build
  mkdir -p build
  cd build
  cmake ..
  make
  sudo make install
  popd

  BUILD_CPP_DRIVER_COMPLETE="- COMPLETE"
}

build_pistache()
{
  pushd modules/pistache
  rm -rf build
  mkdir -p build
  cd build
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
  make
  sudo make install
  popd

  BUILD_PISTACHE_COMPLETE="- COMPLETE"
}

build_rapidjson() {
	echo "Installing rapidjson"
	pushd modules/rapidjson
  rm -rf build
	mkdir -p build
  cd build
	cmake ..
	make
	sudo make install
  popd
}

################################################################################

step_4()
{
  TITLE="Build all C3PO components"
  CONFIG_NUM=1
  TEXT[1]="Build all C3PO components"
  FUNC[1]="build_c3po"
  CPLT[1]=$BUILD_C3PO_COMPLETE
}

build_c3po()
{
  make clean
  make

  BUILD_C3PO_COMPLETE="- COMPLETE"
}

################################################################################

SETUP_PROXY="setup_http_proxy"
STEPS[1]="step_1"
STEPS[2]="step_2"
STEPS[3]="step_3"
STEPS[4]="step_4"

QUIT=0

# clear the .argee file
rm -rf .agree

clear

#echo -n "Checking for user permission.. "
#sudo -n true
#if [ $? -ne 0 ]; then
#   echo "Password-less sudo user must run this script" 1>&2
#   exit 1
#fi
#echo "Done"
#clear

while [ "$QUIT" == "0" ]; do
  OPTION_NUM=1
  for s in $(seq ${#STEPS[@]}) ; do
    ${STEPS[s]}

    echo "----------------------------------------------------------"
    echo " Step $s: ${TITLE}"
    echo "----------------------------------------------------------"

    for i in $(seq ${#TEXT[@]}) ; do
      echo "[$OPTION_NUM] ${TEXT[i]} ${CPLT[i]}"
      OPTIONS[$OPTION_NUM]=${FUNC[i]}
      let "OPTION_NUM+=1"
    done

    # Clear TEXT and FUNC arrays before next step
    unset TEXT
    unset FUNC
    unset CPLT

    echo ""
  done

  echo "[$OPTION_NUM] Exit Script"
  OPTIONS[$OPTION_NUM]="quit"
  echo ""
  echo -n "Option: "
  read our_entry

  ${OPTIONS[our_entry]} ${our_entry}

  if [ "$QUIT" == "0" ] ; then
    echo
    echo -n "Press enter to continue ..."; read
    clear
    continue
    exit
  fi
  echo "Installation complete. Please refer to README.md for more information"
done

