#include "game_manager.h"

Cell GameManager::GetBestMove_(TurnType turn)
{
	SortedSequence<WeightedCell>* top_cells = GetTopCells_(turn);
	float best_score = (turn == TurnType::AI) ? -1 : 1;
	WeightedCell cell = top_cells->Get(0);
	Cell best_move(cell.x, cell.y);
	for (int i = top_cells->GetLength() - 1; i >= 0; i--)
	{
		cell = top_cells->Get(i);
		TurnType child_turn = (turn == TurnType::AI) ? TurnType::Player : TurnType::AI;
		Sequence<Cell>* field_of_view = field_->GetFieldOfViewCopy();
		field_->Set(cell.x, cell.y, Turn2Cell_(turn));
		float score = ScoreField_(child_turn, cell.x, cell.y, 1);
		field_->Set(cell.x, cell.y, CellType::None);
		field_->SetFieldOfView(field_of_view);
		if (turn == TurnType::AI && score > best_score || turn == TurnType::Player && score < best_score)
		{
			best_score = score;
			best_move.x = cell.x;
			best_move.y = cell.y;
		}
	}
	delete(top_cells);
	return best_move;
;}

float GameManager::GetWeight_(TurnType turn, int x, int y)
{
	float weight = 0.0f;
	weight += CheckLine_(turn, x, y, 1, 0);
	weight += CheckLine_(turn, x, y, 0, 1);
	weight += CheckLine_(turn, x, y, 1, 1);
	weight += CheckLine_(turn, x, y, 1, -1);
	return weight;
}

WeightedCell GameManager::GetWeightedCell_(TurnType turn, int x, int y)
{
	float player_weight = GetWeight_(TurnType::Player, x, y);
	float ai_weight = GetWeight_(TurnType::AI, x, y);
	float agg = (turn == TurnType::Player) ? player_agg_ : (1 - ai_agg_);
	float weight = ai_weight * (1 - agg) + player_weight * agg;
	WeightedCell w_cell(x, y, weight, ai_weight, player_weight);
	return w_cell;
}

SortedSequence<WeightedCell>* GameManager::GetTopCells_(TurnType turn)
{
	SortedSequence<WeightedCell>* cells = new SortedSequence<WeightedCell>();
	IIterator<Cell>* it = field_->GetFieldOfView()->GetIterator();
	for (; it->HasNext(); it->Next())
	{
		int x = it->GetCurrentItem().x;
		int y = it->GetCurrentItem().y;
		cells->Add(GetWeightedCell_(turn, x, y));
	}
	delete(it);

	int start = std::max((int)cells->GetLength() - (int)top_n_ - 1, 0);
	SortedSequence<WeightedCell>* top_cells = cells->GetSubsequence(start, cells->GetLength());
	delete(cells);
	return top_cells;
}

float GameManager::CheckLine_(TurnType turn, int x, int y, int dx, int dy)
{
	Sequence<Attack*>* attacks = new ListSequence<Attack*>();
	Attack* center_attack = new Attack(1, 0, 0);
	CheckRay_(turn, x, y, dx, dy, attack_range_, attacks, center_attack);
	CheckRay_(turn, x, y, -dx, -dy, attack_range_, attacks, center_attack);
	attacks->Append(center_attack);

	float weight = 0.0f;
	IIterator<Attack*>* it = attacks->GetIterator();
	for (; it->HasNext(); it->Next())
	{
		Attack* ai = it->GetCurrentItem();
		weight += attack_weight_[ai->power][ai->potential];
		delete(ai);
	}
	delete(it);
	delete(attacks);
	return weight;
}

void GameManager::CheckRay_(TurnType turn, int x, int y, int dx, int dy, 
	int range, Sequence<Attack*>* attacks, Attack* center_attack)
{
	Attack* attack = center_attack;
	size_t remoteness = 0;
	for (int i = 1; i < range; i++)
	{
		int xi = x + dx * i;
		int yi = y + dy * i;
		CellType cell = field_->Get(xi, yi);
		if (cell == Turn2Cell_(turn))
		{
			if (!attack)
				attack = new Attack(1, 1, remoteness);
			else
				attack->power += 1;
		}
		else if (cell == CellType::None)
		{
			if (attack)
			{
				attack->potential++;
				if (attack != center_attack)
					attacks->Append(attack);
				attack = nullptr;
			}
			remoteness++;
		}
		else
			break;
	}
	if (attack)
	{
		CellType cell = field_->Get(x + vision_range_ * dx, y + vision_range_ * dy);
		if (cell == CellType::None)
			attack->potential++;
		if (attack != center_attack)
			attacks->Append(attack);
	}
}

float GameManager::GetPlayerAgg()
{
	return player_agg_;
}

