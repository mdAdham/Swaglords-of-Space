#pragma once

#include <stdint.h>

constexpr uint32_t WINDOW_WIDHT = 1920;
constexpr uint32_t WINDOW_HEIGHT = 1080;

const std::string Resource_root_folder = "Swag_Space_2.0_Resource/";

const std::string SHIP_TEXTURE = Resource_root_folder + "tex/Ship.dat";
const std::string BULLET_TEXTURE = Resource_root_folder + "tex/Bullet.dat";
//const std::string BACKGROUND_TEXTURE = Resource_root_folder + "tex/"

const std::string Radial_Button_UC = Resource_root_folder + "tex/RB_UC.dat";
const std::string Radial_Button_C = Resource_root_folder + "tex/RB_C.dat";

const std::string Core_Shader_Vertex = Resource_root_folder + "shr/core_sh.vertex.glsl";
const std::string Core_Shader_Fragment = Resource_root_folder + "shr/core_sh.fragment.glsl";

const std::string Test_PNG = Resource_root_folder + "tex/background.jpg";

const std::string Colliders_Mask_Map_Folder = Resource_root_folder + "catch/Colliders/";