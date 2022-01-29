#include "Starship.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"


Starship::Starship()
{
	TextureManager::Instance().load("../Assets/textures/ncl.png","starship");

	const auto size = TextureManager::Instance().getTextureSize("starship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(70.0f, 400.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//starting propeties
	m_maxSpeed = 20.0f;
	m_turnRate = 5.0f;
	m_accelerationRate = 4.0f;

	setType(AGENT);
}

Starship::~Starship()
= default;

void Starship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance().draw("starship", x, y, 0, 255, true);
}

void Starship::update()
{
	m_move();
}

void Starship::clean()
{
}

float Starship::getMaxSpeed() const
{
	return m_maxSpeed;
}

float Starship::getTurnRate() const
{
	return m_turnRate;
}

float Starship::getAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 Starship::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

void Starship::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void Starship::setTurnRate(const float angle)
{
	m_turnRate = angle;
}

void Starship::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void Starship::setDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position);
}

void Starship::Seek()
{
	setDesiredVelocity(getTargetPosition());

	//normalize the target direction
	const glm::vec2 steering_direction = getDesiredVelocity() - getCurrentDirection();

	//change our direction towards the target
	setCurrentDirection(steering_direction); // instantly

	getRigidBody()->acceleration = getCurrentDirection() * getAccelerationRate();
}

void Starship::LookWhereYoureGoing()
{
}

void Starship::m_move()
{
	Seek();

	const float delta_time = TheGame::Instance().getDeltaTime();
	//compute position term
	const glm::vec2 initial_position = getTransform()->position;

	//compute initial velocity
	const glm::vec2 velocity_term = getRigidBody()->velocity;// *delta_time;

	//compute initial acceleration
	const glm::vec2 acceleration_term = getRigidBody()->acceleration * 0.5f *delta_time;

	//compute new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	getTransform()->position = final_position;

	//add our acceleration to velocity
	getRigidBody()->velocity += getRigidBody()->acceleration;

	//clmap velocity at max speed
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, getMaxSpeed());
}
