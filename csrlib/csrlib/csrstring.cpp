#include "csrstring.h"

csrstring::csrstring(void)
{
	m_str = new char[1];
	m_str[0] = '\0';
}

csrstring::csrstring(char* cstr) :m_str(cstr)
{

}

csrstring::csrstring(const csrstring& cstr)
{

}

