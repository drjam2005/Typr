#include <algorithm>
#include <deque>
#include <raylib.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <font.h>

#define WORD_SIZE 256
// testing phase
struct Word {
	size_t index = 0;
	size_t word_len = 0;
	char word[WORD_SIZE] = {0};
	char guess[WORD_SIZE] = {0};

	Word(const char* text){
		strcpy(word, text);
		word_len = strlen(word);
	}

	char get_word_char_at(size_t index){
		if(word_len < index)
			return 0;
		return word[index];
	}
	char get_guess_char_at(size_t index){
		if(this->index < index)
			return 0;
		return guess[index];
	}

	void update(){
		if(index < WORD_SIZE && ((guess[index] = GetCharPressed()) && guess[index] != KEY_SPACE))
			index++;
		if(index > 0 && GetKeyPressed() == KEY_BACKSPACE)
			guess[index--] = 0;
	}
};

class WordList {
	std::vector<Word> words;
public:
	void clear_words(){
		words.clear();
	}
	void set_words(std::vector<Word> words){
		clear_words();
		add_words(words);
	}

	void add_word(const char* text){
		words.emplace_back(text);
	}
	void add_words(std::vector<Word> givenWords){
		for(auto& word : givenWords)
			this->words.emplace_back(word.word);
	}

	std::vector<Word>& get_words(){
		return words;
	}
};

enum EVENT_TYPE {
	EVENT_PRESS,
	EVENT_INDEX_INFO
};

struct Event {
	EVENT_TYPE type;
	union{
		char press_key;
		size_t index_info;
	} data;
};

class EventBus {
private:
	std::deque<Event> events;
public:
	std::deque<Event>& get_events(){
		return this->events;
	}

	void push_event(Event event){
		events.push_back(event);
	}

	void clear_events(){
		this->events.clear();
	}
};

// updater 
class Updater {
	WordList& wordList;
	EventBus& eventBus;
	size_t currentWord = 0;
public:
	Updater(WordList& wordList, EventBus& eventBus) : wordList(wordList), eventBus(eventBus) { }

	void Loop(){
		Event thisEvent = {
			.type = EVENT_INDEX_INFO,
			.data = {
				.index_info = currentWord
			}
		};
		eventBus.push_event(thisEvent);
		if(IsKeyPressed(KEY_SPACE)){
			currentWord = std::min(wordList.get_words().size() - 1, currentWord + 1);
		}
		if(IsKeyPressed(KEY_BACKSPACE) && !wordList.get_words()[currentWord].index){
			--currentWord;
		}

		wordList.get_words()[currentWord].update();
	}
};

// renderer
// renderer config
struct RendererConfig {
	Rectangle renderer_dimensions;
	size_t font_size;
	char* font_name;
};

class Renderer {
	WordList& wordList;
	EventBus& eventBus;
	RendererConfig config;
	Font font = GetFontDefault();
private:
	Vector2 _render_text(Word& word, Vector2 pos, bool isCurrent=false){
		int x = pos.x, y = pos.y;

		size_t wordWidth = 0, height = 0, guessWidth = 0;
		if(isCurrent)
			DrawRectangle(x + (15*word.index), y, 20, 20, DARKGRAY);

		for(size_t i = 0; i < word.word_len; i++){
			char str[2] = {word.word[i], '\0'};
			DrawTextEx(font, str, (Vector2){(float)x, (float)y}, 20, 5, GRAY);
			x += 15;
			wordWidth += 15;
		}
		x = pos.x;
		for(size_t i = 0; i < word.index; i++){
			char str[2] = {word.guess[i], '\0'};
			if(word.guess[i] == word.word[i])
				DrawTextEx(font, str, (Vector2){(float)x, (float)y}, 20, 5, WHITE);
			else{
				if(i < word.word_len)
					str[0] = word.word[i];
				DrawTextEx(font, str, (Vector2){(float)x, (float)y}, 20, 5, RED);
			}
			x += 15;
			guessWidth += 15;
		}

		return Vector2 {
			(float)std::max(wordWidth, guessWidth),
			MeasureTextEx(font, "a", 20, 5).y
		};
	}
public:
	Renderer(WordList& wordList, EventBus& eventBus) : wordList(wordList), eventBus(eventBus) {
		font = LoadFontFromMemory(".ttf", RobotoMonoNerdFont_SemiBold_ttf, RobotoMonoNerdFont_SemiBold_ttf_len, 32, 0, 0);
	}

	void Loop(){
		// word rendering
		size_t yPos = 20;
		size_t x = 20;
		size_t index = 0;
		
		size_t currentIndex;
		for(auto& event : eventBus.get_events()){
			if(event.type == EVENT_INDEX_INFO)
				currentIndex = event.data.index_info;
		}

		for(auto& word : wordList.get_words()){
			x += _render_text(word, (Vector2){(float)x, (float)yPos}, (currentIndex == index)).x;
			x += 10; // space width or something

			++index;
		}
	}
};

class Game {
	Updater updater;
	Renderer renderer;
	WordList wordList;
	EventBus eventBus;
public:
	Game(WordList wordList) : 
		updater(Updater(this->wordList, eventBus)),
		renderer(Renderer(this->wordList, eventBus)) 
	{
		this->wordList = wordList;
	}
	void Loop() {
		updater.Loop();
		renderer.Loop();
		eventBus.clear_events();
	}
};

class App {
	// todo
	Game game;
public:
	void Loop();
};

int main(){
	InitWindow(800, 600, "Typr");
	SetTargetFPS(60);

	WordList words;
	{
		words.add_word("word1");
		words.add_word("word2");
		words.add_word("word3");
		words.add_word("word4");
		words.add_word("word5");
	}

	Game game(words);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		game.Loop();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
