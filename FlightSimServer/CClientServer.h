// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once
#include "boost/asio.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "SConnection.h"
#include <boost/enable_shared_from_this.hpp>
#include "Enums.h"
#include "Certificate.h"
#include "boost/smart_ptr.hpp"

class CUser;

using boost::asio::ip::tcp;

class CClientServer : public boost::enable_shared_from_this<CClientServer>
{
	using con_handle_t = std::list<SConnection>::iterator;
	using StringVec = std::vector<std::string>;
	using MsgBuffer = std::shared_ptr<std::string>;

public:
	CClientServer(const boost::shared_ptr<CCertificate> Certificate) : ClientIoService(),
	                                                                   ClientAcceptor(ClientIoService),
	                                                                   ClientConnections()
	{
		Certificates = Certificate;
	}

	~CClientServer();

	//Get the command in enum form, plus an out index for convienient string manip!
	static EClientCommand GetCommand(const std::string& InString, int& OutCmdIndex);

	void HandleRead(const con_handle_t& InConnectionHandle, boost::system::error_code const& InError,
	                size_t InBytesTransferred);

	//Parse the incoming line, grabbing what we need, and working out what command we should process!
	void ParseIncomingLine(const con_handle_t& InConnectionHandle, const std::string& InLine);

	void HandleMessage(const con_handle_t& InConnectionHandle, const StringVec& Strings);

	//Add an ATC to the network
	void AddController(const con_handle_t& InConnectionHandle, const StringVec& InControllerData);

	//Show an error and disconnect them!
	void ShowErrorAndDisconnect(const con_handle_t& InConnectionHandle, const std::string& Callsign,
	                            EClientError Error);

	void ProcessControllerPosition(const con_handle_t& InConnectionHandle, StringVec& PositionData);
	void DoAsyncRead(const con_handle_t& InConnectionHandle);
	void SendServerMessage(const con_handle_t& InConnectionHandle, CUser* InTargetUser, const std::string& InMessage);
	void HandleWrite(const con_handle_t& InConnectionHandle, const MsgBuffer& MessageBuffer,
	                 boost::system::error_code const& Err);
	void HandleWrite(const con_handle_t& InConnectionHandle, CUser* SrcUser, CUser* TargetUser, const MsgBuffer&
	                 InMessageBuffer, boost::system::error_code const& InError);
	void HandleAccept(const con_handle_t& InConnectionHandle, boost::system::error_code const& Err);
	void HandleClientDisconnect(const con_handle_t& InConnectionHandle);
	void StartAccept();
	void Listen(uint16_t InListenPort);
	void Poll();

	void ShowIdleDisconnect(const con_handle_t& Conn);

	int CurrentClientIndex = 0;
	boost::asio::io_service ClientIoService;
	tcp::acceptor ClientAcceptor;
	std::list<SConnection> ClientConnections;
	boost::weak_ptr<CCertificate> Certificates;
};
