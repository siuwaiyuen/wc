#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<cstring>

using namespace std;

void baseCount(char *filename, int *cnt)
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
				cnt[1]++;   //统计词数 
			wFlag = !mark;
			if (isgraph(ch))
				cnt[0]++;   //统计字符数
			if (ch == '\n')
				cnt[2]++; //统计行数
			lch = ch;
		}
		inFile >> ch;
	}
	inFile.close();
	if (lch != '\n')
		cnt[2]++;
	return;
}

int main(int argc, char* argv[])
{
	int cnt[3] = { 0 };
	baseCount(argv[argc - 1], cnt);
	for (int i = 1; i < argc - 1; i++)
	{
		if (!strcmp(argv[i], "-c"))
			cout << "characters: " << cnt[0] << endl;
		if (!strcmp(argv[i], "-w"))
			cout << "words:" << cnt[1] << endl;
		if (!strcmp(argv[i], "-l"))
			cout << "lines:" << cnt[2] << endl;
	}
	system("pause");
	return 0;

	system("pause");
	return 0;

}

