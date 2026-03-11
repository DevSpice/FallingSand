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
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState, Coord startingPos, int speed, int direction) {
    // A function which calculates the best move for this particle, given
    // its speed, direction (1 : up, -1 : down), and coordinate/state information.
    auto newX = startingPos.x;
    auto newY = startingPos.y;
    while (speed >= 1)
    {
        auto leftOrRight = distrib(rng) % 2 == 1 ? -1 : 1;
        auto yMod = direction; // Gets -1 or 1
        while (yMod != direction*-1) { // While yMod == yMod or yMod == 0
            // Check down/up middle, down/up left, and down/up right, and then just left and right
            if (yMod != 0 && !VerifyIndexHelper(newX, newY+yMod) || checkStates(newX, newY+yMod, inState, outState)) {
                newY = newY+yMod;
                break;
            }
            // Check if can move up/down and to the left or right.
            else if (!VerifyIndexHelper(newX-leftOrRight, newY+yMod) || checkStates(newX-leftOrRight, newY+yMod, inState, outState)) {
                newX = newX-leftOrRight;
                newY = newY+direction;
                break;
            }
            else if (!VerifyIndexHelper(newX+leftOrRight, newY+yMod) || checkStates(newX+leftOrRight, newY+yMod, inState, outState)) {
                newX = newX+leftOrRight;
                newY = newY+direction;
                break;
            }
            yMod += direction*-1;
        }
        // Otherwise, is trapped, so stay still and exit movement loop.
        if (newX == startingPos.x && newY == startingPos.y) {
            break;
        }
        speed--;
    }
    return Coord{newX, newY};
}

Coord Gas::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, -1); // (0,0) is top left, so negative speed means going up.
    return pos;
}

Coord Liquid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, 1); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord MobileSolid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, 1); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord ImmobileSolid::Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    // It should not move at all, so just return its current position.
    return pos;
}