#include "PhysicsScene.h"
#include "RigidBody.h"
#include <list>

const collisionFunction PhysicsScene::collisionFunctions[] =
{
	collision_manager::circle_vs_circle, collision_manager::circle_vs_aabb, collision_manager::circle_vs_line,
	collision_manager::aabb_vs_circle, collision_manager::aabb_vs_aabb, collision_manager::aabb_vs_line,
	collision_manager::line_vs_circle, collision_manager::line_vs_aabb, collision_manager::line_vs_line
};


PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i] = nullptr;
		delete m_objects[i];
	}
	m_objects.clear();
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

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pObject : m_objects)
		{
			pObject->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}

	static std::list<PhysicsObject*> dirty;

	int objectCount = m_objects.size();

	for (int outer = 0; outer < objectCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < objectCount; inner++)
		{
			PhysicsObject* object1 = m_objects[outer];
			PhysicsObject* object2 = m_objects[inner];
			ShapeType shapeId1 = object1->getShapeID();
			ShapeType shapeId2 = object2->getShapeID();

			// using function pointers
			int functionIdx = (int(shapeId1) * int(ShapeType::SHAPECOUNT)) + int(shapeId2);
			collisionFunction collisionFunctionPtr = collisionFunctions[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
				auto result = collisionFunctionPtr(object1, object2);
				if (glm::length(result) > 0.001)
				{
					RigidBody* r1 = dynamic_cast<RigidBody*>(object1);
					RigidBody* r2 = dynamic_cast<RigidBody*>(object2);

					//if (r1) r1->setVelocity(glm::vec2(0.0f));
					//if (r2) r2->setVelocity(glm::vec2(0.0f));
					// Resolve collision
					// Seperate
					if (r1 && r2) // Even split
					{
						r1->setPosition(r1->getPosition() + 0.5f * result);
						r2->setPosition(r2->getPosition() - 0.5f * result);
						r1->resolveCollision(r2, result);
					}
					else if (r1)
					{
						r1->setPosition(r1->getPosition() + result);

						line* l = dynamic_cast<line*>(object2);
						l->resolveCollision(r1);
					}

					else if (r2)
					{
						r2->setPosition(r2->getPosition() + result);

						line* l = dynamic_cast<line*>(object1);
						l->resolveCollision(r2);
					}

					// Apply reaction force
				}
			}
		}
	}

	//for (auto pObject : m_objects)
	//{
	//	for (auto pOtherObject : m_objects)
	//	{
	//		if (pObject == pOtherObject)
	//			continue;

	//		if (std::find(dirty.begin(), dirty.end(), pObject) != dirty.end() &&
	//			std::find(dirty.begin(), dirty.end(), pOtherObject) != dirty.end())
	//			continue;

	//		RigidBody* pRigidBody = dynamic_cast<RigidBody*>(pOtherObject);

	//		if (pRigidBody->checkCollision(pObject))
	//		{
	//			pRigidBody->applySeparationForce(dynamic_cast<RigidBody*>(pObject),
	//				pRigidBody->getVelocity() * pRigidBody->getMass());

	//			dirty.push_back(pRigidBody);
	//			dirty.push_back(pOtherObject);
	//		}
	//	}
	//	dirty.clear();
	//}
}

void PhysicsScene::updateGizmos()
{
	for (auto pObject : m_objects)
	{
		pObject->makeGizmo();
	}
}