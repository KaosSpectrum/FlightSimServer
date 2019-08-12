#include "CClientServer.h"
#include "iostream"
#include "CUser.h"
#include "StringHelpers.h"
#include "boost/format.hpp"
#include "boost/format/free_funcs.hpp"

const int CLIENT_TIMEOUT_VALUE = 10;

const char* CLIENT_COMMANDS[] =
{
	"#AA",
	"#DA",
	"#AP",
	"#DP",
	"$HO",
	"#TM",
	"#RW",
	"@",
	"%",
	"$PI",
	"$PO",
	"$HA",
	"$FP",
	"#SB",
	"#PC",
	"#WX",
	"#CD",
	"#WD",
	"#TD",
	"$C?",
	"$CI",
	"$AX",
	"$AR",
	"$ER",
	"$CQ",
	"$CR",
	"$!!",
	"#DL",
	nullptr
};


CClientServer::~CClientServer() = default;

EClientCommand CClientServer::GetCommand(const std::string& InString, int& OutCmdIndex)
{
	for (int Index = 0; CLIENT_COMMANDS[Index]; Index++)
	{
		if (!strncmp(InString.c_str(), CLIENT_COMMANDS[Index], strlen(CLIENT_COMMANDS[Index])))
		{
			OutCmdIndex = Index;
			return static_cast<EClientCommand>(Index);
		}
	}
	return EClientCommand::CC_NOTHING;
}

void CClientServer::HandleRead(const con_handle_t& InConnectionHandle, boost::system::error_code const& InError,
                               size_t InBytesTransferred)
{
	if (InBytesTransferred > 0)
	{
		std::istream Is(&InConnectionHandle->read_buffer);
		const std::time_t Result = std::time(nullptr);
		InConnectionHandle->LastResponseTime = Result;
		std::string Line;
		std::getline(Is, Line);
		std::cout << "Message Received: " << Line << std::endl;
		ParseIncomingLine(InConnectionHandle, Line);
	}

	if (InConnectionHandle->bWantsToRemove)
	{
		InConnectionHandle->socket.shutdown(tcp::socket::shutdown_both);
		InConnectionHandle->socket.close();
		HandleClientDisconnect(InConnectionHandle);
		return;
	}

	if (!InError)
	{
		DoAsyncRead(InConnectionHandle);
	}
	else
	{
		std::cerr << "We had an error: " << InError.message() << std::endl;
		HandleClientDisconnect(InConnectionHandle);
	}
}

void CClientServer::ParseIncomingLine(const con_handle_t& InConnectionHandle, const std::string& InLine)
{
	std::vector<std::string> SplitLine = SStringHelpers::SplitString(InLine, ":");
	if (!SplitLine.empty())
	{
		//We need this to keep an index for the length split below
		int CommandIndex = -1;

		//Get the command we are currently wanting to issue
		const EClientCommand Command = GetCommand(SplitLine[0], CommandIndex);

		//Make the first split string, the callsign only (remove the command identifier
		SplitLine[0] = SplitLine[0].substr(strlen(CLIENT_COMMANDS[CommandIndex]), SplitLine[0].length() - 1);

		switch (Command)
		{
		case EClientCommand::CC_ADD_ATC:
			AddController(InConnectionHandle, SplitLine);
			break;
		case EClientCommand::CC_ADD_PILOT:
			break;
		case EClientCommand::CC_ATC_POSITION:
			ProcessControllerPosition(InConnectionHandle, SplitLine);
			break;
		case EClientCommand::CC_CLOUD_DATA:
			break;
		case EClientCommand::CC_ACKNOWLEGE_HANDOFF:
			break;
		case EClientCommand::CC_CQ:
			break;
		case EClientCommand::CC_CR:
			break;
		case EClientCommand::CC_ERROR:
			break;
		case EClientCommand::CC_FLIGHT_PLAN:
			break;
		case EClientCommand::CC_KILL:
			break;
		case EClientCommand::CC_MESSAGE:
			HandleMessage(InConnectionHandle, SplitLine);
			break;
		case EClientCommand::CC_PC:
			break;
		case EClientCommand::CC_PILOT_POSITION:
			break;
		case EClientCommand::CC_PING:
			break;
		case EClientCommand::CC_PONG:
			break;
		case EClientCommand::CC_REMOVE_ATC:
			break;
		case EClientCommand::CC_REMOVE_PILOT:
			break;
		case EClientCommand::CC_REQUEST_ACARS:
			break;
		case EClientCommand::CC_REQUEST_COMM:
			break;
		case EClientCommand::CC_REQUEST_HANDOFF:
			break;
		case EClientCommand::CC_REQUEST_WEATHER:
			break;
		case EClientCommand::CC_SB:
			break;
		case EClientCommand::CC_TEMP_DATA:
			break;
		case EClientCommand::CC_WDELTA:
			break;
		case EClientCommand::CC_WEATHER:
			break;
		case EClientCommand::CC_WIND_DATA:
			break;
		default: break;
		}
	}
}

