#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <cmath>


// Debugging
const bool kPlotVectorField = false;

// Size constants
const int kEyePercentTop = 32 ;
const int kEyePercentSide = 18 ;
const int kEyePercentHeight = 20;
const int kEyePercentWidth = 20;

// Preprocessing
const bool kSmoothFaceImage = false;
const float kSmoothFaceFactor = 0.005;

// Algorithm Parameters
const int kFastEyeWidth = 50;
const int kWeightBlurSize = 5;
const bool kEnableWeight = true;
const float kWeightDivisor = 1.0;
const double kGradientThreshold = 50.0;

// Postprocessing
const bool kEnablePostProcess = true;
const float kPostProcessThreshold = 0.97;

// Eye Corner
const bool kEnableEyeCorner = false;

#endif
