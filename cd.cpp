#include "ShellApi.h"

class CdCommand : public Command
{
public:
    CdCommand()
        : Command(String(_T("cd")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    // if used without parameters, cd acts the same as pwd
    virtual bool ExecCommand(StringVector params, String cmdline) override
    {
        if (params.size() < 2)
        {
            String CDir;
            Environment::LoadCurrentDirectory(CDir);
            _tcout << CDir << endl;
            return true;
        }

        return Environment::ChangeCurrrentDirectory(params[1]);
    }

    // $cd is the same as $pwd
    virtual bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        Environment::LoadCurrentDirectory(rettext);
        return true;
    }

} Cd_Instance;