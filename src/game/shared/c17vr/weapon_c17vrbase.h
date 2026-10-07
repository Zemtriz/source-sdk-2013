//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef WEAPON_C17VRBASE_H
#define WEAPON_C17VRBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "c17vr_player_shared.h"
#include "basecombatweapon_shared.h"
#include "c17vr_weapon_parse.h"

#if defined( CLIENT_DLL )
	#define CWeaponC17VRBase C_WeaponC17VRBase
	void UTIL_ClipPunchAngleOffset( QAngle &in, const QAngle &punch, const QAngle &clip );
#endif

class CC17VR_Player;

// These are the names of the ammo types that go in the CAmmoDefs and that the 
// weapon script files reference.

// Given an ammo type (like from a weapon's GetPrimaryAmmoType()), this compares it
// against the ammo name you specify.
// MIKETODO: this should use indexing instead of searching and strcmp()'ing all the time.
bool IsAmmoType( int iAmmoType, const char *pAmmoName );

class CWeaponC17VRBase : public CBaseCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponC17VRBase, CBaseCombatWeapon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponC17VRBase();

	#ifdef GAME_DLL
		DECLARE_DATADESC();
	
		void SendReloadSoundEvent( void );

		void Materialize( void );
		virtual	int	ObjectCaps( void );
	#endif

	// All predicted weapons need to implement and return true
	virtual bool	IsPredicted() const;

	CBasePlayer* GetPlayerOwner() const;
	CC17VR_Player* GetC17VRPlayerOwner() const;

	void WeaponSound( WeaponSound_t sound_type, float soundtime = 0.0f );
	
	CC17VRSWeaponInfo const	&GetC17VRWpnData() const;


	virtual void FireBullets( const FireBulletsInfo_t &info );
	virtual void FallInit( void );
	
public:
	#if defined( CLIENT_DLL )
		
		virtual bool	ShouldPredict();
		virtual void	OnDataChanged( DataUpdateType_t type );

		virtual bool	OnFireEvent( C_BaseViewModel *pViewModel, const Vector& origin, const QAngle& angles, int event, const char *options );

	#else

		virtual void	Spawn();

	#endif

	float		m_flPrevAnimTime;
	float  m_flNextResetCheckTime;

	Vector	GetOriginalSpawnOrigin( void ) { return m_vOriginalSpawnOrigin;	}
	QAngle	GetOriginalSpawnAngles( void ) { return m_vOriginalSpawnAngles;	}

private:

	CWeaponC17VRBase( const CWeaponC17VRBase & );

	Vector m_vOriginalSpawnOrigin;
	QAngle m_vOriginalSpawnAngles;
};


#endif // WEAPON_HL2MPBASE_H
