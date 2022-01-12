#include "tic_tac_toe.h"

TicTacToe* TicTacToe::instance_ = 0;

TicTacToe::TicTacToe(std::string config_path)
{
	//Read config
	std::ifstream fs("config.json");
	json config;
	fs >> config;
	fs.close();

	//Init fields
	window_title_ = config["window"]["title"].get<std::string>();
	window_size_w_ = config["window"]["size"]["w"].get<int>();
	window_size_h_ = config["window"]["size"]["h"].get<int>();

	icon_path_ = config["window"]["icon"].get<std::string>();
	icon_size_ = config["window"]["icon_size"].get<int>();

	player_is_first_ = config["game_manager"]["player_is_first"].get<bool>();
	init_player_agg_ = config["game_manager"]["init_player_agg"].get<float>();
	ai_agg_ = config["game_manager"]["ai_agg"].get<float>();
	agg_damp_ = config["game_manager"]["agg_damp"].get<float>();
	vision_range_ = config["game_manager"]["vision_range"].get<int>();
	attack_range_ = config["game_manager"]["attack_range"].get<int>();
	top_n_ = config["game_manager"]["top_n"].get<int>();
	max_depth_ = config["game_manager"]["max_depth"].get<int>();
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++)
			attack_weight_[i][j] = config["game_manager"]["attack_weight"][i][j].get<float>();

	tile_size_ = config["settings"]["tile_size"].get<int>();
	field_size_ = config["settings"]["field_size"].get<int>();
	show_hints_ = config["settings"]["show_hints"].get<bool>();;

	font_file_name_ = config["font"]["file_name"].get<std::string>();
	fonst_size_ = config["font"]["size_pixels"].get<float>();

	show_victory_window = false;
	winner = TurnType::Player;

	gm_ = new GameManager(player_is_first_, init_player_agg_, ai_agg_,
		agg_damp_, vision_range_, attack_range_, top_n_, max_depth_, attack_weight_);

	best_move_ = gm_->GetBestMove(TurnType::Player);

	center_.x = -field_size_ / 2;
	center_.y = -field_size_ / 2;

	//Create SFML window
	sf::RenderWindow window(sf::VideoMode(window_size_w_, window_size_h_), window_title_, sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);
	window.setTitle(window_title_.c_str());

	//Set icon
	sf::Image icon;
	icon.loadFromFile(icon_path_);
	window.setIcon(icon_size_, icon_size_, icon.getPixelsPtr());

	//Set ImGui font
	ImGuiIO& IO = ImGui::GetIO();
	IO.Fonts->Clear();
	IO.Fonts->AddFontFromFileTTF(font_file_name_.c_str(), fonst_size_);
	ImGui::SFML::UpdateFontTexture();

	
}

void TicTacToe::SettingsWindow_()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(window_size_w_, -1));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11f, 0.11f, 0.11f, 0.97f));
	if (ImGui::Begin("Settings", nullptr, window_flags))
	{
		if (ImGui::Button("Restart", ImVec2(-1, 0.0f))) {
			Restart();
		}
		ImGui::Checkbox("Show hints", &show_hints_);
		ImGui::Checkbox("Player goes first", &player_is_first_);
		if (ImGui::CollapsingHeader("Advanced"))
		{
			ImGui::SliderFloat("AI aggressivness", &ai_agg_, 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("Aggressivness damp", &agg_damp_, 0.0f, 1.0f, "%.2f");
			ImGui::SliderInt("Vision range", &vision_range_, 1, 5);
			ImGui::SliderInt("Attack range", &attack_range_, 5, 10);
			ImGui::SliderInt("Top n cells", &top_n_, 1, 8);
			ImGui::SliderInt("Max recursion depth", &max_depth_, 0, 6);
		}
		ImGui::End();
	}
}

