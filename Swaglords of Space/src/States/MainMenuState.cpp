#include "MainMenuState.h"
#include <sstream>

#include "Core/DEFFINITIONS.h"

#include <iostream>

#include "GameState.h"
#include "OptionState.h"
#include "LevelState.h"
#include "NewGame.h"

namespace Swag
{
	MainMenuState::MainMenuState(GameDataRef data)
		: _data(data)
	{
		
	}

	void MainMenuState::Init()
	{
		this->optionButtonStatus = ButtonStatus::idle;
		this->quitButtonStatus = ButtonStatus::idle;
		this->playButtonStatus = ButtonStatus::idle;
		this->newGameButtonStatus = ButtonStatus::idle;

		asserts.LoadMusic("BackgroundMusic", MAIN_MENU_BACKGROUND_SOUND_FILEPATH);
		asserts.GetMusic("BackgroundMusic").setLoop(true);

		asserts.LoadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH, false);
		asserts.LoadTexture("Play Button", PLAY_BUTTON, true);
		asserts.LoadFont("Alger Font", ALGER_FONT);
		asserts.LoadFont("Party Font", PARTY_FONT);
		

		this->_background.setTexture(asserts.GetTexture("Background"));
		this->_playButton.setTexture(asserts.GetTexture("Play Button"));

		this->_quitButton.setFont(asserts.GetFont("Alger Font"));
		this->_optionButton.setFont(asserts.GetFont("Alger Font"));
		this->_newGameButton.setFont(asserts.GetFont("Party Font"));

		this->_optionButton.setString("Options");
		this->_optionButton.setCharacterSize(50);
		this->_quitButton.setString("QUIT");
		this->_quitButton.setCharacterSize(50);
		this->_newGameButton.setString("New Game");
		this->_newGameButton.setCharacterSize(30);
		

