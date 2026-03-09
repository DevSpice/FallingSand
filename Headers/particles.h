#ifndef PARTICLES_H
# define PARTICLES_H

namespace fallingsandgame {

int constexpr width = 320;
int constexpr height = 180;

struct Coord {
    int x;
    int y;
};

class Particle {
public:
	Particle(int x, int y, int s) : pos(Coord{x, y}), speed(s){};
    virtual Coord Move();
    virtual void InteractReplaceSelf(Particle other);
    virtual void InteractDeleteSelf();
private:
    Coord pos;
    int speed;
};

class Gas : public Particle {
public:
	Gas(int x, int y, int s) : Particle(x, y, s){};
    Coord Move();
    void InteractReplaceSelf(Particle other);
    void InteractDeleteSelf();
};

class Liquid : public Particle {
public:
	Liquid(int x, int y, int s) : Particle(x, y, s){};
    Coord Move();
    void InteractReplaceSelf(Particle other);
    void InteractDeleteSelf();
};

class Solid : public Particle {
public:
	Solid(int x, int y, int s) : Particle(x, y, s){};
    Coord Move();
    void InteractReplaceSelf(Particle other);
    void InteractDeleteSelf();
};
}
#endif