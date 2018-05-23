#include "Main.h"

class PwdCommand : public Command
{
public:
    PwdCommand()
        : Command(String(_T("pwd")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        Environment::LoadCurrentDirectory(rettext);
        return true;
    }

} Pwd_Instance;
