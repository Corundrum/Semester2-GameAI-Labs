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
	m_maxSpeed = 10.0f;
	m_turnRate = 5.0f;
	m_accelerationRate = 2.0f;

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
	setTargetPosition(target_position);
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position) * getMaxSpeed();
	getRigidBody()->velocity = getDesiredVelocity() - getRigidBody()->velocity;
}

void Starship::Seek()
{
	//compute the target direction and magnitude
	auto target_direction = getTargetPosition() - getTransform()->position;

	//normalize the target direction
	target_direction = Util::normalize(target_direction) - getCurrentDirection();

	//change our direction towards the target
	setCurrentDirection(target_direction);

	//implement LookWhereYou'reGoing

}

void Starship::m_move()
{
	Seek();

	auto delta_time = TheGame::Instance().getDeltaTime();
	auto initial_position = getTransform()->position;

	//compute initial velocity
	getRigidBody()->velocity += getCurrentDirection() * getMaxSpeed() * (delta_time);

	//compute initial acceleration
	getRigidBody()->acceleration = (getCurrentDirection() * getAccelerationRate()) * 0.5f * (delta_time);

	//compute movement
	getTransform()->position = initial_position + getRigidBody()->velocity + getRigidBody()->acceleration;
}