float GameManager::ScoreField_(TurnType turn, int x, int y, size_t depth)
{
	int victory_check = 0;
	if (turn == TurnType::AI)
	{
		victory_check = VictoryCheck_(TurnType::Player, x, y);
		if (victory_check == 2)
			return -1;
	}
	else
	{
		victory_check = VictoryCheck_(TurnType::AI, x, y);
		if (victory_check == 2)
			return 1;
	}
	if (victory_check == 1)
		depth = std::min(depth, max_depth_ - 1);

	if (depth < max_depth_)
	{
		SortedSequence<WeightedCell>* top_cells = GetTopCells_(turn);
		float score = (turn == TurnType::AI) ? -1 : 1;
		for (int i = top_cells->GetLength() - 1; i >= 0 ; i--)
		{
			WeightedCell cell = top_cells->Get(i);
			TurnType child_turn = (turn == TurnType::AI) ? TurnType::Player : TurnType::AI;
			Sequence<Cell>* field_of_view = field_->GetFieldOfViewCopy();
			field_->Set(cell.x, cell.y, Turn2Cell_(turn));
			float child_score = ScoreField_(child_turn, cell.x, cell.y, depth + 1);
			field_->Set(cell.x, cell.y, CellType::None);
			field_->SetFieldOfView(field_of_view);
			if (turn == TurnType::AI)
				score = std::max(score, child_score);
			else
				score = std::min(score, child_score);
		}
		delete(top_cells);
		return score;
	}
	else
		return 0;
}

int GameManager::VictoryCheck_(TurnType turn, int x, int y)
{
	int victory = 0;
	victory = std::max(victory, VictoryCheckLine_(turn, x, y, 0, 1));
	victory = std::max(victory, VictoryCheckLine_(turn, x, y, 1, 0));
	victory = std::max(victory, VictoryCheckLine_(turn, x, y, 1, 1));
	victory = std::max(victory, VictoryCheckLine_(turn, x, y, 1, -1));
	return victory;
}

int GameManager::VictoryCheckLine_(TurnType turn, int x, int y, int dx, int dy)
{
	Sequence<Attack*>* attacks = new ListSequence<Attack*>();
	Attack* center_attack = new Attack(1, 0, 0);
	CheckRay_(turn, x, y, dx, dy, 5, attacks, center_attack);
	CheckRay_(turn, x, y, -dx, -dy, 5, attacks, center_attack);
	int victory = 0;
	if (center_attack->power == 5)
		victory = 2;
	else if (center_attack->power == 4 && center_attack->potential == 2)
		victory = 1;
	
	IIterator<Attack*>* it = attacks->GetIterator();
	for (; it->HasNext(); it->Next())
		delete(it->GetCurrentItem());
	delete(it);
	delete(attacks);
	delete(center_attack);
	
	return victory;
}

GameManager::GameManager(bool player_is_first, float player_agg, float ai_agg,
	float agg_damp, int vision_range, int attack_range, size_t top_n,
	size_t max_depth, float attack_weight[6][3])
{
	player_is_first_ = player_is_first;
	player_agg_ = player_agg;
	ai_agg_ = ai_agg;
	agg_damp_ = agg_damp;
	vision_range_ = vision_range;
	attack_range_ = attack_range;
	top_n_ = top_n;
	max_depth_ = max_depth;
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++)
			attack_weight_[i][j] = attack_weight[i][j];
	turn_ = (player_is_first_) ? TurnType::Player : TurnType::AI;
	field_ = new Field(vision_range_);
}

GameManager::~GameManager()
{
	delete(field_);
}

bool GameManager::MakeMoveAI()
{
	if (turn_ != TurnType::AI)
		throw "Trying to make AI move durning player turn.";
	Cell cell = GetBestMove_(TurnType::AI);
	turn_ = TurnType::Player;
	field_->Set(cell.x, cell.y, Turn2Cell_(TurnType::AI));
	return VictoryCheck_(TurnType::AI, cell.x, cell.y) == 2;
}

bool GameManager::MakeMovePlayer(int x, int y)
{
	if (turn_ != TurnType::Player)
		throw "Trying to make player move durning AI turn.";
	turn_ = TurnType::AI;
	WeightedCell cell = GetWeightedCell_(TurnType::Player, x, y);
	field_->Set(x, y, Turn2Cell_(TurnType::Player));
	float current_agg = cell.w_player / (cell.w_player + cell.w_ai);
	player_agg_ = (player_agg_ * (1 - agg_damp_) + current_agg  * agg_damp_);
	return VictoryCheck_(TurnType::Player, x, y) == 2;
}

CellType GameManager::GetCell(int x, int y)
{
	return field_->Get(x, y);
}

Cell GameManager::GetBestMove(TurnType turn)
{
	return GetBestMove_(turn);
}

CellType GameManager::Turn2Cell_(TurnType turn)
{
	if (player_is_first_)
	{
		if (turn == TurnType::Player)
			return CellType::X;
		else
			return CellType::O;
	}
	else
	{
		if (turn == TurnType::AI)
			return CellType::X;
		else
			return CellType::O;
	}
}

TurnType GameManager::Cell2Turn_(CellType cell)
{
	if (player_is_first_)
	{
		if (cell == CellType::X)
			return TurnType::Player;
		else
			return TurnType::AI;
	}
	else
	{
		if (cell == CellType::O)
			return TurnType::Player;
		else
			return TurnType::AI;
	}
}
