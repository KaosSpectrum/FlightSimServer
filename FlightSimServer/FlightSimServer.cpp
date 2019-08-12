// FlightSimServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "CFSServer.h"

#define PATH_CONFIG_FILE "config.txt"

void DoSignals()
{
#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
#endif
}

void Run(const std::string& ConfigFile)
{
	CFSServer* FsServer = new CFSServer();
	if (!FsServer) return;
	FsServer->RunServer(ConfigFile);

	delete(FsServer);
}

int main(int Argc, char** Argv)
{
	const std::string Configfile = PATH_CONFIG_FILE;
	
	DoSignals();
	Run(Configfile);
}