#pragma once

#include <stdint.h>

constexpr uint32_t WINDOW_WIDHT = 1920;
constexpr uint32_t WINDOW_HEIGHT = 1080;

constexpr uint32_t WINDOW_MIN_WIDTH = WINDOW_WIDHT/4;
constexpr uint32_t WINDOW_MIN_HEIGHT = WINDOW_HEIGHT/4;

#define GAME_WINDOW_FULLSCREEN

constexpr float FRAMERATELIMIT = 120.f;
constexpr float ENEMY_MOVEMENT_INERTIA = 0.2f;

constexpr float AIR_DRAG = 0.995f;

const std::string Resource_root_folder = "Swag_Space_2.0_Resource/";

const std::string SHIP_TEXTURE = Resource_root_folder + "tex/Ship.dat";
const std::string BULLET_TEXTURE = Resource_root_folder + "tex/Bullet.dat";
const std::string BACKGROUND_TEXTURE = Resource_root_folder + "tex/Background.png";
const std::string FONT_ARIAL = Resource_root_folder + "fonts/arial.ttf";

const std::string Radial_Button_UC = Resource_root_folder + "tex/RB_UC.dat";
const std::string Radial_Button_C = Resource_root_folder + "tex/RB_C.dat";

const std::string Core_Shader_Vertex = Resource_root_folder + "shr/core_sh.vertex.glsl";
const std::string Core_Shader_Fragment = Resource_root_folder + "shr/core_sh.fragment.glsl";

const std::string Test_PNG = Resource_root_folder + "tex/background.jpg";

const std::string Colliders_Mask_Map_Folder = Resource_root_folder + "catch/Colliders/";

const std::string GREY_SCALE_SHADER_V = Resource_root_folder + "shr/grayscale.vertex.glsl";
const std::string GREY_SCALE_SHADER_F = Resource_root_folder + "shr/grayscale.fragment.glsl";

const std::string GLOW_SHADER_V = Resource_root_folder + "shr/glow.vertex.glsl";
const std::string GLOW_SHADER_F = Resource_root_folder + "shr/glow.fragment.glsl";

const std::string GAME_RENDER_TEXTURE_SHADER_F = Resource_root_folder + "shr/game_renderTexture_F.glsl";

const std::string GROUP_SOUND_BULLET = "shooting";

const std::string SBUFFER_B_S1 = "sb_bs1";
const std::string SBUFFER_B_S2 = "sb_bs2";
const std::string SBUFFER_B_S3 = "sb_bs3";
const std::string SBUFFER_Ship = "sb_ship";
const std::string SBUFFER_Rock = "sb_rock";

const std::string S_B_S1 = "s_bs1";
const std::string S_B_S2 = "s_bs2";
const std::string S_B_S3 = "s_bs3";
const std::string S_Ship = "s_ship";
const std::string S_Rock = "s_rock";

const std::string SOUND_BULLET_SHOOTING1 = Resource_root_folder + "sounds/SHOOT011.wav";
const std::string SOUND_BULLET_SHOOTING2 = Resource_root_folder + "sounds/SHOOT012.wav";
const std::string SOUND_BULLET_SHOOTING3 = Resource_root_folder + "sounds/SHOOT013.wav";
const std::string SOUND_ROCKET_LAUNCH = Resource_root_folder + "sounds/rocket_launch.wav";
const std::string SOUND_ROCK_COLLISION = Resource_root_folder + "sounds/rock_breaking.wav";