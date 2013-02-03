#include <SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <time.h>
#include "util.hpp"
#include "Bullet.hpp"
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "Font.hpp"
#include "glutil.hpp"
#include "PowerUpShield.hpp"

enum { W = 800, H = 600, DELAY_TIME = 20 };

enum GameStateType {
	 GAMESTATE_QUIT = -1
	,GAMESTATE_START = 0
	,GAMESTATE_PLAY
	,GAMESTATE_SCORE
};

static v3 random_v3(void)
{
	return v3(
		(abs(rand()) % 100) - 50,
		(abs(rand()) % 100) - 50,
		0.0f).normalize();
}

static Player player;

class GameState // {{{
{
	private:
		Uint32 timestamp;
		Uint32 delay_time;
	protected:
		void init_time();
		void delay();
		GLfloat update_time();
	public:
		virtual ~GameState() {}
		virtual GameStateType exec() = 0;
};

void GameState::init_time() // {{{
{
	timestamp = delay_time = SDL_GetTicks();
} // }}}

void GameState::delay() // {{{
{
	delay_time = SDL_GetTicks() - delay_time; // time passed to handle everything: keyboard, rendering, etc.
	SDL_Delay(delay_time > DELAY_TIME ? 0 : DELAY_TIME - delay_time);
	delay_time = SDL_GetTicks();
} // }}}

GLfloat GameState::update_time() // {{{
{
	Uint32 t = SDL_GetTicks();
	GLfloat dt = (t - timestamp) / 1000.0;
	timestamp = t;
	return dt;
} // }}}

// }}}

class GameStateScore : public GameState // {{{
{
	private:
		void render();
	public:
		virtual ~GameStateScore();
		virtual GameStateType exec();
};

GameStateScore::~GameStateScore()
{}

void GameStateScore::render()
{
	static const Font FONT1(v3(20.0f, 40.0f), FontBase::color_t(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 5.0f);
	static const Font FONT2(v3(10.0f, 20.0f), FontBase::color_t(0.0f, 0.0f, 1.0f, 1.0f), 5.0f, 2.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glText(v3(100.0f, 400.0f), FONT1, "game over");
	glText(v3(100.0f, 200.0f), FONT1, "score:%u", player.score);
	glText(v3(100.0f, 100.0f), FONT2, "press escape ...");
	SDL_GL_SwapBuffers();
}

GameStateType GameStateScore::exec()
{
	init_time();
	for (;;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return GAMESTATE_QUIT;
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: return GAMESTATE_START;
					default: break;
				}
			}
		}
		render();
		delay();
	}
}

// }}}

class GameStateStart : public GameState // {{{
{
	private:
		typedef std::vector<Asteroid *> Asteroids;
	private:
		Asteroids asteroid;
	private:
		void update();
		void render();
	public:
		GameStateStart();
		virtual ~GameStateStart();
		virtual GameStateType exec();
};

GameStateStart::GameStateStart()
{
	// asteroid animation in the background
	for (int i = 0; i < 50; ++i) {
		asteroid.push_back(new Asteroid(3 + rand() % 5, rand() % Asteroid::MAX_LEVEL,
			v3(W/2.0f, H/2.0f, 0.0f),
			(30.0f + (rand() % 30)) * random_v3(),
			0.0f, (rand() % 40) - 20.0f
			));
	}
}

GameStateStart::~GameStateStart()
{
	delete_all(asteroid);
}

void GameStateStart::update()
{
	GLfloat dt = update_time();

	// update
	for_each(asteroid.begin(), asteroid.end(), Sprite::Step(dt));
	for_each(asteroid.begin(), asteroid.end(), Sprite::Wrap(0.0f, W-1, 0.0f, H-1, 0.0f, 0.0f));
}

