#include <renderer.h>
#include <objects.h>
#include <cmath>
#include <iostream>
#include <raylib.h>


Renderer::Renderer() {
    text.count = 1;
    text.lines = (Line*)malloc(sizeof(Line));
    text.lines[0].length = 0;

    text.lines[0].text = (char*)malloc(1);
    text.lines[0].text[0] = '\0';
}


void Renderer::Render(){
	std::cout << text.count << std::endl;
	for (int line = 0; line < text.count; ++line){
		Line* currLine = &text.lines[line];
		for (int chr = 0; chr < currLine->length; ++chr){
			char str[] = {currLine->text[chr], '\0'};
			DrawText(str, 20+(chr*15), 20+(line*20), 20, WHITE);
		}
	}
	return;
}

void Renderer::Update(){
	MoveCursor();
	int key = GetCharPressed();
    if (key >= 32 && key <= 126) { 
        text.AddChar(text.count-1, text.lines[text.count-1].length, (char)key);
    }
	key = GetKeyPressed();
	if (key == KEY_BACKSPACE){
		if (text.lines[text.count-1].length > 0) {
            text.RemoveChar(text.count-1, text.lines[text.count-1].length - 1);
        } else if (text.count-1 > 0) {
            text.count--;
        }
	}

	if (key == KEY_ENTER){
        text.NewLine();
	}
	
	return;
}

void Renderer::MoveCursor(){
}
