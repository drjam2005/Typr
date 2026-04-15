#include <algorithm>
#include <raylib.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
//#include <font.h>

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

// updater 
class Updater {
	WordList& wordList;
	size_t currentWord = 0;
public:
	Updater(WordList& wordList) : wordList(wordList) { }

	void Loop(){
		if(IsKeyPressed(KEY_SPACE))
			currentWord = std::min(wordList.get_words().size() - 1, currentWord + 1);
		if(IsKeyPressed(KEY_BACKSPACE) && !wordList.get_words()[currentWord].index)
			currentWord--;

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
	RendererConfig config;
	Font font = GetFontDefault();
public:
	Renderer(WordList& wordList) : wordList(wordList) {
		//font = LoadFontFromMemory(".ttf", RobotoMonoNerdFont_SemiBold_ttf, RobotoMonoNerdFont_SemiBold_ttf_len, 32, 0, 0);
	}

	void Loop(){
		// word rendering
		size_t yPos = 20;
		for(auto& word : wordList.get_words()){
			int x = 20;
			for(size_t i = 0; i < word.word_len; i++){
				char str[2] = {word.word[i], '\0'};
				DrawTextEx(font, str, (Vector2){(float)x, (float)yPos}, 20, 5, GRAY);
				x += 15;
			}
			x = 20;
			for(size_t i = 0; i < word.index; i++){
				char str[2] = {word.guess[i], '\0'};
				if(word.guess[i] == word.word[i])
					DrawTextEx(font, str, (Vector2){(float)x, (float)yPos}, 20, 5, WHITE);
				else
					DrawTextEx(font, str, (Vector2){(float)x, (float)yPos}, 20, 5, RED);
				x += 15;
			}
			yPos += 30;
		}
	}
};

class Game {
	Updater updater;
	Renderer renderer;
	WordList wordList;
public:
	Game(WordList wordList) : 
		updater(Updater(this->wordList)),
		renderer(Renderer(this->wordList)) 
	{
		this->wordList = wordList;
	}
	void Loop() {
		updater.Loop();
		renderer.Loop();
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
