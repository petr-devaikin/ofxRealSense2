//
//  ofxLibRealSense2.hpp
//
//  Created by Petr Devaikin on 2019/06/13
//

#pragma once
#include <librealsense2/includes/rs.hpp>
#include "ofMain.h"

class ofxRealSense2 {
private:
    bool connected;
    
    int width;
    int height;
    
    float depthScale;
    
    bool enableColor;
    bool enableIr;
    bool enableDepth;
    
    bool alignToColor;
    
    rs2::frameset newFrames;
    bool newFramesArrived;
    
    rs2::config cfg;
    
    rs2::pipeline pipe;
    rs2::frameset frames;
    
    rs2::temporal_filter temp_filter;
    rs2::hole_filling_filter hole_filter;
    rs2::align alignerToColor;
    
    ofPixels lastColorPixels;
    ofPixels lastIrPixels;
    ofShortPixels lastDepthPixels;
public:
    ofxRealSense2() : alignerToColor(RS2_STREAM_COLOR) { };
    
    bool setup(int width = 1280, int height = 720, bool enableColor = true, bool enableIr = true, bool enableDepth = true, bool alignToColor = true);
    void start();
    void stop();
    
    bool isConnected();
    
    bool found = false;
    
    void update();
    
    bool hasNewFrames();
    
    ofPixels& getColorPixels();
    ofPixels& getIrPixels();
    ofShortPixels& getDepthPixels();
    
    float getDepthScale();
};
