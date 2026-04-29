#pragma once
#ifndef _UPDATER_H
#include <objects.h>
#include <events.h>
#include <stddef.h>
#include <raylib.h>

class Updater {
	WordList& wordList;
	EventBus& eventBus;
	int currentWord = 0;
	bool started = false;
public:
	Updater(WordList& wordList, EventBus& eventBus);
	void Loop();
};

#endif
