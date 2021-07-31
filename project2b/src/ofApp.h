#pragma once

#include "ofMain.h"
#include "ofxEasyCubemap.h"
#include "CameraOrbit.h"
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
		ofMesh bunny;
		ofMesh sky;
		glm::mat4 view, projection, model, model_sky;
		ofShader bunny_shader;
		ofShader sky_shader;
		ofxEasyCubemap cube_map;
		CameraOrbit cam;
		glm::mat4 translate_matrix, rotate_matrix, zoom_matrix;
		int old_x, old_y;
		bool changed = 0;
};
