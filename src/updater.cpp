#include <updater.h>

Updater::Updater(WordList& wordList, EventBus& eventBus) : wordList(wordList), eventBus(eventBus) { }

void Updater::Loop(){
	Event thisEvent = {
		.type = EVENT_INDEX_INFO,
		.data = {
			.index_info = currentWord
		}
	};
	eventBus.push_event(thisEvent);
	if(IsKeyPressed(KEY_SPACE)){
		if(!started){
			eventBus.push_event((Event){.type = EVENT_START});
			started = true;
		}
		currentWord = std::min((int)wordList.get_words().size() - 1, currentWord + 1);
	}
	if(IsKeyPressed(KEY_BACKSPACE) && !wordList.get_words()[currentWord].index){
		if(!started){
			eventBus.push_event((Event){.type = EVENT_START});
			started = true;
		}
		currentWord = std::max(0, currentWord-1);
	}

	wordList.get_words()[currentWord].update();
}
