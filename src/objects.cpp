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


void Text::MergeLines(int index1, int index2) {
    if (index1 < 0 || index2 < 0 || index2 >= count) return;

    Line* line1 = &lines[index1];
    Line* line2 = &lines[index2];

    char* newText = (char*)malloc(line1->length + line2->length + 1);
    if (!newText) return;

    memcpy(newText, line1->text, line1->length);
    memcpy(newText + line1->length, line2->text, line2->length);
    newText[line1->length + line2->length] = '\0';

    free(line1->text);
    line1->text = newText;
    line1->length = line1->length + line2->length;

    for (int i = index2; i < count - 1; ++i) {
        lines[i] = lines[i + 1];
    }

    count--;
    lines = (Line*)realloc(lines, sizeof(Line) * count);
}

void Text::SplitLine(int yIndex, int xIndex) {
    if (yIndex < 0 || yIndex >= count) return;
    Line& original = lines[yIndex];
    if (xIndex < 0) xIndex = 0;
    if (xIndex > original.length) xIndex = original.length;

    int len1 = xIndex;
    int len2 = original.length - xIndex;

    char* text1 = (char*)malloc(len1 + 1);
    char* text2 = (char*)malloc(len2 + 1);

    memcpy(text1, original.text, len1);
    memcpy(text2, original.text + xIndex, len2);

    text1[len1] = '\0';
    text2[len2] = '\0';

    free(original.text);
    original.text = text1;
    original.length = len1;

    Line* temp = (Line*)realloc(lines, sizeof(Line) * (count + 1));
    if (!temp) return; // bail safely
    lines = temp;

    for (int i = count; i > yIndex + 1; --i) {
        lines[i] = lines[i - 1];
    }

    Line newLine;
    newLine.text = text2;
    newLine.length = len2;

    lines[yIndex + 1] = newLine;
    count++;
}
