#pragma once

#include "ofMain.h"
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
		ofNode meshNode;
		float bound_x = 0, bound_y = 0, bound_z = 0;
		float bound_dx = 0, bound_dy = 0, bound_dz = 0;
		float average_x = 0, average_y = 0, average_z = 0;
		float shininess = 64.0;
		glm::vec3 eye = glm::vec3(0.0, 0.0, 2.0);
		glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 l_diffuse = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 l_specular = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 m_diffuse = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 m_ambient = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 m_specular = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 m_emission = glm::vec3(0.0, 0.0, 0.0);
		glm::mat4 translate_matrix, zoom_matrix, rotate_matrix = glm::mat4(1.0);
		glm::mat4 view, projection, model, pvm, normal_matrix, vm;
		int old_x, old_y;
		ofShader shader;
		ofImage diffuse_Tex;
		ofImage normal_Tex;
		ofImage specular_Tex;
		bool changed = 0;
};
