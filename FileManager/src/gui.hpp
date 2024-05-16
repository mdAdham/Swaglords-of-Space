#include <math.h>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <string>

enum ModeToEncrypt { I2D = 0, A2D };
enum ModeToDecrypt { D2I = 0, D2A };

class Gui
{
public:
	Gui(sf::RenderWindow* window);
	void Run();
private:
	sf::RenderWindow* window = nullptr;
	int mode = 0;

	void Encrypt();
	void Decrypt();

	void DoEncryptThing();
	void DoDecryptThing();
	
	//Encrypt
	char enbufferorgname[255]{};
	char enbufferrename[255]{};
	std::string EncryptFileName = "";
	int encurrentItem = 0;
	ModeToEncrypt enmode = I2D;

	//Decrypt
	char debufferorgname[255]{};
	char debufferrename[255]{};
	std::string DecryptFileName = "";
	int decurrentItem = 0;
	ModeToDecrypt demode = D2I;
};

namespace FileDialogs {

	std::string OpenFile(const char* filter, sf::RenderWindow& window);
	std::string SaveFile(const char* filter);
}