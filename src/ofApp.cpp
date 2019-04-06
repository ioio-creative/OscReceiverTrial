#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	strcpy(mouseButtonState, "");

	ofBackground(30, 30, 130);


	// sender
	sender.setup("10.0.1.40", 12345);
}

//--------------------------------------------------------------
void ofApp::update(){
	// hide old messages
	for (int i = 0; i<NUM_MSG_STRINGS; i++)
	{
		if (timers[i] < ofGetElapsedTimef())
			msg_strings[i] = "";
	}

	// check for waiting messages
	while (receiver.hasWaitingMessages())
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		string receivedMessageAddress = m.getAddress();

		// check for mouse moved message
		if (receivedMessageAddress.compare("/mouse/position") == 0)
		{
			// both the arguments are int32's
			mouseX = m.getArgAsInt32(0);
			mouseY = m.getArgAsInt32(1);

			// send message back
			ofxOscMessage m;
			m.setAddress("/test");
			m.addStringArg("hello position");
			sender.sendMessage(m);
		}
		// check for mouse button message
		else if (receivedMessageAddress.compare("/mouse/button") == 0)
		{
			// the single argument is a string
			strcpy(mouseButtonState, m.getArgAsString(0).c_str());

			
			// send message back
			ofxOscMessage m;
			m.setAddress("/test");
			m.addStringArg("hello button");
			sender.sendMessage(m);
		}
		else
		{
			// unrecognized message: display on the bottom of the screen
			char msg_string[16384];
			strcpy(msg_string, receivedMessageAddress.c_str());
			strcat(msg_string, ": ");
			for (int i = 0; i<m.getNumArgs(); i++)
			{
				// get the argument type
				strcat(msg_string, m.getArgTypeName(i).c_str());
				strcat(msg_string, ":");
				// display the argument - make sure we get the right type
				if (m.getArgType(i) == OFXOSC_TYPE_INT32)
					sprintf(msg_string, "%s%d ", msg_string, m.getArgAsInt32(i));
				else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
					sprintf(msg_string, "%s%f ", msg_string, m.getArgAsFloat(i));
				else if (m.getArgType(i) == OFXOSC_TYPE_STRING)
					sprintf(msg_string, "%s\"%s\" ", msg_string, m.getArgAsString(i));
				else
					strcat(msg_string, "unknown");
			}

			cout << msg_string << endl;
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	char buf[256];
	sprintf(buf, "listening for osc messages on port %d", PORT);
	ofDrawBitmapString(buf, 10, 20);

	// draw mouse state
	sprintf(buf, "mouse: % 4d % 4d", mouseX, mouseY);
	ofDrawBitmapString(buf, 430, 20);
	ofDrawBitmapString(mouseButtonState, 580, 20);


	for (int i = 0; i<NUM_MSG_STRINGS; i++)
	{
		ofDrawBitmapString((char*)msg_strings[i].c_str(), 10, 40 + 15 * i);
	}
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
