#ifndef DS_LOG_H
#define DS_LOG_H

#include <sstream>

namespace ds {

/**
 * Simple logging capability.
 * This class uses standard I/O stream to log textual messages.
 * The reporting level is handled globally via static definition.
 */
class Log {
public:

    /// Log level
    enum Level {
        Level_Error     = 0,
        Level_Warning   = 1,
        Level_Info      = 2,
        Level_Debug     = 3,
        Level_Debug1    = 4,
        Level_Debug2    = 5,
        Level_Debug3    = 6,
        Level_Debug4    = 7
    };

    Log();
    Log(const Log &log);
    Log& operator =(const Log &log);
    ~Log();

    std::ostringstream &stream(Level level = Level_Info);

    static Level reportingLevel();
    static void setReportingLevel(Level level);

private:

    std::ostringstream m_os;    ///< Output stream.
    Level m_level;
};

} // namespace ds

#if defined(WIN32) || defined(_MSC_VER)
    #define __FILE_SHORT__ \
    (strrchr(__FILE__,'\\') \
    ? strrchr(__FILE__,'\\')+1 \
    : __FILE__ \
    )
#else
    #define __FILE_SHORT__ \
    (strrchr(__FILE__,'/') \
    ? strrchr(__FILE__,'/')+1 \
    : __FILE__ \
    )
#endif

#define dsError()   (ds::Log().stream(ds::Log::Level_Error))
#define dsWarning() (ds::Log().stream(ds::Log::Level_Warning))
#define dsInfo()    (ds::Log().stream(ds::Log::Level_Info))
#define dsDebug()    (ds::Log().stream(ds::Log::Level_Debug) << "(" << __FILE_SHORT__ << ":" << __LINE__ << ")\t")

#endif // DS_LOG_H
