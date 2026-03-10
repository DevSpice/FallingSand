#ifndef ELEMS_H
# define ELEMS_H

#include "Particles.h"

namespace fallingsandgame {

int constexpr width = 320;
int constexpr height = 180;

class Steam : public Gas {
    Steam(int x, int y, int s) : Gas(x, y, s,
        std::unordered_map<Element, Interaction>{{},}) {};
        // Should Steam destroy Water, so that it can float up through the water? Or nah?
};

class Fire : public Gas {
    Fire(int x, int y, int s) : Gas(x, y, s,
        std::unordered_map<Element, Interaction>{{Element::WATER, Interaction::DESTROY},}) {};
};

class Water : public Liquid {
    Water(int x, int y, int s) : Liquid(x, y, s,
        std::unordered_map<Element, Interaction>{{Element::FIRE, Interaction::REPLACE},}) {};
        // Example setup of interaction mapping
};
}
#endif