void TicTacToe::FieldWindow_()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowPos(ImVec2(0, 25));
	ImGui::SetNextWindowSize(ImVec2(window_size_w_, window_size_h_ - 25));
	if (ImGui::Begin("TicTacToe", nullptr, window_flags))
	{
		if (ImGui::BeginTable("Field", field_size_ + 2))
		{
			ImGui::TableNextRow();
			for (int i = 0; i < field_size_ / 2 + 2; i++)
				ImGui::TableNextColumn();
			ImGui::PushID(0);
			if (ImGui::ImageButton(sprite_up_, 0))
				center_.x--;
			ImGui::PopID();
			for (int i = 0; i < field_size_; i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (i == field_size_ / 2)
				{
					ImGui::PushID(1);
					if (ImGui::ImageButton(sprite_left_, 0))
						center_.y--;
					ImGui::PopID();
				}
				for (int j = 0; j < field_size_; j++)
				{
					int x = center_.x + i;
					int y = center_.y + j;
					ImGui::TableNextColumn();
					ImGui::PushID(i * field_size_ + j + 4);
					sf::Sprite sprite;
					if (show_hints_ && best_move_.x == x && best_move_.y == y)
						sprite = sprite_hint_;
					else if (gm_->GetCell(x, y) == CellType::O)
						sprite = sprite_zero_;
					else if (gm_->GetCell(x, y) == CellType::X)
						sprite = sprite_cross_;
					else
						sprite = sprite_cell_;

					if (ImGui::ImageButton(sprite, 2) && !show_victory_window)
					{
						if (gm_->GetCell(x, y) == CellType::None)
						{
							if (gm_->MakeMovePlayer(x, y))
							{
								show_victory_window = true;
								winner = TurnType::Player;
							}
							else if (gm_->MakeMoveAI())
							{
								show_victory_window = true;
								winner = TurnType::AI;
							}
							if (show_hints_)
								best_move_ = gm_->GetBestMove(TurnType::Player);
						}
					}
					ImGui::PopID();
					ImGui::SameLine();

				}
				if (i == field_size_ / 2)
				{
					ImGui::TableNextColumn();
					ImGui::PushID(2);
					if (ImGui::ImageButton(sprite_right_, 0))
						center_.y++;
					ImGui::PopID();
				}
			}
			ImGui::TableNextRow();
			for (int i = 0; i < field_size_ / 2 + 2; i++)
				ImGui::TableNextColumn();
			ImGui::PushID(3);
			if (ImGui::ImageButton(sprite_down_, 0))
				center_.x++;
			ImGui::PopID();
			ImGui::EndTable();
		}
		ImGui::End();
	}
}

void TicTacToe::VictoryWindow_(int x, int y)
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(200, -1));
	if (ImGui::Begin("VictoryWindow", nullptr, window_flags))
	{
		if (winner == TurnType::Player)
			ImGui::LabelText("", "You have won!");
		else
			ImGui::LabelText("", "You have lost.");

		if (ImGui::Button("OK", ImVec2(-1, 0.0f)))
		{
			show_victory_window = false;
			Restart();
		}
	}
	ImGui::End();
}

TicTacToe* TicTacToe::GetInstance(std::string config_path)
{
	if (!instance_)
		instance_ = new TicTacToe(config_path);

	return instance_;
}

void TicTacToe::Restart()
{
	delete(gm_);
	gm_ = new GameManager(player_is_first_, init_player_agg_, ai_agg_,
		agg_damp_, vision_range_, attack_range_, top_n_, max_depth_, attack_weight_);
	if (!player_is_first_)
		gm_->MakeMoveAI();
	if (show_hints_)
		best_move_ = gm_->GetBestMove(TurnType::Player);
	center_.x = -field_size_ / 2;
	center_.y = -field_size_ / 2;
}

void TicTacToe::Start()
{
	//Create SFML window
	sf::RenderWindow window(sf::VideoMode(window_size_w_, window_size_h_), window_title_, sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);
	window.setTitle(window_title_.c_str());

	//Set icon
	sf::Image icon;
	icon.loadFromFile(icon_path_);
	window.setIcon(icon_size_, icon_size_, icon.getPixelsPtr());

	//Set ImGui font
	ImGuiIO& IO = ImGui::GetIO();
	IO.Fonts->Clear();
	IO.Fonts->AddFontFromFileTTF(font_file_name_.c_str(), fonst_size_);
	ImGui::SFML::UpdateFontTexture();

	//Initialize sprites
	sf::Texture texture_cell, texture_cross, texture_zero, texture_hint,
		texture_up, texture_down, texture_left, texture_right;
	texture_cell.loadFromFile("images/cell.jpg");
	texture_cross.loadFromFile("images/cross.jpg");
	texture_zero.loadFromFile("images/zero.jpg");
	texture_hint.loadFromFile("images/hint.jpg");
	texture_up.loadFromFile("images/up.jpg");
	texture_down.loadFromFile("images/down.jpg");
	texture_left.loadFromFile("images/left.jpg");
	texture_right.loadFromFile("images/right.jpg");
	sprite_cell_ = sf::Sprite(texture_cell);
	sprite_cross_ = sf::Sprite(texture_cross);
	sprite_zero_ = sf::Sprite(texture_zero);
	sprite_hint_ = sf::Sprite(texture_hint);
	sprite_up_ = sf::Sprite(texture_up);
	sprite_down_ = sf::Sprite(texture_down);
	sprite_left_ = sf::Sprite(texture_left);
	sprite_right_ = sf::Sprite(texture_right);

	//App cycle
	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());


		SettingsWindow_();
		FieldWindow_();

		if (show_victory_window)
		{
			VictoryWindow_(window.getSize().x * 0.5f, window.getSize().y * 0.5f);
		}

		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}
