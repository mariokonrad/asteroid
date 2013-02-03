
SDL_CONFIG=sdl-config

CC=gcc
CXX=g++
CFLAGS+=`$(SDL_CONFIG) --cflags` -O2 -Wall
CXXFLAGS+=`$(SDL_CONFIG) --cflags` -O2 -Wall
#LDFLAGS+=`$(SDL_CONFIG) --libs` -lopengl32
LDFLAGS+=`$(SDL_CONFIG) --libs` -lGL

OBJECTS=\
	glutil.o \
	v3.o \
	Boundary.o \
	RenderObject.o \
	Sprite.o \
	Bullet.o \
	Ship.o \
	ShipDecorator.o \
	ShipDecoInvulnerable.o \
	Asteroid.o \
	Player.o \
	PowerUp.o \
	PowerUpShield.o \
	FontBase.o \
	Font.o \
	asteroids.o

asteroids : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean :
	rm -f *.o *.exe asteroids *.stackdump

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

