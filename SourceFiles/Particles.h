#ifndef PARTICLES_H
# define PARTICLES_H

#include <unordered_map>
#include <vector>
#include <memory>
#include "Interactions.h"


int constexpr Width = 320;
int constexpr Height = 180;

namespace fallingsandgame {

bool VerifyIndexHelper(int x, int y) {
    // Doesn't let our pixels move off the board, essentially making the bounds
    // of our screen delete anything that tries to move past them.
    return (x >= 0 && x < Width) && (y >= 0 && y < Height);
}

struct Coord {
    int x;
    int y;
};


struct Element {
    ElementName type;
    int speed;
    int mass;
};


class Particle {
public:
	Particle(int x, int y, Element e) : pos(Coord{x, y}), speed(e.speed), mass(e.mass){};
    virtual Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState){return Coord{0,0};};
    virtual ~Particle() = default;
protected:
    Coord pos;
    int speed;
    int mass;
};

// A lot of repeated code, just for them to only differ in Move. Is there a better way?

class Gas : public Particle {
public:
	Gas(int x, int y, int s, Element e) : Particle(x, y, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};

class Liquid : public Particle {
public:
	Liquid(int x, int y, Element e) : Particle(x, y, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};

class MobileSolid : public Particle {
public:
	MobileSolid(int x, int y, Element e) : Particle(x, y, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};

class ImmobileSolid : public Particle {
public:
	ImmobileSolid(int x, int y, Element e) : Particle(x, y, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};
}
#endif