//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#if !defined( CLIENTMODE_HLNORMAL_H )
#define CLIENTMODE_HLNORMAL_H
#ifdef _WIN32
#pragma once
#endif

#include "clientmode_shared.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui/Cursor.h>

class CHudViewport;


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class ClientModeC17VRNormal : public ClientModeShared
{
public:
	DECLARE_CLASS( ClientModeC17VRNormal, ClientModeShared );

	ClientModeC17VRNormal();
	~ClientModeC17VRNormal();

	virtual void	Init();
	virtual int		GetDeathMessageStartHeight( void );
};

extern IClientMode *GetClientModeNormal();
extern vgui::HScheme g_hVGuiCombineScheme;

extern ClientModeC17VRNormal* GetClientModeC17VRNormal();

#endif // CLIENTMODE_HLNORMAL_H
