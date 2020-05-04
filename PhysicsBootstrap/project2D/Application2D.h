#pragma once

#include "Application.h"
#include "Renderer2D.h"

class PhysicsScene;
class circle;
class alignedBoundingBox;


class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	circle* star;
	circle* earth;
	alignedBoundingBox* box;
	circle* circle1;
	circle* circle2;
	circle* circle3;

	float m_timer;
};