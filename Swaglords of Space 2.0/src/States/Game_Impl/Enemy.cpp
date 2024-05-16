#include "Enemy.hpp"
#include "Core/Deffinitions.hpp"
#include "sstream"

namespace _Swag {
	static int val = 1;
	Enemy::Enemy(float pos_x, float pos_y, float enemy_damage_factor = 1, float enemy_speed_factor = 1, float enemy_points_factor = 1)
	{
		this->pointCount = (rand() % 8) + 3; // min = 3 max = 10
		this->speed = static_cast<float>(this->pointCount / 2) * enemy_speed_factor;
		this->damage = this->pointCount * enemy_damage_factor;

		this->playerpoints = this->pointCount * enemy_points_factor;
		
		this->shape.setRadius(static_cast<float>(this->pointCount) * 5);
		this->shape.setPointCount(static_cast<size_t>(this->pointCount));
		this->shape.setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255));

		sf::Image img;
		sf::RenderTexture rtex;
		sf::Texture tex;

		rtex.create(shape.getRadius() * 2, shape.getRadius() * 2);
		rtex.clear();
		rtex.draw(shape);
		rtex.display();

		tex = rtex.getTexture();
		img = tex.copyToImage();
		img.createMaskFromColor(sf::Color::Black);
		tex.loadFromImage(img);
		_sprite.setTexture(tex);
		std::stringstream ss;
		ss << Colliders_Mask_Map_Folder << val++ << ".png";
		img.saveToFile(ss.str());

		//_SWAG_TRACE("Enemy: {0},{1}", shape.getGlobalBounds().width, shape.getGlobalBounds().height);

		this->shape.setPosition(pos_x, pos_y);
		_sprite.setPosition(shape.getPosition());
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

	const int& Enemy::getDamage() const
	{
		return this->damage;
	}

	//Functions
	void Enemy::update(float dt)
	{
		this->shape.move(0.f, this->speed);
		this->_collider->UpdateBounds(shape.getGlobalBounds());
	}

	void Enemy::render(sf::RenderTarget* target) const
	{
		target->draw(this->shape);
	}
}