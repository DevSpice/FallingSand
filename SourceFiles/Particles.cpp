#include "Particles.h"
#include <unordered_map>
#include <random>

using namespace fallingsandgame;


std::random_device rd;  // a seed source for the random number engine
std::mt19937 rng(rd()); // mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> distrib(-1, 1);

Coord moveHelper(const std::vector<std::vector<std::unique_ptr<Particle>>>& state, Coord startingPos, int speed, int direction) {
    // A function which calculates the best move for this particle, given
    // its speed, direction (1 : up, -1 : down), and coordinate/state information.
    auto newX = startingPos.x;
    auto newY = startingPos.y;
    while (speed >= 1)
    {
        // Check if can move straight up/down.
        if (!VerifyIndexHelper(newX, newY+direction) || !state[newX][newY+direction]) {
            newY = newY+direction;
        }
        // Check if can move up/down and to the left or right.
        else if (!VerifyIndexHelper(newX-1, newY+direction) || !state[newX-1][newY+direction]) {
            newX = newX-1;
            newY = newY+direction;
        }
        else if (!VerifyIndexHelper(newX+1, newY+direction) || !state[newX+1][newY+direction]) {
            newX = newX+1;
            newY = newY+direction;
        }
        // Check if can move to the left or right.
        else if (!VerifyIndexHelper(newX-1, newY) || !state[newX-1][newY]) {
            newX = newX-1;
        }
        else if (!VerifyIndexHelper(newX+1, newY) || !state[newX+1][newY]) {
            newX = newX+1;
        }
        // Otherwise, is trapped, so stay still and exit movement loop.
        else {
            break;
        }
        speed--;
    }
    return Coord{newX, newY};
}

Coord Gas::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& state) {
    pos = moveHelper(state, pos, speed, -1); // (0,0) is top left, so negative speed means going up.
    return pos;
}

Coord Liquid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& state) {
    pos = moveHelper(state, pos, speed, 1); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord MobileSolid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& state) {
    pos = moveHelper(state, pos, speed, 1); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord ImmobileSolid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& state) {
    // It should not move at all, so just return its current position.
    return pos;
}