#include <objects.h>
#include <stdlib.h>
#include <string.h>

void Text::NewLine() {
    this->lines = (Line*)realloc(this->lines, sizeof(Line) * (this->count + 1));
    if (!this->lines) return;

    this->lines[this->count].length = 0;
    this->lines[this->count].text = (char*)malloc(1);
    this->lines[this->count].text[0] = '\0';

    this->count++;
}


void Text::AppendLine(Line& line) {
    this->lines = (Line*)realloc(this->lines, sizeof(Line) * (this->count + 1));

    this->lines[this->count].length = line.length;
    this->lines[this->count].text = (char*)malloc(line.length + 1);
    strcpy(this->lines[this->count].text, line.text);

    this->count++;
}



void Text::AddChar(int lineIndex, int charIndex, char chr) {
    if (lineIndex < 0 || lineIndex >= this->count) return;

    Line* line = &this->lines[lineIndex];
    if (charIndex < 0 || charIndex > line->length) return;

    char* newText = (char*)malloc(line->length + 2);
    if (!newText) return;

    strncpy(newText, line->text, charIndex);
    newText[charIndex] = chr;
    strcpy(newText + charIndex + 1, line->text + charIndex);

    free(line->text);
    line->text = newText;
    line->length += 1;
}

void Text::RemoveChar(int lineIndex, int charIndex) {
    if (lineIndex < 0 || lineIndex >= this->count) return;

    Line* line = &this->lines[lineIndex];
    if (charIndex < 0 || charIndex >= line->length) return;

    char* newText = (char*)malloc(line->length);
    if (!newText) return;

    strncpy(newText, line->text, charIndex);
    strcpy(newText + charIndex, line->text + charIndex + 1);

    free(line->text);
    line->text = newText;
    line->length -= 1;
}


