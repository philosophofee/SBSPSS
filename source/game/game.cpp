/**********************/
/*** Main Game File ***/
/**********************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"pad\pads.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"
#include	"utils\utils.h"
#include	"level\level.h"
#include	"game\game.h"
#include	"system\gstate.h"
#include	"gfx\font.h"
#include	"gfx\fdata.h"

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif
#include "player\demoplay.h"

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __JELLFISH_JELLFISH_H__
#include "jellfish\jellfish.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __GFX_BUBICLES__
#include "gfx\bubicles.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef _ANIMTEX_HEADER_
#include "gfx\animtex.h"
#endif

#ifndef	__GAME_PAUSE_H__
#include "game\pause.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef __SHOP_SHOP_H__
#include "shop\shop.h"
#endif

#ifndef __FMA_FMA_H__
#include "fma\fma.h"
#endif


#include "gfx\actor.h"

int			RenderZ=378;//256; Increased to make depth less, and SB more visible

/*****************************************************************************/

FontBank		*CGameScene::s_genericFont;
SpriteBank		*CGameScene::s_GlobalSpritebank;
CLayerCollision	*CGameScene::s_GlobalCollision;
MATRIX			CGameScene::CamMtx;

/*****************************************************************************/
CGameScene::ACTOR_TYPE CGameScene::actorType[40] =
{
	ACTOR_PLAYER,				//SpongeBob=0
	ACTOR_FRIEND_NPC,			//BarnacleBoy=1
	ACTOR_FRIEND_NPC,			//Gary=2
	ACTOR_FRIEND_NPC,			//Krusty=3
	ACTOR_FRIEND_NPC,			//MermaidMan=4
	ACTOR_FRIEND_NPC,			//Patrick=5
	ACTOR_FRIEND_NPC,			//Sandy=6
	ACTOR_FRIEND_NPC,			//Squidward=7
	ACTOR_FRIEND_NPC,			//Plankton=8
	ACTOR_UNKNOWN,
	ACTOR_ENEMY_NPC,			//SmallJellyfish-Level1=10
	ACTOR_ENEMY_NPC,			//SmallJellyfish-Level2=11
	ACTOR_ENEMY_NPC,			//Motherjellyfish=12
	ACTOR_ENEMY_NPC,			//Anenome-Level1=13
	ACTOR_ENEMY_NPC,			//SpikeyAnenome=14
	ACTOR_ENEMY_NPC,			//Anenome-Level3=15
	ACTOR_ENEMY_NPC,			//BabyOctopus=16
	ACTOR_ENEMY_NPC,			//Ballblob=17
	ACTOR_ENEMY_NPC,			//Boogermonster=18
	ACTOR_ENEMY_NPC,			//Caterpillar=19
	ACTOR_ENEMY_NPC,			//Clam-Level1=20
	ACTOR_ENEMY_NPC,			//Clam-Level2=21
	ACTOR_ENEMY_NPC,			//Eyeball=22
	ACTOR_ENEMY_NPC,			//Flamingskull=23
	ACTOR_ENEMY_NPC,			//FlyingDutchman=24
	ACTOR_ENEMY_NPC,			//Ghost=25
	ACTOR_ENEMY_NPC,			//GiantWorm=26
	ACTOR_ENEMY_NPC,			//HermitCrab=27
	ACTOR_ENEMY_NPC,			//IronDogFish=28
	ACTOR_ENEMY_NPC,			//PuffaFish=29
	ACTOR_ENEMY_NPC,			//SeaSnake=30
	ACTOR_ENEMY_NPC,			//Sharkman=31
	ACTOR_ENEMY_NPC,			//SharkSub=32
	ACTOR_ENEMY_NPC,			//Skeletalfish=33
	ACTOR_ENEMY_NPC,			//SpiderCrab=34
	ACTOR_ENEMY_NPC,			//Squiddart=35
	ACTOR_ENEMY_NPC,			//Stomper=36
	ACTOR_ENEMY_NPC,			//DustDevil=37
	ACTOR_ENEMY_NPC,			//SiderCrabSpawner=38
	ACTOR_ENEMY_NPC,			//Shell=39
};

/*****************************************************************************/

int s_globalLevelSelectThing=0;
int CGameScene::s_readyToExit;
int CGameScene::s_levelFinished;
#ifdef __VERSION_DEBUG__
int CGameScene::s_skipToNextLevel;
#endif
int	CGameScene::s_restartLevel;
int	CGameScene::s_bossHasBeenKilled;

/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
void	CGameScene::AspectCorrectCamera()
{
const s32 Scale = (512<<12)/(256);

		CamMtx.m[0][0] = ((s32)CamMtx.m[0][0] * Scale)>>12;
		CamMtx.m[0][1] = ((s32)CamMtx.m[0][1] * Scale)>>12;
		CamMtx.m[0][2] = ((s32)CamMtx.m[0][2] * Scale)>>12;

		CamMtx.t[0]  = (CamMtx.t[0] * Scale)>>12;
}

/*****************************************************************************/
void 	CGameScene::init()
{
// Setup Constant Camera Matrix
		SetIdentTrans(&CamMtx,0,0,RenderZ);
		SetGeomScreen(RenderZ);
		SetTransMatrix(&CamMtx);		

		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );
		VidSetClearScreen(0);



		m_pauseMenu=new ("Pause Menu") CPauseMenu();
		m_pauseMenu->init();

		s_readyToExit=false;
		s_restartLevel=false;

		CFader::setFadingIn();
		initLevel();

}
/*****************************************************************************/
// This is a seperate funtion ( and virtual ) so that we can overload it for
// the demo mode (pkg)
// NB: This function should *only* include the new function - Don't add anything else here!
void	CGameScene::createPlayer()
{
		m_player=new ("player") CPlayer();
		m_player->setThingSubType(0);
}

