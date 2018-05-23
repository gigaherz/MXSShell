#include "Main.h"

#include <fstream>
#include <iterator>
#include <algorithm>

using namespace std;

bool Running;

bool Exec::Exit(StringVector params, String cmdline)
{
    if (params.size() == 2)
    {
        Environment::SetError(_tcstol(params[1].c_str(), nullptr, 0));
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

void Interactive()
{
    _TCHAR Char;
    String Line;

    String PS;
    Environment::GetVariable(PS, _T("PROMPT"), _T(""));
    if (PS.size() == 0)
    {
        PS = _T("$pwd()$_(): ");
        Environment::SetVariable(_T("PROMPT"), PS);
    }

    _tcout << "MXSS v0.2 by gigaherz" << endl << endl;

    Line.clear();
    Prompt();

    while (!_tcin.eof())
    {
        _tcin.get(Char);

        if (Char != _T('\n') && !_tcin.eof())
        {
            Line.append(1, Char);
        }
        else
        {
            bool result = ExecLine(Line);
            if (!Running)
            {
                if (!result) Environment::SetError(1);
                return;
            }

            if (_tcin.eof())
                return;

            Line.clear();
            Prompt();
        }
    }
}

void ExecuteFile(String file)
{
    char Char;
    String Line;

    Line.clear();
    Prompt();

    // TODO: Detect and handle file encoding (ASCII, UTF-8, UTF-16LE/BE, UTF-32LE/BE)
    ifstream input(file, std::ios::binary);

    while (!input.eof())
    {
        input.get(Char);

        if ((Char == _T('\n')) || input.eof())
        {
            bool result = ExecLine(Line);
            if (!Running)
            {
                if (!result) Environment::SetError(1);
                return;
            }

            if (input.eof())
                return;

            Line.clear();
            Prompt();
        }
        else Line.append(1, Char);
    }
}

int _tmain(int argc, _TCHAR* argv [])
{
    Running = true;
    Environment::SetError(0);

    Interactive();

    return Environment::GetError();
}

