/*=========================================================================

	pmboots.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player/pmboots.h"

#ifndef __PLAYER_PLAYER_H__
#include "player/player.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int boottime=60*20;
void	CPlayerModeBoots::enter(class CPlayer *_player)
{
	m_timer=boottime;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBoots::think(class CPlayer *_player)
{
	if(--m_timer==0)
	{
		_player->setMode(PLAYER_MODE_FULLUNARMED);
	}
}

/*===========================================================================
end */
