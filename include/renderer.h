#pragma once
#include <raylib.h>
#include <objects.h>

enum MODE {
	INSERT,
	NORMAL,
	VISUAL,
	VISUAL_LINE,
	COMMAND,
};

class Renderer {
private:
	Text text;
	char* currFileName = nullptr;
	Vector2 index = {0, 0};
	MODE mode = INSERT;

	// visual stuff
	Vector2 visualStart;
	Vector2 visualEnd;
public:
	Renderer();
	void Render();
	void Update();
	void ParseInsert();
	void ParseNormal();
	void ParseVisual();
	void ParseVisualLine();
};
