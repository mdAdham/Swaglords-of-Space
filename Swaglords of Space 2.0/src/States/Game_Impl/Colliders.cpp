#include "Colliders.hpp"
#include "Core/Log.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

#include "Core/Deffinitions.hpp"

namespace _Swag {

	static int id = 0;
	Collider::Collider(sf::Sprite& spr)
	{
		bottom = spr.getGlobalBounds().top + spr.getGlobalBounds().height;
		left = spr.getGlobalBounds().left;
		right = spr.getGlobalBounds().left + spr.getGlobalBounds().width;
		top = spr.getGlobalBounds().top;
		
		bounds = spr.getGlobalBounds();

		sf::Image img = spr.getTexture()->copyToImage();
		std::stringstream ss;
		ss << Colliders_Mask_Map_Folder << "Colliders Mask Map #" << id++ << ".txt";
		std::fstream file(ss.str(), std::ios::out);
		for (int i = 0; i < img.getSize().y; i++)
		{
			std::vector<bool> tempmask;
			for (int j = 0; j < img.getSize().x; j++)
			{
				if (img.getPixel(j, i).a > 0)
				{
					file << "#";
					tempmask.push_back(true);
				}
				else
				{
					file << " ";
					tempmask.push_back(false);
				}
			}
			file << "\n";
			mask.push_back(tempmask);
		}
		file.close();
	}
	
	void Collider::UpdateBounds(sf::FloatRect rect)
	{
		bounds = rect;
		bottom = bounds.top + bounds.height;
		left = bounds.left;
		right = bounds.left + bounds.width;
		top = bounds.top;
	}

	bool Collider::IsCollide(const Ref<Collider>& another)
	{
		sf::FloatRect intersection;
		if (bounds.intersects(another->GetBounds(), intersection))
		//if (right  <= another->left || left >= another->right ||
		//	top >= another->bottom || bottom <= another->top)
		{
			//_SWAG_TRACE("Collides intersection area: Pos:[{0}, {1}], Bounds:[{2}, {3}]", intersection.left, intersection.top, intersection.width, intersection.height);

			int_bottom = std::min(bottom, another->bottom);
			int_top = std::max(top, another->top);
			int_left = std::max(left, another->left);
			int_right = std::min(right, another->right);
			//                  or                      //
			/*int_left = intersection.left;
			int_top = intersection.top;
			int_right = intersection.left + intersection.width;
			int_bottom = intersection.top + intersection.height;*/
			//_SWAG_TRACE("Collision! bot:{0}, top:{1}, left:{2}, right:{3}", int_bottom, int_top, int_left, int_right);
			
			/*std::stringstream ss;
			ss << Colliders_Mask_Map_Folder << "Test #" << id + 10 << ".txt";
			std::fstream file(ss.str(), std::ios::out);
			for (int i = int_top; i < int_bottom; i++)
			{
				for (int j = int_left; j < int_right; j++)
				{
					file << (mask[j - left][i - top] && another->mask[j - another->left][i - another->top]) << " ";
					if (mask[j - left][i - top] && another->mask[j - another->left][i - another->top])
						_SWAG_TRACE("Collide!");
				}
				file << "\n";
			}
			file.close();*/
			for (int i = int_top; i < int_bottom; i++)
			{
				for (int j = int_left; j < int_right; j++)
				{
					if (mask[j - left][i - top] && another->mask[j - another->left][i - another->top])
					{
						//_SWAG_TRACE("Collide and returns true");
						return true;
					}
				}
			}
		}		
		return false;
	}
	
	const sf::FloatRect& Collider::GetBounds()
	{
		return this->bounds;
	}
	void Collider::Render(sf::RenderWindow& window)
	{
		/*sf::RectangleShape spr;
		spr.setPosition(bounds.left, bounds.top);
		spr.setSize(sf::Vector2f(bounds.width, bounds.height));

		sf::Texture tex;
		sf::Image img;
		img.create(bounds.width, bounds.height);
		for (int i = 0; i < bounds.width; i++)
		{
			for (int j = 0; j < bounds.height; j++)
			{
				if (mask[i][j] == true)
					img.setPixel(i, j, sf::Color::Green);
			}
		}*/
		/*tex.loadFromImage(img);

		spr.setTexture(&tex);

		window.draw(spr);*/
	}
}