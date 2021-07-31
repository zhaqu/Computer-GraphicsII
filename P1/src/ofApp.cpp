#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//load the mesh and collect faces and vertices
	mesh.load("torus.ply");
	shader.load("vertex_color.vert", "vertex_color.frag");
	//mesh.enableColors();
	auto vertices = mesh.getVertices();
	auto faces = mesh.getUniqueFaces();
	//calculate the bound and center

	for (unsigned i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x > bound_x) bound_x = vertices[i].x;
		if (vertices[i].y > bound_y) bound_y = vertices[i].y;
		if (vertices[i].z > bound_z) bound_z = vertices[i].z;
		if (vertices[i].x < bound_dx) bound_dx = vertices[i].x;
		if (vertices[i].y < bound_dy) bound_dy = vertices[i].y;
		if (vertices[i].z < bound_dz) bound_dz = vertices[i].z;
		average_x += vertices[i].x;
		average_y += vertices[i].y;
		average_z += vertices[i].z;
	}
	average_x /= vertices.size();
	average_y /= vertices.size();
	average_z /= vertices.size();
	//output
	std::cout << "Num of Vertices is " << vertices.size() << endl;
	std::cout << "Num of Faces is " << faces.size() << endl;
	std::cout << "Bounding x:" << bound_x<<" to "<<bound_dx << " y:" << bound_y <<" to "\
		<<bound_dy << " z:" << bound_z << " to "<<bound_dz<< endl;
	std::cout << "the center is (" << average_x << "," << average_y << "," << average_z << ")" << endl;
	//build model matrix
	glm::mat4 model = glm::mat4(1.0);
	
	//calculate lookat matrix
	view = glm::lookAt(eye, center, up);
	//calculate projection matrix
	projection = glm::perspective(glm::radians(60.0f), (float)ofGetHeight() / (float)ofGetWidth(), 0.1f, 1000.0f);
	pvm = projection * view * model;
	vm = view * model;
	normal_matrix = glm::transpose(glm::inverse(view*model));
	


	//light.setPosition(1, 0, -1);
	//light.setDirectional();
	//light.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
	//light.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	/*cam.setPosition(eye);
	cam.lookAt(center, up);
	cam.setNearClip(1.0f);*/
	//cam.enableMouseInput();
	//cam.enableMouseMiddleButton();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	if (changed)
	{
		model = translate_matrix * rotate_matrix * zoom_matrix * model;
		pvm = projection * view * model;
		vm = view * model;
		normal_matrix = glm::transpose(glm::inverse(view*model));
		changed = 0;
		translate_matrix = glm::mat4(1.0);
		zoom_matrix = glm::mat4(1.0);
		rotate_matrix = glm::mat4(1.0);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	//ofSetColor(100, 100, 100);
	//light.enable();
	//cam.begin();
	
	//ofColor(255, 255, 100);
	//meshNode.transformGL();
	shader.begin();
	shader.setUniformMatrix4f("pvm", pvm);
	shader.setUniformMatrix4f("vm", vm);
	shader.setUniformMatrix4f("normal_matrix", normal_matrix);
	shader.setUniform3f("l_diffuse", l_diffuse);
	//shader.setUniform4f("l_ambient", l_ambient);
	shader.setUniform3f("l_specular", l_specular);
	shader.setUniform3f("m_diffuse", m_diffuse);
	shader.setUniform3f("m_ambient", m_ambient);
	shader.setUniform3f("m_specular", m_specular);
	shader.setUniform3f("m_emission", m_emission);
	shader.setUniform1f("shininess", shininess);
	/*shader.setUniformMatrix4f("projection", projection);
	shader.setUniformMatrix4f("modelview", vm);
	shader.setUniformMatrix4f("normalMat", normal_matrix);*/
	
	mesh.draw();
	
	//light.draw();
	shader.end();
	//cam.end();
	//meshNode.restoreTransformGL();
	
	
	//light.disable();

	ofDisableDepthTest();
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
	//left click with rotation
	if (button == 0)
	{
		glm::vec3 P_d, P_c, Axis;
		float half_width = (float)ofGetWidth()/2.0;
		float half_height = (float)ofGetHeight() / 2.0;
		float x_redirct = ((float)x - half_width) / half_width;
		float y_redirct = (half_height - (float)y) / half_height;
		float x_redirct_2 = x_redirct * x_redirct;
		float y_redirct_2 = y_redirct * y_redirct;
		float length_2 = x_redirct_2 + y_redirct_2;

		float old_x_redirct = ((float)old_x - half_width) / half_width;
		float old_y_redirct = (half_height - (float)old_y) / half_height;
		float old_x_redirct_2 = old_x_redirct * old_x_redirct;
		float old_y_redirct_2 = old_y_redirct * old_y_redirct;
		float old_length_2 = old_x_redirct_2 + old_y_redirct_2;
		

		if (length_2 <= 1.0)
		{
			P_d = glm::vec3(x_redirct, y_redirct, -glm::sqrt(1.0 - x_redirct_2 - y_redirct_2));
		}
		else
		{
			P_d = glm::vec3(x_redirct / glm::sqrt(length_2), y_redirct / glm::sqrt(length_2), 0.0);
		}
		
		
		if (old_length_2 <= 1.0)
		{
			P_c = glm::vec3(
				old_x_redirct, old_y_redirct, -glm::sqrt(1.0 - old_x_redirct_2 - old_y_redirct_2));
		}
		else
		{
			P_c = glm::vec3(old_x_redirct / glm::sqrt(old_length_2), old_y_redirct / glm::sqrt(old_length_2), 0.0);
		}
		//P_c = glm::normalize(P_c);
		//P_d = glm::normalize(P_d);
		//cout << "P_d normal " << P_d << endl;
		//cout <<"P_c "<< P_c << endl;

		Axis = glm::cross(P_d, P_c);
		//cout << Axis << endl;
		//cout << "dot " << 0.8*glm::dot(P_c, P_d) << endl;
		//cout << Axis << endl;
		float angle = glm::acos(glm::dot(P_c, P_d));

		//cout << angle << endl;
		rotate_matrix = glm::rotate(angle, Axis);
		old_x = x;
		old_y = y;
		changed = 1;
		
	}
	//middle click with translate
	if (button == 1)
	{
		
		int temp_x = x - old_x;
		int temp_y = old_y - y;
		translate_matrix = glm::translate(
			translate_matrix, glm::vec3(
				5*(float)temp_x/(float)ofGetWidth(), 5*(float)temp_y/(float)ofGetHeight(), 0.0));
		old_x = x;
		old_y = y;
		//cout << old_x << " " << old_y << endl;
		changed = 1;
	}
	//right click with zoom
	if (button == 2)
	{
		float signal = 0.01;
		int temp = x - old_x + old_y - y;
		if (temp < 0)
			signal = -0.01;
		else if (temp == 0)
			signal = 0;
		else
			signal = 0.01;
		zoom_matrix = glm::scale(glm::vec3(1.0 + signal , 1.0 + signal , 1.0));
		old_x = x;
		old_y = y;
		changed = 1;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//left click with rotation
	if (button == 0)
	{
		old_x = x;
		old_y = y;
	}
	//middle click with translate
	if (button == 1)
	{
		old_x = x;
		old_y = y;
	}
	//right click with zoom
	if (button == 2)
	{
		old_x = x;
		old_y = y;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//left click with rotation
	if (button == 0)
	{
		changed = 0;
	}
	//middle click with translate
	if (button == 1)
	{
		changed = 0;
	}
	//right click with zoom
	if (button == 2)
	{
		changed = 0;
	}
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