		this->_playButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 2 - 70) - (this->_playButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2) - this->_playButton.getGlobalBounds().height / 2);
		//this->_quitButton.setPosition(this->_data->window.getSize().x - this->_quitButton.getGlobalBounds().height - 90, this->_data->window.getSize().y - this->_quitButton.getGlobalBounds().width);
		this->_quitButton.setPosition(((static_cast<float>(SCREEN_HEIGHT) / 2) - (this->_quitButton.getGlobalBounds().width / 2))+ 50, ((static_cast<float>(SCREEN_WIDTH) / 2 - 10) - (this->_quitButton.getGlobalBounds().height / 2)) + 200);
		this->_optionButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 2 + 100) - (this->_optionButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2 - 10) - this->_optionButton.getGlobalBounds().height / 2);
		this->_newGameButton.setPosition((static_cast<float>(SCREEN_HEIGHT) / 2 + 100) - (this->_newGameButton.getGlobalBounds().width / 2), (static_cast<float>(SCREEN_WIDTH) / 2 - 60) - this->_newGameButton.getGlobalBounds().height / 2);

		if (!this->m_coreshader.loadFromFile(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER))
		{
			SWAG_ERROR("MainMenu: Failed to load Core Shader");
		}

		sf::VertexBuffer buffer;
		buffer.setUsage(sf::VertexBuffer::Static);
		buffer.create(4);
		buffer.setPrimitiveType(sf::Triangles);
		
		m_quadVertexArray.setPrimitiveType(sf::Quads);
		m_quadVertexArray.resize(4);
		m_quadVertexArray[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black, sf::Vector2f(0.0f, 0.0f));
		m_quadVertexArray[1] = sf::Vertex(sf::Vector2f(SCREEN_HEIGHT, 0), sf::Color::Black, sf::Vector2f(1.0f, 0.0f));
		m_quadVertexArray[2] = sf::Vertex(sf::Vector2f(SCREEN_HEIGHT, SCREEN_WIDTH), sf::Color::Black, sf::Vector2f(1.0f, 1.0f));
		m_quadVertexArray[3] = sf::Vertex(sf::Vector2f(0, SCREEN_WIDTH), sf::Color::Black, sf::Vector2f(0.0f, 1.0f));
		std::string vertexShader = R"(

			out vec4 v_color;			
			in vec2 texCoord;
			void main()
			{
				// Transform the vertex position
				gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
				v_color = gl_Color;
			}
		
		)";
		std::string fragmentShader = R"(
			#version 330
			in vec4 v_color;
			out vec4 FragColor;
			uniform vec4 u_color;
			
		
			void main()
			{
				vec4 result = u_color * v_color;
				//if(result.b < 0.5)
				//	result.a = 0;
				gl_FragColor = result;
			}
		)";
		
		m_quadShader.loadFromMemory(vertexShader, fragmentShader);
	}

	void MainMenuState::HandleInput()
	{
		sf::Event event{};

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				asserts.GetMusic("BackgroundMusic").stop();
				this->_data->window.close();
			}

			if (this->_data->input.isSpriteClicked(this->_playButton, sf::Mouse::Left, this->_data->window))
			{
				asserts.GetMusic("BackgroundMusic").stop();
				this->_data->machine.AddState(StateRef(new GameState(_data)), true);
			}

			if (this->_data->input.isTextClicked(this->_optionButton, sf::Mouse::Left, this->_data->window))
				this->_data->machine.AddState(StateRef(new OptionState(this->_data)), false);

			if (this->_data->input.isTextClicked(this->_quitButton, sf::Mouse::Left, this->_data->window))
				this->_data->window.close();

			if (this->_data->input.isTextClicked(this->_newGameButton, sf::Mouse::Left, this->_data->window))
				this->_data->machine.AddState(StateRef(new NewGameState(this->_data)), true);
		}

	}

	void MainMenuState::Update(float dt)
	{
		if (this->_optionButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->optionButtonStatus = ButtonStatus::hover;
		else
			this->optionButtonStatus = ButtonStatus::idle;

		if (this->_quitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->quitButtonStatus = ButtonStatus::hover;
		else
			this->quitButtonStatus = ButtonStatus::idle;

		if (this->_playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->playButtonStatus = ButtonStatus::hover;
		else
			this->playButtonStatus = ButtonStatus::idle;

		if (this->_newGameButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(this->_data->input.GetMousePosition(this->_data->window))))
			this->newGameButtonStatus = ButtonStatus::hover;
		else
			this->newGameButtonStatus = ButtonStatus::idle;


		if (this->optionButtonStatus == ButtonStatus::hover)
			this->_optionButton.setFillColor(sf::Color::Green);

		if (this->quitButtonStatus == ButtonStatus::hover)
			this->_quitButton.setFillColor(sf::Color::Green);
		
		if (this->playButtonStatus == ButtonStatus::hover)
			this->_playButton.setColor(sf::Color::Green);

		if (this->newGameButtonStatus == ButtonStatus::hover)
			this->_newGameButton.setColor(sf::Color::Red);


		if (this->optionButtonStatus == ButtonStatus::idle)
			this->_optionButton.setFillColor(sf::Color::White);

		if (this->quitButtonStatus == ButtonStatus::idle)
			this->_quitButton.setFillColor(sf::Color::White);

		if (this->playButtonStatus == ButtonStatus::idle)
			this->_playButton.setColor(sf::Color::White);

		if (this->newGameButtonStatus == ButtonStatus::idle)
			this->_newGameButton.setColor(sf::Color::White);

		if (this->music_count == 0)
		{
			asserts.GetMusic("BackgroundMusic").play();
			this->music_count++;
		}

	}

	void MainMenuState::Draw(float dt)
	{
		this->_data->window.clear();

		sf::Transform quadTrans;
		quadTrans.translate(0, 0);

		m_coreshader.setUniform("hasTexture", false);
		m_coreshader.setUniform("lightPos", sf::Glsl::Vec2((_playButton.getGlobalBounds().left + _playButton.getGlobalBounds().width/2.f), (_playButton.getGlobalBounds().top + _playButton.getGlobalBounds().height / 2.f)));
		m_coreshader.setUniform("texture", &asserts.GetTexture("Play Button"));

		m_quadShader.setUniform("u_color", sf::Glsl::Vec4(1, 1, 1, 0.1));

		sf::RenderStates stats;
		stats.transform = quadTrans;
		stats.shader = &m_quadShader;
		//stats.texture = &asserts.GetTexture("Play Button");
		//m_quadShader.setUniform("tex", asserts.GetTexture("Play Button"));

		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->m_quadVertexArray, stats);
		this->_data->window.draw(this->_playButton);
		this->_data->window.draw(this->_quitButton);
		this->_data->window.draw(this->_optionButton);
		this->_data->window.draw(this->_newGameButton);

		this->_data->window.display();
	}

	void MainMenuState::DontDestroyOnLoad()
	{
		
	}
}