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
			ofxOscSender sender;
			sender.setup("127.0.0.1", 12345);

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
			ofxOscSender sender;
			sender.setup("127.0.0.1", 12345);

			ofxOscMessage m;
			m.setAddress("/test");
			m.addStringArg("hello position");
			sender.sendMessage(m);
		}	
		else if (receivedMessageAddress.compare("/request/play1") == 0)
		{
			testSendOscMsgBack(m, "/ack/play1");
		}		
		else if (receivedMessageAddress.compare("/request/play2") == 0)
		{
			testSendOscMsgBack(m, "/ack/play2");
		}
		else if (receivedMessageAddress.compare("/request/play4") == 0)
		{
			testSendOscMsgBack(m, "/ack/play4");
		}
		else if (receivedMessageAddress.compare("/request/play5") == 0)
		{
			testSendOscMsgBack(m, "/ack/play5");
		}
		else if (receivedMessageAddress.compare("/request/play5") == 0)
		{
			testSendOscMsgBack(m, "/ack/play5");
		}
		else if (receivedMessageAddress.compare("/request/play") == 0)
		{
			testSendOscMsgBack(m, "/ack/play");
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


			// send message back
			string remoteHost = m.getRemoteHost();
			string remoteIp = m.getRemoteIp();
			int remotePort = m.getRemotePort();

			cout << "remote host: " + remoteHost << endl;
			cout << "remote ip: " + remoteIp << endl;
			cout << "remote port: " + ofToString(remotePort) << endl;

			ofxOscSender sender;
			sender.setup(remoteHost, remotePort);

			ofxOscMessage outMsg;
			string outGoingMsgAddr = "/ack";
			outMsg.setAddress(outGoingMsgAddr);
			outMsg.addStringArg("ack");
			sender.sendMessage(outMsg);
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





void ofApp::testSendOscMsgBack(ofxOscMessage receivedMsg, string outGoingMsgAddr) {
	string message = receivedMsg.getArgAsString(0);
	cout << "incoming message: " + message << endl;
	cout << "outgoing message address: " + outGoingMsgAddr << endl;

	string remoteHost = receivedMsg.getRemoteHost();
	string remoteIp = receivedMsg.getRemoteIp();
	int remotePort = receivedMsg.getRemotePort();

	cout << "remote host: " + remoteHost << endl;
	cout << "remote ip: " + remoteIp << endl;
	cout << "remote port: " + ofToString(remotePort) << endl;


	// send message back
	ofxOscSender sender;
	sender.setup(remoteHost, remotePort);

	ofxOscMessage m;
	m.setAddress(outGoingMsgAddr);
	m.addStringArg(message + "_ack");
	sender.sendMessage(m);
}
