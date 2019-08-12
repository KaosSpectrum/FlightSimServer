#include "FlightPlan.h"
#include "StringHelpers.h"


SFlightPlan::SFlightPlan(const std::string& Callsign, int Revision, const std::string& Type, const std::string& Aircraft, int TasCruise, const std::string& DepAirport, int DepTime, int ActualDepTime, const std::string& Altitude, const std::string& DestinationAirport, int EnrouteHours, int MinEnRoute, int HoursFuel, int MinFuel, const std::string& AltAirport, const std::string& Remarks, const std::string& Route)
{
	m_Callsign = SStringHelpers::ToUpper(Callsign);
	m_Type = Type;
	m_Aircraft = SStringHelpers::ToUpper(Aircraft);
	MTasCruise = TasCruise;
	m_DepAirport = SStringHelpers::ToUpper(DepAirport);
	m_DepTime = DepTime;
	m_ActualDepTime = ActualDepTime;
	m_Altitude = Altitude;
	m_EnrouteHours = EnrouteHours;
	m_MinEnroute = MinEnRoute;
	m_HoursFuel = HoursFuel;
	m_MinFuel = MinFuel;
	m_AltAiport = SStringHelpers::ToUpper(AltAirport);
	m_Remarks = Remarks;
	m_Route = Route;
}