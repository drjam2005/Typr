#pragma once
#ifndef _RENDERER_H
#include <raylib.h>
#include <objects.h>
#include <events.h>
#include <updater.h>
#include <font.h>

#define WIDTH (800)
#define HEIGHT (600)

struct RendererConfig {
	Rectangle renderer_dimensions;
	size_t font_size;
	size_t font_spacing;
	size_t word_spacing;
	int row_focus;
	char* font_name;
};

const RendererConfig defaultConfig = {
	.renderer_dimensions = {50, 50, WIDTH-100, HEIGHT-100},
	.font_size = 67,
	.font_spacing = 0,
	.word_spacing = 15,
	.row_focus = 2,
	.font_name = (char*)"test"
};

class Renderer {
	WordList& wordList;
	EventBus& eventBus;
	RendererConfig config = defaultConfig;
	Font font = GetFontDefault();
public:
	Renderer(WordList& wordList, EventBus& eventBus);
	RendererConfig& get_working_config();
	Vector2 _measure_word(Word word);
	bool pendingRequest = false;
	void Loop();
private:
	int draggingCorner = -1;
    int currentRow = 0;
	float smoothY = 0;
	float targetY = 0;
	Vector2 renderIndexes;
	void _render_word(Word& word, Vector2 pos, bool isCurrent);
};

#endif
