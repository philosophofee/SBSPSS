/******************/
/*** Layer Core ***/
/******************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
//#include	"GLEnabledView.h"

//#include	"MapEditDoc.h"
//#include	"MapEditView.h"

#include	"Layer.h"
#include	"Utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayer::CLayer()
{

}
/*****************************************************************************/
CLayer::~CLayer()
{
}

/*****************************************************************************/
void	CLayer::Init()
{

}

/*****************************************************************************/
void	CLayer::SetName(char *Str)
{
	strcpy(Name,Str);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayer::Render()
{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_QUADS);
		glColor3f(1,0,1);
			BuildGLBox(-1,LayerWidth+1,-1,0,LayerZ,LayerZ+1);						// Bottom
			BuildGLBox(-1,LayerWidth+1,LayerHeight+1,LayerHeight,LayerZ,LayerZ+1);	// Top
			BuildGLBox(-1,0,LayerHeight,0,LayerZ,LayerZ+1);							// Left
			BuildGLBox(LayerWidth,LayerWidth+1,LayerHeight,0,LayerZ,LayerZ+1);		// Right
		glEnd();
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


