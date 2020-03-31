#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "PhysicsScene.h"
#include "Circle.h"
#include "alignedBoundingBox.h"
#include "line.h"

Application2D::Application2D()
{

}

Application2D::~Application2D()
{

}

bool Application2D::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	//aie::Gizmos::add2DCircle(glm::vec2{ 0,0 }, 4.0f, 32, glm::vec4{ 1.0f, 0,0,1.0f });
	//aie::Gizmos::add2DAABB(glm::vec2{ 10,0 }, glm::vec2{ 2.5f, 2.5f }, glm::vec4{ 0, 0, 1.0f,1.0f });
	//aie::Gizmos::add2DLine(glm::vec2{ -12, 4 }, glm::vec2{ -8, -4 }, glm::vec4{ 1.0f, 0, 1.0f, 1.0f });

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	m_physicsScene->setGravity(glm::vec2(0.0f));

	star = new circle(glm::vec2(10.0f, 0.0f), glm::vec2(0.0f), 1.0f, 1.0f);
	star->setColour(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	earth = new circle(glm::vec2(-10.0f, 0.0f), glm::vec2(0.0f), 1.0f, 1.0f);
	earth->setColour(glm::vec4(0, 0.5f, 1.0f, 1.0f));
	box = new alignedBoundingBox(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f), 1.0f, glm::vec2(1.0f));
	box->setColour(glm::vec4(0.8f, 0.3f, 0.0f, 1.0f));

	line* base = new line(glm::vec2(0, 1.0f), -8.0f);		// Base
	line* base2 = new line(glm::vec2(1.0f, 0.0f), -15.0f);	// Left Wall
	line* base3 = new line(glm::vec2(1.0, 1.0f), -14.0f);	// Left Corner
	line* base4 = new line(glm::vec2(-1.0f, 0.0f), -15.0f); // Right Wall
	line* base5 = new line(glm::vec2(-1.0f, 1.0f), -14.0f); // Right Corner

	m_physicsScene->addObject(star);
	m_physicsScene->addObject(box);
	m_physicsScene->addObject(earth);
	//m_physicsScene->addObject(new alignedBoundingBox(glm::vec2(0.1f, -2.0f), glm::vec2(0.0f), 1.0f, glm::vec2(1.0f)));
	//m_physicsScene->addObject(new alignedBoundingBox(glm::vec2(-100.0f, -2.0f), glm::vec2(0.0f), 1.0f, glm::vec2(1.0f)));
	//m_physicsScene->addObject(new alignedBoundingBox(glm::vec2(-10.0f, -2.0f), glm::vec2(0.0f), 1.0f, glm::vec2(1.0f)));
	//m_physicsScene->addObject(new alignedBoundingBox(glm::vec2(-20.0f, -2.0f), glm::vec2(0.0f), 1.0f, glm::vec2(1.0f)));
	//m_physicsScene->addObject(new alignedBoundingBox(glm::vec2(-25.0f, -2.0f), glm::vec2(0.0f), 1.0f, glm::vec2(1.0f)));
	m_physicsScene->addObject(base);
	m_physicsScene->addObject(base2);
	m_physicsScene->addObject(base3);
	m_physicsScene->addObject(base4);
	m_physicsScene->addObject(base5);

	m_timer = 0;

	return true;
}

void Application2D::shutdown()
{
	aie::Gizmos::destroy();
	delete star;
	delete box;
	delete earth;
	delete m_physicsScene;
	delete m_font;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime)
{
	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	// Update the camera position using the arrow keys
	float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->setCameraPos(camPosX, camPosY);

	if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
	{
		m_physicsScene->setGravity(glm::vec2(0.0f, -10.0f));
		//box->applyForce(glm::vec2(10.0f));
		star->applyForce(glm::vec2(-10.0f, 10.0f));
		earth->applyForce(glm::vec2(10.0f, 10.0f));
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// Physics update and mesh batching
	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// V = u + at
	// ^X = ut + 1/2 a t^2

	//float u = 10.0f;
	//float a = -10.0f;
	//int samples = 10;
	//float t = 2.0f / samples;

	//for (int i = 0; i < samples; i++)
	//{
	//	float thisT = t * i;
	//	//float x = u * thisT + 0.5f * 0 * (thisT * thisT);
	//	float x = u * thisT;
	//	float y = u * thisT + 0.5f * a * (thisT * thisT);

	//	aie::Gizmos::add2DCircle(glm::vec2(-10.0f, 0.0f) + glm::vec2(x, y), 0.1f, 16, glm::vec4(0, 1.0f, 0, 1.0f));
	//}

	//aie::Gizmos::add2DCircle(star->getPosition(), star->getRadius() - 0.05f, 16, glm::vec4(1.0f, 0, 0, 1.0f));
	//aie::Gizmos::add2DAABB(box->getPosition(), glm::vec2(box->getHeight(), box->getWidth()) * 0.5f, glm::vec4(0, 0.5f, 1.0f, 1.0f));
}

void Application2D::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw a rotating red box
	//m_2dRenderer->setRenderColour(1, 0, 0, 1);
	//m_2dRenderer->drawBox(200, 200, 60, 60, m_timer);

	// Draw all batched Gizmos
	aie::Gizmos::draw2D(glm::ortho<float>(-16, 16, -9, 9, -1, 1));

	// output some text, uses the last used colour
	m_2dRenderer->setRenderColour(1, 1, 1, 1);
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
}