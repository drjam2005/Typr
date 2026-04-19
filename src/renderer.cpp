#include <renderer.h>

Renderer::Renderer(WordList& wordList, EventBus& eventBus) : wordList(wordList), eventBus(eventBus) {
	font = LoadFontFromMemory(".ttf", RobotoMonoNerdFont_SemiBold_ttf, RobotoMonoNerdFont_SemiBold_ttf_len, 32, 0, 0);
}

void Renderer::Loop(){
	float x = config.renderer_dimensions.x;
	float y = config.renderer_dimensions.y;
	float maxWidth = config.renderer_dimensions.width;
	float maxHeight = config.renderer_dimensions.height;
	float currentWidth = 0;
	float currentHeight = 0;

	size_t index = 0;
	size_t currentRow = 0;
	size_t currentIndex;
	for(auto& event : eventBus.get_events()){
		if(event.type == EVENT_INDEX_INFO)
			currentIndex = event.data.index_info;
	}

	for(auto& word : wordList.get_words()){
		Vector2 wordDims = _measure_word(word);
		if(currentWidth + wordDims.x < maxWidth){
			_render_word(word, (Vector2){(float)(x+currentWidth), (float)(y+currentHeight)}, (currentIndex == index));
		}else {
			currentWidth = 0;
			currentRow += 1;
			currentHeight += _measure_word(word).y;
			_render_word(word, (Vector2){(float)(x+currentWidth), (float)(y+currentHeight)}, (currentIndex == index));
		}
		if(y+currentHeight > maxHeight){
			eventBus.push_event((Event)
				{
					.type = EVENT_NEED_WORDS,
					.data = {0}
				}
			);
		}
		currentWidth += _measure_word(word).x;
		currentWidth += _measure_word(Word(" ")).x;
		++index;
	}
}

// helper functions
Vector2 Renderer::_measure_word(Word word){
	size_t wordWidth = 0, height = 0, guessWidth = 0;
	float maxHeight = MeasureTextEx(font, "a", config.font_size, config.font_spacing).y;
	for(size_t i = 0; i < word.word_len; i++){
		char str[2] = {word.get_word_char_at(i), '\0'};
		Vector2 measuredText = MeasureTextEx(font, str, config.font_size, config.font_spacing);
		wordWidth += measuredText.x;
		maxHeight = (measuredText.y > maxHeight) ? : maxHeight;
	}
	for(size_t i = 0; i < word.index; i++){
		char str[2] = {word.guess[i], '\0'};
		Vector2 measuredText = MeasureTextEx(font, str, config.font_size, config.font_spacing);
		guessWidth += measuredText.x;
		maxHeight = (measuredText.y > maxHeight) ? : maxHeight;
	}

	return Vector2 {
		(float)std::max(wordWidth, guessWidth),
		maxHeight
	};
}

void Renderer::_render_word(Word& word, Vector2 pos, bool isCurrent=false){
	int x = pos.x, y = pos.y;

	Vector2 charDims = MeasureTextEx(font, "a", config.font_size, config.font_spacing);
	if(isCurrent)
		DrawRectangle(x + (charDims.x*word.index), y, charDims.x, charDims.y, DARKGRAY);

	for(size_t i = 0; i < word.word_len; i++){
		char str[2] = {word.get_word_char_at(i), '\0'};
		DrawTextEx(font, str, (Vector2){(float)x, (float)y}, config.font_size, config.font_spacing, GRAY);
		x += MeasureTextEx(font, str, config.font_size, config.font_spacing).x;
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
	}
}
