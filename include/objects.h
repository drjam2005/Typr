#pragma once
#include <raylib.h>

struct Line {
	char* text = nullptr;
	int length = 0;
};

struct Text {
	Line* lines = nullptr;
	int count = 0;

	void NewLine();
	void AppendLine(Line& line);
	void AddChar(int lineIndex, int charIndex, char chr);
	void RemoveChar(int lineIndex, int charIndex);
};
