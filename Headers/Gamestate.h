#ifndef STATE_H
# define STATE_H

#include "Elements.h"
#include <unordered_map>
#include <vector>
#include <memory>
using namespace std;

namespace fallingsandgame {

int constexpr Width = 320;
int constexpr Height = 180;

class GameState {
public:
    GameState(){};
	void Tick();
protected:
    vector<vector<shared_ptr<Particle>>> state;
};
}
#endif