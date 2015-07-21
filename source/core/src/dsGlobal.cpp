#include "dsGlobal.h"

std::string trim(const std::string &str, const std::string &whitespace)
{
    std::string s(str);
    size_t endpos = s.find_last_not_of(whitespace);
    if (std::string::npos != endpos) {
        s = s.substr(0, endpos + 1);
    }
    size_t startpos = str.find_first_not_of(whitespace);
    if (std::string::npos != startpos) {
        s = s.substr(startpos);
    }
    return s;
}

std::vector<std::string> split(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> res;
    size_t begin = 0, end = 0;
    while (end != std::string::npos) {
        end = str.find(delimiter, begin);
        res.push_back(str.substr(begin,
                                 (end == std::string::npos) ? std::string::npos : end - begin)
                      );
        begin = ((end > (std::string::npos - delimiter.size())) ? std::string::npos : end + delimiter.size());
    }
    return res;
}
