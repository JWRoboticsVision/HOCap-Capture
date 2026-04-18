<h1>Calibration Build Guide</h1>

<h2>Table of Contents</h2>

- [Install Dependencies](#install-dependencies)
- [Build and Install ARPG](#build-and-install-arpg)
  - [Preparation](#preparation)
    - [Install ceres-solver](#install-ceres-solver)
  - [Install ARPG](#install-arpg)
    - [Create Folders](#create-folders)
    - [Update Environment Variables](#update-environment-variables)
    - [Install Sophus](#install-sophus)
    - [Install CVars](#install-cvars)
    - [Install Pangolin](#install-pangolin)
    - [Install Calibu](#install-calibu)
    - [Install HAL](#install-hal)
    - [Install Vicalib](#install-vicalib)
- [Create Calibration Pattern](#create-calibration-pattern)

## Install Dependencies

```sh
sudo apt-get update && sudo apt install \
  software-properties-common \
  build-essential cmake wget \
  libeigen3-dev \
  libgoogle-glog-dev \
  libtinyxml2-dev \
  libopencv-dev \
  libprotobuf-dev \
  protobuf-compiler \
  libgl1-mesa-dev \
  freeglut3-dev libglu1-mesa-dev libglew-dev \
  doxygen \
  imagemagick
```

## Build and Install ARPG

### Preparation

- Set Environment Variables

```sh
CLIB=$PWD
```

- Make sure gcc version is 9.x.x

```sh
gcc --version
```

#### Install ceres-solver

- Create Folders
  ```sh
  mkdir -p $CLIB/ceres-solver-1.14.0/release
  mkdir -p $CLIB/ceres-solver-1.14.0/build
  ```
- Build and Install
  ```sh
  cd $CLIB/ceres-solver-1.14.0/build
  cmake .. \
    -DCMAKE_BUILD_TYPE=RELEASE \
    -DCMAKE_INSTALL_PREFIX=$CLIB/ceres-solver-1.14.0/release
  make -j${nproc}
  make test
  make install
  ./bin/simple_bundle_adjuster ../data/problem-16-22106-pre.txt
  ```

### Install ARPG

#### Create Folders

```sh
mkdir -p $CLIB/arpg-k4a-realsense2/releases
mkdir -p $CLIB/arpg-k4a-realsense2/builds
```

#### Update Environment Variables

- bash

```sh
echo "" >> $HOME/.bashrc
echo "# for arpg-k4a-realsense2" >> $HOME/.bashrc
echo "export PATH=$CLIB/arpg-k4a-realsense2/releases/bin${PATH:+:${PATH}}" >> $HOME/.bashrc
echo "export LD_LIBRARY_PATH=$CLIB/arpg-k4a-realsense2/releases/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}" >> $HOME/.bashrc
echo "export LIBRARY_PATH=$CLIB/arpg-k4a-realsense2/releases/lib${LIBRARY_PATH:+:${LIBRARY_PATH}}" >> $HOME/.bashrc
echo "export C_INCLUDE_PATH=$CLIB/arpg-k4a-realsense2/releases/include${C_INCLUDE_PATH:+:${C_INCLUDE_PATH}}" >> $HOME/.bashrc
echo "export CPLUS_INCLUDE_PATH=$CLIB/arpg-k4a-realsense2/releases/include${CPLUS_INCLUDE_PATH:+:${CPLUS_INCLUDE_PATH}}" >> $HOME/.bashrc
```

- zsh

```sh
echo "" >> $HOME/.zshrc
echo "# for arpg-k4a-realsense2" >> $HOME/.zshrc
echo "export PATH=$CLIB/arpg-k4a-realsense2/releases/bin${PATH:+:${PATH}}" >> $HOME/.zshrc
echo "export LD_LIBRARY_PATH=$CLIB/arpg-k4a-realsense2/releases/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}" >> $HOME/.zshrc
echo "export LIBRARY_PATH=$CLIB/arpg-k4a-realsense2/releases/lib${LIBRARY_PATH:+:${LIBRARY_PATH}}" >> $HOME/.zshrc
echo "export C_INCLUDE_PATH=$CLIB/arpg-k4a-realsense2/releases/include${C_INCLUDE_PATH:+:${C_INCLUDE_PATH}}" >> $HOME/.zshrc
echo "export CPLUS_INCLUDE_PATH=$CLIB/arpg-k4a-realsense2/releases/include${CPLUS_INCLUDE_PATH:+:${CPLUS_INCLUDE_PATH}}" >> $HOME/.zshrc
```

#### Install Sophus

```sh
cd $CLIB/arpg-k4a-realsense2
mkdir -p builds/Sophus && cd builds/Sophus
cmake ../../Sophus \
  -Wno-dev \
  -DCMAKE_INSTALL_PREFIX=$CLIB/arpg-k4a-realsense2/releases
make -j${nproc}
make test
make install
```

#### Install CVars

```sh
cd $CLIB/arpg-k4a-realsense2
mkdir -p builds/CVars && cd builds/CVars
cmake ../../CVars \
  -Wno-dev \
  -DCMAKE_INSTALL_PREFIX=$CLIB/arpg-k4a-realsense2/releases
make -j${nproc}
make install
```

#### Install Pangolin

```sh
cd $CLIB/arpg-k4a-realsense2
mkdir -p builds/Pangolin && cd builds/Pangolin
cmake ../../Pangolin \
  -Wno-dev \
  -DCMAKE_INSTALL_PREFIX=$CLIB/arpg-k4a-realsense2/releases
make -j${nproc}
make install
```

#### Install Calibu

```sh
cd $CLIB/arpg-k4a-realsense2
mkdir -p builds/Calibu && cd builds/Calibu
cmake ../../Calibu \
  -Wno-dev \
  -DCMAKE_INSTALL_PREFIX=$CLIB/arpg-k4a-realsense2/releases
make -j${nproc}
make install
```

#### Install HAL

```sh
cd $CLIB/arpg-k4a-realsense2
# sed -i 's/CV_GRAY2RGB/cv::COLOR_GRAY2RGB/g' HAL/HAL/Camera/Drivers/Convert/ConvertDriver.cpp
# sed -i 's/CV_RGB2GRAY/cv::COLOR_RGB2GRAY/g' HAL/HAL/Camera/Drivers/Convert/ConvertDriver.cpp
# sed -i 's/CV_RGB2BGR/cv::COLOR_RGB2BGR/g' HAL/HAL/Camera/Drivers/Convert/ConvertDriver.cpp
# sed -i 's/CV_BGR2RGB/cv::COLOR_BGR2RGB/g' HAL/HAL/Camera/Drivers/Convert/ConvertDriver.cpp
# sed -i 's/CV_GRAY2BGR/cv::COLOR_GRAY2BGR/g' HAL/HAL/Camera/Drivers/Convert/ConvertDriver.cpp
# sed -i 's/CV_BGR2GRAY/cv::COLOR_BGR2GRAY/g' HAL/HAL/Camera/Drivers/Convert/ConvertDriver.cpp
# sed -i 's/elif CV_VERSION_MAJOR == 3/elif CV_VERSION_MAJOR == 3 || CV_VERSION_MAJOR == 4/g' HAL/HAL/Camera/Drivers/OpenCV/# OpenCVDriver.h
# sed -i 's/CV_RGB2GRAY/cv::COLOR_RGB2GRAY/g' HAL/HAL/Camera/Drivers/OpenCV/OpenCVDriver.cpp
# sed -i 's/OpenCV_VERSION_MAJOR EQUAL 3/OpenCV_VERSION_MAJOR GREATER 2/g' HAL/HAL/CMakeLists.txt
mkdir -p builds/HAL && cd builds/HAL
cmake ../../HAL \
  -Wno-dev \
  -DCMAKE_INSTALL_PREFIX=$CLIB/arpg-k4a-realsense2/releases \
  -Dk4a_INCLUDE_DIRS=/usr/include \
  -Dk4a_LIBRARIES=/usr/lib/x86_64-linux-gnu/libk4a.so
make -j${nproc}
make install
```

#### Install Vicalib

```sh
cd $CLIB/arpg-k4a-realsense2
mkdir -p builds/vicalib && cd builds/vicalib
cmake ../../vicalib \
  -Wno-dev \
  -DCMAKE_INSTALL_PREFIX=$CLIB/arpg-k4a-realsense2/releases \
  -DCMAKE_PREFIX_PATH=$CLIB/ceres-solver-1.14.0/release/lib/cmake/Ceres
make -j${nproc}
make install
```

## Create Calibration Pattern

```sh
# modify policy.xml
sudo sed -i 's/policy domain="coder" rights="none" pattern="PDF"/policy domain="coder" rights="read|write" pattern="PDF"/g' /etc/ImageMagick-6/policy.xml

 create pattern
cd $CLIB
$CLIB/arpg-k4a-realsense2/releases/bin/vicalib \
  -grid_preset medium \
  -output_pattern_file $CLIB/pattern-medium.svg
convert \
  -density 300 \
  $CLIB/pattern-medium.svg $CLIB/pattern-medium.pdf
```
