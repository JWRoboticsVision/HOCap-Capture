<h1>Container Installation</h1>

<h2>Table of Contents</h2>

- [Docker Setup](#docker-setup)
  - [1. Install Docker](#1-install-docker)
  - [2. Install the NVIDIA Container Toolkit](#2-install-the-nvidia-container-toolkit)
- [Building the Container](#building-the-container)
  - [1. Build the Container for ROS1](#1-build-the-container-for-ros1)

## Docker Setup

#### 1. Install Docker

We follow the official Docker [installation guide](https://docs.docker.com/engine/install/ubuntu/#install-using-the-repository) for Ubuntu to set up Docker on your system. This includes adding the Docker repository, installing the necessary packages, and configuring user permissions.

- Set up Docker's apt repository

```sh
# Add Docker's official GPG key:
sudo apt update
sudo apt install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
sudo tee /etc/apt/sources.list.d/docker.sources <<EOF
Types: deb
URIs: https://download.docker.com/linux/ubuntu
Suites: $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}")
Components: stable
Architectures: $(dpkg --print-architecture)
Signed-By: /etc/apt/keyrings/docker.asc
EOF

sudo apt update
```

- Install the Docker Packages

```sh
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

- Verify Docker service is running

```sh
# To verify Docker is running
sudo systemctl status docker

# If Docker is not running, start the service
sudo systemctl start docker
```

- Add your user to the Docker group to run Docker without `sudo`

```sh
sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker

# A logout/login may be required for the group change to take effect
```

- Verify Docker Installation

```sh
docker run hello-world
```

#### 2. Install the NVIDIA Container Toolkit

We follow the official NVIDIA Container Toolkit [installation guide](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html) to set up the NVIDIA Container Toolkit on your system. This includes configuring the repository, installing the necessary packages, and verifying the installation.

- Configure the repository
-

```sh
curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | sudo gpg --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg \
  && curl -s -L https://nvidia.github.io/libnvidia-container/stable/deb/nvidia-container-toolkit.list | \
    sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
    sudo tee /etc/apt/sources.list.d/nvidia-container-toolkit.list \
    && sudo apt-get update
```

- Install the NVIDIA Container Toolkit packages

```sh
export NVIDIA_CONTAINER_TOOLKIT_VERSION=1.19.0-1
sudo apt-get install -y \
    nvidia-container-toolkit=${NVIDIA_CONTAINER_TOOLKIT_VERSION} \
    nvidia-container-toolkit-base=${NVIDIA_CONTAINER_TOOLKIT_VERSION} \
    libnvidia-container-tools=${NVIDIA_CONTAINER_TOOLKIT_VERSION} \
    libnvidia-container1=${NVIDIA_CONTAINER_TOOLKIT_VERSION}
```

- Configuring Docker

```sh
# Configure the container runtime
sudo nvidia-ctk runtime configure --runtime=docker
# Restart the Docker daemon to apply changes
sudo systemctl restart docker
```

- Verify NVIDIA Container Toolkit

```sh
docker run --rm --runtime=nvidia --gpus all ubuntu nvidia-smi
```

Your output should resemble the following output:

```
+-----------------------------------------------------------------------------------------+
| NVIDIA-SMI 580.126.09             Driver Version: 580.126.09     CUDA Version: 13.0     |
+-----------------------------------------+------------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  NVIDIA GeForce RTX 2080 Ti     Off |   00000000:01:00.0  On |                  N/A |
|  0%   49C    P8             15W /  260W |     508MiB /  11264MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+

+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI              PID   Type   Process name                        GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
|  No running processes found                                                             |
+-----------------------------------------------------------------------------------------+
```

## Building the Container

#### 1. Build the Container for ROS1

```sh
# Generate the .env file with the necessary environment variables
bash docker/generate_env.sh

# Build the ROS1 container using the provided script
bash docker/container_handler.sh build ros1
```
