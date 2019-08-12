// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#include "CFSServer.h"
#include "boost/make_shared.hpp"
#include "CClientServer.h"
#include "Certificate.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

const std::string CERT_FILE = "\\cert.db";

CFSServer::CFSServer() = default;


CFSServer::~CFSServer() = default;

void CFSServer::RunServer(const std::string& ConfigFile)
{
	const boost::filesystem::path FullPath(boost::filesystem::current_path());
	Certificates = boost::make_shared<CCertificate>(FullPath.string() + CERT_FILE);
	ClientServer = boost::make_shared<CClientServer>(Certificates);

	if (ClientServer)
	{
		ClientServer->Listen(6809);
	}

	while (true)
	{
		if (ClientServer)
		{
			ClientServer->Poll();
		}
	}
}
