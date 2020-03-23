#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::~PhysicsScene()
{
	while (!m_objects.empty())
	{
		m_objects.erase(m_objects.end() - 1);
	}
}

void PhysicsScene::addObject(PhysicsObject* object)
{
	m_objects.push_back(object);
}

void PhysicsScene::removeObject(PhysicsObject* object)
{
	auto location = std::find(m_objects.begin(), m_objects.end(), object);
	m_objects.erase(location);
	delete object;
	object = nullptr;
}

void PhysicsScene::update(float deltaTime)
{
	// update physics at a fixed time step
	
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while(accumulatedTime >= m_timeStep)
	{
		for(auto pObject : m_objects)
		{
			pObject->fixedUpdate(m_gravity, m_timeStep);
		}
		
		accumulatedTime -= m_timeStep;}
}

void PhysicsScene::updateGizmos()
{
	for (auto pObject : m_objects)
	{
		pObject->makeGizmo();
	}
}