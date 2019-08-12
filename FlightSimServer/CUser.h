// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once
#include "Enums.h"
#include "FlightPlan.h"
#include "vector"
#include "string"

class CUser
{
public:
	CUser();
	~CUser();

	EConnectionType ConnectionType;
	SFlightPlan FlightPlan;
	time_t StartTime{};
	bool bIsSupervisor{};
	int Type{}, Rating{};
	unsigned int PBH{};
	int Flags{};
	time_t Alive{};
	std::string CID;
	std::string Callsign;
	std::string Protocol;
	std::string RealName;
	std::string Sector;
	std::string IdentFlag;


	double Lat{}, Lon{};

	EFacilityType FacType;

	int Transponder{}, Altitude{}, GroundSpeed{}, Frequency{};
	bool bPositionOk{};
	int VisualRange{}, SimType{};

	void UpdatePilot(const std::vector<std::string>& Data);
	void UpdateController(const std::vector<std::string>& Data);
	void HandleFlightPlan(const std::vector<std::string>& Data);
	void SetAlive();
	double GetDistance(CUser* OtherUser) const;
	int GetRange();
};
