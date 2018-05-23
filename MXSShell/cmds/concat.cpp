#include "Main.h"

class ConcatCommand : public Command
{
public:
    ConcatCommand()
        : Command(String(_T("+")), RegisterAsCommand | RegisterAsFunction)
    {
    }

    bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        Str::JoinVector(rettext, params, 1, params.size() - 1, -1);
        return true;
    }

} Concat_Instance;