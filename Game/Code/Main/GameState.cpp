#include "GameState.h"
GameState::State GameState::gameState = GameState::State::Game;

GameState::State& GameState::getState()
{
	return gameState;
}

void GameState::setState(const State& state)
{
	gameState = state;
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
	case GameState::State::RoundPlay:
		os << "Round play\n";
		break;
	case GameState::State::Loss:
		os << "Loss\n";
		break;
	default:
		break;
	}

	return os;
}
