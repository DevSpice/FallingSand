#ifndef PARTICLES_H
# define PARTICLES_H

#include <unordered_map>
#include <vector>
#include <memory>
#include "Interactions.h"


int constexpr Width = 320;
int constexpr Height = 180;

namespace fallingsandgame {

struct Coord {
    int x;
    int y;
};

class Particle {
public:
	Particle(int x, int y, int s, int m, Element e) : pos(Coord{x, y}), speed(s), mass(m), element(e){};
    virtual Coord Move(const std::vector<std::vector<std::shared_ptr<Particle>>>& state) = 0;
    Element GetElement();
    virtual ~Particle() = default;
protected:
    Coord pos;
    int speed;
    int mass;
    Element element;
};

// A lot of repeated code, just for them to only differ in Move. Is there a better way?

class Gas : public Particle {
public:
	Gas(int x, int y, int s, int m, Element e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::shared_ptr<Particle>>>& state);
};

class Liquid : public Particle {
public:
	Liquid(int x, int y, int s, int m, Element e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::shared_ptr<Particle>>>& state);
};

class Solid : public Particle {
public:
	Solid(int x, int y, int s, int m, Element e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::shared_ptr<Particle>>>& state);
};
}
#endif