void GameStateStart::render()
{
	static const Font FONT1(v3(30.0f, 50.0f), FontBase::color_t(1.0f, 0.0f, 0.0f, 1.0f), 5.0f, 5.0f);
	static const Font FONT2(v3(10.0f, 20.0f), FontBase::color_t(0.0f, 0.0f, 1.0f, 1.0f), 5.0f, 2.0f);
	static const Font FONT3(v3( 6.0f, 10.0f), FontBase::color_t(0.0f, 1.0f, 0.0f, 0.4f), 5.0f, 1.5f);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	for_each(asteroid.begin(), asteroid.end(), Sprite::Render());
	glText(v3(50.0f, 400.0f), FONT1, "A S T E R O I D S");
	glText(v3(50.0f, 370.0f), FONT3, "written 2009 by mario konrad");
	glText(v3(50.0f, 200.0f), FONT2, "press space to start");
	glText(v3(50.0f, 150.0f), FONT2, "press escape to quit");
	SDL_GL_SwapBuffers();
}

GameStateType GameStateStart::exec()
{
	init_time();
	for (;;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return GAMESTATE_QUIT;
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: return GAMESTATE_QUIT;
					case SDLK_SPACE: return GAMESTATE_PLAY;
					default: break;
				}
			}
		}
		update();
		render();
		delay();
	}
	return GAMESTATE_START;
}

// }}}

class GameStatePlay : public GameState // {{{
{
	private:
		enum State { READY, PLAY };
	private:
		typedef std::vector<Asteroid *> Asteroids;
		typedef std::vector<PowerUp *> PowerUps;
		typedef std::vector<Bullet *> Bullets;
		typedef std::vector<Sprite *> Sprites;
	private:
		Ship ship;
		Asteroids asteroid;
		PowerUps powerup;
		Bullets bullet;
		State state;
		GLfloat countdown;
		GLfloat p_powerup_on_hit;
	private:
		template <typename T>
		static void destroy_dead(std::vector<T*> & c)
		{
			for (;;) {
				typename std::vector<T*>::iterator i = find_if(c.begin(), c.end(), T::not_alive);
				if (i == c.end()) break;
				delete *i;
				c.erase(i);
			}
		}
	private:
		bool collision_bullet_asteroid();
		bool collision_ship_asteroid();
		bool collision_ship_powerup();
		void wrap_arround();
		void update_play(GLfloat);
		void init_asteroids();
		void init_ship();
		void render();
		void handle_keys_play();
		void spawn_powerup_on_hit(const v3 &);
		static PowerUp * factory(int);
	public:
		GameStatePlay();
		virtual ~GameStatePlay();
		virtual GameStateType exec();
};

GameStatePlay::GameStatePlay()
	: state(READY)
	, countdown(2.0f)
	, p_powerup_on_hit(0.05f)
{
	player = Player();
}

GameStatePlay::~GameStatePlay()
{
	delete_all(asteroid);
	delete_all(powerup);
	delete_all(bullet);
}

PowerUp * GameStatePlay::factory(int type)
{
	switch (type) {
		case PowerUpShield::TYPE: return new PowerUpShield();
	}
	return NULL;
}

void GameStatePlay::spawn_powerup_on_hit(const v3 & x)
{
	static const int POWERUPS[] = {
		PowerUpShield::TYPE
	};

	if ((abs(rand()) % 100) / 100.0f > p_powerup_on_hit) return;
	PowerUp * p = factory(POWERUPS[abs(rand()) % sizeof(POWERUPS)/sizeof(int)]);
	if (!p) return;
	p->pos(x);
	p->vel((30.0f + (rand() % 30)) * random_v3());
	powerup.push_back(p);
}

