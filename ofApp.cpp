#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(width, height);
    irChannel.allocate(width, height, OF_IMAGE_GRAYSCALE);
    rgbChannel.allocate(width, height, OF_IMAGE_COLOR);
    depthChannel.allocate(width, height, OF_IMAGE_GRAYSCALE);
    
    //Create a configuration for configuring the pipeline with a non default profile
    rs2::config cfg;
    
    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_INFRARED, width, height, RS2_FORMAT_Y8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, 30);
    cfg.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_BGR8, 30);
    
    //Instruct pipeline to start streaming with the requested configuration
    pipe.start(cfg);
    
    frames = pipe.wait_for_frames();
}

//--------------------------------------------------------------
void ofApp::update(){
    frameset newFrames;
    if (pipe.poll_for_frames(&newFrames))
        frames = newFrames;
    
    // infrared
    video_frame irFrame = frames.get_infrared_frame();
    memcpy(irChannel.getPixels().getData(), irFrame.get_data(), width * height);
    irChannel.update();
    
    // rgb
    frame rgbFrame = frames.get_color_frame();
    const unsigned char * ptr = (unsigned char *)rgbFrame.get_data();
    for (int i = 0; i < width * height; i++) {
        *(rgbChannel.getPixels().getData() + 3 * i) = *(ptr + 3 * i + 2);
        *(rgbChannel.getPixels().getData() + 3 * i + 1) = *(ptr + 3 * i + 1);
        *(rgbChannel.getPixels().getData() + 3 * i + 2) = *(ptr + 3 * i + 0);
    }
    rgbChannel.update();
    
    // depth
    depth_frame depthFrame = frames.get_depth_frame();
    ptr = (unsigned char *)depthFrame.get_data();
    for (int i = 0; i < width * height; i++) {
        unsigned short d = *((unsigned short *)(ptr + 2 * i)) * 10 / 255;
        *(depthChannel.getPixels().getData() + i) = d;
    }
    depthChannel.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    rgbChannel.draw(0, 0, width / 2, height / 2);
    irChannel.draw(width / 2, 0, width / 2, height / 2);
    depthChannel.draw(0, height / 2, width / 2, height / 2);
}
