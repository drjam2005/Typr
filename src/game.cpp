#include <game.h>
#include <iostream>

Game::Game(WordList wordList) : 
	updater(Updater(this->wordList, eventBus)),
	renderer(Renderer(this->wordList, eventBus)) 
{
	this->wordList = wordList;
}

void Game::Loop() {
	updater.Loop();
	renderer.Loop();

	for(auto& event : eventBus.get_events()){
	}

	eventBus.clear_events();
}
