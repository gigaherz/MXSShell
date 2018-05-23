#include "Main.h"

using namespace std;

bool Parse::GetText(String &dest, String line, _TCHAR delim)
{
    dest.clear();
    for (auto sit : line)
    {
        if (sit == delim)
            return true;
        dest.append(1, sit);
    }

    if (delim != 0)
    {
        _tcout << "Syntax Error: End of line found in unfinished quoted string." << endl;
        return false;
    }
    return true;
}

bool Parse::ParseLine(String &dest, String &src, _TCHAR delim1, _TCHAR delim2)
{
    String line;
    String tmp;

    dest.clear();
    for (auto tline = src.c_str(); *tline != 0; tline++)
    {
        _TCHAR ch = *tline;

        if (ch == _T('\\'))
        {
            ch = *(++tline);
            switch (ch)
            {
            case _T('a'): line.append(1, _T('\a')); break;
            case _T('b'): line.append(1, _T('\b')); break;
            case _T('f'): line.append(1, _T('\f')); break;
            case _T('n'): line.append(1, _T('\n')); break;
            case _T('r'): line.append(1, _T('\r')); break;
            case _T('t'): line.append(1, _T('\t')); break;
            case _T('v'): line.append(1, _T('\v')); break;

            default:
                line.append(1, _T('\\'));
                line.append(1, ch);
                break;
            }
        }
        else if (ch == _T('\"'))
        {
            tline++;
            line.append(1, _T('\"'));
            GetText(tmp, tline, _T('\"'));
            line.append(tmp);
            tline += tmp.length();
            line.append(1, _T('\"'));
        }
        else if (ch == _T('{'))
        {
            tline++;
            line.append(1, _T('{'));
            GetText(tmp, tline, _T('}'));
            line.append(tmp);
            tline += tmp.length();
            line.append(1, _T('}'));
        }
        else if (ch == _T('['))
        {
            String tmp2(++tline);
            ParseLine(tmp, tmp2, _T(']'));
            line.append(tmp);
            src.assign(tmp2);
            tline = src.c_str();
        }
        else
        {
            line.append(1, ch);
        }
    }

    dest.clear();
    for (auto tline = line.c_str(); *tline != 0; tline++)
    {
        _TCHAR ch = *tline;

        if (ch == delim1 || ch == delim2)
        {
            src.assign(tline);
            return true;
        }

        if (ch == _T('\\'))
        {
            ch = *(++tline);
            switch (ch)
            {
            case _T(' '): line += _T("\\ "); break;
            case _T('\''): line.append(1, _T('\'')); break;
            case _T('"'): line.append(1, _T('"')); break;
            case _T('?'): line.append(1, _T('?')); break;
            case _T('$'): line.append(1, _T('$')); break;
            case _T('%'): line.append(1, _T('%')); break;
            default:
                String chars = _T("0123456789abcdef");
                int digits, base, cnt, acc;

                if (ch == 'u') // unicode character
                {
                    digits = 4;
                    base = 16;
                }

                if (ch == 'x') // hex character
                {
                    digits = 2;
                    base = 16;
                }

                if (ch >= '0' && ch <= '9') // decimal character
                {
                    digits = 3;
                    base = 10;
                }

                cnt = 0;
                acc = 0;
                while ((ch = *(++tline)) && cnt < digits)
                {
                    int idx = chars.find(tolower(ch));
                    if (idx < base)
                    {
                        acc = acc * base + idx;
                        cnt++;
                    }
                    else
                    {
                        --tline;
                        break;
                    }
                }

                break;
            }
        }
        else if (ch == _T('\"'))
        {
            tline++;
            dest.append(1, _T('\"'));
            GetText(tmp, tline, _T('\"'));
            dest.append(tmp);
            tline += tmp.length();
            dest.append(1, _T('\"'));
        }
        else if (ch == _T('\''))
        {
            tline++;
            GetText(tmp, tline, _T('\''));
            dest.append(tmp);
            tline += tmp.length();
        }
        else if (ch == _T('%'))
        {
            tmp.clear();
            while (*tline != 0)
            {
                tline++;
                ch = *tline;
                if (((ch >= _T('a')) && (ch <= _T('z'))) ||
                    ((ch >= _T('A')) && (ch <= _T('Z'))) ||
                    (ch == _T('_')))
                {
                    tmp.append(1, ch);
                }
                else break;
            }
            tline--; //go back 1 char
            String tmp2;
            Environment::GetVariable(tmp2, tmp, _T(""));
            dest.append(tmp2);
        }
        else if (ch == _T('$'))
        {
            StringVector params;

            tmp.clear();
            while (*tline != 0)
            {
                ch = *(++tline);
                if (((ch >= _T('a')) && (ch <= _T('z'))) ||
                    ((ch >= _T('A')) && (ch <= _T('Z'))) ||
                    ((ch >= _T('0')) && (ch <= _T('9'))) ||
                    (ch == _T('+')) ||
                    (ch == _T('-')) ||
                    (ch == _T('_')))
                {
                    tmp.append(1, ch);
                }
                else if (ch == _T('('))
                {
                    String tmp2(++tline);
                    String text;
                    ParseLine(text, tmp2, _T(')'));
                    line.assign(tmp2);
                    tline = line.c_str();
                    if (text.length() > 0)
                    {
                        tmp.append(1, _T(','));
                        tmp.append(text);
                    }
                    break;
                }
                else
                {
                    ch = *(--tline);
                    break;
                }
            }

            Str::SplitLine(params, tmp, _T(','));

            String tmp2;

            Str::JoinVector(tmp2, params, 0, params.size() - 1, _T(' '));

            Exec::ExecCommand(params, tmp2, &tmp);
            dest.append(tmp);

            if (tline == nullptr) break;
        }
        else
        {
            dest.append(1, ch);
        }
    }

    src.clear();

    if (delim1 != 0)
    {
        _tcout << "Syntax Error: Unexpected End of Line." << endl;
        return false;
    }
    return true;

}

bool Parse::ParseBlock(String &src)
{
    String tmp;
    return ParseLine(tmp, src, _T('}'));
}