/*********************/
/*** Texture Cache ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<gl\glaux.h>		// Header File For The Glaux Library
#include	<Vector>

#include	"TexCache.h"
#include	"utils.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CTexCache::GetTexIdx(char *Name,int Flags)
{
int		ListSize=TexList.size();

		for (int Count=0;Count<ListSize;Count++)
		{
			if (strcmp(Name,TexList[Count].Name)==0 && TexList[Count].Flags==Flags)
			{
				return(Count);
			}
		}

	return(-1);
}

/*****************************************************************************/
int		CTexCache::ProcessTexture(char *TexName,char *Path,int Flags,sRGBData *RGBData)
{
int		ListSize=TexList.size();
int		Idx;
char	Name[_MAX_FNAME];

		_splitpath(TexName,0,0,Name,0);

// Check if Tex exists		
/*		for (int Count=0;Count<ListSize;Count++)
		{
			if (strcmp(Name,TexList[Count].Name)==0 && TexList[Count].Flags==Flags)
			{
				return(Count);
			}
		}
*/
		Idx=GetTexIdx(Name,Flags);
		if (Idx!=-1) return(Idx);

sTex		NewTex;
sRGBData	ThisRGB;

		strcpy(NewTex.Name,Name);
		strcpy(NewTex.Path,Path);
		NewTex.Flags=Flags;

		if (!RGBData)	// Need to load file
		{
			char	Filename[_MAX_PATH];

			sprintf(Filename,"%s%s.Bmp",Path,Name);
			TRACE1("Loading Texture %s\n",Filename);
			LoadBMP(Filename,ThisRGB);
			RGBData=&ThisRGB;
			LoadTex(NewTex,RGBData);
			FreeBMP(ThisRGB);
		}
		else
		{
			LoadTex(NewTex,RGBData);
		}

		TexList.push_back(NewTex);
				
		return(ListSize);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
void	CTexCache::LoadBMP(char *Filename,sRGBData &RGBData)
{
FILE			*File=NULL;
AUX_RGBImageRec *Aux;

		Aux=auxDIBImageLoad(Filename);
		RGBData.Width=Aux->sizeX;
		RGBData.Height=Aux->sizeY;
		RGBData.RGB=Aux->data;
		free(Aux);	// Safe to free aux now, contents copied (I HATE AUX)
}

/**************************************************************************************/
void	CTexCache::FreeBMP(sRGBData &RGBData)
{
		if (RGBData.RGB)
		{
			free((unsigned char*)RGBData.RGB);
		}
}

/**************************************************************************************/
void	CTexCache::LoadTex(sTex &ThisTex,sRGBData *TexData)
{
std::vector<u8>	Buffer;
int		Width=TexData->Width;
int		Height=TexData->Height;
int		Size=Width*Height;
// create RGB & alpha texture
		Buffer.resize(Size*4);
u8		*RgbPtr=TexData->RGB;

		for (int i=0;i<Size;i++)
		{
			u8	R=*RgbPtr++;
			u8	G=*RgbPtr++;
			u8	B=*RgbPtr++;
			u8	A=255;
			if ((R==BlankRGB.rgbRed && G==BlankRGB.rgbGreen && B==BlankRGB.rgbBlue))	// Create alpha for transparent pixels (flagged with PINK!!)
			{
				A=0;
			}

			Buffer[(i*4)+0]=R;
			Buffer[(i*4)+1]=G;
			Buffer[(i*4)+2]=B;
			Buffer[(i*4)+3]=A;
		}
		

		ThisTex.Width=Width;
		ThisTex.Height=Height;
		
		glGenTextures(1, &ThisTex.TexID);

		glBindTexture(GL_TEXTURE_2D, ThisTex.TexID);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &Buffer[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/

void	CTexCache::Purge()
{
int	ListSize=TexList.size();

	TRACE1("Purging %i textures\n",ListSize);

	for (int i=0; i<ListSize; i++)
	{
		glDeleteTextures(1,&TexList[i].TexID);
	}

	TexList.clear();
}