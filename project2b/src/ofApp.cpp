#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	bunny.load("bunny.ply");
	bunny_shader.load("bunny.vert", "bunny.frag");
	sky.load("cube.ply");
	sky_shader.load("sky.vert", "sky.frag");
	cam.Zoom(3);
	translate_matrix = glm::translate(-bunny.getCentroid());
	rotate_matrix = glm::rotate(glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	model = rotate_matrix * translate_matrix;
	cube_map.load("nvlobby_front.jpg", "nvlobby_back.jpg", "nvlobby_right.jpg", "nvlobby_left.jpg", "nvlobby_top.jpg", "nvlobby_bottom.jpg");
	model_sky = glm::scale(glm::vec3(6.0, 6.0, 6.0));
	cam.RotateHorizotal(0.3);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	
	sky_shader.begin();
	glDepthFunc(GL_LEQUAL);
	sky_shader.setUniformMatrix4f("model", model_sky);
	sky_shader.setUniformMatrix4f("view", cam.ViewMatrix());
	sky_shader.setUniformMatrix4f("projection", cam.ProjectionMatrix());
	sky_shader.setUniformTexture("envMap", cube_map.getTexture(), 0);
	sky.draw();
	sky_shader.end();
	
	bunny_shader.begin();
	glDepthFunc(GL_LESS);
	bunny_shader.setUniformMatrix4f("model", model);
	bunny_shader.setUniformMatrix4f("view", cam.ViewMatrix());
	bunny_shader.setUniformMatrix4f("projection", cam.ProjectionMatrix());
	bunny_shader.setUniform3f("cam_pos", cam.Pos());
	bunny_shader.setUniformTexture("envMap", cube_map.getTexture(), 1);
	bunny.draw();
	bunny_shader.end();
}	
	

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (button == 2)
	{
		int temp = x - old_x + old_y - y;
		if (temp < 0)
			cam.Zoom(-0.1);
		else if (temp > 0)
			cam.Zoom(0.1);
		else
			;
		old_x = x;
		old_y = y;
		changed = 1;
	}
	if (button == 0)
	{
		float x_degree = (x - old_x) * 0.001;
		float y_degree = (y - old_y) * 0.001;
		old_x = x;
		old_y = y;
		cam.RotateHorizotal(x_degree);
		cam.RotateVertical(y_degree);
		changed = 1;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	old_x = x;
	old_y = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	changed = 0;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
