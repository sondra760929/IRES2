// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#pragma warning(  disable : 4244 )        // Issue warning 4244
#include "Standard_ShortReal.hxx"
#pragma warning(  default : 4244 )        // Issue warning 4244

///=> for OCC
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <Adaptor3d_Curve.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <TCollection_AsciiString.hxx>
#include <TopExp_Explorer.hxx>

#include <queue>
using namespace std;

#pragma comment(lib, "winmm")
#include <mmsystem.h>
#include <afxdisp.h>
#include <afxwin.h>

enum UNIT_MODE
{
	UNIT_MM = 0,
	UNIT_M,
	UNIT_INCH
};

#include "I-RES2View.h"
#include <afxcontrolbars.h>
#include <afxwin.h>

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

#define XHTMLXML
#define XHTMLHTML

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

extern CString m_strAppPath;
extern CString m_strEnvPath;
extern CString m_strProjectPath;
extern bool m_bUseOutline;
extern bool m_bUseShadow;
extern bool m_bUseVBO;
extern COLORREF m_colorUp;
extern COLORREF m_colorDown;
extern float m_fFaceDeflection;
extern float m_fEdgeDeflection;
extern vector< CString > m_aAnalysisInput;
extern vector< CString > m_aAnalysisOutput;
extern vector< CString > m_aAnalysisCommand;
extern vector< CString > m_aAnalysisPGM;
extern void SetButtons(CMFCButton* btn, UINT img, CString tooltip);
extern void char_to_utf8(char* strMultibyte, char* out);
extern void lpctstr_to_utf8(LPCTSTR in, char* out);
extern wstring utf_to_unicode(string in);
extern string utf_to_multibyte(string in);
extern CIRES2View* m_pCurrentView;
extern float UNIT_TO_M;
extern float M_TO_UNIT;
extern UNIT_MODE m_iCurrentUnitMode;
extern void CopyFiles(CString from, CString to, bool is_delete = false);
extern void DeleteTempFiles(CString from);
extern bool parse_double(std::string in, double& res);
extern HKEY main_key;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


