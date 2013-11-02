#include "ShellApi.h"
#include "Streaming.h"

bool TextReader::ReadLine(String &dest)
{
    return ReadDelimited(dest, String(_T("\n\r")), true);
}

bool TextReader::ReadToEnd(String &dest)
{
    return ReadDelimited(dest, String(), true);
}

bool Win32PipeReader::ReadDelimited(String &dest, String delimiters, bool allowEOF)
{
    if ((delimiters.size() == 0) && !allowEOF)
        return false;

    for (;;)
    {
        static char dbuff[1024];
        static _TCHAR wbuff[1024];
        DWORD read;
        DWORD avail;
        bool isPeek = true;
        if (!PeekNamedPipe(hReadEnd, dbuff, 1023, &read, &avail, NULL))
        {
            cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
            return false;
        }
        if (read == 0)
        {
            isPeek = false;
            // Wait for more data
            if (!ReadFile(hReadEnd, dbuff, 1, &read, NULL))
            {
                cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
                return false;
            }
        }
        dbuff[read] = 0;
        if (read == 0) break;

        MultiByteToWideChar(CP_THREAD_ACP, 0, dbuff, read, wbuff, 1024);

        for (DWORD i = 0; i < read; i++)
        {
            if (delimiters.find_first_of(wbuff[i]) != delimiters.npos)
            {
                DWORD nread = i + 1;
                wbuff[nread] = 0;
                dest.append(wbuff);
                if (isPeek)
                {
                    if (!ReadFile(hReadEnd, dbuff, nread, &read, NULL))
                    {
                        cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
                        return false;
                    }
                }
                break;
            }
        }

    }

    return true;
}

bool Win32PipeReader::ReadAvailableBytes(String &dest)
{
    char dbuff[1025];
    _TCHAR wbuff[1025];
    for (;;) {
        DWORD read;
        DWORD avail;

        if (!PeekNamedPipe(hReadEnd, NULL, 0, NULL, &avail, NULL))
        {
            cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
            return false;
        }

        if (avail == 0)
            break;

        if (avail > 1024)
            avail = 1024;

        if (!ReadFile(hReadEnd, dbuff, avail, &read, NULL))
        {
            cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
            return false;
        }
        dbuff[read] = 0;
        if (read == 0) break;

        MultiByteToWideChar(CP_THREAD_ACP, 0, dbuff, read, wbuff, 1024);

        dest.append(wbuff);
    }

    return true;
}

int Win32PipeReader::GetAvailableBytes()
{
    DWORD avail;

    if (!PeekNamedPipe(hReadEnd, NULL, 0, NULL, &avail, NULL))
    {
        cout << "Pipe Read failed (" << GetLastError() << ")." << endl;
        return -1;
    }

    return avail;
}
