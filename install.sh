#!/usr/bin/env bash

sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libgl1-mesa-dev \
    xorg \
    xinit \
    mesa-utils
