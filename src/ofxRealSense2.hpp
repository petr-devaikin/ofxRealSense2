//
//  ofxLibRealSense2.hpp
//
//  Created by Petr Devaikin on 2019/06/13
//

#pragma once
#include <librealsense2/includes/rs.hpp>
#include "ofMain.h"

class ofxRealSense2 {
public:
    bool setup(int width = 1280, int height = 720, bool enableColor = true, bool enableIr = true, bool enableDepth = true);
    void start();
    void stop();
    
    void update();
    
    bool hasNewFrames();
    
    ofPixels& getColorPixels();
    ofPixels& getIrPixels();
    ofPixels& getDepthPixels();
    
    float getDepthScale();
private:
    int width;
    int height;
    
    bool enableColor;
    bool enableIr;
    bool enableDepth;
    
    rs2::frameset newFrames;
    bool newFramesArrived;
    
    rs2::config cfg;
    rs2::depth_sensor sensor;
    
    rs2::pipeline pipe;
    rs2::frameset frames;
    
    rs2::temporal_filter temp_filter;
    rs2::hole_filling_filter hole_filter;
    
    ofPixels lastColorPixels;
    ofPixels lastIrPixels;
    ofPixels lastDepthPixels;
};