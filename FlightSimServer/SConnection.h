// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "boost/asio/io_service.hpp"

class CUser;

struct SConnection
{
	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf read_buffer;
	std::time_t LastResponseTime;
	int ClientIndex;
	bool bSocketHasConnection = false;
	bool bWantsToRemove = false;
	bool bWantsToRemoveAfterWrite = false;
	CUser* User;

	SConnection(boost::asio::io_service& io_service) : socket(io_service), read_buffer()
	{
	}

	SConnection(boost::asio::io_service& io_service, size_t max_buffer_size) : socket(io_service),
	                                                                           read_buffer(max_buffer_size)
	{
	}

	SConnection();

	bool operator==(const SConnection& Other)
	{
		return ClientIndex == Other.ClientIndex;
	}
};
