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


const int dquadNAttribsPerVertex = 8;
const int dquadNVertices = 4;
const int dquadNTriangles = 2;
const float dquadVertices[] = {
  0.75f,0.75f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  1.0f,0.75f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
  0.75f,1.0f,0.0f,-0.0f,0.0f,1.0f,0.0f,1.0f,
}; // end planeVertices

const unsigned dquadTriangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end planeTriangles

const int dlquadNAttribsPerVertex = 8;
const int dlquadNVertices = 4;
const int dlquadNTriangles = 2;
const float dlquadVertices[] = {
  -1.0f,0.75f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  -0.75f,0.75f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  -0.75f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
  -1.0f,1.0f,0.0f,-0.0f,0.0f,1.0f,0.0f,1.0f,
}; // end planeVertices

const unsigned dlquadTriangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end planeTriangles

