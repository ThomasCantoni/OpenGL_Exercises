#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include "OGLWindow.h"
#include "Ex01TriangleDraw.h"
#include "Ex02QuadDraw.h"
#include "Ex03QuadIndexDraw.h"
#include "Ex04QuadColor.h"
#include "Ex05QuadTextureDraw.h"
#include "Ex06QuadPerspDraw.h"
#include "Ex07CubePerspDraw.h"
#include "Ex08PhongDraw.h"
#include "Ex09InstancingDraw.h"
#include "Ex10Exercise.h"
#include "Ex11Cubelight.h"
#include "Ex12OrbitingCubes.h"

#ifdef _WIN32
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif

int main() {
	std::cout << "It's Working" << std::endl;

	OGLWindow Win{600, 400, "OpenGL Window"};

	Win.PrintInfo();

	float TimeElapsed = 0.f;
	
	//Ex01TriangleDraw Scene;
	//Ex02QuadDraw Scene;
	//Ex03QuadIndexDraw Scene;
	//Ex04QuadColor Scene;
	//Ex05QuadTextureDraw Scene;
	//Ex06QuadPerspDraw Scene;
	//Ex07CubePerspDraw Scene;
	//Ex08PhongDraw Scene;
	//Ex09InstancingDraw Scene;
	//Ex10Exercise Scene;
	//Ex11Cubelight Scene;
	Ex12OrbitingCubes Scene;

	Scene.Start();

	while(Win.IsOpened()) 
	{
		float DeltaTime = Win.GetDeltaTime();
		TimeElapsed += DeltaTime;
		if (TimeElapsed >= 1.f)
		{
			TimeElapsed -= 1.f;
			float Fps = 1.f / DeltaTime;
			std::string Title = std::format("OpenGL App | DeltaTime: {} - FPS: {}", DeltaTime, Fps);
			Win.SetTitle(Title);
		}

		Scene.Update(DeltaTime);

		Win.Update();
	}
	Scene.Destroy();
	return 0;	
}