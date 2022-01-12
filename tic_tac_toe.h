#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <json.hpp>

#include <fstream> 
#include "game_manager.h"
#pragma once

using json = nlohmann::json;

class TicTacToe
{
private:
	static TicTacToe* instance_;

	GameManager* gm_;
	std::string window_title_;
	int window_size_w_;
	int window_size_h_;

	std::string icon_path_;
	int icon_size_;

	bool player_is_first_; 
	float init_player_agg_; 
	float ai_agg_; 
	float agg_damp_; 
	int vision_range_; 
	int attack_range_; 
	int top_n_; 
	int max_depth_; 
	float attack_weight_[6][3]; 

	int tile_size_;
	int field_size_;
	bool show_hints_; 

	std::string font_file_name_;
	float fonst_size_;

	bool show_victory_window; 
	TurnType winner; 

	Cell best_move_; 
	Cell center_;

	sf::Sprite sprite_cell_;
	sf::Sprite sprite_cross_;
	sf::Sprite sprite_zero_;
	sf::Sprite sprite_hint_;
	sf::Sprite sprite_up_;
	sf::Sprite sprite_down_;
	sf::Sprite sprite_left_;
	sf::Sprite sprite_right_;

	TicTacToe(std::string config_path);

	void SettingsWindow_();
	void FieldWindow_();
	void VictoryWindow_(int x, int y);
public:
	static TicTacToe* GetInstance(std::string config_path);
	void Restart();
	void Start();
};
