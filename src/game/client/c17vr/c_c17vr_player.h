//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef C17VR_PLAYER_H
#define C17VR_PLAYER_H
#pragma once

class C_C17VR_Player;
#include "c_basehlplayer.h"
#include "c17vr_player_shared.h"
#include "beamdraw.h"

//=============================================================================
//=============================================================================
class CC17VRSuitPowerDevice
{
public:
	CC17VRSuitPowerDevice( int bitsID, float flDrainRate ) { m_bitsDeviceID = bitsID; m_flDrainRate = flDrainRate; }
private:
	int		m_bitsDeviceID;	// tells what the device is. DEVICE_SPRINT, DEVICE_FLASHLIGHT, etc. BITMASK!!!!!
	float	m_flDrainRate;	// how quickly does this device deplete suit power? ( percent per second )

public:
	int		GetDeviceID( void ) const { return m_bitsDeviceID; }
	float	GetDeviceDrainRate( void ) const
	{
		//if ( g_pGameRules->GetSkillLevel() == SKILL_EASY && hl2_episodic.GetBool() && !( GetDeviceID() & bits_SUIT_DEVICE_SPRINT ) )
		//	return m_flDrainRate * 0.5f;
		//else
			return m_flDrainRate;
	}
};

extern ConVar hl2_sprintspeed;

//=============================================================================
// >> HL2MP_Player
//=============================================================================
class C_C17VR_Player : public C_BaseHLPlayer
{
public:
	DECLARE_CLASS( C_C17VR_Player, C_BaseHLPlayer );

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();


	C_C17VR_Player();
	~C_C17VR_Player( void );

	void ClientThink( void );

	static C_C17VR_Player* GetLocalC17VRPlayer();
	
	virtual int DrawModel( int flags );
	virtual void AddEntity( void );

	QAngle GetAnimEyeAngles( void ) { return m_angEyeAngles; }
	Vector GetAttackSpread( CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget = NULL );


	// Should this object cast shadows?
	virtual ShadowType_t		ShadowCastType( void );
	virtual C_BaseAnimating *BecomeRagdollOnClient();
	virtual const QAngle& GetRenderAngles();
	virtual bool ShouldDraw( void );
	virtual void OnDataChanged( DataUpdateType_t type );
	virtual float GetFOV( void );
	virtual CStudioHdr *OnNewModel( void );
	virtual void TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator );
	virtual void ItemPreFrame( void );
	virtual void ItemPostFrame( void );
	virtual float GetMinFOV()	const { return 5.0f; }
	virtual Vector GetAutoaimVector( float flDelta );
	virtual void NotifyShouldTransmit( ShouldTransmitState_t state );
	virtual void CreateLightEffects( void ) {}
	virtual bool ShouldReceiveProjectedTextures( int flags );
	virtual void PostDataUpdate( DataUpdateType_t updateType );
	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force );
	virtual void PreThink( void );
	virtual void DoImpactEffect( trace_t &tr, int nDamageType );
	IRagdoll* GetRepresentativeRagdoll() const;
	virtual void CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	virtual const QAngle& EyeAngles( void );

	void SuitPower_Update( void );
	bool SuitPower_Drain( float flPower ); // consume some of the suit's power.
	void SuitPower_Charge( float flPower ); // add suit power.
	void SuitPower_SetCharge( float flPower ) { m_HL2Local.m_flSuitPower = flPower; }
	void SuitPower_Initialize( void );
	bool SuitPower_IsDeviceActive( const CC17VRSuitPowerDevice& device );
	bool SuitPower_AddDevice( const CC17VRSuitPowerDevice& device );
	bool SuitPower_RemoveDevice( const CC17VRSuitPowerDevice& device );
	bool SuitPower_ShouldRecharge( void );
	float SuitPower_GetCurrentPercentage( void ) { return m_HL2Local.m_flSuitPower; }
	
	bool	CanSprint( void );
	void	StartSprinting( void );
	void	StopSprinting( void );
	virtual void	HandleSpeedChanges( CMoveData *mv ) OVERRIDE;
	virtual void	ReduceTimers( CMoveData* mv ) OVERRIDE;
	void	UpdateLookAt( void );
	void	Initialize( void );
	int		GetIDTarget() const;
	void	UpdateIDTarget( void );
	void	PrecacheFootStepSounds( void );
	const char	*GetPlayerModelSoundPrefix( void );

	C17VRPlayerState State_Get() const;

	// Walking
	void StartWalking( void );
	void StopWalking( void );
	bool IsWalking( void ) { return m_fIsWalking; }

	virtual void PostThink( void );

private:
	
	C_C17VR_Player( const C_C17VR_Player & );

	CVRPlayerAnimState m_PlayerAnimState;

	QAngle	m_angEyeAngles;

	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	EHANDLE	m_hRagdoll;

	int	m_headYawPoseParam;
	int	m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;

	bool m_isInit;
	Vector m_vLookAtTarget;

	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;

	int	  m_iIDEntIndex;

	CountdownTimer m_blinkTimer;

	int	  m_iSpawnInterpCounter;
	int	  m_iSpawnInterpCounterCache;

	int	  m_iPlayerSoundType;

	void ReleaseFlashlight( void );
	Beam_t	*m_pFlashlightBeam;

	CNetworkVar( C17VRPlayerState, m_iPlayerState );	

	bool m_fIsWalking = false;
};

inline C_C17VR_Player *ToC17VRPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<C_C17VR_Player*>( pEntity );
}


class C_C17VRRagdoll : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS( C_C17VRRagdoll, C_BaseAnimatingOverlay );
	DECLARE_CLIENTCLASS();
	
	C_C17VRRagdoll();
	~C_C17VRRagdoll();

	virtual void OnDataChanged( DataUpdateType_t type );

	int GetPlayerEntIndex() const;
	IRagdoll* GetIRagdoll() const;

	void ImpactTrace( trace_t *pTrace, int iDamageType, const char *pCustomImpactName );
	void UpdateOnRemove( void );
	virtual void SetupWeights( const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights );
	
private:
	
	C_C17VRRagdoll( const C_C17VRRagdoll & ) {}

	void Interp_Copy( C_BaseAnimatingOverlay *pDestinationEntity );
	void CreateC17VRRagdoll( void );

private:

	EHANDLE	m_hPlayer;
	CNetworkVector( m_vecRagdollVelocity );
	CNetworkVector( m_vecRagdollOrigin );
};

#endif //HL2MP_PLAYER_H
