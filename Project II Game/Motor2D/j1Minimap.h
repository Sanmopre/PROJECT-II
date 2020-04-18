#include "SDL\include\SDL.h"
#include "j1App.h"
#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;
struct SDL_Rect;


class j1Minimap : public j1Module
{
public:
	j1Minimap();
	virtual ~j1Minimap();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


public:
	bool display;
	iPoint position;

	bool clicking_map = false;

private:
	void DrawCamera();
	void MinimapToWorldCamera();

private:
	SDL_Texture* minimap_tex = nullptr;
	SDL_Texture* minimap_camera = nullptr;

	SDL_Rect rect = {0,0,256,128};
	SDL_Rect cameraminimap = {0,0,26,15};

};

