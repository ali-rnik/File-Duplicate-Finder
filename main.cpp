// compiled and tested in linux 64 bit //

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <algorithm>

#include <dirent.h>
#include <sys/stat.h>

using namespace std;

// define global map for hashing files addresses and their content
map<string, vector<string> > mp;

// declaring functions
void explore(char *dir_name);
void printDuplicateFilesAdr();


int main()
{

	string homeAdr = getenv("HOME");
	explore((char*)homeAdr.c_str());
	
	
	cout << "<===========================================>" << endl;
	printDuplicateFilesAdr();
	return 0;
}



// defining functions

// this function explore for regular files and hash them to map
void explore(char *dir_name)
{
	DIR *dir; //pointer to an open directory
	struct dirent *entry; //stuff in the directory
	struct stat info; //information about each entry
	
	//1 open
	dir = opendir(dir_name);
	if(!dir)
	{
		cout << "Directory not found" << endl;
		return;
	}
	
	//2 read
	while((entry = readdir(dir)) != NULL)
	{
		if(entry->d_name[0] != '.')
		{
			//saving the path of directory to variable path
			string path = string(dir_name) + "/" + string(entry->d_name);
			stat(path.c_str(), &info);
			
			//check whether it is a file or folder
			if(S_ISREG(info.st_mode))
			{
				//opening file
				ifstream inf;
				inf.open(path.c_str());
				if(!inf)
					cout << "cant open file = " << path << endl;
				
				// define a flag checking content length
				bool flag = false;
					
				// writing file content to  string
				string fileContentSegment;
				string txt;
				txt.clear(), fileContentSegment.clear();
				while(inf && flag == false)
				{
					getline(inf, fileContentSegment);
					txt += fileContentSegment;
					
					//reduce the accuracy check veryHuge files
					//for reducing the the time of run of program
					if(txt.size() > 1000000)
						flag = true;
				}
				inf.close();
				//hash string and address
				mp[txt].push_back(path);
			}
				
			//checking whether there is another directory in current directory
			// and recursivily check that directory
			if(S_ISDIR(info.st_mode))
				explore( (char*)path.c_str() );
		}
	}
	
	//3 close
	closedir(dir);
	
}

// this function will output the results saved in mp (map)
void printDuplicateFilesAdr()
{
	// I changed the format of showing output for more accuracy of presentation
	// the default showing was : ‫‪/test/a.txt‬‬ => ‫‪/test2/c.cpp‬
	map<string, vector<string> >::iterator it;
	for(it = mp.begin(); it != mp.end(); it++)
	{
		string hash = it->first;
		if(mp[hash].size() == 1)
			continue;
		for(int i = 0; i < mp[hash].size(); i++)
		{
			cout << mp[hash][i];
			if(i+1 != mp[hash].size())
				cout << " => " << endl; 
		}
		cout << endl << endl;
		cout << "The above files having the same content and they are duplicate." << endl;
		cout << "<===========================================>" << endl;
	}
}

