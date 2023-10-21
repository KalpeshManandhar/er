#pragma once

#include <math/mat.h>

Mat4 parallelProjection();
Mat4 perspectiveProjection(float fovXDegree, float fovYDegree, float zNear, float zFar);


