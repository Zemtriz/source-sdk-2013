//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef C17VR_WEAPON_PARSE_H
#define C17VR_WEAPON_PARSE_H
#ifdef _WIN32
#pragma once
#endif


#include "weapon_parse.h"
#include "networkvar.h"


//--------------------------------------------------------------------------------------------------------
class CC17VRSWeaponInfo : public FileWeaponInfo_t
{
public:
	DECLARE_CLASS_GAMEROOT( CC17VRSWeaponInfo, FileWeaponInfo_t );
	
	CC17VRSWeaponInfo();
	
	virtual void Parse( ::KeyValues *pKeyValuesData, const char *szWeaponName );


public:

	int m_iPlayerDamage;
};


#endif // HL2MP_WEAPON_PARSE_H
