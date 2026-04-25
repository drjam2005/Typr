#include <raylib.h>
#include <game.h>

int main(){
	InitWindow(WIDTH, HEIGHT, "Typr");
	//SetTargetFPS(60);

	WordList words;
//	{
//		words.add_word("test");
//	}
	Game game(words);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		game.Loop();
		DrawFPS(20, 20);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
