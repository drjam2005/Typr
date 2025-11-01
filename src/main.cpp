#include <raylib.h>

int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 600, "Title");
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(20, 20);
		DrawText("Hello, World!", 20, 50, 50, WHITE);

		EndDrawing();
	}
    return 0;
}