// This is also to be overloaded for demomode.. to stop the pause menu appearing
int		CGameScene::canPause()
{
		return true;
}


/*****************************************************************************/

void	CGameScene::shutdown()
{
		shutdownLevel();

		m_pauseMenu->shutdown();	delete m_pauseMenu;
		s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
void 	CGameScene::render()
{
//		CamMtx.t[2]=ZPos;	// Temp

		m_pauseMenu->render();
		CConversation::render();
		CThingManager::renderAllThings();

		SetIdentTrans(&CamMtx,0,0,RenderZ);
		SetGeomScreen(RenderZ);
		SetRotMatrix(&CamMtx);
		SetTransMatrix(&CamMtx);

		Level.render();
		CActorPool::CleanUpCache();
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
//#ifdef __USER_paul__
//	if(!CConversation::isActive()&&PadGetDown(0)&PAD_START)
//	{
//		CConversation::trigger(SCRIPTS_SPEECHTEST_DAT);
//	}
//#endif

	if(s_readyToExit)
	{
		// Temporarily.. exiting game scene always goes back to the front end (pkg)
		GameState::setNextScene(&FrontEndScene);
	}
	else if(s_levelFinished)
	{
		// Level finished - go to map or fma
		if(getLevelNumber()==4)
		{
			GameState::setNextScene(&FmaScene);
		}
		else if(getLevelNumber()==5)
		{
			if(getChapterNumber()==5)
			{
				ShopScene.setGotoPartyScreen();
			}
			GameState::setNextScene(&ShopScene);
		}
		else
		{
			GameState::setNextScene(&MapScene);
		}
		s_readyToExit=true;
	}
#ifdef __VERSION_DEBUG__
	else if(s_skipToNextLevel)
	{
		// Skip to next level
		bool	Finished=Level.GetNextLevel(s_globalLevelSelectThing);
		shutdownLevel();
		initLevel();
		s_skipToNextLevel=false;
	}
#endif
	else if (s_restartLevel)
	{
		if(m_player->getLivesLeft()!=0)
		{
			respawnLevel();
		}
		else
		{
			s_readyToExit=true;
			GameState::setNextScene(&FrontEndScene);
		}
		s_restartLevel=false;
	}


	if(!m_pauseMenu->isActive()&&PadGetDown(0)&PAD_START&&canPause())
	{
		m_pauseMenu->select();
	}

	CConversation::think(_frames);
	m_pauseMenu->think(_frames);
	if(!CConversation::isActive()&& !m_pauseMenu->isActive())
	{
		DVECTOR	camPos;
		CJellyfishGenerator::think( _frames, &Level );
		CThingManager::thinkAllThings(_frames);
		camPos=m_player->getCameraPos();
		CBubicleFactory::setMapOffset(&camPos);
		Level.setCameraCentre(camPos);
		Level.think(_frames);

#ifdef __VERSION_DEBUG__
		if(PadGetDown(0)&PAD_R2)
		{
			s_skipToNextLevel=true;
		}
#endif
	}
}


/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return s_readyToExit;
}

/*****************************************************************************/
CPlayer	* CGameScene::getPlayer()
{
	return( m_player );
}

/*****************************************************************************/
int		CGameScene::getChapterNumber()
{
	return Level.getCurrentChapter();
}
int		CGameScene::getLevelNumber()
{
	return Level.getCurrentChapterLevel();
}

int		CGameScene::getTotalSpatCountForThisLevel()
{
	return Level.getTotalSpatCount();
}

/*****************************************************************************/
void	CGameScene::respawnLevel()
{
	m_player->respawn();
	Level.respawnLevel();
}


/*****************************************************************************/
void CGameScene::sendEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CThingManager::processEventAllThings(evt, sourceThing);
}



/*****************************************************************************/
void	CGameScene::initLevel()
{
	SYSTEM_DBGMSG("InitLevel\n");
	CThingManager::init();

	CConversation::init();
	CConversation::registerConversationLevelScripts( s_globalLevelSelectThing );

	s_levelFinished=false;
#ifdef __VERSION_DEBUG__
	s_skipToNextLevel=false;
#endif
	Level.init(s_globalLevelSelectThing);

	createPlayer();
	m_player->setRespawnPos(Level.getPlayerSpawnPos());
	m_player->init();
	DVECTOR				mapSize=Level.getMapSize();
	CPlayer::CameraBox	camBox={0,0,mapSize.vx<<4,mapSize.vy<<4};
	m_player->setCameraBox(camBox);

	s_bossHasBeenKilled=false;

	// Song is loaded/dumped by the level, and played from here. This just gives some
	// better timing over when it starts (pkg)
	CSoundMediator::playSong();
	CActorPool::SetUpCache();
	SYSTEM_DBGMSG("InitLevelDone\n");

}


/*****************************************************************************/
void	CGameScene::shutdownLevel()
{
	CSoundMediator::dumpSong();
	CConversation::shutdown();
	CThingManager::shutdown();
	Level.shutdown();
}

/*****************************************************************************/