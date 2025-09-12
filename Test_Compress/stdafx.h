// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
/*
Specify size of PixelPacket color Quantums (8, 16, or 32).
A value of 8 uses half the memory than 16 and typically runs 30% faster,
but provides 256 times less color resolution than a value of 16.
*/

//#define _CRT_SECURE_NO_WARNINGS
//#define _SCL_SECURE_NO_WARNINGS

#include "targetver.h"
#include <iostream>

#include <Eigen/Dense>
#include <Magick++.h>



//#define MAGICKCORE_QUANTUM_DEPTH 8

#define DEBUG(x) cout << x << endl
#define DEBUG_CREATE_WEIGHT_MATRIX(x)
#define DEBUG_ANN_LEARNING(x) DEBUG(x)
#define DEEP_DEBUG_ANN_LEARNING(x) //DEBUG(x)
#define DEBUG_READ_IMAGE_TO_ARRAY(x) DEBUG(x)
#define ERROR_READ_IMAGE_TO_ARRAY(x) DEBUG(x)
#define DEEP_DEBUG_DATA_TO_ARRAY(x) //DEBUG(x)
#define DEEP_DEBUG_READ_IMAGE_TO_ARRAY(x)
#define DEEP_DEBUG_DECOMPRESS_CHANN(x)
#define DEEP_DEBUG_CREATE_OUTPUT_IMAGE(x)// DEBUG(x)

#define MESSAGE_ANN_LEARNING(x)	x

#define MatrixColor Matrix<Quantum, Dynamic, Dynamic>
#define MatrixColorFloat Matrix<double, Dynamic, Dynamic>

#define DEBUG_SPEED
//#define COLOR_FLOAT_RANGE_0_1
#define COLOR_FLOAT_RANGE_1_1

//#define IMAGE_GRAYSCALE

//#define CORRECT_ERR		1000000
//#define CORRECT_COEFF	1000000000

#define CORRECT_ERR		1
#define CORRECT_COEFF	1

#define MESSAGE_LEARNING_INTERVAL 10

using namespace std;
using namespace Magick;
using namespace Eigen;


#include "ANN.h"
#include "MathOperations.h"
#include "Image.h"
#include "TestCompress.h"

#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <bitset>
#include <conio.h>
#include <iomanip>
#include <windows.h>

// TODO: reference additional headers your program requires here


