#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1SceneManager.h"
#include "j1TransitionManager.h"
#include "j1Transitions.h"
#include "j1Render.h"
#include "FadeToColor.h"
#include "j1App.h"
#include "j1Window.h"
#include "p2Log.h"

// Similar code to FadeToBlack, but now we can control the color easily

FadeToColor::FadeToColor(j1Color color, float time, int scene) : j1Transitions(time) {

	this->color = color;

	this->scene = scene;

	uint w, h;
	App->win->GetWindowSize(w, h);
	screen = { 0, 0, (int)w, (int)h };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

}

FadeToColor::~FadeToColor()
{}

void FadeToColor::Start() {

	j1Transitions::Start();

	// Filling the screen with a color
	FillScreen(Interpolation(0, 255, percentage));
}

void FadeToColor::Change() {

	j1Transitions::Change();

	FillScreen(255.0f);
		
	// Scene changes
	
	if (scene == 1)
		App->scenemanager->ChangeScene(1);

	if (scene == 2)
		App->scenemanager->ChangeScene(2);

	if (scene == 3)
		App->scenemanager->ChangeScene(3);
}

void FadeToColor::Exit() {

	j1Transitions::Exit();

	// Color disappears
	FillScreen(Interpolation(255, 0, percentage));
}

void FadeToColor::FillScreen(float percentage) {
	
	if (percentage < 0)
		percentage = 0;
	
	if (percentage > 255)
		percentage = 255;

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, percentage);
	SDL_RenderFillRect(App->render->renderer, &screen);
}