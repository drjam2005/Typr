#include <renderer.h>
#include <raymath.h>
#include <cmath>

Renderer::Renderer(WordList& wordList, EventBus& eventBus) : wordList(wordList), eventBus(eventBus) {
	font = LoadFontFromMemory(".ttf", RobotoMonoNerdFont_SemiBold_ttf, RobotoMonoNerdFont_SemiBold_ttf_len, 64, 0, 0);
	renderIndexes.x = config.renderer_dimensions.x;
	renderIndexes.y = config.renderer_dimensions.y;
}

void Renderer::Loop(){
    float x = config.renderer_dimensions.x;
    float y = config.renderer_dimensions.y;
    float maxWidth = config.renderer_dimensions.width;
    float maxHeight = config.renderer_dimensions.height;
    size_t index = 0;
    size_t currentIndex = 0;
    size_t row = 0;

    for(auto& event : eventBus.get_events()){
        if(event.type == EVENT_INDEX_INFO)
            currentIndex = event.data.index_info;
    }

    Vector2 spaceDims = _measure_word(Word(" "));
    float spaceWidth = spaceDims.x;
    float lineHeight = spaceDims.y;

    targetY = y - fmax(0, currentRow - config.row_focus + 1) * lineHeight;
    float speed = 10.f;
    smoothY += (targetY - smoothY) * speed * GetFrameTime();

    renderIndexes.x = x;
    renderIndexes.y = smoothY;


	BeginScissorMode(x, y, maxWidth, maxHeight);
	bool reachedEnd = true;
    for(auto& word : wordList.get_words()){
        Vector2 wordDims = _measure_word(word);
		
		if(renderIndexes.y > y + maxHeight){
			reachedEnd = false;
			break;
		}

        if(renderIndexes.x + wordDims.x > x + maxWidth){
            renderIndexes.x = x;
            renderIndexes.y += wordDims.y;
            ++row;
        }
        if(index == currentIndex)
            currentRow = row;

		_render_word(word, renderIndexes, index == currentIndex);

        renderIndexes.x += spaceWidth + wordDims.x;
        ++index;
    }
	EndScissorMode();

	if(reachedEnd && !pendingRequest){
		pendingRequest = true;
		eventBus.push_event((Event){
			.type = EVENT_NEED_WORDS
		});
	}

	DrawRectangleLines(x, y, config.renderer_dimensions.width, config.renderer_dimensions.height, BLUE);


	static const float HANDLE_RADIUS = 10.f;
	Vector2 tL = {x, y};
	Vector2 tR = {x + maxWidth, y};
	Vector2 bL = {x, y + maxHeight};
	Vector2 bR = {x + maxWidth, y + maxHeight};

	Vector2 corners[4] = {tL, tR, bL, bR};
	Color colors[4] = {RED, GREEN, BLUE, YELLOW};
	for(int i = 0; i < 4; i++)
		DrawCircleV(corners[i], HANDLE_RADIUS, colors[i]);

	Vector2 mousePos = GetMousePosition();

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		for(int i = 0; i < 4; i++){
			if(Vector2DistanceSqr(mousePos, corners[i]) <= HANDLE_RADIUS * HANDLE_RADIUS){
				draggingCorner = i;
				break;
			}
		}
	}

	if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		draggingCorner = -1;

	if(draggingCorner != -1){
		switch(draggingCorner){
			case 0: // top-left: moves origin, adjusts width+height
				config.renderer_dimensions.width  += config.renderer_dimensions.x - mousePos.x;
				config.renderer_dimensions.height += config.renderer_dimensions.y - mousePos.y;
				config.renderer_dimensions.x = mousePos.x;
				config.renderer_dimensions.y = mousePos.y;
				break;
			case 1: // top-right: adjusts width and y
				config.renderer_dimensions.width  = mousePos.x - config.renderer_dimensions.x;
				config.renderer_dimensions.height += config.renderer_dimensions.y - mousePos.y;
				config.renderer_dimensions.y = mousePos.y;
				break;
			case 2: // bottom-left: adjusts x and height
				config.renderer_dimensions.width  += config.renderer_dimensions.x - mousePos.x;
				config.renderer_dimensions.x = mousePos.x;
				config.renderer_dimensions.height = mousePos.y - config.renderer_dimensions.y;
				break;
			case 3: // bottom-right: adjusts width+height only
				config.renderer_dimensions.width  = mousePos.x - config.renderer_dimensions.x;
				config.renderer_dimensions.height = mousePos.y - config.renderer_dimensions.y;
				break;
		}
	}
}



// helper functions
Vector2 Renderer::_measure_word(Word word){
	Vector2 wordDims = MeasureTextEx(font, word.word, config.font_size, config.font_spacing);
	Vector2 guessDims = MeasureTextEx(font, word.guess, config.font_size, config.font_spacing);

	return Vector2 {
		(float)std::max(wordDims.x, guessDims.x),
		(float)std::max(wordDims.y, guessDims.y)
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

RendererConfig& Renderer::get_working_config(){
	return config;
}

Font Renderer::get_font(){
	return this->font;
}