void CClientServer::HandleMessage(const con_handle_t& InConnectionHandle, const StringVec& Strings)
{
	for (auto& ClientConn : ClientConnections)
	{
		if (!ClientConn.bSocketHasConnection || ClientConn.User == nullptr || ClientConn.User->Callsign != Strings[1])
		{
			continue;
		}
		std::string S1 = "#TM";
		const std::string S2 = str(
			boost::format("%s:%s:%s\n") % InConnectionHandle->User->Callsign % Strings[1] % Strings[2]);
		S1.append(S2);
		const std::shared_ptr<std::basic_string<char>> Buff = std::make_shared<std::string>(S1);
		write(ClientConn.socket, boost::asio::buffer(*Buff));
		break;
	}
}

void CClientServer::AddController(const con_handle_t& InConnectionHandle, const StringVec& InControllerData)
{
	//ATC command should be 5 strings!

	//if (Strings.size() < 7)
	//{
	//	ShowErrorAndDisconnect(EClientError::CE_SYNATX);
	//	return;
	//}

	if (!Certificates.lock())
	{
		ShowErrorAndDisconnect(InConnectionHandle, InControllerData[0], EClientError::CE_CIDINVALID);
		return;
	}

	if (Certificates.lock())
	{
		if (!Certificates.lock()->IsCertificateValid(InControllerData[2], InControllerData[3]))
		{
			ShowErrorAndDisconnect(InConnectionHandle, InControllerData[0], EClientError::CE_CIDINVALID);
			return;
		}
	}

	//if (!CallsignOK(Strings[0]))
	//{
	//	ShowErrorAndDisconnect(EClientError::CE_CSINVALID);
	//	return;
	//}

	if (InControllerData.size() > 5)
	{
		for (auto& Conn : ClientConnections)
		{
			if (Conn.bSocketHasConnection && Conn.User && Conn.User->Callsign == InControllerData[0])
			{
				//Callsign already in use
				return;
			}
		}

		//All checks passed, create the new user
		CUser* NewUser = new CUser();
		NewUser->ConnectionType = EConnectionType::ATC;
		NewUser->RealName = InControllerData[1];
		NewUser->Callsign = InControllerData[0];
		NewUser->CID = InControllerData[2];
		NewUser->Alive = std::time(nullptr);
		InConnectionHandle->User = NewUser;

		SendServerMessage(InConnectionHandle, NewUser, "Welcome to Flight Sim Server 1.0 Beta");
		SendServerMessage(InConnectionHandle, NewUser, "Please note bugs will exist!");
	}
}

void CClientServer::ShowErrorAndDisconnect(const con_handle_t& InConnectionHandle, const std::string& Callsign,
                                           EClientError Error)
{
	std::string ErrMessage;
	switch (Error)
	{
	case EClientError::CE_CIDINVALID:
		ErrMessage = "This CID/Password is not correct, please try again, disconnecting!";
		break;
	case EClientError::CE_CSINUSE:
		ErrMessage = "This callsign is in use, disconnecting!";
		break;
	case EClientError::CE_CSINVALID:
		ErrMessage = "This callsign is invalid, disconnecting!";
		break;
	case EClientError::CE_LEVEL:
		ErrMessage = "Your level is not high enough for this CID, disconnecting!";
		break;
	case EClientError::CE_NOFP:
		ErrMessage = "No flightplan, disconnecting!";
		break;
	case EClientError::CE_NOSUCHCS:
		ErrMessage = "No such callsign, disconnecting!";
		break;
	case EClientError::CE_NOTRATING:
		ErrMessage = "This rating is not appropriate to this CID, disconnecting!";
		break;
	case EClientError::CE_NOTSTAFF:
		ErrMessage = "This is not a valid staff position, disconnecting!";
		break;
	case EClientError::CE_NOTVALIDCS:
		ErrMessage = "This is not a valid callsign, disconnecting!";
		break;
	case EClientError::CE_NOTVALIDPOS:
		ErrMessage = "This is not a valid position, disconnecting!";
		break;
	case EClientError::CE_NOWEATHER:
		ErrMessage = "No weather, disconnecting!";
		break;
	case EClientError::CE_REGISTERED:
		ErrMessage = "You are already registered, disconnecting!";
		break;
	case EClientError::CE_REVISION:
		ErrMessage = "Your client is too old, disconnecting!";
		break;
	case EClientError::CE_SERVERFULL:
		ErrMessage = "Sorry server is full, disconnecting!";
		break;
	case EClientError::CE_SRCINVALID:
		ErrMessage = "Packet src invalid, disconnecting!";
		break;
	case EClientError::CE_SUSPENDED:
		ErrMessage = "You are suspended, disconnecting!";
		break;
	case EClientError::CE_SYNATX:
		ErrMessage = "Invalid Syntax, disconnecting!";
		break;
	default: break;
	}


	std::string S2 = str(boost::format("#TMserver:%s:%s\r\n") % Callsign % ErrMessage);
	std::shared_ptr<std::basic_string<char>> Buff = std::make_shared<std::string>(S2);
	auto Handler = boost::bind(&CClientServer::HandleWrite, shared_from_this(), InConnectionHandle, nullptr, nullptr,
	                           Buff, boost::asio::placeholders::error);
	async_write(InConnectionHandle->socket, boost::asio::buffer(*Buff), Handler);
	InConnectionHandle->bWantsToRemoveAfterWrite = true;
}

