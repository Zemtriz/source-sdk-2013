//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "weapon_c17vrbase.h"
#include "weapon_c17vrbasehlmpcombatweapon.h"

#ifndef BASEHLCOMBATWEAPON_H
#define BASEHLCOMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
	#define CC17VRMachineGun C_C17VRMachineGun
#endif

//=========================================================
// Machine gun base class
//=========================================================
class CC17VRMachineGun : public CBaseC17VRCombatWeapon
{
public:
	DECLARE_CLASS( CC17VRMachineGun, CBaseC17VRCombatWeapon );
	DECLARE_DATADESC();

	CC17VRMachineGun();
	
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	void	PrimaryAttack( void );

	// Default calls through to m_hOwner, but plasma weapons can override and shoot projectiles here.
	virtual void	ItemPostFrame( void );
	virtual void	FireBullets( const FireBulletsInfo_t &info );
	virtual bool	Deploy( void );

	virtual const Vector &GetBulletSpread( void );

	int				WeaponSoundRealtime( WeaponSound_t shoot_type );

	// utility function
	static void DoMachineGunKick( CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime );

private:
	
	CC17VRMachineGun( const CC17VRMachineGun & );

protected:

	int	m_nShotsFired;	// Number of consecutive shots fired

	float	m_flNextSoundTime;	// real-time clock of when to make next sound
};

#endif // BASEHLCOMBATWEAPON_H
