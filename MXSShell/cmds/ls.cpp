#include "Main.h"
#include <Windows.h>
#include <strsafe.h>

class LsCommand : public Command
{
public:
    LsCommand()
        : Command(String(_T("ls")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    // if used as a command, ls lists the contents of the directory
    bool ExecCommand(StringVector params, String cmdline) override
    {
        vector<String> fileNames;

        if (!Path::EnumerateDirectory(fileNames))
            return false;

        for (auto fn : fileNames)
        {
            _tcout << fn << endl;
        }

        return true;
    }

    // if used as a function, ls returns the list as a string
    bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        vector<String> fileNames;

        if (!Path::EnumerateDirectory(fileNames))
            return false;

        bool first = true;
        for (auto fn : fileNames)
        {
            if (!first)
                rettext.append(_T(" "));
            rettext.append(_T("\""));
            rettext.append(fn);
            rettext.append(_T("\""));
            first = false;
        }

        return true;
    }

} Ls_Instance;