//========= Copyright Valve Corporation, All rights reserved. ============//
#ifndef WEAPON_BASEC17VRCOMBATWEAPON_SHARED_H
#define WEAPON_BASEC17VRCOMBATWEAPON_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#ifdef CLIENT_DLL
	#include "c_c17vr_player.h"
#else
	#include "c17vr_player.h"
#endif

#include "weapon_c17vrbase.h"

#if defined( CLIENT_DLL )
#define CBaseC17VRCombatWeapon C_BaseC17VRCombatWeapon
#endif

class CBaseC17VRCombatWeapon : public CWeaponC17VRBase
{
#if !defined( CLIENT_DLL )
	DECLARE_DATADESC();
#endif

	DECLARE_CLASS( CBaseC17VRCombatWeapon, CWeaponC17VRBase );
public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CBaseC17VRCombatWeapon();

	virtual bool	WeaponShouldBeLowered( void );

	virtual bool	Ready( void );
	virtual bool	Lower( void );
	virtual bool	Deploy( void );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual void	WeaponIdle( void );

	virtual void	AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles );
	virtual	float	CalcViewmodelBob( void );

	virtual Vector	GetBulletSpread( WeaponProficiency_t proficiency );
	virtual float	GetSpreadBias( WeaponProficiency_t proficiency );

	virtual const	WeaponProficiencyInfo_t *GetProficiencyValues();
	static const	WeaponProficiencyInfo_t *GetDefaultProficiencyValues();

	virtual void	ItemHolsterFrame( void );

protected:

	bool			m_bLowered;			// Whether the viewmodel is raised or lowered
	float			m_flRaiseTime;		// If lowered, the time we should raise the viewmodel
	float			m_flHolsterTime;	// When the weapon was holstered

private:
	
	CBaseC17VRCombatWeapon( const CBaseC17VRCombatWeapon & );
};

#endif // WEAPON_BASEHL2MPCOMBATWEAPON_SHARED_H
