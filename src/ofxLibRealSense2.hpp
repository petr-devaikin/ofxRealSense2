//
//  ofxLibRealSense2.hpp
//
//  Created by Petr Devaikin on 2019/06/13
//

#pragma once
#include <librealsense2/includes/rs.hpp>
#include "ofMain.h"

enum RealSenseChannel {
    Color = 0,
    IR = 1 << 1,
    Depth = 1 << 2
}

class ofxLibRealSense2 {
public:
    void setup(int width = 1280, int height = 720, RealSenseChannel channels = (Color | IR | Depth));
    bool start();
    void stop();
    
    &ofPixels getColorPixels();
    &ofPixels getIrPixels();
    &ofPixels getDepthPixels();
private:
    float cameraDepthScale;
    
    rs2::pipeline pipe;
    rs2::frameset frames;
    
    rs2::temporal_filter temp_filter;
    rs2::hole_filling_filter hole_filter;
    
    ofPixels lastColorPixels;
    ofPixels lastIrPixels;
    ofPixels lastDepthPixels;
};
