#include "Pong_Ball.h"

Pong_Ball::Pong_Ball() {
}

void Pong_Ball::update() {
    mX += mDir * mSpd;
}
