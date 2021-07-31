#pragma once

#include "ofMain.h"
#include "Cylinder.h"
#include "CameraOrbit.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <glm/vec3.hpp>
#include <string.h>
#include <queue>
#define BUNNY 0;
#define COW 0;
#define LILIUM 0;
#define ROOF 0;
#define DOUBLE_TORUS 0;
#define MUSHROOM 0;
#define DISK 1;
#define AD 0;
#define LA 1;
#define LACO 0;//not finished
class HalfEdge;
class Face
{
public:
    HalfEdge* edge;
    int index;
    std::vector<HalfEdge* > face_Edges();
};

class Vertex
{
public:
    glm::vec3 pos;
    HalfEdge* edge;
    int index;
    int next_point[20] = {-1};
    std::vector<HalfEdge* > vertex_Edges();
	float Vertex::vertex_color();
	float Vertex::vertex_LA_color();
};

class HalfEdge
{
public:
    HalfEdge* opposite;
    HalfEdge* next;
    Vertex* vert;
    Vertex* arrived;
    Face* face;  
    int index;
};



class Mesh
{
public:
    std::vector<Face* > face;
    std::vector<Vertex* > vertex;
    std::vector<HalfEdge* > half_edge;
    std::vector<HalfEdge* > boundary_edge;
    std::vector<std::vector<HalfEdge* > > Boundaries();
    void InsertVertex(float x, float y, float z, int index);
    void InsertFace();
    void BuildFromObj(); 
    void ObjScan(const char* name);
    std::vector<HalfEdge*> ShortestPath(Vertex* v_source, Vertex* v_target);
};



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofMesh mesh;
		vector<ofMesh> edge_mesh;
		ofShader edge_shader;
		ofNode meshNode;
		float bound_x = 0, bound_y = 0, bound_z = 0;
		float bound_dx = 0, bound_dy = 0, bound_dz = 0;
		float average_x = 0, average_y = 0, average_z = 0;
		float shininess = 64.0;
		glm::vec3 eye = glm::vec3(0.0, 0.0, 5.0);
		glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 l_diffuse = glm::vec3(1.0, 0.0, 0.0);
		glm::vec3 l_specular = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 m_diffuse = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 m_ambient = glm::vec3(0.0, 1.0, 1.0);
		glm::vec3 m_specular = glm::vec3(0.0, 1.0, 1.0);
		glm::vec3 m_emission = glm::vec3(0.0, 0.0, 1.0);
		glm::mat4 translate_matrix, zoom_matrix, rotate_matrix = glm::mat4(1.0);
		glm::mat4 view, projection, model, pvm, normal_matrix, vm;
		int old_x, old_y;
		ofShader shader;
};
