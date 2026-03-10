#ifndef PARTICLES_H
# define PARTICLES_H

#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;


int constexpr Width = 320;
int constexpr Height = 180;

namespace fallingsandgame {

struct Coord {
    int x;
    int y;
};

enum class Element {
    // enums representing each element, because I couldn't find a way to actully store
    // types in something.
    STEAM,
    WATER,
    SAND,
    FIRE,
    NONE
};

enum class Interaction {
    // Defines the interaction which this element has upon being near another element
    
    // E.g., for water, its FIRE interaction is REPLACE with STEAM, because the water
    // itself will handle that. But for fire, its WATER interaction is DESTROY, since
    // the water extinguishes the fire, so the fire itself will handle that.
    REPLACE,
    DESTROY
};

class Particle {
public:
	Particle(int x, int y, int s, std::unordered_map<Element, Interaction> i) : pos(Coord{x, y}), speed(s), interactions(i){};
    virtual Coord Move(const vector<vector<shared_ptr<Particle>>>& state);
    virtual Particle InteractReplaceSelf();
    virtual void InteractDeleteSelf();
protected:
    Coord pos;
    int speed;
    std::unordered_map<Element, Interaction> interactions;
};

// A lot of repeated code, just for them to only differ in Move. Is there a better way?

class Gas : public Particle {
public:
	Gas(int x, int y, int s, std::unordered_map<Element, Interaction> i) : Particle(x, y, s, i){};
    Coord Move(const vector<vector<shared_ptr<Particle>>>& state);
    Particle InteractReplaceSelf();
    void InteractDeleteSelf();
};

class Liquid : public Particle {
public:
	Liquid(int x, int y, int s, std::unordered_map<Element, Interaction> i) : Particle(x, y, s, i){};
    Coord Move(const vector<vector<shared_ptr<Particle>>>& state);
    Particle InteractReplaceSelf();
    void InteractDeleteSelf();
};

class Solid : public Particle {
public:
	Solid(int x, int y, int s, std::unordered_map<Element, Interaction> i) : Particle(x, y, s, i){};
    Coord Move(const vector<vector<shared_ptr<Particle>>>& state);
    Particle InteractReplaceSelf();
    void InteractDeleteSelf();
};
}
#endif