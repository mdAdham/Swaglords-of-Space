#include "Enemy.hpp"
#include "Core/Deffinitions.hpp"
#include <sstream>
#include <glm.hpp>

namespace _Swag {
	static int val = 1;
	Enemy::Enemy(float pos_x, float pos_y, float enemy_damage_factor = 1, float enemy_speed_factor = 1, float enemy_points_factor = 1, float lifetime = 10.f)
	{
		this->pointCount = static_cast<unsigned int>((rand() % 8) + 3); // min = 3 max = 10
		this->speed = static_cast<float>(this->pointCount / 2) * enemy_speed_factor;
		this->damage = static_cast<unsigned int>(static_cast<float>(this->pointCount) * enemy_damage_factor);

		this->playerpoints = static_cast<int>(this->pointCount * enemy_points_factor);
		
		this->shape.setRadius(static_cast<float>(this->pointCount) * 5);
		this->shape.setPointCount(static_cast<size_t>(this->pointCount));
		this->shape.setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255));
		this->lifetime = lifetime;

		sf::Image img;
		sf::RenderTexture rtex;
		sf::Texture tex;

		rtex.create(static_cast<unsigned int>(shape.getRadius() * 2), static_cast<unsigned int>(shape.getRadius() * 2));
		rtex.clear();
		rtex.draw(shape);
		rtex.display();

		tex = rtex.getTexture();
		img = tex.copyToImage();
		img.createMaskFromColor(sf::Color::Black);
		tex.loadFromImage(img);

		this->_sprite = CreateRef<sf::Sprite>();

		_sprite->setTexture(tex);

#ifdef DEBUG
		std::stringstream ss;
		ss << Colliders_Mask_Map_Folder << val++ << ".png";
		img.saveToFile(ss.str());
#endif

		//_SWAG_TRACE("Enemy: {0},{1}", shape.getGlobalBounds().width, shape.getGlobalBounds().height);

		this->shape.setPosition(pos_x, pos_y);
		this->shape.setOrigin(shape.getRadius(), shape.getRadius());

		_sprite->setPosition(shape.getPosition());
		_sprite->setOrigin(this->shape.getOrigin());

		_collider = CreateRef<Collider>(_sprite);
	}

	//Accessors
	const sf::FloatRect Enemy::getBounds() const
	{
		return this->shape.getGlobalBounds();
	}

	const int& Enemy::getPoints() const
	{
		return this->playerpoints;
	}

	const int Enemy::getDamage() const
	{
		return this->damage;
	}

	const int Enemy::getPointCount() const
	{
		return this->pointCount;
	}

	const sf::Color Enemy::getColor() const
	{
		return this->shape.getFillColor();
	}

	const float Enemy::getRadius() const
	{
		return this->shape.getRadius();
	}

	const bool Enemy::isAlive()
	{
		this->lifetimeCounter += dt;
		if (this->lifetimeCounter >= this->lifetime)
		{
			this->lifetimeCounter = 0.f;
			return false;
		}
		return true;
	}

	//Functions
	void Enemy::update(float dt)
	{
		this->dt = dt;
		//this->shape.move(0.f, this->speed);
		//_sprite->move(0.0f, this->speed);
		this->_collider->UpdateBounds(_sprite->getGlobalBounds());
	}

	void Enemy::follow(const Ref<Player>& player)
	{
		glm::vec2 my_pos = { shape.getPosition().x, shape.getPosition().y };
		glm::vec2 pos = { player->getPos().x, player->getPos().y };

		float theta = std::atan2((pos.y - my_pos.y) * ENEMY_MOVEMENT_INERTIA, (pos.x - my_pos.x) * ENEMY_MOVEMENT_INERTIA);
		sf::Vector2f direction(glm::cos(theta), std::sin(theta));

		sf::Vector2f offset = speed * direction * dt * 100.f;

		this->shape.move(offset);
		_sprite->move(offset);
	}

	void Enemy::render(sf::RenderTarget* target) const
	{
		target->draw(this->shape);
	}
}