#!/bin/bash

source $(dirname $0)/config.sh

# Generate a .env file for Docker build arguments
log_message "Generating .env file for Docker build arguments..."

cat <<EOF >${DOCKER_DIR}/.env
DOCKER_USER_ARG=${USER_NAME}
DOCKER_USER_UID_ARG=${USER_ID}
DOCKER_USER_GID_ARG=${GROUP_ID}
TORCH_CUDA_ARCH_LIST_ARG=${TORCH_CUDA_ARCH_LIST}
ROS1_APT_PACKAGE_ARG=${ROS1_APT_PACKAGE}
ROS2_APT_PACKAGE_ARG=${ROS2_APT_PACKAGE}
DISPLAY=${DISPLAY:-:1}
TERM=$TERM
PROJ_ROOT=$PROJ_ROOT
EOF

log_message "Done!!!"
