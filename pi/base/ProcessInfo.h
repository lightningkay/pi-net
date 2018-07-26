#ifndef PI_BASE_PROCESSINFO_H
#define PI_BASE_PROCESSINFO_H

#include <pi/base/StringPiece.h>
#include <pi/base/Types.h>
#include <pi/base/Timestamp.h>
#include <vector>

namespace pi
{
    namespace ProcessInfo
    {
        pid_t pid();
        string pidString();
        uid_t uid();
        string username();
        uid_t eid();
        Timestamp startTime();
        int clockTicksPerSecond();
        int pageSize();
        bool isDebugBuild();

        string hostname();
        string procname();
        StringPiece procname(const string& stat);

        string procStatus();

        string procStat();

        string threadStat();

        string exePath();

        int openedFiles();
        int maxOpenFiles();

        struct CpuTime
        {
            double userSeconds;
            double systemSeconds;

            CpuTime() : userSeconds(0.0), systemSeconds(0.0) { }
        };
        CpuTime cpuTime();

        int numThreads();
        std::vector<pid_t> threads();
    }
}

#endif
