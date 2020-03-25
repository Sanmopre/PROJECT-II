#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>

j1Entity::~j1Entity()
{
	tradeable_list.erase(tradeable_list.begin(), tradeable_list.end());
	tradeable_list.shrink_to_fit();
}

void  j1Entity::ShowHPbar(int extra_width, int height)
{
	if (!showing_hpbar)
	{
		showing_hpbar = true;

		SDL_Rect Brect = { GetRenderPositionX() - extra_width, GetRenderPositionY() - 20, rect.w + extra_width * 2, height };
		Color Bcolor(96u, 96u, 96u);

		float max_w = Brect.w;
		float width = max_w;

		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health grey bar

		width /= max_health;
		width *= health;
		Brect.w = width;
		Bcolor.SetColor(0u, 204u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health bar

		Brect.y += 10;

		width = max_w;
		width /= storage.maxweight;
		width *= storage.wood;
		Brect.w = width;
		Bcolor.SetColor(120u, 72u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // wood bar

		Brect.x += Brect.w;
		width = max_w;
		width /= storage.maxweight;
		width *= storage.cotton;
		Brect.w = width;
		Bcolor.SetColor(240u, 240u, 240u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // cotton bar

		Brect.x += Brect.w;
		width = max_w;
		width /= storage.maxweight;
		width *= storage.metal;
		Brect.w = width;
		Bcolor.SetColor(107u, 120u, 119u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // metal bar
	}
}

void j1Entity::Trading()
{
	for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if (*e != this && team == (*e)->team)
			if (position.x + trading_range > (*e)->position.x &&
				position.x - trading_range < (*e)->position.x &&
				position.y + trading_range >(*e)->position.y &&
				position.y - trading_range < (*e)->position.y)
			{
				bool found = false;
				for (std::vector<j1Entity*>::iterator t = tradeable_list.begin(); t != tradeable_list.end(); t++)
				{
					if (*e == *t)
					{
						found = true;
						break;
					}
				}
				if (!found)
					tradeable_list.push_back(*e);
			}
			else
			{
				for (std::vector<j1Entity*>::iterator t = tradeable_list.begin(); t != tradeable_list.end(); t++)
				{
					if (*e == *t)
					{
						tradeable_list.erase(t);
						break;
					}
				}
			}
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		trading_entity_offset--;

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		trading_entity_offset++;
	
	if (trading_entity_offset < 0)
		trading_entity_offset = tradeable_list.size() - 1;
	if (trading_entity_offset > tradeable_list.size() - 1)
		trading_entity_offset = 0;

	int counter = 0;

	while (counter != tradeable_list.size())
	{
		std::vector<j1Entity*>::iterator t = tradeable_list.begin();
		t += counter;
		for (; t != tradeable_list.end(); t++)
		{
			if ((*t)->health == 0)
			{
				tradeable_list.erase(t);
				tradeable_list.shrink_to_fit();
				break;
			}
			else if (t == tradeable_list.begin() + trading_entity_offset)
			{
				trading_entity = *t;
				App->render->AddBlitEvent(0, nullptr, 0, 0, { (*t)->GetRenderPositionX(), (*t)->GetRenderPositionY(), 30, 30 }, false, false, 255, 255, 0, 100);
			}

			counter++;
		}
	}
}

j1Entity* j1Entity::FindTarget(int range, EntityType type)
{
	j1Entity* ret = nullptr;

	float targetdistance = range;
	float distance = 0.0f;

	for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
		if (*e != this && (*e)->team != team)
			if(type == EntityType::NONE || (*e)->type == type)
				if (position.x + range > (*e)->position.x &&
					position.x - range < (*e)->position.x &&
					position.y + range >(*e)->position.y &&
					position.y - range < (*e)->position.y)
				{
					distance = sqrtf((position.x - (*e)->position.x) * (position.x - (*e)->position.x) + (position.y - (*e)->position.y) * (position.y - (*e)->position.y));

					if (distance < range && distance < targetdistance)
					{
						ret = *e;
						targetdistance = distance;
					}

					ShowHPbar(10, 5);
				}

	if (distance == 0.0f)
		ret = nullptr;

	return ret;
}

int j1Entity::GetRenderPositionX()
{
	return position.x - rect.w / 2;
}

int j1Entity::GetRenderPositionY()
{
	return position.y - rect.h / 2;
}
