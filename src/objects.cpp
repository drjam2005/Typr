#include <objects.h>
#include <cstring>

Word::Word(const char* text){
	strcpy(word, text);
	word_len = strlen(word);
}

char Word::get_word_char_at(size_t index){
	if(word_len < index)
		return 0;
	return word[index];
}
char Word::get_guess_char_at(size_t index){
	if(this->index < index)
		return 0;
	return guess[index];
}

void Word::update(){
	if(index < WORD_SIZE && ((guess[index] = GetCharPressed()) && guess[index] != KEY_SPACE))
		index++;
	if(index > 0 && GetKeyPressed() == KEY_BACKSPACE)
		guess[index--] = 0;
}

void WordList::clear_words(){
	words.clear();
}
void WordList::set_words(std::vector<Word> words){
	clear_words();
	add_words(words);
}

void WordList::add_word(const char* text){
	words.emplace_back(text);
}
void WordList::add_words(std::vector<Word> givenWords){
	for(auto& word : givenWords)
		this->words.emplace_back(word.word);
}

std::vector<Word>& WordList::get_words(){
	return words;
}
