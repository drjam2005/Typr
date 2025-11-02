#include <renderer.h>
#include <cmath>
#include <objects.h>
#include <stdlib.h>
#include <raylib.h>

Renderer::Renderer() {
    text.count = 1;
    text.lines = (Line*)malloc(sizeof(Line));
    text.lines[0].length = 0;

    text.lines[0].text = (char*)malloc(1);
    text.lines[0].text[0] = '\0';
}


void Renderer::Render() {
    DrawRectangle(0, 0, GetScreenWidth(), 35, GRAY);
    DrawRectangle(0, 30, GetScreenWidth(), 5, DARKGRAY);

    for (int line = 0; line < text.count; ++line) {
        Line* currLine = &text.lines[line];
        int posY = 50 + (line * 20);

        bool inVisualLine = false;
        if (mode == VISUAL_LINE) {
            int top = fmin(visualStart.y, visualEnd.y);
            int bottom = fmax(visualStart.y, visualEnd.y);
            inVisualLine = (line >= top && line <= bottom);
        }

        if (inVisualLine) {
            DrawRectangle(28, posY - 2, GetScreenWidth() - 56, 20, BLUE);
        }

        for (int chr = 0; chr < currLine->length; ++chr) {
            char str[] = { currLine->text[chr], '\0' };
            int posX = 30 + (chr * 15);

            Color charColor = WHITE;
            if (index.x == chr && index.y == line) {
                DrawRectangle(posX - 2, posY - 2, 15, 20, SKYBLUE);
                charColor = BLACK;
            }

            DrawText(str, posX, posY, 20, charColor);
        }

        if (index.x == currLine->length && index.y == line) {
            int posX = 30 + (currLine->length * 15);
            DrawRectangle(posX - 2, posY - 2, 15, 20, GRAY);
        }
    }
}

void Renderer::Update() {
	switch(mode){
		case NORMAL:
			ParseNormal();
			break;
		case INSERT:
			ParseInsert();
			break;
		case VISUAL:
			ParseVisual();
			break;
		case VISUAL_LINE:
			ParseVisualLine();
			break;
		case COMMAND:
			break;
	}
}

void Renderer::ParseInsert(){
	int key = GetCharPressed();
	if (key >= 32 && key <= 126) {
		text.AddChar(index.y, index.x, (char)key);
		index.x++;
	}

	key = GetKeyPressed();
	if (key == KEY_TAB){
		text.AddChar(index.y, index.x, ' ');
		index.x++;
		text.AddChar(index.y, index.x,' ');
		index.x++;
		text.AddChar(index.y, index.x,' ');
		index.x++;
		text.AddChar(index.y, index.x,' ');
		index.x++;
		return;
	}
	if (key == KEY_ESCAPE){
		if(index.x)
			index.x--;
		mode = NORMAL;
		return;
	}
	if (key == KEY_BACKSPACE) {
		if (index.x > 0) {
			text.RemoveChar(index.y, index.x - 1);
			index.x--;
		} else if (index.y > 0) {
			int prevLen = text.lines[(int)index.y - 1].length;
			text.MergeLines(index.y - 1, index.y);
			index.y--;
			index.x = prevLen;
		}
	}
	if (key == KEY_W){
		if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
			while (index.x > 0) {
				index.x--;
				char c = text.lines[(int)index.y].text[(int)index.x];
				text.RemoveChar(index.y, index.x);

				if (c == ' ' || c == '.' || c == ';') {
					break;
				}

				if (index.x == 0) {
					break;
				}
			}
		}
	}

	if (key == KEY_ENTER) {
		text.SplitLine(index.y, index.x);
		index.y++;
		index.x = 0;
	}
}

void Renderer::ParseNormal(){
	int key = GetKeyPressed();
	// NORMAL STUFF
	if(key == KEY_H){
		if(index.x)
			index.x--;
	}
	if(key == KEY_L){
		if(index.x < text.lines[(int)index.y].length-1)
			index.x++;
	}
	if(key == KEY_J){
		if(index.y+1 < text.count){
			index.y++;
		}
		int comp = text.lines[(int)index.y].length-1;
		if(index.x > comp)
			index.x = comp;
		if(comp+1 == 0)
			index.x = 0;
	}
	if(key == KEY_K){
		if(index.y)
			index.y--;
		int comp = text.lines[(int)index.y].length-1;
		if(index.x > comp)
			index.x = comp;
		if(comp+1 == 0)
			index.x = 0;
	}

	if(key == KEY_X){
		text.RemoveChar(index.y, index.x);
		if(index.x == text.lines[(int)index.y].length)
			index.x--;
	}

	if(key == KEY_V){
		visualStart = index;
		mode = VISUAL_LINE;
	}

	// INSERT STUFF
	if(key == KEY_I){
		mode = INSERT;
	}
	if(key == KEY_A){
		mode = INSERT;
		if(index.x)
			index.x++;
		if(IsKeyDown(KEY_LEFT_SHIFT)){
			index.x = text.lines[(int)index.y].length;
		}
	}
	if(key == KEY_O){
		mode = INSERT;
		if(IsKeyDown(KEY_LEFT_SHIFT)){
			if(!(index.y))
				return;
			text.SplitLine(--index.y, text.lines[(int)index.y+1].length);
			index.x = 0; index.y++;
		}else{
			text.SplitLine(index.y, text.lines[(int)index.y].length);
			index.x = 0; index.y++;
		}
	}
}

void Renderer::ParseVisual(){
	if(IsKeyPressed(KEY_ESCAPE))
		mode = NORMAL;
	return;
}

void Renderer::ParseVisualLine(){
	ParseNormal();
	visualEnd = index;
	if(IsKeyPressed(KEY_ESCAPE))
		mode = NORMAL;
	if(IsKeyPressed(KEY_D)){
		text.RemoveLines(fmin(visualStart.y, visualEnd.y), fabs(visualEnd.y - visualStart.y)+1);
		mode = NORMAL;
		if (text.count == 0) {
            text.NewLine();
            index = {0, 0};
        } else {
            index.y = fmin(visualStart.y, visualEnd.y)-1;
            index.x = fmin(index.x, text.lines[(int)index.y].length);
        }
	}
	return;
}
