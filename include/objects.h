#pragma once
#ifndef _OBJECTS_H
#include <raylib.h>
#include <vector>

#define WORD_SIZE (256)

struct Word {
	size_t index = 0;
	size_t word_len = 0;
	char word[WORD_SIZE] = {0};
	char guess[WORD_SIZE] = {0};

	Word(const char* text);
	char get_word_char_at(size_t index);
	char get_guess_char_at(size_t index);
	bool is_correct();
	void update();
};

class WordList {
	std::vector<Word> words;
public:

	void clear_words();
	void set_words(std::vector<Word> words);
	void add_word(const char* text);
	void add_words(std::vector<Word> givenWords);
	size_t get_correct_words();

	std::vector<Word>& get_words();
};
#endif
