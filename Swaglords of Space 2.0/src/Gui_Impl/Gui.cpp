#include <math.h>
#include "Gui.hpp"

#include "Core/Log.hpp"

namespace _Swag::_gui {
	const float p2pX(const float perc, const sf::VideoMode& vm)
	{
		/*
		 * Converts a percentage value to pixels relative to the current resolution in the x-axis.
		 *
		 * @param		float perc				The percentage value.
		 * @param		sf::VideoMode& vm		The current videomode of the window (resolution).
		 *
		 * @return		float					The calculated pixel value.
		 */

		return std::floor(static_cast<float>(vm.width) * (perc / 100.f));
	}

	const float p2pY(const float perc, const sf::VideoMode& vm)
	{
		/*
		 * Converts a percentage value to pixels relative to the current resolution in the y-axis.
		 *
		 * @param		float perc				The percentage value.
		 * @param		sf::VideoMode& vm		The current videomode of the window (resolution).
		 *
		 * @return		float					The calculated pixel value.
		 */

		return std::floor(static_cast<float>(vm.height) * (perc / 100.f));
	}

	const unsigned calcCharSize(const sf::VideoMode& vm, const unsigned modifier)
	{
		/*
		 * Calculates the character size for text using the current resolution and a constant.
		 *
		 * @param		sf::VideoMode& vm		The current videomode of the window (resolution).
		 * @param		unsigned modifier		Used to modify the character size in a more custom way.
		 *
		 * @return		unsigned				The calculated character size value.
		 */

		return static_cast<unsigned>((vm.width + vm.height) / modifier);
	}

	Button::Button(float x, float y, float width, float height,
		sf::Font* font, std::string text, unsigned character_size,
		sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
		sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
		sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
		short unsigned id)
	{
		this->buttonState = BTN_IDLE;
		this->id = id;

		this->shape.setPosition(sf::Vector2f(x, y));
		this->shape.setSize(sf::Vector2f(width, height));
		this->shape.setFillColor(idle_color);
		this->shape.setOutlineThickness(2.f);
		this->shape.setOutlineColor(outline_idle_color);

		this->font = font;
		this->text.setFont(*this->font);
		this->text.setString(text);
		this->text.setFillColor(text_idle_color);
		this->text.setCharacterSize(character_size);
		//std::cout << this->text.getGlobalBounds().width << "\n";
		updateTextCenter();

		this->textIdleColor = text_idle_color;
		this->textHoverColor = text_hover_color;
		this->textActiveColor = text_active_color;

		this->idleColor = idle_color;
		this->hoverColor = hover_color;
		this->activeColor = active_color;

		this->outlineIdleColor = outline_idle_color;
		this->outlineHoverColor = outline_hover_color;
		this->outlineActiveColor = outline_active_color;
	}

	Button::~Button()
	{

	}

	//Accessors
	const bool Button::isPressed() const
	{
		if (this->buttonState == BTN_ACTIVE)
			return true;

		return false;
	}

	const std::string Button::getText() const
	{
		return this->text.getString();
	}

	const short unsigned& Button::getId() const
	{
		return this->id;
	}

	//Modifiers
	void Button::setText(const std::string text)
	{
		this->text.setString(text);
	}

	void Button::setId(const short unsigned id)
	{
		this->id = id;
	}

	void Button::updatePosition(float x, float y, float width, float height, unsigned char_size)
	{
		this->shape.setPosition(sf::Vector2f(x, y));
		this->shape.setSize(sf::Vector2f(width, height));
		this->shape.setOutlineThickness(1.f);

		this->text.setCharacterSize(char_size);
		//std::cout << this->text.getGlobalBounds().width << "\n";
		updateTextCenter();
	}

	void Button::updateTextCenter()
	{
		this->text.setPosition(
			this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
			this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height
		);
	}


