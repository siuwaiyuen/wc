#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<cstring>
#include<io.h>
#include<vector>
#include <direct.h>

#define MAX_PATH 80

using namespace std;

void getFiles(string path, string path2, vector<string>& files)
{
	long   hFile = 0;
	struct _finddata_t fileinfo;   //file information
	string p, p2, suffixName;
	if ((hFile = _findfirst(p.assign(path).append(path2).append("*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\"), p2.assign(fileinfo.name).append("\\"), files);
			}
			else
			{
				files.push_back(p.assign(path2).append(fileinfo.name));  //relative path
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void basicCount(string filename, int *cnt)
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
void extendedCount(string filename, int *cnt)
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
	bool _w = false, _c = false, _l = false, _a = false, _s = false;
	int i;
	for (i = 1; i < argc - 1; i++)
	{
		if (!strcmp(argv[i], "-s"))
			_s = true;
		if (!strcmp(argv[i], "-c"))
			_c = true;
		if (!strcmp(argv[i], "-w"))
			_w = true;
		if (!strcmp(argv[i], "-l"))
			_l = true;
		if (!strcmp(argv[i], "-a"))
			_a = true;
	}
	vector<string> files;
	char   buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	int cnt[6] = { 0 };
	string filePath = argv[argc - 1];
	if (_s)
	{
		filePath.assign(buffer).append("\\");
		getFiles(filePath, "", files);
		int size = files.size();
		for (i = 0; i < size; i++)
		{
			cout << files[i].c_str() << endl;
			basicCount(files[i].c_str(), cnt);
			extendedCount(files[i].c_str(), cnt);
			if (_w)
				cout << "words:" << cnt[0] << endl;
			if (_c)
				cout << "characters:" << cnt[1] << endl;
			if (_l)
				cout << "lines:" << cnt[2] << endl;
			if (_a)
			{
				cout << "blank lines:" << cnt[3] << endl;
				cout << "comments lines:" << cnt[4] << endl;
				cout << "codes lines:" << cnt[5] << endl;
			}
		}
	}
	else
	{
		basicCount(argv[argc - 1], cnt);
		extendedCount(argv[argc - 1], cnt);
		if (_w)
			cout << "words:" << cnt[0] << endl;
		if (_c)
			cout << "characters:" << cnt[1] << endl;
		if (_l)
			cout << "lines:" << cnt[2] << endl;
		if (_a)
		{
			cout << "blank lines:" << cnt[3] << endl;
			cout << "comments lines:" << cnt[4] << endl;
			cout << "codes lines:" << cnt[5] << endl;
		}
	}

	system("pause");
	return 0;

	system("pause");
	return 0;

}