bool GameStatePlay::collision_bullet_asteroid() // {{{
{
	Bullets to_remove;
	for (Bullets::iterator i = bullet.begin(); i != bullet.end(); ++i) {
		Bullet * b = *i;
		Sprites c;
		for_each(asteroid.begin(), asteroid.end(), Sprite::Collision(b, c));

		// crack or remove collided asteroids
		if (c.size()) {
			for (Sprites::iterator j = c.begin(); j != c.end(); ++j) {
				Asteroid * a = dynamic_cast<Asteroid *>(*j);
				player.score += a->score();
				spawn_powerup_on_hit(a->pos());
				a->crack(asteroid); // add new asteroids, delete old
				asteroid.erase(find(asteroid.begin(), asteroid.end(), a));
				delete a;
			}

			// bullet hit something, destroy bullet, continue with next bullet
			to_remove.push_back(b);
		}
	}

	// remove all consumed bullets
	for (Bullets::iterator i = to_remove.begin(); i != to_remove.end(); ++i) {
		Bullet * b = *i;
		bullet.erase(find(bullet.begin(), bullet.end(), b));
		delete b;
	}
	return to_remove.size() > 0;
} // }}}

void GameStatePlay::wrap_arround() // {{{
{
	ship.wrap(0.0f, W-1, 0.0f, H-1, 0.0f, 0.0f);
	Sprite::Wrap w(0.0f, W-1, 0.0f, H-1, 0.0f, 0.0f);
	for_each(asteroid.begin(), asteroid.end(), w);
	for_each(powerup.begin(), powerup.end(), w);
	for_each(bullet.begin(), bullet.end(), w);
/*
	for_each(asteroid.begin(), asteroid.end(), Sprite::Wrap(0.0f, W-1, 0.0f, H-1, 0.0f, 0.0f));
	for_each(powerup.begin(), powerup.end(), Sprite::Wrap(0.0f, W-1, 0.0f, H-1, 0.0f, 0.0f));
	for_each(bullet.begin(), bullet.end(), Sprite::Wrap(0.0f, W-1, 0.0f, H-1, 0.0f, 0.0f));
*/
} // }}}

void GameStatePlay::update_play(GLfloat dt) // {{{
{
	ship.step(dt);
	for_each(asteroid.begin(), asteroid.end(), Sprite::Step(dt));
	for_each(powerup.begin(), powerup.end(), Sprite::Step(dt));
	for_each(bullet.begin(), bullet.end(), Sprite::Step(dt));
	wrap_arround();
} // }}}

