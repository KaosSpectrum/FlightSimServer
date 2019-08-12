#pragma once
#include "string"

/**
 * \brief Handles the certificates of connecting pilots/atc/supervisors
 * 
 */
class CCertificate
{
public:
	explicit CCertificate(const std::string& CertFile);
	~CCertificate();

	/**
	 * \brief Checks if the certificate is valid
	 * \param CID Certificate ID of the pilot/atc
	 * \param Password Password of the pilot/atc
	 * \return true if the details are valid
	 */
	bool IsCertificateValid(const std::string& CID, std::string Password);

	static int Callback(void *data, int argc, char **argv, char **azColName);

	
	/**
	 * \brief Callback when reading a cert from the database
	 * \param Data This is the certificate class (this)
	 * \param Argc How many values returned back from SQLite
	 * \param Argv Actual values returned back from SQLite
	 * \param AzColName Column name returned back from SQLite
	 * \return 
	 */
	static int ReadCallback(void *Data, int Argc, char **Argv, char **AzColName);

	
	/**
	 * \brief The certificate file location
	 */
	std::string CertificateFile;

	/**
	 * \brief If the last CID check was valid.
	 */
	bool bLastResultValid;
};