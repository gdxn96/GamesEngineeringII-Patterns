// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <string>
#include <iostream>
#include <thread>

class GameObject;
class Falling;
class Idle;
class Jumping;
class Walking;

class IState
{
public:
	virtual ~IState() {}
	virtual void update(GameObject& object, float dt) 
	{}
};

class GameObject
{
public:
	GameObject(IState* state) : m_state(state)
	{
	}

	void update(float dt)
	{
		m_state->update(*this, dt);
	}

	void SetState(IState * s)
	{
		if (s != nullptr)
		{
			delete m_state;
			m_state = s;
		}
	}

private:
	IState* m_state;
};

class Falling : public IState
{
public:
	void update(GameObject& object, float dt);

private:

};

class Jumping : public IState
{
public:
	void update(GameObject& object, float dt) override 
	{
		std::cout << "Jumping" << std::endl;
		object.SetState(new Falling());
	}

private:

};

class Walking : public IState
{
public:
	void update(GameObject& object, float dt) override 
	{
		std::cout << "Walking" << std::endl;
		object.SetState(new Jumping());
	}

private:

};

class Idle : public IState
{
public:
	void update(GameObject& object, float dt) override
	{
		std::cout << "Idle" << std::endl;
		object.SetState(new Walking());
	}

private:

};

void Falling::update(GameObject& object, float dt) 
{
	std::cout << "Falling" << std::endl;
	object.SetState(new Idle());
}



int main() 
{
	GameObject x(new Idle());
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		x.update(1);
	}
	system("PAUSE");
}

