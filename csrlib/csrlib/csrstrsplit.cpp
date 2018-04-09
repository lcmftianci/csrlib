#include <stdio.h>
#include <iostream>
#include <vector>

char *mystrncpy(char *str, int n)
{
	//要求截取的字符串不可以改变，但指向字符串的指针可以改变
	char *p = str;
	if (p == NULL)
	{
		//如果截取的字符串是空的直接返回
		return NULL;
	}
	else
	{
		int i = 0;
		while (*p != '\0')
		{
			//循环直到达n个字符串终止
			if (i == n)
			{
				break;
			}
			i++;
			p++;
		}
		*(p++) = '\0';
		//赋值结束字符串
		return str;
	}
}

char* substr(char*str, unsigned int start, unsigned int end)
{
	unsigned n = end - start;
	static char stbuf[256];
	strncpy(stbuf, str + start, n);
	stbuf[n] = 0;
	return stbuf;
}

int splitwords(char* strSrc, std::vector<std::string> & arrStr, char chStr)
{
	unsigned int index = 0, len = 0, subIndex = 0, strlength = 0;
	char *tmpSrc = strSrc;
	while (*tmpSrc != '\0')
	{
		strlength++;
		tmpSrc++;
	}
	tmpSrc = strSrc;
	strlength++;
	while (strlength--)
	{
		index++;
		subIndex++;
		if (*tmpSrc == chStr)
		{
			if (subIndex == 1)
			{
				index = 0;
				tmpSrc++;
				continue;
			}
			if (index == subIndex)
			{
				char* sr/* = (char*)malloc(index)*/;
				sr = substr(strSrc, 0, index - 1);
				std::string str = sr;
				arrStr.push_back(str);
				index = 0;
				//free(sr);
			}
			else
			{
				if (subIndex - index == subIndex - 1)
				{
					index = 0;
					tmpSrc++;
					continue;
				}

				char* sr/* = (char*)malloc(subIndex - index)*/;
				sr = substr(strSrc, subIndex - index, subIndex - 1);
				std::string str = sr;
				arrStr.push_back(str);
				index = 0;
				//free(sr);
			}
		}
		else if (index != 0 && subIndex != 0 && *tmpSrc == '\0')
		{
			char* sr/* = (char*)malloc(subIndex - index)*/;
			sr = substr(strSrc, subIndex - index, subIndex - 1);
			std::string str = sr;
			if (str == "")
				return arrStr.size();
			arrStr.push_back(str);
			index = 0;
			//free(sr);
			return arrStr.size();
		}
		tmpSrc++;
	}
	return arrStr.size();
}

int main(int argc, char **argv)
{
	std::vector<std::string> arrWords;
	splitwords("___hello___ngd+__f_gf=ffg-fd_gi__hao__,_chi_na___,__", arrWords, '_');
	for (int i = 0; i < arrWords.size(); i++)
	{
		std::cout << arrWords[i].c_str() << std::endl;
	}
	system("pause");
	return 0;
}
