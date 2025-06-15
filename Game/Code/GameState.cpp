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