#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/intersect.hpp"
#include "State.h"
#include "Shader.h"
#include "Cube.h"
#include "Quad.h"
#include "Geometry.h"
#include "PlaneGenerator.h"
#include "Crosshair.h"
#include "Skybox.h"
#include "Light.h"

class Application
{
public:
	Application(const int& width, const int& height);
	int Init();
	int Run();
	static bool RayCast(double mouseX, double mouseY, glm::vec2& q);
private:
	
};