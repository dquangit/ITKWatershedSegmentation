//
// Created by dquang on 7/24/17.
//

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkOtsuThresholdImageFilter.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkMedianImageFilter.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkBinaryFillholeImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkExpNegativeImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkBinaryShapeOpeningImageFilter.h"

using namespace std;
using namespace itk;

typedef Image<unsigned char, 2>       GrayImage;
typedef ImageFileReader<GrayImage>    ImageReader;
typedef Image<float, 2>               FloatImageType;
typedef RGBPixel<unsigned char>       RGBPixelType;
typedef Image<RGBPixelType, 2>        RGBImageType;
typedef ImageFileWriter<GrayImage> FileWriterType;
typedef Image<IdentifierType, 2> LabeledImageType;
typedef GradientMagnitudeImageFilter<
        GrayImage, GrayImage >  GradientMagnitudeImageFilterType;
typedef MeanImageFilter< GrayImage, GrayImage > 		MeanFilterType;
typedef OtsuThresholdImageFilter<GrayImage, GrayImage> OtsuFilterType;
typedef ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType> RGBFilterType;
typedef RGBToLuminanceImageFilter< RGBImageType, GrayImage> RGBToGrayScaleFilter;
typedef AdaptiveHistogramEqualizationImageFilter<GrayImage> HistogramEqualization;
typedef MedianImageFilter<GrayImage,GrayImage> MedianFilterType;
typedef BinaryFillholeImageFilter<GrayImage> ImageFill;
typedef RelabelComponentImageFilter<GrayImage, GrayImage> LabelRelabelComponentType;
typedef itk::ExpNegativeImageFilter< GrayImage, GrayImage > NegativeFilterType;
typedef itk::InvertIntensityImageFilter <GrayImage>
        InvertIntensityImageFilterType;
typedef BinaryShapeOpeningImageFilter<GrayImage>  BinaryShapeOpeningImage;
typedef WatershedImageFilter<GrayImage> WatershedFilterType;
//typedef Image< InputPixelType, InputDimension > DICOMImageType;
//typedef ImageFileReader<DICOMImageType> DICOMImageReader;
