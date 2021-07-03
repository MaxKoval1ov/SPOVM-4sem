#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>

extern "C"
{
	void WriteInFile(int file, std::string buff)
	{
		write(file, buff.c_str(), buff.size()-1);
		write(file,"\n\n",2);
	}
}

extern "C"
{
	void ReadFromFile(int file, std::string &buff)
	{
		int res;
		buff = "";
		char temp;
		do
		{
			temp = 0;
			res = read(file, &temp, sizeof(char));
			buff += temp;
		} while (res != 0);
	}
}