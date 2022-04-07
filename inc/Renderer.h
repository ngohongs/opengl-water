#pragma once

#include "State.h"

#include "RenderTarget.h"


#include "Texture.h"

#include "Shader.h"


class Renderer
{
public:
	virtual void Render() = 0;
	virtual const Texture& Result() = 0;
private:
};

