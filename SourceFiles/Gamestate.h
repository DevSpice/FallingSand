#ifndef STATE_H
# define STATE_H

#include "Particles.h"
#include <unordered_map>
#include <vector>
#include <memory>
// #include <raylib.h>


namespace fallingsandgame {

// using State = std::vector<std::vector<std::unique_ptr<Particle>>>;

class GameState {
public:
    GameState(){};
	void Tick();
	void ApplyUserInteraction(Coord interactedPos, float scalingFactor, Element elemToSpawn);
    Element GetPixelElement(int x, int y);
protected:
    std::shared_ptr<std::vector<std::vector<std::unique_ptr<Particle>>>> inState; // When move instate to outstate, first do that for every pixel.
    // But sometimes, you have a unique_ptr owning an object, and you wanna clear it so
    // it's not owning an object anymore. The reset method does that. E.g. if its outState wants to
    // move off the screen.
    std::shared_ptr<std::vector<std::vector<std::unique_ptr<Particle>>>> outState;
};
}
#endif