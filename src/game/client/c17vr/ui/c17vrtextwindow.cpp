//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "c17vrtextwindow.h"
#include "backgroundpanel.h"
#include <cdll_client_int.h>

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <filesystem.h>
#include <KeyValues.h>
#include <convar.h>
#include <vgui_controls/ImageList.h>

#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/BuildGroup.h>

#include "IGameUIFuncs.h" // for key bindings
#include <igameresources.h>
extern IGameUIFuncs *gameuifuncs; // for key binding details

#include <game/client/iviewport.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CC17VRTextWindow::CC17VRTextWindow(IViewPort *pViewPort) : CTextWindow( pViewPort )
{
	SetProportional( true );

	m_iScoreBoardKey = BUTTON_CODE_INVALID; // this is looked up in Activate()

	CreateBackground( this );
	m_backgroundLayoutFinished = false;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CC17VRTextWindow::~CC17VRTextWindow()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17VRTextWindow::Update()
{
	BaseClass::Update();

	m_pOK->RequestFocus();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17VRTextWindow::SetVisible(bool state)
{
	BaseClass::SetVisible(state);

	if ( state )
	{
		m_pOK->RequestFocus();
	}
}

//-----------------------------------------------------------------------------
// Purpose: shows the text window
//-----------------------------------------------------------------------------
void CC17VRTextWindow::ShowPanel(bool bShow)
{
	if ( bShow )
	{
		// get key binding if shown
		if ( m_iScoreBoardKey == BUTTON_CODE_INVALID ) // you need to lookup the jump key AFTER the engine has loaded
		{
			m_iScoreBoardKey = gameuifuncs->GetButtonCodeForBind( "showscores" );
		}
	}

	BaseClass::ShowPanel( bShow );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17VRTextWindow::OnKeyCodePressed(KeyCode code)
{
	if ( m_iScoreBoardKey != BUTTON_CODE_INVALID && m_iScoreBoardKey == code )
	{
		gViewPortInterface->ShowPanel( PANEL_SCOREBOARD, true );
		gViewPortInterface->PostMessageToPanel( PANEL_SCOREBOARD, new KeyValues( "PollHideCode", "code", code ) );
	}
	else
	{
		BaseClass::OnKeyCodePressed( code );
	}
}

//-----------------------------------------------------------------------------
// Purpose: The CS background is painted by image panels, so we should do nothing
//-----------------------------------------------------------------------------
void CC17VRTextWindow::PaintBackground()
{
}

//-----------------------------------------------------------------------------
// Purpose: Scale / center the window
//-----------------------------------------------------------------------------
void CC17VRTextWindow::PerformLayout()
{
	BaseClass::PerformLayout();

	// stretch the window to fullscreen
	if ( !m_backgroundLayoutFinished )
		LayoutBackgroundPanel( this );
	m_backgroundLayoutFinished = true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CC17VRTextWindow::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	ApplyBackgroundSchemeSettings( this, pScheme );
}

CC17VRSpectatorGUI::CC17VRSpectatorGUI(IViewPort *pViewPort) : CSpectatorGUI(pViewPort)
{
}


bool CC17VRSpectatorGUI::NeedsUpdate( void )
{
	if ( !C_BasePlayer::GetLocalPlayer() )
		return false;

	if ( m_nLastSpecMode != C_BasePlayer::GetLocalPlayer()->GetObserverMode() )
		return true;

	if ( m_nLastSpecTarget != C_BasePlayer::GetLocalPlayer()->GetObserverTarget() )
		return true;

	return BaseClass::NeedsUpdate();
}


void CC17VRSpectatorGUI::Update()
{
	BaseClass::Update();

	C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();

	if( pLocalPlayer )
	{
		m_nLastSpecMode = pLocalPlayer->GetObserverMode();
		m_nLastSpecTarget = pLocalPlayer->GetObserverTarget();
	}
}


