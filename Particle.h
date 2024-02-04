#include <SDL2/SDL.h>
#pragma once

namespace screen {
	struct Particle {
		SDL_FPoint point_;
		SDL_Color color_;
		float vx;
		float fx;
		float vy;
		float fy;
	};
}