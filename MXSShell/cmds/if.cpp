#include "ShellApi.h"

bool ifCondition;

class IfCommand : public Command
{
public:
    IfCommand()
        : Command(String(_T("if")), RegisterAsCommand)
    {
    }

    virtual bool ExecCommand(StringVector params, String cmdline) override
    {
        if (params.size() != 3)
            return false;

        const auto condition = params.at(1);

        ifCondition = true;

        if (condition == _T("$false") ||
            condition == _T("$null") ||
            condition == _T("0"))
            ifCondition = false;

        if (ifCondition)
        {
            return Parse::ParseBlock(params.at(2));
        }

        return true;
    }

} If_Instance;

class ElseCommand : public Command
{
public:
    ElseCommand()
        : Command(String(_T("else")), RegisterAsCommand)
    {
    }

    virtual bool ExecCommand(StringVector params, String cmdline) override
    {
        if (params.size() != 2)
            return false;

        if (!ifCondition)
        {
            return Parse::ParseBlock(params.at(1));
        }

        return true;
    }

} Else_Instance;

class TrueCommand : public Command
{
public:
    TrueCommand()
        : Command(String(_T("true")), RegisterAsFunction)
    {
    }

    virtual bool ExecFunction(String &rettext, StringVector params, String cmdline) override
    {
        rettext.clear();
        rettext.append(_T("$true"));
        return true;
    }

} True_Instance;
