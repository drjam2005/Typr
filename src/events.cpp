#include <events.h>


std::deque<Event>& EventBus::get_events(){
	return this->events;
}

void EventBus::push_event(Event event){
	events.push_back(event);
}

void EventBus::clear_events(){
	this->events.clear();
}
