#include "Gamestate.h"
#include <unordered_map>
#include <random>

using namespace fallingsandgame;

GameState::GameState() {
    for(int x = Width-1; x >= 0; x--) { // Goes from Width-1 to 0, AKA left to right
        for(int y = Height-1; y >= 0; y--) { // Goes from Height-1 to 0, AKA bottom to top
            inState[x][y] = nullptr;
            outState[x][y] = nullptr;
        }
    }
};

void GameState::Tick() {
    for(int x = Width-1; x >= 0; x--) { // Goes from Width-1 to 0, AKA left to right
        for(int y = Height-1; y >= 0; y--) { // Goes from Height-1 to 0, AKA bottom to top
            if(inState[x][y] == nullptr) {
                // If this space is blank in inState, we want it blank in outState,
                // so that other particles can know that the space is free
                outState[x][y] = nullptr;
            }
            auto newPos = inState[x][y]->Move(inState);
            outState[newPos.x][newPos.y] = inState[x][y];
        }
    }
};

void GameState::ApplyUserInteraction(Coord interactedPos, float scalingFactor, Element elemToSpawn) {
    auto scaledPos = Coord{int(interactedPos.x*scalingFactor), int(interactedPos.y*scalingFactor)};
    for (int x = scaledPos.x - 1; x <= scaledPos.x + 1; x++) {
        for (int y = scaledPos.y - 1; y <= scaledPos.y + 1; y++) {
            std::shared_ptr<Particle> waterPtr = std::make_shared<Liquid>(x, y, 1, 10, Element::WATER); // Water
            outState[x][y] = waterPtr;
        }
    }
}

Element GameState::GetPixelElement(int x, int y) {
    if (outState[x][y] != nullptr) {
        return outState[x][y]->GetElement();
    }
    return Element::NONE;
}