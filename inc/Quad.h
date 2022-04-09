#pragma once

const int quadNAttribsPerVertex = 8;
const int quadNVertices = 4;
const int quadNTriangles = 2;
const float quadVertices[] = {
	// positions   // texCoords
  -1.0f,1.0f,0.0f,-0.0f,0.0f,1.0f,0.0f,1.0f,
  -1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
  
}; // end planeVertices

const unsigned quadTriangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end planeTriangles
