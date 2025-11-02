#include <app.h>

App::App(){
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(800, 600, "Title");
	SetExitKey(0);
}

App::~App(){
	CloseWindow();
}

void App::Start(){
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		r.Update();
		r.Render();

		EndDrawing();
	}
}


