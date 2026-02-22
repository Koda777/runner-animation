#pragma once
#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(const char* path, const float speed) : Sprite(path) {
    this->speed = speed;
}
