
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <map>
#include <cmath>
#include <thread>
#include <mutex>
#include "Particle.h"

#pragma once

#define WIDTH_DEFAULT 640
#define HEIGHT_DEFAULT 480

namespace screen{
	struct context {
		int frameCount, timerFPS, lastFrame, fps, lastTime;
		int setFPS = 40;

	};

	struct rule_params {
		std::string name1;
		std::string name2;
		float gravity = 0;
	};
	class G final {

		context ctx_;

		SDL_Window* pWindow_;
		SDL_Renderer* pRender_;
		SDL_bool done_;
		SDL_Event event_;


		std::mutex mut_;

 
		std::map<std::string, std::vector<Particle>> particles_;

		std::vector<rule_params> rules_;
 
		int DISPLAY_WIDTH_ = WIDTH_DEFAULT;
		int DISPLAY_HEIGHT_ = HEIGHT_DEFAULT;
		int SCALE_COEFFICIENT = 1;
		bool isPause_ = false;

	public:
		G(const int width, const int height, const int coefScale);
		void drawpixel(float xm, float ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
		void clearpixel();
		void update();
		void input();

		void pause();
		void play();
		void changePause();

		void CalcPoints();
		void RenderPoints();

		void(*createUniverse)(screen::G& g);

		unsigned int random(bool width);

		std::vector<Particle>& create(int number, std::string groupName, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void create_rule(std::string&& name1, std::string&& name2, float gravity);
		void rule(std::string&& name, std::string&& name2, float g);
		~G();
	};
}