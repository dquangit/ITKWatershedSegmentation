//
// Created by dquang on 7/23/17.
//

#ifndef WATERSHEDIMAGEFILTER_WATERSHEDSEGMENT_H
#define WATERSHEDIMAGEFILTER_WATERSHEDSEGMENT_H

#include "header.h"

class WatershedSegment {
public:

    typedef WatershedSegment* Pointer;

    static WatershedSegment* New();

    ~WatershedSegment();

    float getThreshold() const;

    WatershedSegment *setThreshold(float threshold);

    float getLevel() const;

    WatershedSegment *setLevel(float level);

    const GrayImage::Pointer &getInputImage() const;

    WatershedSegment *setInputImage(const GrayImage::Pointer &inputImage);

    LabeledImageType::Pointer execute() {
        typedef WatershedImageFilter<GrayImage> WatershedFilterType;
        WatershedFilterType::Pointer watershed = WatershedFilterType::New();
        watershed->SetThreshold(threshold);
        watershed->SetLevel(level);
        watershed->SetInput(inputImage);
        watershed->Update();
        return watershed->GetOutput();

//        typedef itk::ScalarToRGBColormapImageFilter<LabeledImageType, GrayImage> RGBFilterType;
//        GrayImage::Pointer colormapImageFilter = RGBFilterType::New();
//        colormapImageFilter->SetInput(watershed->GetOutput());
//        colormapImageFilter->SetColormap( RGBFilterType::Grey );
//        colormapImageFilter->Update();
//        return colormapImageFilter->GetOutput();
    }

private:
    float threshold;
    float level;
    GrayImage::Pointer inputImage;
    WatershedSegment();
};


#endif
