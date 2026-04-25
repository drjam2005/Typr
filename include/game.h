#pragma once
#ifndef _GAME_H

#include <objects.h>
#include <updater.h>
#include <renderer.h>
#include <events.h>

class Game {
	Updater updater;
	Renderer renderer;
	std::vector<Word> words;
	WordList wordList;
	EventBus eventBus;
public:
	Game(WordList& wordList);
	void Loop();
};

#endif
