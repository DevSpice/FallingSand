#include "Particles.h"
#include <unordered_map>
#include <random>

using namespace fallingsandgame;


std::random_device rd;  // a seed source for the random number engine
std::mt19937 rng(rd()); // mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> distrib(-1, 1);

Coord Gas::Move() {
    int newX = pos.x + distrib(rng);
    int newY = pos.y - speed; // (0,0) is top left, so negative speed means going up.
    pos = Coord{newX, newY};
}

Coord Liquid::Move() {
    int newX = pos.x + distrib(rng);
    int newY = pos.y + speed;
    pos = Coord{newX, newY};
}

Coord Solid::Move() {
    int newX = pos.x + distrib(rng);
    int newY = pos.y + speed;
    pos = Coord{newX, newY};
}