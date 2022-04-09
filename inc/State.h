#pragma once
#include <map>
#include <string>
#include <vector>
#include "Camera.h"
#include "Window.h"
#include "Model.h";
#include "PlaneGenerator.h"
#include "Geometry.h"
#include "Cube.h"
#include "Quad.h"
#include "Light.h"
#include "Skybox.h"
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
extern void processInput();

enum { M1, KEY_COUNT };

struct State
{
public:
	State() = default;
	State(const int& width, const int& height);
	int Init();
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_OrthogonalMatrix;
	void Update();
	bool m_KeyMap[KEY_COUNT] = { false };
	bool m_M1Prev = false;
	std::map<std::string, Model> m_Models;
	std::map<std::string, Geometry> m_Geometry;
	int m_Res;
	Light m_Light;
	void GenerateGeometriesModels();
	Texture m_HeightField;
	Texture m_CausticMap;
	Texture m_RefractionsPositions;
	Texture m_ReflectionsPositions;
	Texture m_Reflections;
	Texture m_Refractions;
	Texture m_ReceiverPositions;
	Texture m_WaterNormals;
	Texture m_Filtered;
	Texture m_RefractivePositions;
	Texture m_RefractiveNormals;
	float m_DeltaTime;
	bool m_Abort;
	glm::vec2 m_DropPos;
	Camera m_Camera;
	Window m_Window;
	bool m_DropTest;

	float m_Radius;
	float m_Amplitude;

	float m_WaveSpeed;
	float m_WaveDamping;
	float m_WaveSlope;

	float m_CausticsAbsorbtion;
	float m_CausticsPower;

	float m_FirstGuess;

	float m_FirstStage;
	float m_SecondStage;
	glm::vec3 m_FirstStageColor;
	glm::vec3 m_SecondStageColor;
	glm::vec3 m_FinalStageColor;
	float m_Attenuation;

private:
	float m_NowTime;
	float m_LastTime;
	
	float m_FOV;
	double m_LastX;
	double m_LastY;
	bool m_FirstMouse;

	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void processInput();
};

extern State state;
