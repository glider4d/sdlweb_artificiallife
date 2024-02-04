#include <SDL2/SDL.h>


#pragma once
namespace screen {
	class Time {
	public:
		int lasttime = 0;
		void Reset() {
			lasttime = SDL_GetTicks();
		}

		int GetTimer() {
			return SDL_GetTicks() - lasttime;
		}
	};
}