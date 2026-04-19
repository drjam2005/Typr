#include <raylib.h>
#include <game.h>

int main(){
	InitWindow(WIDTH, HEIGHT, "Typr");
	SetTargetFPS(60);

	WordList words;
	{
		words.add_word("the");
		words.add_word("quick");
		words.add_word("brown");
		words.add_word("fox");
		words.add_word("jumps");
		words.add_word("over");
		words.add_word("the");
		words.add_word("lazy");
		words.add_word("dog");
		words.add_word("the");
		words.add_word("quick");
		words.add_word("brown");
		words.add_word("fox");
		words.add_word("jumps");
		words.add_word("over");
		words.add_word("the");
		words.add_word("lazy");
		words.add_word("dog");
	}

	Game game(words);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		game.Loop();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
