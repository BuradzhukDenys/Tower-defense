#pragma once
#include <ostream>
class GameState
{
public:
	enum class State
	{
		Pause,
		Game,
		RoundPlay,
		Win,
		Loss
	};

	static State& getState();
	static void setState(const State& state);

	GameState() = delete;
private:
	static State gameState;
};

std::ostream& operator<<(std::ostream& os, const GameState::State& gameState);

