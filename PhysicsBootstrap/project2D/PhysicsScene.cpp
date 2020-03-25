#include "PhysicsScene.h"
#include "RigidBody.h"
#include <list>

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
	accumulatedTime += deltaTime;

	while(accumulatedTime >= m_timeStep)
	{
		for(auto pObject : m_objects)
		{
			pObject->fixedUpdate(m_gravity, m_timeStep);
		}
		
		accumulatedTime -= m_timeStep;
	}

	static std::list<PhysicsObject*> dirty;

	for (auto pObject : m_objects)
	{
		for (auto pOtherObject : m_objects)
		{
			if (pObject == pOtherObject)
				continue;

			if (std::find(dirty.begin(), dirty.end(), pObject) != dirty.end() &&
				std::find(dirty.begin(), dirty.end(), pOtherObject) != dirty.end())
				continue;

			RigidBody* pRigidBody = dynamic_cast<RigidBody*>(pOtherObject);

			if (pRigidBody->checkCollision(pObject))
			{
				pRigidBody->applySeparationForce(dynamic_cast<RigidBody*>(pObject),
					pRigidBody->getVelocity() * pRigidBody->getMass());

				dirty.push_back(pRigidBody);
				dirty.push_back(pOtherObject);
			}
		}
		dirty.clear();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pObject : m_objects)
	{
		pObject->makeGizmo();
	}
}