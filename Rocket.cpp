#include "Rocket.h"
#include <iostream>

namespace game
{
	/* Constructor */
	Rocket::Rocket(SceneNode* node , glm::vec3 dir)
	{
		offset = 0.0;
		this->direction = glm::normalize(dir);
		this->speed = 0.8;				// Hardcode speed 
		this->timer = 200;				// Hardcoded timer to get rid of rocket
		this->node = node;
		boundingRadius = 0.1;
	}

	/* Destructor */
	Rocket::~Rocket(){}

	/* Update */
	void Rocket::update() 
	{ 
		timer--;	// Decrement timer 
		// If timer is zero then we should delete the bullet else update the position of the bullet 
		if (timer <= 0) { node->del = true; }	
		else { node->Translate(glm::vec3(speed * direction)); }
	}

	// Collision between rocket and other collidables USE ABSOLUTE POSITION
	bool Rocket::collision(SceneNode* collidable , Camera* camera_)
	{
		//when at a certain distance do ray sphere collision detection
		glm::vec3 apos, mpos, dif;
		apos = collidable->GetPosition();
		mpos = node->GetPosition();
		dif = mpos - apos;

		if ((((std::sqrt(std::pow(dif[0], 2) + std::pow(dif[1], 2) + std::pow(dif[2], 2))) <= 5.0)))
		{
			glm::vec3 l;
			l = glm::normalize(node->GetPosition() - collidable->GetPosition());
			float theta = acos(glm::dot(l , camera_->GetForward()));
			float test = tan(theta) * glm::length(node->GetPosition() - collidable->GetPosition());
			if (test < 0.0 && test > -1.0) return true;
		}

		return false;
	}

	/* Collision */
	bool Rocket::collision(SceneNode* object , float off, float boundRad)
	{
		//find real center of the object
		glm::vec3 objUpVec = glm::normalize(object->getAbsoluteOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 objRealCenter = object->getAbsolutePosition() + objUpVec * off;

		//find my real center
		glm::vec3 myUpVec = glm::normalize(node->getAbsoluteOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 myRealCenter = node->getAbsolutePosition() + myUpVec * offset;

		//find difference in positions
		glm::vec3 difference = myRealCenter - objRealCenter;

		//compare distances 
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad + boundingRadius);
	}
}