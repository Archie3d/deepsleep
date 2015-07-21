#include <iostream>
#include <ctime>
#include <map>
#include "dsLog.h"

namespace ds {

static Log::Level s_reportingLevel = Log::Level_Info;

std::map<Log::Level, const char*> generateLogLevelToStringMap()
{
    std::map<Log::Level, const char*> map;
    map[Log::Level_Error]   = " ERROR ";
    map[Log::Level_Warning] = "WARNING";
    map[Log::Level_Info]    = " INFO  ";
    map[Log::Level_Debug]   = " DEBUG ";
    map[Log::Level_Debug1]  = " DEBUG1";
    map[Log::Level_Debug2]  = " DEBUG2";
    map[Log::Level_Debug3]  = " DEBUG3";
    map[Log::Level_Debug4]  = " DEBUG4";
    return map;
}
static std::map<Log::Level, const char*> s_logLevelToStringMap = generateLogLevelToStringMap();

Log::Level Log::reportingLevel()
{
    return s_reportingLevel;
}

void Log::setReportingLevel(Level level)
{
    s_reportingLevel = level;
}

Log::Log()
    : m_os(),
      m_level(Level_Info)
{
}

Log::Log(const Log &log)
    : m_os(),
      m_level(log.m_level)
{
}

Log& Log::operator =(const Log &log)
{
    if (this != &log) {
        m_level = log.m_level;
    }
    return *this;
}

Log::~Log()
{
    if (m_level <= Log::reportingLevel()) {
        m_os << std::endl;
        FILE *f = stdout;
        if (m_level == Level_Error) {
            f = stderr;
        }
        fprintf(f, "%s", m_os.str().c_str());
        fflush(f);
    }
}

std::ostringstream &Log::stream(Level level)
{
    m_level = level;

    time_t rawTime;
    struct tm timeinfo;
    char buffer[80];

    time(&rawTime);
    localtime_s(&timeinfo, &rawTime);
    strftime(buffer, 80, "%c", &timeinfo);

    m_os << buffer << " [" << s_logLevelToStringMap[level] << "]\t";

    return m_os;
}

} // namespace ds
