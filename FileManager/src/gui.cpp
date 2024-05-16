#include "gui.hpp"

#define NOMINMAX
#include "Windows.h"
#include <string>
#include <locale>
#include <codecvt>

#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace FileDialogs {

	std::string OpenFile(const char* filter, sf::RenderWindow& window)
	{
		OPENFILENAME ofn;
		CHAR szFile[MAX_PATH] = { 0 }; // Buffer to hold single file name

		// Initialize OPENFILENAME structure
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = (LPWSTR)szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.hwndOwner = window.getSystemHandle();
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("All Files\0*.*\0"); // Filter for all files
		ofn.nFilterIndex = 1; // Default filter index
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Options (no multi-select)

		// Open file dialog
		if (GetOpenFileName(&ofn) == TRUE) {
			// User selected a file
			std::wstring filePath = ofn.lpstrFile;
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			std::string narrowStr = converter.to_bytes(filePath);
			// Do something with the selected file path
			return narrowStr;
		}

		return std::string();
	}

	std::string SaveFile(const char* filter)
	{
		return std::string();
	}

}

Gui::Gui(sf::RenderWindow* window)
	:window(window)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsClassic();
}

void Gui::Run()
{
	ImGui::SetNextWindowPos(ImVec2(50, 50));
	ImGui::SetNextWindowSize(ImVec2(200, 60));

	ImGui::Begin("Option", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	
	static const char* items[] = { "Encrypt", "Decrypt" };
	static int currentItem = 0;
	if (ImGui::Combo("Mode", &currentItem, items, IM_ARRAYSIZE(items))) {
		if (currentItem == 0)
		{
			memset(enbufferorgname, 0, 255);
			memset(enbufferrename, 0, 255);
			EncryptFileName = "";
			mode = 0;
			enmode = I2D;
		}
		else if (currentItem == 1)
			mode = 1;
	}
	ImGui::End();

	if (mode == 0)
		Encrypt();
	else if (mode == 1)
		Decrypt();
}

void Gui::Encrypt()
{
	ImGui::SetNextWindowSize(ImVec2(600, 80));
	ImGui::SetNextWindowPos(ImVec2(50, 150));
	ImGui::Begin("OriginalFileName", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | 
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Original File Path");
	ImGui::InputText("File Path", enbufferorgname, 255);
	if(ImGui::Button("OpenFile"))
	{
		EncryptFileName = FileDialogs::OpenFile("", *this->window);
		for (int i = 0; i < EncryptFileName.size(); i++)
		{
			enbufferorgname[i] = EncryptFileName[i];
		}
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(600, 80));
	ImGui::SetNextWindowPos(ImVec2(50, 300));
	ImGui::Begin("EncryptFileName", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Encrypted File Name");
	ImGui::InputText("File Name", enbufferrename, 255);
	ImGui::Text(" with any Extentions(filename.data)");
	ImGui::End();


	ImGui::SetNextWindowSize(ImVec2(600, 40));
	ImGui::SetNextWindowPos(ImVec2(50, 400));
	ImGui::Begin("Mode to Encrypt", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	
	static const char* modeitems[] = { "Image to Data" , "Audio to Data" };

	if (ImGui::Combo("Convert From", &encurrentItem, modeitems, IM_ARRAYSIZE(modeitems)))
	{
		switch (encurrentItem)
		{
		case 0:
			enmode = I2D;
			break;
		case 1:
			enmode = A2D;
			break;
		default:
			break;
		}
	}

	ImGui::End();


	ImGui::SetNextWindowSize(ImVec2(600, 65));
	ImGui::SetNextWindowPos(ImVec2(50, 500));
	ImGui::Begin("Encrypt_Button", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	if (ImGui::Button("Encrypt", ImVec2(600, 50)))
	{
		if (enbufferorgname != NULL && enbufferrename != NULL)
		{
			DoEncryptThing();
		}
	}
	ImGui::End();
}

#include <iostream>

void Gui::Decrypt()
{
	ImGui::SetNextWindowSize(ImVec2(600, 80));
	ImGui::SetNextWindowPos(ImVec2(50, 150));
	ImGui::Begin("EncryptedFileName", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Encrypted File Path");
	ImGui::InputText("File Path", debufferorgname, 255);
	if (ImGui::Button("OpenFile"))
	{
		DecryptFileName = FileDialogs::OpenFile("", *this->window);
		std::cout << DecryptFileName;
		for (int i = 0; i < DecryptFileName.size(); i++)
		{
			debufferorgname[i] = EncryptFileName[i];
		}
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(600, 80));
	ImGui::SetNextWindowPos(ImVec2(50, 300));
	ImGui::Begin("DecryptFileName", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Decrypted File Name");
	ImGui::InputText("File Name", debufferrename, 255);
	ImGui::Text(" with specified file type extension like (filename.png)(filename.wav)");
	ImGui::End();


	ImGui::SetNextWindowSize(ImVec2(600, 40));
	ImGui::SetNextWindowPos(ImVec2(50, 400));
	ImGui::Begin("Mode to Encrypt", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	static const char* modeitems[] = { "Data to Image" , "Data to Audio" };

	if (ImGui::Combo("Convert From", &decurrentItem, modeitems, IM_ARRAYSIZE(modeitems)))
	{
		switch (decurrentItem)
		{
		case 0:
			demode = D2I;
			break;
		case 1:
			demode = D2A;
			break;
		default:
			break;
		}
	}

	ImGui::End();


	ImGui::SetNextWindowSize(ImVec2(600, 65));
	ImGui::SetNextWindowPos(ImVec2(50, 500));
	ImGui::Begin("Decrypt_Button", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	if (ImGui::Button("Decrypt", ImVec2(600, 50)))
	{
		if (debufferorgname != NULL && debufferrename != NULL)
		{
			DoDecryptThing();
		}
	}
	ImGui::End();
}

void Gui::DoEncryptThing()
{
	switch (enmode)
	{
	case I2D:{
		sf::Image img;
		if (!img.loadFromFile(std::string(this->enbufferorgname)))
		{
			MessageBoxA(this->window->getSystemHandle(),
				"File Not Exist (or) File Type is Not Supported as Image File Type!!",
				"Error", MB_ICONERROR);
			MessageBeep(MB_ICONERROR);
			return;
		}

		std::fstream data_file(enbufferrename, std::ios::out | std::ios::binary);

		if (data_file.is_open())
		{
			const sf::Vector2u imageSize = img.getSize();
			data_file.write(reinterpret_cast<const char*>(&imageSize.x), sizeof(imageSize.x));
			data_file.write(reinterpret_cast<const char*>(&imageSize.y), sizeof(imageSize.y));
			data_file.write(reinterpret_cast<const char*>(img.getPixelsPtr()), static_cast<std::streamsize>(imageSize.x) * imageSize.y * 4);
			data_file.close();
		}
		else
		{
			MessageBoxA(this->window->getSystemHandle(), "Failed To Encrypt.", "Information", MB_ICONERROR);
			MessageBeep(MB_ICONERROR);
		}

		MessageBoxA(this->window->getSystemHandle(), "Successfully Encrypted!!", "Info", MB_ICONINFORMATION | MB_OK);
		MessageBeep(MB_OK);

	}break;
	case A2D: {
		sf::SoundBuffer soundBuffer;
		if (!soundBuffer.loadFromFile(enbufferorgname))
			return;

		const sf::Int16* samples = soundBuffer.getSamples();
		const sf::Uint64 sampleCount = soundBuffer.getSampleCount();
		const unsigned int channelCount = soundBuffer.getChannelCount();
		const unsigned int sampleRate = soundBuffer.getSampleRate();

		std::fstream data_file(enbufferrename, std::ios::binary | std::ios::out);
		if (data_file.is_open())
		{
			data_file.write(reinterpret_cast<const char*>(&sampleRate), sizeof(sampleRate));
			data_file.write(reinterpret_cast<const char*>(&channelCount), sizeof(channelCount));
			data_file.write(reinterpret_cast<const char*>(samples), sampleCount * sizeof(sf::Int16));

			data_file.close();
		}

	}break;
	default:
		break;
	}
}

void Gui::DoDecryptThing()
{
	switch (demode)
	{
	case D2I: {
		std::fstream data_file(debufferorgname, std::ios::in | std::ios::binary);
		if (data_file.is_open())
		{
			sf::Vector2u imageSize;
			data_file.read(reinterpret_cast<char*>(&imageSize.x), sizeof(imageSize.x));
			data_file.read(reinterpret_cast<char*>(&imageSize.y), sizeof(imageSize.y));

			std::vector<sf::Uint8> pixelData(imageSize.x * imageSize.y * 4);

			data_file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

			sf::Image image;
			image.create(imageSize.x, imageSize.y, pixelData.data());
			image.saveToFile(debufferrename);

			data_file.close();
		}
		else
		{
			MessageBoxA(this->window->getSystemHandle(), "Failed To Decrypt..", "Information", MB_ICONERROR);
			MessageBeep(MB_ICONERROR);
		}

		MessageBoxA(this->window->getSystemHandle(), "Successfully Decrypted!!", "Info", MB_ICONINFORMATION | MB_OK);
		MessageBeep(MB_OK);

	}break;
	case D2A: {
		std::fstream data_file(debufferorgname, std::ios::in | std::ios::binary);
		if (data_file.is_open())
		{
			unsigned int sampleRate;
			unsigned int channelCount;
			data_file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
			data_file.read(reinterpret_cast<char*>(&channelCount), sizeof(channelCount));

			data_file.seekg(0, std::ios::end);
			std::streampos length = data_file.tellg() - (std::streampos)sizeof(sampleRate) - sizeof(channelCount);
			data_file.seekg(sizeof(sampleRate) + sizeof(channelCount));

			std::vector<sf::Int16> audioData(length / sizeof(sf::Int16));
			data_file.read(reinterpret_cast<char*>(audioData.data()), length);

			sf::SoundBuffer soundBuffer;
			soundBuffer.loadFromSamples(audioData.data(), audioData.size(), channelCount, sampleRate);

			soundBuffer.saveToFile(debufferrename);

			data_file.close();
		}

	}break;
	default:
		break;
	}
}
