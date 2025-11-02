#pragma once
#include <raylib.h>
#include <renderer.h>

class App{
	Renderer r;
public:
	App();
	~App();

	void Start();
};

