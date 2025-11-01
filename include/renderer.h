#pragma once
#include <raylib.h>
#include <objects.h>
#include <string>


class Renderer {
private:
	Text text;
	Vector2 index = {0, 0};
public:
	Renderer();
	void Render();
	void Update();
	void MoveCursor();
};
