#include "Gamestate.h"
#include <unordered_map>
#include <random>

using namespace fallingsandgame;

void GameState::Tick() {
    for(int x = Width-1; x >= 0; x--) { // Goes from Width-1 to 0, AKA left to right
        for(int y = Height-1; y >= 0; y--) { // Goes from Height-1 to 0, AKA bottom to top
            (*outState)[x][y].reset();
        }
    }
    for(int y = Height-1; y >= 0; y--) { // Goes from Height-1 to 0, AKA bottom to top
        for(int x = Width-1; x >= 0; x--) { // Goes from Width-1 to 0, AKA left to right
            if((*inState)[x][y]) { // unique_ptr is true if it's managing an object, ad false otherwise
                // nullptr is only for raw ptrs.

                // If NONE material, we just want it to be empty when we get to outState
                if ((*inState)[x][y]->GetElement() == ElementName::NONE) {
                    continue;
                }
                
                // If this space is blank in inState, we want it blank in outState,
                // so that other particles can know that the space is free
                auto newPos = (*inState)[x][y]->Move(*inState, *outState);
                if (VerifyIndexHelper(newPos.x, newPos.y)) {
                    (*outState)[newPos.x][newPos.y] = std::move((*inState)[x][y]);
                }
            }
        }
    }
    std::swap(inState, outState);
};

struct Element {
    ElementName type;
    int speed;
    int mass;
};

std::unordered_map<ElementName, Element> elems {};
                                            
void GameState::ApplyUserInteraction(Coord interactedPos, float scalingFactor, ElementName elemToSpawn) {
    auto scaledPos = Coord{int(interactedPos.x*scalingFactor), int(interactedPos.y*scalingFactor)};
    for (int x = scaledPos.x - 1; x <= scaledPos.x + 1; x++) {
        for (int y = scaledPos.y - 1; y <= scaledPos.y + 1; y++) {
            if(x < 0 || x > Width-1 || y < 0 || y > Height-1) {
                continue;
            }
            std::unique_ptr<Particle> newPtr;
            switch (elemToSpawn) {
                case ElementName::WATER:
                    newPtr = std::make_unique<Liquid>(x, y, elems[elemToSpawn]); // Water
                    break;
                case ElementName::CONCRETE:
                    newPtr = std::make_unique<ImmobileSolid>(x, y, elems[elemToSpawn]); // Concrete
                    break;
                case ElementName::STEAM:
                    newPtr = std::make_unique<Gas>(x, y, elems[elemToSpawn]); // Steam
                    break;
                case ElementName::FIRE:
                    newPtr = std::make_unique<Gas>(x, y, elems[elemToSpawn]); // Fire
                    break;
                case ElementName::SAND:
                    newPtr = std::make_unique<MobileSolid>(x, y, elems[elemToSpawn]); // Sand
                    break;
                default:
                    newPtr = std::make_unique<ImmobileSolid>(x, y, elems[ElementName::NONE]); // Eraser, essentially
            }
             
            (*inState)[x][y] = std::move(newPtr); // Move ownership from in state to out state, and then 
            // when done, all particles are owned by outState.
        }
    }
}

ElementName GameState::GetPixelElementName(int x, int y) {
    // After Tick(), we swap the calculated outState to inState, so inState should always
    // be the one that we draw.
    if ((*inState)[x][y]) {
        return (*inState)[x][y]->GetElement();
    }
    return ElementName::NONE;
}