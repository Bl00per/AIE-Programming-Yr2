#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "PhysicsScene.h"

Application2D::Application2D()
{

}

Application2D::~Application2D()
{

}

bool Application2D::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	aie::Gizmos::add2DCircle(glm::vec2{ 0,0 }, 4.0f, 32, glm::vec4{ 1.0f, 0,0,1.0f });
	aie::Gizmos::add2DAABB(glm::vec2{ 10,0 }, glm::vec2{ 2.5f, 2.5f }, glm::vec4{ 0, 0, 1.0f,1.0f });
	aie::Gizmos::add2DLine(glm::vec2{ -12, 4 }, glm::vec2{ -8, -4 }, glm::vec4{ 1.0f, 0, 1.0f, 1.0f });

	m_2dRenderer = new aie::Renderer2D();

	m_texture = new aie::Texture("./textures/numbered_grid.tga");
	m_shipTexture = new aie::Texture("./textures/ship.png");

	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);

	m_timer = 0;

	return true;
}

void Application2D::shutdown()
{
	aie::Gizmos::destroy();
	delete m_physicsScene;
	delete m_font;
	delete m_texture;
	delete m_shipTexture;
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

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// Physics update and mesh batching
	aie::Gizmos::clear();
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();
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