void CClientServer::ProcessControllerPosition(const con_handle_t& InConnectionHandle, StringVec& InPositionData)
{
	//This should be handled better, why do we need to loop over every client?
	//Could we make a more generic faster MC function?

	for (std::list<SConnection>::value_type& Conn : ClientConnections)
	{
		if (!Conn.bSocketHasConnection)
		{
			continue;
		}

		if (Conn.User->Callsign == InConnectionHandle->User->Callsign)
		{
			//Don't broadcast out if its ourselves, but do update our data!
			Conn.User->FacType = static_cast<EFacilityType>(std::stoi(InPositionData[2]));
			Conn.User->VisualRange = Conn.User->GetRange();
			Conn.User->Lat = std::stod(InPositionData[4]);
			Conn.User->Lon = std::stod(InPositionData[5]);
			Conn.User->Altitude = std::stoi(InPositionData[6]);
			continue;
		}
		std::string Prefix = "%";
		const std::string S2 = str(
			boost::format("%s:%s:%s:%s:%s:%s:%s\r\n") % InPositionData[0] % InPositionData[1] % InPositionData[2] %
			InPositionData[3] % InPositionData[4] % InPositionData[5] % InPositionData[6]);
		Prefix.append(S2);
		std::shared_ptr<std::basic_string<char>> Buff = std::make_shared<std::string>(Prefix);
		write(Conn.socket, boost::asio::buffer(*Buff));
	}
}

