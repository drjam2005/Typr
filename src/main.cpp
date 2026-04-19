#include <algorithm>
#include <deque>
#include <raylib.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <font.h>

#define WIDTH (800)
#define HEIGHT (600)
#define WORD_SIZE (256)

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
	size_t font_spacing;
	size_t word_spacing;
	char* font_name;
};

const RendererConfig defaultConfig = {
	.renderer_dimensions = {0, 0, WIDTH, HEIGHT},
	.font_size = 30,
	.font_spacing = 20, // doesn't really work yet
	.word_spacing = 15,
	.font_name = (char*)"test"
};

class Renderer {
	WordList& wordList;
	EventBus& eventBus;
	RendererConfig config = defaultConfig;
	Font font = GetFontDefault();
private:
	Vector2 _render_word(Word& word, Vector2 pos, bool isCurrent=false){
		int x = pos.x, y = pos.y;

		size_t wordWidth = 0, height = 0, guessWidth = 0;
		Vector2 charDims = MeasureTextEx(font, "a", config.font_size, config.font_spacing);
		if(isCurrent)
			DrawRectangle(x + (charDims.x*word.index), y, charDims.x, charDims.y, DARKGRAY);

		for(size_t i = 0; i < word.word_len; i++){
			char str[2] = {word.get_word_char_at(i), '\0'};
			DrawTextEx(font, str, (Vector2){(float)x, (float)y}, config.font_size, config.font_spacing, GRAY);
			x += MeasureTextEx(font, str, config.font_size, config.font_spacing).x;
			wordWidth += MeasureTextEx(font, str, config.font_size, config.font_spacing).x;
		}
		x = pos.x;
		for(size_t i = 0; i < word.index; i++){
			char str[2] = {word.guess[i], '\0'};
			if(word.get_guess_char_at(i) == word.get_word_char_at(i))
				DrawTextEx(font, str, (Vector2){(float)x, (float)y}, config.font_size, config.font_spacing, WHITE);
			else{
				if(i < word.word_len)
					str[0] = word.get_word_char_at(i);
				DrawTextEx(font, str, (Vector2){(float)x, (float)y}, config.font_size, config.font_spacing, RED);
			}
			x += MeasureTextEx(font, str, config.font_size, config.font_spacing).x;
			guessWidth += MeasureTextEx(font, str, config.font_size, config.font_spacing).x;
		}

		return Vector2 {
			(float)std::max(wordWidth, guessWidth),
			charDims.y
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
			x += _render_word(word, (Vector2){(float)x, (float)yPos}, (currentIndex == index)).x;
			x += config.word_spacing;
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
	InitWindow(WIDTH, HEIGHT, "Typr");
	SetTargetFPS(60);

	WordList words;
	{
		words.add_word("the");
		words.add_word("quick");
		words.add_word("brown");
		words.add_word("fox");
		words.add_word("jumps");
		words.add_word("over");
		words.add_word("the");
		words.add_word("lazy");
		words.add_word("dog");
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
