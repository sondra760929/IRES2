// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����




#pragma warning(  disable : 4244 )        // Issue warning 4244
#include "Standard_ShortReal.hxx"
#pragma warning(  default : 4244 )        // Issue warning 4244

///=> for OCC
#include <Standard_TypeDef.hxx>
#include <Standard.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
//#include <Graphic3d_WNTGraphicDevice.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <WNT_Window.hxx>
#include <Standard_ErrorHandler.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <Quantity_HArray1OfColor.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <STEPControl_Writer.hxx>
#include <gp_Trsf.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <ProjLib.hxx>
#include <ElSLib.hxx>
#include <gp_Pln.hxx>
#include <V3d_Plane.hxx>
#include <gp_Trsf.hxx>
#include <UnitsAPI.hxx>
#include <BRepBuilderAPI_NurbsConvert.hxx>
#include <Aspect_PolygonOffsetMode.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>
#include <AIS_Line.hxx>
#include <Geom_Line.hxx>
#include <Prs3d_Drawer.hxx>
#include <TopoDS.hxx>
#include <Prs3d_IsoAspect.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <BRep_Tool.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>
#include <TDF_Label.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <GeomPlate_BuildPlateSurface.hxx>
#include <Handle_BRepAdaptor_HCurve.hxx>
#include <BRepAdaptor_HCurve.hxx>
#include <BRepFill_CurveConstraint.hxx>
#include <Handle_BRepFill_CurveConstraint.hxx>
#include <GeomPlate_MakeApprox.hxx>
#include <GeomPlate_Surface.hxx>

#pragma comment(lib, "TKBRep.lib")
#pragma comment(lib, "TKBO.lib")
#pragma comment(lib, "FWOSPlugin.lib")
#pragma comment(lib, "PTKernel.lib")
#pragma comment(lib, "TKBool.lib")
#pragma comment(lib, "TKCAF.lib")
#pragma comment(lib, "TKCDF.lib")
#pragma comment(lib, "TKernel.lib")
#pragma comment(lib, "TKFeat.lib")
#pragma comment(lib, "TKFillet.lib")
#pragma comment(lib, "TKG2d.lib")
#pragma comment(lib, "TKG3d.lib")
#pragma comment(lib, "TKGeomAlgo.lib")
#pragma comment(lib, "TKGeomBase.lib")
#pragma comment(lib, "TKHLR.lib")
#pragma comment(lib, "TKMath.lib")
#pragma comment(lib, "TKOffset.lib")
#pragma comment(lib, "TKPCAF.lib")
#pragma comment(lib, "TKPrim.lib")
#pragma comment(lib, "TKPShape.lib")
#pragma comment(lib, "TKService.lib")
#pragma comment(lib, "TKTopAlgo.lib")
//#pragma comment(lib, "TKV2d.lib")
#pragma comment(lib, "TKV3d.lib")
//#pragma comment(lib, "TKVrml.lib")
#pragma comment(lib, "TKStl.lib")
#pragma comment(lib, "TKShHealing.lib")
#pragma comment(lib, "TKStep.lib")
#pragma comment(lib, "TKStepBase.lib")
#pragma comment(lib, "TKStepAttr.lib")
#pragma comment(lib, "TKXDESTEP.lib")

#pragma comment(lib, "TKIGES.lib")
#pragma comment(lib, "TKXDEIGES.lib")

#pragma comment(lib, "TKXSBase.lib")
#pragma comment(lib, "TKShapeSchema.lib")
#pragma comment(lib, "TKMesh.lib")

#pragma comment(lib, "TKLCAF.lib")
#pragma comment(lib, "TKXCAF.lib")

#pragma comment(lib, "TKOpenGl.lib")
//lua
//#pragma comment(lib, "lua5.1.lib")
//#pragma comment(lib, "lua51.lib")

#include <queue>
using namespace std;

#pragma comment(lib, "winmm")
#include <mmsystem.h>

#define WM_RETURN_FROM_UI WM_USER+10
#define WM_OK_FROM_UI WM_USER+11
#define WM_CANCEL_FROM_UI WM_USER+12
#define WM_GRD_LCLICK_MSG WM_USER + 13
#define WM_GRD_DCLICK_MSG WM_USER + 14
#define WM_GRD_MOUSEMOVE_MSG WM_USER + 15
#define WM_GRD_THLCLICK_MSG WM_USER + 16
#define WM_GRD_CELL_NOTIFY WM_USER + 17
#define WM_GRD_DELETE_KEY WM_USER + 18
#define WM_GRD_EDITE_VERIFY WM_USER + 19
#define WM_GRD_EDITE_FINISH WM_USER + 20
//#define OCC_VIEW

#ifdef _DEBUG
#pragma comment(lib, "OpenThreadsd.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgTextd.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "osgManipulatord.lib")
#pragma comment(lib, "osgFxd.lib")
#pragma comment(lib, "osgShadowd.lib")
#pragma comment(lib, "osgWidgetd.lib")
#else
#pragma comment(lib, "OpenThreads.lib")
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgUtil.lib")
#pragma comment(lib, "osgGA.lib")
#pragma comment(lib, "osgViewer.lib")
#pragma comment(lib, "osgText.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "osgManipulator.lib")
#pragma comment(lib, "osgFx.lib")
#pragma comment(lib, "osgShadow.lib")
#pragma comment(lib, "osgWidget.lib")
#endif

//#pragma comment(lib, "bass.lib")
//#pragma comment(lib, "libiconv.lib")

//#pragma comment(lib, "ode_single.lib")

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

//extern int m_iFileVersion;
//extern const int m_iCurrentFileVersion;
//extern double time_cell;
//extern double time_device;
//extern double time_part;
//extern double time_path;
//extern double time_tag;
//extern double time_geometry;
//extern CString m_strCurrentPath;
//extern CString m_strAppPath;
extern bool m_bUseOutline;
extern bool m_bUseShadow;
//extern float tag_font_size;
//extern int m_iJogPaneID;
extern bool m_bUseVBO;
extern COLORREF m_colorUp;
extern COLORREF m_colorDown;
extern float m_fFaceDeflection;
extern float m_fEdgeDeflection;
//
//extern FILE* m_fileLog;
//extern bool m_bShowFKin;
//extern bool m_bUseTestFunc;
//extern bool m_bUseBasePlane;
//extern float m_fLMotionSpeed;
//extern CString m_strDefaultCell;
//
//extern float m_fSTEP_Precision;
//extern bool m_bSTEP_WireGeometry;
//extern bool m_bSTEP_FaceEdge;
//
//extern CString m_strOptionsFile;
//extern int status_step;
//
//extern bool m_bLoadGeometryTree;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


