// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once
#include "string"
#include "boost/shared_ptr.hpp"

class CClientServer;
class CCertificate;

class CFSServer
{
public:
	CFSServer();
	~CFSServer();

	void RunServer(const std::string& ConfigFile);

	boost::shared_ptr<CClientServer> ClientServer;
	boost::shared_ptr<CCertificate> Certificates;
};
