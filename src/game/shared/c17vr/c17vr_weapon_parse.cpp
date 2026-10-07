//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include <KeyValues.h>
#include "c17vr_weapon_parse.h"
#include "ammodef.h"

FileWeaponInfo_t* CreateWeaponInfo()
{
	return new CC17VRSWeaponInfo;
}



CC17VRSWeaponInfo::CC17VRSWeaponInfo()
{
	m_iPlayerDamage = 0;
}


void CC17VRSWeaponInfo::Parse( KeyValues *pKeyValuesData, const char *szWeaponName )
{
	BaseClass::Parse( pKeyValuesData, szWeaponName );

	m_iPlayerDamage = pKeyValuesData->GetInt( "damage", 0 );
}


