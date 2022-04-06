#pragma once
#include "Renderer.h"
class SimulationRenderer :
    public Renderer
{
public:
    SimulationRenderer();
    void Render() override;
private:
};

