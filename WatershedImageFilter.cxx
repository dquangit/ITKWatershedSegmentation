#include <iostream>
#include <itkCastImageFilter.h>
#include "header.h"
#include "WatershedSegment.h"
#include <sstream>
#include <itkMeanImageFilter.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkAdaptiveHistogramEqualizationImageFilter.h>
#include <itkExpNegativeImageFilter.h>

// Run with:
// ./WatershedImageFilter threshold level
// e.g.
// ./WatershedImageFilter 0.005 .5
// (A rule of thumb is to set the Threshold to be about 1 / 100 of the Level.)

std::string getFileNameWithoutExtension(std::string fileName) {
    std::string dot(".");
    std::size_t found = fileName.find(dot);
    if (found!=std::string::npos) {
        return fileName.substr(0, found);
    }
    return fileName;
}

GrayImage::Pointer meanFilter(GrayImage::Pointer input) {

    // Apply filter
    MeanFilterType::Pointer filter = MeanFilterType::New();
    filter->SetInput(input);
    filter->Update();

    // Convert image to write

    return filter->GetOutput();
}

GrayImage::Pointer gradient(GrayImage::Pointer input) {
    GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();
    gradientMagnitudeImageFilter->SetInput(input);
    gradientMagnitudeImageFilter->Update();
    return gradientMagnitudeImageFilter->GetOutput();
}

GrayImage::Pointer medianFilter(GrayImage::Pointer input) {
    MedianFilterType::Pointer filter = MedianFilterType::New();
    filter->SetInput(input);
    filter->Update();
    return filter->GetOutput();
}

GrayImage::Pointer histogramEqualization(GrayImage::Pointer input) {
    HistogramEqualization::Pointer adapter =  HistogramEqualization::New();
    adapter->SetInput(input);
    adapter->Update();
    return adapter->GetOutput();
}

GrayImage::Pointer imfill(GrayImage::Pointer input) {
    ImageFill::Pointer imfill =  ImageFill::New();
    imfill->SetInput(input);
    imfill->SetForegroundValue( itk::NumericTraits<unsigned char>::min() );
    imfill->Update();
    return imfill->GetOutput();
}

GrayImage::Pointer negativeImage(GrayImage::Pointer input) {
    InvertIntensityImageFilterType::Pointer invertIntensityFilter
            = InvertIntensityImageFilterType::New();
    invertIntensityFilter->SetInput(input);
    invertIntensityFilter->SetMaximum(255);
    invertIntensityFilter->Update();
    return invertIntensityFilter->GetOutput();
}

GrayImage::Pointer removeSmallObject(GrayImage::Pointer input) {
    BinaryShapeOpeningImage::Pointer filter = BinaryShapeOpeningImage::New();
    filter->SetInput(input);
    GrayImage::SizeType inputSize = input->GetLargestPossibleRegion().GetSize();
    double minSize = inputSize[0] * inputSize[1] * 0.2;
    cout << "min size: " << minSize << endl;
    filter->SetLambda(minSize);
//    filter->SetAttribute("Size");
    filter->Update();
    return filter->GetOutput();
}

LabeledImageType::Pointer watershedSegmentation(GrayImage::Pointer inputImage, double threshold, double level) {
    WatershedFilterType::Pointer watershed = WatershedFilterType::New();
    watershed->SetThreshold(threshold);
    watershed->SetLevel(level);
    watershed->SetInput(inputImage);
//    try {
        watershed->Update();
//    } catch (exception &e) {
//            printf("exception: %s", e.what());
//    }
    return watershed->GetOutput();
}

GrayImage::Pointer RGBToGrayScale(RGBImageType::Pointer input) {
    RGBToGrayScaleFilter::Pointer colormapImageFilter = RGBToGrayScaleFilter::New();
    colormapImageFilter->SetInput(input);
    colormapImageFilter->Update();
    return colormapImageFilter->GetOutput();
}

void writeImage(string imageName, string directory, GrayImage::Pointer image) {
    FileWriterType::Pointer writer = FileWriterType::New();
    std::string filePath = "../result/" + directory + "/"  + imageName;
    writer->SetFileName(filePath);
    writer->SetInput(image);
    writer->Update();
}

std::string floatToString (double number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}

int main(int argc, char *argv[]) {

//    if (argc < 1) {
//        return 0;
//    }
//    argv[1] = "2-11-500-500-100";
    std::string fileName = argc < 2? "2-11-500-500-100.jpg" : argv[1];
    cout << fileName << endl;
    ImageReader::Pointer reader = ImageReader::New();
//    DICOMImageReader::Pointer reader = DICOMImageReader::New();
    reader->SetFileName(fileName);
    GrayImage::Pointer image = reader->GetOutput();
//    image = medianFilter(image);
    image = gradient(image);
//    image = negativeImage(image);

    WatershedSegment::Pointer watershed = WatershedSegment::New();
    RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
    GrayImage::Pointer imageToProcess = medianFilter(image);
    register int levelIndex;
    register int thresholdIndex;
    double threshold;
    double level;
    int levelQuantity = 50;
    int thresholdQuantity = 50;
    for (levelIndex = 1; levelIndex < levelQuantity; levelIndex ++) {
//        for (thresholdIndex = 1; thresholdIndex < thresholdQuantity; thresholdIndex ++) {
            level = (double) levelIndex/levelQuantity;
            threshold = level * (levelQuantity%2==0?0.01 : 0.05);
            if (threshold >= level) continue;

//    level = 0.24;
//    threshold = 0.048;
//    if (argc == 4) {
//        std::string strLevel = argv[2];
//        std::string strThreshold = argv[3];
//        std::stringstream ssLevel;
//        std::stringstream ssThreshold;
//        ssLevel << strLevel;
//        ssLevel >> level;
//        ssThreshold << strThreshold;
//        ssThreshold >> threshold;
//    }

            std::string fileOut = getFileNameWithoutExtension(fileName) + "_threshold_" +
                                  floatToString(threshold) + "_level_" + floatToString(level) + "output.png";
            cout << "executing: " << fileOut << endl;

            LabeledImageType::Pointer labeledImage = watershedSegmentation(imageToProcess, threshold, level);

            colormapImageFilter->SetInput(labeledImage);
            colormapImageFilter->SetColormap(RGBFilterType::Jet);
            colormapImageFilter->Update();

            GrayImage::Pointer resultImage = RGBToGrayScale(colormapImageFilter->GetOutput());

            OtsuFilterType::Pointer otsu = OtsuFilterType::New();
            otsu->SetInput(resultImage);
            otsu->Update();


        resultImage = otsu->GetOutput();
//        resultImage = imfill(resultImage);
//            image = negativeImage(image);
        GrayImage::Pointer finalResult = removeSmallObject(resultImage);
            writeImage(fileOut, getFileNameWithoutExtension(fileName), finalResult);
//        }
    }
}
