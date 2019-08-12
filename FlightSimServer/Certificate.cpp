#include "Certificate.h"
#include "sqlite3.h"
#include "iostream"
#include "string"
#include "boost/format/free_funcs.hpp"
#include "boost/format.hpp"

CCertificate::CCertificate(const std::string& CertFile)
{
	CertificateFile = CertFile;
	sqlite3* Database;
	const int CanNotOpen = sqlite3_open(CertFile.c_str(), &Database);

	if (CanNotOpen)
	{
		std::cout << "Could not open Certificate DB\n";
	}
	else
	{
		std::cout << "Certificate DB Found!\n";
		sqlite3_close(Database);
	}
}

CCertificate::~CCertificate() = default;

bool CCertificate::IsCertificateValid(const std::string& CID, const std::string Password)
{
	sqlite3* Database;
	const int CanNotOpen = sqlite3_open(CertificateFile.c_str(), &Database);

	if (CanNotOpen)
	{
		return false;
	}
	else
	{

		if (Database)
		{
			char* ErrMsg = nullptr;

			const std::string Query = boost::str(boost::format("SELECT * FROM Certificates WHERE CertID='%s' AND  Password='%s'") % std::stoi(CID) % Password);

			const int RC = sqlite3_exec(Database, Query.c_str(), CCertificate::ReadCallback, this, &ErrMsg);

			if (RC != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", ErrMsg);
				sqlite3_free(ErrMsg);
			}

			const bool bValid = bLastResultValid;
			bLastResultValid = false;

			sqlite3_close(Database);
			return bValid;
		}
	}
	return false;
}

int CCertificate::ReadCallback(void *Data, int Argc, char **Argv, char **AzColName) 
{
	CCertificate* Cert = static_cast<CCertificate*>(Data);
	if (Cert)
	{
		Cert->bLastResultValid = true;
	}
	return 0;
}


