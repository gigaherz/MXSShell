#pragma once

#pragma once

#define UNICODE

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif !defined(UNICODE)
#undef _UNICODE
#endif

#include <iostream>
#include <tchar.h>

#ifdef _UNICODE
#define _tcin wcin
#define _tcout wcout
#define _tcerr wcerr
#else
#define _tcin  cin
#define _tcout cout
#define _tcerr cerr
#endif

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

// forward declaration
class Command;

//////////////////////////////////////////////////////////////////////////
/// API functions

// exec.cpp
// main.cpp
namespace Exec
{
	bool RegisterCommand(String name, Command* instance, int regAs);
	bool FindProgram(String &dest, String name);
	bool ExecCommand(StringVector params, String cmdline, String* _rettext = NULL);
	bool Exit(StringVector params, String cmdline);
}

// environment.cpp
namespace Environment
{
	bool GetVariable(String &dest, String envvar, String defval);
	bool SetVariable( String varname, String value );
	bool LoadCurrentDirectory(String &dest);
	bool ChangeCurrrentDirectory(String NDir);
}

// string.cpp
namespace Str
{
	int  Replace(String &dest, String src, _TCHAR from, _TCHAR to);
	void JoinVector(String &dest, StringVector data, size_t first, size_t last, int delim);
	String::size_type SplitLine(StringVector &dest, String data, _TCHAR delim, bool keepQuotes=false);
}

// string.cpp
namespace Path
{
    bool FileHasExtension(const String &path);
    bool EnumerateDirectory(vector<String> fileNames);
}


class Command
{
public:
	Command(String name, int regAs)
	{
		Exec::RegisterCommand(name, this, regAs);
	}

	virtual bool ExecCommand(StringVector params, String cmdline)
	{
		String ret;
		if(ExecFunction(ret, params, cmdline))
		{
			_tcout << ret << endl;
			return true;
		}
		return false;
	}

	virtual bool ExecFunction(String &rettext, StringVector params, String cmdline)
	{
		rettext.clear();
		return true;
	}

	virtual bool SupportsPiping()
	{
		return false;
	}
};
