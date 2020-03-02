#ifndef __j1INGAMEUI_H__
#define __j1INGAMEUI_H__

#include "j1Module.h"

struct Game_Menu {

	j1GUIelement* Menu_button = nullptr;
	j1GUIelement* Title = nullptr;
	j1GUIelement* Image = nullptr;
	j1GUIelement* Exit_button = nullptr;
	j1GUIelement* Return_button = nullptr;
	j1GUIelement* Resume_button = nullptr;
	j1GUIelement* Music_scroll = nullptr;
	j1GUIelement* Save = nullptr;
	j1GUIelement* Load = nullptr;
	j1GUIelement* label1 = nullptr;
};

class j1InGameUI : public j1Module
{
public:

	j1InGameUI();
	~j1InGameUI();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void Add_UI();
	void Activate_Menu();
	void GUI_Event_Manager(GUI_Event type, j1GUIelement* element);

private:
	int MiddleScreenW;
	int MiddleScreenH;
	Game_Menu menu;
};

#endif // __j1WINDOW_H__