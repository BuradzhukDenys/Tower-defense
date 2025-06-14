#pragma once
#include <ostream>
class GameState
{
public:
	enum class State
	{
		Pause,
		Game,
		WavePlay,
		Win,
		Loss,
		Restart,
		Exit
	};

	static State& getState();
	static void setState(const State& state);
	static State& getStateBeforePause();
	static void setStateBeforePause(const State& state);

	GameState() = delete;
private:
	static State stateBeforePause;
	static State gameState;
};

std::ostream& operator<<(std::ostream& os, const GameState::State& gameState);//delete

