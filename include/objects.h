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
	void RemoveLines(int index, int lineCount);
	void MergeLines(int index1, int index2);
	void SplitLine(int index1, int index2);
	void AddChar(int lineIndex, int charIndex, char chr);
	void RemoveChar(int lineIndex, int charIndex);
};
