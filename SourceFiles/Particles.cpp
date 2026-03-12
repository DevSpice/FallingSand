#include "Particles.h"
#include <unordered_map>
#include <random>
#include <iostream>
#include <optional>

using namespace fallingsandgame;


std::random_device rd;  // a seed source for the random number engine
std::mt19937 rng(rd()); // mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> distrib(1, 100);

std::optional<Particle> checkStates(int x, int y,  std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
     std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    // Make sure not occupied in inState or outState
    if (inState[x][y]){
        return *inState[x][y];
    } 
    if (outState[x][y]) {
        return *outState[x][y];
    }
    return {};
}

void TrySwap(Particle p1, Particle p2, std::vector<std::vector<std::unique_ptr<Particle>>>& inState) {
    // Sand and Water Swap
    if (p1.GetElement() == ElementName::SAND && p2.GetElement() == ElementName::WATER) {
        Coord p1Pos = p1.GetPos();
        Coord p2Pos = p2.GetPos();

        std::swap(inState[p2Pos.x][p2Pos.y], inState[p1Pos.x][p1Pos.y]);
    }
}

Coord moveHelper(std::vector<std::vector<std::unique_ptr<Particle>>>& inState, std::vector<std::vector<std::unique_ptr<Particle>>>& outState, Coord startingPos, int speed, int direction, bool fluid) {
    // A function which calculates the best move for this particle, given
    // its speed, direction (1 : up, -1 : down), and coordinate/state information.
    auto newX = startingPos.x;
    auto newY = startingPos.y;
    while (speed >= 1)
    {
        auto currX = newX;
        auto currY = newY;
        int leftOrRight = distrib(rng) % 2 == 1 ? -1 : 1; // -1 or +1, to add to x-coordinate to move left or right at random.
        std::vector<int> possXs{newX, newX-leftOrRight, newX+leftOrRight};
        std::vector<int> possYs{newY+direction};
        if (fluid) {
            possYs.push_back(newY);
        }
        Particle currP = *inState[newX][newY];

        for (auto y : possYs) // Check in order: Move Up/Down, and (if fluid) Stay at same y
            for (auto x : possXs) { // Check in order: Stay at same x, Move Left/Right, and Move (-) Left/Right
            {
                if (!VerifyIndexHelper(x, y)) {
                    currX = x;
                    currY = y;
                    break;
                }
                else if ((x != newX || y != newY)) {
                    std::optional<Particle> p = checkStates(x, y, inState, outState);
                    if (p.has_value()) {
                        // std::cout <<  ElementName(p.value().GetElement()) << std::endl;
                        TrySwap(currP, p.value(), inState);

                    }
                    else {
                            
                        // std::cout << "Nada" << std::endl;
                        currX = x;
                        currY = y;
                        break;

                    }
                } 
            }
            if (currX != newX || currY != newY) { // If we've gotten a successful move, we can break this current loop
                break;
            }
        }
        if (currX == newX && currY == newY) { // If didn't move, is trapped, so stay still and exit movement loop.
            break;
        }
        newX = currX;
        newY = currY;
        speed--;
    }
    return Coord{newX, newY};
}

Coord Gas::Move( std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
     std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, -1, true); // (0,0) is top left, so negative speed means going up.
    return pos;
}

Coord Liquid::Move( std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
     std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, 1, true); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord MobileSolid::Move( std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
     std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    pos = moveHelper(inState, outState, pos, speed, 1, false); // (0,0) is top left, so positive speed means going down.
    return pos;
}

Coord ImmobileSolid::Move( std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
     std::vector<std::vector<std::unique_ptr<Particle>>>& outState) {
    // It should not move at all, so just return its current position.
    return pos;
}