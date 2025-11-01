#include <raylib.h>
#include <renderer.h>

int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 600, "Title");
	SetExitKey(0);
	Renderer r;
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(0, 0, GetScreenWidth(), 20, DARKGRAY);

		r.Update();
		r.Render();

		EndDrawing();
	}
    return 0;
}
