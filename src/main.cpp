#include <raylib.h>
#include <renderer.h>

int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 600, "Title");
	Renderer r;
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(20, 20);
		r.Update();
		r.Render();

		EndDrawing();
	}
    return 0;
}
