#include "Main.h"

class CrlfCommand : public Command
{
public:
    CrlfCommand()
        : Command(String(_T("_")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        rettext = _T("\n\r");
        return true;
    }

} Crlf_Instance;