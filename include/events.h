#pragma once
#ifndef _EVENTS_H
#include <deque>

enum EVENT_TYPE {
	EVENT_START,
	EVENT_PRESS,
	EVENT_INDEX_INFO,
	EVENT_NEED_WORDS
};

struct Event {
	EVENT_TYPE type;
	union{
		char press_key;
		int index_info;
		float words_max;
	} data;
};

class EventBus {
private:
	std::deque<Event> events;
public:

	void push_event(Event event);
	void clear_events();

	std::deque<Event>& get_events();
};

#endif
