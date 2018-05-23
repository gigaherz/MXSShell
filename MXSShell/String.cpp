#include "Main.h"

int Str::Replace(String &dest, String src, _TCHAR from, _TCHAR to)
{
    int nr = 0;
    dest.clear();
    for (auto ch : src)
    {
        if (ch == from)
        {
            dest.append(1, to); nr++;
        }
        else
            dest.append(1, ch);
    }
    return nr;
}

void Str::JoinVector(String &dest, StringVector data, size_t first, size_t last, int delim)
{
    dest.clear();
    if (last < first) return;

    if (delim == -1)
    {
        for (size_t i = first; i <= last; i++)
        {
            dest.append(data[i]);
        }
    }
    else
    {
        for (size_t i = first; i < last; i++)
        {
            dest.append(data[i]);
            dest.append(1, delim);
        }
        dest.append(data[last]);
    }
}

String::size_type Str::SplitLine(StringVector &dest, String data, _TCHAR delim, bool keepQuotes)
{
    String elm(_T(""));
    String::size_type start = 0;

    dest.clear();

    int InAString = 0;

    elm.clear();
    for (auto ch : data)
    {
        switch (InAString)
        {
        case 2:
            if (ch == _T('\''))
            {
                if (keepQuotes) elm.append(1, ch);
                InAString = 0;
            }
            else elm.append(1, ch);
            break;
        case 1:
            if (ch == _T('"'))
            {
                if (keepQuotes) elm.append(1, ch);
                InAString = 0;
            }
            else elm.append(1, ch);
            break;
        default:
            switch (ch)
            {
            case _T('"'):
                if (keepQuotes) elm.append(1, ch);
                InAString = 1;
                break;
            case _T('\''):
                if (keepQuotes) elm.append(1, ch);
                InAString = 2;
                break;
            default:
                if (ch == delim)
                {
                    dest.push_back(elm);
                    elm.clear();
                }
                else elm.append(1, ch);
                break;
            }
            break;
        }
    }
    dest.push_back(elm);
    return dest.size();
}
