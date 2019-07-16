//
//  ofxRealSense2.cpp
//
//  Created by Petr Devaikin on 2019/06/13
//

#include "ofxRealSense2.hpp"

bool ofxRealSense2::setup(int width, int height, bool enableColor, bool enableIr, bool enableDepth, bool alignToColor) {
    this->width = width;
    this->height = height;
    this->enableColor = enableColor;
    this->enableIr = enableIr;
    this->enableDepth = enableDepth;
    this->alignToColor = alignToColor;
    
    // Allocate buffers
    
    if (enableColor) {
        lastColorPixels.allocate(width, height, 3);
        cfg.enable_stream(RS2_STREAM_COLOR, width, height);
    }
    if (enableIr) {
        lastIrPixels.allocate(width, height, 1);
        cfg.enable_stream(RS2_STREAM_INFRARED, width, height);
    }
    if (enableDepth) {
        lastDepthPixels.allocate(width, height, 1);
        cfg.enable_stream(RS2_STREAM_DEPTH, width, height);
    }
    
    // Looking for devices
    rs2::context ctx;
    auto device_list = ctx.query_devices();
    
    found = device_list.size();
    
    return found;
}

void ofxRealSense2::start() {
    rs2::pipeline_profile profile = pipe.start(cfg);
    auto sensor = profile.get_device().first<rs2::depth_sensor>();
    
    depthScale = sensor.get_depth_scale();
    
    // depth sensor settings
    sensor.set_option(RS2_OPTION_VISUAL_PRESET, RS2_RS400_VISUAL_PRESET_DEFAULT);
    //sensor(RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0.f);
    //sensor(RS2_OPTION_EXPOSURE, 30000);
    
    newFramesArrived = false;
}

void ofxRealSense2::stop() {
    pipe.stop();
}

bool ofxRealSense2::isConnected() {
    return found;
}

void ofxRealSense2::update() {
    if (pipe.poll_for_frames(&newFrames)) {
        newFramesArrived = true;
        
        newFrames = temp_filter.process(newFrames);
        newFrames = hole_filter.process(newFrames);
        if (enableColor && alignToColor) {
            newFrames = alignerToColor.process(newFrames);
        }
        
        // Copy data from camera
        if (enableDepth) {
            memcpy(lastDepthPixels.getData(), newFrames.get_depth_frame().get_data(), width * height * 2);
        }
        if (enableIr) {
            memcpy(lastIrPixels.getData(), newFrames.get_infrared_frame().get_data(), width * height);
        }
        if (enableColor) {
            memcpy(lastColorPixels.getData(), newFrames.get_color_frame().get_data(), width * height * 3);
        }
    }
}

bool ofxRealSense2::hasNewFrames() {
    return newFramesArrived;
}

ofPixels& ofxRealSense2::getColorPixels() {
    newFramesArrived = false;
    return lastColorPixels;
}

ofPixels& ofxRealSense2::getIrPixels() {
    newFramesArrived = false;
    return lastIrPixels;
}

ofShortPixels& ofxRealSense2::getDepthPixels() {
    newFramesArrived = false;
    return lastDepthPixels;
}

float ofxRealSense2::getDepthScale() {
    return depthScale;
}

float ofxRealSense2::getWidth() {
    return width;
}

float ofxRealSense2::getHeight() {
    return height;
}
