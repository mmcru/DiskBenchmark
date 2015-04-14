//DiskBenchmark
//by Matt Crupi

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <time.h>
#include <windows.h>
#include <direct.h>
#include <string>
#include <sstream>

int _tmain(int argc, _TCHAR* argv[])
{
	//blockSize is 4096
	int blockSize = 4096;
	//data buffer
	char* mydata = (char*)malloc(1*blockSize);
	//name of the temp file
	char tempname[] = "/temp.txt";
	//get a directory from the user
	string input = "";
	cout << "\nthis program can test any drive, but it needs to write a temp file to a directory within that drive";
	cout << "\nchoose a directory in which to store temp file:\n> ";
	getline(cin, input);

	//this directory determines which storage device will be tested
	//it is also the location of temp.txt
	std::string directory;
	directory += input;
	directory += tempname;

	//commencing scan message
	std::cout<<"\nstorage drive benchmark\nrunning on drive: "<<input[0];

	//initialized with random data
	srand(time(NULL));
	for(int i=0;i<blockSize;i++){
		mydata[i] = rand()%256;
	}

	double startt, endt, difft;
	int times = 10*25000;
	int i=0,j=0;
	DWORD written;
	HANDLE f, g;
	DWORD read;

	f=CreateFileA(
		directory.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_FLAG_NO_BUFFERING,
		NULL
    );

	//if createfile fails, it throws invalid_handle_value
	if(f==INVALID_HANDLE_VALUE){
		std::cout<<"\n\nerror creating temp file for write test\ncheck your directory (unix-style paths are ok):  " << directory.c_str() << "\n";
		std::cout<<"this program needs to be able to create a file at that directory in order to work.\n";
		system("pause");
		return -1;
	}

	//start timer
	startt = clock();
	std::cout<<"\n\ntesting write speed...";
	//loop that actually performs the write test
	for(i=0;i<times;i++){
		WriteFile(
		f,
		mydata,
		blockSize,
		&written,
		NULL
		);
	}
	endt = clock();
	//display info to user
	difft = 1.0*(endt-startt)/(1.0*CLOCKS_PER_SEC);
	std::cout<<"\nwrite time:  "<<difft<<" seconds";
	std::cout<<"\nwrite speed:  "<<1.0*times*blockSize/difft/1024/1024<<" MB/s";

	CloseHandle(f);
	//------------------------------------------------------------------------------------------------

	g=CreateFile(directory.c_str(),
					GENERIC_READ,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_FLAG_NO_BUFFERING,
					NULL
					);
	//if createfile fails, it throws invalid_handle_value
	if(g==INVALID_HANDLE_VALUE){
		std::cout<<"\nerror creating temp file for read test\ncheck your directory:  " << directory.c_str() << "\n(unix style paths are ok\n";
		std::cout<<"this program needs to be able to create a file at that directory in order to work.\n";
		system("pause");
		return -1;
	}
	//start timer
	startt = clock();
	std::cout<<"\n\ntesting read speed...";
	//loop that performs read test
	for(i=0;i<times;i++){
		ReadFile(
		g,
		mydata,
		blockSize,
		&read,
		NULL
		);
	}
	endt = clock();
	//info to user
	difft = 1.0*(endt-startt)/(1.0*CLOCKS_PER_SEC);
	std::cout<<"\nread time:  "<<difft<<" seconds";
	std::cout<<"\nread speed:  "<<1.0*times*blockSize/difft/1024/1024<<" MB/s";

	std::cout<<"\nfinished!\n";

	CloseHandle(g);
	//system pause adds the "any key to continue" dialogue
	system("pause");
	return 0;
}

