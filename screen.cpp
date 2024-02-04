#include "screen.h"
#include "Time.h"
#include <sstream>


screen::G::G(const int width, const int height, const int coefScale) : DISPLAY_WIDTH_(width), DISPLAY_HEIGHT_(height), SCALE_COEFFICIENT(coefScale) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(DISPLAY_WIDTH_ * SCALE_COEFFICIENT, DISPLAY_HEIGHT_ * SCALE_COEFFICIENT, /*SDL_WINDOW_FULLSCREEN_DESKTOP*/SDL_WINDOW_SHOWN/*SDL_VIDEO_VULKAN*/, &pWindow_, &pRender_);
	SDL_RenderSetScale(pRender_, static_cast<float>(SCALE_COEFFICIENT), static_cast<float>(SCALE_COEFFICIENT));
 
}

void screen::G::drawpixel(float xm, float ym, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {

 }

void screen::G::clearpixel() {
	for (auto& [name, points] : particles_){
		points.clear();
	}
	particles_.clear();
	rules_.clear();
	 
}

void screen::G::create_rule(std::string&& name1, std::string&& name2, float gravity) {
	rules_.emplace_back(name1, name2, gravity);
}
void screen::G::pause() {
	isPause_ = true;
}
void screen::G::play() {
	isPause_ = false;
}

void screen::G::CalcPoints() {
	for (auto&& rule_ : rules_) {
		rule(std::move(rule_.name1), std::move(rule_.name2), rule_.gravity);
	}

}

void screen::G::RenderPoints() {
	SDL_SetRenderDrawColor(pRender_, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(pRender_);
	for (const auto& [name, points] : particles_) {

		for (const auto& item : points) {
			SDL_SetRenderDrawColor(pRender_, item.color_.r, item.color_.g, item.color_.b, item.color_.a);
			SDL_RenderDrawPointF(pRender_, item.point_.x, item.point_.y);
		}

	}
	SDL_RenderPresent(pRender_);
}

void screen::G::update()
{ 
 
 
	ctx_.lastFrame = SDL_GetTicks();
	if (ctx_.lastFrame >= (ctx_.lastTime + 1000)) {
		ctx_.lastTime = ctx_.lastFrame;
		ctx_.fps = ctx_.frameCount;
		ctx_.frameCount = 0;

		//
		std::stringstream fps{};
		fps << ctx_.fps;
//        std::cout<<"fps = "<<fps.str().c_str()<<std::endl;
	}
 
 
	if (!isPause_) {
		std::lock_guard<std::mutex> lock(mut_);
		CalcPoints();

 

		RenderPoints();
 
	}

 
 
	ctx_.frameCount++;
	ctx_.timerFPS = SDL_GetTicks() - ctx_.lastFrame;
 	if (ctx_.timerFPS < (1000 / ctx_.setFPS)) {
		
		SDL_Delay((1000 / ctx_.setFPS) - ctx_.timerFPS);
	} 

	
}

 
void screen::G::rule(std::string&& name, std::string&& name2, float g)
{
	auto& particles1 = particles_[name];
	auto& particles2 = particles_[name2];
	for (auto&& a : particles1)
	{
		float fx = 0;
		float fy = 0;
		for (auto&& b : particles2) {
			float dx = a.point_.x - b.point_.x;
			float dy = a.point_.y - b.point_.y;

			double d = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));//distance

			if (d > 0 && d < 80)
			{
				float F = g * 1 / static_cast<float>(d);
				fx += (F * dx);
				fy += (F * dy);
			}
		}

		a.vx = (a.vx + fx) * 0.5f;
		a.vy = (a.vy + fy) * 0.5f;
		a.point_.x += a.vx;
		a.point_.y += a.vy;
		if (a.point_.x <= 0 || a.point_.x >= (DISPLAY_WIDTH_)) {
			if (a.point_.x <= 0) a.point_.x = 0;
			if (a.point_.x >= (DISPLAY_WIDTH_)) a.point_.x = static_cast<float>(DISPLAY_WIDTH_ - 1);
			a.vx *= -1;
		}
		if (a.point_.y <= 0 || a.point_.y >= DISPLAY_HEIGHT_) {
			if (a.point_.y <= 0) a.point_.y = 0;
			if (a.point_.y >= DISPLAY_HEIGHT_) a.point_.y = static_cast<float>(DISPLAY_HEIGHT_ - 1);
			a.vy *= -1;
		}


	}
}
 
unsigned int screen::G::random(bool width) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distDsp(1, width ? DISPLAY_WIDTH_ - 1 : DISPLAY_HEIGHT_ - 1);
	return distDsp(rng);
}
std::vector<screen::Particle>& screen::G::create(int number, std::string groupName, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {

	particles_[groupName] = std::vector<Particle>();


	for (int i = 0; i < number; i++) {
		particles_[groupName].push_back(Particle{ SDL_FPoint{ static_cast<float>(random(true)), static_cast<float>(random(false))}, SDL_Color{r, g, b, a} });

	}
	return particles_[groupName];

 
}
void screen::G::changePause() {
	if (isPause_) play(); else pause();
}
void screen::G::input() {
	while (SDL_PollEvent(&event_))
	{
		switch (event_.type)
		{
		case SDL_KEYDOWN:
			if (event_.key.keysym.scancode == SDL_SCANCODE_R) {
 
				
				pause();
				std::lock_guard<std::mutex> lock(mut_);
 				clearpixel();
				this->createUniverse(*this);
				play(); 
				
		 ;
			} else if (event_.key.keysym.scancode == SDL_SCANCODE_P) {
 				changePause();
			}
 			break;
			/* SDL_QUIT event (window close) */
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;

		default:
			break;
		}
	}
}

screen::G::~G() {
	pause();
	clearpixel();
}