#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	this->number_of_targets = 39;

	for (int i = 0; i < this->number_of_targets; i++) {
	
		this->circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
		this->circles.back().get()->setPhysics(1.5, 0.5, 0.1);
		this->circles.back().get()->setup(this->box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 15);

		int color_type = ofRandom(3);
		glm::vec4 color;
		switch (color_type) {
		case 0:
			color = glm::vec4(1.0, 0.0, 0.0, 0.0);
			break;
		case 1:
			color = glm::vec4(0.0, 1.0, 0.0, 0.0);
			break;
		case 2:
			color = glm::vec4(0.0, 0.0, 1.0, 0.0);
			break;
		}
		this->colors.push_back(color);
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");

	this->force_fields.push_back(ofPoint(ofGetWidth() / 4, ofGetHeight() / 4));
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4 * 3, ofGetHeight() / 4));
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4 * 3, ofGetHeight() / 4 * 3));
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4, ofGetHeight() / 4 * 3));

	this->force_field_radius = 300;
}
//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->circles.size(); i++) {

		shared_ptr<ofxBox2dCircle> circle = this->circles[i];
		ofPoint point = circle->getPosition();

		for (int field_index = 0; field_index < this->force_fields.size(); field_index++) {

			float distance = point.distance(this->force_fields[field_index]);
			if (distance < this->force_field_radius) {

				ofPoint p = ofPoint(this->force_field_radius * cos((field_index * 90 + 10) * DEG_TO_RAD), this->force_field_radius * sin((field_index * 90 + 10) * DEG_TO_RAD));
				circle->addForce(p, ofMap(distance, 0, this->force_field_radius, 0.1, 0.01));
			}
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	vector<glm::vec4> points;
	for (int i = 0; i < this->number_of_targets; i++) {
		
		glm::vec4 p = glm::vec4(this->circles[i]->getPosition().x, this->circles[i]->getPosition().y, 0, 0);
		points.push_back(p);
	}

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform4fv("targets", &points[0].x, this->number_of_targets);
	this->shader.setUniform4fv("colors", &this->colors[0].x, this->number_of_targets);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();

}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}