#include <iostream>
#include <SDL.h>
#include <vector>
#include <random>
#include "screen.h"
#include <thread>
#include <emscripten.h>
 


 
const int DISPLAY_HEIGHT = 350;
const int DISPLAY_WIDTH = 640;

const int SCALE_COEFFICIENT = 4;

std::vector<int> particles;



unsigned int random(bool width) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distDsp(1, width ? DISPLAY_WIDTH - 1  : DISPLAY_HEIGHT - 1);
	return distDsp(rng);
}


int updateLoopThread(screen::G &g){
	while (true) {
		g.update();
	}
}

void mainloop(void *arg){
    screen::G *g = static_cast<screen::G*>(arg);
    g->update();
    g->input();
}

void loop(screen::G& g){
	std::thread gThread(updateLoopThread, std::ref(g));
	while (true) {
		g.input();
	}


	gThread.join();
}


void CreateGUniverse(screen::G& g) {
	g.createUniverse = [](screen::G& g) {
		g.create(1000, "yellow", 255, 255, 0, 255);
		g.create(180, "red", 255, 0, 0, 255);
		g.create(100, "green", 0, 255, 0, 255);
		//	g.create(300, "blue", 0, 0, 255, 0);
		g.create_rule("red", "red", 0.1f);
		g.create_rule("yellow", "red", 0.15f);
//		g.create_rule("yellow", "green", -0.05f);
//		g.create_rule("yellow", "yellow", 0.01);
		g.create_rule("green", "green", -.7f);
		g.create_rule("green", "red", -.2f);
		g.create_rule("red", "green", -.1f);
		};
	g.createUniverse(g);
//	g.createUniverse();
}

void ReseteGUniverse(screen::G& g) {

}

int main(int argc, char* argv[]) {

	screen::G g(DISPLAY_WIDTH, DISPLAY_HEIGHT, SCALE_COEFFICIENT);
	CreateGUniverse(g);
//    std::thread gThread(updateLoopThread, std::ref(g));
    emscripten_set_main_loop_arg(mainloop, &g, -1, 1);

	//loop(g);
 
	return 0; 
}