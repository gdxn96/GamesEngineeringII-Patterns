// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include "AutoLister.h"

template<typename T>
T* getComponentById(int id)
{
	auto& list = EntityList::get<T>();
	for (auto& el : list)
	{
		if (el->ID == id)
		{
			return el;
		}
	}

	return nullptr;
}

struct Vec
{
	Vec(float x = 0, float y = 0) : x(x), y(y)
	{

	}
	float x, y;
};


class ISystem
{
public:
	virtual void process() = 0;
};

struct IComponent
{
	IComponent(int id) : ID(id) {};
	const int ID;	
};

class IEntity : public AutoLister<IEntity>
{
public:
	IEntity(int id, std::vector<IComponent*> list) : ID(id), m_components(list)
	{
	}
	const int ID;
protected:
	std::vector<IComponent*> m_components;
	
};

struct HealthComponent : public IComponent, public AutoLister<HealthComponent>
{
	HealthComponent(int objectId, int health) : IComponent(objectId), health(health) {};
	int health;
};

struct PositionComponent : public IComponent, public AutoLister<PositionComponent>
{
	PositionComponent(int objectId, Vec pos) : IComponent(objectId), position(pos) {};
	Vec position;
};

struct ControlComponent : public IComponent, public AutoLister<ControlComponent>
{
	ControlComponent(int objectId) : IComponent(objectId) {};
	void process()
	{
		std::cout << "processing input" << std::endl;
	}
};


//Control system - Deals with position & control components
class ControlSystem : public ISystem
{
public:
	static void process()
	{
		auto& controlComponents = EntityList::get<ControlComponent>();
		for (auto& component : controlComponents)
		{
			//process all control components
			component->process();
		}
	}
};

//Render system - position
class RenderSystem : public ISystem
{
public:
	static void process()
	{
		auto& positionComponents = EntityList::get<PositionComponent>();
		for (auto& component : positionComponents)
		{
			//draw each one at position
			std::cout << "Render component at position X:" << component->position.x << " Y:" << component->position.y << std::endl;
		}
	}
};

//AI system - health, position
class AISystem : public ISystem
{
public:
	static void process()
	{
		auto& entities = EntityList::get<IEntity>();

		for (auto& entity : entities)
		{
			auto health = getComponentById<HealthComponent>(entity->ID);
			auto position = getComponentById<PositionComponent>(entity->ID);

			if (health && position)
			{
				//do something with health and position
				std::cout << "AISystem updating object: " << entity->ID << " at position " << position->position.x << ", " << position->position.y << " with health " << health->health << std::endl;
			}
		}
	}
};

int counter()
{
	static int x = 0;
	x++;
	return x;
}


int main() 
{
	//Cat - health, position
	int id = counter();
	IEntity cat = IEntity(id, {new HealthComponent(id,900), new PositionComponent(id, Vec(50, 50))});

	//Alien - health, position
	id = counter();
	IEntity Alien = IEntity(id, { new HealthComponent(id,50), new PositionComponent(id, Vec(100, 50)) });

	//Dog - health, position
	id = counter();
	IEntity dog = IEntity(id, { new HealthComponent(id,20), new PositionComponent(id, Vec(100, 100)) });

	//Player - health, position, control components
	id = counter();
	IEntity player = IEntity(id, { new HealthComponent(id,500), new PositionComponent(id, Vec(0, 0)), new ControlComponent(id) });


	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((int)((1)* 1000)));
		AISystem::process();
		ControlSystem::process();
		RenderSystem::process();
	}

	system("PAUSE");
}

