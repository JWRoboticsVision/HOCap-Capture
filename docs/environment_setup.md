<h1>Environment Setup (Ubuntu 20.04)</h1>

<h2>Table of Contents</h2>

- [General Environment Information](#general-environment-information)
- [Install ROS Noetic](#install-ros-noetic)
    - [1. Setup sources.list and keys](#1-setup-sourceslist-and-keys)
    - [2. Install ROS-Noetic-Desktop](#2-install-ros-noetic-desktop)
    - [3. Environment setup](#3-environment-setup)
- [Install Intel® RealSense™ SDK (v2.50.0)](#install-intel-realsense-sdk-v2500)
    - [Build from Sources](#build-from-sources)
    - [Method 2: Install from Debian Package](#method-2-install-from-debian-package)
- [Install ROS Wrapper for Intel® RealSense™ Devices (v2.3.2)](#install-ros-wrapper-for-intel-realsense-devices-v232)
    - [Method 1: Install from Source](#method-1-install-from-source)
- [Install Azure Kinect \& Body Tracking SDK](#install-azure-kinect--body-tracking-sdk)
- [Install Azure\_Kinect\_ROS\_Driver](#install-azure_kinect_ros_driver)
- [USB memory on Ubuntu](#usb-memory-on-ubuntu)
- [Install AprilTag ROS](#install-apriltag-ros)

## General Environment Information

| Name                             | Version                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        |
| :------------------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| OS                               | Ubuntu 20.04 LTS with kernel `Linux 5.8.0-63-generic`                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| ROS                              | [Noetic](http://wiki.ros.org/noetic/Installation/Ubuntu)                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| CUDA                             | [v11.8.0](https://developer.nvidia.com/cuda-11-8-0-download-archive)                                                                                                                                                                                                                                                                                                                                                                                                                           |
| Realsense-SDK                    | [v2.50.0](https://github.com/realsenseai/librealsense/releases/tag/v2.50.0)                                                                                                                                                                                                                                                                                                                                                                                                                    |
| Realsense-ROS                    | [v2.3.2](https://github.com/IntelRealSense/realsense-ros/releases/tag/2.3.2)                                                                                                                                                                                                                                                                                                                                                                                                                   |
| Azure Kinect & Body Tracking SDK | [k4a-tools 1.4.1](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/k/k4a-tools/)<br>[libk4a1.4](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4/)<br>[libk4a1.4-dev](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4-dev/)<br>[libk4abt1.1](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.1/)<br>[libk4abt1.1-dev](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.1-dev/) |
| Firmware                         | Realsense D400 [v5.15.0.2](https://dev.intelrealsense.com/docs/firmware-releases) or later<br>Azure Kinect DK [v1.6.110079014](https://download.microsoft.com/download/3/d/6/3d6d9e99-a251-4cf3-8c6a-8e108e960b4b/AzureKinectDK_Fw_1.6.110079014.bin) or [v1.6.110080014](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/tree/develop/firmware)                                                                                                                                          |

## Install [ROS Noetic](http://wiki.ros.org/noetic/Installation/Ubuntu)

#### 1. Setup sources.list and keys

```sh
# setup sources.list
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'

# setup keys
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
```

#### 2. Install ROS-Noetic-Desktop

```sh
sudo apt update && sudo apt install ros-noetic-desktop-full
```

#### 3. Environment setup

- Bash
  ```sh
  # bash
  echo "" >> ~/.bashrc
  echo "# ROS Initialization" >> ~/.bashrc
  echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
  echo "" >> ~/.bashrc
  source ~/.bashrc
  ```
- Zsh
  ```sh
  # zsh
  echo "" >> ~/.zshrc
  echo "# ROS Initialization" >> ~/.zshrc
  echo "source /opt/ros/noetic/setup.zsh" >> ~/.zshrc
  echo "" >> ~/.zshrc
  source ~/.zshrc
  ```
- Install Dependencies for building packages

  ```sh
  sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
  ```

- Initialize rosdep

  ```sh
  sudo rosdep init && rosdep update
  ```

- Verify Noetic installation

  ```sh
  # check ros path
  roscd && pwd

  # start roscore node
  roscore
  ```

## Install [Intel® RealSense™ SDK (v2.50.0)](https://github.com/realsenseai/librealsense/tree/v2.50.0)

#### [Build from Sources](https://github.com/realsenseai/librealsense/blob/v2.50.0/doc/installation.md)

- Clone git Repo

  ```sh
  git clone --recursive git@github.com:IntelRealSense/librealsense.git --branch v2.50.0 &&
  cd librealsense
  ```

- Unplug any Connected Intel RealSense Camera
- Install the Required Core Packages

  ```sh
  sudo apt-get update && sudo apt-get install git libssl-dev libusb-1.0-0-dev libudev-dev pkg-config libgtk-3-dev
  sudo apt-get update && sudo apt-get install libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev at
  ```

- Setting-up Permissions for RealSense Devices

  ```sh
  # setup permissions
  bash ./scripts/setup_udev_rules.sh

  # remove permissions
  bash ./scripts/setup_udev_rules.sh --uninstall
  ```

- Build and Apply Patched Kernel Modules for Ubuntu 20.04 with LTS kernel
  - Ubuntu 20/22 (focal/jammy) with LTS kernel 5.13, 5.15

  ```sh
  bash ./scripts/patch-realsense-ubuntu-lts-hwe.sh
  ```

  - Ubuntu 14/16/18/20 with LTS kernel (< 5.13)

  ```sh
  bash ./scripts/patch-realsense-ubuntu-lts.sh
  ```

- Check the Patched Kernel Modules Installation

  ```sh
  sudo dmesg | tail -n 50
  ```

  The log should indicate that a new uvcvideo driver has been registered.

  > `[ 7837.692511] usbcore: registered new interface driver uvcvideo`

- Building librealsense2 SDK

  Navigate to librealsense root directory and run:

  ```sh
  mkdir build && cd build

  cmake ../ -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=true -DBUILD_GRAPHICAL_EXAMPLES=true -DBUILD_WITH_CUDA=true -DBUILD_WITH_OPENMP=true

  sudo make uninstall && make clean && make -j$(nproc)

  sudo make install
  ```

#### Method 2: Install from Debian Package

- Register the server's public key

  ```sh
  # Ensure the directory exists
  sudo mkdir -p /etc/apt/keyrings
  # Download and dearmor
  curl -sSf https://librealsense.realsenseai.com/Debian/librealsenseai.asc | \
  gpg --dearmor | sudo tee /etc/apt/keyrings/librealsenseai.gpg > /dev/null
  ```

- Add the server to the list of repositories

  ```sh
  echo "deb [signed-by=/etc/apt/keyrings/librealsenseai.gpg] https://librealsense.realsenseai.com/Debian/apt-repo `lsb_release -cs` main" | \
  sudo tee /etc/apt/sources.list.d/librealsense.list
  sudo apt-get update
  ```

- Install the libraries

  ```sh
  # Install the libraries
  sudo apt-get install librealsense2-dkms
  sudo apt-get install librealsense2-utils

  # Install the developer and debug packages
  sudo apt-get install librealsense2-dev
  sudo apt-get install librealsense2-dbg
  ```

## Install [ROS Wrapper for Intel® RealSense™ Devices (v2.3.2)](https://github.com/realsenseai/realsense-ros/tree/2.3.2)

There are two methods to install the realsense-ros (debian package or build from source).
Becauce the debian package is almost behind the one available in RealSense™ official repository, we suggest to install from source.

#### Method 1: Install from Source

- Install required packages

  ```sh
  sudo apt update && sudo apt install libeigen3-dev ros-noetic-ddynamic-reconfigure
  ```

- Clone the latest Intel® RealSense™ ROS into `catkin_ws/src/`

  ```sh
  # create the catkin workspace
  mkdir -p $HOME/catkin_ws/src && cd $HOME/catkin_ws/src/

  # clone the latest RealSense-ROS
  git clone --recursive https://github.com/IntelRealSense/realsense-ros.git && cd realsense-ros/
  git checkout 2.3.2
  ```

- Compile & Install

  ```sh
  # initialize the catkin workspace
  cd $HOME/catkin_ws/src && catkin_init_workspace

  # install the dependencies
  cd $HOME/catkin_ws && rosdep install --from-paths src --ignore-src -r -y
  sudo apt purge ros-noetic-librealsense2

  # compile and install
  catkin_make clean
  catkin_make -DCATKIN_ENABLE_TESTING=False -DCMAKE_BUILD_TYPE=Release -j${nproc}
  catkin_make install
  ```

- Configure Environment

  ```sh
  # bash
  echo "source $HOME/catkin_ws/devel/setup.bash" >> ~/.bashrc
  source ~/.bashrc

  # zsh
  echo "source $HOME/catkin_ws/devel/setup.zsh" >> ~/.zshrc
  source ~/.zshrc
  ```

- In case failed

  ```sh
  cd $HOME/catkin_ws
  unlink src/CMakeLists.txt
  rm -rf build devel .catkin_workspace
  cd $HOME/catkin_ws/src/
  catkin_init_workspace
  cd ..
  rosdep install --from-paths src --ignore-src -r -y
  sudo apt purge ros-noetic-librealsense2
  catkin_make clean
  catkin_make -DCATKIN_ENABLE_TESTING=False -DCMAKE_BUILD_TYPE=Release
  catkin_make install
  ```

- Unit Test
  - Run test

    ```sh
    # start roscore node
    roscore

    # download test .bag files
    cd $HOME/catkin_ws/src/realsense-ros/
    wget --no-check-certificate "https://librealsense.intel.com/rs-tests/TestData/outdoors_1color.bag" -P "records/"
    wget --no-check-certificate "https://librealsense.intel.com/rs-tests/D435i_Depth_and_IMU_Stands_still.bag" -P "records/"

    # run test
    python3 realsense2_camera/scripts/rs2_test.py --all
    ```

  - Test results should be similiar as:

    ```
                         TEST RESULTS
    ---------------------------------------------------
    index test name                score : message
    ----- ----------------------  ------   --------------
    0     depth_avg_decimation_1      OK :
    1     depth_w_cloud_1             OK :
    2     non_existent_file           OK : 'num_channels'
    3     vis_avg_2                   OK :
    4     depth_avg_1                 OK :
    exit (0)
    ```

## Install [Azure Kinect & Body Tracking SDK](https://github.com/microsoft/Azure-Kinect-Sensor-SDK)

- Currently, **the only supported distribution** is `Ubuntu 18.04`. However, we could use them on `Ubuntu 20.04`.
- The Body Tracking SDK recommands a NVIDIA GPU installed on host machine.
- Tested versions on Ubuntu 20.04
- [k4a-tools 1.4.1](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/k/k4a-tools/)
- [libk4a1.4](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4/)
- [libk4a1.4-dev](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4-dev/)
- [libk4abt1.1](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.1/)
- [libk4abt1.1-dev](https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.1-dev/)
- [CUDA v11.8.0](https://developer.nvidia.com/cuda-11-8-0-download-archive)
- Install required packages

```sh
sudo apt update && sudo apt install libsoundio1 ros-noetic-ddynamic-reconfigure
```

- Download k4a packages

  ```sh
  # download .deb packages
  wget https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/k/k4a-tools/k4a-tools_1.4.1_amd64.deb
  wget https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4/libk4a1.4_1.4.1_amd64.deb
  wget https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.4-dev/libk4a1.4-dev_1.4.1_amd64.deb
  wget https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.1/libk4abt1.1_1.1.2_amd64.deb
  wget https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.1-dev/libk4abt1.1-dev_1.1.2_amd64.deb
  ```

- Install .deb pakages

  ```sh
  sudo dpkg -i libk4a1.4_1.4.1_amd64.deb
  sudo dpkg -i libk4a1.4-dev_1.4.1_amd64.deb
  sudo dpkg -i libk4abt1.1_1.1.2_amd64.deb
  sudo dpkg -i libk4abt1.1-dev_1.1.2_amd64.deb
  sudo dpkg -i k4a-tools_1.4.1_amd64.deb
  ```

- Setup `udev` rules

  ```sh
  sudo bash -c "curl https://raw.githubusercontent.com/microsoft/Azure-Kinect-Sensor-SDK/develop/scripts/99-k4a.rules > /etc/udev/rules.d/99-k4a.rules"
  ```

- Verify Installation
  - Verify `k4a`
    ```sh
    k4aviewer
    ```
  - Verify `k4abt`
    ```sh
    k4abt_simple_3d_viewer
    ```

## Install [Azure_Kinect_ROS_Driver](https://github.com/microsoft/Azure_Kinect_ROS_Driver)

- Clone git repo

  ```sh
  # clone git repo
  mkdir -p $HOME/catkin_ws/src && cd $HOME/catkin_ws/src
  git clone --recursive https://github.com/microsoft/Azure_Kinect_ROS_Driver
  ```

- Compile from sources

  ```sh
  # modify k4abtConfig.cmake
  sudo sed -i 's/find_dependency(k4a 1.4 REQUIRED)/find_dependency(k4a 1.4.1 REQUIRED)/g' /usr/lib/cmake/k4abt/k4abtConfig.cmake

  # compile from sources
  cd $HOME/catkin_ws
  catkin_make clean
  catkin_make --force-cmake -DCATKIN_ENABLE_TESTING=False -DCMAKE_BUILD_TYPE=Release
  # catkin_make install
  ```

- Verify Installation

  ```sh
  # start roscore node
  roscore

  # launch kinect ros driver
  roslaunch azure_kinect_ros_driver driver.launch \
      color_enabled:=true color_resolution:=720P \
      depth_enabled:=true depth_mode:=NFOV_UNBINNED \
      fps:=30 \
      point_cloud:=false rgb_point_cloud:=false point_cloud_in_depth_frame:=false \
      body_tracking_enabled:=true
  ```

## USB memory on Ubuntu

Related issue: [485#](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/issues/485#)

According to [this](https://learn.microsoft.com/en-us/azure/kinect-dk/multi-camera-sync#linux-computers-usb-memory-on-ubuntu), to support multiple devices, the USB controller has to have more memory.

- Set the maximum usbfs memory limit permanently

  ```sh
  # modify grub
  sudo vi /etc/default/grub

  # locate GRUB_CMDLINE_LINUX_DEFAULT="quiet splash" and modify as below
  GRUB_CMDLINE_LINUX_DEFAULT="quiet splash usbcore.usbfs_memory_mb=32"

  # update grub
  sudo update-grub

  # restart computer
  sudo reboot
  ```

These commands set the USB memory to 32 MB. This is an example setting at two times the default value. You can set a much larger value, as appropriate for your solution.

- Set the maximum usbfs memory limit temporarily

  ```sh
  # set memory limit to 1000 MB temporarily
  sudo sh -c 'echo 1000 > /sys/module/usbcore/parameters/usbfs_memory_mb'

  # replug the devices
  ```

## Install [AprilTag ROS](https://github.com/AprilRobotics/apriltag_ros)

- Clone git repo

  ```sh
  # clone git repo
  mkdir -p $HOME/catkin_ws/apriltag_ros/src && cd $HOME/catkin_ws/apriltag_ros/src
  git clone --recursive git@github.com:AprilRobotics/apriltag.git
  git clone --recursive git@github.com:AprilRobotics/apriltag_ros.git

  # checkout the latest tag
  cd $HOME/catkin_ws/apriltag_ros/src/apriltag && git checkout `git tag | sort -V | tail -1`
  cd $HOME/catkin_ws/apriltag_ros/src/apriltag_ros && git checkout `git tag | sort -V | tail -1`
  ```

- Compile from sources

  ```sh
  cd $HOME/catkin_ws/apriltag_ros/
  catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release

  echo "source $HOME/catkin_ws/apriltag_ros/devel_isolated/setup.bash" >> $HOME/.bashrc
  echo "source $HOME/catkin_ws/apriltag_ros/devel_isolated/setup.zsh" >> $HOME/.zshrc
  ```

- Set the type of the tag to detect

  ```sh
  cd $HOME/catkin_ws/apriltag_ros/src/apriltag_ros/apriltag_ros/config
  cp settings.yaml settings.yaml.bak
  cp tags.yaml tags.yaml.bak
  ```

  - settings.yaml

    ```yaml
    tag_family:        'tag36h11'
    tagCircle49h12
    tag_threads:       2
    tag_decimate:      1.0
    tag_blur:          0.0
    tag_refine_edges:  1
    tag_debug:         0
    max_hamming_dist:  2
    # Other parameters
    publish_tf:        true
    ```

  - tags.yaml
    ```yaml
    standalone_tags: [{ id: 0, size: 0.172 }, { id: 1, size: 0.172 }]
    ```
