#include "Spider.h"

// Spider is fixed
namespace game
{
	Spider::Spider(SceneNode* spiderBody, SceneNode* spiderLeftLeg, SceneNode* spiderRightLeg)
	{
		targetPos = glm::vec3(0, 0, 0);				// Store target position
		targetOrientation = glm::quat(0, 0, 0, 0);	// Store target orientation
		lastUpdate = -1;							// Last update time
		updateTime = 0.5;							// Update time
		state = 0;									// Machine state
		speed = 0.3;								// Speed of movement the spider
		fireRate = 0;								// FireRate of the shooting webs
		maxFireRate = 120;							// Maximum fire rate of shooting
		maxHealth = 30;								// Maximum health
		health = maxHealth;							// Health
		firing = false;								// Controls if the enemy is shooting
		shotTimer = -1.f;
		timer = 5;									// Timer for leg movement
		legMovement = true;							// Check for leg movement
		isMoving = false;							// Check for movement
		boundingRadius = 0.8;						// Radius bounding
		offset = 0.7;

		onFloor = false;							// Check whether on floor or not
		gravity = -0.2f;							// Gravity
		body = spiderBody;							// Body of the spider node
		leftLeg = spiderLeftLeg;					// Left leg of the spider node
		rightLeg = spiderRightLeg;					// Right leg of the spider node
	}

	Spider::~Spider() {}

	/* Update */
	void Spider::update()
	{
		if (!onFloor) { body->Translate(glm::vec3(0, gravity, 0)); }
		fireRate--;

		direction = glm::normalize(targetPos - body->getAbsolutePosition());

		body->SetOrientation(targetOrientation);

		if (isMoving)
		{
			if (legMovement)
			{
				timer--;
				leftLeg->Translate(glm::vec3(0, 0, 0.01));
				rightLeg->Translate(glm::vec3(0, 0, -0.01));
				if (timer <= 0) { legMovement = false; }
			}
			else
			{
				timer++;
				leftLeg->Translate(glm::vec3(0, 0, -0.01));
				rightLeg->Translate(glm::vec3(0, 0, 0.01));
				if (timer >= 10) { legMovement = true; }
			}
		}

		time_t t = time(0);
		if (lastUpdate == -1 || t - lastUpdate > updateTime)
		{
			state = int(rand() % 3);
			lastUpdate = t;
		}

		// State machine
		if (state == 0) {}
	    else if (state == 1) 
		{ 
			//Move to player
			//body->SetOrientation(targetOrientation);

			//body->Translate(glm::vec3(((targetPos.x - body->GetPosition().x) * 0.01), ((targetPos.y - body->GetPosition().y) * 0.01), ((targetPos.z - body->GetPosition().z) * 0.01)));
			isMoving = true;
		}
		else if (state == 2) 
		{ 
			//Attack
			if (fireRate <= 0)
			{
				this->firing = true;
				fireRate = maxFireRate;
			}
			else { state = 0; }
			isMoving = true;
		}
		else if (state == 3) {} //Patrol
		else { std::cout << "Invalid state in Spider" << std::endl; }

		// Check Webs 
		for (int i = 0; i < projectiles.size(); i++)
		{
			// when timer is 0 delete the rocket
			if (projectiles[i]->timer <= 0) { projectiles.erase(projectiles.begin() + i); }
			else { projectiles[i]->update(); }
		}
	}

	/* Update target orientation */
	void Spider::updateTargetOrientation(glm::quat orient)
	{
		targetOrientation = orient;
		//glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
		//targetOrientation = rotation * targetOrientation;
	}

	/* Collision */
	bool Spider::collision(SceneNode* object, float off, float boundRad)
	{
		//find real center of the object
		glm::vec3 objUpVec = glm::normalize(object->getAbsoluteOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 objRealCenter = object->getAbsolutePosition() + objUpVec * off;

		//find my real center
		glm::vec3 myUpVec = glm::normalize(body->getAbsoluteOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 myRealCenter = body->getAbsolutePosition() + myUpVec * offset;

		//find difference in positions
		glm::vec3 difference = myRealCenter - objRealCenter;

		//compare distances 
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad + boundingRadius);
	}
}