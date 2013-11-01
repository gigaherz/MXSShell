#include "ShellApi.h"

Command::Command(String name, int regAs)
{
    Exec::RegisterCommand(name, this, regAs);
}

bool Command::ExecCommand(StringVector params, String cmdline)
{
    String ret;
    if (ExecFunction(ret, params, cmdline))
    {
        _tcout << ret << std::endl;
        return true;
    }
    return false;
}

bool Command::ExecFunction(String &rettext, StringVector params, String cmdline)
{
    rettext.clear();
    return true;
}

bool Command::SupportsPiping()
{
    return false;
}
