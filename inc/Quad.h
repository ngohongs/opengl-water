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

const int ddlquadNAttribsPerVertex = 8;
const int ddlquadNVertices = 4;
const int ddlquadNTriangles = 2;
const float ddlquadVertices[] = {
  -1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  -0.75f,-1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  -0.75f,-0.75f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
  -1.0f,-0.75f,0.0f,-0.0f,0.0f,1.0f,0.0f,1.0f,
}; // end planeVertices

const unsigned ddlquadTriangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end planeTriangles

const int ddquadNAttribsPerVertex = 8;
const int ddquadNVertices = 4;
const int ddquadNTriangles = 2;
const float ddquadVertices[] = {
  0.75f,-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  1.0f,-0.75f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
  0.75f,-0.75f,0.0f,-0.0f,0.0f,1.0f,0.0f,1.0f,
}; // end planeVertices

const unsigned ddquadTriangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end planeTriangles
