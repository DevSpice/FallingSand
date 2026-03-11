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

class Particle {
public:
	Particle(int x, int y, int s, int m, ElementName e) : pos(Coord{x, y}), speed(s), mass(m), element(e){};
    virtual Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState){return Coord{0,0};};
    ElementName GetElement() {return element;};
    virtual ~Particle() = default;
protected:
    Coord pos;
    int speed;
    int mass;
    ElementName element;
};

// A lot of repeated code, just for them to only differ in Move. Is there a better way?

class Gas : public Particle {
public:
	Gas(int x, int y, int s, int m, ElementName e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};

class Liquid : public Particle {
public:
	Liquid(int x, int y, int s, int m, ElementName e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};

class MobileSolid : public Particle {
public:
	MobileSolid(int x, int y, int s, int m, ElementName e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};

class ImmobileSolid : public Particle {
public:
	ImmobileSolid(int x, int y, int s, int m, ElementName e) : Particle(x, y, s, m, e){};
    Coord Move(const std::vector<std::vector<std::unique_ptr<Particle>>>& inState,
    const std::vector<std::vector<std::unique_ptr<Particle>>>& outState) override;
};
}
#endif