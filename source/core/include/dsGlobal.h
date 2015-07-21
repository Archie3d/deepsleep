#ifndef DS_GLOBAL_H
#define DS_GLOBAL_H

#include <iostream>
#include <string>
#include <vector>
#include "dsLog.h"

#define DS_DISABLE_COPY(clazz) \
    clazz(const clazz&);    \
    clazz& operator =(const clazz&);

#define DS_UNUSED(value)    \
    ((void)(value))

#define DS_ASSERT(cond)   \
    {  \
        if (!(cond)) { \
            dsError() << __FILE_SHORT__ << ":" << __LINE__\
                      << " Assertion (" << # cond << ") failed in function " << __FUNCSIG__;\
            std::exit(-1);   \
        }   \
    }

#define DS_CHECK_GL_ERROR   {\
        GLenum err = glGetError();\
        if (err != GL_NO_ERROR) {\
            dsError() << __FILE__ << ":" << __LINE__ << " OpenGL error detected: " << err;\
        }\
    }

std::string trim(const std::string &str, const std::string &whitespace = " \t");
std::vector<std::string> split(const std::string &str, const std::string &delimiter);

#endif // DS_GLOBAL_H
