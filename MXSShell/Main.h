#pragma once

#include "UnicodeStuff.h"

#include <shlwapi.h>
#include <windows.h>

#include <string>
#include <vector>

using namespace std;

typedef basic_string<_TCHAR> String;
typedef vector<String> StringVector;

enum RegisterAs
{
    RegisterAsCommand = 1,
    RegisterAsFunction = 2,
};

// command.cpp
class Command
{
public:
    Command(String name, int regAs);
    virtual ~Command() = default;
    virtual bool ExecCommand(StringVector params, String cmdline);
    virtual bool ExecFunction(String &rettext, StringVector params, String cmdline);
    virtual bool SupportsPiping();
};

// exec.cpp
// main.cpp
namespace Exec
{
    bool RegisterCommand(String name, Command* instance, int regAs);
    bool FindProgram(String &dest, String name);
    bool ExecCommand(StringVector params, String cmdline, String* _rettext = nullptr);
    bool Exit(StringVector params, String cmdline);
}

// environment.cpp
namespace Environment
{
    bool GetVariable(String &dest, String envvar, String defval);
    bool SetVariable(String varname, String value);
    bool LoadCurrentDirectory(String &dest);
    bool ChangeCurrrentDirectory(String NDir);
    void SetError(long error);
    long GetError();
}

// string.cpp
namespace Str
{
    int  Replace(String &dest, String src, _TCHAR from, _TCHAR to);
    void JoinVector(String &dest, StringVector data, size_t first, size_t last, int delim);
    String::size_type SplitLine(StringVector &dest, String data, _TCHAR delim, bool keepQuotes = false);
}

// string.cpp
namespace Path
{
    bool FileExists(const String path);
    bool FileHasExtension(const String path);
    bool EnumerateDirectory(StringVector& fileNames);
}

// parse.cpp
namespace Parse
{
    bool GetText(String &dest, String line, _TCHAR delim);
    bool ParseLine(String &dest, String &src, _TCHAR delim1=0, _TCHAR delim2=0);
    bool ParseBlock(String &src);
}