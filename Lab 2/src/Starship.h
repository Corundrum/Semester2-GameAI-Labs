#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__
#include "Agent.h"

class Starship final : public Agent
{
public:
	Starship();
	~Starship();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	//getters
	float getMaxSpeed() const;
	float getTurnRate() const;
	float getAccelerationRate() const;
	glm::vec2 getDesiredVelocity() const;

	//setters
	void setMaxSpeed(float speed);
	void setTurnRate(float angle);
	void setAccelerationRate(float rate);
	void setDesiredVelocity(glm::vec2 target_position);

	//public member function
	void Seek();

	void LookWhereYoureGoing(glm::vec2 target_direction);

private:
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	//where we want to go
	glm::vec2 m_desiredVelocity;

	//private move funtion
	void m_move();
};


#endif // !1
