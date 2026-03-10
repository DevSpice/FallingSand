#include "Gamestate.h"
#include <unordered_map>
#include <random>

using namespace fallingsandgame;

void GameState::Tick() {
    for(int x = Width-1; x >= 0; x--) { // Goes from Width-1 to 0, AKA left to right
        for(int y = Height-1; y >= 0; y--) { // Goes from Height-1 to 0, AKA bottom to top
            if((*inState)[x][y]) { // unique_ptr is true if it's managing an object, ad false otherwise
                // nullptr is only for raw ptrs.
                
                // If this space is blank in inState, we want it blank in outState,
                // so that other particles can know that the space is free
                (*outState)[x][y].reset();
            }
            auto newPos = (*inState)[x][y]->Move(inState);
            (*outState)[newPos.x][newPos.y] = std::move((*inState)[x][y]);
        }
    }
    std::swap(inState, outState);
};

void GameState::ApplyUserInteraction(Coord interactedPos, float scalingFactor, Element elemToSpawn) {
    auto scaledPos = Coord{int(interactedPos.x*scalingFactor), int(interactedPos.y*scalingFactor)};
    for (int x = scaledPos.x - 1; x <= scaledPos.x + 1; x++) {
        for (int y = scaledPos.y - 1; y <= scaledPos.y + 1; y++) {
            std::unique_ptr<Particle> waterPtr = std::make_unique<Liquid>(x, y, 1, 10, Element::WATER); // Water
            (*outState)[x][y] = std::move(waterPtr); // Move ownership from in state to out state, and then 
            // when done, all particles are owned by outState.
        }
    }
}

Element GameState::GetPixelElement(int x, int y) {
    // After Tick(), we swap the calculated outState to inState, so inState should always
    // be the one that we draw.
    if ((*inState)[x][y]) {
        return (*inState)[x][y]->GetElement();
    }
    return Element::NONE;
}