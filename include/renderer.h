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
	size_t row_focus;
	char* font_name;
};

const RendererConfig defaultConfig = {
	.renderer_dimensions = {0, 0, WIDTH, HEIGHT},
	.font_size = 100,
	.font_spacing = 20,
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
	void Loop();
private:
	Vector2 _measure_word(Word word);
	void _render_word(Word& word, Vector2 pos, bool isCurrent);
};

#endif
