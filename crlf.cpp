#include "ShellApi.h"

class CrlfCommand : public Command
{
public:
    CrlfCommand()
        : Command(String(_T("_")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    virtual bool ExecFunction(String &rettext, StringVector params, String cmdline)
    {
        rettext = _T("\n\r");
        return true;
    }

} Crlf_Instance;