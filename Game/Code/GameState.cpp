#include "GameState.h"
GameState::State GameState::gameState = GameState::State::Game;
GameState::State GameState::stateBeforePause = GameState::State::Game;

GameState::State& GameState::getState()
{
	return gameState;
}

void GameState::setState(const State& state)
{
	gameState = state;
}

GameState::State& GameState::getStateBeforePause()
{
	return stateBeforePause;
}

void GameState::setStateBeforePause(const State& state)
{
	stateBeforePause = state;
}

std::ostream& operator<<(std::ostream& os, const GameState::State& gameState)
{
	switch (gameState)
	{
	case GameState::State::Pause:
		os << "Pause\n";
		break;
	case GameState::State::Game:
		os << "Game\n";
		break;
	case GameState::State::WavePlay:
		os << "Wave play\n";
		break;
	case GameState::State::Win:
		os << "Win\n";
		break;
	case GameState::State::Loss:
		os << "Loss\n";
		break;
	case GameState::State::Exit:
		os << "Exit\n";
		break;
	default:
		break;
	}

	return os;
}
