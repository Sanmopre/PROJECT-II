#include "j1Entities.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1InGameUI.h"
#include "j1EntityManager.h"

j1Unit::j1Unit() 
{ 
	main_type = EntityType::UNIT;
	orientation = Orientation::NORTH_WEST; 
	player_command = false;
	damage = 0;
}

j1Unit::~j1Unit()
{
	target = nullptr;
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
}

void j1Unit::Primitive_Update(float dt)
{
	showing_hpbar = false;

	SelectAnimation();

	if (App->InGameUI->selected != nullptr)
		if (this == App->InGameUI->selected->trading_entity)
			ShowHPbar(10, 5, -10);

	if (selected)
	{
		if (this == App->InGameUI->selected)
		{
			Trading();
			App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, trading_range, 0 }, false, false, 0, 255, 0, 200);
			App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, range, 0 }, false, false, 255, 0, 0);
		}

		ShowHPbar(10, 5, -10);
	}

	selectable_area.x = GetRenderPositionX();
	selectable_area.y = GetRenderPositionY();
}

void j1Unit::UpdateMap(Node* node)
{
	map = &node->island->map;
}

void j1Unit::GoTo(fPoint destination, NodeType terrain)
{
	vector<fPoint> p = App->pathfinding->PathTo(this->position, destination, terrain, map);
	if (p.size() != 0)
	{
		path.swap(p);

		this->destination = *path.begin();
		if (team == 1)
			player_command = true;
	}
	else
		this->destination = position;
}

void  j1Unit::Move(float dt)
{
	orientation = Orientation::NONE;

	if (position.x < destination.x)
	{
		orientation = Orientation::EAST;
		position.x += speed * dt;

		if (position.x > destination.x)
			position.x = destination.x;
	}
	if (position.x > destination.x)
	{
		orientation = Orientation::WEST;
		position.x -= speed * dt;

		if (position.x < destination.x)
			position.x = destination.x;
	}
	if (position.y < destination.y)
	{
		if (orientation == Orientation::NONE)
		{
			orientation = Orientation::SOUTH;
			position.y += speed * dt;
		}
		else
		{
			if (orientation == Orientation::EAST)
				orientation = Orientation::SOUTH_EAST;
			else// if (orientation == Orientation::WEST)
				orientation = Orientation::SOUTH_WEST;

			position.y += speed / 2 * dt;
		}

		if (position.y > destination.y)
			position.y = destination.y;
	}
	if (position.y > destination.y)
	{
		if (orientation == Orientation::NONE)
		{
			orientation = Orientation::NORTH;
			position.y -= speed * dt;
		}
		else
		{
			if (orientation == Orientation::EAST)
				orientation = Orientation::NORTH_EAST;
			else// if (orientation == Orientation::WEST)
				orientation = Orientation::NORTH_WEST;

			position.y -= speed / 2 * dt;
		}

		if (position.y < destination.y)
			position.y = destination.y;
	}
}

void j1Unit::NextStep()
{
	if (path.size() != 0)
	{
		if (path.size() == 1)
			player_command = false;
		path.erase(path.begin());
		destination = *path.begin();
	}
}

void  j1Unit::SetPosition(fPoint position)
{
	this->position = position;
	destination = position;
	path.erase(path.begin(), path.end());
}

void j1Unit::SelectAnimation()
{
	switch (orientation)
	{
	case Orientation::NORTH:
		rect = north.GetCurrentFrame();
		break;

	case Orientation::NORTH_EAST:
		rect = north_east.GetCurrentFrame();

		break;

	case Orientation::EAST:
		rect = east.GetCurrentFrame();

		break;

	case Orientation::SOUTH_EAST:
		rect = south_east.GetCurrentFrame();

		break;

	case Orientation::SOUTH:
		rect = south.GetCurrentFrame();

		break;

	case Orientation::SOUTH_WEST:
		rect = south_west.GetCurrentFrame();

		break;

	case Orientation::WEST:
		rect = west.GetCurrentFrame();

		break;

	case Orientation::NORTH_WEST:
		rect = north_west.GetCurrentFrame();

		break;
	}
}

void j1Unit::Chase(int range, int enemy)
{
	if (target == nullptr)
	{
		if (path.size() == 0)
		{
			j1Entity* chased = FindTarget(position.x, position.y, range, EntityType::NONE, EntityType::NONE, enemy);
			if (chased != nullptr)
			{
				GoTo(chased->position, terrain);
				player_command = false;
			}
		}
	}
	else
	{
		if (path.size() != 0 && !player_command)
		{
			destination = *path.begin();
			path.erase(path.begin(), path.end());
		}
	}

	if (selected && App->show_chasing_range)
		App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, range, 0 }, false, false, 0, 0, 255);
}

void j1Unit::GetBasicAnimations()
{
	north = App->anim->GetAnimation("unit-north");
	north_east = App->anim->GetAnimation("unit-north_east");
	north_west = App->anim->GetAnimation("unit-north_west");
	south = App->anim->GetAnimation("unit-south");
	south_east = App->anim->GetAnimation("unit-south_east");
	south_west = App->anim->GetAnimation("unit-south_west");
	east = App->anim->GetAnimation("unit-east");
	west = App->anim->GetAnimation("unit-west");

	rect = north.GetCurrentFrame();
}

void j1Unit::GetStats(int& attack, int& health, int& maxhealth, int& speed, int& maxresources)
{
	attack = damage;
	health = this->health;
	maxhealth = max_health;
	speed = this->speed;
	maxresources = load.maxweight;
}

void j1Unit::InfoLoad(pugi::xml_node& data)
{
	pugi::xml_node node = data.child("unit");

	destination.x = node.child("destination").attribute("x").as_float();
	destination.y = node.child("destination").attribute("y").as_float();

	pugi::xml_node p_node = node.child("path");

	player_command = p_node.attribute("player_command").as_bool();

	for (pugi::xml_node step = p_node.child("step"); step != NULL; step = step.next_sibling("step"))
	{
		fPoint s = { step.attribute("x").as_float(),step.attribute("y").as_float() };

		path.push_back(s);
	}

	DeepInfoLoad(node);
}

void j1Unit::Save(pugi::xml_node& data)
{
	pugi::xml_node node = data.append_child("unit");

	pugi::xml_node pos = node.append_child("destination");
	pos.append_attribute("x").set_value(destination.x);
	pos.append_attribute("y").set_value(destination.y);

	pugi::xml_node p_node = node.append_child("path");
	p_node.append_attribute("player_command").set_value(player_command);
	for (auto p = path.begin(); p != path.end(); p++)
	{
		pos = p_node.append_child("step");
		pos.append_attribute("x").set_value((*p).x);
		pos.append_attribute("y").set_value((*p).y);
	}

	DeepSave(node);
}