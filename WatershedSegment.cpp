//
// Created by dquang on 7/23/17.
//

#include "WatershedSegment.h"

WatershedSegment* WatershedSegment::New() {
    WatershedSegment::Pointer result = new WatershedSegment();
    return result;
}

WatershedSegment::WatershedSegment() {
    threshold = 0;
    level = 0;
}

WatershedSegment::~WatershedSegment() {
}

float WatershedSegment::getThreshold() const {
    return threshold;
}

WatershedSegment *WatershedSegment::setThreshold(float threshold) {
    WatershedSegment::threshold = threshold;
    return this;
}

float WatershedSegment::getLevel() const {
    return level;
}

WatershedSegment *WatershedSegment::setLevel(float level) {
    WatershedSegment::level = level;
    return this;
}

const GrayImage::Pointer &WatershedSegment::getInputImage() const {
    return inputImage;
}

WatershedSegment *WatershedSegment::setInputImage(const GrayImage::Pointer &inputImage) {
    WatershedSegment::inputImage = inputImage;
    return this;
}
