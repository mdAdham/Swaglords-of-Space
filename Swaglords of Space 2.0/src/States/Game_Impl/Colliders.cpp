#include "Colliders.hpp"
#include "Core/Log.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

#include "Core/Deffinitions.hpp"

#include <execution>

//#define SAVE_COLLIDER 
//#define OLD_DETECTION
#define NEW_MASKGEN_ALG

namespace _Swag {

	static int id = 0;
	Collider::Collider(Ref<sf::Sprite> spr)
		:sp(spr)
	{
		bottom = spr->getGlobalBounds().top + spr->getGlobalBounds().height;
		left = spr->getGlobalBounds().left;
		right = spr->getGlobalBounds().left + spr->getGlobalBounds().width;
		top = spr->getGlobalBounds().top;
		
		bounds = spr->getGlobalBounds();

		mask.reserve(static_cast<size_t>(bounds.height));

		if (!spr->getTexture()) return;

#ifdef SAVE_COLLIDER
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
#else
#ifdef OLD_MASKGEN_ALG


		sf::Image img = spr.getTexture()->copyToImage();
		for (int y = 0; y < img.getSize().y; y++)
		{
			std::vector<bool> tempmask;
			tempmask.reserve(bounds.width);
			for (int x = 0; x < img.getSize().x; x++)
			{
				if (img.getPixel(x, y).a > 0)
				{
					tempmask.push_back(true);
				}
				else
				{
					tempmask.push_back(false);
				}
			}
			mask.push_back(tempmask);
		}
#else
#ifdef NEW_MASKGEN_ALG

		sf::Image image = spr->getTexture()->copyToImage();
		sf::Vector2u size = image.getSize();

		mask.resize(size.y, std::vector<bool>(size.x, false));

		// Parallel Generation
		std::for_each(std::execution::par, mask.begin(), mask.end(), [&image, this, size](std::vector<bool>& row) {
			int y = static_cast<int>(& row - &mask[0]); //Get the current row index
			for (unsigned int x = 0; x < size.x; ++x)
				row[x] = (image.getPixel(x, y).a > 0);
		});

#endif // NEW_MASKGEN_ALG
#endif // OLD_MASKGEN_ALG
#endif
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
#ifdef OLD_DETECTION

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
			/*
			std::for_each(std::execution::par, int_top, int_bottom, [this, another](uint32_t i) {
				std::for_each(std::execution::par, int_left, int_right, [this, another, i](uint32_t j) {
					if (mask[j - left][i - top] && another->mask[j - another->left][i - another->top])
					{
						//_SWAG_TRACE("Collide and returns true");
						return true;
					}
					});
			});
			*/
			
			for (int y = int_top; y < int_bottom; y++)
			{
				for (int x = int_left; x < int_right; x++)
				{
					if (mask[x - left][y - top] && another->mask[x - another->left][y - another->top])
					{
						_SWAG_DEBUGS("{0},{1} -> {2},{3}", x - left, y - top, x - another->left, y - another->top);
						_SWAG_TRACE("Collide and returns true");
						return true;
					}
				}
			}
			
		}
#else
		sf::FloatRect intersection;
		if (!bounds.intersects(another->bounds, intersection))
		{
			//_SWAG_INFO("Not Intersecting");
			return false;
		}
		//_SWAG_INFO("Intersecting");
		//_SWAG_TRACE("Intersection: left: {0}, width: {1}, top: {2}, height: {3}", intersection.left, intersection.width, intersection.top, intersection.height);


		for (float y = intersection.top; y < intersection.top + intersection.height; ++y)
		{
			for (float x = intersection.left; x < intersection.left + intersection.width; ++x)
			{
				// Convert the Global Coordinate to Local Texture Coorinate - which is crutial for pixel perfect collision 
				sf::Vector2f localA = sp->getInverseTransform().transformPoint(x, y);
				//_SWAG_INFO("Inverse Transform of A {0},{1}", localA.x, localA.y);
				sf::Vector2f localB = another->sp->getInverseTransform().transformPoint(x, y);
				//_SWAG_INFO("Inverse Transform of B {0},{1}", localB.x, localB.y);

				//l - local
				int lxA = static_cast<int>(localA.x);
				int lyA = static_cast<int>(localA.y);
				int lxB = static_cast<int>(localB.x);
				int lyB = static_cast<int>(localB.y);

				//_SWAG_INFO("lxA: {0}, lyA: {1}, lxB: {2}, lyb: {3}", lxA, lyA, lxB, lyB);

				if (lxA >= 0 && lxA < mask[0].size() && lyA >= 0 && lyA < mask.size() &&
					lxB >= 0 && lxB < another->mask[0].size() && lyB >= 0 && lyB < another->mask.size() &&

					mask[lyA][lxA] && another->mask[lyB][lxB])
				{
					//_SWAG_TRACE("HEyy");
					return true; // Collision Detected
				}
			}
		}

#endif // OLD_DETECTION
		return false;
	}
	
	const sf::FloatRect& Collider::GetBounds()
	{
		return this->bounds;
	}
	void Collider::Render(sf::RenderWindow& window)
	{/*
		sf::RectangleShape spr;
		spr.setPosition(bounds.left, bounds.top);
		spr.setSize(sf::Vector2f(bounds.width, bounds.height));

		sf::Texture tex;
		sf::Image img;
		img.create(bounds.width, bounds.height, sf::Color(10, 10, 10, 100));
		for (int i = 0; i < bounds.height; i++)
		{
			for (int j = 0; j < bounds.width; j++)
			{
				if (mask[i][j] == true)
					img.setPixel(j, i, sf::Color::Green);
			}
		}
		*/
		/*
		for (int i = 0; i < mask.max_size(); i++)
		{
			for (int j = 0; j < mask[i].max_size(); j++)
			{
				if (mask[j][i] == true)
					img.setPixel(j, i, sf::Color::Red);
			}
		}
		*/
		/*tex.loadFromImage(img);

		spr.setTexture(&tex);

		window.draw(spr);*/
	}
}
