//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef C17VR_PLAYER_SHARED_H
#define C17VR_PLAYER_SHARED_H
#pragma once

#define C17VR_PUSHAWAY_THINK_INTERVAL		(1.0f / 20.0f)
#include "studio.h"


enum
{
	VR_PLAYER_SOUNDS_CITIZEN = 0,
	VR_PLAYER_SOUNDS_COMBINESOLDIER,
	VR_PLAYER_SOUNDS_METROPOLICE,
	VR_PLAYER_SOUNDS_MAX,
};

enum C17VRPlayerState
{
	// Happily running around in the game.
	VR_STATE_ACTIVE=0,
	VR_STATE_OBSERVER_MODE,		// Noclipping around, watching players, etc.
	VR_NUM_PLAYER_STATES
};


#if defined( CLIENT_DLL )
#define CC17VR_Player C_C17VR_Player
#endif

class CVRPlayerAnimState
{
public:
	enum
	{
		TURN_NONE = 0,
		TURN_LEFT,
		TURN_RIGHT
	};

	CVRPlayerAnimState( CC17VR_Player *outer );

	Activity			BodyYawTranslateActivity( Activity activity );

	void				Update();

	const QAngle&		GetRenderAngles();
				
	void				GetPoseParameters( CStudioHdr *pStudioHdr, float poseParameter[MAXSTUDIOPOSEPARAM] );

	CC17VR_Player		*GetOuter();

private:
	void				GetOuterAbsVelocity( Vector& vel );

	int					ConvergeAngles( float goal,float maxrate, float dt, float& current );

	void				EstimateYaw( void );
	void				ComputePoseParam_BodyYaw( void );
	void				ComputePoseParam_BodyPitch( CStudioHdr *pStudioHdr );
	void				ComputePoseParam_BodyLookYaw( void );

	void				ComputePlaybackRate();

	CC17VR_Player		*m_pOuter;

	float				m_flGaitYaw;
	float				m_flStoredCycle;

	// The following variables are used for tweaking the yaw of the upper body when standing still and
	//  making sure that it smoothly blends in and out once the player starts moving
	// Direction feet were facing when we stopped moving
	float				m_flGoalFeetYaw;
	float				m_flCurrentFeetYaw;

	float				m_flCurrentTorsoYaw;

	// To check if they are rotating in place
	float				m_flLastYaw;
	// Time when we stopped moving
	float				m_flLastTurnTime;

	// One of the above enums
	int					m_nTurningInPlace;

	QAngle				m_angRender;

	float				m_flTurnCorrectionTime;
};

#endif //HL2MP_PLAYER_SHARED_h
