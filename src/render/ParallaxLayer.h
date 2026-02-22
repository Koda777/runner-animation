#pragma once
#include "Sprite.h"

class ParallaxLayer : Sprite {
public:
    explicit ParallaxLayer(const char* path, float speed);
private:
    float speed;
};
