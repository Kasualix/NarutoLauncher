#pragma once

#include <QString>

#include <ostream>
#include <windows.h>

namespace console {

inline bool isConsole()
{
    DWORD procIDs[2];
    DWORD maxCount = 2;
    DWORD result = GetConsoleProcessList((LPDWORD)procIDs, maxCount);
    return result > 1;
}

}  // namespace console
