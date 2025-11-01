#pragma once
#include <raylib.h>
#include <objects.h>

enum MODE {
	INSERT,
	VISUAL,
};

class Renderer {
private:
	Text text;
	Vector2 index = {0, 0};
	MODE mode = INSERT;
public:
	Renderer();
	void Render();
	void Update();
	void ParseVisual();
};
