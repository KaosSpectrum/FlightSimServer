#pragma once
#include "string"

struct SFlightPlan
{
public:
	SFlightPlan(const std::string& Callsign, int Revision, const std::string& Type, const std::string& Aircraft, int TasCruise, const std::string& DepAirport,
				int DepTime, int ActualDepTime, const std::string& Altitude, const std::string& DestinationAirport, int EnrouteHours, int MinEnRoute,
				int HoursFuel, int MinFuel, const std::string& AltAirport, const std::string& Remarks, const std::string& Route);

	SFlightPlan() = default;

	std::string m_Callsign;
	int m_Revision{};
	std::string m_Type;
	std::string m_Aircraft;
	int MTasCruise{};
	std::string m_DepAirport;
	int m_DepTime{};
	int m_ActualDepTime{};
	std::string m_Altitude;
	std::string m_DestAirport;
	int m_EnrouteHours{};
	int m_MinEnroute{};
	int m_HoursFuel{};
	int m_MinFuel{};
	std::string m_AltAiport;
	std::string m_Remarks;
	std::string m_Route;

};

