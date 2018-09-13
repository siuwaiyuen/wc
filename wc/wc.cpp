#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<cstring>

using namespace std;

void basicCount(char *filename, int *cnt)
{
	char ch;
	char lch;
	fstream inFile;
	inFile.open(filename);
	if (!inFile.is_open())
	{
		cerr << "Open Failed." << endl;
		exit(1);
	}
	bool wFlag = false;
	bool mark;
	inFile >> noskipws;
	inFile >> ch;
	while (!inFile.eof())
	{
		if (inFile.good())
		{
			mark = (ch == ' ' || ch == ';' || ch == ',' || ch == '\t' || ch == '\n' || ch == '(' || ch == ')');
			if (mark && wFlag)
				cnt[1]++;   //word 
			wFlag = !mark;
			if (isgraph(ch))
				cnt[0]++;   //character
			if (ch == '\n')
				cnt[2]++; //line
			lch = ch;
		}
		inFile >> ch;
	}
	inFile.close();
	if (lch != '\n')
		cnt[2]++;
	return;
}
void extendedCount(char *filename, int *cnt)
{
	string line;
	int flagQuotes = 0, flagSlah = 0, flagCode = 0, flagComment = 0;
	int i, nBoth = 0;
	fstream inFile;
	inFile.open(filename);
	if (!inFile.is_open())
	{
		cerr << "Open Failed." << endl;
		exit(1);
	}
	while (!inFile.eof())
	{
		getline(inFile, line);
		i = 0;
		flagCode = 0;
		flagComment = 0;
		if (flagSlah)
			flagComment = 1;
		while (line[i] == ' ' || line[i] == '\t' || line[i] == '\r' || line[i] == '\n')
			++i;
		if (!flagSlah && (line[i] == '\0' || line[i + 1] == '\0'))
		{
			cnt[3]++;
			continue;
		}
		while (1)
		{
			if (!flagSlah && line[i] == '/' && line[i + 1] == '/')
			{
				if (flagCode)
				{
					cnt[4]++;     //comment
					cnt[5]++;     //code
					nBoth++;     //code & comment
				}
				else          //only comment
				{
					cnt[4]++;
				}
				break;
			}
			//Multi-line comments start
			if (!flagSlah && line[i] == '/' && line[i + 1] == '*')
			{
				i += 2;
				flagSlah = 1;
				flagComment = 1;
				continue;
			}
			//Multi-line comments
			if (flagSlah)
			{
				//"*/End of comment"
				if (line[i] == '*' && line[i + 1] == '/')
				{
					i++;
					flagSlah = 0;
				}
				else if (line[i] == '\0')
				{
					if (flagCode)
					{
						cnt[4]++;
						cnt[5]++;
						nBoth++;
					}
					else
					{
						++cnt[4];       //only comment
					}
					break;
				}
				++i;
				continue;
			}
			if (line[i] == '\0')
			{
				if (flagComment)
					cnt[4]++;
				if (flagCode)
					cnt[5]++;
				if (flagComment && flagCode)
					nBoth++;
				break;
			}
			i++;
			flagCode = 1;
		}

	}
	inFile.close();
}

int main(int argc, char* argv[])
{
	int cnt[6] = { 0 };
	basicCount(argv[argc - 1], cnt);
	extendedCount(argv[argc - 1], cnt);
	for (int i = 1; i < argc - 1; i++)
	{
		if (!strcmp(argv[i], "-c"))
			cout << "characters: " << cnt[0] << endl;
		if (!strcmp(argv[i], "-w"))
			cout << "words:" << cnt[1] << endl;
		if (!strcmp(argv[i], "-l"))
			cout << "lines:" << cnt[2] << endl;
		if (!strcmp(argv[i], "-a"))
			cout << "blank lines:" << cnt[3] << endl << "comment lines:" << cnt[4] << endl << "code lines:" << cnt[5] << endl;
	}
	system("pause");
	return 0;

	system("pause");
	return 0;

}
