#ifndef STATE_H
# define STATE_H

#include "Particles.h"
#include <unordered_map>
#include <vector>
#include <memory>
// #include <raylib.h>


namespace fallingsandgame {

// using State = vector<vector<shared_ptr<Particle>>>;

class GameState {
public:
    GameState();
	void Tick();
	void ApplyUserInteraction(Coord interactedPos, float scalingFactor, Element elemToSpawn);
    Element GetPixelElement(int x, int y);
protected:
    std::vector<std::vector<std::shared_ptr<Particle>>> inState;
    std::vector<std::vector<std::shared_ptr<Particle>>> outState;
};
}
#endif