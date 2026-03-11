#include "Particles.h"
#include <unordered_map>
#include <random>

using namespace fallingsandgame;


std::random_device rd;  // a seed source for the random number engine
std::mt19937 rng(rd()); // mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> distrib(1, 20);

bool checkStates(int x, int y, const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    // Make sure not occupied in inState or outState
    return !inState[x][y] && !outState[x][y];
}

Coord moveHelper(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState, Coord startingPos, int speed, int direction, bool fluid) {
    // A function which calculates the best move for this particle, given
    // its speed, direction (1 : up, -1 : down), and coordinate/state information.
    auto newX = startingPos.x;
    auto newY = startingPos.y;
    while (speed >= 1)
    {
        int leftOrRight = distrib(rng) % 2 == 1 ? -1 : 1; // -1 or +1, to add to x-coordinate to move left or right at random.
        // Check if can move straight up/down.
        if (!VerifyIndexHelper(newX, newY+direction) || checkStates(newX, newY+direction, inState, outState)) {
            newY = newY+direction;
        }
        // Check if can move up/down and to the left or right.
        else if (!VerifyIndexHelper(newX-leftOrRight, newY+direction) || checkStates(newX-leftOrRight, newY+direction, inState, outState)) {
            newX = newX-leftOrRight;
            newY = newY+direction;
        }
        else if (!VerifyIndexHelper(newX+leftOrRight, newY+direction) || checkStates(newX+leftOrRight, newY+direction, inState, outState)) {
            newX = newX+leftOrRight;
            newY = newY+direction;
        }
        else if (fluid) {
            // Check if can move to the left or right.
            if (!VerifyIndexHelper(newX-leftOrRight, newY) || checkStates(newX-leftOrRight, newY, inState, outState)) {
                newX = newX-leftOrRight;
            }
            else if (!VerifyIndexHelper(newX+leftOrRight, newY) || checkStates(newX+leftOrRight, newY, inState, outState)) {
                newX = newX+leftOrRight;
            }
        }
        // Otherwise, is trapped, so stay still and exit movement loop.
        else {
            break;
        }
        speed--;
    }
    return Coord{newX, newY};
}

Coord Gas::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, -1, true); // (0,0) is top left, so negative speed means going up.
    return pos;
}

Coord Liquid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, 1, true); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord MobileSolid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, 1, false); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord ImmobileSolid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    // It should not move at all, so just return its current position.
    return pos;
}