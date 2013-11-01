#pragma once

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif !defined(UNICODE)
#undef _UNICODE
#endif

#include <iostream>
#include <tchar.h>

#ifdef _UNICODE
#define _tcin std::wcin
#define _tcout std::wcout
#define _tcerr std::wcerr
#else
#define _tcin  std::cin
#define _tcout std::cout
#define _tcerr std::cerr
#endif
