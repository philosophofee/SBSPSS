// MapEditView.cpp : implementation of the CMapEditView class
//

#include	"stdafx.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"
#include	"MapEdit.h"

#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditView

IMPLEMENT_DYNCREATE(CMapEditView, CGLEnabledView)

BEGIN_MESSAGE_MAP(CMapEditView, CGLEnabledView)
	//{{AFX_MSG_MAP(CMapEditView)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_LAYERBAR_NEW, OnLayerbarNew)
	ON_BN_CLICKED(IDC_LAYERBAR_DELETE, OnLayerbarDelete)
	ON_BN_CLICKED(IDC_LAYERBAR_UP, OnLayerbarUp)
	ON_BN_CLICKED(IDC_LAYERBAR_DOWN, OnLayerbarDown)
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditView construction/destruction

CMapEditView::CMapEditView()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
			LayerBar=Frm->GetLayerBar();
	
}

CMapEditView::~CMapEditView()
{
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::VideoMode(ColorsNumber & c, ZAccuracy & z, BOOL & dbuf)
{
	c=THOUSANDS;	// ask for 65355 colors...
	z=NORMAL;		// ...16 bit Z-buffer...
	dbuf=TRUE;		// ...double-buffering
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnCreateGL()
{
		glEnable(GL_TEXTURE_2D);       // Enable Texture Mapping
		glShadeModel(GL_SMOOTH);       // Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
		glClearDepth(1.0f);         // Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);       // Enables Depth Testing
		glDepthFunc(GL_LEQUAL);        // The Type Of Depth Testing To Do
		glEnable(GL_LIGHT0);        // Quick And Dirty Lighting (Assumes Light0 Is SetUp)
		glEnable(GL_LIGHTING);        // Enable Lighting
		glEnable(GL_COLOR_MATERIAL);      // Enable Material Coloring
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

		Core.Init(this);

}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnDrawGL()
{
		Core.Render();
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditView diagnostics

#ifdef _DEBUG
void CMapEditView::AssertValid() const
{
	CGLEnabledView::AssertValid();
}

void CMapEditView::Dump(CDumpContext& dc) const
{
	CGLEnabledView::Dump(dc);
}

CMapEditDoc* CMapEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditDoc)));
	return (CMapEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditView message handlers

void CMapEditView::OnSetFocus(CWnd* pOldWnd) 
{
CMapEditDoc	*CurDoc=GetDocument();
	CGLEnabledView::OnSetFocus(pOldWnd);
	theApp.SetCurrent(CurDoc);
	UpdateAll();
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditView::UpdateAll() 
{
	UpdateLayerBar();

}
/*********************************************************************************/
/*** Layer Commands **************************************************************/
/*********************************************************************************/
void CMapEditView::OnLayerbarNew() 
{
	Core.LayerAdd();
	UpdateLayerBar();
}

/*********************************************************************************/
void CMapEditView::OnLayerbarDelete() 
{
int	Sel=GetLayerCurSel();

		if (Sel==-1) return;
		Core.LayerDelete(Sel);
		UpdateLayerBar();
}

/*********************************************************************************/
void CMapEditView::OnLayerbarUp() 
{
int	Sel=GetLayerCurSel();

	if (Sel==-1) return;
	if (Sel>0)
	{
		Core.LayerMoveUp(Sel);
		UpdateLayerBar();
	}

}

/*********************************************************************************/
void CMapEditView::OnLayerbarDown() 
{
int	Sel=GetLayerCurSel();
	if (Sel==-1) return;
	if (Sel<GetLayerCount()-1)
	{
		Core.LayerMoveDown(GetLayerCurSel());
		UpdateLayerBar();
	}
}

/*********************************************************************************/
void CMapEditView::UpdateLayerBar()
{
int			LayerCount=Core.LayerGetCount();
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);
int			CurSel=Dlg->GetCurSel();

		Dlg->ResetContent();

		for (int i=0;i<LayerCount;i++)
		{
			CLayer	&ThisLayer=Core.LayerGet(i);
			Dlg->AddString(ThisLayer.GetName());
		}
		Dlg->SetCurSel(CurSel);

}

/*********************************************************************************/
int	CMapEditView::GetLayerCurSel()
{
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);
		return(Dlg->GetCurSel());

}

/*********************************************************************************/
int	CMapEditView::GetLayerCount()
{
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);
	return(Dlg->GetCount());

}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditView::OnLButtonDown(UINT nFlags, CPoint point)				{Core.LButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnLButtonUp(UINT nFlags, CPoint point)				{Core.LButtonControl(nFlags,point,FALSE);}
void CMapEditView::OnMButtonDown(UINT nFlags, CPoint point)				{Core.MButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnMButtonUp(UINT nFlags, CPoint point)				{Core.MButtonControl(nFlags,point,FALSE);}
BOOL CMapEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)	{Core.MouseWheel(nFlags,zDelta,pt) ;return(0);}
void CMapEditView::OnRButtonDown(UINT nFlags, CPoint point)				{Core.RButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnRButtonUp(UINT nFlags, CPoint point)				{Core.RButtonControl(nFlags,point,FALSE);}
void CMapEditView::OnMouseMove(UINT nFlags, CPoint point)				{Core.MouseMove(nFlags, point,GetCapture()==this);}


void CMapEditView::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
TRACE0("!!!!");	
	CGLEnabledView::OnCaptureChanged(pWnd);
}