void GameStatePlay::render() // {{{
{
	static const Font FONT1(v3(8.0f, 10.0f), Font::color_t(0.0f, 1.0f, 0.0f, 0.5f), 5.0f, 2.0f);
	static const Font FONT2(v3(30.0f, 50.0f), Font::color_t(0.0f, 1.0f, 0.0f, 0.5f), 10.0f, 3.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	ship.render();
	for_each(asteroid.begin(), asteroid.end(), Sprite::Render());
	for_each(powerup.begin(), powerup.end(), Sprite::Render());
	for_each(bullet.begin(), bullet.end(), Sprite::Render());

	glText(v3( 20.0f, H-20.0f), FONT1, "score: %u", player.score);
	glText(v3(W/2, H-20.0f), FONT1, "ships:%u", player.ships-1);
	glText(v3(W/2+150.0, H-20.0f), FONT1, "level:%u", player.level);

	if (state == READY) glText(v3(230.0, H/2.0f), FONT2, "get ready");

	SDL_GL_SwapBuffers();
} // }}}

void GameStatePlay::init_asteroids() // {{{
{
	for (unsigned int i = player.level; i; --i) {
		asteroid.push_back(new Asteroid(
			5 + abs(rand()) % 3,
			Asteroid::MAX_LEVEL,
			v3(0.0f, 0.0f, 0.0f),
			50.0f * random_v3(),
			0.0f, (rand() % 40) - 20.0f
			));
	}
} // }}}

void GameStatePlay::init_ship() // {{{
{
	ship = Ship(v3(W/2, H/2, 0.0f));
} // }}}

bool GameStatePlay::collision_ship_asteroid() // {{{
{
	if (ship.invulnerable()) return false;
	Sprites c;
	for_each(asteroid.begin(), asteroid.end(), Sprite::Collision(&ship, c));
	return c.size() > 0;
} // }}}

bool GameStatePlay::collision_ship_powerup() // {{{
{
	PowerUps to_remove;
	Sprites c;
	for_each(powerup.begin(), powerup.end(), Sprite::Collision(&ship, c));

	if (c.empty()) return false;
	for (Sprites::iterator i = c.begin(); i != c.end(); ++i) {
		PowerUp * p = dynamic_cast<PowerUp *>(*i);
		if (!p) continue;
		to_remove.push_back(p);
		ship.powerup(*p);
	}

	for (PowerUps::iterator i = to_remove.begin(); i != to_remove.end(); ++i) {
		PowerUp * o = *i;
		powerup.erase(find(powerup.begin(), powerup.end(), o));
		delete o;
	}
	return to_remove.size() > 0;
} // }}}

void GameStatePlay::handle_keys_play() // {{{
{
	Uint8 * key = SDL_GetKeyState(NULL);
	if (key[SDLK_LEFT]) ship.rot_left();
	if (key[SDLK_RIGHT]) ship.rot_right();
	ship.accelerate(key[SDLK_UP]);
} // }}}

GameStateType GameStatePlay::exec() // {{{
{
	init_ship();
	init_asteroids();
	init_time();
	for (;;) {

		// handle keyboard events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return GAMESTATE_QUIT;
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: return GAMESTATE_SCORE;
					case SDLK_SPACE:
						switch (state) {
							case PLAY:
								ship.shoot(bullet);
								break;
							case READY:
								countdown = 0.0f;
								state = PLAY;
								break;
						}
						break;
					default: break;
				}
			}
		}
		switch (state) {
			case PLAY: handle_keys_play(); break;
			case READY: break;
		}

		// play action
		GLfloat dt = update_time();
		switch (state) {
			case PLAY:
				update_play(dt);
				destroy_dead(bullet);
				destroy_dead(powerup);
				collision_bullet_asteroid();
				collision_ship_powerup();
				if (collision_ship_asteroid()) {
					player.ships--;
					if (player.ships == 0) return GAMESTATE_SCORE;
					init_ship();
					countdown = 2.0f;
					state = READY;
				}
				if (asteroid.empty()) {
					delete_all(bullet);
					delete_all(powerup);
					player.level++;
					countdown = 2.0f;
					init_asteroids();
					state = READY;
					continue;
				}
				break;
			case READY:
				countdown -= dt;
				if (countdown < 0.0f) {
					ship.invulnerable(3.0f);
					state = PLAY;
					countdown = 0.0f;
				}
				break;
		}
		render();
		delay();
	}
	return GAMESTATE_START;
} // }}}

// }}}

static GameState * factory(GameStateType t)
{
	switch (t) {
		case GAMESTATE_QUIT:  return NULL;
		case GAMESTATE_START: return new GameStateStart;
		case GAMESTATE_PLAY:  return new GameStatePlay;
		case GAMESTATE_SCORE: return new GameStateScore;
	}
	return NULL;
}

int main(int argc, char ** argv) // {{{
{
	srand(time(0));

	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("%s: unable to initialize SDL: %s\n", argv[0], SDL_GetError());
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	atexit(SDL_Quit);

	SDL_Surface * screen = SDL_SetVideoMode(W, H, 32, SDL_OPENGL); // | SDL_FULLSCREEN );
	if (screen == NULL) {
		printf("%s: unable to set video mode: %s\n", argv[0], SDL_GetError());
		return -1;
	}

	SDL_WM_SetCaption("Asteroids", NULL);
	SDL_WM_GrabInput(SDL_GRAB_OFF);

	// init OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, W, 0, H, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glFrontFace(GL_CCW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	for (GameState * state = factory(GAMESTATE_START); state;) {
		GameStateType t = state->exec();
		delete state;
		state = factory(t);
	}

	SDL_Quit();
	return 0;
} // }}}

