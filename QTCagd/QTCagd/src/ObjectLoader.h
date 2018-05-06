#pragma once

#include "GLSLProgram.h"

struct HE_vert;
struct HE_face;
struct HE_edge {
	HE_vert* vert; // start-vertex of the half-edge
	HE_edge* pair; // oppositely oriented adjacent half-edge 
	HE_face* face; // face the half-edge borders
	HE_edge* next; // next half-edge around the face
};
struct HE_vert {
	float x;
	float y;
	float z;
	HE_edge* edge; // one of the half-edges emanating from the vertex
	int valence;
	glm::vec3* normal;
	glm::vec2* texture;
};
struct HE_face{
	HE_edge* edge; // one of the half-edges bordering the face
	int valence;
};

bool loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<GLuint> &out_indices, std::vector<HE_vert*> &out_HEvertices, std::vector<HE_face*> &out_HEfaces, std::vector<HE_edge*> &out_HEedges, std::vector<float> &bbox);

bool loadMayaOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<GLuint> &out_indices, std::vector<HE_vert*> &out_HEvertices, std::vector<HE_face*> &out_HEfaces, std::vector<HE_edge*> &out_HEedges, std::vector<float> &bbox);

bool loadBezier(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<GLuint> &out_indices, std::vector<float> &bbox);
