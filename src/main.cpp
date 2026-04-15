#include <raylib.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

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
		printf("word: %s\nlen: %zu", word, word_len);
	}

	void update(){
		if(index < WORD_SIZE && (guess[index] = GetCharPressed()))
			index++;
		if(index > 0 && GetKeyPressed() == KEY_BACKSPACE)
			guess[index--] = 0;
	}
};

class WordList {
	std::vector<Word> words;
public:
	void add_word(const char* text){
		words.emplace_back(text);
	}

	std::vector<Word>& get_words_ref(){
		return words;
	}
};

class Game {
	
};

class Updater {
	
};

class Renderer {

};


int main(){
	InitWindow(800, 600, "Typr");
	SetTargetFPS(60);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("Hello, Raylib!\n", 20, 20, 30, WHITE);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
