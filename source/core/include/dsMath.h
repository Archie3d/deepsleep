#ifndef DS_MATH_H
#define DS_MATH_H

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtx/io.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"

#define DS_EPSILON  ((float)1e-6)
#define DS_PI (3.14159265358979323846f)
#define DS_DEG(a)   ((a) * DS_PI / 180.0f)
#define DS_RAD(a)   ((a) * 180.0f / DS_PI)


#endif // DS_MATH_H
