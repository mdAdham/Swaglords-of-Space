#pragma once

#include <SFML/Graphics.hpp>

namespace _Swag::_gui {
	enum _button_status { BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};
	
	/*
		 * Converts a percentage value to pixels relative to the current resolution in the x-axis.
		 *
		 * @param		float perc				The percentage value.
		 * @param		sf::VideoMode& vm		The current videomode of the window (resolution).
		 *
		 * @return		float					The calculated pixel value.
		 */
	const float p2pX(const float perc, const sf::VideoMode& vm);

	/*
		 * Converts a percentage value to pixels relative to the current resolution in the y-axis.
		 *
		 * @param		float perc				The percentage value.
		 * @param		sf::VideoMode& vm		The current videomode of the window (resolution).
		 *
		 * @return		float					The calculated pixel value.
		 */
	const float p2pY(const float perc, const sf::VideoMode& vm);
	const unsigned calcCharSize(const sf::VideoMode& vm, const unsigned modifier = 60);

	class Button
	{
	private:
		short unsigned buttonState;
		short unsigned id;

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

	public:
		Button(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent,
			short unsigned id = 0);
		~Button();

		//Accessors
		const bool isPressed() const;
		const std::string getText() const;
		const short unsigned& getId() const;

		//Modifiers
		void setText(const std::string text);
		void setId(const short unsigned id);

		void updatePosition(float x, float y, float width, float height, 
							unsigned char_size);

		void updateTextCenter();

		//Functions
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};

	class DropDownList
	{
	private:
		float keytime;
		float keytimeMax;

		sf::Font& font;
		Button* activeElement;
		std::vector<Button*> list;
		bool showList;

	public:
		DropDownList(float x, float y, float width, float height,
			sf::Font& font, std::string list[],
			unsigned nrOfElements, unsigned default_index = 0);
		~DropDownList();

		//Accessors
		const unsigned short& getActiveElementId() const;

		//Functions
		const bool getKeytime();
		void updateKeytime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class ProgressBar
	{
	private:
		std::string barString;
		sf::Text text;
		float maxWidth;
		sf::RectangleShape back;
		sf::RectangleShape inner;

	public:
		ProgressBar(float x, float y, float width, float height,
			sf::Color inner_color, unsigned character_size,
			sf::VideoMode& vm, sf::Font* font = NULL);
		~ProgressBar();

		//Accessors

		//Modifiers

		//Functions
		void update(const int current_value, const int max_value);
		void render(sf::RenderTarget& target);
	};

	class RadialButton
	{
	public:
		RadialButton(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size, bool default_state,
			sf::Texture& unchecked, sf::Texture& checked);
		~RadialButton();

		//Accessors
		const bool isOn() const;
		const std::string getText() const;
		const short unsigned getId() const;

		//Modifiers
		void setText(const std::string text);
		void setId(const short unsigned id);

		void updatePosition(float x, float y, float width, float height,
			unsigned char_size);

		void updateTextCenter();

		//Functions
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	private:
		sf::RectangleShape _shape;
		sf::Text _ButtonName;
		sf::Font* _font;

		sf::Sprite _button;
		sf::Texture	&_checked, &_unchecked;

		bool tof = false;
	};
}