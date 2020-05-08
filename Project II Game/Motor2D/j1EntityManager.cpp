#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Pathfinding.h"
#include <algorithm>

using namespace std;

struct {
	bool operator()(j1Entity* a, j1Entity* b) const
	{
		if(a->position.y != b->position.y)
			return a->position.y < b->position.y;
		else
			return a->position.x < b->position.x;
	}
} customLess;


j1EntityManager::j1EntityManager()
{

}

j1EntityManager::~j1EntityManager()
{

}

bool j1EntityManager::Start()
{
	LOG("EntityManager Started");
	return true;
}
#include "j1Map.h"
#include "j1Pathfinding.h"

bool j1EntityManager::Update(float dt)
{
	int n = 0;
	int counter = 0;
	selected_units.erase(selected_units.begin(), selected_units.end());
	while (counter != entities.size())
	{
		vector<j1Entity*>::iterator entity = entities.begin();
		entity += counter;
		for (; entity != entities.end(); entity++)
		{
			if ((*entity)->to_delete || (*entity)->to_remove)
			{
				QuickDeleteEntity(entity);
				break;
			}
			else
			{
				(*entity)->Primitive_Update(dt);
				(*entity)->Update(dt);
				if ((*entity)->selected && (*entity)->team == 1)
					n++;
				if ((*entity)->main_type == EntityType::UNIT && (*entity)->selected && (*entity)->team == 1)
					selected_units.push_back(*entity);
			}

			counter++;
		}
	}
	selected_n = n;
	if (buffer.size() != 0)
	{
		for (vector<j1Entity*>::iterator bufferentity = buffer.begin(); bufferentity != buffer.end(); bufferentity++)
		{
			entities.push_back(*bufferentity);
		}
		buffer.erase(buffer.begin(), buffer.end());
		buffer.shrink_to_fit();
	}
	std::sort(entities.begin(), entities.end(), customLess);
	////////////////////////////////////ENTITIES_DEBUG///////////////////////////////////////////////////
	if (App->godmode)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			iPoint placing_tile = App->map->WorldToMap(test.x, test.y);
			test = App->map->MapToWorld<iPoint>(placing_tile.x, placing_tile.y);
			AddEntity(test.x, test.y, EntityType::CARRIER, 1, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			iPoint placing_tile = App->map->WorldToMap(test.x, test.y);
			test = App->map->MapToWorld<iPoint>(placing_tile.x, placing_tile.y);
			AddEntity(test.x, test.y, EntityType::BOAT, 1, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			iPoint placing_tile = App->map->WorldToMap(test.x, test.y);
			test = App->map->MapToWorld<iPoint>(placing_tile.x, placing_tile.y);
			AddEntity(test.x, test.y, EntityType::BOAT, 1, 2);
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{	
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			AddEntity(test.x, test.y, EntityType::BOATHOUSE, 0, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			AddEntity(test.x, test.y, EntityType::BOATHOUSE, 0, 2);
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::HARVESTER, 1, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			AddEntity(test.x, test.y, EntityType::STORAGE, 0, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::ALL_COTTON, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::ALL_WOOD, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::ALL_METAL, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			for (vector<j1Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
				if ((*entity)->selected)
					(*entity)->CleanUp();
		}

		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			DeleteAll();
		}
	}
	//*///////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

//Schedules an entity creation in the next iteration, however the entity can be modified in the iteration of cretion with out problem. 
//Types: BOAT, HARVESTER, BOATHOUSE, STORAGE, STORAGE, TOWNHALL, ALL_COTTON, ALL_WOOD, ALL_METAL
j1Entity* j1EntityManager::AddEntity(float x, float y, EntityType type, int level, int team)
{
	switch (type)
	{
	case EntityType::BOAT:
		buffer.push_back(new j1Boat(x, y, level, team));
		break;
	case EntityType::HARVESTER:
		buffer.push_back(new j1Harvester(x, y, level, team));
		break;
	case EntityType::CARRIER:
		buffer.push_back(new j1Carrier(x, y, level, team));
		break;
	case EntityType::BOATHOUSE:
		buffer.push_back(new j1BoatHouse(x, y, team));
		break;
	case EntityType::STORAGE:
		buffer.push_back(new j1Storage(x, y, team));
		break;
	case EntityType::TOWNHALL:
		buffer.push_back(new j1TownHall(x, y, team));
		break;
	case EntityType::ALL_COTTON:
		buffer.push_back(new j1Resource(x, y, level, type));
		break;
	case EntityType::ALL_WOOD:
		buffer.push_back(new j1Resource(x, y, level, type));
		break;
	case EntityType::ALL_METAL:
		buffer.push_back(new j1Resource(x, y, level, type));
		break;
	}	

	return *(buffer.end() - 1);
}

void j1EntityManager::AddToBuffer(j1Entity* entity)
{
	buffer.push_back(entity);
}

void j1EntityManager::DeleteEntity(j1Entity* entity_)
{
	if(entities.size() != 0)
		for (auto entity = entities.begin(); entity != entities.end(); entity++)
		{
			if ((*entity) == entity_)
			{
				(*entity)->CleanUp();
				delete (*entity);
				entities.erase(entity, entity + 1);
				if(entities.size() <= entities.capacity()/2)
					entities.shrink_to_fit();
				break;
			}
		}
}

void j1EntityManager::QuickDeleteEntity(std::vector<j1Entity*>::iterator entity)
{
	if((*entity)->to_delete)
		delete (*entity);
	entities.erase(entity, entity + 1);
	if (entities.size() <= entities.capacity() / 2)
		entities.shrink_to_fit();
}

void j1EntityManager::DeleteAll()
{
	while (entities.size() != 0)
	{
		(*entities.begin())->CleanUp(); 
		delete (*entities.begin());
		entities.erase(entities.begin(), entities.begin() + 1);
	}
	entities.shrink_to_fit();

	while (buffer.size() != 0)
	{
		(*entities.begin())->CleanUp();  
		delete (*buffer.begin());
		buffer.erase(buffer.begin(), buffer.begin() + 1);
	}
	buffer.shrink_to_fit();
}

bool j1EntityManager::CleanUp()
{
	DeleteAll();

	return true;
}