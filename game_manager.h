#include "field.h"
#include "sorted_sequence.h"
#pragma once

enum class TurnType
{
	Player = 1,
	AI
};

struct Attack
{
	size_t power;
	size_t potential;
	size_t remoteness;

	Attack(size_t power, size_t potential, size_t remoteness)
	{
		this->power = power;
		this->potential = potential;
		this->remoteness = remoteness;
	}
};

struct WeightedCell
{
	int x;
	int y;
	float w;
	float w_ai;
	float w_player;

	WeightedCell()
	{
		this->x = 0;
		this->y = 0;
		this->w = 0.0f;
		this->w_ai = 0.0f;
		this->w_player = 0.0f;
	}

	WeightedCell(int x, int y, float w, float w_ai, float w_player)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->w_ai = w_ai;
		this->w_player = w_player;
	}

	friend bool operator == (WeightedCell const& a, WeightedCell const& b)
	{
		return (a.x == b.x) && (a.y == b.y);
	}
	friend bool operator != (WeightedCell const& a, WeightedCell const& b)
	{
		return (a.x != b.x) || (a.y != b.y);
	}
	friend bool operator >= (WeightedCell const& a, WeightedCell const& b)
	{
		return a.w >= b.w;
	}
	friend bool operator <= (WeightedCell const& a, WeightedCell const& b)
	{
		return a.w <= b.w;
	}
	friend bool operator > (WeightedCell const& a, WeightedCell const& b)
	{
		return a.w > b.w;
	}
	friend bool operator < (WeightedCell const& a, WeightedCell const& b)
	{
		return a.w < b.w;
	}
};

class GameManager
{
private:
	Field* field_;
	TurnType turn_;

	bool player_is_first_;
	float player_agg_;
	float ai_agg_;
	float agg_damp_;
	int vision_range_;
	int attack_range_;
	size_t top_n_;
	size_t max_depth_;
	float attack_weight_[6][3];

	Cell GetBestMove_(TurnType turn);
	float GetWeight_(TurnType turn, int x, int y);
	WeightedCell GetWeightedCell_(TurnType turn, int x, int y);
	SortedSequence<WeightedCell>* GetTopCells_(TurnType turn);
	float CheckLine_(TurnType turn, int x, int y, int dx, int dy);
	void CheckRay_(TurnType turn, int x, int y, int dx, int dy,
		int range, Sequence<Attack*>* attacks, Attack* center_attack);
	float ScoreField_(TurnType turn, int x, int y, size_t depth=0);
	int VictoryCheck_(TurnType turn, int x, int y);
	int VictoryCheckLine_(TurnType turn, int x, int y, int dx, int dy);

public:
	GameManager(bool player_is_first, float player_agg, float ai_agg,
		float agg_damp, int vision_range, int attack_range, size_t top_n, 
		size_t max_depth, float attack_weight[6][3]);
	~GameManager();
	bool MakeMoveAI();
	bool MakeMovePlayer(int x, int y);
	CellType GetCell(int x, int y);
	Cell GetBestMove(TurnType turn);
	CellType Turn2Cell_(TurnType turn);
	TurnType Cell2Turn_(CellType cell);
	float GetPlayerAgg();
};