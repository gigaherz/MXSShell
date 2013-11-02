#include "ShellApi.h"

using namespace std;

bool Running;

bool Exec::Exit(StringVector params, String cmdline)
{
    if (params.size() == 2)
    {
        Environment::SetError(_tcstol(params[1].c_str(), NULL, 0));
    }
    Running = false;
    return true;
}

bool ExecLine(String line)
{
    String tmp(line);
    String parsed;
    String command;
    bool result = Parse::ParseLine(parsed, tmp, 0);
    if (!result) return false;

    command.assign(parsed);
    StringVector params;
    String::size_type argc = Str::SplitLine(params, parsed, ' ');
    if (argc <= 0) return false;

    return Exec::ExecCommand(params, command);
}

void Prompt()
{
    String PS;
    Environment::GetVariable(PS, _T("PROMPT"), _T(": "));

    String parsed;
    bool result = Parse::ParseLine(parsed, PS, 0);

    _tcout << parsed;
}

int _tmain(int argc, _TCHAR* argv [])
{
    _TCHAR Char;
    String Line;

    Running = true;
    Environment::SetError(0);

    String PS;
    Environment::GetVariable(PS, _T("PROMPT"), _T("$pwd()$_(): "));
    if (PS.size() == 0)
        PS = _T("$pwd()$_(): ");
    Environment::SetVariable(_T("PROMPT"), PS);

    _tcout << "MXSS v0.2 by gigaherz" << endl << endl;

    Prompt();

    Line.clear();
    while (!_tcin.eof())
    {
        _tcin.get(Char);

        if ((Char == _T('\n')) || _tcin.eof())
        {
            bool result = ExecLine(Line);
            if (!Running)
            {
                if (!result) return 1;
                return Environment::GetError();
            }

            if (_tcin.eof())
                return Environment::GetError();
            else
                Line.clear();

            Prompt();
        }
        else Line.append(1, Char);
    }

    return Environment::GetError();
}

