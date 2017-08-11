#include <iostream>
#include <itkCastImageFilter.h>
#include "header.h"
#include "WatershedSegment.h"
#include <sstream>
#include <itkMeanImageFilter.h>
#include <itkRGBToLuminanceImageFilter.h>
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

std::string floatToString (float number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}

int main(int argc, char *argv[]) {

    if (argc < 1) {
        return 0;
    }

    std::string fileName = argv[1];
    cout << fileName << endl;
//    ImageReader::Pointer reader = ImageReader::New();
    DICOMImageReader::Pointer reader = DICOMImageReader::New();
    reader->SetFileName(fileName);
    GrayImage::Pointer image = reader->GetOutput();
    GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();
    gradientMagnitudeImageFilter->SetInput(reader->GetOutput());
    gradientMagnitudeImageFilter->Update();
    GrayImage::Pointer imageToProcess = meanFilter(gradientMagnitudeImageFilter->GetOutput());
    WatershedSegment::Pointer watershed = WatershedSegment::New();
    FileWriterType::Pointer writer = FileWriterType::New();

    RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
    RGBToGrayScaleFilter::Pointer converter = RGBToGrayScaleFilter::New();

    register int levelIndex;
    register int thresholdIndex;
    float threshold;
    float level;
    int levelQuantity = 50;
    int thresholdQuantity = 50;
//    for (levelIndex = 1; levelIndex < levelQuantity; levelIndex ++) {
//        for (thresholdIndex = 1; thresholdIndex < thresholdQuantity; thresholdIndex ++) {
//            threshold = (float) 0.4 * thresholdIndex/thresholdQuantity;
//            level = (float) levelIndex/levelQuantity;
            threshold = 0.048;
            level = 0.24;
            std::string fileOut = getFileNameWithoutExtension(fileName) + "_threshold_" +
                                  floatToString(threshold) + "_level_" + floatToString(level) + "output.png";
            cout << "executing: " << fileOut << endl;

            LabeledImageType::Pointer labeledImage = watershed
                    ->setInputImage(imageToProcess)
                    ->setLevel(level)
                    ->setThreshold(threshold)
                    ->execute();

            colormapImageFilter->SetInput(labeledImage);
            colormapImageFilter->SetColormap(RGBFilterType::Jet);
            colormapImageFilter->Update();

            converter->SetInput(colormapImageFilter->GetOutput());
            converter->Update();

            OtsuFilterType::Pointer otsu = OtsuFilterType::New();
            otsu->SetInput(converter->GetOutput());
            otsu->Update();

            std::string filePath = "../result/" + fileOut;
            writer->SetFileName(filePath);
            writer->SetInput(otsu->GetOutput());
            writer->Update();
//        }
//    }
}

    // Parse arguments
//    float threshold = 0.005;
//    float level = 0.5;
//
//    ImageReader::Pointer imageReader = ImageReader::New();
//    std::string fileName = "coins.jpg";
//    imageReader->SetFileName(fileName);
//    UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
//    image->setFileName("");
//    UnsignedCharImageType::Pointer reader = UnsignedCharImageType::New();
//    reader->SetFileName(argv[1]);
//    UnsignedCharImageType::Pointer image = imageReader->GetOutput();
//
//
//    GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();
//    gradientMagnitudeImageFilter->SetInput(image);
//    gradientMagnitudeImageFilter->Update();
//
//    // Custom parameters
//    PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), threshold, level);
//
//    // Fixed parameters
//    PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .0025, .25);
//    PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .005, .5);
//    PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .0075, .75);
//    PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .009, .9);
//
//    return EXIT_SUCCESS;
//}
//
//
//void PerformSegmentation(FloatImageType::Pointer image, const float threshold, const float level)
//{
//    typedef itk::WatershedImageFilter<FloatImageType> WatershedFilterType;
//    WatershedFilterType::Pointer watershed = WatershedFilterType::New();
//    watershed->SetThreshold(threshold);
//    watershed->SetLevel(level);
//    watershed->SetInput(image);
//    watershed->Update();
//
//    typedef itk::ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType> RGBFilterType;
//    RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
//    colormapImageFilter->SetInput(watershed->GetOutput());
//    colormapImageFilter->SetColormap( RGBFilterType::Jet );
//    colormapImageFilter->Update();
//
//    std::stringstream ss;
//    ss << "output_" << threshold << "_" << level << ".png";
//
//    typedef itk::ImageFileWriter<RGBImageType> FileWriterType;
//    FileWriterType::Pointer writer = FileWriterType::New();
//    writer->SetFileName(ss.str());
//    writer->SetInput(colormapImageFilter->GetOutput());
//    writer->Update();
//
//}