// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once

enum class EClientCommand
{
	CC_ADD_ATC,
	CC_REMOVE_ATC,
	CC_ADD_PILOT,
	CC_REMOVE_PILOT,
	CC_REQUEST_HANDOFF,
	CC_MESSAGE,
	CC_REQUEST_WEATHER,
	CC_PILOT_POSITION,
	CC_ATC_POSITION,
	CC_PING,
	CC_PONG,
	CC_ACKNOWLEGE_HANDOFF,
	CC_FLIGHT_PLAN,
	CC_SB,
	CC_PC,
	CC_WEATHER,
	CC_CLOUD_DATA,
	CC_WIND_DATA,
	CC_TEMP_DATA,
	CC_REQUEST_COMM,
	CC_REQUEST_ACARS,
	CC_ERROR,
	CC_CQ,
	CC_CR,
	CC_KILL,
	CC_WDELTA,
	CC_NOTHING,
};

enum class EClientError
{
	CE_OK,
	CE_CSINUSE,
	CE_CSINVALID,
	CE_REGISTERED,
	CE_SYNATX,
	CE_SRCINVALID,
	CE_CIDINVALID,
	CE_NOSUCHCS,
	CE_NOFP,
	CE_NOWEATHER,
	CE_REVISION,
	CE_LEVEL,
	CE_SERVERFULL,
	CE_SUSPENDED,
	CE_NOTRATING,
	CE_NOTVALIDCS,
	CE_NOTVALIDPOS,
	CE_NOTSTAFF,
	CE_IDLE,
};

enum class EConnectionType
{
	PILOT,
	ATC
};

enum class EFacilityType
{
	NONE,
	FSS,
	DEL,
	GND,
	TWR,
	APP,
	CTR,
	OBS
};
