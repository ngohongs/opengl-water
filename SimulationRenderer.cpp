#include "SimulationRenderer.h"

SimulationRenderer::SimulationRenderer()
{
	int res = 100;
	RenderTarget heightField[2] = { {res, res, COLOR, LINEAR}, {res, res, COLOR, LINEAR} };
	RenderTarget dropped = { res, res, COLOR, LINEAR };
}

void SimulationRenderer::Render()
{
	int i = 1;
}
