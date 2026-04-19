#pragma once
#ifndef _UPDATER_H
#include <objects.h>
#include <events.h>
#include <stddef.h>
#include <raylib.h>

class Updater {
	WordList& wordList;
	EventBus& eventBus;
	size_t currentWord = 0;
public:
	Updater(WordList& wordList, EventBus& eventBus);
	void Loop();
};

#endif