void CClientServer::DoAsyncRead(const con_handle_t& InConnectionHandle)
{
	if (InConnectionHandle->bWantsToRemove)
	{
		HandleClientDisconnect(InConnectionHandle);
		return;
	}
	auto Handler = boost::bind(&CClientServer::HandleRead, shared_from_this(), InConnectionHandle,
	                           boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	async_read_until(InConnectionHandle->socket, InConnectionHandle->read_buffer, "\n", Handler);
}

void CClientServer::SendServerMessage(const con_handle_t& InConnectionHandle, CUser* InTargetUser,
                                      const std::string& InMessage)
{
	if (!InTargetUser)
	{
		return;
	}

	SConnection* TargetConn = nullptr;
	for (auto& ClientCons : ClientConnections)
	{
		if (!ClientCons.bSocketHasConnection)
		{
			continue;
		}

		if (ClientCons.User && ClientCons.User->Callsign == InTargetUser->Callsign)
		{
			TargetConn = &ClientCons;
		}
	}

	if (!TargetConn)
	{
		return;
	}

	std::string S2 = str(boost::format("#TMserver:%s:%s\r\n") % InTargetUser->Callsign % InMessage);
	const std::shared_ptr<std::basic_string<char>> Buff = std::make_shared<std::string>(S2);
	auto Handler = boost::bind(&CClientServer::HandleWrite, shared_from_this(), InConnectionHandle,
	                           InConnectionHandle->User, TargetConn->User, Buff, boost::asio::placeholders::error);
	async_write(TargetConn->socket, boost::asio::buffer(*Buff), Handler);
}

void CClientServer::HandleWrite(const con_handle_t& InConnectionHandle, CUser* SrcUser, CUser* TargetUser,
                                const MsgBuffer& InMessageBuffer, boost::system::error_code const& InError)
{
	if (!InError)
	{
		std::cout << "Finished sending message\n";
	}
	else
	{
		std::cerr << "We had an error: " << InError.message() << std::endl;
	}

	if (InConnectionHandle->bWantsToRemoveAfterWrite)
	{
		InConnectionHandle->bWantsToRemove = true;
	}
}

void CClientServer::HandleWrite(const con_handle_t& InConnectionHandle, const MsgBuffer& MessageBuffer,
                                boost::system::error_code const& Err)
{
	if (!Err)
	{
		std::cout << "Finished sending message\n";
		if (InConnectionHandle->socket.is_open())
		{
			// Write completed successfully and connection is open
		}
	}
	else
	{
		std::cerr << "We had an error: " << Err.message() << std::endl;
		HandleClientDisconnect(InConnectionHandle);
	}

	if (InConnectionHandle->bWantsToRemoveAfterWrite)
	{
		InConnectionHandle->bWantsToRemove = true;
	}
}

void CClientServer::HandleAccept(const con_handle_t& InConnectionHandle, boost::system::error_code const& InError)
{
	if (InConnectionHandle->bWantsToRemove)
	{
		HandleClientDisconnect(InConnectionHandle);
		return;
	}

	if (!InError)
	{
		CurrentClientIndex++;
		const std::time_t Result = std::time(nullptr);
		InConnectionHandle->LastResponseTime = Result;
		InConnectionHandle->ClientIndex = CurrentClientIndex;
		InConnectionHandle->bSocketHasConnection = true;

		//construct new user here, we don't add them to the map yet, cause we are not sure if they are valid client
		//at this point, we have just pre-initialized them here. Once we recieve auth, we will go ahead and update
		//them and add them to the clients dictionary!
		CUser* NewUser = new CUser();
		InConnectionHandle->User = NewUser;

		std::cout << "Connection from: " << InConnectionHandle->socket.remote_endpoint().address().to_string() << "\n";

		DoAsyncRead(InConnectionHandle);
	}
	else
	{
		std::cerr << "We had an error: " << InError.message() << std::endl;
		HandleClientDisconnect(InConnectionHandle);
	}
	StartAccept();
}

void CClientServer::HandleClientDisconnect(const con_handle_t& InConnectionHandle)
{
	//Here we handle when a client has gone away
	//Atm we just remove him from the array, but other things can happen!
	ClientConnections.erase(InConnectionHandle);
}

void CClientServer::StartAccept()
{
	con_handle_t ConHandle = ClientConnections.emplace(ClientConnections.begin(), ClientIoService);

	auto Handler = boost::bind(&CClientServer::HandleAccept, shared_from_this(), ConHandle,
	                           boost::asio::placeholders::error);
	ClientAcceptor.async_accept(ConHandle->socket, Handler);
}

void CClientServer::Listen(const uint16_t InListenPort)
{
	const auto Endpoint = tcp::endpoint(tcp::v4(), InListenPort);
	ClientAcceptor.open(Endpoint.protocol());
	ClientAcceptor.set_option(tcp::acceptor::reuse_address(true));
	ClientAcceptor.bind(Endpoint);
	ClientAcceptor.listen();
	StartAccept();
}

void CClientServer::Poll()
{
	ClientIoService.poll();


	const std::time_t CurrentTime = std::time(nullptr);
	int Index = 0;
	for (con_handle_t RevIt = ClientConnections.begin(); RevIt != ClientConnections.end(); ++RevIt)
	{
		if (RevIt->bSocketHasConnection == false)
		{
			continue;
		}

		if ((CurrentTime - RevIt->LastResponseTime) > CLIENT_TIMEOUT_VALUE)
		{
			ShowIdleDisconnect(RevIt);
			RevIt->bWantsToRemove = true;
		}
		Index++;
	}
}

void CClientServer::ShowIdleDisconnect(const con_handle_t& Conn)
{
	std::string S2 = str(
		boost::format("#TMserver:%s:Disconnected for being idle. Please do not idle.\r\n") % Conn->User->Callsign);
	const std::shared_ptr<std::basic_string<char>> Buff = std::make_shared<std::string>(S2);
	auto Handler = boost::bind(&CClientServer::HandleWrite, shared_from_this(), Conn, Conn->User, Conn->User, Buff,
	                           boost::asio::placeholders::error);
	async_write(Conn->socket, boost::asio::buffer(*Buff), Handler);
	Conn->bWantsToRemoveAfterWrite = true;
}
