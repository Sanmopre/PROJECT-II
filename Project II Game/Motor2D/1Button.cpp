#include "j1App.h"
#include "j1GUIElements.h"

#include "j1Input.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"


j1GUIButton::j1GUIButton() {
	this->type = GUItype::GUI_BUTTON;
}

j1GUIButton::~j1GUIButton() {

}

bool j1GUIButton::Start()
{

	texture_button = App->tex->Load("textures/BOTON.png");
	texture_button_1 = App->tex->Load("textures/BOTON_1.png");


	if (text != nullptr)
		label = App->gui->AddElement(GUItype::GUI_LABEL, this, map_position, inside_position, true, true, { 0,0,0,0 }, text);

	return true;
}

bool j1GUIButton::PreUpdate()
{
	if (label != nullptr)
		label->enabled = enabled;

	above = OnAbove();

	return true;
}

bool j1GUIButton::Update(float dt)
{
	if (interactable)
	{
		if (above)
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				OnClick();

			if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
			{
				if (X_drag || Y_drag)
					dragging = true;

				iPoint mouseClick = { 0,0 };
				App->input->GetMousePosition(mouseClick.x, mouseClick.y);
				drag = { mouseClick.x - ((int)this->map_position.x), mouseClick.y - ((int)this->map_position.y) };

			}

		}

		if (dragging) {

			if (App->input->GetMouseButtonDown(1) == KEY_IDLE || App->input->GetMouseButtonDown(1) == KEY_UP)
				dragging = false;
			else
			{
				Dragging();
				MovingIt(dt);
			}
		}
	}

	if (enabled) {
		if (above && interactable)
		{
			App->render->AddBlitEvent(2, texture_button_1, map_position.x - App->render->camera.x / App->win->scale, map_position.y - App->render->camera.y / App->win->scale, rect);
		}
		else {
			App->render->AddBlitEvent(2, texture_button, map_position.x - App->render->camera.x / App->win->scale, map_position.y - App->render->camera.y / App->win->scale, rect);
		}
	}


	return true;
}

bool j1GUIButton::PostUpdate()
{

	return true;
}

bool j1GUIButton::CleanUp()
{
	return true;
}

void j1GUIButton::Dragging()
{

}

void j1GUIButton::MovingIt(float dt)
{

	iPoint mouse_pos = { 0,0 };
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	fPoint currentPos = this->map_position;


	if (X_drag)
		this->map_position.x += ((mouse_pos.x - this->map_position.x) - drag.x);

	if (Y_drag)
		this->map_position.y += ((mouse_pos.y - this->map_position.y) - drag.y);


	if (parent != nullptr)
	{
		if (X_drag)
			this->inside_position.x += currentPos.x - this->map_position.x;

		if (Y_drag)
			this->inside_position.y += currentPos.y - this->map_position.y;

		if (X_drag)
			this->map_position.x = parent->map_position.x - inside_position.x;

		if (Y_drag)
			this->map_position.y = parent->map_position.y - inside_position.y;
	}

}

void j1GUIButton::OnRelease()
{

}
