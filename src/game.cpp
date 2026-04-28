#include "events.h"
#include <cmath>
#include <game.h>
#include <fstream>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <raylib.h>

static std::vector<std::string> englishWords; // loaded once, sampled from

Game::Game(WordList& wordList) : 
    updater(Updater(this->wordList, eventBus)),
    renderer(Renderer(this->wordList, eventBus)) 
{
    // load english200.json once
    std::ifstream f("../include/english200.json");
    nlohmann::json j = nlohmann::json::parse(f);
    for(auto& w : j["words"])
        englishWords.push_back(w.get<std::string>().c_str());

    this->wordList = wordList;
    eventBus.push_event((Event){ .type = EVENT_NEED_WORDS });

}

void Game::Loop() {

	float dt = GetFrameTime();
	if(started)
		elapsedTime += dt;

	DrawTextEx(renderer.get_font(), TextFormat("%.0f wpm", elapsedTime ? (wordList.get_correct_words()/std::ceil(elapsedTime))*60 : 0.0f), 
			(Vector2){
				renderer.get_working_config().renderer_dimensions.x,
				renderer.get_working_config().renderer_dimensions.y-renderer._measure_word(Word(" ")).y
			}, 
			renderer.get_working_config().font_size, renderer.get_working_config().font_spacing, GRAY);
    for(auto& event : eventBus.get_events()){
        if(event.type == EVENT_NEED_WORDS){
            RendererConfig config = renderer.get_working_config();
            float spaceWidth = renderer._measure_word(Word(" ")).x;
            float currentWidth = 0;

            while(true){
                // pick a random word from the list
                const std::string& picked = englishWords[rand() % englishWords.size()];
                Word w(picked.c_str());
                float wordWidth = renderer._measure_word(w).x;

                if(currentWidth + wordWidth > config.renderer_dimensions.width)
                    break;

                wordList.add_word(w.word);
                currentWidth += wordWidth + spaceWidth;
            }

            renderer.pendingRequest = false;
        }
		if(event.type == EVENT_START)
			started = true;
    }
    eventBus.clear_events();

    updater.Loop();
    renderer.Loop();
}
