// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#include "CUser.h"
#include "MathHelper.h"


CUser::CUser() = default;


CUser::~CUser() = default;

void CUser::UpdatePilot(const std::vector<std::string>& Data)
{
}

void CUser::UpdateController(const std::vector<std::string>& Data)
{
}

void CUser::HandleFlightPlan(const std::vector<std::string>& Data)
{
}

void CUser::SetAlive()
{
}

double CUser::GetDistance(CUser* OtherUser) const
{
	if (!OtherUser)
	{
		return -1;
	}

	if (!bPositionOk || !OtherUser->bPositionOk)
	{
		return -1;
	}

	return SMathHelper::Distance(Lat, Lon, OtherUser->Lat, OtherUser->Lon);
}

int CUser::GetRange()
{
	if (ConnectionType == EConnectionType::PILOT)
	{
		if (Altitude < 0)
		{
			Altitude = 0;
		}

		return static_cast<int>(10 + 1.414 * sqrt(static_cast<double>(Altitude)));
	}

	switch (FacType)
	{
	case EFacilityType::APP:
		return 100;
	case EFacilityType::CTR:
		return 400;
	case EFacilityType::DEL:
		return 5;
	case EFacilityType::GND:
		return 5;
	case EFacilityType::TWR:
		return 30;
	case EFacilityType::FSS:
		return 1500;
	case EFacilityType::OBS:
		return 1500;
	case EFacilityType::NONE:
		return 40;
	}

	return 40;
}