	//Functions
	void Button::update(const sf::Vector2i& mousePosWindow)
	{
		/*Update the booleans for hover and pressed*/

		//Idle
		this->buttonState = BTN_IDLE;

		//Hover
		if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			this->buttonState = BTN_HOVER;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->buttonState = BTN_ACTIVE;
			}
		}


		switch (this->buttonState)
		{
		case BTN_IDLE:
			this->shape.setFillColor(this->idleColor);
			this->text.setFillColor(this->textIdleColor);
			this->shape.setOutlineColor(this->outlineIdleColor);
			break;

		case BTN_HOVER:
			this->shape.setFillColor(this->hoverColor);
			this->text.setFillColor(this->textHoverColor);
			this->shape.setOutlineColor(this->outlineHoverColor);
			break;

		case BTN_ACTIVE:
			this->shape.setFillColor(this->activeColor);
			this->text.setFillColor(this->textActiveColor);
			this->shape.setOutlineColor(this->outlineActiveColor);
			break;

		default:
			this->shape.setFillColor(sf::Color::Red);
			this->text.setFillColor(sf::Color::Blue);
			this->shape.setOutlineColor(sf::Color::Green);
			break;
		}
	}

	void Button::render(sf::RenderTarget& target)
	{
		target.draw(this->shape);
		target.draw(this->text);
	}

	DropDownList::DropDownList(float x, float y, float width, float height,
		sf::Font& font, std::string list[],
		unsigned nrOfElements, unsigned default_index)
		: font(font), showList(false), keytimeMax(1.f), keytime(0.f)
	{
		//unsigned nrOfElements = sizeof(list) / sizeof(std::string);

		this->activeElement = new Button(
			x, y, width, height,
			&this->font, list[default_index], 30,
			sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
			sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(60, 60, 60, 50)
		);

		for (unsigned i = 0; i < nrOfElements; i++)
		{
			this->list.push_back(
				new Button(
					x, y + ((i + 1) * height), width, height,
					&this->font, list[i], 20,
					sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
					sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
					sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
					i
				)
			);
		}
	}

	DropDownList::~DropDownList()
	{
		delete this->activeElement;
		for (size_t i = 0; i < this->list.size(); i++)
		{
			delete this->list[i];
		}
	}

	//Accessors
	const bool DropDownList::getKeytime()
	{
		if (this->keytime >= this->keytimeMax)
		{
			this->keytime = 0.f;
			return true;
		}

		return false;
	}

	const unsigned short& DropDownList::getActiveElementId() const
	{
		return this->activeElement->getId();
	}

	//Functions
	void DropDownList::updateKeytime(const float& dt)
	{
		if (this->keytime < this->keytimeMax)
			this->keytime += 10.f * dt;
	}

	void DropDownList::update(const sf::Vector2i& mousePosWindow, const float& dt)
	{
		this->updateKeytime(dt);

		this->activeElement->update(mousePosWindow);
		this->activeElement->updateTextCenter();

		//Show and hide the list
		if (this->activeElement->isPressed() && this->getKeytime())
		{
			if (this->showList)
				this->showList = false;
			else
				this->showList = true;
		}
		if (this->showList)
		{
			for (auto& i : this->list)
			{
				i->update(mousePosWindow);

				if (i->isPressed() && this->getKeytime())
				{
					this->showList = false;
					this->activeElement->setText(i->getText());
					this->activeElement->setId(i->getId());
				}
			}
		}

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->activeElement->isPressed())
			this->showList = false;

	}

	void DropDownList::render(sf::RenderTarget& target)
	{
		this->activeElement->render(target);

		if (this->showList)
		{
			for (auto& i : this->list)
			{
				i->render(target);
			}
		}
	}

	ProgressBar::ProgressBar(float _x, float _y, float _width, float _height,
		sf::Color inner_color, unsigned character_size,
		sf::VideoMode& vm, sf::Font* font)
	{
		float width = p2pX(_width, vm);
		float height = p2pY(_height, vm);
		float x = p2pX(_x, vm);
		float y = p2pY(_y, vm);

		this->maxWidth = width;

		this->back.setSize(sf::Vector2f(width, height));
		this->back.setFillColor(sf::Color(50, 50, 50, 200));
		this->back.setPosition(x, y);

		this->inner.setSize(sf::Vector2f(width, height));
		this->inner.setFillColor(inner_color);
		this->inner.setPosition(this->back.getPosition());

		if (font)
		{
			this->text.setFont(*font);
			this->text.setCharacterSize(calcCharSize(vm, character_size));
			this->text.setPosition(
				this->inner.getPosition().x + p2pX(0.53f, vm),
				this->inner.getPosition().y + p2pY(0.2f, vm)
			);
		}
	}

	ProgressBar::~ProgressBar()
	{

	}

	//Functions
	void ProgressBar::update(const int current_value, const int max_value)
	{
		float percent = static_cast<float>(current_value) / static_cast<float>(max_value);

		this->inner.setSize(
			sf::Vector2f(
				static_cast<float>(back.getSize().x * percent),
				this->inner.getSize().y
			)
		);

		this->barString = std::to_string(current_value) + " / " + std::to_string(max_value);
		this->text.setString(this->barString);
	}

	void ProgressBar::render(sf::RenderTarget& target)
	{
		target.draw(this->back);
		target.draw(this->inner);
		target.draw(this->text);
	}

	RadialButton::RadialButton(float x, float y, float width, float height, sf::Font* font, std::string text, unsigned character_size, bool default_state, sf::Texture& unchecked, sf::Texture& checked)
		: _font(font), tof(default_state), _checked(checked), _unchecked(unchecked)
	{
		_shape.setPosition(x, y);
		_shape.setSize(sf::Vector2f(width, height));

		_ButtonName.setPosition(_shape.getPosition());
		_ButtonName.setFont(*_font);
		_ButtonName.setString(text);
		_ButtonName.setCharacterSize(character_size);

		_button.setPosition(_ButtonName.getPosition().x + _ButtonName.getGlobalBounds().width,
			_ButtonName.getPosition().y);

		if (tof == true)
			_button.setTexture(checked);
		else
			_button.setTexture(unchecked);

	}
	RadialButton::~RadialButton()
	{
	}
	const bool RadialButton::isOn() const
	{
		return tof;
	}
	const std::string RadialButton::getText() const
	{
		return std::string();
	}
	const short unsigned RadialButton::getId() const
	{
		return 0;
	}
	void RadialButton::setText(const std::string text)
	{
		_ButtonName.setString(text);
	}
	void RadialButton::setId(const short unsigned id)
	{
		(void)0;
	}
	void _Swag::_gui::RadialButton::updatePosition(float x, float y, float width, float height, unsigned char_size)
	{
	}
	void RadialButton::updateTextCenter()
	{
	}
	void RadialButton::update(const sf::Vector2i& mousePosWindow)
	{
		if (this->_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			//Pressed
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->tof = !tof;
			}
		}

		if (tof == true)
			_button.setTexture(_checked);
		else
			_button.setTexture(_unchecked);
	}

	void RadialButton::render(sf::RenderTarget& target)
	{
		target.draw(this->_ButtonName);
		target.draw(this->_button);
	}
}