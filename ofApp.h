#pragma once

#include "ofMain.h"
#include <librealsense2/rs.hpp>

using namespace rs2;

class ofApp : public ofBaseApp{
    private:
        pipeline pipe;
        frameset frames;
        ofImage rgbChannel;
        ofImage irChannel;
        ofImage depthChannel;
    
        int width = 1280;
        int height = 720;
	public:
		void setup();
		void update();
		void draw();
};
