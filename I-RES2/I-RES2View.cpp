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

// I-RES2View.cpp : CIRES2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "I-RES2.h"
#endif

#include "I-RES2Doc.h"
#include "I-RES2View.h"
#include "MainFrm.h"
#include "IGESControl_Reader.hxx"
#include <BRepMesh_IncrementalMesh.hxx>
#include <GeomLib.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Connect.hxx>
#include <TShort_Array1OfShortReal.hxx>
#include <Poly_PolygonOnTriangulation.hxx>
#include <GCPnts_UniformDeflection.hxx>
#include <osg/ComputeBoundsVisitor>
#include <osgUtil/PlaneIntersector>
#include <osg/Point>
#include <IntAna2d_AnaIntersection.hxx>
#include <gp_Lin2d.hxx>
#include "OptImportExportBase.h"
#include <osg/BlendFunc>
#include "CDlgSelectSections.h"
#include "DlgDefineInp.h"
#include "zip.h"
#include "FrameContainer.h"
#include "PosterPrinter.h"
#include "DlgCalc.h"
#include <BRep_Tool.hxx>
#include <osgUtil/Tessellator>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIRES2View

CString GetDocumentFolder()
{
	LPWSTR path;
	CString document_folder;
	SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
	document_folder = path;
	CoTaskMemFree(path);
	return document_folder;
}

CString GetNewProjectFolder()
{
	CString document_folder = GetDocumentFolder();

	CString new_proj_folder;
	CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
	CString strDate; // 반환되는 날짜와 시간을 저장할 CString 변수 선언
	strDate.Format("%04d%02d%02d%02d%02d%02d", cTime.GetYear(), // 현재 년도 반환
		cTime.GetMonth(), // 현재 월 반환
		cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond()); // 현재 일 반환
	new_proj_folder = document_folder + "\\" + strDate;
	return new_proj_folder;
}


BOOL DeleteDirectoryFile(LPCTSTR RootDir)
{
	if (RootDir == NULL)
	{
		return FALSE;
	}

	BOOL bRval = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");


	// 해당 디렉토리의 모든 파일을 검사한다.
	szRoot.Format(_T("%s\\*.*"), RootDir);

	CFileFind find;

	bRval = find.FindFile(szRoot);

	if (bRval == FALSE)
	{
		return bRval;
	}

	while (bRval)
	{
		bRval = find.FindNextFile();

		// . or .. 인 경우 무시 한다. 
		if (find.IsDots() == FALSE)
		{
			// Directory 일 경우 재귀호출 한다.
			if (find.IsDirectory())
			{
				DeleteDirectoryFile(find.GetFilePath());
			}
			// file일 경우 삭제
			else
			{
				bRval = DeleteFile(find.GetFilePath());
			}
		}
	}
	find.Close();
	bRval = RemoveDirectory(RootDir);
	return bRval;
}

IMPLEMENT_DYNCREATE(CIRES2View, CView)

BEGIN_MESSAGE_MAP(CIRES2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIRES2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_IMPORT_HULL, &CIRES2View::OnButtonImportHull)
	ON_COMMAND(ID_BUTTON_OPEN, &CIRES2View::OnButtonOpen)
	ON_COMMAND(ID_BUTTON_SAVE, &CIRES2View::OnButtonSave)
	ON_COMMAND(ID_BUTTON_SAVE_IMAGE, &CIRES2View::OnButtonSaveImage)
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_BUTTON_DEFINE_SECTIONS, &CIRES2View::OnButtonDefineSections)
	ON_COMMAND(ID_CHECK_BOW_BREAKING, &CIRES2View::OnCheckBowBreaking)
	ON_UPDATE_COMMAND_UI(ID_CHECK_BOW_BREAKING, &CIRES2View::OnUpdateCheckBowBreaking)
	ON_COMMAND(ID_CHECK_STERN, &CIRES2View::OnCheckStern)
	ON_UPDATE_COMMAND_UI(ID_CHECK_STERN, &CIRES2View::OnUpdateCheckStern)
	ON_COMMAND(ID_EDIT_START, &CIRES2View::OnEditStart)
	ON_COMMAND(ID_EDIT_END, &CIRES2View::OnEditEnd)
	ON_COMMAND(ID_EDIT_SPACE, &CIRES2View::OnEditSpace)
	ON_COMMAND(ID_EDIT_POINT_GAP, &CIRES2View::OnEditPointGap)
	ON_COMMAND(ID_BUTTON_CALCULATE_SECTION_POINTS, &CIRES2View::OnButtonCalculateSectionPoints)
	//ON_COMMAND(ID_BUTTON_SHOW_HIDE_SECTIONS, &CIRES2View::OnButtonShowHideSections)
	ON_COMMAND(ID_BUTTON_SHOW_HIDE_SECTION_CUT, &CIRES2View::OnButtonShowHideSectionCut)
	//ON_COMMAND(ID_BUTTON_HIDE_SHOW_WATERLINES, &CIRES2View::OnButtonHideShowWaterlines)
	ON_COMMAND(ID_BUTTON_SHOW_HIDE_WATERLINE_CUT, &CIRES2View::OnButtonShowHideWaterlineCut)
	//ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_HIDE_SECTIONS, &CIRES2View::OnUpdateButtonShowHideSections)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_HIDE_SECTION_CUT, &CIRES2View::OnUpdateButtonShowHideSectionCut)
	//ON_UPDATE_COMMAND_UI(ID_BUTTON_HIDE_SHOW_WATERLINES, &CIRES2View::OnUpdateButtonHideShowWaterlines)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_HIDE_WATERLINE_CUT, &CIRES2View::OnUpdateButtonShowHideWaterlineCut)
	ON_COMMAND(ID_BUTTON_SAVE_HULL, &CIRES2View::OnButtonSaveHull)
	ON_COMMAND(ID_BUTTON_ANALYSIS, &CIRES2View::OnButtonAnalysis)
	ON_COMMAND(ID_BUTTON_SELECT_SECTION, &CIRES2View::OnButtonSelectSection)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_HULL_POINT_TO_POINT, &CIRES2View::OnButtonHullPointToPoint)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_Nurbs_Conversion, &CIRES2View::OnButtonNurbsConversion)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_Nurbs_Conversion, &CIRES2View::OnUpdateButtonNurbsConversion)
	ON_COMMAND(ID_BUTTON_VIEW_FRONT, &CIRES2View::OnButtonfront)
	ON_COMMAND(ID_BUTTON_VIEW_TOP, &CIRES2View::OnButtontop)
	ON_COMMAND(ID_BUTTON_VIEW_LEFT, &CIRES2View::OnButtonleft)
	ON_COMMAND(ID_BUTTON_VIEW_REAR, &CIRES2View::OnButtonback)
	ON_COMMAND(ID_BUTTON_VIEW_RIGHT, &CIRES2View::OnButtonright)
	ON_COMMAND(ID_BUTTON_VIEW_BOTTOM, &CIRES2View::OnButtonbottom)
	ON_COMMAND(ID_BUTTON_VIEW_ISO, &CIRES2View::OnButtonaxo)
	ON_COMMAND(ID_BUTTON_VIEW_PERSPECTIVE, &CIRES2View::OnButtonViewPerspective)
	ON_COMMAND(ID_BUTTON_VIEW_ORTHO, &CIRES2View::OnButtonViewOrtho)
	//ON_COMMAND(ID_BUTTON_VIEW_FRONT, &CIRES2View::OnButtonreset)
	//ON_COMMAND(ID_BUTTON_VIEW_FRONT, &CIRES2View::OnButtonzoomwin)
	ON_COMMAND(ID_BUTTON_VIEW_ALL, &CIRES2View::OnButtonzoomall)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CHECK_DISTANCE_FOR_AXIS, &CIRES2View::OnCheckDistanceForAxis)
	ON_UPDATE_COMMAND_UI(ID_CHECK_DISTANCE_FOR_AXIS, &CIRES2View::OnUpdateCheckDistanceForAxis)
	ON_COMMAND(ID_CHECK_DISTANCE, &CIRES2View::OnCheckDistance)
	ON_UPDATE_COMMAND_UI(ID_CHECK_DISTANCE, &CIRES2View::OnUpdateCheckDistance)
	ON_COMMAND(ID_CHECK_NUMBER, &CIRES2View::OnCheckNumber)
	ON_UPDATE_COMMAND_UI(ID_CHECK_NUMBER, &CIRES2View::OnUpdateCheckNumber)
	ON_COMMAND(ID_EDIT_DISTANCE, &CIRES2View::OnEditDistance)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DISTANCE, &CIRES2View::OnUpdateEditDistance)
	ON_COMMAND(ID_EDIT_NUMBER, &CIRES2View::OnEditNumber)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NUMBER, &CIRES2View::OnUpdateEditNumber)
	ON_COMMAND(ID_EDIT_START_WATERLINE, &CIRES2View::OnEditStartWaterline)
	ON_COMMAND(ID_EDIT_END_WATERLINE, &CIRES2View::OnEditEndWaterline)
	ON_COMMAND(ID_CHECK_DISTANCE_WATERLINE, &CIRES2View::OnCheckDistanceWaterline)
	ON_UPDATE_COMMAND_UI(ID_CHECK_DISTANCE_WATERLINE, &CIRES2View::OnUpdateCheckDistanceWaterline)
	ON_COMMAND(ID_CHECK_NUMBER_WATERLINE, &CIRES2View::OnCheckNumberWaterline)
	ON_UPDATE_COMMAND_UI(ID_CHECK_NUMBER_WATERLINE, &CIRES2View::OnUpdateCheckNumberWaterline)
	ON_COMMAND(ID_EDIT_DISTANCE_WATERLINE, &CIRES2View::OnEditDistanceWaterline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DISTANCE_WATERLINE, &CIRES2View::OnUpdateEditDistanceWaterline)
	ON_COMMAND(ID_EDIT_NUMBER_WATERLINE, &CIRES2View::OnEditNumberWaterline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NUMBER_WATERLINE, &CIRES2View::OnUpdateEditNumberWaterline)
	ON_COMMAND(ID_BUTTON_DEFINE, &CIRES2View::OnButtonDefine)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

int N_FRAME;
int NS_S;
float S_N[5000] = { 0 };
int N_BETA[5000] = { 0 };
float Y_VAL_ST[5000][5000] = { 0 };
float Z_VAL_ST[5000][5000] = { 0 };
float BETA_ST[5000][5000] = { 0 };
float Sin_Beta[5000][5000] = { 0 };
float SLOPE_Y[5000][5000] = { 0 };
float SLOPE_Z[5000][5000] = { 0 };
float SLOPE[5000][5000] = { 0 };
float SLOPE_BETA_DEG[5000][5000] = { 0 };
float R_SP[5000][5000] = { 0 };
float Z_BUOY[5000][5000] = { 0 };
float Y_BUOY[5000][5000] = { 0 };
float DIST_ICE[5000][5000] = { 0 };
float R_SF[5000][5000] = { 0 };
float R_total[5000][5000] = { 0 };

int N_BUOY[5000] = { 0 };
float Staion_length_Buoy[5000] = { 0 };
float GIRTH_LENGTH[5000] = { 0 };
float N_END_GIRTH[5000] = { 0 };
float R_BO[5000] = { 0 };

float __X_COOR[5000] = { 0 };
float Y[5000] = { 0 };
float Z_COOR[5000] = { 0 };
float X_NORM[5000] = { 0 };
float Y_NORM[5000] = { 0 };
float Z_NORM[5000] = { 0 };
float __ALPHA[5000] = { 0 };
float BETA[5000] = { 0 };
float GAMMA[5000] = { 0 };
float max_y;
float PI3;
int MBR = 1;
int MCL = 1;
int MBU = 1;
float Sum_X = 0;
float Sum_Y = 0;
float Sum_Z = 0;
int NSIGMA = 0;
int NH = 0;
int NV = 0;
float RHO0 = 0;

int HULL_TYPE;
float FG;
float SIGMAP, SIGMAK, SSIGMA;
float HH, HK, SH;
float VS, VE, VI;
float GG, XK1H1, XK1H2, XK2H1, XK2H2, XK3H1, XK3H2, RHO, RHOL, E_young;
//vector< float > VSP;
//vector< float > VELOCI;
//vector< float > FROUD;
//vector< float > SIGMA;
//vector< float > THCK;
//vector< vector< float > > R_BR;
//vector< vector< float > > R_CL;
//vector< vector< float > > R_CLEAR;
//vector< vector< float > > R_BREAK;
//vector< float > R_BOUYA;
//vector< vector< float > > DEPTH_BUOY;
//vector< float > YH;
float VSP[5000] = { 0 };
float VELOCI[5000] = { 0 };
float FROUD[5000] = { 0 };
float SIGMA[5000] = { 0 };
float THCK[5000] = { 0 };
float R_BR[5000][5000] = { 0 };
float R_CL[5000][5000] = { 0 };
float R_CLEAR[5000][5000] = { 0 };
float R_BREAK[5000][5000] = { 0 };
float R_BOUYA[5000] = { 0 };
float DEPTH_BUOY[5000][5000] = { 0 };
float YH[5000] = { 0 };
FILE* fp_4 = NULL;
FILE* fp_5 = NULL;
FILE* fp_6 = NULL;
FILE* fp_7 = NULL;
FILE* fp_8 = NULL;
FILE* fp_9 = NULL;
FILE* fp_10 = NULL;
FILE* fp_15 = NULL;
// CIRES2View 생성/소멸
vector< float > ICE_S;
vector< float > ICE_T;
vector< float > A_SPEED[3];

class Node {
public:
	int row, col;
	double g, h, f;
	double speed;
	double time;

public:
	Node(int _x, int _y, double _speed)
	{
		this->row = _x;
		this->col = _y;
		this->speed = _speed;
		time = 0;
		g = 0;
		h = 0;
		f = 0;
	}

	void changeValue(int _x, int _y, double _speed)
	{
		this->row = _x;
		this->col = _y;
		this->speed = _speed;
	}

	void changeScore(double _g, double _h, double _f)
	{
		g = _g;
		h = _h;
		f = _f;
	}

	void getRow()
	{
		cout << this->row;
	}
	void getCol()
	{
		cout << this->col;
	}
	void getSpeed()
	{
		cout << this->speed;
	}
};

class NodePath {
public:
	int row, col, dicision;

public:
	NodePath(int _r, int _c, double _d)
	{
		this->row = _r;
		this->col = _c;
		this->dicision = _d;
	}
};


CIRES2View::CIRES2View()
	: mOSG(0L)
	, m_bBowBreaking(true)
	//, m_bShowSection(true)
	, m_bShowSectionData(true)
	//, m_bShowWaterline(true)
	, m_bShowWaterlineData(true)
	, m_bInitialize(false)
	, m_bUseDistanceForAxis(false)
	, m_bUseDistanceForAxisWaterline(false)
	, m_fWaterlinePointGap(0.5f)
	, m_pMainFrame(0)
	, m_bViewEdge(false)
	, m_fCrossSectionOffset(5.0f)
	, m_fCrossSectionPointGap(0.5f)
	, m_bConditionConstant(false)
	, m_isCreateFolder(false)
	, m_pTranslationDlg(0)
	, m_iSelectionMode(SELECTION_NONE)
	, m_bDoubleCalc(false)
	, m_fInitSpeed(0)
	, m_fMaxSpeed(0)
	, m_fIncreSpeed(0)
{
	//m_iHULLPos[0] = 0;
	//m_iHULLPos[1] = 0;
	//m_iHULLPos[2] = 0;
	//m_iHULLRot[0] = 0;
	//m_iHULLRot[1] = 0;
	//m_iHULLRot[2] = 0;
	//m_iSectionRot[0] = 0;
	//m_iSectionRot[1] = 0;
	//m_iSectionRot[2] = 0;
	//m_iWaterLinePos[0] = 0;
	//m_iWaterLinePos[1] = 0;
	//m_iWaterLinePos[2] = 0;
	//m_iWaterLineRot[0] = 0;
	//m_iWaterLineRot[1] = 0;
	//m_iWaterLineRot[2] = 0;
	m_pCurrentView = this;
	m_bRunForInterface = false;
}

CIRES2View::~CIRES2View()
{
	if (m_isCreateFolder)
	{
		if(m_strProjectPath != m_strAppPath)
			DeleteDirectoryFile(m_strProjectPath);
	}
}

BOOL CIRES2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CIRES2View 그리기

void CIRES2View::OnDraw(CDC* /*pDC*/)
{
	CIRES2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CIRES2View 인쇄


void CIRES2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIRES2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CIRES2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CIRES2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CIRES2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
}

void CIRES2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIRES2View 진단

#ifdef _DEBUG
void CIRES2View::AssertValid() const
{
	CView::AssertValid();
}

void CIRES2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIRES2Doc* CIRES2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIRES2Doc)));
	return (CIRES2Doc*)m_pDocument;
}
#endif //_DEBUG


// CIRES2View 메시지 처리기


int CIRES2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	mOSG = new cOSG2(this->m_hWnd);
	mOSG->InitOSG();
	mOSG->SetView(this);

	hints = new osg::TessellationHints;
	hints->setDetailRatio(0.5f);

	mThreadHandle = new CRenderingThread(mOSG);
	mThreadHandle->start();

	osgAxis = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgAxis);

	osgSelectPoint = new osg::MatrixTransform();
	//mOSG->mRoot->addChild(osgSelectPoint);
	osg::ref_ptr<osg::Geode> base_point = new osg::Geode;
	osg::ref_ptr<osg::Geometry> base_point_geo = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
	v_array->push_back(osg::Vec3(0, 0, 0));
	base_point_geo->setVertexArray(v_array);
	osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
	cross_color->push_back(osg::Vec4(1, 0, 0, 1));
	base_point_geo->setColorArray(cross_color.get());
	base_point_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
	osg::DrawArrays* drawArrayPoints = new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, v_array->size()); // Is your line strip 
	base_point_geo->addPrimitiveSet(drawArrayPoints);
	osg::Point* point = new osg::Point;
	point->setSize(5);
	base_point_geo->getOrCreateStateSet()->setAttribute(point);
	base_point_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	base_point->addDrawable(base_point_geo);
	osgSelectPoint->addChild(base_point);

	osgHull_Center = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgHull_Center);


	osg::Geode* geode = new osg::Geode;
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//osg::MatrixTransform* frame_tr = new osg::MatrixTransform();
	//FrameContainer* frameContainer = new FrameContainer;
	//frameContainer->setFrameSize(40.0f);
	//frameContainer->setFrame(frame_tr);
	//frame_tr->addChild(geode);
	//osgHull_Center->addChild(frameContainer);
	osgHull_Center->addChild(geode);

	double pyramidBaseZ = 0.1f;
	double outerBaseRadius = 0.05f;
	osg::Vec4 color(1, 1, 0, 1);
	//	x axis
	osg::Cone* cone = new osg::Cone(osg::Vec3(1.0f, 0.0f, 0.0f), outerBaseRadius, pyramidBaseZ);
	osg::Quat rotation;
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(1, 0, 0));
	cone->setRotation(rotation);
	osg::ShapeDrawable* shape = new osg::ShapeDrawable(cone);
	shape->setColor(color);
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createXAxis(1.0f, pyramidBaseZ, outerBaseRadius, color));

	//	 y axis
	cone = new osg::Cone(osg::Vec3(0.0f, 1.0f, 0.0f), outerBaseRadius, pyramidBaseZ);
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0, 1, 0));
	cone->setRotation(rotation);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(color);
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createYAxis(1.0f, pyramidBaseZ, outerBaseRadius, color));

	//	 z axis
	cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 1.0f), outerBaseRadius, pyramidBaseZ);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(color);
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createZAxis(1.0f, pyramidBaseZ, outerBaseRadius, color));

	geode->addDrawable(mOSG->createAxisLabel("X", osg::Vec3(1.1f, 0, 0), color));
	geode->addDrawable(mOSG->createAxisLabel("Y", osg::Vec3(0, 1.1f, 0), color));
	geode->addDrawable(mOSG->createAxisLabel("Z", osg::Vec3(0, 0, 1.1f), color));


	osgHull = new osg::MatrixTransform();
	osgHull_Center->addChild(osgHull);


	m_goedeRectangle = new osg::Geode;
	m_ptSelectionRect = new osg::Vec3Array;
	osg::Vec4Array* color1 = new osg::Vec4Array;
	m_geometrySelectionRect = new osg::Geometry;
	m_geometrySelectionRect->setDataVariance(osg::Object::DYNAMIC);
	m_geometrySelectionRect->setUseDisplayList(false);
	m_geometrySelectionRect->setUseVertexBufferObjects(true);

	color1->push_back(osg::Vec4(0.0, 0.0, 0.0, 0.8f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	m_ptSelectionRect->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));

	m_geometrySelectionRect->setVertexArray(m_ptSelectionRect);
	m_geometrySelectionRect->setColorArray(color1);
	m_geometrySelectionRect->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_geometrySelectionRect->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, m_ptSelectionRect->size()));

	m_goedeRectangle->addDrawable(m_geometrySelectionRect);
	osg::StateSet* state1 = m_geometrySelectionRect->getOrCreateStateSet();
	state1->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::LineWidth* lineWidth1 = new osg::LineWidth(2);
	state1->setAttributeAndModes(lineWidth1, osg::StateAttribute::ON);

	m_cameraStatus = new osg::Camera;
	m_cameraStatus->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	m_cameraStatus->setProjectionMatrixAsOrtho2D(0, 1280, 0, 1024);
	m_cameraStatus->setViewMatrix(osg::Matrix::identity());
	m_cameraStatus->setClearMask(GL_DEPTH_BUFFER_BIT);
	m_cameraStatus->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	m_cameraStatus->addChild(m_goedeRectangle);

	//osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
	//osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode;
	//polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	//stateset->setAttributeAndModes(polymode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
	//osgHull->setStateSet(stateset);

	//osgWaterline = new osg::MatrixTransform();
	//mOSG->mRoot->addChild(osgWaterline);

	osgWaterlineSection = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgWaterlineSection);


	m_bInitialize = true;
	return 0;
}


void CIRES2View::OnDestroy()
{
	CView::OnDestroy();

	delete mThreadHandle;
	if (mOSG != 0) delete mOSG;

	m_DlgProgress->DestroyWindow();
	delete m_DlgProgress;
}


void CIRES2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	PI3 = acos(-1.0f);

	m_pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (m_pMainFrame)
	{
		//m_pMainFrame->m_wndDlgToolbar.m_MainToolbar.m_pView = this;
		//m_pMainFrame->m_wndDlgToolbar.m_SectionToolbar.m_pView = this;
		m_pMainFrame->m_wndClassView.m_pView = this;
		m_pMainFrame->m_wndClassView.m_MainToolbar.m_pView = this;
		m_pMainFrame->m_wndClassView.m_SectionToolbar.m_pView = this;

		m_pMainFrame->m_wndClassView1.m_pView = this;
		m_pMainFrame->m_wndClassView1.m_MainToolbar.m_pView = this;
		m_pMainFrame->m_wndClassView1.m_SectionToolbar.m_pView = this;

		m_pMainFrame->m_wndClassView2.m_pView = this;
		m_pMainFrame->m_wndClassView2.m_MainToolbar.m_pView = this;
		m_pMainFrame->m_wndClassView2.m_SectionToolbar.m_pView = this;

		m_pMainFrame->m_wndFileView.m_pView = this;
		m_pMainFrame->m_wndFileView.m_OutputToolbar.m_pView = this;
		//m_pEditStart = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START));
		//m_pEditEnd = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END));
		//m_pEditSpace = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE));
		////m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));
		//m_pEditPointsDistance = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE));
		//m_pEditPointsNumber = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER));

		//m_pEditStartWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START_WATERLINE));
		//m_pEditEndWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END_WATERLINE));
		//m_pEditSpaceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE_WATERLINE));
		//m_pEditPointsDistanceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE_WATERLINE));
		//m_pEditPointsNumberWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER_WATERLINE));

		//m_pHULLSpinXPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_X_POS));
		//m_pHULLSpinXPos->EnableSpinButtons(-100000000, 100000000);
		//m_pHULLSpinXPos->SetEditText("0");
		//m_iHULLPos[0] = 0;
		//m_pHULLSpinYPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Y_POS));
		//m_pHULLSpinYPos->EnableSpinButtons(-100000000, 100000000);
		//m_pHULLSpinYPos->SetEditText("0");
		//m_iHULLPos[1] = 0;
		//m_pHULLSpinZPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Z_POS));
		//m_pHULLSpinZPos->EnableSpinButtons(-100000000, 100000000);
		//m_pHULLSpinZPos->SetEditText("0");
		//m_iHULLPos[2] = 0;

		//m_pHULLSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_X_ANGLE));
		//m_pHULLSpinXRot->EnableSpinButtons(-360, 360);
		//m_pHULLSpinXRot->SetEditText("0");
		//m_iHULLRot[0] = 0;
		//m_pHULLSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Y_ANGLE));
		//m_pHULLSpinYRot->EnableSpinButtons(-360, 360);
		//m_pHULLSpinYRot->SetEditText("0");
		//m_iHULLRot[1] = 0;
		//m_pHULLSpinZRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Z_Angle));
		//m_pHULLSpinZRot->EnableSpinButtons(-360, 360);
		//m_pHULLSpinZRot->SetEditText("0");
		//m_iHULLRot[2] = 0;

		//m_pSectionSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_X));
		//m_pSectionSpinXRot->EnableSpinButtons(-360, 360);
		//m_pSectionSpinXRot->SetEditText("0");
		m_iSectionRot[0] = 0;
		//m_pSectionSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_Y));
		//m_pSectionSpinYRot->EnableSpinButtons(-360, 360);
		//m_pSectionSpinYRot->SetEditText("0");
		m_iSectionRot[1] = 0;
		//m_pSectionSpinZRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_Z));
		//m_pSectionSpinZRot->EnableSpinButtons(-360, 360);
		//m_pSectionSpinZRot->SetEditText("0");
		m_iSectionRot[2] = 0;

		//m_pWaterlineSpinZPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Z));
		//m_pWaterlineSpinZPos->EnableSpinButtons(-100000000, 100000000);
		//m_pWaterlineSpinZPos->SetEditText("0");
		//m_iWaterLinePos[2] = 0;

		//m_pWaterlineSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_X));
		//m_pWaterlineSpinXRot->EnableSpinButtons(-360, 360);
		//m_pWaterlineSpinXRot->SetEditText("0");
		//m_iWaterLineRot[0] = 0;
		//m_pWaterlineSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Y));
		//m_pWaterlineSpinYRot->EnableSpinButtons(-360, 360);
		//m_pWaterlineSpinYRot->SetEditText("0");
		//m_iWaterLineRot[1] = 0;
	}

	m_DlgProgress = new CDlgProgress();
	m_DlgProgress->Create(IDD_DIALOG_PROGRESS);


	m_pTranslationDlg = new CDlgTranslation();
	m_pTranslationDlg->Create(IDD_DIALOG_TRANSLATE);

	ResizeControl(0, 0);

	OnButtonzoomall();

	SetTimer(2, 1000, NULL);

	//ShowAllMap();
	//LoadMap();

	CString ires_path = AfxGetApp()->GetProfileString(_T("HMS"), _T("INIT_IRES"));
	OpenFromIRES(ires_path);
}

void CIRES2View::RunExecute(CString command_string)
{
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = NULL;
	si.hStdInput = NULL;
	si.hStdError = NULL;
	si.wShowWindow = SW_HIDE;       /* 눈에 보이지 않는 상태로 프로세스 시작 */

	DWORD ret = CreateProcess(NULL, command_string.GetBuffer(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (ret)
	{
		WaitForSingleObject(pi.hProcess, 0xffffffff);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}

void CIRES2View::OpenFromIRES(CString path_name)
{
	ClearProject(m_strProjectPath);

	CString document_folder = GetDocumentFolder();
	m_strProjectPath = GetNewProjectFolder();

	CString command_string;
	command_string = m_strAppPath + "\\unzip.exe " + path_name + " -d " + m_strProjectPath;

	RunExecute(command_string);

	CString temp_string;
	temp_string = m_strProjectPath + "\\hull.osg";
	if (PathFileExists(temp_string))
	{
		m_isCreateFolder = true;

		time(&start_time);
		time(&end_time);
		double diff_time1 = difftime(end_time, start_time);

		CT2CA pszConvertedAnsiString(temp_string);
		std::string strStd(pszConvertedAnsiString);
		osg::Node* geo_node = osgDB::readNodeFile(strStd);
		if (geo_node)
		{
			PreFrameUpdateData pf(osgHull, NULL);
			m_QRemoveChild.push(pf);

			PreFrameUpdateData pf1(osgHull, geo_node);
			m_QAddChild.push(pf1);

			ClearSections();
			m_pMainFrame->m_wndClassView.SetHulllStatus(true);
			m_pMainFrame->m_wndClassView.SetDraftStatus(true);
			m_pMainFrame->m_wndClassView.SetCrossStatus(true);
			m_pMainFrame->m_wndClassView.SetMaterialStatus(true);
			m_pMainFrame->m_wndClassView.SetConditionStatus(true);

			m_pMainFrame->m_wndClassView1.SetHulllStatus(true);
			m_pMainFrame->m_wndClassView1.SetDraftStatus(true);
			m_pMainFrame->m_wndClassView1.SetCrossStatus(true);
			m_pMainFrame->m_wndClassView1.SetMaterialStatus(true);
			//m_pMainFrame->m_wndClassView1.SetConditionStatus(true);

			m_pMainFrame->m_wndClassView2.SetHulllStatus(true);
			m_pMainFrame->m_wndClassView2.SetDraftStatus(true);
			m_pMainFrame->m_wndClassView2.SetCrossStatus(true);
			m_pMainFrame->m_wndClassView2.SetMaterialStatus(true);
			if (LoadEstimation(m_strProjectPath + "\\ICE_MAP.inp"))
			{
				if (LoadEstimation2(m_strProjectPath + "\\ICE_MAP_INPUT.inp"))
				{
					m_pMainFrame->m_wndClassView2.SetEstimationStatus(true);
				}
			}
			//m_pMainFrame->m_wndClassView2.SetConditionStatus(true);

			m_pMainFrame->m_wndFileView.Clear();
			m_pMainFrame->m_wndClassView.ClearJobList();
			m_pMainFrame->m_wndClassView1.ClearJobList();
			m_pMainFrame->m_wndClassView2.ClearJobList();

			//LoadDatumFile();

			temp_string = m_strProjectPath + "\\JOB";
			CFileFind file;
			BOOL b = file.FindFile(temp_string + _T("\\*.*"));		// 모든 확장자를 다 사용.
		//	CString strMusicFilter = ".MP3.OGG.WMA.WAV";			// 필터링 하고 싶으면 이렇게 쓰면 됨
			CString strFolderItem, strFileExt, strTempString;
			while (b)
			{
				b = file.FindNextFile();
				if (file.IsDirectory() && !file.IsDots())			// 디렉토리 발견시 
				{
					strFolderItem = file.GetFileTitle();
					m_pMainFrame->m_wndClassView.AddJobItem(strFolderItem);
					m_pMainFrame->m_wndFileView.AddItem(strFolderItem);
				}
				if (!file.IsDots())									// 파일 탐색 필터 정의에따라 해당 StringList에 추가
				{
					if (file.IsDirectory()) continue;				// 폴더만 남는 경우는 넣으면 안됨 
				}
			}
		}
	}
}
void CIRES2View::OnButtonOpen()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"I-RES2 Files (*.ires)|*.ires|All Files (*.*)|*.*||",
		NULL);

	if (dlg.DoModal() == IDOK)
	{
		OpenFromIRES(dlg.GetPathName());
	}
}

void CIRES2View::OnButtonSave()
{
	if (m_isCreateFolder)
	{
		CFileDialog dlg(FALSE,
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"I-RES2 Files (*.ires)|*.osgb|All Files (*.*)|*.*||",
			NULL);

		if (dlg.DoModal() == IDOK)
		{
			SetCurrentDirectory(m_strProjectPath);
			SaveDatumFile();
			CString file_ext = dlg.GetFileExt();
			file_ext.MakeLower();
			CString file_path = dlg.GetPathName();
			if (file_ext == "")
			{
				file_path += ".ires";
			}
			CString command_string;
			command_string = m_strAppPath + "\\zip.exe -r " + file_path + " *";
			STARTUPINFO si;
			SECURITY_ATTRIBUTES sa;
			PROCESS_INFORMATION pi;

			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;

			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
			si.hStdOutput = NULL;
			si.hStdInput = NULL;
			si.hStdError = NULL;
			si.wShowWindow = SW_HIDE;       /* 눈에 보이지 않는 상태로 프로세스 시작 */

			CreateProcess(NULL, command_string.GetBuffer(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
	else
	{
		AfxMessageBox("No files for saving");
	}
}

void CIRES2View::OnButtonSaveImage()
{
	osg::Vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f);
	osg::Camera::RenderTargetImplementation renderImplementation = osg::Camera::FRAME_BUFFER_OBJECT;

	CFileDialog pDlg(FALSE, "bmp", "post.bmp");
	if (pDlg.DoModal() == IDOK)
	{
		CRect rect;
		GetClientRect(&rect);
		int tileWidth = rect.Width(), tileHeight = rect.Height();
		int posterWidth = tileWidth * 2, posterHeight = tileHeight * 2;

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setClearColor(bgColor);
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setRenderOrder(osg::Camera::PRE_RENDER);
		camera->setRenderTargetImplementation(renderImplementation);
		camera->setViewport(0, 0, tileWidth, tileHeight);


		camera->addChild(mOSG->mRoot);

		osg::ref_ptr<PosterPrinter> printer = new PosterPrinter;
		printer->setTileSize(tileWidth, tileHeight);
		printer->setPosterSize(posterWidth, posterHeight);
		printer->setCamera(camera.get());

		osg::ref_ptr<osg::Image> posterImage = 0;

		posterImage = new osg::Image;
		posterImage->allocateImage(posterWidth, posterHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE);
		printer->setFinalPoster(posterImage.get());
		printer->setOutputPosterName(string(pDlg.GetPathName()));

		mOSG->MainScene->addChild(camera.get());

		printer->init(mOSG->getViewer()->getCamera());

		mOSG->getCapture(printer);
	}
}

void CIRES2View::OnButtonRotate()
{
	if (osgHull->getNumChildren() < 1)
	{
		AfxMessageBox("Import HULL data first.");
		CalculateOutputResult(current_calculate_type, false);
		return;
	}

	PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
	m_QAddChild.push(pf);
	m_iCurrentStatus = 0;
	m_iSelectionMode = SELECTION_ROTATION;
	m_pTranslationDlg->SetCaption(" << Select a start point for the rotation axis vector or enter X, Y, Z (m)", "Rotation", 3);
	m_pTranslationDlg->ShowWindow(SW_SHOW);
	m_pTranslationDlg->SetFocus();
}

void CIRES2View::OnButtonTranslate()
{
	if (osgHull->getNumChildren() < 1)
	{
		AfxMessageBox("Import HULL data first.");
		CalculateOutputResult(current_calculate_type, false);
		return;
	}

	PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
	m_QAddChild.push(pf);
	m_iCurrentStatus = 0;
	m_iSelectionMode = SELECTION_TRANSLATION;
	m_pTranslationDlg->SetCaption(" << Select a start point for the translation vector or enter X, Y, Z (m)", "Translation", 3);
	m_pTranslationDlg->ShowWindow(SW_SHOW);
	m_pTranslationDlg->SetFocus();
}

void CIRES2View::OnButtonImportHull()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"IGES Files (*.iges , *.igs)|*.iges; *.igs|I-RES2 Files (*.osgb)|*.osgb|All Files (*.*)|*.*||",
		NULL);

	if (dlg.DoModal() == IDOK)
	{
		time_t start_time;
		time_t end_time;
		double diff_time1;

		CString ext_str = dlg.GetFileExt().MakeLower();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		if (ext_str == "iges" || ext_str == "igs")
		{
			time(&start_time);
			char strUtf8[512] = { 0, };
			lpctstr_to_utf8(dlg.GetPathName(), strUtf8);
			IGESControl_Reader Reader;
			Standard_Integer status = Reader.ReadFile(strUtf8);
			if (status == IFSelect_RetDone)
			{
				Reader.TransferRoots();
				TopoDS_Shape aShape = Reader.OneShape();
				time(&end_time);
				diff_time1 = difftime(end_time, start_time);

				osg::Geode* geode = new osg::Geode;
				LoadShapesGeo(aShape, geode);

				if (geode->getNumChildren() > 0)
				{
					osgUtil::Optimizer optimizer;
					optimizer.optimize(geode, osgUtil::Optimizer::OptimizationOptions::ALL_OPTIMIZATIONS);
					optimizer.reset();

					PreFrameUpdateData pf(osgHull, NULL);
					m_QRemoveChild.push(pf);

					PreFrameUpdateData pf1(osgHull, geode);
					m_QAddChild.push(pf1);
					//osgHull->addChild(geode);
					ClearProject(m_strProjectPath);

					m_strProjectPath = GetNewProjectFolder();

					CreateDirectory(m_strProjectPath, NULL);
					CopyFiles(m_strAppPath, m_strProjectPath);
					m_isCreateFolder = true;
					ClearSections();
					m_pMainFrame->m_wndClassView.SetHulllStatus(true);
					m_pMainFrame->m_wndClassView1.SetHulllStatus(true);
					m_pMainFrame->m_wndClassView2.SetHulllStatus(true);
				}
			}
		}
		else//
		{
			time(&start_time);
			time(&end_time);
			diff_time1 = difftime(end_time, start_time);

			CT2CA pszConvertedAnsiString(dlg.GetPathName());
			std::string strStd(pszConvertedAnsiString);
			osg::Node* geo_node = osgDB::readNodeFile(strStd);
			if (geo_node)
			{
				PreFrameUpdateData pf(osgHull, NULL);
				m_QRemoveChild.push(pf);

				PreFrameUpdateData pf1(osgHull, geo_node);
				m_QAddChild.push(pf1);
				//osgHull->addChild(geo_node);

				m_strProjectPath = GetNewProjectFolder();

				CreateDirectory(m_strProjectPath, NULL);
				CopyFiles(m_strAppPath, m_strProjectPath);
				m_isCreateFolder = true;
				ClearSections();
				m_pMainFrame->m_wndClassView.SetHulllStatus(true);
				m_pMainFrame->m_wndClassView1.SetHulllStatus(true);
				m_pMainFrame->m_wndClassView2.SetHulllStatus(true);
			}
		}

		//if (load_ok)
		//{
		//	bbHull.expandBy(osgHull->getBound());
		//	bbHullRadius = osgHull->getBound().radius();
		//	//osg::ComputeBoundsVisitor cbbv;
		//	//osgHull->accept(cbbv);
		//	//osg::BoundingBox bbHull = cbbv.getBoundingBox();

		//	//CString temp_string;
		//	//temp_string.Format("%lf, %lf, %lf, %lf, %lf, %lf", bbHull.xMin(), bbHull.xMax(), bbHull.yMin(), bbHull.yMax(), bbHull.zMin(), bbHull.zMax());
		//	//AfxMessageBox(temp_string);

		//	int child_no = osgWaterline->getNumChildren();
		//	if (child_no > 0)
		//	{
		//		osgWaterline->removeChildren(0, child_no);
		//	}

		//	osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;

		//	osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
		//	osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
		//	v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
		//	v_array->push_back(osg::Vec3(bbHullRadius, -bbHullRadius, 0));
		//	v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));

		//	v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
		//	v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));
		//	v_array->push_back(osg::Vec3(-bbHullRadius, bbHullRadius, 0));

		//	m_iWaterLinePos[0] = bbHull.center().x();
		//	m_iWaterLinePos[1] = bbHull.center().y();
		//	m_iWaterLinePos[2] = bbHull.center().z();

		//	osg::Matrix tr;
		//	tr.setTrans(bbHull.center());
		//	osgWaterline->setMatrix(tr);

		//	osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
		//	n_array->push_back(osg::Vec3(0, 0, 1));

		//	base_palne_geo->setVertexArray(v_array);
		//	osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
		//	cross_color->push_back(osg::Vec4(0.8, 0.8, 1.0, 0.5));
		//	base_palne_geo->setColorArray(cross_color.get());
		//	base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
		//	base_palne_geo->setNormalArray(n_array.get());
		//	base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

		//	osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
		//	de->push_back(0);
		//	de->push_back(1);
		//	de->push_back(2);
		//	de->push_back(3);
		//	de->push_back(4);
		//	de->push_back(5);
		//	base_palne_geo->addPrimitiveSet(de.get());
		//	base_palne_geo->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		//	base_plane->addDrawable(base_palne_geo);

		//	PreFrameUpdateData pf(osgWaterline, base_plane);
		//	m_QAddChild.push(pf);
		//	//osgWaterline->addChild(base_plane);

		//	UpdateGlobalAxis(max(max(bbHull.xMax(), bbHull.yMax()), bbHull.zMax()));

		//	if (m_pEditStart)
		//	{
		//		CString temp_string;
		//		temp_string.Format("%g", bbHull.xMin() * UNIT_TO_M);
		//		m_pEditStart->SetEditText(temp_string);

		//		temp_string.Format("%g", bbHull.xMax() * UNIT_TO_M);
		//		m_pEditEnd->SetEditText(temp_string);

		//		//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 * UNIT_TO_M);
		//		m_pEditSpace->SetEditText("0.5");
		//	}

		//}

		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		//FitWorld();

		time_t end_time1;
		time(&end_time1);
		double diff_time2 = difftime(end_time1, end_time);
		CString temp_string;
		temp_string.Format("Import HULL [%s] : %.1f / %.1f s", dlg.GetFileName(), diff_time1, diff_time2);
		AfxMessageBox(temp_string);
	}
}

bool CIRES2View::LoadShapesGeo(const TopoDS_Shape& aShape, osg::Geode* geode)
{
	//osg::Node* new_geo = NULL;

	TopAbs_ShapeEnum type = aShape.ShapeType();
	fprintf(stderr, ">> LoadShapesGeo : %d\n", type);
	bool find_child = true;

	switch (type)
	{
	case TopAbs_EDGE:
	{
		TopoDS_Edge s = TopoDS::Edge(aShape);
		//if (m_bFaceEdge)
		//{
		//	new_geo = CurveToGeometry(s, m_ParentFace);
		//	if (new_geo)
		//	{

		//		osg::StateSet *ss = new_geo->getOrCreateStateSet();
		//		osg::Material *mtl = new osg::Material();
		//		mtl->setDiffuse(osg::Material::FRONT_AND_BACK,
		//			osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		//		ss->setAttributeAndModes(mtl, osg::StateAttribute::ON |
		//			osg::StateAttribute::OVERRIDE);
		//	}
		//}
		//else
		//{
		if (m_bViewEdge)
		{
			osg::Geometry* geo = CurveToGeometry(s, m_fEdgeDeflection);
			if (geo)
			{
				geode->addChild(geo);
			}
		}
		//}
	}
	break;
	case TopAbs_WIRE:
	{
		//new_geo = new osg::Geode;
	}
	break;
	case TopAbs_SOLID:
	{
		//new_geo = new osg::Group;
	}
	break;
	case TopAbs_SHELL:
	{
		//new_geo = new osg::Group;
	}
	break;
	case TopAbs_FACE:
	{
		TopoDS_Face f = TopoDS::Face(aShape);
		//new_geo = new osg::Group;
		//osg::Geode* geode = new osg::Geode;
		//new_geo->asGroup()->addChild(geode);

		osg::Geometry* geo = FaceToGeometry(f, m_fFaceDeflection);
		if (geo != NULL)
		{
			geo->setDataVariance(osg::Object::DYNAMIC);
			geo->setUseDisplayList(false);
			geo->setUseVertexBufferObjects(true);

			osg::StateSet *ss = geo->getOrCreateStateSet();
			osg::Material *mtl = new osg::Material();
			mtl->setDiffuse(osg::Material::FRONT_AND_BACK,
				osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			ss->setAttributeAndModes(mtl, osg::StateAttribute::ON |
				osg::StateAttribute::OVERRIDE);

			geode->addChild(geo);
		}

		if (m_bViewEdge)
		{
			TopExp_Explorer Ex4(f, TopAbs_EDGE);
			while (Ex4.More())
			{
				TopoDS_Edge s = TopoDS::Edge((Ex4.Current()));
				geo = CurveToGeometry(s, f);
				if (geo)
				{
					osg::StateSet *ss = geo->getOrCreateStateSet();
					osg::Material *mtl = new osg::Material();
					mtl->setDiffuse(osg::Material::FRONT_AND_BACK,
						osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
					ss->setAttributeAndModes(mtl, osg::StateAttribute::ON |
						osg::StateAttribute::OVERRIDE);

					geode->addChild(geo);
				}

				Ex4.Next();
			}
		}
		find_child = false;
	}
	break;
	case TopAbs_VERTEX:
	{
		return NULL;
	}
	break;
	default:
	{
		//new_geo = new osg::Group;
	}
	break;
	}

	if (find_child)
	{
		//new_geo->setName(name);

		//if (col > 0 && m_aColors.size() > col)
		//{
		//	osg::StateSet *ss = new_geo->getOrCreateStateSet();
		//	osg::Material *mtl = new osg::Material();
		//	mtl->setDiffuse(osg::Material::FRONT_AND_BACK,
		//		osg::Vec4(m_aColors[col].x, m_aColors[col].y, m_aColors[col].z, 1.0f));
		//	ss->setAttributeAndModes(mtl, osg::StateAttribute::ON |
		//		osg::StateAttribute::OVERRIDE);
		//}

		for (TopoDS_Iterator its(aShape); its.More(); its.Next()) {
			LoadShapesGeo(its.Value(), geode);
		}
	}

	fprintf(stderr, "LoadShapesGeo << : %d\n", type);
	return true;
}

osg::Geometry* CIRES2View::FaceToGeometry(const TopoDS_Face& aFace, float face_deflection)
{
	//pface->m_Shape = aFace;
	osg::Geometry* sub_geo = NULL;
	try
	{
		if (aFace.IsNull())
			return NULL;
		//	BRepTools::Clean(aFace);
		BRepMesh_IncrementalMesh(aFace, face_deflection);

		TopLoc_Location aLoc;

		// doing the meshing and checking the result
		//BRepMesh_IncrementalMesh MESH(aFace,fDeflection);
		Handle(Poly_Triangulation) aPoly = BRep_Tool::Triangulation(aFace, aLoc);
		//if (aPoly.IsNull()) throw Base::Exception("Empty face trianglutaion\n");
		if (aPoly.IsNull()) return NULL;

		// getting the transformation of the shape/face
		gp_Trsf myTransf;
		Standard_Boolean identity = true;
		if (!aLoc.IsIdentity()) {
			identity = false;
			myTransf = aLoc.Transformation();
		}

		Standard_Integer i;
		int PointNum = aPoly->NbNodes();
		int TriNum = aPoly->NbTriangles();

		//osg::Geode* geode = new osg::Geode();
		//geode->setDataVariance(osg::Object::STATIC);
		sub_geo = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array(PointNum);
		//osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
		//cross_color->push_back(osg::Vec4(color.x, color.y, color.z, 1.0f));
		// geting size and create the array
		//pface->PointNum = aPoly->NbNodes();
		//pface->TriNum = aPoly->NbTriangles();
		//pface->Points.SetCount(pface->PointNum);
		//pface->Normals.SetCount(pface->PointNum);
		//pface->TriList.SetCount(pface->TriNum);

		// check orientation
		TopAbs_Orientation orient = aFace.Orientation();

		// cycling through the poly mesh
		const Poly_Array1OfTriangle& Triangles = aPoly->Triangles();
		const TColgp_Array1OfPnt& Nodes = aPoly->Nodes();

		for (int i = 1; i <= PointNum; i++)
		{
			gp_Pnt V1 = Nodes(i);
			if (!identity) {
				V1.Transform(myTransf);
			}
			v_array->push_back(osg::Vec3(V1.X(), V1.Y(), V1.Z()));
		}

		Standard_Boolean hasUV = aPoly->HasUVNodes();
		Handle_Geom_Surface Surface = BRep_Tool::Surface(aFace);
		if (aPoly->HasNormals())
		{
			const TShort_Array1OfShortReal& nors = aPoly->Normals();
			if (aFace.Orientation() == TopAbs_REVERSED)
			{
				for (i = 0; i < PointNum; i++)
				{
					(*n_array)[i] = osg::Vec3(-nors(3 * i), -nors(3 * i + 1), -nors(3 * i + 2));
				}
			}
			else
			{
				for (i = 0; i < PointNum; i++)
				{
					(*n_array)[i] = osg::Vec3(nors(3 * i), nors(3 * i + 1), nors(3 * i + 2));
				}
			}
		}
		else if (hasUV && !Surface.IsNull())
		{
			const TColgp_Array1OfPnt2d& UVNodes = aPoly->UVNodes();
			gp_Dir Nor;
			Standard_Real Tol = Precision::Confusion();
			for (i = UVNodes.Lower(); i <= UVNodes.Upper(); i++) {

				GeomLib::NormEstim(Surface, UVNodes(i), Tol, Nor);

				if (aFace.Orientation() == TopAbs_REVERSED)
				{
					(*n_array)[i - UVNodes.Lower()] = osg::Vec3(-Nor.X(), -Nor.Y(), -Nor.Z());
				}
				else
				{
					(*n_array)[i - UVNodes.Lower()] = osg::Vec3(Nor.X(), Nor.Y(), Nor.Z());
				}

			}
		}
		else {
			Standard_Integer n[3];
			Standard_Real Tol = Precision::Confusion();

			Poly_Connect pc(aPoly);
			for (i = Nodes.Lower(); i <= Nodes.Upper(); i++) {
				gp_XYZ eqPlan(0, 0, 0);
				for (pc.Initialize(i); pc.More(); pc.Next()) {
					Triangles(pc.Value()).Get(n[0], n[1], n[2]);
					gp_XYZ v1(Nodes(n[1]).Coord() - Nodes(n[0]).Coord());
					gp_XYZ v2(Nodes(n[2]).Coord() - Nodes(n[1]).Coord());
					gp_XYZ vv = v1^v2;
					Standard_Real mod = vv.Modulus();

					if (mod < Tol) continue;

					eqPlan += vv / mod;
				}

				//Standard_Real modmax = eqPlan.Modulus();

				//if(modmax > Tol) Nor(i) = gp_Dir(eqPlan);
				//else Nor(i) = gp_Dir(0., 0., 1.);

				if (aFace.Orientation() == TopAbs_REVERSED)
				{
					(*n_array)[i - Nodes.Lower()] = osg::Vec3(-eqPlan.X(), -eqPlan.Y(), -eqPlan.Z());
				}
				else
				{
					(*n_array)[i - Nodes.Lower()] = osg::Vec3(eqPlan.X(), eqPlan.Y(), eqPlan.Z());
				}

			}
		}

		osg::ref_ptr<osg::DrawElementsUInt> de =
			new osg::DrawElementsUInt(GL_TRIANGLES, 0);

		for (int i = 1; i <= TriNum; i++)
		{
			Standard_Integer N1, N2, N3;
			Triangles(i).Get(N1, N2, N3);
			de->push_back(N1 - 1);
			de->push_back(N2 - 1);
			de->push_back(N3 - 1);
		}

		sub_geo->setVertexArray(v_array);
		sub_geo->setNormalArray(n_array);
		sub_geo->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		//sub_geo->setColorArray(cross_color.get());
		//sub_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
		sub_geo->addPrimitiveSet(de.get());

	}
	catch (Standard_Failure e)
	{
		AfxMessageBox(e.GetMessageString());
		sub_geo = NULL;
	}
	return sub_geo;
}

osg::Geometry* CIRES2View::CurveToGeometry(const TopoDS_Edge& s, const TopoDS_Face& f)
{
	TopLoc_Location aLoc;
	gp_Trsf myTransf;

	Handle(Poly_Triangulation) aPolyTria = BRep_Tool::Triangulation(f, aLoc);
	if (!aLoc.IsIdentity()) {
		myTransf = aLoc.Transformation();
	}

	if (aPolyTria.IsNull()) return NULL;

	// this holds the indices of the edge's triangulation to the actual points
	Handle(Poly_PolygonOnTriangulation) aPoly = BRep_Tool::PolygonOnTriangulation(s, aPolyTria, aLoc);
	if (aPoly.IsNull())
		return NULL;

	// getting size and create the array
	Standard_Integer nbNodesInFace = aPoly->NbNodes();

	const TColStd_Array1OfInteger& indices = aPoly->Nodes();
	const TColgp_Array1OfPnt& Nodes = aPolyTria->Nodes();

	osg::Geometry* sub_geo = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;

	gp_Pnt V;
	int pos = 0;
	// go through the index array
	for (Standard_Integer i = indices.Lower(); i <= indices.Upper(); i++) {
		V = Nodes(indices(i));
		V.Transform(myTransf);
		v_array->push_back(osg::Vec3((float)(V.X()), (float)(V.Y()), (float)(V.Z())));
	}

	sub_geo->setVertexArray(v_array);

	sub_geo->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, nbNodesInFace));
	osg::LineWidth* lineWidth = new osg::LineWidth(2);
	sub_geo->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
	sub_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return sub_geo;
}

osg::Geometry* CIRES2View::CurveToGeometry(const TopoDS_Edge& s, float edge_deflection)
{
	try{
		BRepAdaptor_Curve aCurve(s);
		int nbintervals = 1;
		Standard_Real U1, U2;
		FindLimits(aCurve, 500, U1, U2);

		if (aCurve.GetType() == GeomAbs_BSplineCurve) {
			nbintervals = aCurve.NbKnots() - 1;
			nbintervals = Max(1, nbintervals / 3);
		}

		osg::Geometry* sub_geo = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;

		//Standard_Boolean isPrimArrayEnabled = Graphic3d_ArrayOfPrimitives::IsEnable() && !drawCurve;
		switch (aCurve.GetType()) {
		case GeomAbs_Line:
		{
			gp_Pnt p = aCurve.Value(U1);
			v_array->push_back(osg::Vec3(p.X(), p.Y(), p.Z()));

			p = aCurve.Value(U2);
			v_array->push_back(osg::Vec3(p.X(), p.Y(), p.Z()));
		}
		break;
		default:
		{
			int NbP = 0;

			GCPnts_UniformDeflection discretizer;
			discretizer.Initialize(aCurve, edge_deflection, U1, U2);//don't forget parameters!!!		  Standard_Real U;
			if (discretizer.IsDone())
			{
				NbP = discretizer.NbPoints();
				Standard_Real U;
				Standard_Integer N = Max(2, NbP*nbintervals);
				Standard_Real DU = (U2 - U1) / (N - 1);
				gp_Pnt p;

				for (Standard_Integer i = 1; i <= N; i++) {
					U = U1 + (i - 1)*DU;
					p = aCurve.Value(U);
					v_array->push_back(osg::Vec3(p.X(), p.Y(), p.Z()));
				}
			}
		}
		break;
		}
		sub_geo->setVertexArray(v_array);

		sub_geo->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, v_array->size()));
		osg::LineWidth* lineWidth = new osg::LineWidth(2);
		sub_geo->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
		sub_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		return sub_geo;

	}
	catch (...)
	{
	}
	return NULL;
}

void CIRES2View::FindLimits(const Adaptor3d_Curve& aCurve,
	const Standard_Real  aLimit,
	Standard_Real&       First,
	Standard_Real&       Last)
{
	try{
		First = aCurve.FirstParameter();
		Last = aCurve.LastParameter();
		Standard_Boolean firstInf = Precision::IsNegativeInfinite(First);
		Standard_Boolean lastInf = Precision::IsPositiveInfinite(Last);

		if (firstInf || lastInf) {
			gp_Pnt P1, P2;
			Standard_Real delta = 1;
			if (firstInf && lastInf) {
				do {
					delta *= 2;
					First = -delta;
					Last = delta;
					aCurve.D0(First, P1);
					aCurve.D0(Last, P2);
				} while (P1.Distance(P2) < aLimit);
			}
			else if (firstInf) {
				aCurve.D0(Last, P2);
				do {
					delta *= 2;
					First = Last - delta;
					aCurve.D0(First, P1);
				} while (P1.Distance(P2) < aLimit);
			}
			else if (lastInf) {
				aCurve.D0(First, P1);
				do {
					delta *= 2;
					Last = First + delta;
					aCurve.D0(Last, P2);
				} while (P1.Distance(P2) < aLimit);
			}
		}
	}
	catch (...)
	{
	}
}


void CIRES2View::FitWorld()
{
	mOSG->getViewer()->home();
	//mOSG->OnViewIso();
}

void CIRES2View::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	SetCenterView(point.x, point.y);

	CView::OnRButtonDblClk(nFlags, point);
}

void CIRES2View::SetCenterView(double x, double y)
{
	osg::Vec3d hit_pt;
	osg::Node* p_element = OnSelectPoint(x, y, hit_pt);
	if (p_element)
	{
		osgGA::CameraManipulator* cm = mOSG->getViewer()->getCameraManipulator();
		osgGA::KeySwitchMatrixManipulator* ksm = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(cm);
		cm = ksm->getCurrentMatrixManipulator();
		osgGA::OptMultiTouchTrackballManipulator* trackball = dynamic_cast<osgGA::OptMultiTouchTrackballManipulator*>(cm);

		//CString temp_string;
		//temp_string.Format("center [%lf, %lf, %lf]", hit_pt.x(), hit_pt.y(), hit_pt.z());
		//CMainFrame* pFrame = (CMainFrame*)::AfxGetApp()->GetMainWnd();
		//pFrame->AddOutput(O_INFO, temp_string);
		if (trackball)
			trackball->setCenter(hit_pt);
	}
}

osg::Node* CIRES2View::OnSelectPoint(double x, double y, osg::Vec3d& hit_pt)
{
	y = mOSG->getViewer()->getCamera()->getViewport()->height() - y;
	//if (sel_type == OPT_PATH || sel_type == OPT_TAG || sel_type == OPT_FRAME)
	//{
		osgUtil::PolytopeIntersector::Intersections poly_intersections;
		double w = 5.0f;
		double h = 5.0f;
		osg::ref_ptr< osgUtil::PolytopeIntersector > poly_picker = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x - w, y - h, x + w, y + h);
		osgUtil::IntersectionVisitor poly_iv(poly_picker.get());
		mOSG->getViewer()->getCamera()->accept(poly_iv);
		if (poly_picker->containsIntersections())
		{
			poly_intersections = poly_picker->getIntersections();
			//if (view->computeIntersections(x,y,intersections))
			//{
			for (osgUtil::PolytopeIntersector::Intersections::iterator hitr = poly_intersections.begin();
				hitr != poly_intersections.end();
				++hitr)
			{
				const osg::NodePath& nodePath = hitr->nodePath;
				unsigned int idx = nodePath.size();
				while (idx--){
					osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(nodePath[idx]);
					if (mt == NULL)
						continue;

					if (mt->getName() == "DATUM")
					{
						osg::Matrix tr = osg::computeLocalToWorld(hitr->nodePath);
						hit_pt = tr.getTrans();
						m_geoVertex = hit_pt;
						return mt;
					}
					//if (m_mapGeoToEntity2.find(mt) != m_mapGeoToEntity2.end())
					//{
					//	if (m_mapGeoToEntity2[mt]->GetType() == sel_type || OPT_ELEMENT == sel_type)
					//	{
					//		return m_mapGeoToEntity2[mt];
					//	}
					//}
				}
			}
		}
	//}
	//else
	//{
		osgUtil::LineSegmentIntersector::Intersections intersections;
		osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
		osgUtil::IntersectionVisitor iv(picker.get());
		mOSG->getViewer()->getCamera()->accept(iv);
		if (picker->containsIntersections())
		{
			intersections = picker->getIntersections();
			//if (view->computeIntersections(x,y,intersections))
			//{
			for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
				hitr != intersections.end();
				++hitr)
			{
				m_SelectedGeo = dynamic_cast<osg::Geometry*>(hitr->drawable.get());
				
				const vector< unsigned int >& selIndices = hitr->indexList;
				if (m_SelectedGeo && selIndices.size() > 2)
				{
					m_vecSelectedNormal = hitr->getLocalIntersectNormal();
					hit_pt = hitr->getWorldIntersectPoint();
					osg::Vec3 point = hit_pt;
					osg::Matrix  vpMatrix = mOSG->getViewer()->getCamera()->getViewMatrix() * mOSG->getViewer()->getCamera()->getProjectionMatrix();
					osg::Vec3 point_2d = point * vpMatrix;
					osg::Matrix m_SelectedGeoLocalToWorld = osg::computeLocalToWorld(hitr->nodePath);
					osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(m_SelectedGeo->getVertexArray());
					float distance_vertex;
					float distance_edge;
					for (int i = 0; i < selIndices.size(); i++)
					{
						unsigned int pos = selIndices[i];
						osg::Vec3 p0 = (*vertices)[pos] * m_SelectedGeoLocalToWorld;
						m_geoFace[i] = p0;
						float cur_distance = (p0 * vpMatrix - point_2d).length();
						if (i == 0)
						{
							distance_vertex = cur_distance;
							m_geoVertex = p0;
						}
						else
						{
							if (cur_distance < distance_vertex)
							{
								distance_vertex = cur_distance;
								m_geoVertex = p0;
							}
						}

						int j = i - 1;
						if (j < 0)
							j = 2;
						unsigned int pos2 = selIndices[j];
						osg::Vec3 p1 = (*vertices)[pos2] * m_SelectedGeoLocalToWorld;
						osg::Vec3 v = p1 - p0;
						osg::Vec3 w = point - p0;
						osg::Vec3 p;
						float c1 = w * v;
						float c2 = v * v;
						if (c1 <= 0)
						{
							cur_distance = (point - p0).length();
							p = p0;
						}
						else if (c2 <= c1)
						{
							cur_distance = (point - p1).length();
							p = p1;
						}
						else
						{
							float b = c1 / c2;
							p = p0 + (v * b);
							cur_distance = (point - p).length();
						}

						if (i == 0)
						{
							distance_edge = cur_distance;
							m_geoEdge[0] = p0;
							m_geoEdge[1] = p1;
							m_geoEdge[2] = p;
						}
						else
						{
							if (cur_distance < distance_edge)
							{
								distance_edge = cur_distance;
								m_geoEdge[0] = p0;
								m_geoEdge[1] = p1;
								m_geoEdge[2] = p;
							}
						}

					}
					m_geoNormal = ((m_geoFace[1]) - (m_geoFace[0])) ^ ((m_geoFace[2]) - (m_geoFace[1]));
					m_geoNormal.normalize();

					const osg::NodePath& nodePath = hitr->nodePath;
					unsigned int idx = nodePath.size();
					while (idx--){
						osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(nodePath[idx]);
						if (mt == NULL)
							continue;

						if(mt == osgHull)
							return mt;
					}
				}
			}
		}
	//}
	return NULL;
}

void CIRES2View::UpdateGlobalAxis(float length)
{
	osg::Geode* geode = new osg::Geode;

	osg::LineWidth* lineWidth = new osg::LineWidth(1);
	geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	double pyramidBaseZ = length / 100.0f;
	double outerBaseRadius = length / 200.0f;
	//	x axis
	osg::Cone* cone = new osg::Cone(osg::Vec3(length - pyramidBaseZ, 0.0f, 0.0f), outerBaseRadius, pyramidBaseZ);
	osg::Quat rotation;
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(1, 0, 0));
	cone->setRotation(rotation);
	osg::ShapeDrawable* shape = new osg::ShapeDrawable(cone);
	shape->setColor(osg::Vec4(1, 0, 0, 1));
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createXAxis(length - pyramidBaseZ, pyramidBaseZ, outerBaseRadius));

	//	 y axis
	cone = new osg::Cone(osg::Vec3(0.0f, length - pyramidBaseZ, 0.0f), outerBaseRadius, pyramidBaseZ);
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0, 1, 0));
	cone->setRotation(rotation);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(osg::Vec4(0, 1, 0, 1));
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createYAxis(length - pyramidBaseZ, pyramidBaseZ, outerBaseRadius));

	//	 z axis
	cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, length - pyramidBaseZ), outerBaseRadius, pyramidBaseZ);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(osg::Vec4(0, 0, 1, 1));
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createZAxis(length - pyramidBaseZ, pyramidBaseZ, outerBaseRadius));

	osg::Vec4 color(0, 0, 0, 1);
	geode->addDrawable(mOSG->createAxisLabel("X", osg::Vec3(length + 10, 0, 0), color));
	geode->addDrawable(mOSG->createAxisLabel("Y", osg::Vec3(0, length + 10, 0), color));
	geode->addDrawable(mOSG->createAxisLabel("Z", osg::Vec3(0, 0, length + 10), color));

	int count = osgAxis->getNumChildren();
	if (count > 0)
	{
		osgAxis->removeChildren(0, count);
	}

	osgAxis->addChild(geode);
}

void CIRES2View::OnButtonDefineSections()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_iCurrentStep < 1)
	{
		AfxMessageBox("Import HULL first.");
		return;
	}

	//if (m_pEditStart == NULL)
	//{
	//	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	//	if (pFrame)
	//	{
	//		m_pEditStart = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START));
	//		m_pEditEnd = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END));
	//		m_pEditSpace = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE));
	//		//m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));
	//		m_pEditPointsDistance = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE));
	//		m_pEditPointsNumber = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER));

	//		m_pEditStartWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START_WATERLINE));
	//		m_pEditEndWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END_WATERLINE));
	//		m_pEditSpaceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE_WATERLINE));
	//		m_pEditPointsDistanceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE_WATERLINE));
	//		m_pEditPointsNumberWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER_WATERLINE));
	//	}
	//}

	//if (m_pEditStart)
	//{
	//	CString temp_string;
	//	m_aSectionStart.clear();
	//	m_aSectionEnd.clear();
	//	m_aSectionOffset.clear();

	//	temp_string = m_pEditStart->GetEditText();
	//	m_aSectionStart.push_back(atof(temp_string) * M_TO_UNIT);

	//	temp_string = m_pEditEnd->GetEditText();
	//	m_aSectionEnd.push_back(atof(temp_string) * M_TO_UNIT);

	//	temp_string = m_pEditSpace->GetEditText();
	//	m_aSectionOffset.push_back(atof(temp_string) * M_TO_UNIT);

	//	ClearSections();
	//	DefineSections();
	//}
}

void CIRES2View::ClearSections()
{
	//for each(auto section in osgSections)
	//{
	//	PreFrameUpdateData pf(mOSG->mRoot, section);
	//	m_QRemoveChild.push(pf);
	//}
	for each(auto section in osgSectionsData)
	{
		PreFrameUpdateData pf(mOSG->mRoot, section);
		m_QRemoveChild.push(pf);
	}

	//osgSections.clear();
	osgSectionsData.clear();
	osgSectionPosList.clear();
	osgSectionEnable.clear();

	//ClearSectionPoints();
}

void CIRES2View::DefineSections()
{
	//RemoveSection();
	//RemoveSectionPoints();

	char label_txt[255];

	for (int i = 0; i < m_aSectionStart.size(); i++)
	{
		float center_x = bbHull.center().x();
		float center_y = bbHull.center().y();
		float center_z = bbHull.center().z();
		if (m_bBowBreaking)
		{
			center_x = m_aSectionStart[i];
			float prev_x = center_x;
			while (center_x >= m_aSectionEnd[i])
			{
				osg::Vec3 center_pnt(center_x, center_y, center_z);
				osgSectionPosList.push_back(center_pnt);
				osgSectionEnable.push_back(true);

				prev_x = center_x;
				center_x -= m_aSectionOffset[i];
			}

			//reverse(osgSectionPosList.begin(), osgSectionPosList.end());

			for each(auto center_pnt in osgSectionPosList)
			{
				//osg::Plane section_plane(osg::Vec3(1, 0, 0), osg::Vec3(0, 0, 0));

				//osg::ref_ptr<osg::MatrixTransform> base_plane_tr = new osg::MatrixTransform;
				//osgSections.push_back(base_plane_tr);

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr1 = new osg::MatrixTransform;
				osgSectionsData.push_back(base_plane_tr1);

				//osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
				//base_plane_tr->addChild(base_plane);

				//osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
				//osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
				//v_array->push_back(osg::Vec3(0, -bbLength[1], -bbLength[2]));
				//v_array->push_back(osg::Vec3(0, bbLength[1], -bbLength[2]));
				//v_array->push_back(osg::Vec3(0, bbLength[1], bbLength[2]));

				//v_array->push_back(osg::Vec3(0, -bbLength[1], -bbLength[2]));
				//v_array->push_back(osg::Vec3(0, bbLength[1], bbLength[2]));
				//v_array->push_back(osg::Vec3(0, -bbLength[1], bbLength[2]));

				//osg::Matrix tr;
				//osg::Quat q(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
				//tr.setTrans(center_pnt);
				//tr.setRotate(q);
				//base_plane_tr->setMatrix(tr);

				//osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
				//n_array->push_back(osg::Vec3(1, 0, 0));

				//base_palne_geo->setVertexArray(v_array);
				//osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
				//cross_color->push_back(osg::Vec4(1, 1, 0.5, 0.5));
				//base_palne_geo->setColorArray(cross_color.get());
				//base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
				//base_palne_geo->setNormalArray(n_array.get());
				//base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

				//osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
				//de->push_back(0);
				//de->push_back(1);
				//de->push_back(2);
				//de->push_back(3);
				//de->push_back(4);
				//de->push_back(5);
				//base_palne_geo->addPrimitiveSet(de.get());

				//osg::StateSet *ss = base_palne_geo->getOrCreateStateSet();
				//ss->setMode(GL_BLEND, osg::StateAttribute::ON);
				//osg::ref_ptr<osg::Depth> depth = new osg::Depth;
				//depth->setWriteMask(true);
				//ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
				//ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				//ss->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

				//base_plane->addDrawable(base_palne_geo);

				//sprintf_s(label_txt, 255, "%.2lf", center_pnt.x() * UNIT_TO_M);
				//osgText::Text* text = new  osgText::Text;
				//osgText::Font* normal_font = osgText::readFontFile("fonts/arial.ttf");
				//text->setFont(normal_font);
				//text->setText(label_txt);
				//text->setPosition(osg::Vec3(0, -bbLength[1], bbLength[2]));
				//text->setCharacterSize(15.0f);
				//text->setFontResolution(20, 20);
				//text->setAutoRotateToScreen(true);
				//text->setColor(osg::Vec4(0, 0, 0, 1));
				//text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
				//text->setAlignment(osgText::Text::CENTER_CENTER);
				//base_plane->addDrawable(text);

				//PreFrameUpdateData pf(mOSG->mRoot, base_plane_tr);
				//m_QAddChild.push(pf);
				PreFrameUpdateData pf1(mOSG->mRoot, base_plane_tr1);
				m_QAddChild.push(pf1);
				//mOSG->mRoot->addChild(base_plane_tr);
				//mOSG->mRoot->addChild(base_plane_tr1);
			}
		}
		else
		{
			center_y = m_aSectionStart[i];
			float prev_y = center_y;
			while (center_y > m_aSectionEnd[i])
			{
				osg::Vec3 center_pnt(center_x, center_y, center_z);
				osgSectionPosList.push_back(center_pnt);
				osgSectionEnable.push_back(true);

				prev_y = center_y;
				center_y -= m_aSectionOffset[i];
			}

			//reverse(osgSectionPosList.begin(), osgSectionPosList.end());

			for each(auto center_pnt in osgSectionPosList)
			{
				//osg::Plane section_plane(osg::Vec3(1, 0, 0), osg::Vec3(0, 0, 0));

				//osg::ref_ptr<osg::MatrixTransform> base_plane_tr = new osg::MatrixTransform;
				//osgSections.push_back(base_plane_tr);

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr1 = new osg::MatrixTransform;
				osgSectionsData.push_back(base_plane_tr1);

				//osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
				//base_plane_tr->addChild(base_plane);

				//osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
				//osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
				//v_array->push_back(osg::Vec3(-bbLength[0], 0, -bbLength[2]));
				//v_array->push_back(osg::Vec3(bbLength[0], 0, -bbLength[2]));
				//v_array->push_back(osg::Vec3(bbLength[0], 0, bbLength[2]));

				//v_array->push_back(osg::Vec3(-bbLength[0], 0, -bbLength[2]));
				//v_array->push_back(osg::Vec3(bbLength[0], 0, bbLength[2]));
				//v_array->push_back(osg::Vec3(-bbLength[0], 0, bbLength[2]));

				//osg::Matrix tr;
				//osg::Quat q(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
				//tr.setTrans(center_pnt);
				//tr.setRotate(q);
				//base_plane_tr->setMatrix(tr);

				//osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
				//n_array->push_back(osg::Vec3(0, 1, 0));

				//base_palne_geo->setVertexArray(v_array);
				//osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
				//cross_color->push_back(osg::Vec4(1, 1, 0.5, 0.5));
				//base_palne_geo->setColorArray(cross_color.get());
				//base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
				//base_palne_geo->setNormalArray(n_array.get());
				//base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

				//osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
				//de->push_back(0);
				//de->push_back(1);
				//de->push_back(2);
				//de->push_back(3);
				//de->push_back(4);
				//de->push_back(5);
				//base_palne_geo->addPrimitiveSet(de.get());

				//osg::StateSet *ss = base_palne_geo->getOrCreateStateSet();
				//ss->setMode(GL_BLEND, osg::StateAttribute::ON);
				//osg::ref_ptr<osg::Depth> depth = new osg::Depth;
				//depth->setWriteMask(true);
				//ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
				//ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				//ss->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

				//base_plane->addDrawable(base_palne_geo);

				//sprintf_s(label_txt, 255, "%.2lf", center_pnt.y() * UNIT_TO_M);
				//osgText::Text* text = new  osgText::Text;
				//osgText::Font* normal_font = osgText::readFontFile("fonts/arial.ttf");
				//text->setFont(normal_font);
				//text->setText(label_txt);
				//text->setPosition(osg::Vec3(bbLength[0], 0, bbLength[2]));
				//text->setCharacterSize(15.0f);
				//text->setFontResolution(20, 20);
				//text->setAutoRotateToScreen(true);
				//text->setColor(osg::Vec4(0, 0, 0, 1));
				//text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
				//text->setAlignment(osgText::Text::CENTER_CENTER);
				//base_plane->addDrawable(text);

				//PreFrameUpdateData pf(mOSG->mRoot, base_plane_tr);
				//m_QAddChild.push(pf);
				PreFrameUpdateData pf1(mOSG->mRoot, base_plane_tr1);
				m_QAddChild.push(pf1);
				//mOSG->mRoot->addChild(base_plane_tr);
				//mOSG->mRoot->addChild(base_plane_tr1);
			}
		}
	}

	//SetCurrentStep(2);
	//ClearSectionPoints();
}


void CIRES2View::OnCheckBowBreaking()
{
	m_bBowBreaking = true;
	if (m_bBowBreaking)
		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
	else
		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));

	osg::Vec3 diff(0, 0, - bbLength[2] * 2.0f);
	for (int i = 0; i < osgSectionPosList.size(); i++)
	{
		osg::Matrix m;
		if (osgSectionEnable[i])
		{
			m.setTrans(osgSectionPosList[i]);
		}
		else
		{
			m.setTrans(osgSectionPosList[i] + diff);
		}
		m.setRotate(osgSectionRotation);
		//osgSections[i]->setMatrix(m);
	}
}


void CIRES2View::OnUpdateCheckBowBreaking(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBowBreaking);
}


void CIRES2View::OnCheckStern()
{
	m_bBowBreaking = false;
	if (m_bBowBreaking)
		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
	else
		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));

	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
	for (int i = 0; i < osgSectionPosList.size(); i++)
	{
		osg::Matrix m;
		if (osgSectionEnable[i])
		{
			m.setTrans(osgSectionPosList[i]);
		}
		else
		{
			m.setTrans(osgSectionPosList[i] + diff);
		}
		m.setRotate(osgSectionRotation);
		//osgSections[i]->setMatrix(m);
	}
}


void CIRES2View::OnUpdateCheckStern(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bBowBreaking);
}


void CIRES2View::OnEditStart()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnEditEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnEditSpace()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnEditPointGap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

bool sort_curves_x(vector< osg::Vec3 > i, vector< osg::Vec3 > j)
{
	return (i[0].x() > j[0].x());
}

bool sort_curves_y(vector< osg::Vec3 > i, vector< osg::Vec3 > j)
{
	return (i[0].y() > j[0].y());
}

bool sort_curves_z(vector< osg::Vec3 > i, vector< osg::Vec3 > j)
{
	return (i[0].z() < j[0].z());
}

bool same_side(osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& a, osg::Vec3& b)
{
	osg::Vec3 cp1 = (b - a) ^ (p1 - a);
	osg::Vec3 cp2 = (b - a) ^ (p2 - a);
	return (cp1*cp2 >= 0);
}

//vector< osg::Vec3 > temp_pt_list;
bool CIRES2View::GetNormal(osg::Drawable* geo, PointData& pd)
{
	osg::Geometry* geom = geo->asGeometry();
	if (geom)
	{
		osg::Vec3Array *vertices = (osg::Vec3Array *)geom->getVertexArray();
		osg::Vec3Array *normals = (osg::Vec3Array *)geom->getNormalArray();
		osg::Geometry::PrimitiveSetList primitiveList = geom->getPrimitiveSetList();
		osg::Matrix world(osg::computeLocalToWorld(geo->getParentalNodePaths()[0]));

		int polygonIndex = 0;
		osg::Vec3 p1, p2, p3;
		int index1, index2, index3;
		//double dot00;
		//double dot01;
		//double dot02;
		//double dot11;
		//double dot12;
		//double invDenom, u, v;
		osg::Vec3 v0, v1, v2, v3;

		for (int x = 0; x <primitiveList.size(); x++)
		{
			//fprintf(FileLog, "GetNormal [%d / %d] cl : %lf, sl : %lf, rl : %lf\n", x, primitiveList.size(), pd.pnt.x(), pd.pnt.y(), pd.pnt.z());

			osg::PrimitiveSet *set = primitiveList[x];
			int numberOfIndices = set->getNumIndices();
			if (set->getMode() == osg::PrimitiveSet::Mode::TRIANGLES)
			{
				for (unsigned int y = 0; y < set->getDrawElements()->getNumIndices() /*numberOfIndices*/; y+=3)
				{
					index1 = set->getDrawElements()->getElement(y); //set->index(y); 
					if (index1 < vertices->size())
					{
						p1 = vertices->at(index1) * world;
					}
					index2 = set->getDrawElements()->getElement(y +1); //set->index(y); 
					if (index2 < vertices->size())
					{
						p2 = vertices->at(index2) * world;
					}
					index3 = set->getDrawElements()->getElement(y +2); //set->index(y); 
					if (index3 < vertices->size())
					{
						p3 = vertices->at(index3) * world;
					}

					//	Same Side Technique
					bool ss1 = same_side(pd.pnt, p1, p2, p3);
					bool ss2 = same_side(pd.pnt, p2, p1, p3);
					bool ss3 = same_side(pd.pnt, p3, p1, p2);
					//fprintf(FileLog, "GetNormal \t %d, %d, %d >> %d, %d, %d\n", index1, index2, index3, ss1, ss2, ss3);
					if (ss1 && ss2 && ss3)
					{
						v1 = p2 - p1;
						v3 = p3 - p2;
						osg::Vec3 normal = v1 ^ v3;
						//osg::Vec3 normal = normals->at(index1) + normals->at(index2) + normals->at(index3);
						normal.normalize();
						pd.normal = normal;

						try	//	for alpha
						{
							gp_Lin2d lin0(gp_Pnt2d(pd.pnt.x(), pd.pnt.y()), gp_Dir2d(pd.normal.y(), -pd.normal.x()));
							gp_Lin2d lin1(gp_Pnt2d(0, 0), gp_Dir2d(1, 0));
							IntAna2d_AnaIntersection Inters;
							Inters.Perform(lin0, lin1);
							if (Inters.IsDone())
							{
								if (Inters.IsDone())
								{
									if (!Inters.IdenticalElements() && !Inters.ParallelElements())
									{
										pd.alpha_exist = true;
										pd.pnt_alpha = osg::Vec3(Inters.Point(1).Value().X(), Inters.Point(1).Value().Y(), pd.pnt.z());
										pd.angle_alpha = abs(lin0.Angle(lin1) * 180.0 / M_PI);
										if (pd.angle_alpha > 90)
										{
											pd.angle_alpha = 180.0f - pd.angle_alpha;
										}
									}
								}
							}
						}
						catch (Standard_Failure e)
						{
							//AfxMessageBox(e.GetMessageString());
						}

						try	//	for beta
						{
							gp_Lin2d lin0(gp_Pnt2d(pd.pnt.y(), pd.pnt.z()), gp_Dir2d(pd.normal.z(), -pd.normal.y()));
							gp_Lin2d lin1(gp_Pnt2d(0, 0), gp_Dir2d(0, 1));
							IntAna2d_AnaIntersection Inters;
							Inters.Perform(lin0, lin1);
							if (Inters.IsDone())
							{
								if (Inters.IsDone())
								{
									if (!Inters.IdenticalElements() && !Inters.ParallelElements())
									{
										pd.beta_exist = true;
										pd.pnt_beta = osg::Vec3(pd.pnt.x(), Inters.Point(1).Value().X(), Inters.Point(1).Value().Y());
										pd.angle_beta = abs(lin0.Angle(lin1) * 180.0 / M_PI);
										if (pd.angle_beta > 90)
										{
											pd.angle_beta = 180.0f - pd.angle_beta;
										}
									}
								}
							}
						}
						catch (Standard_Failure e)
						{
							//AfxMessageBox(e.GetMessageString());
						}

						try	//	for gamma
						{
							gp_Lin2d lin0(gp_Pnt2d(pd.pnt.x(), pd.pnt.z()), gp_Dir2d(pd.normal.z(), -pd.normal.x()));
							gp_Lin2d lin1(gp_Pnt2d(0, 0), gp_Dir2d(0, 1));
							IntAna2d_AnaIntersection Inters;
							Inters.Perform(lin0, lin1);
							if (Inters.IsDone())
							{
								if (Inters.IsDone())
								{
									if (!Inters.IdenticalElements() && !Inters.ParallelElements())
									{
										pd.gamma_exist = true;
										pd.pnt_gamma = osg::Vec3(Inters.Point(1).Value().X(), pd.pnt.y(), Inters.Point(1).Value().Y());
										pd.angle_gamma = abs(lin0.Angle(lin1) * 180.0 / M_PI);
										if (pd.angle_gamma > 90)
										{
											pd.angle_gamma = 180.0f - pd.angle_gamma;
										}
									}
								}
							}
						}
						catch (Standard_Failure e)
						{
							//AfxMessageBox(e.GetMessageString());
						}
						return true;
					}
				}
			}
		}
	}
	pd.normal = osg::Vec3(0, 0, 0);
	return false;
}

void CIRES2View::CalculateSectionWaterline(osg::Vec3 plane_normal, osg::Vec3 plane_point, int align_axis, vector< PointData >& section_point_data, bool check_point_distance, float point_distance, vector< vector< osg::Vec3 > >& section_line, bool use_start_end, float start_pos, float end_pos)
{
	map< double, osg::Drawable* > loop_geo;
	osg::Plane water_plane(plane_normal, plane_point);
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(water_plane, osg::Polytope());
	//intersector->enter(*dynamic_cast<osg::Node*>(m_mtScan.get()));
	osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	osgUtil::IntersectionVisitor            _intersectionVisitor;
	_intersectionVisitor.reset();
	_intersectionVisitor.setIntersector(intersector.get());

	osgHull_Center.get()->accept(_intersectionVisitor);
	int prev_count = m_iStatus;
	int total_count = 0;
	int current_count = 0;
	typedef osgUtil::PlaneIntersector::Intersection::Polyline Polyline;
	if (!intersections.empty())
	{
		total_count = intersections.size() * 3;

		osgUtil::PlaneIntersector::Intersections::iterator itr;
		for (itr = intersections.begin();
			itr != intersections.end();
			++itr)
		{
			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
			current_count++;
			m_iStatus = prev_count + (current_count / total_count) * 100;
			UpdateProgress();
			if (intersection.matrix.valid())
			{
				// osg::notify(osg::NOTICE)<<"  transforming "<<std::endl;
				// transform points on polyline 
				for (Polyline::iterator pitr = intersection.polyline.begin();
					pitr != intersection.polyline.end();
					++pitr)
				{
					*pitr = (*pitr) * (*intersection.matrix);
				}

				// matrix no longer needed.
				intersection.matrix = 0;
			}
		}

		for (itr = intersections.begin();
			itr != intersections.end();
			++itr)
		{
			current_count++;
			m_iStatus = prev_count + (current_count / total_count) * 100;
			UpdateProgress();
			//	more에서 문제가 있어서 중간 포인트 정리하는 과정을 생략한다.
			//map< CString, int > temp_exist;
			CString str_exist;
			vector< osg::Vec3 > temp_loop;
			osg::Vec3 temp_pt;
			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
			Polyline::iterator pitr = intersection.polyline.begin();
			for (;
				pitr != intersection.polyline.end();
				++pitr)
			{
				temp_pt = *pitr;
				//str_exist.Format("%.0lf-%.0lf-%.0lf", temp_pt.x() * 100.0f, temp_pt.y() * 100.0f, temp_pt.z() * 100.0f);
				//if (temp_exist.find(str_exist) == temp_exist.end())
				//{
				temp_loop.push_back(temp_pt);
				//	temp_exist[str_exist] = 1;
				//}

			}

			switch (align_axis)
			{
			case 0:
			{
				if (temp_loop[0].x() < temp_loop[temp_loop.size() - 1].x()) std::reverse(temp_loop.begin(), temp_loop.end());
				loop_geo[temp_loop[0].x()] = intersection.drawable.get();
			}
			break;
			case 1:
			{
				if (temp_loop[0].y() < temp_loop[temp_loop.size() - 1].y()) std::reverse(temp_loop.begin(), temp_loop.end());
				loop_geo[temp_loop[0].y()] = intersection.drawable.get();
			}
			break;
			case 2:
			{
				if (temp_loop[0].z() > temp_loop[temp_loop.size() - 1].z()) std::reverse(temp_loop.begin(), temp_loop.end());
				loop_geo[temp_loop[0].z()] = intersection.drawable.get();
			}
			break;
			}
			section_line.push_back(temp_loop);
		}

		switch (align_axis)
		{
		case 0:
		{
			sort(section_line.begin(), section_line.end(), sort_curves_x);
			if (check_point_distance)
			{
				float remain_length = 0;
				for (int j = 0; j < section_line.size(); j++)
				{
					current_count++;
					m_iStatus = prev_count + (current_count / total_count) * 100;
					UpdateProgress();
					if (section_line[j].size() > 0)
					{
						osg::Drawable* geo = loop_geo[section_line[j][0].x()];

						for (int i = 1; i < section_line[j].size(); i++)
						{
							osg::Vec3 dir = section_line[j][i] - section_line[j][i - 1];
							float current_length = dir.length();
							float step_length = 0;
							dir.normalize();
							//fprintf(stderr, "before while current_length : %lf, step_length : %lf, remain_length : %lf\n", current_length, step_length, remain_length);
							while (current_length - step_length > remain_length)
							{
								PointData pd;
								step_length += remain_length;
								pd.pnt = section_line[j][i - 1] + (dir * step_length);
								if ((pd.pnt.z() * UNIT_TO_M <= DRAFT) && ((use_start_end == false) || (pd.pnt.x() <= start_pos && pd.pnt.x() >= end_pos)))
								{
									if(GetNormal(geo, pd))
										section_point_data.push_back(pd);
								}
								remain_length = point_distance;
								//fprintf(stderr, "in while current_length : %lf, step_length : %lf, remain_length : %lf (%.2lf, %.2lf, %.2lf)\n", current_length, step_length, remain_length, pd.pnt.x(), pd.pnt.y(), pd.pnt.z());
							}

							remain_length = remain_length - (current_length - step_length);
						}
					}
				}
			}
			else
			{
				//	waterline
				for (int j = 0; j < section_line.size(); j++)
				{
					current_count++;
					m_iStatus = prev_count + (current_count / total_count) * 100;
					UpdateProgress();
					if (section_line[j].size() > 0)
					{
						osg::Drawable* geo = loop_geo[section_line[j][0].x()];
						float current_x = /*floor*/(section_line[j][0].x() * UNIT_TO_M) * M_TO_UNIT + point_distance;
						while (current_x > section_line[j][0].x())
						{
							current_x -= point_distance;
						}

						if (use_start_end)
						{
							if (current_x > start_pos)
								current_x = start_pos;
						}

						for (int i = 1; i < section_line[j].size(); i++)
						{
							while (section_line[j][i].x() <= current_x && ((use_start_end==false) || (current_x >= end_pos)))
							{
								PointData pd;
								float ratio = (current_x - section_line[j][i - 1].x()) / (section_line[j][i].x() - section_line[j][i - 1].x());
								osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
								vec *= ratio;
								pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
								if (pd.pnt.z()  * UNIT_TO_M <= DRAFT)
								{
									if (GetNormal(geo, pd))
										section_point_data.push_back(pd);
								}
								current_x -= point_distance;
							}
						}
					}
				}
			}
		}
		break;
		case 1:
		{
			sort(section_line.begin(), section_line.end(), sort_curves_y);
			if (check_point_distance)
			{
				//	sections
				float remain_length = 0;
				for (int j = 0; j < section_line.size(); j++)
				{
					current_count++;
					m_iStatus = prev_count + (current_count / total_count) * 100;
					UpdateProgress();
					if (section_line[j].size() > 0)
					{
						osg::Drawable* geo = loop_geo[section_line[j][0].y()];

						//fprintf(FileLog, "lines %d\n", j);
						for (int i = 1; i < section_line[j].size(); i++)
						{
							osg::Vec3 dir = section_line[j][i] - section_line[j][i - 1];
							float current_length = dir.length();
							float step_length = 0;
							dir.normalize();
							//fprintf(FileLog, "[%d / %d] cl : %lf, sl : %lf, rl : %lf\n", i, section_line[j].size(), current_length, step_length, remain_length);
							while (current_length - step_length > remain_length)
							{
								PointData pd;
								step_length += remain_length;
								//fprintf(FileLog, "add > \tcl : %lf, \tsl : %lf, \trl : %lf ", current_length, step_length, remain_length);
								pd.pnt = section_line[j][i - 1] + (dir * step_length);
								if (pd.pnt.z() * UNIT_TO_M <= DRAFT)
								{
									if (GetNormal(geo, pd))
										section_point_data.push_back(pd);
								}
								remain_length = point_distance;
							}

							remain_length -= (current_length - step_length);
						}
					}
				}
			}
			else
			{
				for (int j = 0; j < section_line.size(); j++)
				{
					current_count++;
					m_iStatus = prev_count + (current_count / total_count) * 100;
					UpdateProgress();
					if (section_line[j].size() > 0)
					{
						osg::Drawable* geo = loop_geo[section_line[j][0].y()];
						int temp_count = section_line[j][0].y() / point_distance;
						float current_y = (float)(temp_count + 1) * point_distance;
						//float current_y = floor(section_line[j][0].y() / 100.0f) * 100.0f + point_distance;
						while (current_y > section_line[j][0].y())
						{
							current_y -= point_distance;
						}

						for (int i = 1; i < section_line[j].size(); i++)
						{
							while (section_line[j][i].y() <= current_y)
							{
								PointData pd;
								float ratio = (current_y - section_line[j][i - 1].y()) / (section_line[j][i].y() - section_line[j][i - 1].y());
								osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
								vec *= ratio;
								pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
								if (pd.pnt.z() * UNIT_TO_M <= DRAFT)
								{
									if (GetNormal(geo, pd))
										section_point_data.push_back(pd);
								}
								current_y -= point_distance;
							}
						}
					}
				}
			}
		}
		break;
		case 2:
		{
			sort(section_line.begin(), section_line.end(), sort_curves_z);
			if (check_point_distance)
			{
				for (int j = 0; j < section_line.size(); j++)
				{
					current_count++;
					m_iStatus = prev_count + (current_count / total_count) * 100;
					UpdateProgress();
					if (section_line[j].size() > 0)
					{
						osg::Drawable* geo = loop_geo[section_line[j][0].z()];

						float remain_length = 0;
						for (int i = 1; i < section_line[j].size(); i++)
						{
							osg::Vec3 dir = section_line[j][i] - section_line[j][i - 1];
							float current_length = dir.length();
							float step_length = remain_length;
							dir.normalize();
							while (current_length - step_length < remain_length)
							{
								PointData pd;
								step_length += remain_length;
								pd.pnt = section_line[j][i - 1] + (dir * step_length);
								if (pd.pnt.z() * UNIT_TO_M <= DRAFT)
								{
									if (GetNormal(geo, pd))
										section_point_data.push_back(pd);
								}
								remain_length = point_distance;
							}

							remain_length = point_distance - (current_length - step_length);
						}
					}
				}
			}
			else
			{
				for (int j = 0; j < section_line.size(); j++)
				{
					current_count++;
					m_iStatus = prev_count + (current_count / total_count) * 100;
					UpdateProgress();
					if (section_line[j].size() > 0)
					{
						osg::Drawable* geo = loop_geo[section_line[j][0].z()];
						float current_z = /*floor*/(section_line[j][0].z() * UNIT_TO_M) * M_TO_UNIT - point_distance;
						while (current_z < section_line[j][0].z())
						{
							current_z += point_distance;
						}

						for (int i = 1; i < section_line[j].size(); i++)
						{
							while (section_line[j][i].z() >= current_z)
							{
								PointData pd;
								float ratio = (current_z - section_line[j][i - 1].z()) / (section_line[j][i].z() - section_line[j][i - 1].z());
								osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
								vec *= ratio;
								pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
								if (pd.pnt.z() * UNIT_TO_M <= DRAFT)
								{
									if (GetNormal(geo, pd))
										section_point_data.push_back(pd);
								}
								current_z += point_distance;
							}
						}
					}
				}
			}
		}
		break;
		}
	}
	else
	{
		osg::notify(osg::NOTICE) << "No intersections found." << std::endl;
	}
	m_iStatus = prev_count + 100;
}

void CIRES2View::AddSectionDataGeo(vector< PointData >& pt_list, osg::Group* group)
{
	osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
	osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
	for each(auto pd in pt_list)
	{
		v_array->push_back(pd.pnt);
		v_array->push_back(pd.pnt + (pd.normal * 0.5f));
	}
	base_palne_geo->setVertexArray(v_array);
	osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
	cross_color->push_back(osg::Vec4(0, 0, 1, 1));
	base_palne_geo->setColorArray(cross_color.get());
	base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
	osg::DrawArrays* drawArrayPoints = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, v_array->size()); // Is your line strip 
	base_palne_geo->addPrimitiveSet(drawArrayPoints);
	osg::Point* point = new osg::Point;
	point->setSize(5);
	base_palne_geo->getOrCreateStateSet()->setAttribute(point);
	osg::LineWidth* lineWidth = new osg::LineWidth(2);
	base_palne_geo->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
	base_palne_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	base_plane->addDrawable(base_palne_geo);

	group->addChild(base_plane);
}

void CIRES2View::AddSectionGeo(vector< vector< osg::Vec3 > >& pt_list, osg::Group* group)
{
	osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
	for (int i = 0; i < pt_list.size(); i++)
	{
		osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
		for each(auto pd in pt_list[i])
		{
			v_array->push_back(pd);
		}
		base_palne_geo->setVertexArray(v_array);
		osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
		cross_color->push_back(osg::Vec4(1, 0, 0, 1));
		base_palne_geo->setColorArray(cross_color.get());
		base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
		osg::DrawArrays* drawArrayPoints = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, v_array->size()); // Is your line strip 
		base_palne_geo->addPrimitiveSet(drawArrayPoints);
		osg::LineWidth* lineWidth = new osg::LineWidth(2);
		base_palne_geo->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
		base_palne_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		base_plane->addDrawable(base_palne_geo);
	}

	group->addChild(base_plane);
}

void CIRES2View::ClearWaterLine()
{
	m_aWaterLine.clear();
	m_aWaterLinePointData.clear();
	PreFrameUpdateData pf(osgWaterlineSection, NULL);
	m_QRemoveChild.push(pf);
}

void CIRES2View::ClearCrossSectionLine()
{
	m_aSectionLine.clear();
	m_aSectionPointDataList.clear();
	for each(auto section in osgSectionsData)
	{
		PreFrameUpdateData pf(section, NULL);
		m_QRemoveChild.push(pf);
	}
}

//void CIRES2View::ClearSectionPoints()
//{
//	m_aWaterLine.clear();
//	m_aWaterLinePointData.clear();
//
//	m_aSectionLine.clear();
//	m_aSectionPointDataList.clear();
//
//	PreFrameUpdateData pf(osgWaterlineSection, NULL);
//	m_QRemoveChild.push(pf);
//
//	for each(auto section in osgSectionsData)
//	{
//		PreFrameUpdateData pf(section, NULL);
//		m_QRemoveChild.push(pf);
//	}
//}

void CIRES2View::SaveIceInput()
{
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "wt");
	if (fp_4)
	{
		SIGMAK = SIGMAP;
		SSIGMA = 0.0f;
		fprintf_s(fp_4, "        %d\n", HULL_TYPE);
		fprintf_s(fp_4, "%*.6lf\n", 16, FG);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, SIGMAP, 16, SIGMAK, 16, SSIGMA);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n\n", 16, HH, 16, HK, 16, SH);
		fprintf_s(fp_4, "%*.6lf%*.6lf\n", 16, DRAFT, 16, BREADTH);
		fprintf_s(fp_4, "%*.6lf%*.6lf%*.6lf\n", 16, VS, 16, VE, 16, VI);
		fclose(fp_4);
	}
}

void CIRES2View::LoadIceInput()
{
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "rt");
	if (fp_4)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_4;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			HULL_TYPE = atoi(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			FG = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			SIGMAP = atof(ifp.m_array_strOutput[0]);
			SIGMAK = atof(ifp.m_array_strOutput[1]);
			SSIGMA = atof(ifp.m_array_strOutput[2]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			HH = atof(ifp.m_array_strOutput[0]);
			HK = atof(ifp.m_array_strOutput[1]);
			SH = atof(ifp.m_array_strOutput[2]);
		}
		ifp.ReadOneLineFromFile();
		if (ifp.ReadOneLineFromFile() > 1)
		{
			DRAFT = atof(ifp.m_array_strOutput[0]);
			BREADTH = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			VS = atof(ifp.m_array_strOutput[0]);
			VE = atof(ifp.m_array_strOutput[1]);
			VI = atof(ifp.m_array_strOutput[2]);
		}
	}
}

void CIRES2View::OnButtonCalculateSectionPoints()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_iCurrentStep < 2)
	{
		AfxMessageBox("Define Sections first.");
		return;
	}

	float points_gap = 500.0f;
	float points_gap_waterline = 500.0f;
	//if (m_bUseDistanceForAxis)
	//{
	//	if (m_pEditPointsDistance)
	//	{
	//		CString temp_string;
	//		temp_string = m_pEditPointsDistance->GetEditText();
	//		points_gap = atof(temp_string) * M_TO_UNIT;
	//	}
	//}
	//else
	//{
	//	if (m_pEditPointsNumber)
	//	{
	//		CString temp_string;
	//		temp_string = m_pEditPointsNumber->GetEditText();
	//		points_gap = atof(temp_string) * M_TO_UNIT;
	//	}
	//}

	//if (m_bUseDistanceForAxisWaterline)
	//{
	//	if (m_pEditPointsDistanceWaterline)
	//	{
	//		CString temp_string;
	//		temp_string = m_pEditPointsDistanceWaterline->GetEditText();
	//		points_gap_waterline = atof(temp_string) * M_TO_UNIT;
	//	}
	//}
	//else
	//{
	//	if (m_pEditPointsNumberWaterline)
	//	{
	//		CString temp_string;
	//		temp_string = m_pEditPointsNumberWaterline->GetEditText();
	//		points_gap_waterline = atof(temp_string) * M_TO_UNIT;
	//	}
	//}

	//if (m_pEditStartWaterline)
	//{
	//	CString temp_string;
	//	temp_string = m_pEditStartWaterline->GetEditText();
	//	m_fWaterlineStartPos = atof(temp_string) * M_TO_UNIT;
	//}
	//if (m_pEditEndWaterline)
	//{
	//	CString temp_string;
	//	temp_string = m_pEditEndWaterline->GetEditText();
	//	m_fWaterlineEndPos = atof(temp_string) * M_TO_UNIT;
	//}

	//if (m_pEditSpaceWaterline)
	//{
	//	CString temp_string;
	//	temp_string = m_pEditSpaceWaterline->GetEditText();
	//	m_fDraftValue = atof(temp_string);
	//}

	//if (m_pEditPointsGap)
	//{
	//	CString temp_string;
	//	temp_string = m_pEditPointsGap->GetEditText();
	//	points_gap = atof(temp_string) * M_TO_UNIT;
	//}

	if (osgHull->getNumChildren() < 1)
	{
		AfxMessageBox("Import HULL data first.");
		CalculateOutputResult(current_calculate_type, false);
		return;
	}
	else if (osgSectionPosList.size() < 1)
	{
		AfxMessageBox("Define Sections first.");
		CalculateOutputResult(current_calculate_type, false);
		return;
	}

	LoadIceInput();

	//ClearSectionPoints();

	m_aWaterLine.clear();
	m_aSectionLine.clear();

	//fopen_s(&FileLog, "d:\log.txt", "wt");
	m_iTotal = (osgSectionPosList.size() + 1) * 100;
	m_iStatus = 0;
	m_strStatus.Format("Calculate Section Points : Waterline");
	BeginProgress();

	osg::Matrix m;
	//osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	osg::Vec3 water_line_pos(bbHull.center().x(), bbHull.center().y(), DRAFT * M_TO_UNIT);
	m.setTrans(water_line_pos);
	//m.setRotate(q);
	osg::Vec3 n(0, 0, 1);
	//n = m.preMult(n) - m_iWaterLinePos;

	CalculateSectionWaterline(n, water_line_pos, 0, m_aWaterLinePointData, m_bUseDistanceForAxisWaterline, points_gap_waterline, m_aWaterLine, true, m_fWaterlineStartPos, m_fWaterlineEndPos);
	AddSectionDataGeo(m_aWaterLinePointData, osgWaterlineSection);
	AddSectionGeo(m_aWaterLine, osgWaterlineSection);

	osg::Vec3 normal;
	osg::Matrix m1;

	if (m_bBowBreaking)
	{
		osg::Quat q1(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
		m1.setRotate(q1);
		osg::Vec3 n(1, 0, 0);
		normal = m1.preMult(n);
	}
	else
	{
		osg::Quat q1(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
		m1.setRotate(q1);
		osg::Vec3 n(0, 1, 0);
		normal = m1.preMult(n);
	}

	for (int i = 0; i < osgSectionPosList.size(); i++)
	{
		m_strStatus.Format("Calculate Section Points : Section [%d / %d]", i+1, osgSectionPosList.size());
		if (osgSectionEnable[i])
		{
			vector< PointData > section_point_data;
			vector< vector< osg::Vec3 > > section_line;
			//float m_iSectionRot[3];
			CalculateSectionWaterline(normal, osgSectionPosList[i], 1, section_point_data, m_bUseDistanceForAxis, points_gap, section_line);
			if (section_point_data.size() > 0)
			{
				m_aSectionLine.push_back(section_line);
				m_aSectionPointDataList.push_back(section_point_data);
				AddSectionDataGeo(section_point_data, osgSectionsData[i]);
				AddSectionGeo(section_line, osgSectionsData[i]);
			}
		}
		else
		{
			m_iStatus += 100;
		}
	}
	//fclose(FileLog);
	EndProgress();
	//SetCurrentStep(3);
}

//void CIRES2View::CheckLand(int c_x, int c_y)
//{
//	TCHAR buffer[512];
//
//	int temp_status = 0;
//	int temp_index = 0;
//	for (int j = c_y; j >= 0; j--)
//	{
//		_stprintf(buffer, "CheckLand : %d - %d\n", c_x, j);
//		OutputDebugString(buffer);
//		if (m_fExSpeed[c_x][j] < 1)
//		{
//			_stprintf(buffer, "해안선으로 인식\n");
//			OutputDebugString(buffer);
//			realMap[c_x][j] = 0;
//			//if (temp_status == 0)	//	이전 위치가 바다나 얼음임
//			//{
//				temp_status = 1;	//	지금부터는 땅
//				temp_index = j;
//			//}
//			//else if (temp_status == 1)	//	이전 위치가 비어 있어서 땅이라고 인식함
//			//{
//			//	temp_status = 0;	//	지금부터는 바다
//			//}
//		}
//		else
//		{
//			_stprintf(buffer, "realMap : %d\n", realMap[c_x][j]);
//			OutputDebugString(buffer);
//			switch (realMap[c_x][j])
//			{
//			case 0:	//	땅
//			{
//
//			}
//			break;
//			case 1:	//	얼음
//			{
//				if (temp_status == 1)	//	땅에 붙어 있는 바다는 땅인줄 알았는데, 얼음이 있음. 그러면 이전 위치도 바다로 바꿈
//				{
//					_stprintf(buffer, "땅에 붙어 있는 바다는 땅인줄 알았는데, 얼음이 있음. 그러면 이전 위치도 바다로 바꿈\n");
//					OutputDebugString(buffer);
//					for (int jj = j + 1; jj < temp_index; jj++)
//					{
//						realMap[c_x][jj] = 2;
//					}
//					temp_status = 0;
//				}
//			}
//			break;
//			case 2:	//	바다
//			{
//				if (temp_status == 0)	//	땅에 붙어 있는 바다
//				{
//				}
//				else if (temp_status == 1)	//	땅에 붙어 있는 바다는 땅으로 인식
//				{
//					_stprintf(buffer, "땅에 붙어 있는 바다는 땅으로 인식\n");
//					OutputDebugString(buffer);
//					realMap[c_x][j] = 0;
//				}
//			}
//			break;
//			}
//			//realMap[i][j] = 1;
//		}
//	}
//	temp_status = 0;
//	for (int j = c_y; j < maxMapSizeCol; j++)
//	{
//		_stprintf(buffer, "CheckLand : %d - %d\n", c_x, j);
//		OutputDebugString(buffer);
//		if (m_fExSpeed[c_x][j] < 1)
//		{
//			_stprintf(buffer, "해안선으로 인식\n");
//			OutputDebugString(buffer);
//			realMap[c_x][j] = 0;
//			//if (temp_status == 0)	//	이전 위치가 바다나 얼음임
//			//{
//				temp_status = 1;	//	지금부터는 땅
//				temp_index = j;
//			//}
//			//else if (temp_status == 1)	//	이전 위치가 비어 있어서 땅이라고 인식함
//			//{
//			//	temp_status = 0;	//	지금부터는 바다
//			//}
//		}
//		else
//		{
//			_stprintf(buffer, "realMap : %d\n", realMap[c_x][j]);
//			OutputDebugString(buffer);
//			switch (realMap[c_x][j])
//			{
//			case 0:	//	땅
//			{
//
//			}
//			break;
//			case 1:	//	얼음
//			{
//				if (temp_status == 1)	//	땅에 붙어 있는 바다는 땅인줄 알았는데, 얼음이 있음. 그러면 이전 위치도 바다로 바꿈
//				{
//					_stprintf(buffer, "땅에 붙어 있는 바다는 땅인줄 알았는데, 얼음이 있음. 그러면 이전 위치도 바다로 바꿈\n");
//					OutputDebugString(buffer);
//					for (int jj = j - 1; jj > temp_index; jj--)
//					{
//						realMap[c_x][jj] = 2;
//					}
//					temp_status = 0;
//				}
//			}
//			break;
//			case 2:	//	바다
//			{
//				if (temp_status == 0)	//	땅에 붙어 있는 바다
//				{
//				}
//				else if (temp_status == 1)	//	땅에 붙어 있는 바다는 땅으로 인식
//				{
//					_stprintf(buffer, "땅에 붙어 있는 바다는 땅으로 인식\n");
//					OutputDebugString(buffer);
//					realMap[c_x][j] = 0;
//				}
//			}
//			break;
//			}
//			//realMap[i][j] = 1;
//		}
//	}
//}

void CIRES2View::CalculateOutputResult(int type, bool refresh)
{
	DeleteTempFiles(m_strProjectPath);

	current_calculate_type = type;
	if (refresh)
	{
		//	결과 파일 저장
		FILE* save_file;
		if (m_aWaterLinePointData.size() > 0)
		{
			fopen_s(&save_file, m_strProjectPath + "\\WATERLINE_OUTSIDE.inp", "wt");
			if (save_file)
			{
				fprintf_s(save_file, "%d\n", m_aWaterLinePointData.size());
				for (int i = 0; i < m_aWaterLinePointData.size(); i++)
				{
					fprintf_s(save_file, "   %.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n",
						m_aWaterLinePointData[i].pnt.x() * UNIT_TO_M,
						m_aWaterLinePointData[i].pnt.y() * UNIT_TO_M,
						m_aWaterLinePointData[i].pnt.z() * UNIT_TO_M,
						m_aWaterLinePointData[i].normal.x(),
						m_aWaterLinePointData[i].normal.y(),
						m_aWaterLinePointData[i].normal.z(),
						m_aWaterLinePointData[i].angle_alpha,
						m_aWaterLinePointData[i].angle_beta,
						m_aWaterLinePointData[i].angle_gamma);
					if (i == 0)
					{
						max_y = m_aWaterLinePointData[i].pnt.y()  * UNIT_TO_M;
					}
					else
					{
						float current_y = m_aWaterLinePointData[i].pnt.y() * UNIT_TO_M;
						if (current_y > max_y)
						{
							max_y = current_y;
						}
					}
				}
				fprintf_s(save_file, "%lf\n", max_y);
				fclose(save_file);
			}
		}

		if (m_aSectionPointDataList.size() > 0)
		{
			//	빈 데이터 삭제
			int i = m_aSectionPointDataList.size();
			for (int j = i - 1; j > -1; j--)
			{
				if (m_aSectionPointDataList[j].size() < 1)
				{
					m_aSectionPointDataList.erase(m_aSectionPointDataList.begin() + j);
				}
			}

			fopen_s(&save_file, m_strProjectPath + "\\FRAME.inp", "wt");
			if (save_file)
			{
				fprintf_s(save_file, "%d\n", m_aSectionPointDataList.size());
				for (int i = 0; i < m_aSectionPointDataList.size(); i++)
				{
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   %.3lf   %d\n", m_aSectionPointDataList[i][0].pnt.x() * UNIT_TO_M, m_aSectionPointDataList[i].size());
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %.3lf", abs(m_aSectionPointDataList[i][j].pnt.y()) * UNIT_TO_M);
					}
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %.3lf", m_aSectionPointDataList[i][j].pnt.z() * UNIT_TO_M);
					}
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %.3lf", m_aSectionPointDataList[i][j].angle_beta);
					}
					fprintf_s(save_file, "\n");
				}
				fclose(save_file);
			}
		}
	}

	//---------------------------------------------------------------------------------------
	//	3D_R.f90 파일 계산 로직 추가
	//---------------------------------------------------------------------------------------
	fopen_s(&fp_4, m_strProjectPath + "\\ICE_INPUT.inp", "rt");
	fopen_s(&fp_6, m_strProjectPath + "\\ECHO.OUT", "wt");
	fopen_s(&fp_7, m_strProjectPath + "\\ice_result.OUT", "wt");
	fopen_s(&fp_8, m_strProjectPath + "\\IMSI.OUT", "wt");
	fopen_s(&fp_9, m_strProjectPath + "\\ICECOFF_INPUT.inp", "rt");
	fopen_s(&fp_10, m_strProjectPath + "\\SELECT MODULE.INP", "rt");

	READ_ICE_INPUT();
	READ_ICECOFF_INPUT();
	switch (current_calculate_type)
	{
	case 0:
	{
		CAL_COND();
		READ_HULL(1);
		SEL_MODE1();

		for (int IH = 1; IH <= NH; IH++)
		{
			R_BOUYA[IH] = R_BO[IH];
			for (int IVS = 1; IVS <= NV; IVS++)
			{
				R_CLEAR[IH][IVS] = R_CL[IH][IVS];
			}
			for (int IS = 1; IS <= NSIGMA; IS++)
			{
				R_BREAK[IH][IS] = R_BR[IH][IS];
			}
		}

		//R_CLEAR.assign(R_CL.begin(), R_CL.end());
		//R_BREAK.assign(R_BR.begin(), R_BR.end());
		//R_BOUYA.assign(R_BO.begin(), R_BO.end());

		//if (HULL_TYPE == 2)
		//{
		//	R_CL.clear();
		//	R_BR.clear();
		//	READ_HULL(2);
		//	//SEL_MODE2();
		//	for (int i = 0; i < R_CLEAR.size(); i++)
		//	{
		//		for (int j = 0; j < R_CLEAR[i].size(); j++)
		//		{
		//			R_CLEAR[i][j] = R_CLEAR[i][j] + R_CL[i][j];
		//		}
		//	}
		//	for (int i = 0; i < R_BREAK.size(); i++)
		//	{
		//		for (int j = 0; j < R_BREAK[i].size(); j++)
		//		{
		//			R_BREAK[i][j] = R_BREAK[i][j] + R_BR[i][j];
		//		}
		//	}
		//}

		//if (HULL_TYPE == 3)
		//{
		//	R_CL.clear();
		//	R_BR.clear();
		//	READ_HULL(3);
		//	//SEL_MODE2();
		//	for (int i = 0; i < R_CLEAR.size(); i++)
		//	{
		//		for (int j = 0; j < R_CLEAR[i].size(); j++)
		//		{
		//			R_CLEAR[i][j] = R_CLEAR[i][j] + R_CL[i][j];
		//		}
		//	}
		//	for (int i = 0; i < R_BREAK.size(); i++)
		//	{
		//		for (int j = 0; j < R_BREAK[i].size(); j++)
		//		{
		//			R_BREAK[i][j] = R_BREAK[i][j] + R_BR[i][j];
		//		}
		//	}
		//}

		CALC_ATTAINABLE_SPEED();

		WRITE_OUT();

		if (current_calculate_type == 1)
		{
			CALC_SATELLITE(0, 0, 0);
		}
	}
	break;
	case 1:
	{
		int count = m_fConcentration.size();
		for (int i = 0; i < 10; i++)
		{
			SATELLITE_DATA[i].clear();
		}

		READ_HULL(1);

		for (int i = 0; i < count; i++)
		{
			HH = m_fIceThickness[i];
			HK = m_fIceThickness[i];
			SIGMAP = m_fFlexuralStrength[i];
			SIGMAK = m_fFlexuralStrength[i];
			VS = m_fShipSpeed[i];
			if (VS <= 0.0f)
				VS = 1.0f;
			VE = VS;

			CAL_COND(false);
			SEL_MODE1();

			for (int IH = 1; IH <= NH; IH++)
			{
				R_BOUYA[IH] = R_BO[IH];
				for (int IVS = 1; IVS <= NV; IVS++)
				{
					R_CLEAR[IH][IVS] = R_CL[IH][IVS];
				}
				for (int IS = 1; IS <= NSIGMA; IS++)
				{
					R_BREAK[IH][IS] = R_BR[IH][IS];
				}
			}

			//WRITE_OUT();
			CALC_SATELLITE(m_fConcentration[i], m_fFlexuralStrength[i], HH, false);
		}

		FILE* fp;
		fopen_s(&fp, m_strProjectPath + "\\satellite" + "_from_data" + ".out", "wt");
		if (fp)
		{
			float R_TOTAL;
			float pre_swan;
			float factor;
			float pack;
			fprintf_s(fp, "   Vs(kts)      Hi(m)     sigf(kPa)         R_br(kN)          R_cl(kN)          R_bu(kN)           R_i(kN)            pre-swan           factor             pack\n");
			for (int i = 0; i < SATELLITE_DATA[0].size(); i++)
			{
				fprintf_s(fp, "%9.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",
					SATELLITE_DATA[0][i], SATELLITE_DATA[1][i], SATELLITE_DATA[2][i],
					SATELLITE_DATA[3][i], SATELLITE_DATA[4][i], SATELLITE_DATA[5][i],
					SATELLITE_DATA[6][i],
					SATELLITE_DATA[7][i], SATELLITE_DATA[8][i], SATELLITE_DATA[9][i]);
			}

			fclose(fp);
		}


	}
	break;
	case 2:
	{
		if (m_fInitSpeed > 0.0f && m_fMaxSpeed > m_fInitSpeed && m_fIncreSpeed > 0)
		{
			VS = m_fInitSpeed;
			VE = m_fMaxSpeed;
			VI = m_fIncreSpeed;
		}
		else
		{
			VS = 1.0f;
			VE = 16.0f;
			VI = 1.0f;
		}

		READ_HULL(1);
		int count = m_fConcentration.size();
		for (int i = 0; i < count; i++)
		{
			HH = m_fIceThickness[i];
			HK = m_fIceThickness[i];
			SIGMAP = m_fFlexuralStrength[i];
			SIGMAK = m_fFlexuralStrength[i];

			CAL_COND(false);
			SEL_MODE1();

			for (int IH = 1; IH <= NH; IH++)
			{
				R_BOUYA[IH] = R_BO[IH];
				for (int IVS = 1; IVS <= NV; IVS++)
				{
					R_CLEAR[IH][IVS] = R_CL[IH][IVS];
				}
				for (int IS = 1; IS <= NSIGMA; IS++)
				{
					R_BREAK[IH][IS] = R_BR[IH][IS];
				}
			}

			//WRITE_OUT();
			CALC_SATELLITE(m_fConcentration[i], m_fFlexuralStrength[i], HH);
		}

		m_fShipSpeed.resize(count, 0);
		FILE* fp;
		fopen_s(&fp, m_strProjectPath + "\\satellite_from_estimation" + ".out", "wt");
		if (fp)
		{
			float R_TOTAL;
			float pre_swan;
			float factor;
			float pack;
			fprintf_s(fp, "   Vs(kts)      Hi(m)     sigf(kPa)         R_br(kN)          R_cl(kN)          R_bu(kN)           R_i(kN)            pre-swan           factor             pack\n");
			for (int i = 0; i < ESTIMATION_DATA[0].size(); i++)
			{
				float es = GetEstimatonSpeed(m_fTargetResistance, ESTIMATION_DATA[0][i], ESTIMATION_DATA[9][i]);
				m_fShipSpeed[i] = es;
				for (int j = 0; j < ESTIMATION_DATA[0][i].size(); j++)
				{
					fprintf_s(fp, "%9.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",
						ESTIMATION_DATA[0][i][j], ESTIMATION_DATA[1][i][j], ESTIMATION_DATA[2][i][j],
						ESTIMATION_DATA[3][i][j], ESTIMATION_DATA[4][i][j], ESTIMATION_DATA[5][i][j],
						ESTIMATION_DATA[6][i][j],
						ESTIMATION_DATA[7][i][j], ESTIMATION_DATA[8][i][j], ESTIMATION_DATA[9][i][j]);
				}
			}

			fclose(fp);
		}

		//fopen_s(&fp, "d:\\satellite_to_estimation.in", "rt");
		//if (fp)
		//{
		//	COptImportExportBase ifp;
		//	ifp.m_fp_input = fp;
		//	int index = 0;
		//	ifp.ReadOneLineFromFile();
		//	while (ifp.ReadOneLineFromFile() > 0)
		//	{
		//		if(index < m_fShipSpeed.size())
		//			m_fShipSpeed[index] = atof(ifp.m_strOneLine);
		//		index++;
		//	}
		//}

		fopen_s(&fp, m_strProjectPath + "\\satellite_to_estimation" + ".out", "wt");
		if (fp)
		{
			float R_TOTAL;
			float pre_swan;
			float factor;
			float pack;
			fprintf_s(fp, "   Vs(kts)\n");
			for (int i = 0; i < m_fShipSpeed.size(); i++)
			{
					fprintf_s(fp, "%9.6lf\n",
						m_fShipSpeed[i]);
			}

			fclose(fp);
		}

		if (m_fLongitude.size() > 0)
		{
			for (int i = 0; i < m_fShipSpeed.size(); i++)
			{
				m_fExSpeed[m_fX[i]][m_fY[i]] = m_fShipSpeed[i];

				//CString temp_string;
				//temp_string.Format("%d > %lf, %lf : %lf\n", i, m_fX[i], m_fY[i], m_fShipSpeed[i]);
				//OutputDebugString(temp_string);
			}

			SpeedDecision();

			realMap_origin.clear();
			realMap_origin.resize(realMap.size(), vector<int>(realMap[0].size(), 0));
			for (int i = 0; i < realMap.size(); i++)
			{
				for (int j = 0; j < realMap[i].size(); j++)
				{
					realMap_origin[i][j] = realMap[i][j];
				}
			}

			for (int i = 0; i < m_fedgeX.size(); i++)
			{
				int temp_x = (int)m_fedgeX[i];
				int temp_y = (int)m_fedgeY[i];
				if (temp_x >= 0 && temp_x < m_fExSpeed.size())
				{
					if (temp_y >= 0 && temp_y < m_fExSpeed[temp_x].size())
					{
						m_fExSpeed[temp_x][temp_y] = 0;
					}
				}
			}

			for (int i = 0; i < m_fedgeX.size(); i++)
			{
				int temp_x = (int)m_fedgeX[i];
				int temp_y = (int)m_fedgeY[i];
				if (temp_x >= 0 && temp_x < m_fExSpeed.size())
				{
					if (temp_y >= 0 && temp_y < m_fExSpeed[temp_x].size())
					{
						realMap[temp_x][temp_y] = 0;
					}
				}
			}
			CheckLand();

			if (m_bRunForInterface)
			{
			}

			int start_row = startRow;
			int start_col = startCol;
			int goal_row = goalRow;
			int goal_col = goalCol;
			int interface_loop = 1;
			if (m_bRunForInterface && m_aNodePointX.size() > 1)
			{
				interface_loop = m_aNodePointX.size() - 1;
				start_row = m_aNodePointX[0];
				start_col = m_aNodePointY[0];
				goal_row = m_aNodePointX[1];
				goal_col = m_aNodePointY[1];
			}

			vector< vector< int > > nodes_rows;
			vector< vector< int > > nodes_cols;

			for (int interface_index = 0; interface_index < interface_loop; interface_index++)
			{
				vector<vector<Node>> ASNode(m_fExSpeed.size(), vector<Node>(m_fExSpeed[0].size(), Node(0, 0, 0)));
				for (int i = 0; i < maxMapSizeRow; i++)
				{
					for (int j = 0; j < maxMapSizeCol; j++)
					{
						ASNode[i][j].changeValue(i, j, m_fExSpeed[i][j]);       //.emplace_back(i, j, ex_speed[i][j]);   뒤로 넣는 방식 말고 객체 이용
																			  // 행, 열의 위치와 속도 대입
					}
				}
				vector<vector<int>> PassList(maxMapSizeRow, vector<int>(maxMapSizeCol, 0));			// 이동 가능한 지점 모음
				vector<vector<int>> DecisionList(maxMapSizeRow, vector<int>(maxMapSizeCol, 0));		// 이동 가능한 지점 중 선택된 지점 모음

				for (int i = 0; i < maxMapSizeRow; i++)
				{
					for (int j = 0; j < maxMapSizeCol; j++)
					{
						if (i == start_row && j == start_col)
						{
							DecisionList[start_row][start_col] = 2;		// DecisionList 이동 가능한 지점에 1 설정
						}												// DecisionList 시작 지점에 2 설정
						else if (realMap[i][j] >= 1)
						{
							DecisionList[i][j] = 1;
						}
					}
				}

				for (int i = 0; i < maxMapSizeRow; i++)
				{
					for (int j = 0; j < maxMapSizeCol; j++)
					{
						if (i == start_row && j == start_col)
						{
							PassList[start_row][start_col] = 2;			// PassList 이동 가능한 지점에 1 설정
						}												// PassList 시작 지점에 2 설정
						//else if (realMap[i][j] >= 1)
						else if (realMap[i][j] >= 1 && m_fExSpeed[i][j] > 0)
						{
							PassList[i][j] = 1;
						}
					}
				}

				ASNode[start_row][start_col].h = 20 * (abs(goal_row - start_row) + abs(goal_col - start_col)) / (3 * 0.5144);
				ASNode[start_row][start_col].f = ASNode[start_row][start_col].g + ASNode[start_row][start_col].h;

				// 여기서부터 시작점

				// *it을 사용하면 어떤 값을 찾았는지 알 수 있음
				int num = 2;
				int itRowInd = 0;			// 출발 지점 행
				int itColInd = 0;			// 출발 지점 열
				vector< int > path_rows;
				vector< int > path_cols;

				while (itRowInd != goal_row || itColInd != goal_col)
				{
					for (int i = 0; i < maxMapSizeRow; i++)				// 시작점(DecisionList 2값 설정한 위치)의 행 열 찾기
					{
						auto it = find(DecisionList[i].begin(), DecisionList[i].end(), num);
						if (it != DecisionList[i].end())
						{
							itRowInd = i;
							itColInd = distance(DecisionList[i].begin(), it);
						}
					}

					path_rows.push_back(itRowInd);
					path_cols.push_back(itColInd);

					if (itRowInd == goal_row && itColInd == goal_col)		// 도착하면 끝내기
					{
						if (m_bRunForInterface)
						{
							//	경로 저장
							vector< NodePath* > find_nodes;
							for (int i = 0; i < path_rows.size(); i++)
							{
								find_nodes.push_back(new NodePath(path_rows[i], path_cols[i], DecisionList[path_rows[i]][path_cols[i]]));
							}

							sort(find_nodes.begin(), find_nodes.end(), [](const NodePath* lhs, const NodePath* rhs)
								{
									return lhs->dicision < rhs->dicision;
								});

							path_rows.clear();
							path_cols.clear();
							for (int i = 0; i < find_nodes.size(); i++)
							{
								if (find_nodes[i]->dicision > 0)
								{
									path_rows.push_back(find_nodes[i]->row);
									path_cols.push_back(find_nodes[i]->col);
								}
								delete find_nodes[i];
							}
							find_nodes.clear();

							nodes_rows.push_back(path_rows);
							nodes_cols.push_back(path_cols);
						}
						else
						{
							fopen_s(&fp, m_strProjectPath + "\\satellite_to_map.out", "wt");
							if (fp)
							{
								fprintf_s(fp, "Estimated arrive time [s] = %.6lf s\n", 2500.0 * ASNode[goal_row][goal_col].f);
								fprintf_s(fp, "Estimated arrive time [h] = %.6lf h\n", 2500.0 * ASNode[goal_row][goal_col].f / 3600.0);
								fprintf_s(fp, "Estimated arrive time [day] = %.6lf day\n", 2500.0 * ASNode[goal_row][goal_col].f / (3600.0 * 24.0));
								fprintf_s(fp, "Rows %d, Columns %d\n", maxMapSizeCol, maxMapSizeRow);
								for (int i = 0; i < maxMapSizeCol; i++)
								{
									for (int j = 0; j < maxMapSizeRow; j++)
									{
										if (DecisionList[j][i] > 1)
										{
											fprintf_s(fp, "*  ");	//	이동 경로 2
										}
										else
										{
											fprintf_s(fp, "%d  ", realMap_origin[j][i]);	//	갈 수 있는 곳 : 물, 얼음
											//if (DecisionList[j][i] == 1)
											//{
											//	fprintf_s(fp, "%d  ", realMap[j][i]);	//	갈 수 있는 곳 : 물, 얼음
											//}
											//else
											//{
											//	fprintf_s(fp, "%d  ", DecisionList[j][i]);	//	못가는 곳 0
											//}
										}
									}
									fprintf_s(fp, "\n");
								}

								for (int i = 1; i < path_rows.size(); i++)
								{
									fprintf_s(fp, "%d, %d -> %d, %d(%.6lf h)\n", path_rows[i - 1], path_cols[i - 1], path_rows[i], path_cols[i], 2500.0 * ASNode[path_rows[i]][path_cols[i]].f / 3600.0);
								}

								fclose(fp);
							}
						}
						break;
					}

					// PassList 값 설정			각 위치마다 존재하는지 확인 후, realMap에서 지나갈 수 있는 길일 때 PassList에 값 넣기	

					int PassNumber = 0;			// 이동 가능한 지점의 개수 파악 

					if (itRowInd - 1 >= 0 && itColInd - 1 >= 0)					// 북동
					{
						if (PassList[itRowInd - 1][itColInd - 1] == 1 || PassList[itRowInd - 1][itColInd - 1] == num + 1)
						{
							PassList[itRowInd - 1][itColInd - 1] = num + 1;
							PassNumber += 1;
						}
					}
					if (itRowInd - 1 >= 0)										// 북
					{
						if (PassList[itRowInd - 1][itColInd] == 1 || PassList[itRowInd - 1][itColInd] == num + 1)
						{
							PassList[itRowInd - 1][itColInd] = num + 1;
							PassNumber += 1;
						}
					}
					if (itRowInd - 1 >= 0 && itColInd + 1 < maxMapSizeCol)		// 북서
					{
						if (PassList[itRowInd - 1][itColInd + 1] == 1 || PassList[itRowInd - 1][itColInd + 1] == num + 1)
						{
							PassList[itRowInd - 1][itColInd + 1] = num + 1;
							PassNumber += 1;
						}
					}
					if (itColInd - 1 >= 0)										// 동
					{
						if (PassList[itRowInd][itColInd - 1] == 1 || PassList[itRowInd][itColInd - 1] == num + 1)
						{
							PassList[itRowInd][itColInd - 1] = num + 1;
							PassNumber += 1;
						}
					}
					if (itColInd + 1 < maxMapSizeCol)							// 서
					{
						if (PassList[itRowInd][itColInd + 1] == 1 || PassList[itRowInd][itColInd + 1] == num + 1)
						{
							PassList[itRowInd][itColInd + 1] = num + 1;
							PassNumber += 1;
						}
					}
					if (itRowInd + 1 < maxMapSizeRow && itColInd - 1 >= 0)		// 남동
					{
						if (PassList[itRowInd + 1][itColInd - 1] == 1 || PassList[itRowInd + 1][itColInd - 1] == num + 1)
						{
							PassList[itRowInd + 1][itColInd - 1] = num + 1;
							PassNumber += 1;
						}
					}
					if (itRowInd + 1 < maxMapSizeRow)							// 남
					{
						if (PassList[itRowInd + 1][itColInd] == 1 || PassList[itRowInd + 1][itColInd] == num + 1)
						{
							PassList[itRowInd + 1][itColInd] = num + 1;
							PassNumber += 1;
						}
					}
					if (itRowInd + 1 < maxMapSizeRow && itColInd + 1 < maxMapSizeCol)		// 남서
					{
						if (PassList[itRowInd + 1][itColInd + 1] == 1 || PassList[itRowInd + 1][itColInd + 1] == num + 1)
						{
							PassList[itRowInd + 1][itColInd + 1] = num + 1;
							PassNumber += 1;
						}
					}

					// 길이 없을 때
					if (PassNumber == 0)
					{
						PassList[itRowInd][itColInd] = 0;
						DecisionList[itRowInd][itColInd] = 0;
						num -= 1;
						continue;
					}


					int RowCoord = 0;		// 변수들의 행 좌표 (출발점 행열 아님 주의)
					int ColCoord = 0;		// 변수들의 열 좌표 (출발점 행열 아님 주의)

					int minRowCoord = 0;	// f 최소인 지점의 행
					int minColCoord = 0;	// f 최소인 지점의 열
					double minfvalue = 0;   // f 최소인 지점의 f 값

					for (int i = 0; i < maxMapSizeRow; i++)
					{
						auto it2 = find(PassList[i].begin(), PassList[i].end(), num + 1);
						while (it2 != PassList[i].end())
						{
							RowCoord = i;
							ColCoord = distance(PassList[i].begin(), it2);

							// g h f 값 구하기
							// g 값
							if (abs(RowCoord - itRowInd) + abs(ColCoord - itColInd) == 1)
							{
								ASNode[RowCoord][ColCoord].g = (10 / (ASNode[itRowInd][itColInd].speed * 0.5144))
									+ (10 / (ASNode[RowCoord][ColCoord].speed * 0.5144)) + ASNode[itRowInd][itColInd].g;
							}
							else if (abs(RowCoord - itRowInd) + abs(ColCoord - itColInd) == 2)
							{
								ASNode[RowCoord][ColCoord].g = (10 * sqrt(2) / (ASNode[itRowInd][itColInd].speed * 0.5144))
									+ (10 * sqrt(2) / (ASNode[RowCoord][ColCoord].speed * 0.5144)) + ASNode[itRowInd][itColInd].g;
							}

							/*
							// h 값	유클리안
							ASNode[RowCoord][ColCoord].h = 20 * sqrt((goalRow - RowCoord) * (goalRow - RowCoord) + (goalRow - RowCoord) * (goalRow - RowCoord))
								/ (ASNode[itRowInd][itColInd].speed * 0.5144);
							//ASNode[RowCoord][ColCoord].h = 20 * (abs(goalRow - RowCoord) + abs(goalCol - ColCoord)) / (3 * 0.5144);
							*/

							// h 값 맨하튼

							ASNode[RowCoord][ColCoord].h = 20 * (abs(goal_row - RowCoord) + abs(goal_col - ColCoord)) / (ASNode[itRowInd][itColInd].speed * 0.5144);
							//ASNode[RowCoord][ColCoord].h = 20 * (abs(goalRow - RowCoord) + abs(goalCol - ColCoord)) / (3 * 0.5144);


							// f 값
							ASNode[RowCoord][ColCoord].f = ASNode[RowCoord][ColCoord].g + ASNode[RowCoord][ColCoord].h;

							// PassList 중에서 f가 최소인 지점 찾기
							if (minfvalue == 0)
							{
								minRowCoord = RowCoord;
								minColCoord = ColCoord;
								minfvalue = ASNode[RowCoord][ColCoord].f;
							}
							else if (ASNode[RowCoord][ColCoord].f < minfvalue)
							{
								minRowCoord = RowCoord;
								minColCoord = ColCoord;
								minfvalue = ASNode[RowCoord][ColCoord].f;
							}
							// f값이 동일할 때 뭐를 택할지 추가
							else if (ASNode[RowCoord][ColCoord].f = minfvalue &&
								((RowCoord - goal_row) * (RowCoord - goal_row) + (ColCoord - goal_col) * (ColCoord - goal_col)) <
								((minRowCoord - goal_row) * (minRowCoord - goal_row) + (minColCoord - goal_col) * (minColCoord - goal_col)))
							{
								minRowCoord = RowCoord;
								minColCoord = ColCoord;
								minfvalue = ASNode[RowCoord][ColCoord].f;
							}

							//cout << RowCoord << " " << ColCoord << endl;		//값 찾기
							it2 = find(it2 + 1, PassList[i].end(), num + 1);
						}
					}

					DecisionList[minRowCoord][minColCoord] = num + 1;		// DecisionList에 최소 f값 지점 설정


					///////////////////////////////////////////////////////////////////////////////////////////////
					// f, g, h 값 확인
					/*
					cout << "\n" << "f, g, h 값 확인" << "\n";
					for (int i = 0; i < maxMapSizeRow; i++)
					{
						for (int j = 0; j < maxMapSizeCol; j++)
						{
							cout << "| " << ASNode[i][j].f << ", (" << ASNode[i][j].row << ", " << ASNode[i][j].col << ") ";
						}
						cout << "|" << endl;
						for (int j = 0; j < maxMapSizeCol; j++)
						{
							cout << "| " << ASNode[i][j].g << ",  " << ASNode[i][j].h << " ";
						}
						cout << "|" << endl << endl;
					}
					*/

					/*
					// DecisionList check
					cout << "DecisionList 값 확인 \n";
					for (int i = 0; i < maxMapSizeRow; i++)
					{
						for (int j = 0; j < maxMapSizeCol; j++)
						{
							cout << DecisionList[i][j] << "  ";
						}
						cout << endl;
					}
					cout << endl;



					cout << "PassList 값 확인 \n";
					// PassList check
					for (int i = 0; i < maxMapSizeRow; i++)
					{
						for (int j = 0; j < maxMapSizeCol; j++)
						{
							cout << PassList[i][j] << "  ";
						}
						cout << endl;
					}
					*/
					//////////////////////////////////////////////////////////////////////////////////////

					num += 1;

				}

				if (m_bRunForInterface)
				{
					start_row = m_aNodePointX[interface_index + 1];
					start_col = m_aNodePointY[interface_index + 1];
					goal_row = m_aNodePointX[interface_index + 2];
					goal_col = m_aNodePointY[interface_index + 2];
				}
			}

			if (m_bRunForInterface)
			{
				//	이미지 저장
				osg::ref_ptr<osg::Image> posterImage = 0;
				posterImage = new osg::Image;
				posterImage->allocateImage(maxMapSizeRow * 10, maxMapSizeCol * 10, 1, GL_RGBA, GL_UNSIGNED_BYTE);
				osg::Vec4 color_path = osg::Vec4(1, 0, 0, 1);
				osg::Vec4 color_sea = osg::Vec4(109.0 / 255.0, 207.0 / 255.0, 246.0 / 255.0, 1);
				osg::Vec4 color_ice = osg::Vec4(1, 1, 1, 1);
				osg::Vec4 color_land = osg::Vec4(254.0 / 255, 238.0 / 255.0, 179.0 / 255.0, 1);
				osg::Vec4 color_node = osg::Vec4(0, 0, 0, 1);
				for (int i = 0; i < maxMapSizeCol; i++)
				{
					for (int j = 0; j < maxMapSizeRow; j++)
					{
						if (realMap_origin[j][i] == 1)
						{
							for (int k = 0; k < 10; k++)
							{
								for (int l = 0; l < 10; l++)
								{
									posterImage->setColor(color_ice, osg::Vec2((float)(j * 10 + k) / (float)(maxMapSizeRow * 10), (float)(i * 10 + l) / (float)(maxMapSizeCol * 10)));
								}
							}
						}
						else if (realMap_origin[j][i] == 0)
						{
							for (int k = 0; k < 10; k++)
							{
								for (int l = 0; l < 10; l++)
								{
									posterImage->setColor(color_sea, osg::Vec2((float)(j * 10 + k) / (float)(maxMapSizeRow * 10), (float)(i * 10 + l) / (float)(maxMapSizeCol * 10)));
								}
							}
						}
						else
						{
							for (int k = 0; k < 10; k++)
							{
								for (int l = 0; l < 10; l++)
								{
									posterImage->setColor(color_land, osg::Vec2((float)(j * 10 + k) / (float)(maxMapSizeRow * 10), (float)(i * 10 + l) / (float)(maxMapSizeCol * 10)));
								}
							}
						}
					}
				}

				osg::Vec2 node_shape[] = {
					osg::Vec2(4, 2), osg::Vec2(5, 2),
					osg::Vec2(4, 3), osg::Vec2(5, 3),
					osg::Vec2(4, 4), osg::Vec2(5, 4), osg::Vec2(2, 4), osg::Vec2(3, 4), osg::Vec2(6, 4), osg::Vec2(7, 4),
					osg::Vec2(4, 5), osg::Vec2(5, 5), osg::Vec2(2, 5), osg::Vec2(3, 5), osg::Vec2(6, 5), osg::Vec2(7, 5),
					osg::Vec2(4, 6), osg::Vec2(5, 6),
					osg::Vec2(4, 7), osg::Vec2(5, 7),
				};
				osg::Vec2 route_shape[] = {
					osg::Vec2(4, 8), osg::Vec2(5, 8),
					osg::Vec2(4, 7), osg::Vec2(5, 7),
					osg::Vec2(4, 6), osg::Vec2(5, 6), osg::Vec2(3, 6), osg::Vec2(6, 6),
					osg::Vec2(4, 5), osg::Vec2(5, 5), osg::Vec2(3, 5), osg::Vec2(6, 5),
					osg::Vec2(4, 4), osg::Vec2(5, 4), osg::Vec2(3, 4), osg::Vec2(6, 4), osg::Vec2(2, 4), osg::Vec2(7, 4),
					osg::Vec2(4, 3), osg::Vec2(5, 3), osg::Vec2(3, 3), osg::Vec2(6, 3), osg::Vec2(2, 3), osg::Vec2(7, 3),
					osg::Vec2(4, 2), osg::Vec2(5, 2), osg::Vec2(3, 2), osg::Vec2(6, 2), osg::Vec2(2, 2), osg::Vec2(7, 2), osg::Vec2(1, 2), osg::Vec2(8, 2),
					osg::Vec2(4, 1), osg::Vec2(5, 1), osg::Vec2(3, 1), osg::Vec2(6, 1), osg::Vec2(2, 1), osg::Vec2(7, 1), osg::Vec2(1, 1), osg::Vec2(8, 1),
				};

				for (int i = 0; i < nodes_rows.size(); i++)
				{
					for (int j = 0; j < nodes_rows[i].size(); j++)
					{
						if (j == 0 || j == nodes_rows[i].size() - 1)
						{
							//	start, end
							for (int k = 0; k < 40; k++)
							{
								posterImage->setColor(color_path, osg::Vec2((float)(nodes_rows[i][j] * 10 + route_shape[k].x()) / (float)(maxMapSizeRow * 10), (float)(nodes_cols[i][j] * 10 + route_shape[k].y()) / (float)(maxMapSizeCol * 10)));
							}
						}
						else
						{
							for (int k = 0; k < 20; k++)
							{
								posterImage->setColor(color_node, osg::Vec2((float)(nodes_rows[i][j] * 10 + node_shape[k].x()) / (float)(maxMapSizeRow * 10), (float)(nodes_cols[i][j] * 10 + node_shape[k].y()) / (float)(maxMapSizeCol * 10)));
							}
						}
					}
				}

				char save_file[512];
				sprintf_s(save_file, "%s\\output.bmp", HMS_OutputFolder);
				osgDB::writeImageFile(*posterImage, save_file);
				AfxGetApp()->WriteProfileString(_T("HMS"), _T("OUTPUT_BMP"), save_file);

				//	경로 저장
				sprintf_s(save_file, "%s\\path.txt", HMS_OutputFolder);
				FILE* fp_path;
				fopen_s(&fp_path, save_file, "wt");
				if (fp_path)
				{
					for (int i = 0; i < nodes_rows.size(); i++)
					{
						fprintf_s(fp_path, "%d\n", nodes_rows[i].size());
						for (int j = 0; j < nodes_rows[i].size(); j++)
						{
							fprintf_s(fp_path, "%d,%d\n", nodes_rows[i][j], nodes_cols[i][j]);
						}
					}
					fclose(fp_path);
					AfxGetApp()->WriteProfileString(_T("HMS"), _T("OUTPUT_NODES"), save_file);
				}
			}
		}
	}
	break;
	}

	if (fp_4)
	{
		fclose(fp_4);
	}
	if (fp_5)
	{
		fclose(fp_5);
	}
	if (fp_6)
	{
		fclose(fp_6);
	}
	if (fp_7)
	{
		fclose(fp_7);
	}
	if (fp_8)
	{
		fclose(fp_8);
	}
	if (fp_9)
	{
		fclose(fp_9);
	}
	if (fp_10)
	{
		fclose(fp_10);
	}
	if (fp_15)
	{
		fclose(fp_15);
	}

	//	입력한 파라미터 유지되도록 프로그램 폴더로 복사
	CopyFiles(m_strProjectPath, m_strAppPath);

	DeleteTempFiles(m_strAppPath);

	for (int i = 0; i < m_aAnalysisPGM.size(); i++)
	{
		CString command_string;
		command_string = m_strAppPath + "\\" + m_aAnalysisPGM[i] + " '" + m_aAnalysisInput[i] + "' '" + m_aAnalysisOutput[i];

		RunExecute(command_string);
	}
	//ShellExecute(NULL, "open", m_strAppPath + "\\ice_result.OUT", NULL, NULL, SW_SHOW);
}

void CIRES2View::SpeedDecision()
{
	maxMapSizeRow = m_fExSpeed.size();
	if (maxMapSizeRow > 0)
	{
		maxMapSizeCol = m_fExSpeed[0].size();

		for (int i = 0; i < maxMapSizeRow; i++)
		{
			for (int j = 0; j < maxMapSizeCol; j++)
			{
				if (m_fExSpeed[i][j] >= 0)
				{
					realMap[i][j] = 1;
					if (m_fExSpeed[i][j] < 1)
					{
						//	원래는 0이어서 못가는 곳 이었으나, 다 갈수 있는 곳으로
						//m_fExSpeed[i][j] = m_fMaxSpeed;
						realMap[i][j] = 0;
					}
					else
					{
						if (m_fExSpeed[i][j] > m_fMaxSpeed)
						{
							m_fExSpeed[i][j] = m_fMaxSpeed;
						}
					}
				}
				else
				{
					//CString temp_string;
					//temp_string.Format("%d, %d : %lf\n", i, j, m_fExSpeed[i][j]);
					//OutputDebugString(temp_string);
					realMap[i][j] = 0;
				}
			}
		}
	}
}

void CIRES2View::CALC_ATTAINABLE_SPEED()
{
	float R_TOTAL;
	float $B$2 = -1.45296f;
	float $C$2 = -0.115354f;
	float $D$2 = 0.0771342f;
	float $E$2 = 0.0324028f;
	float $F$2 = 0.17456f;
	float $G$2 = 0.349291f;
	float $H$2 = -0.131372f;
	float $I$2 = -0.00690513f;
	float $J$2 = 0.0159465f;
	float $K$2 = -0.0774989f;
	float $L$2 = 0.0310866f;
	float $M$2 = 0.000636468f;
	float $G$5 = 60.0f;
	float $I$5 = 80.0f;
	float $K$5 = 90.0f;
	float Attainable_Speed_60;
	float Attainable_Speed_80;
	float Attainable_Speed_90;
	ICE_S.clear();
	ICE_T.clear();
	A_SPEED[0].clear();
	A_SPEED[1].clear();
	A_SPEED[2].clear();

	for (int IS = 1; IS <= NSIGMA; IS++)
	{
		for (int IH = 1; IH <= NH; IH++)
		{
			float ice_strength = SIGMA[IS] * 0.001f;
			float ice_thickness = THCK[IH];
			bool is_end = false;
			for (int IV = 1; IV <= NV; IV++)
			{
				is_end = true;
				R_TOTAL = R_BREAK[IH][IS] + R_CLEAR[IH][IV] + R_BOUYA[IH];

				float sheep_speed = VSP[IV];
				float Preswan_resistance = R_TOTAL * 0.001f;

				float Attainable_Net_Thurst = (-0.007f * pow(sheep_speed, 4.0f) + -0.0198f * pow(sheep_speed, 3.0f) + 1.4903f * pow(sheep_speed, 2.0f) - 17.543f * sheep_speed + 989.57f);

				float factor_60 = ($B$2 + $C$2 * ice_thickness + $D$2 * pow(ice_thickness, 2.0f) + $E$2 * $G$5) +
					($F$2 + $G$2 * ice_thickness + $H$2 * pow(ice_thickness, 2.0f) + $I$2 * $G$5) * sheep_speed +
					($J$2 + $K$2 * ice_thickness + $L$2 * pow(ice_thickness, 2.0f) + $M$2 * $G$5) * pow(sheep_speed, 2.0f);
				float r_total_60 = factor_60 * Preswan_resistance;

				float factor_80 = ($B$2 + $C$2 * ice_thickness + $D$2 * pow(ice_thickness, 2.0f) + $E$2 * $I$5) +
					($F$2 + $G$2 * ice_thickness + $H$2 * pow(ice_thickness, 2.0f) + $I$2 * $I$5) * sheep_speed +
					($J$2 + $K$2 * ice_thickness + $L$2 * pow(ice_thickness, 2.0f) + $M$2 * $I$5) * pow(sheep_speed, 2.0f);
				float r_total_80 = factor_80 * Preswan_resistance;

				float factor_90 = ($B$2 + $C$2 * ice_thickness + $D$2 * pow(ice_thickness, 2.0f) + $E$2 * $K$5) +
					($F$2 + $G$2 * ice_thickness + $H$2 * pow(ice_thickness, 2.0f) + $I$2 * $K$5) * sheep_speed +
					($J$2 + $K$2 * ice_thickness + $L$2 * pow(ice_thickness, 2.0f) + $M$2 * $K$5) * pow(sheep_speed, 2.0f);
				float r_total_90 = factor_90 * Preswan_resistance;

				if (Attainable_Net_Thurst >= r_total_60)
				{
					Attainable_Speed_60 = sheep_speed;
					is_end = false;
				}

				if (Attainable_Net_Thurst >= r_total_80)
				{
					Attainable_Speed_80 = sheep_speed;
					is_end = false;
				}

				if (Attainable_Net_Thurst >= r_total_90)
				{
					Attainable_Speed_90 = sheep_speed;
					is_end = false;
				}

				if (is_end)
					break;
			}
			ICE_S.push_back(ice_strength);
			ICE_T.push_back(ice_thickness);
			A_SPEED[0].push_back(Attainable_Speed_60);
			A_SPEED[1].push_back(Attainable_Speed_80);
			A_SPEED[2].push_back(Attainable_Speed_90);
		}
	}

	FILE* fp;
	fopen_s(&fp, m_strProjectPath + "\\Attainable_speed.out", "wt");
	if (fp)
	{
		fprintf_s(fp, "IceStrength(kPa),IceThickness(m),AttainableSpeed(kts)_60,AttainableSpeed(kts)_80,AttainableSpeed(kts)_90\n");
		for (int i = 0; i < ICE_S.size(); i++)
		{
			fprintf_s(fp, "%10.3f,%10.3f,%10.3f,%10.3f,%10.3f\n", ICE_S[i], ICE_T[i], A_SPEED[0][i], A_SPEED[1][i], A_SPEED[2][i]);
		}

		fclose(fp);
	}

}
void CIRES2View::CALC_SATELLITE(float concentration, float flexural_strength, float ice_thickness, bool save_file)
{
	float R_TOTAL;
	float a0 = -1.4530f;
	float a1 = -0.1154;
	float a2 = 0.0771f;
	float a3 = 0.0324f;
	float b0 = 0.1746f;
	float b1 = 0.3493f;
	float b2 = -0.1314f;
	float b3 = -0.0069f;
	float c0 = 0.0159f;
	float c1 = -0.0775f;
	float c2 = 0.0311f;
	float c3 = 0.0006f;

	if (save_file)
	{
		//CString file_desc;
		//file_desc.Format("%lf_%lf_%lf", concentration, flexural_strength, ice_thickness);
		//FILE* fp;
		//fopen_s(&fp, m_strProjectPath + "\\satellite" + file_desc + ".out", "wt");
		//if (fp)
		//{
		//	float R_TOTAL;
		//	float pre_swan;
		//	float factor;
		//	float pack;
		//	fprintf_s(fp, "   Vs(kts)      Hi(m)     sigf(kPa)         R_br(kN)          R_cl(kN)          R_bu(kN)           R_i(kN)            pre-swan           factor             pack\n");
		//	for (int IV = 1; IV <= NV; IV++)
		//	{
		//		for (int IS = 1; IS <= NSIGMA; IS++)
		//		{
		//			for (int IH = 1; IH <= NH; IH++)
		//			{
		//				R_TOTAL = R_BREAK[IH][IS] + R_CLEAR[IH][IV] + R_BOUYA[IH];
		//				pre_swan = ((R_TOTAL * 0.001f) - (R_BREAK[IH][IS] * 0.001f)) * 2.0f;
		//				factor = (a0 + (a1 * THCK[IH]) + (a2 * THCK[IH] * THCK[IH]) + (a3 * concentration)) +
		//					((b0 + (b1 * THCK[IH]) + (b2 * THCK[IH] * THCK[IH]) + (b3 * concentration)) * VSP[IV]) +
		//					((c0 + (c1 * THCK[IH]) + (c2 * THCK[IH] * THCK[IH]) + (c3 * concentration)) * VSP[IV] * VSP[IV]);
		//				pack = pre_swan * factor;
		//				fprintf_s(fp, "%9.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",
		//					VSP[IV], THCK[IH], SIGMA[IS], R_BREAK[IH][IS] * 0.001f, R_CLEAR[IH][IV] * 0.001f, R_BOUYA[IH] * 0.001f, R_TOTAL * 0.001f,
		//					pre_swan, factor, pack);
		//			}
		//		}
		//	}

		//	fclose(fp);
		//}
		float R_TOTAL;
		float pre_swan;
		float factor;
		float pack;
		vector< float > value[10];
		for (int IV = 1; IV <= NV; IV++)
		{
			for (int IS = 1; IS <= NSIGMA; IS++)
			{
				for (int IH = 1; IH <= NH; IH++)
				{
					R_TOTAL = R_BREAK[IH][IS] + R_CLEAR[IH][IV] + R_BOUYA[IH];
					pre_swan = ((R_TOTAL * 0.001f) - (R_BREAK[IH][IS] * 0.001f)) * 2.0f;
					factor = (a0 + (a1 * THCK[IH]) + (a2 * THCK[IH] * THCK[IH]) + (a3 * concentration)) +
						((b0 + (b1 * THCK[IH]) + (b2 * THCK[IH] * THCK[IH]) + (b3 * concentration)) * VSP[IV]) +
						((c0 + (c1 * THCK[IH]) + (c2 * THCK[IH] * THCK[IH]) + (c3 * concentration)) * VSP[IV] * VSP[IV]);
					pack = pre_swan * factor;

					value[0].push_back(VSP[IV]);
					value[1].push_back(THCK[IH]);
					value[2].push_back(SIGMA[IS]);
					value[3].push_back(R_BREAK[IH][IS] * 0.001f);
					value[4].push_back(R_CLEAR[IH][IV] * 0.001f);
					value[5].push_back(R_BOUYA[IH] * 0.001f);
					value[6].push_back(R_TOTAL * 0.001f);
					value[7].push_back(pre_swan);
					value[8].push_back(factor);
					value[9].push_back(pack);
				}
			}
		}

		ESTIMATION_DATA[0].push_back(value[0]);
		ESTIMATION_DATA[1].push_back(value[1]);
		ESTIMATION_DATA[2].push_back(value[2]);
		ESTIMATION_DATA[3].push_back(value[3]);
		ESTIMATION_DATA[4].push_back(value[4]);
		ESTIMATION_DATA[5].push_back(value[5]);
		ESTIMATION_DATA[6].push_back(value[6]);
		ESTIMATION_DATA[7].push_back(value[7]);
		ESTIMATION_DATA[8].push_back(value[8]);
		ESTIMATION_DATA[9].push_back(value[9]);
	}
	else
	{
		float R_TOTAL;
		float pre_swan;
		float factor;
		float pack;
		for (int IV = 1; IV <= NV; IV++)
		{
			for (int IS = 1; IS <= NSIGMA; IS++)
			{
				for (int IH = 1; IH <= NH; IH++)
				{
					R_TOTAL = R_BREAK[IH][IS] + R_CLEAR[IH][IV] + R_BOUYA[IH];
					pre_swan = ((R_TOTAL * 0.001f) - (R_BREAK[IH][IS] * 0.001f)) * 2.0f;
					factor = (a0 + (a1 * THCK[IH]) + (a2 * THCK[IH] * THCK[IH]) + (a3 * concentration)) +
						((b0 + (b1 * THCK[IH]) + (b2 * THCK[IH] * THCK[IH]) + (b3 * concentration)) * VSP[IV]) +
						((c0 + (c1 * THCK[IH]) + (c2 * THCK[IH] * THCK[IH]) + (c3 * concentration)) * VSP[IV] * VSP[IV]);
					pack = pre_swan * factor;

					SATELLITE_DATA[0].push_back(VSP[IV]);
					SATELLITE_DATA[1].push_back(THCK[IH]);
					SATELLITE_DATA[2].push_back(SIGMA[IS]);
					SATELLITE_DATA[3].push_back(R_BREAK[IH][IS] * 0.001f);
					SATELLITE_DATA[4].push_back(R_CLEAR[IH][IV] * 0.001f);
					SATELLITE_DATA[5].push_back(R_BOUYA[IH] * 0.001f);
					SATELLITE_DATA[6].push_back(R_TOTAL * 0.001f);
					SATELLITE_DATA[7].push_back(pre_swan);
					SATELLITE_DATA[8].push_back(factor);
					SATELLITE_DATA[9].push_back(pack);
				}
			}
		}
	}
}

void CIRES2View::READ_ICE_INPUT()
{
	if (fp_4)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_4;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			HULL_TYPE = atoi(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			FG = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			SIGMAP = atof(ifp.m_array_strOutput[0]);
			SIGMAK = atof(ifp.m_array_strOutput[1]);
			SSIGMA = atof(ifp.m_array_strOutput[2]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			HH = atof(ifp.m_array_strOutput[0]);
			HK = atof(ifp.m_array_strOutput[1]);
			SH = atof(ifp.m_array_strOutput[2]);
		}
		ifp.ReadOneLineFromFile();
		if (ifp.ReadOneLineFromFile() > 1)
		{
			DRAFT = atof(ifp.m_array_strOutput[0]);
			BREADTH = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			VS = atof(ifp.m_array_strOutput[0]);
			VE = atof(ifp.m_array_strOutput[1]);
			VI = atof(ifp.m_array_strOutput[2]);
		}
		ifp.m_fp_input = NULL;
	}
}

void CIRES2View::READ_ICECOFF_INPUT()
{
	if (fp_9)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_9;
		ifp.m_array_strSplit.push_back(' ');
		ifp.m_array_strSplit.push_back(':');
		if (ifp.ReadOneLineFromFile() > 1)
		{
			GG = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK1H1 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK1H2 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK2H1 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK2H2 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK3H1 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			XK3H2 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			RHO = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			RHOL = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			E_young = atof(ifp.m_array_strOutput[1]);
		}
		ifp.m_fp_input = NULL;

		//fprintf(stderr, "%lf, %lf %lf %lf %lf %lf %lf %lf %lf %lf", GG, XK1H1, XK1H2, XK2H1, XK2H2, XK3H1, XK3H2, RHO, RHOL, E_young);
	}
}

void CIRES2View::CAL_COND(bool save_file)
{
	RHO0 = RHO - RHOL;

	float SV = (VE - VS) / VI + 1.0f;
	if (VS == VE) SV = 1.0f;
	NV = (int)SV;

	if (fp_8 && save_file)
	{
		fprintf_s(fp_8, " VS =   %lf     VE =   %lf     VI =   %lf\n", VS, VE, VI);
	}

	float SS = (SIGMAK - SIGMAP) / SSIGMA + 1.0f;
	if (SIGMAP == SIGMAK) SS = 1.0f;
	NSIGMA = int(SS);

	float DH = (HK - HH) / SH + 1.0f;
	if (HH == HK) DH = 1.0f;
	NH = int(DH);

	//VSP.resize(NV + 10);
	//VELOCI.resize(NV + 10);
	//FROUD.resize(NV + 10);
	for (int IVS = 1; IVS <= NV; IVS++)
	{
		VSP[IVS] = VS + VI*(IVS - 1);
		VELOCI[IVS] = VSP[IVS] * 0.5144f;
		FROUD[IVS] = VELOCI[IVS] / sqrt(GG * BREADTH);
		//fprintf(stderr, "%lf, %lf\n", VSP[IVS], FROUD[IVS]);
		if (fp_8 && save_file)
		{
			fprintf_s(fp_8, " VSP =   %lf     FROUD =   %lf\n", VSP[IVS], FROUD[IVS]);
		}
	}

	//SIGMA.resize(NSIGMA + 10);
	for (int ISIG = 1; ISIG <= NSIGMA; ISIG++)
	{
		SIGMA[ISIG] = SIGMAP + SSIGMA*(ISIG - 1);
	}

	//THCK.resize(NH + 10);
	for (int IH = 1; IH <= NH; IH++)
	{
		THCK[IH] = HH + SH*(IH - 1);
	}


	if (fp_6 && save_file)
	{
		fprintf_s(fp_6, "     SPEED            = %lf  Knots \n", VS);
		fprintf_s(fp_6, "     BREADTH          = %lf\n", BREADTH);
		fprintf_s(fp_6, "     FRICTIONAL COEFF.= %e\n", FG);
	}
}

void CIRES2View::READ_HULL(int ID)
{
	//	READ_HULL
	errno_t errno;
	if (ID == 1)
	{
		errno = fopen_s(&fp_5, m_strProjectPath + "\\WATERLINE_OUTSIDE.inp", "rt");
	}
	else if (ID == 2)
	{
		errno = fopen_s(&fp_5, m_strProjectPath + "\\WATERLINE_INSIDE.inp", "rt");
	}
	else if (ID == 3)
	{
		errno = fopen_s(&fp_5, m_strProjectPath + "\\WATERLINE_MIDDLE.inp", "rt");
	}
	else
	{
		AfxMessageBox("HULL TYPE ERROR");
	}

	if (fp_5)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_5;
		ifp.m_array_strSplit.push_back(' ');
		ifp.m_array_strSplit.push_back('\t');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			N_FRAME = atoi(ifp.m_array_strOutput[0]);
			//X_COOR.resize(N_FRAME + 10);
			//Y.resize(N_FRAME + 10);
			//Z_COOR.resize(N_FRAME + 10);
			//X_NORM.resize(N_FRAME + 10);
			//Y_NORM.resize(N_FRAME + 10);
			//Z_NORM.resize(N_FRAME + 10);
			//ALPHA.resize(N_FRAME + 10);
			//BETA.resize(N_FRAME + 10);
			//GAMMA.resize(N_FRAME + 10);

			for(int i=0; i<N_FRAME; i++)
			{
				if (ifp.ReadOneLineFromFile() > 7)
				{
					__X_COOR[i+1] = (atof(ifp.m_array_strOutput[0]));
					Y[i+1] = (atof(ifp.m_array_strOutput[1]));
					Z_COOR[i+1] = (atof(ifp.m_array_strOutput[2]));
					X_NORM[i+1] = (atof(ifp.m_array_strOutput[3]));
					Y_NORM[i+1] = (atof(ifp.m_array_strOutput[4]));
					Z_NORM[i+1] = (atof(ifp.m_array_strOutput[5]));
					__ALPHA[i+1] = (atof(ifp.m_array_strOutput[6]));
					BETA[i+1] = (atof(ifp.m_array_strOutput[7]));
					GAMMA[i+1] = (atof(ifp.m_array_strOutput[8]));
				}
			}

			//N_FRAME = X_COOR.size();
		}
		ifp.m_fp_input = NULL;
		fclose(fp_5);
		fp_5 = NULL;
	}
	else
	{
		AfxMessageBox("READ_HULL ERROR");
	}

	fprintf_s(fp_6, "THE NUMBER OF FRAMES = %d\n", N_FRAME);
	fprintf_s(fp_6, "X_COOR(I)        Y(I)    Z_COOR(I)    X_NORM(I)    Y_NORM(I)    Z_NORM(I)     ALPHA(I)      BETA(I)     GAMMA(I)\n");
	for (int I = 1; I <= N_FRAME; I++)
	{
		fprintf_s(fp_6, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
			__X_COOR[I], Y[I], Z_COOR[I], X_NORM[I], Y_NORM[I], Z_NORM[I], __ALPHA[I], BETA[I], GAMMA[I]);

		__ALPHA[I] = __ALPHA[I] / (180 / PI3);
		BETA[I] = BETA[I] / (180 / PI3);
		GAMMA[I] = GAMMA[I] / (180 / PI3);
	}

	__X_COOR[N_FRAME + 1] = 2.0f * __X_COOR[N_FRAME] - __X_COOR[N_FRAME - 1];
	Y[N_FRAME + 1] = Y[N_FRAME];
	Z_COOR[N_FRAME + 1] = Z_COOR[N_FRAME];
	X_NORM[N_FRAME + 1] = X_NORM[N_FRAME];
	Y_NORM[N_FRAME + 1] = Y_NORM[N_FRAME];
	Z_NORM[N_FRAME + 1] = Z_NORM[N_FRAME];
	__ALPHA[N_FRAME + 1] = __ALPHA[N_FRAME];
	BETA[N_FRAME + 1] = BETA[N_FRAME];
	GAMMA[N_FRAME + 1] = GAMMA[N_FRAME];

	//vector< float > element(10);

	for (int II = 1; II <= N_FRAME; II++)
	{
		float TEM_I = min(1.0f, max(-1.0f, (Z_NORM[II] * Z_NORM[II] + Y_NORM[II] * Y_NORM[II])));
		float TEM_J = min(1.0f, max(-1.0f, (X_NORM[II] * Y_NORM[II])));
		__ALPHA[II] = abs(acos(TEM_I));
		fprintf_s(fp_6, "   %d   %lf   %lf\n", II, __ALPHA[II] * 180 / PI3, BETA[II] * 180 / PI3);
	}

	if (ID == 1)
	{
		fopen_s(&fp_15, m_strProjectPath + "\\FRAME.inp", "rt");
		if (fp_15)
		{
			COptImportExportBase ifp;
			ifp.m_fp_input = fp_15;
			ifp.m_array_strSplit.push_back(' ');
			ifp.m_array_strSplit.push_back('\t');
			if (ifp.ReadOneLineFromFile() > 0)
			{
				NS_S = atoi(ifp.m_array_strOutput[0]);
				//vector< float > element(10);
				//S_N.resize(NS_S + 10);
				//N_BETA.resize(NS_S + 10);
				//Y_VAL_ST.resize(NS_S + 10, element);
				//Z_VAL_ST.resize(NS_S + 10, element);
				//BETA_ST.resize(NS_S + 10, element);
				for (int kk = 1; kk <= NS_S; kk++)
				{
					ifp.ReadOneLineFromFile();
					if (ifp.ReadOneLineFromFile() > 1)
					{
						S_N[kk] = (atof(ifp.m_array_strOutput[0]));
						N_BETA[kk] = (atoi(ifp.m_array_strOutput[1]));
						ifp.ReadOneLineFromFile();
						//Y_VAL_ST[kk].resize(N_BETA[kk] + 10);
						//Z_VAL_ST[kk].resize(N_BETA[kk] + 10);
						//BETA_ST[kk].resize(N_BETA[kk] + 10);
						for (int j = 1; j <= N_BETA[kk]; j++)
						{
							Y_VAL_ST[kk][j] = (atof(ifp.m_array_strOutput[j-1]));
						}

						ifp.ReadOneLineFromFile();
						for (int j = 1; j <= N_BETA[kk]; j++)
						{
							Z_VAL_ST[kk][j] = (atof(ifp.m_array_strOutput[j-1]));
						}

						ifp.ReadOneLineFromFile();
						for (int j = 1; j <= N_BETA[kk]; j++)
						{
							BETA_ST[kk][j] = (atof(ifp.m_array_strOutput[j - 1]));
						}
					}
				}
			}
			ifp.m_fp_input = NULL;
		}

		//Sin_Beta.resize(NS_S + 10, element);
		for (int KK = 1; KK <= NS_S; KK++)
		{
			fprintf_s(fp_6, "   %lf,   %d\n", S_N[KK], N_BETA[KK]);
			for (int I = 1; I <= N_BETA[KK]; I++)
			{
				fprintf_s(fp_6, "   %lf", Y_VAL_ST[KK][I]);
			}
			fprintf_s(fp_6, "\n");
			for (int I = 1; I <= N_BETA[KK]; I++)
			{
				fprintf_s(fp_6, "   %lf", Z_VAL_ST[KK][I]);
			}
			fprintf_s(fp_6, "\n");
			for (int I = 1; I <= N_BETA[KK]; I++)
			{
				fprintf_s(fp_6, "   %lf", BETA_ST[KK][I]);
			}
			fprintf_s(fp_6, "\n");

			//Sin_Beta[KK].resize(N_BETA[KK] + 10);
			for (int I = 1; I <= N_BETA[KK]; I++)
			{
				Sin_Beta[KK][I] = sin(BETA_ST[KK][I] * PI3 / 180.0);
			}
		}
	}
}

void CIRES2View::SEL_MODE1()
{
	switch (MBR)
	{
	case 0:
	case 1:
		BREAKING1();
		break;
		//case 2:
		//	BREAKING2();
		//	break;
		//case 3:
		//	BREAKING3();
		//	break;
	}
	switch (MCL)
	{
	case 0:
	case 1:
		CLEARING1();
		break;
		//case 2:
		//	CLEARING2();
		//	break;
		//case 3:
		//	CLEARING3();
		//	break;
	}
	switch (MBU)
	{
	case 0:
	case 1:
		BOUYANCY1();
		break;
		//case 2:
		//	BOUYANCY2();
		//	break;
		//case 3:
		//	BOUYANCY3();
		//	break;
	}
}

void CIRES2View::BREAKING1()
{
	Sum_X = 0;
	Sum_Y = 0;
	Sum_Z = 0;
	float X_TEM, Y_TEM, Z_TEM;
	for (int IA = 1; IA <= N_FRAME-1; IA++)
	{
		X_TEM = GAUS(IA, 1);
		Y_TEM = GAUS(IA, 2);
		Z_TEM = GAUS(IA, 3);
		Sum_X = Sum_X + X_TEM;
		Sum_Y = Sum_Y + Y_TEM;
		Sum_Z = Sum_Z + Z_TEM;
	}

	for (int IS = 1; IS <= NSIGMA; IS++)
	{
		for (int IH = 1; IH <= NH; IH++)
		{
			R_BR[IH][IS] = 0;
		}
	}

	float Eta_1 = Sum_Z / Sum_X;
	//R_BR.clear();
	//vector< float > sigmas(NSIGMA + 10);
	//R_BR.resize(NH + 10, sigmas);
	for (int I = 1; I <= N_FRAME; I++)
	{
		for (int IS = 1; IS <= NSIGMA; IS++)
		{
			for (int IH = 1; IH <= NH; IH++)
			{
				float R_Lamda = pow(((3.0 * RHO * GG) / (E_young * pow(THCK[IH], 3.0))), (1.0 / 4.0));
				R_BR[IH][IS] += (R_Lamda * SIGMA[IS] * THCK[IH] * THCK[IH] / (1.93 * Eta_1)) * Y[I];
			}
		}
	}
}
void CIRES2View::CLEARING1()
{
	//YH.resize(N_FRAME + 10);
	for (int I = 1; I <= N_FRAME; I++)
	{
		YH[I] = tan(__ALPHA[I]);
	}

	float B5 = 0.0;
	float B6 = 0.0;
	float B5_TEM = 0.0;
	float B6_TEM = 0.0;
	float SUM_5, SUM_6;
	float B13 = 0.0;
	float B14 = 0.0;
	float B15 = 0.0;

	for (int IA = 1; IA <= N_FRAME-1; IA++)
	{
		float P = 1.0f;
		if (IA == 1)
			P = 0.5f;
		if (IA == N_FRAME - 1)
			P = 0.5f;

		SUM_5 = GAUS(IA, 15);
		SUM_6 = GAUS(IA, 16);
		B5_TEM = B5_TEM + SUM_5 * P;
		B6_TEM = B6_TEM + SUM_6 * P;
	}
	B5 = B5_TEM;
	B6 = B6_TEM;

	//vector< float > sigmas(NV + 10);
	//R_CL.resize(NH + 10, sigmas);
	for (int IVS = 1; IVS <= NV; IVS++)
	{
		for (int IH = 1; IH <= NH; IH++)
		{
			if(m_bDoubleCalc)
				B13 = 2.0 * RHOL * GG * THCK[IH] * BREADTH * FROUD[IVS];
			else
				B13 = RHOL * GG * THCK[IH] * BREADTH * FROUD[IVS];
			B14 = B13 * XK3H1;
			B15 = B13 * XK3H2 *FG;
			float RV = B5 * B14;
			float RFV = B6 * B15;
			R_CL[IH][IVS] = RV + RFV;
		}
	}
}

void CIRES2View::BOUYANCY1()
{
	//vector< float > element(10);
	//DEPTH_BUOY.resize(NS_S + 10, element);
	//SLOPE_Y.resize(NS_S + 10, element);
	//SLOPE_Z.resize(NS_S + 10, element);
	//SLOPE.resize(NS_S + 10, element);
	//SLOPE_BETA_DEG.resize(NS_S + 10, element);
	//Z_BUOY.resize(NS_S + 10, element);
	//Y_BUOY.resize(NS_S + 10, element);
	//DIST_ICE.resize(NS_S + 10, element);
	//R_SP.resize(NS_S + 10, element);
	//R_SF.resize(NS_S + 10, element);
	//Staion_length_Buoy.resize(NS_S + 10);
	//GIRTH_LENGTH.resize(NS_S + 10);
	//N_END_GIRTH.resize(NS_S + 10);
	for (int KK = 1; KK <= NS_S; KK++)
	{
		//DEPTH_BUOY[KK].resize(N_BETA[KK] + 10);

		//SLOPE_Y[KK].resize(N_BETA[KK] + 10);
		//SLOPE_Z[KK].resize(N_BETA[KK] + 10);
		//SLOPE[KK].resize(N_BETA[KK] + 10);
		//SLOPE_BETA_DEG[KK].resize(N_BETA[KK] + 10);
		//Z_BUOY[KK].resize(N_BETA[KK] + 10);
		//Y_BUOY[KK].resize(N_BETA[KK] + 10);
		//DIST_ICE[KK].resize(N_BETA[KK] + 10);
		//R_SP[KK].resize(N_BETA[KK] + 10);
		//R_SF[KK].resize(N_BETA[KK] + 10);

		for (int I6 = 1; I6 <= N_BETA[KK]; I6++)
		{
			//if(I6 == N_BETA[KK - 1])
			//	DEPTH_BUOY[KK - 1][I6 - 1] = (DRAFT - Z_VAL_ST[KK - 1][I6 - 1]) + (Z_VAL_ST[KK - 1][I6 - 1]) / 2.0;
			//else
			DEPTH_BUOY[KK][I6] = (DRAFT - Z_VAL_ST[KK][I6]) + (Z_VAL_ST[KK][I6] - Z_VAL_ST[KK][I6 + 1]) / 2.0;
		}
	}

	int NO_BUOY;
	//N_BUOY.resize(NS_S + 10);
	for (int KK = 1; KK <= NS_S; KK++)
	{
		NO_BUOY = 6;
		for (int I5 = 2; I5 <= N_BETA[KK]; I5++)
		{
			SLOPE_Y[KK][I5 - 1] = abs(abs(Y_VAL_ST[KK][I5]) - abs(Y_VAL_ST[KK][I5 - 1]));
			SLOPE_Z[KK][I5 - 1] = abs(Z_VAL_ST[KK][I5] - Z_VAL_ST[KK][I5 - 1]);
			//if (SLOPE_Z[KK][I5 - 1] == 0.0f)
			//{
			//	SLOPE[KK - 1][I5 - 2] = 0.0f;
			//}
			//else
			{
				SLOPE[KK][I5 - 1] = SLOPE_Y[KK][I5 - 1] / SLOPE_Z[KK][I5 - 1];
			}

			SLOPE_BETA_DEG[KK][I5 - 1] = atan(SLOPE[KK][I5 - 1]) * (180 / PI3);
			if (NO_BUOY == 6)
			{
				N_BUOY[KK] = I5 - 1;
				if (SLOPE_BETA_DEG[KK][I5 - 1] >= 80.0f)
				{
					NO_BUOY = 5;
				}
			}
		}
	}

	//R_BO.resize(NH + 10);
	for (int IH = 1; IH <= NH; IH++)
	{
		float R_sf = 0.;
		float R_sf_Tem = 0.;
		R_SP[1][1] = 0.;
		float R_sp_Tem = 0.;

		float B_STATION = 0.;
		float X_LPP = 0.;
		float DEP_MEAN = 0.;
		float BETA_MEAN_1 = 0.;
		float GIRTH_IMSI = 0.;
		int I_FINISH;
		for (int KK = 1; KK <= NS_S - 1; KK++)
		{
			Staion_length_Buoy[KK] = abs(S_N[KK + 1] - S_N[KK]);
			GIRTH_LENGTH[KK] = 0.;
			I_FINISH = 5;
			for (int I3 = 1; I3 <= N_BETA[KK]; I3++)
			{
				//if (I3 == N_BETA[KK - 1])
				//{
				//	Z_BUOY[KK][I3] = abs(Z_VAL_ST[KK][I3]);
				//	Y_BUOY[KK][I3] = abs(Y_VAL_ST[KK][I3]);
				//}
				//else
				{
					Z_BUOY[KK][I3] = abs(Z_VAL_ST[KK][I3 + 1] - Z_VAL_ST[KK][I3]);
					Y_BUOY[KK][I3] = abs(Y_VAL_ST[KK][I3 + 1] - Y_VAL_ST[KK][I3]);
				}

				if (Y_BUOY[KK][I3] == 0)
				{
					DIST_ICE[KK][I3] = 0;

					if (I_FINISH == 5)
					{
						N_END_GIRTH[KK] = I3 - 1;
						I_FINISH = 3;
					}
				}
				else if (Y_BUOY[KK][I3] > 0)
				{
					DIST_ICE[KK][I3] = sqrt(pow(Y_BUOY[KK][I3], 2.0) + pow(Z_BUOY[KK][I3], 2.0));
					if (m_bDoubleCalc)
					{
						if (Y_VAL_ST[KK][I3] >= BREADTH / 2.0)
						{
							DIST_ICE[KK][I3] = 0.;
						}
					}
					else
					{
						if (Y_VAL_ST[KK][I3] >= BREADTH)
						{
							DIST_ICE[KK][I3] = 0.;
						}
					}
				}
				GIRTH_LENGTH[KK] = GIRTH_LENGTH[KK] + DIST_ICE[KK][I3];
				N_END_GIRTH[KK] = I3 - 1;
				if (m_bDoubleCalc)
				{
					if (GIRTH_LENGTH[KK] >= BREADTH / 2.0)
					{
						N_END_GIRTH[KK] = I3 - 1;
						//break;
					}
				}
				else
				{
					if (GIRTH_LENGTH[KK] >= BREADTH)
					{
						N_END_GIRTH[KK] = I3 - 1;
						//break;
					}
				}
			}
		}

		float R_SP_TOTAL = 0.;
		float R_SF_TOTAL = 0.;

		for (int KK = 1; KK <= NS_S - 1; KK++)
		{
			float R_SP_ST = 0.;
			float R_SF_ST = 0.;

			for (int I4 = 1; I4 <= N_END_GIRTH[KK]; I4++)
			{
				R_SP[KK][I4] = GG * RHO0 * DIST_ICE[KK][I4] * THCK[IH] * DEPTH_BUOY[KK][I4] * Staion_length_Buoy[KK];
				R_SF[KK][I4] = GG * RHO0 * Staion_length_Buoy[KK] * DIST_ICE[KK][I4] * THCK[IH] * Sin_Beta[KK][I4] * FG;
				R_SF_ST = R_SF_ST + R_SF[KK][I4];
				R_SP_ST = R_SP_ST + R_SP[KK][I4];
			}
			R_SF_TOTAL = R_SF_TOTAL + R_SF_ST;
			R_SP_TOTAL = R_SP_TOTAL + R_SP_ST;
		}

		//if (NS_S > 1)
			R_SP_TOTAL = R_SP_TOTAL / (S_N[1] - S_N[NS_S]);
		//else if (NS_S > 0)
		//	R_SP_TOTAL = R_SP_TOTAL / S_N[0];

		if (m_bDoubleCalc)
		{
			R_BO[IH] = (R_SP_TOTAL + R_SF_TOTAL) * 2.0;
		}
		else
		{
			R_BO[IH] = (R_SP_TOTAL + R_SF_TOTAL);
		}
	}
}

float ALP(float X, int II)
{
	float A = __X_COOR[1] - __X_COOR[II];
	float B = __X_COOR[1] - __X_COOR[II+1];
	float DXX = __X_COOR[II] - __X_COOR[II + 1];
	return (__ALPHA[II] * (B - X) + __ALPHA[II + 1] * (X - A)) / DXX;
}

float BT(float X, int II)
{
	float A = __X_COOR[1] - __X_COOR[II];
	float B = __X_COOR[1] - __X_COOR[II+1];
	float DXX = __X_COOR[II] - __X_COOR[II+1];
	return (BETA[II] * (B - X) + BETA[II + 1] * (X - A)) / DXX;
}

float YY(float X, int II)
{
	float A = __X_COOR[1] - __X_COOR[II];
	float B = __X_COOR[1] - __X_COOR[II + 1];
	float DXX = __X_COOR[II] - __X_COOR[II + 1];
	return (Y[II] * (B - X) + Y[II + 1] * (X - A)) / DXX;
}

float YYH(float X, int II)
{
	float A = __X_COOR[1] - __X_COOR[II];
	float B = __X_COOR[1] - __X_COOR[II + 1];
	float DXX = __X_COOR[II] - __X_COOR[II + 1];
	return (YH[II] * (B - X) + YH[II + 1] * (X - A)) / DXX;
}

float FFX(float X, int II)
{
	return tan(ALP(X, II)) * tan(ALP(X, II))* sqrt(1 + (tan(ALP(X, II)) * tan(ALP(X, II)))) /
		(1 + tan(ALP(X, II)) * tan(ALP(X, II)) + tan(BT(X, II)) * tan(BT(X, II)));
}

float FY(float X, int II)
{
	return tan(ALP(X, II)) * sqrt(1 + (tan(ALP(X, II)) * tan(ALP(X, II)))) /
		(1 + tan(ALP(X, II)) * tan(ALP(X, II)) + tan(BT(X, II)) * tan(BT(X, II)));
}

float FFZ(float X, int II)
{
	return  tan(ALP(X, II)) * tan(BT(X, II)) * sqrt(1 + (tan(ALP(X, II)) * tan(ALP(X, II)))) /
		(1 + tan(ALP(X, II)) * tan(ALP(X, II)) + tan(BT(X, II)) * tan(BT(X, II)));
}

float FB5(float X, int II)
{
	return 1. / cos(ALP(X, II))  * YYH(X, II)*YYH(X, II);
}

float FB6(float X, int II)
{
	return 1. / cos(ALP(X, II))  * YYH(X, II);
}

float CIRES2View::GAUS(int II, int IOP)
{
	float W[] = { 0,  .2955242247, .2692667193, .2190863625, .1494513491, .0666713443 };
	float X[] = { 0,  .1488743389, .4333953941, .6794095682, .8650633666, .9739065285 };

	float A = __X_COOR[1] - __X_COOR[II];
	float B = __X_COOR[1] - __X_COOR[II + 1];
	float XM = 0.5*(B + A);
	float XR = 0.5*(B - A);
	float SS = 0;
	float DX = 0;
	if (XR == 0)
	{
		return 0;
	}
	switch (IOP)
	{
	case 1:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FFX(XM + DX, II) + FFX(XM - DX, II));
		}
	}
	break;
	case 2:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FY(XM + DX, II) + FY(XM - DX, II));
		}
	}
	break;
	case 3:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FFZ(XM + DX, II) + FFZ(XM - DX, II));
		}
	}
	break;
	case 15:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FB5(XM + DX, II) + FB5(XM - DX, II));
		}
	}
	break;
	case 16:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FB6(XM + DX, II) + FB6(XM - DX, II));
		}
	}
	break;
	}

	SS = XR * SS;

	if (SS > 100000000 || SS < -100000000)
		SS = 0;
	return SS;
}

float CIRES2View::GAUS2(int II, int IOP)
{
	float W[] = { 0,  .2955242247, .2692667193, .2190863625, .1494513491, .0666713443 };
	float X[] = { 0,  .1488743389, .4333953941, .6794095682, .8650633666, .9739065285 };

	float A = __X_COOR[1] - __X_COOR[II];
	float B = __X_COOR[1] - (__X_COOR[II]-0.5);
	float XM = 0.5*(B + A);
	float XR = 0.5*(B - A);
	float SS = 0;
	float DX = 0;
	if (XR == 0)
	{
		return 0;
	}
	switch (IOP)
	{
	case 1:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FFX(XM + DX, II) + FFX(XM - DX, II));
		}
	}
	break;
	case 2:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FY(XM + DX, II) + FY(XM - DX, II));
		}
	}
	break;
	case 3:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FFZ(XM + DX, II) + FFZ(XM - DX, II));
		}
	}
	break;
	case 15:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FB5(XM + DX, II) + FB5(XM - DX, II));
		}
	}
	break;
	case 16:
	{
		for (int J = 1; J <= 5; J++)
		{
			DX = XR * X[J];
			SS = SS + W[J] * (FB6(XM + DX, II) + FB6(XM - DX, II));
		}
	}
	break;
	}

	SS = XR * SS;
	return SS;
}

void CIRES2View::WRITE_OUT()
{
	float R_TOTAL;
	fprintf_s(fp_7, "   Vs(kts)      Hi(m)     sigf(kPa)         R_br(kN)          R_cl(kN)          R_bu(kN)           R_i(kN)\n");
	for (int IV = 1; IV <= NV; IV++)
	{
		for (int IS = 1; IS <= NSIGMA; IS++)
		{
			for (int IH = 1; IH <= NH; IH++)
			{
				fprintf_s(fp_8, " IV = %d   SIGMA = %d   THICK = %d\n", IV, IS, IH);
				R_TOTAL = R_BREAK[IH][IS] + R_CLEAR[IH][IV] + R_BOUYA[IH];
				fprintf_s(fp_7, "%9.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",
					VSP[IV], THCK[IH], SIGMA[IS], R_BREAK[IH][IS] * 0.001f, R_CLEAR[IH][IV] * 0.001f, R_BOUYA[IH] * 0.001f, R_TOTAL * 0.001f);
			}
		}
	}
}

//void CIRES2View::OnButtonShowHideSections()
//{
//	UnSetFlipNormal();
//	UnSetCenterPoint();
//
//	if (m_bShowSection)
//	{
//		for (int i = 0; i < osgSections.size(); i++)
//		{
//			PreFrameUpdateData pf(mOSG->mRoot, osgSections[i]);
//			m_QRemoveChild.push(pf);
//			//mOSG->mRoot->removeChild(osgSections[i]);
//		}
//		m_bShowSection = false;
//	}
//	else
//	{
//		for (int i = 0; i < osgSections.size(); i++)
//		{
//			PreFrameUpdateData pf(mOSG->mRoot, osgSections[i]);
//			m_QAddChild.push(pf);
//			//mOSG->mRoot->addChild(osgSections[i]);
//		}
//		m_bShowSection = true;
//	}
//}

void CIRES2View::OnButtonToggleOblique()
{
	m_bDoubleCalc = !m_bDoubleCalc;
}

void CIRES2View::OnButtonShowHideSectionCut()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_bShowSectionData)
	{
		for (int i = 0; i < osgSectionsData.size(); i++)
		{
			PreFrameUpdateData pf(mOSG->mRoot, osgSectionsData[i]);
			m_QRemoveChild.push(pf);

			mOSG->mRoot->removeChild(osgSectionsData[i]);
		}
		m_bShowSectionData = false;
	}
	else
	{
		for (int i = 0; i < osgSectionsData.size(); i++)
		{
			PreFrameUpdateData pf(mOSG->mRoot, osgSectionsData[i]);
			m_QAddChild.push(pf);

			mOSG->mRoot->addChild(osgSectionsData[i]);
		}
		m_bShowSectionData = true;
	}
}


//void CIRES2View::OnButtonHideShowWaterlines()
//{
//	UnSetFlipNormal();
//	UnSetCenterPoint();
//
//	if (m_bShowWaterline)
//	{
//		PreFrameUpdateData pf(mOSG->mRoot, osgWaterline);
//		m_QRemoveChild.push(pf);
//		//mOSG->mRoot->removeChild(osgWaterline);
//		m_bShowWaterline = false;
//	}
//	else
//	{
//		PreFrameUpdateData pf(mOSG->mRoot, osgWaterline);
//		m_QAddChild.push(pf);
//		//mOSG->mRoot->addChild(osgWaterline);
//		m_bShowWaterline = true;
//	}
//}


void CIRES2View::OnButtonShowHideWaterlineCut()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_bShowWaterlineData)
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgWaterlineSection);
		m_QRemoveChild.push(pf);
		//mOSG->mRoot->removeChild(osgWaterlineSection);
		m_bShowWaterlineData = false;
	}
	else
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgWaterlineSection);
		m_QAddChild.push(pf);
		//mOSG->mRoot->addChild(osgWaterlineSection);
		m_bShowWaterlineData = true;
	}
}

//
//void CIRES2View::OnUpdateButtonShowHideSections(CCmdUI *pCmdUI)
//{
//	pCmdUI->SetCheck(m_bShowSection);
//}


void CIRES2View::OnUpdateButtonShowHideSectionCut(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSectionData);
}


//void CIRES2View::OnUpdateButtonHideShowWaterlines(CCmdUI *pCmdUI)
//{
//	pCmdUI->SetCheck(m_bShowWaterline);
//}


void CIRES2View::OnUpdateButtonShowHideWaterlineCut(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowWaterlineData);
}


void CIRES2View::OnButtonSaveHull()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (osgHull->getNumChildren() > 0)
	{
		CFileDialog dlg(TRUE,
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"I-RES2 Files (*.osgb)|*.osgb|All Files (*.*)|*.*||",
			NULL);

		if (dlg.DoModal() == IDOK)
		{
			CT2CA pszConvertedAnsiString(dlg.GetPathName());
			std::string strStd(pszConvertedAnsiString);
			osg::Node* node = osgHull->getChild(0);
			bool result = osgDB::writeNodeFile(*node, strStd);
		}
	}
}

void CIRES2View::SaveCurrentHull()
{
	CT2CA pszConvertedAnsiString(m_strProjectPath + "\\hull.osg");
	std::string strStd(pszConvertedAnsiString);
	osg::Node* node = osgHull->getChild(0);
	bool result = osgDB::writeNodeFile(*node, strStd);
}

void CIRES2View::UpdageHullSize()
{
	int count = osgHull->getNumChildren();
	//for (int i = 0; i < count; i++)
	if (count > 0)
	{
		osg::Geode* geo = osgHull->getChild(0)->asGeode();
		if (geo)
		{
			osg::ComputeBoundsVisitor cbbv;
			geo->accept(cbbv);
			bbHull = cbbv.getBoundingBox();
			char temp_str[200];
			sprintf_s(temp_str, 200, "X: Max %.2lfm  Min %.2lfm", bbHull.xMax() * UNIT_TO_M, bbHull.xMin() * UNIT_TO_M);
			mOSG->m_widgetHullSize[1]->setLabel(temp_str);
			sprintf_s(temp_str, 200, "Y: Max %.2lfm  Min %.2lfm", bbHull.yMax() * UNIT_TO_M, bbHull.yMin() * UNIT_TO_M);
			mOSG->m_widgetHullSize[2]->setLabel(temp_str);
			sprintf_s(temp_str, 200, "Z: Max %.2lfm  Min %.2lfm", bbHull.zMax() * UNIT_TO_M, bbHull.zMin() * UNIT_TO_M);
			mOSG->m_widgetHullSize[3]->setLabel(temp_str);
			if(m_bDoubleCalc)
				sprintf_s(temp_str, 200, "Parallel to X : O");
			else
				sprintf_s(temp_str, 200, "Parallel to X : X");
			mOSG->m_widgetHullSize[4]->setLabel(temp_str);
		}
	}
}

void CIRES2View::UpdateWaterLineGeo()
{
	//int child_no = osgWaterline->getNumChildren();
	//if (child_no > 0)
	//{
	//	osgWaterline->removeChildren(0, child_no);
	//}

	//osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;

	//osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
	//osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
	//v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
	//v_array->push_back(osg::Vec3(bbHullRadius, -bbHullRadius, 0));
	//v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));

	//v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
	//v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));
	//v_array->push_back(osg::Vec3(-bbHullRadius, bbHullRadius, 0));

	bbLength[0] = (bbHull.xMax() - bbHull.xMin()) * 0.6f;
	bbLength[1] = (bbHull.yMax() - bbHull.yMin()) * 0.6f;
	bbLength[2] = (bbHull.zMax() - bbHull.zMin()) * 0.6f;
	//v_array->push_back(osg::Vec3(-bbLength[0], -bbLength[1], 0));
	//v_array->push_back(osg::Vec3(bbLength[0], -bbLength[1], 0));
	//v_array->push_back(osg::Vec3(bbLength[0], bbLength[1], 0));

	//v_array->push_back(osg::Vec3(-bbLength[0], -bbLength[1], 0));
	//v_array->push_back(osg::Vec3(bbLength[0], bbLength[1], 0));
	//v_array->push_back(osg::Vec3(-bbLength[0], bbLength[1], 0));

	//m_iWaterLinePos.set(bbHull.center().x(), bbHull.center().y(), bbHull.center().z());
	DRAFT = bbHull.center().z() * UNIT_TO_M;
	m_fCrossSectionStart = bbHull.xMax();
	m_fCrossSectionEnd = (bbHull.xMax() - bbHull.xMin()) / 2.0f;
	CheckDouble();
	BREADTH = bbHull.yMax();
	//UpdateWaterlinePos();
	SaveIceInput();
	UpdageHullSize();

	osg::Matrix tr;
	osg::Vec3 water_line_pos(bbHull.center().x(), bbHull.center().y(), DRAFT * M_TO_UNIT);

	tr.setTrans(water_line_pos);
	//osgWaterline->setMatrix(tr);

	//osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
	//n_array->push_back(osg::Vec3(0, 0, 1));

	//base_palne_geo->setVertexArray(v_array);
	//osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
	//cross_color->push_back(osg::Vec4(0.8, 0.8, 1.0, 0.5));
	//base_palne_geo->setColorArray(cross_color.get());
	//base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
	//base_palne_geo->setNormalArray(n_array.get());
	//base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//de->push_back(0);
	//de->push_back(1);
	//de->push_back(2);
	//de->push_back(3);
	//de->push_back(4);
	//de->push_back(5);
	//base_palne_geo->addPrimitiveSet(de.get());

	//osg::StateSet *ss = base_palne_geo->getOrCreateStateSet();
	//ss->setMode(GL_BLEND, osg::StateAttribute::ON);
	//osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	//depth->setWriteMask(true);
	//ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
	//ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//ss->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

	//base_plane->addDrawable(base_palne_geo);

	////PreFrameUpdateData pf(osgWaterline, base_plane);
	////m_QAddChild.push(pf);
	//osgWaterline->addChild(base_plane);
}

void CIRES2View::PreFrameUpdate()
{
	mOSG->ResizeToolbar(screen_width, screen_height);

	while (!m_QRemoveChild.empty())
	{
		PreFrameUpdateData pd = m_QRemoveChild.front();
		if (pd.parent_node != NULL)
		{
			if (pd.child_node != NULL)
			{
				if(pd.parent_node->containsNode(pd.child_node))
					pd.parent_node->removeChild(pd.child_node);
			}
			else
			{
				int count = pd.parent_node->getNumChildren();
				if (count > 0)
				{
					pd.parent_node->removeChildren(0, count);
				}
			}
		}

		m_QRemoveChild.pop();
	}

	while (!m_QAddChild.empty())
	{
		PreFrameUpdateData pd = m_QAddChild.front();
		if (pd.parent_node != NULL && pd.child_node != NULL)
		{
			if(!(pd.parent_node->containsNode(pd.child_node)))
			pd.parent_node->addChild(pd.child_node);
		}
		m_QAddChild.pop();
		
		if (pd.parent_node == osgHull)
		{
			SaveCurrentHull();

			UpdageHullSize();

			LoadIceInput();

			//CString temp_string;
			//temp_string.Format("%lf, %lf, %lf, %lf, %lf, %lf", bbHull.xMin(), bbHull.xMax(), bbHull.yMin(), bbHull.yMax(), bbHull.zMin(), bbHull.zMax());
			//AfxMessageBox(temp_string);
			UpdateWaterLineGeo();

			UpdateGlobalAxis(max(max(bbHull.xMax(), bbHull.yMax()), bbHull.zMax()));

			LoadDatumFile();

			ClearCrossSectionLine();
			ClearSections();

			OnButtonzoomall();
			SetTimer(1, 10, NULL);
			//if (m_pEditStart)
			//{
			//	CString temp_string;
			//	temp_string.Format("%g", bbHull.xMin() * UNIT_TO_M);
			//	m_pEditStart->SetEditText(temp_string);

			//	temp_string.Format("%g", bbHull.xMax() * UNIT_TO_M);
			//	m_pEditEnd->SetEditText(temp_string);

			//	//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 * UNIT_TO_M);
			//	m_pEditSpace->SetEditText("0.5");
			//}

		}
	}
}

//void CIRES2View::SetCurrentStep(int i_step)
//{
//	m_pMainFrame->m_wndClassView.SetHulllStatus(true);
//	//mOSG->m_widgetOPTType[m_iCurrentStep]->hide();
//	m_iCurrentStep = i_step;
//	//mOSG->m_widgetOPTType[m_iCurrentStep]->show();
//	mOSG->ResizeToolbar(screen_width, screen_height);
//}
//
void CIRES2View::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		//if (m_pEditStart)
		//{
		//	CString temp_string;
		//	temp_string.Format("%g", bbHull.xMax() * UNIT_TO_M);
		//	m_pEditStart->SetEditText(temp_string);
		//	m_pEditStartWaterline->SetEditText(temp_string);

		//	temp_string.Format("%g", bbHull.xMin() * UNIT_TO_M);
		//	m_pEditEnd->SetEditText(temp_string);
		//	m_pEditEndWaterline->SetEditText(temp_string);

		//	//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 * UNIT_TO_M);
		//	m_pEditSpace->SetEditText("0.5");

		//	UpdateWaterlinePos();
		//	
		//	FitWorld();
		//}

		KillTimer(1);
	}
	else if (nIDEvent == 2)
	{
		int nValue;
		nValue = AfxGetApp()->GetProfileInt(_T("HMS"), _T("Update"), 0);
		if (nValue == 1)
		{
			AfxGetApp()->WriteProfileInt(_T("HMS"), _T("Update"), 0);

			CString strFilePath;
			strFilePath = AfxGetApp()->GetProfileString(_T("HMS"), _T("ICE_INFO"));
			if (PathFileExists(strFilePath))
			{
				if (LoadEstimation(strFilePath))
				{
					strFilePath = AfxGetApp()->GetProfileString(_T("HMS"), _T("NODE_INFO"));
					HMS_OutputFolder = AfxGetApp()->GetProfileString(_T("HMS"), _T("OUTPUT_Folder"));
					if (LoadEstimationNodes(strFilePath))
					{
						m_bRunForInterface = true;
						CalculateOutputResult(2);
						m_bRunForInterface = false;

						AfxGetApp()->WriteProfileInt(_T("HMS"), _T("Update"), 2);
					}
				}
			}
			else
			{
				AfxGetApp()->WriteProfileString(_T("HMS"), _T("Error"), _T("-"));
			}
		}
	}

	CView::OnTimer(nIDEvent);
}


void CIRES2View::OnButtonAnalysis()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_iCurrentStep < 3)
	{
		AfxMessageBox("Calculate Section Points first.");
		CalculateOutputResult(current_calculate_type, false);
		return;
	}

	CalculateOutputResult(current_calculate_type);
	//SetCurrentStep(4);
}

void CIRES2View::ResizeControl(int cx, int cy)
{
	//if (m_pTranslationDlg)
	//{
	//	CRect rect;
	//	GetWindowRect(&rect);
	//	m_pTranslationDlg->MoveWindow(rect.left + 100, rect.bottom - 32, rect.Width() - 200, 25);
	//}
}

void CIRES2View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	screen_width = cx;
	screen_height = cy;

	if (mOSG && mOSG->m_WindowManager)
	{
		mOSG->ResizeToolbar(cx, cy);
	}

	if (m_bInitialize)
	{
		m_cameraStatus->setProjectionMatrixAsOrtho2D(0, mOSG->getViewer()->getCamera()->getViewport()->width(), 0, mOSG->getViewer()->getCamera()->getViewport()->height());
		mOSG->m_fAspect = mOSG->getViewer()->getCamera()->getViewport()->width() / mOSG->getViewer()->getCamera()->getViewport()->height();
	}

	ResizeControl(cx, cy);
}


void CIRES2View::OnButtonSelectSection()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	CDlgSelectSections pDlg(this);
	if (pDlg.DoModal() == IDOK)
	{
	}
}


//void CIRES2View::OnSpinHullXPos()
//{
//	CString str_x = m_pHULLSpinXPos->GetEditText();
//	str_x.Replace(",", "");
//	m_iHULLPos[0] = atof(str_x) * M_TO_UNIT;
//	osg::Matrix m;
//	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iHULLPos);
//	m.setRotate(q);
//	osgHull_Center->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinHullYPos()
//{
//	CString str_x = m_pHULLSpinYPos->GetEditText();
//	str_x.Replace(",", "");
//	m_iHULLPos[1] = atof(str_x) * M_TO_UNIT;
//	osg::Matrix m;
//	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iHULLPos);
//	m.setRotate(q);
//	osgHull_Center->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinHullZPos()
//{
//	CString str_x = m_pHULLSpinZPos->GetEditText();
//	str_x.Replace(",", "");
//	m_iHULLPos[2] = atof(str_x) * M_TO_UNIT;
//	osg::Matrix m;
//	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iHULLPos);
//	m.setRotate(q);
//	osgHull_Center->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinHullXAngle()
//{
//	CString str_x = m_pHULLSpinXRot->GetEditText();
//	str_x.Replace(",", "");
//	m_iHULLRot[0] = osg::DegreesToRadians(atof(str_x));
//	osg::Matrix m;
//	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iHULLPos);
//	m.setRotate(q);
//	osgHull_Center->setMatrix(m);
//}
//
//
//
//void CIRES2View::OnSpinSectionX()
//{
//	CString str_x = m_pSectionSpinXRot->GetEditText();
//	str_x.Replace(",", "");
//	m_iSectionRot[0] = osg::DegreesToRadians(atof(str_x));
//	if(m_bBowBreaking)
//		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
//	else
//		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
//
//	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
//	for (int i = 0; i < osgSections.size(); i++)
//	{
//		osg::Matrix m;
//		if (osgSectionEnable[i])
//		{
//			m.setTrans(osgSectionPosList[i]);
//		}
//		else
//		{
//			m.setTrans(osgSectionPosList[i] + diff);
//		}
//		m.setRotate(osgSectionRotation);
//		osgSections[i]->setMatrix(m);
//	}
//}
//
//void CIRES2View::OnSpinSectionY()
//{
//	CString str_x = m_pSectionSpinYRot->GetEditText();
//	str_x.Replace(",", "");
//	m_iSectionRot[1] = osg::DegreesToRadians(atof(str_x));
//	if (m_bBowBreaking)
//		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
//	else
//		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
//
//	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
//	for (int i = 0; i < osgSections.size(); i++)
//	{
//		osg::Matrix m;
//		if (osgSectionEnable[i])
//		{
//			m.setTrans(osgSectionPosList[i]);
//		}
//		else
//		{
//			m.setTrans(osgSectionPosList[i] + diff);
//		}
//		m.setRotate(osgSectionRotation);
//		osgSections[i]->setMatrix(m);
//	}
//}
//
//void CIRES2View::OnSpinSectionZ()
//{
//	CString str_x = m_pSectionSpinZRot->GetEditText();
//	str_x.Replace(",", "");
//	m_iSectionRot[2] = osg::DegreesToRadians(atof(str_x));
//	if (m_bBowBreaking)
//		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
//	else
//		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
//
//	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
//	for (int i = 0; i < osgSections.size(); i++)
//	{
//		osg::Matrix m;
//		if (osgSectionEnable[i])
//		{
//			m.setTrans(osgSectionPosList[i]);
//		}
//		else
//		{
//			m.setTrans(osgSectionPosList[i] + diff);
//		}
//		m.setRotate(osgSectionRotation);
//		osgSections[i]->setMatrix(m);
//	}
//}
//
//
//void CIRES2View::OnSpinWaterlineZ()
//{
//	CString str_z = m_pWaterlineSpinZPos->GetEditText();
//	str_z.Replace(",", "");
//	m_iWaterLinePos[2] = atof(str_z) * M_TO_UNIT;
//
//	osg::Matrix m;
//	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iWaterLinePos);
//	m.setRotate(q);
//	osgWaterline->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinWaterlineX()
//{
//	CString str_z = m_pWaterlineSpinXRot->GetEditText();
//	str_z.Replace(",", "");
//	m_iWaterLineRot[0] = osg::DegreesToRadians(atof(str_z));
//
//	osg::Matrix m;
//	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iWaterLinePos);
//	m.setRotate(q);
//	osgWaterline->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinWaterlineY()
//{
//	CString str_z = m_pWaterlineSpinYRot->GetEditText();
//	str_z.Replace(",", "");
//	m_iWaterLineRot[1] = osg::DegreesToRadians(atof(str_z));
//
//	osg::Matrix m;
//	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iWaterLinePos);
//	m.setRotate(q);
//	osgWaterline->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinHullYAngle()
//{
//	CString str_x = m_pHULLSpinYRot->GetEditText();
//	str_x.Replace(",", "");
//	m_iHULLRot[1] = osg::DegreesToRadians(atof(str_x));
//	osg::Matrix m;
//	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iHULLPos);
//	m.setRotate(q);
//	osgHull_Center->setMatrix(m);
//}
//
//
//void CIRES2View::OnSpinHullZAngle()
//{
//	CString str_x = m_pHULLSpinZRot->GetEditText();
//	str_x.Replace(",", "");
//	m_iHULLRot[2] = osg::DegreesToRadians(atof(str_x));
//	osg::Matrix m;
//	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iHULLPos);
//	m.setRotate(q);
//	osgHull_Center->setMatrix(m);
//}


void CIRES2View::OnButtonHullPointToPoint()
{
	UnSetFlipNormal();

	PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
	m_QAddChild.push(pf);
	m_iSelectionMode = SELECTION_CENTER;
}

void CIRES2View::SetSelectionWindow(CPoint start, CPoint end)
{
	float y = mOSG->getViewer()->getCamera()->getViewport()->height();

	float center_x = (start.x + end.x) / 2.0f;
	float center_y = (start.y + end.y) / 2.0f;
	m_ptSelectionRect->at(0).set(osg::Vec3(start.x, y - start.y, 0));
	m_ptSelectionRect->at(1).set(osg::Vec3(start.x, y - end.y, 0));
	m_ptSelectionRect->at(2).set(osg::Vec3(start.x, y - end.y, 0));
	m_ptSelectionRect->at(3).set(osg::Vec3(end.x, y - end.y, 0));
	m_ptSelectionRect->at(4).set(osg::Vec3(end.x, y - end.y, 0));
	m_ptSelectionRect->at(5).set(osg::Vec3(end.x, y - start.y, 0));
	m_ptSelectionRect->at(6).set(osg::Vec3(end.x, y - start.y, 0));
	m_ptSelectionRect->at(7).set(osg::Vec3(start.x, y - start.y, 0));
	m_ptSelectionRect->at(8).set(osg::Vec3(center_x - 10.0f, y - center_y, 0));
	m_ptSelectionRect->at(9).set(osg::Vec3(center_x + 10.0f, y - center_y, 0));
	m_ptSelectionRect->at(10).set(osg::Vec3(center_x, y - center_y - 10.0f, 0));
	m_ptSelectionRect->at(11).set(osg::Vec3(center_x, y - center_y + 10.0f, 0));

	m_ptSelectionRect->dirty();
	m_geometrySelectionRect->dirtyBound();
}

void CIRES2View::OnMouseMove(UINT nFlags, CPoint point)
{
	switch (m_iSelectionMode)
	{
	case SELECTION_NONE:
		break;
	case SELECTION_CENTER:
	{
		osg::Vec3d hit_pt;
		osg::Node* p_element = OnSelectPoint(point.x, point.y, hit_pt);
		if (p_element)
		{
			osg::Matrix m;
			m.setTrans(m_geoVertex);
			osgSelectPoint->setMatrix(m);
		}
	}
		break;
	case SELECTION_WINDOW:
	{
		SetSelectionWindow(m_ptStart, point);
	}
	break;
	case SELECTION_NORMAL:
	{
		osg::Vec3d hit_pt;
		osg::Node* p_element = OnSelectPoint(point.x, point.y, hit_pt);
		if (m_SelectedGeo)
		{
			osg::Matrix m;
			m.setTrans(hit_pt);
			selection_point = hit_pt;
			TRACE("%lf, %lf, %lf\n", hit_pt.x(), hit_pt.y(), hit_pt.z());
			osgSelectPoint->setMatrix(m);
		}
	}
	break;
	case SELECTION_MAKEDATUM:
	case SELECTION_TRANSLATION:
	case SELECTION_ROTATION:
	{
		osg::Vec3d hit_pt;
		osg::Node* p_element = OnSelectPoint(point.x, point.y, hit_pt);
		if (p_element)
		{
			osg::Matrix m;
			selection_point = m_geoVertex;
			m.setTrans(m_geoVertex);
			osgSelectPoint->setMatrix(m);
		}
	}
	break;
	default:
		break;
	}

	mOSG->UpdateOrtho();
	CView::OnMouseMove(nFlags, point);
}


void CIRES2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch (m_iSelectionMode)
	{
	case SELECTION_CENTER:
	{
		//osg::Matrix current_tr = osgHull_Center->getMatrix();
		//osg::Matrix inv_tr;
		//inv_tr.invert(current_tr);
		//osg::Vec3 diff = inv_tr.preMult(m_geoVertex);
		////osg::Vec3 diff = m_geoVertex - m_iHULLPos;
		//m_iHULLPos = m_geoVertex;

		//osg::Matrix m;
		//osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
		//m.setTrans(m_iHULLPos);
		//m.setRotate(q);
		//osgHull_Center->setMatrix(m);

		//m.makeIdentity();
		//m.setTrans(osgHull->getMatrix().getTrans() - diff);
		//osgHull->setMatrix(m);

		////UpdateHullPos();

		//PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		//m_QRemoveChild.push(pf);
		m_iSelectionMode = SELECTION_NONE;
	}
	break;
	case SELECTION_WINDOW:
	{
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		mOSG->SetCruise(false);
		m_ptStart = point;
		SetSelectionWindow(m_ptStart, point);
		PreFrameUpdateData pf(mOSG->mRoot, m_cameraStatus);
		m_QAddChild.push(pf);
	}
	break;
	case SELECTION_NORMAL:
	{
		if (m_SelectedGeo)
		{
			//SetNormalFromReference(m_SelectedGeo);
			ConvertNormal(m_SelectedGeo);
			CT2CA pszConvertedAnsiString(m_strProjectPath + "\\hull.osg");
			std::string strStd(pszConvertedAnsiString);
			osg::Node* node = osgHull->getChild(0);
			bool result = osgDB::writeNodeFile(*node, strStd);

			//ClearFunctions();

			//osg::Vec3Array *vertices = (osg::Vec3Array *)m_SelectedGeo->getVertexArray();
			//osg::Vec3Array *normals = (osg::Vec3Array *)m_SelectedGeo->getNormalArray();
			//if (vertices != NULL && normals != NULL)
			//{
			//	osg::Geometry::PrimitiveSetList primitiveList = m_SelectedGeo->getPrimitiveSetList();

			//	int polygonIndex = 0;
			//	osg::Vec3 p1, p2, p3;
			//	int index1, index2, index3;
			//	//double dot00;
			//	//double dot01;
			//	//double dot02;
			//	//double dot11;
			//	//double dot12;
			//	//double invDenom, u, v;
			//	osg::Vec3 v0, v1, v2, v3;

			//	for (int i = 0; i < normals->size(); i++)
			//	{
			//		normals->at(i).set(-normals->at(i));
			//	}
			//	normals->dirty();

			//	for (int x = 0; x < primitiveList.size(); x++)
			//	{
			//		osg::PrimitiveSet *set = primitiveList[x];
			//		int numberOfIndices = set->getNumIndices();
			//		if (set->getMode() == osg::PrimitiveSet::Mode::TRIANGLES)
			//		{
			//			for (unsigned int y = 0; y < set->getDrawElements()->getNumIndices() /*numberOfIndices*/; y += 3)
			//			{
			//				index1 = set->getDrawElements()->getElement(y); //set->index(y); 
			//				index2 = set->getDrawElements()->getElement(y + 1); //set->index(y); 
			//				index3 = set->getDrawElements()->getElement(y + 2); //set->index(y); 

			//				set->getDrawElements()->setElement(y + 1, index3);
			//				set->getDrawElements()->setElement(y + 2, index2);
			//			}
			//			set->dirty();
			//		}
			//	}
			//	m_SelectedGeo->dirtyBound();
			//}
		}
	}
	break;
	case SELECTION_MAKEDATUM:
	case SELECTION_TRANSLATION:
	case SELECTION_ROTATION:
	{
		if (m_pTranslationDlg)
		{
			CString temp_string;
			temp_string.Format("%.3lf, %.3lf, %.3lf", selection_point.x() * UNIT_TO_M, selection_point.y() * UNIT_TO_M, selection_point.z() * UNIT_TO_M);

			m_pTranslationDlg->SetValueString(temp_string);
		}
	}
	break;
	}

	CView::OnLButtonDown(nFlags, point);
}

//void CIRES2View::UpdateHullPos()
//{
//	if (m_pHULLSpinXPos)
//	{
//		CString temp_string;
//		temp_string.Format("%.2lf", m_iHULLPos[0] * UNIT_TO_M);
//		m_pHULLSpinXPos->SetEditText(temp_string);
//		temp_string.Format("%.2lf", m_iHULLPos[1] * UNIT_TO_M);
//		m_pHULLSpinYPos->SetEditText(temp_string);
//		temp_string.Format("%.2lf", m_iHULLPos[2] * UNIT_TO_M);
//		m_pHULLSpinZPos->SetEditText(temp_string);
//	}
//}
//
//void CIRES2View::UpdateWaterlinePos()
//{
//	if (m_pWaterlineSpinZPos)
//	{
//		CString temp_string;
//		temp_string.Format("%.2lf", m_iWaterLinePos[2] * UNIT_TO_M);
//		m_pWaterlineSpinZPos->SetEditText(temp_string);
//		m_pEditSpaceWaterline->SetEditText(temp_string);
//	}
//}

void CIRES2View::OnButtonNurbsConversion()
{
	UnSetCenterPoint();

	if (m_iSelectionMode == SELECTION_NORMAL)
	{
		UnSetFlipNormal();
	}
	else
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QAddChild.push(pf);
		m_iSelectionMode = SELECTION_NORMAL;
	}
}


void CIRES2View::OnUpdateButtonNurbsConversion(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_iSelectionMode == SELECTION_NORMAL);
}

void CIRES2View::UnSetCenterPoint()
{
	if (m_iSelectionMode == SELECTION_CENTER)
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QRemoveChild.push(pf);
		m_iSelectionMode = SELECTION_NONE;
	}
}

void CIRES2View::UnSetFlipNormal()
{
	if (m_iSelectionMode == SELECTION_NORMAL)
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QRemoveChild.push(pf);
		m_iSelectionMode = SELECTION_NONE;
	}
}

void CIRES2View::OnButtonfront()
{
	mOSG->OnViewFront();
}

void CIRES2View::OnButtontop()
{
	mOSG->OnViewTop();
}

void CIRES2View::OnButtonleft()
{
	mOSG->OnViewLeft();
}

void CIRES2View::OnButtonback()
{
	mOSG->OnViewRear();
}

void CIRES2View::OnButtonright()
{
	mOSG->OnViewRight();
}

void CIRES2View::OnButtonbottom()
{
	mOSG->OnViewBottom();
}

void CIRES2View::OnButtonaxo()
{
	mOSG->OnViewIso();
}

void CIRES2View::OnButtonViewPerspective()
{
	mOSG->OnViewPerspective();
}

void CIRES2View::OnButtonViewOrtho()
{
	mOSG->OnViewOrtho();
}

void CIRES2View::OnButtonreset()
{
	FitWorld();
}


void CIRES2View::OnButtonzoomwin()
{
	m_iSelectionMode = SELECTION_WINDOW;
}


void CIRES2View::OnButtonzoomall()
{
	mOSG->OnViewAll();
}


void CIRES2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (m_iSelectionMode)
	{
	case SELECTION_WINDOW:
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		mOSG->SetCruise(true);
		PreFrameUpdateData pf(mOSG->mRoot, m_cameraStatus);
		m_QRemoveChild.push(pf);
		m_iSelectionMode = SELECTION_NONE;
		float dx = abs(m_ptStart.x - point.x);
		float dy = abs(m_ptStart.y - point.y);

		if (dx > 10 && dy > 10)
		{
			float y = mOSG->getViewer()->getCamera()->getViewport()->height();
			float x = mOSG->getViewer()->getCamera()->getViewport()->width();
			float hx = x / 2.0f;
			float hy = y / 2.0f;
			float dhx = min(m_ptStart.x, point.x) + (dx / 2.0f);
			float dhy = min(m_ptStart.y, point.y) + (dy / 2.0f);
			float ratio = max(dx / x, dy / y);
			float center_dx = dhx - hx;
			float center_dy = dhy - hy;
			float distance = mOSG->trackball->getDistance();

			osg::Vec3d hit_pt;
			osg::Node* node = OnSelectPoint(dhx, dhy, hit_pt);
			if (node)
			{
				osg::Matrixd inv = mOSG->trackball->getInverseMatrix();
				osg::Vec3d offset = hit_pt * inv;

				float distance_offset = -offset.z() - distance;
				float new_distance = (distance * ratio) + distance_offset;
				if (new_distance < 1.0f)
					new_distance = 1.0f;
				mOSG->trackball->setCenter(hit_pt);
				mOSG->trackball->setDistance(new_distance);
			}
			else
			{
				osg::Matrix rotation_matrix;
				rotation_matrix.makeRotate(mOSG->trackball->getRotation());

				osg::Vec3d dv(dx, dy, 0);
				mOSG->trackball->setCenter(mOSG->trackball->getCenter() + (dv * rotation_matrix));
				mOSG->trackball->setDistance(distance * ratio);
			}
		}
		else
		{
			osg::Vec3d hit_pt;
			osg::Node* node = OnSelectPoint(point.x, point.y, hit_pt);
			if (node)
			{
				mOSG->trackball->setCenter(hit_pt);
			}
		}
	}
	break;
	}

	CView::OnLButtonUp(nFlags, point);
}

void CIRES2View::BeginProgress()
{
	m_DlgProgress->ShowWindow(SW_SHOW);
	time(&start_time);
}

void CIRES2View::EndProgress()
{
	m_DlgProgress->ShowWindow(SW_HIDE);
}

void CIRES2View::UpdateProgress()
{
	CString temp_string;
	int tm_hour;
	int tm_min;
	int tm_sec;
	time(&end_time);
	double d_diff = difftime(end_time, start_time);
	tm_hour = d_diff / (60 * 60);
	d_diff = d_diff - (tm_hour * 60 * 60);

	tm_min = d_diff / 60;
	d_diff = d_diff - (tm_min * 60);

	tm_sec = d_diff;
	int percent = 0;
	if (m_iTotal > 0)
		percent = m_iStatus * 100 / m_iTotal;
	temp_string.Format("%s : %d%% [%d / %d] [%d : %d : %d]", m_strStatus, percent, m_iStatus, m_iTotal, tm_hour, tm_min, tm_sec);
	m_DlgProgress->m_stStatus.SetWindowText(temp_string);
	m_DlgProgress->m_progressBar.SetPos(percent);
	m_DlgProgress->RedrawWindow();

	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}


void CIRES2View::OnCheckDistanceForAxis()
{
	m_bUseDistanceForAxis = !m_bUseDistanceForAxis;
}


void CIRES2View::OnUpdateCheckDistanceForAxis(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUseDistanceForAxis);
}


void CIRES2View::OnCheckDistance()
{
	m_bUseDistanceForAxis = true;
}


void CIRES2View::OnUpdateCheckDistance(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUseDistanceForAxis);
}


void CIRES2View::OnCheckNumber()
{
	m_bUseDistanceForAxis = false;
}


void CIRES2View::OnUpdateCheckNumber(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bUseDistanceForAxis);
}


void CIRES2View::OnEditDistance()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnUpdateEditDistance(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bUseDistanceForAxis);
}


void CIRES2View::OnEditNumber()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnUpdateEditNumber(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bUseDistanceForAxis);
}


void CIRES2View::OnEditStartWaterline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnEditEndWaterline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


//void CIRES2View::OnEditSpaceWaterline()
//{
//	CString str_z = m_pEditSpaceWaterline->GetEditText();
//	str_z.Replace(",", "");
//	m_iWaterLinePos[2] = atof(str_z) * M_TO_UNIT;
//
//	osg::Matrix m;
//	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
//	m.setTrans(m_iWaterLinePos);
//	m.setRotate(q);
//	osgWaterline->setMatrix(m);
//}


void CIRES2View::OnCheckDistanceWaterline()
{
	m_bUseDistanceForAxisWaterline = true;
}


void CIRES2View::OnUpdateCheckDistanceWaterline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUseDistanceForAxisWaterline);
}


void CIRES2View::OnCheckNumberWaterline()
{
	m_bUseDistanceForAxisWaterline = false;
}


void CIRES2View::OnUpdateCheckNumberWaterline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bUseDistanceForAxisWaterline);
}


void CIRES2View::OnEditDistanceWaterline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnUpdateEditDistanceWaterline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bUseDistanceForAxisWaterline);
}


void CIRES2View::OnEditNumberWaterline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnUpdateEditNumberWaterline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bUseDistanceForAxisWaterline);
}


void CIRES2View::OnButtonDefineSectionsWaterline()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CIRES2View::OnButtonDefine()
{
	CDlgDefineInp pDlg;
	if (pDlg.DoModal() == IDOK)
	{

	}
}


void CIRES2View::CalculateWaterSectionPoint()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if ((m_pMainFrame->m_wndClassView.GetHulllStatus() == false) || (osgHull->getNumChildren() < 1))
	{
		AfxMessageBox("Import HULL data first.");
		return;
	}

	LoadIceInput();

	ClearWaterLine();

	m_iTotal = (osgSectionPosList.size() + 1) * 100;
	m_iStatus = 0;
	m_strStatus.Format("Calculate Section Points : Waterline");
	BeginProgress();


	//osg::Matrix m;
	//osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	//m.setTrans(m_iWaterLinePos);
	//m.setRotate(q);
	osg::Vec3 n(0, 0, 1);
	//n = m.preMult(n) - m_iWaterLinePos;

	osg::Vec3 water_line_pos(bbHull.center().x(), bbHull.center().y(), DRAFT * M_TO_UNIT);

	osg::Matrix tr;
	tr.setTrans(water_line_pos);
	//osgWaterline->setMatrix(tr);

	CalculateSectionWaterline(n, water_line_pos, 0, m_aWaterLinePointData, m_bUseDistanceForAxisWaterline, m_fWaterlinePointGap, m_aWaterLine, false, m_fWaterlineStartPos, m_fWaterlineEndPos);
	AddSectionDataGeo(m_aWaterLinePointData, osgWaterlineSection);
	AddSectionGeo(m_aWaterLine, osgWaterlineSection);

	EndProgress();

	m_pMainFrame->m_wndClassView.SetDraftStatus(true);
	m_pMainFrame->m_wndClassView1.SetDraftStatus(true);
	m_pMainFrame->m_wndClassView2.SetDraftStatus(true);
}

void CIRES2View::CalculateSectionPoint()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if ((m_pMainFrame->m_wndClassView.GetHulllStatus() == false) || (osgHull->getNumChildren() < 1))
	{
		AfxMessageBox("Import HULL data first.");
		return;
	}

	LoadIceInput();

	ClearCrossSectionLine();

	m_aSectionStart.clear();
	m_aSectionEnd.clear();
	m_aSectionOffset.clear();

	m_aSectionStart.push_back(m_fCrossSectionStart);
	m_aSectionEnd.push_back(m_fCrossSectionEnd);
	m_aSectionOffset.push_back(m_fCrossSectionOffset);

	ClearSections();
	DefineSections();

	m_iTotal = (osgSectionPosList.size() + 1) * 100;
	m_iStatus = 0;
	m_strStatus.Format("Calculate Section Points : Waterline");
	BeginProgress();

	//osg::Matrix m;
	//osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	//m.setTrans(m_iWaterLinePos);
	//m.setRotate(q);
	osg::Vec3 n(0, 0, 1);
	//n = m.preMult(n) - m_iWaterLinePos;

	osg::Vec3 normal;
	osg::Matrix m1;

	if (m_bBowBreaking)
	{
		osg::Quat q1(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
		m1.setRotate(q1);
		osg::Vec3 n(1, 0, 0);
		normal = m1.preMult(n);
	}
	else
	{
		osg::Quat q1(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
		m1.setRotate(q1);
		osg::Vec3 n(0, 1, 0);
		normal = m1.preMult(n);
	}

	for (int i = 0; i < osgSectionPosList.size(); i++)
	{
		m_strStatus.Format("Calculate Section Points : Section [%d / %d]", i+1, osgSectionPosList.size());
		if (osgSectionEnable[i])
		{
			vector< PointData > section_point_data;
			vector< vector< osg::Vec3 > > section_line;
			//float m_iSectionRot[3];
			CalculateSectionWaterline(normal, osgSectionPosList[i], 1, section_point_data, m_bUseDistanceForAxis, m_fCrossSectionPointGap, section_line);
			if (section_point_data.size() > 0)
			{
				m_aSectionLine.push_back(section_line);
				m_aSectionPointDataList.push_back(section_point_data);
				AddSectionDataGeo(section_point_data, osgSectionsData[i]);
				AddSectionGeo(section_line, osgSectionsData[i]);
			}
		}
		else
		{
			m_iStatus += 100;
		}
	}
	//fclose(FileLog);
	EndProgress();

	m_pMainFrame->m_wndClassView.SetCrossStatus(true);
	m_pMainFrame->m_wndClassView1.SetCrossStatus(true);
	m_pMainFrame->m_wndClassView2.SetCrossStatus(true);
}

void CIRES2View::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	mOSG->OnMouseHWheel(nFlags, zDelta, pt);
	CView::OnMouseHWheel(nFlags, zDelta, pt);
}

void CIRES2View::LoadDraftSectionSetting()
{
	FILE* fp_draft;
	fopen_s(&fp_draft, m_strProjectPath + "\\DRAFT_SECTION.INP", "rt");
	if (fp_draft)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_draft;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			DRAFT = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_bUseDistanceForAxisWaterline = (atoi(ifp.m_array_strOutput[0]) == 1);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_fWaterlinePointGap = atof(ifp.m_array_strOutput[0]);
		}
	}
}

void CIRES2View::SaveDraftSectionSetting()
{
	FILE* fp_draft;
	fopen_s(&fp_draft, m_strProjectPath + "\\DRAFT_SECTION.INP", "wt");
	if (fp_draft)
	{
		fprintf_s(fp_draft, "%.6lf\n", DRAFT);
		fprintf_s(fp_draft, "%d\n", m_bUseDistanceForAxisWaterline ? 1 : 0);
		fprintf_s(fp_draft, "%.6lf\n", m_fWaterlinePointGap);
		fclose(fp_draft);
	}
}

void CIRES2View::LoadCrossSectionSetting()
{
	FILE* fp_cross;
	fopen_s(&fp_cross, m_strProjectPath + "\\CROSS_SECTION.INP", "rt");
	if (fp_cross)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_cross;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_fCrossSectionStart = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_fCrossSectionEnd = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_fCrossSectionOffset = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_bUseDistanceForAxis = (atoi(ifp.m_array_strOutput[0]) == 1);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			m_fCrossSectionPointGap = atof(ifp.m_array_strOutput[0]);
		}
	}
}

void CIRES2View::SaveCrossSectionSetting()
{
	FILE* fp_cross;
	fopen_s(&fp_cross, m_strProjectPath + "\\CROSS_SECTION.INP", "wt");
	if (fp_cross)
	{
		fprintf_s(fp_cross, "%.6lf\n", m_fCrossSectionStart);
		fprintf_s(fp_cross, "%.6lf\n", m_fCrossSectionEnd);
		fprintf_s(fp_cross, "%.6lf\n", m_fCrossSectionOffset);
		fprintf_s(fp_cross, "%d\n", m_bUseDistanceForAxis ? 1 : 0);
		fprintf_s(fp_cross, "%.6lf\n", m_fCrossSectionPointGap);
		fclose(fp_cross);
	}
}

bool CIRES2View::SelectJob(CString job_name)
{
	CString job_path = m_strProjectPath + "\\JOB\\" + job_name + "\\";
	if (PathFileExists(job_path))
	{
		CopyFiles(m_strProjectPath + "\\JOB\\" + job_name, m_strProjectPath);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ICE_INPUT.inp", m_strAppPath + "\\ICE_INPUT.inp", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ICECOFF_INPUT.inp", m_strAppPath + "\\ICECOFF_INPUT.inp", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\SELECT MODULE.INP", m_strAppPath + "\\SELECT MODULE.INP", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\FRAME.inp", m_strAppPath + "\\FRAME.inp", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\WATERLINE_OUTSIDE.inp", m_strAppPath + "\\WATERLINE_OUTSIDE.inp", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ice_result.OUT", m_strAppPath + "\\ice_result.OUT", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ECHO.OUT", m_strAppPath + "\\ECHO.OUT", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\IMSI.OUT", m_strAppPath + "\\IMSI.OUT", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\CROSS_SECTION.INP", m_strAppPath + "\\CROSS_SECTION.INP", FALSE);
		//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\DRAFT_SECTION.INP", m_strAppPath + "\\DRAFT_SECTION.INP", FALSE);

		LoadDraftSectionSetting();
		CalculateWaterSectionPoint();
		LoadCrossSectionSetting();
		CalculateSectionPoint();

		return true;
	}
	else
	{
		AfxMessageBox("Job folder does not exist.");
	}
	return false;
}

bool CIRES2View::DeleteJob(CString job_name)
{

	CString job_path = m_strProjectPath + "\\JOB\\" + job_name + "\\";
	DeleteDirectoryFile(job_path);

	//if (PathFileExists(job_path))
	//{
	//	CopyFiles(m_strProjectPath + "\\JOB\\" + job_name, m_strProjectPath);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ICE_INPUT.inp", m_strAppPath + "\\ICE_INPUT.inp", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ICECOFF_INPUT.inp", m_strAppPath + "\\ICECOFF_INPUT.inp", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\SELECT MODULE.INP", m_strAppPath + "\\SELECT MODULE.INP", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\FRAME.inp", m_strAppPath + "\\FRAME.inp", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\WATERLINE_OUTSIDE.inp", m_strAppPath + "\\WATERLINE_OUTSIDE.inp", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ice_result.OUT", m_strAppPath + "\\ice_result.OUT", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\ECHO.OUT", m_strAppPath + "\\ECHO.OUT", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\IMSI.OUT", m_strAppPath + "\\IMSI.OUT", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\CROSS_SECTION.INP", m_strAppPath + "\\CROSS_SECTION.INP", FALSE);
	//	//CopyFile(m_strAppPath + "\\JOB\\" + job_name + "\\DRAFT_SECTION.INP", m_strAppPath + "\\DRAFT_SECTION.INP", FALSE);

	//	LoadDraftSectionSetting();
	//	CalculateWaterSectionPoint();
	//	LoadCrossSectionSetting();
	//	CalculateSectionPoint();

	//	return true;
	//}
	//else
	//{
	//	AfxMessageBox("Job folder does not exist.");
	//}
	return false;
}

bool CIRES2View::CreateJob(CString job_name, int type)
{
	CalculateOutputResult(type);

	CString job_path = m_strProjectPath + "\\JOB\\";
	CreateDirectory(job_path, NULL);
	job_path = m_strProjectPath + "\\JOB\\" + job_name + "\\";
	CreateDirectory(job_path, NULL);

	CopyFiles(m_strProjectPath, m_strProjectPath + "\\JOB\\" + job_name, true);

	//CopyFile(m_strAppPath + "\\ICE_INPUT.inp", m_strAppPath + "\\JOB\\" + job_name + "\\ICE_INPUT.inp", FALSE);
	//CopyFile(m_strAppPath + "\\ICECOFF_INPUT.inp", m_strAppPath + "\\JOB\\" + job_name + "\\ICECOFF_INPUT.inp", FALSE);
	//CopyFile(m_strAppPath + "\\SELECT MODULE.INP", m_strAppPath + "\\JOB\\" + job_name + "\\SELECT MODULE.INP", FALSE);
	//CopyFile(m_strAppPath + "\\FRAME.inp", m_strAppPath + "\\JOB\\" + job_name + "\\FRAME.inp", FALSE);
	//CopyFile(m_strAppPath + "\\WATERLINE_OUTSIDE.inp", m_strAppPath + "\\JOB\\" + job_name + "\\WATERLINE_OUTSIDE.inp", FALSE);
	//CopyFile(m_strAppPath + "\\ice_result.OUT", m_strAppPath + "\\JOB\\" + job_name + "\\ice_result.OUT", FALSE);
	//CopyFile(m_strAppPath + "\\ECHO.OUT", m_strAppPath + "\\JOB\\" + job_name + "\\ECHO.OUT", FALSE);
	//CopyFile(m_strAppPath + "\\IMSI.OUT", m_strAppPath + "\\JOB\\" + job_name + "\\IMSI.OUT", FALSE);
	//CopyFile(m_strAppPath + "\\CROSS_SECTION.INP", m_strAppPath + "\\JOB\\" + job_name + "\\CROSS_SECTION.INP", FALSE);
	//CopyFile(m_strAppPath + "\\DRAFT_SECTION.INP", m_strAppPath + "\\JOB\\" + job_name + "\\DRAFT_SECTION.INP", FALSE);

	return true;
}

void CIRES2View::HideOutputSummury()
{
	if (mOSG && mOSG->m_bShowSummury)
	{
		mOSG->m_bShowSummury = false;
		mOSG->m_WindowManager->removeChild(mOSG->m_widgetOutputSumurry);
	}
}

void CIRES2View::HideMap()
{
	if (mOSG && mOSG->m_bShowMap)
	{
		mOSG->m_bShowMap = false;
		mOSG->m_WindowManager->removeChild(mOSG->m_widgetMap);
	}
}

void CIRES2View::ShowAllMap()
{
	map_center = new osg::MatrixTransform();

	if (PathFileExists("d://output.txt"))
	{
		FILE* fp;
		fopen_s(&fp, "d://output.txt", "rt");
		if (fp)
		{
			COptImportExportBase ifp;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			ifp.m_array_strSplit.push_back(',');
			ifp.m_array_strSplit.push_back('(');
			ifp.m_array_strSplit.push_back(')');
			BeginProgress();
			m_iStatus = 0;
			m_iTotal = 179837;
			osg::ref_ptr<osg::Geode> l_pGeodeSurface = new osg::Geode;
			osg::ref_ptr<osg::Vec3Array> map_normal = new osg::Vec3Array;
			map_normal->push_back(osg::Vec3(0, 0, 1));
			osg::Vec4 color_path = osg::Vec4(1, 0, 0, 1);
			osg::Vec4 color_sea = osg::Vec4(109.0 / 255.0, 207.0 / 255.0, 246.0 / 255.0, 1);
			osg::Vec4 color_ice = osg::Vec4(1, 1, 1, 1);
			osg::Vec4 color_land = osg::Vec4(254.0 / 255, 238.0 / 255.0, 179.0 / 255.0, 1);

			osg::ref_ptr<osg::Vec4Array> map_color = new osg::Vec4Array;
			osg::ref_ptr<osg::Vec4Array> map_color1 = new osg::Vec4Array;
			osg::ref_ptr<osg::Vec4Array> map_color2 = new osg::Vec4Array;
			map_color->push_back(color_ice);
			map_color1->push_back(color_land);
			map_color2->push_back(color_path);
			float minx = 0;
			float miny = 0;
			float maxx = 0;
			float maxy = 0;

			int temp_count = ifp.ReadOneLineFromFile();
			while (temp_count > 0)
			{
				osg::ref_ptr<osg::Vec3Array> l_pvOriginalPoints = new osg::Vec3Array;

				int pt_count = atoi(ifp.m_array_strOutput[0]);
				for (int i = 0; i < pt_count; i++)
				{
					int temp_count = ifp.ReadOneLineFromFile();
					if (temp_count > 1)
					{
						float x = atof(ifp.m_array_strOutput[0]);// *25000.0f - 3063000.0f;
						float y = atof(ifp.m_array_strOutput[1]);// *25000.0f - 3112000.0f;

						l_pvOriginalPoints->push_back(osg::Vec3(x, y, 0));

						maxx = (maxx < x) ? x : maxx;
						maxy = (maxy < y) ? y : maxy;
						minx = (minx > x) ? x : minx;
						miny = (miny > y) ? y : miny;
					}
				}

				//osg::ref_ptr<osg::Geometry> l_pGeometrySurface = new osg::Geometry;
				//l_pGeometrySurface->setVertexArray(l_pvOriginalPoints);
				//l_pGeometrySurface->setColorArray(map_color);
				//l_pGeometrySurface->setColorBinding(osg::Geometry::BIND_OVERALL);
				//l_pGeometrySurface->setNormalArray(map_normal);
				//l_pGeometrySurface->setNormalBinding(osg::Geometry::BIND_OVERALL);
				//l_pGeometrySurface->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, l_pvOriginalPoints->size()));
				////l_pGeometrySurface->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 4, 4));

				//osgUtil::Tessellator tessellator;
				//tessellator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
				//tessellator.setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);
				//tessellator.retessellatePolygons(*l_pGeometrySurface);

				//l_pGeodeSurface->addDrawable(l_pGeometrySurface);

				osg::ref_ptr<osg::Geometry> l_pGeometryBorder = new osg::Geometry;
				l_pGeometryBorder->setVertexArray(l_pvOriginalPoints);
				l_pGeometryBorder->setColorArray(map_color1);
				l_pGeometryBorder->setColorBinding(osg::Geometry::BIND_OVERALL);
				l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, l_pvOriginalPoints->size()));
				//l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 4, 4));
				l_pGeometryBorder->getOrCreateStateSet()->setAttribute(new osg::LineWidth(5.0f));

				l_pGeodeSurface->addDrawable(l_pGeometryBorder);

				m_iStatus++;
				UpdateProgress();
				temp_count = ifp.ReadOneLineFromFile();
			}

			osg::ref_ptr<osg::Vec3Array> l_pvOriginalPoints = new osg::Vec3Array;
			osg::ref_ptr<osg::Geometry> l_pGeometryBorder = new osg::Geometry;
			int i_minx = (int)minx;
			int i_maxx = (int)maxx;
			int i_miny = (int)miny;
			int i_maxy = (int)maxy;
			l_pvOriginalPoints->push_back(osg::Vec3(i_minx, i_miny, 0));
			l_pvOriginalPoints->push_back(osg::Vec3(i_maxx, i_miny, 0));
			l_pvOriginalPoints->push_back(osg::Vec3(i_maxx, i_maxy, 0));
			l_pvOriginalPoints->push_back(osg::Vec3(i_minx, i_maxy, 0));

			l_pGeometryBorder->setVertexArray(l_pvOriginalPoints);
			l_pGeometryBorder->setColorArray(map_color1);
			l_pGeometryBorder->setColorBinding(osg::Geometry::BIND_OVERALL);
			l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, l_pvOriginalPoints->size()));
			//l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 4, 4));
			l_pGeometryBorder->getOrCreateStateSet()->setAttribute(new osg::LineWidth(5.0f));

			l_pGeodeSurface->addDrawable(l_pGeometryBorder);
			mOSG->mRoot2->addChild(map_center);
			mOSG->mRoot2->addChild(l_pGeodeSurface);

			EndProgress();

			FILE* fp_out;
			fopen_s(&fp_out, "d://output_range.txt", "wt");
			fprintf_s(fp_out, "%d,%d,%d,%d", i_minx, i_miny, i_maxx, i_maxy);
			fclose(fp_out);
		}
	}
	else
	{
		FILE* fp;
		fopen_s(&fp, "d://points.xy", "rt");
		if (fp)
		{
			COptImportExportBase ifp;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			ifp.m_array_strSplit.push_back(',');
			ifp.m_array_strSplit.push_back('(');
			ifp.m_array_strSplit.push_back(')');
			ifp.m_array_strSplit.push_back('\t');
			FILE* fp_out;
			fopen_s(&fp_out, "d://output.txt", "wt");
			FILE* fp1;
			fopen_s(&fp1, "d://nums.txt", "rt");
			if (fp1)
			{
				BeginProgress();
				m_iStatus = 0;
				m_iTotal = 179837;
				osg::ref_ptr<osg::Geode> l_pGeodePoints = new osg::Geode;
				osg::ref_ptr<osg::Geode> l_pGeodeSurface = new osg::Geode;
				osg::ref_ptr<osg::Vec3Array> map_normal = new osg::Vec3Array;
				map_normal->push_back(osg::Vec3(0, 0, 1));
				osg::Vec4 color_path = osg::Vec4(1, 0, 0, 1);
				osg::Vec4 color_sea = osg::Vec4(109.0 / 255.0, 207.0 / 255.0, 246.0 / 255.0, 1);
				osg::Vec4 color_ice = osg::Vec4(1, 1, 1, 1);
				osg::Vec4 color_land = osg::Vec4(254.0 / 255, 238.0 / 255.0, 179.0 / 255.0, 1);

				float minx, miny, maxx, maxy;

				FILE* fp2;
				fopen_s(&fp2, "d://example_minimum.txt", "rt");
				if (fp2)
				{
					COptImportExportBase ifp2;
					ifp2.m_fp_input = fp2;
					ifp2.m_array_strSplit.push_back(' ');
					ifp2.m_array_strSplit.push_back(',');

					osg::ref_ptr<osg::Geometry> l_pGeometrySurface = new osg::Geometry;
					osg::ref_ptr<osg::Vec3Array> l_pvOriginalPoints = new osg::Vec3Array;
					osg::ref_ptr<osg::Vec4Array> map_color = new osg::Vec4Array;
					map_color->push_back(color_ice);

					int temp_count = ifp2.ReadOneLineFromFile();
					while (temp_count > 1)
					{
						float x = atof(ifp2.m_array_strOutput[temp_count - 2]);// *25000.0f - 3063000.0f;
						float y = atof(ifp2.m_array_strOutput[temp_count - 1]);// *25000.0f - 3112000.0f;

						l_pvOriginalPoints->push_back(osg::Vec3(x, y, 0));
						temp_count = ifp2.ReadOneLineFromFile();

						if (l_pvOriginalPoints->size() == 1)
						{
							minx = maxx = x;
							miny = maxy = y;
						}
						else
						{
							maxx = (maxx < x) ? x : maxx;
							maxy = (maxy < y) ? y : maxy;
							minx = (minx > x) ? x : minx;
							miny = (miny > y) ? y : miny;
						}
					}

					l_pGeometrySurface->setVertexArray(l_pvOriginalPoints);
					l_pGeometrySurface->setColorArray(map_color);
					l_pGeometrySurface->setColorBinding(osg::Geometry::BIND_OVERALL);
					l_pGeometrySurface->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, l_pvOriginalPoints->size()));
					//l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 4, 4));
					l_pGeometrySurface->getOrCreateStateSet()->setAttribute(new osg::Point(5.0f));
					l_pGeodePoints->addDrawable(l_pGeometrySurface);
				}

				COptImportExportBase ifp1;
				ifp1.m_fp_input = fp1;
				int temp_count = ifp1.ReadOneLineFromFile();
				while (temp_count > 0)
				{
					osg::ref_ptr<osg::Geometry> l_pGeometrySurface = new osg::Geometry;
					osg::ref_ptr<osg::Vec3Array> l_pvOriginalPoints = new osg::Vec3Array;
					osg::ref_ptr<osg::Vec4Array> map_color = new osg::Vec4Array;
					osg::ref_ptr<osg::Vec4Array> map_color1 = new osg::Vec4Array;
					map_color->push_back(color_land);
					map_color1->push_back(color_path);

					int pt_count = atoi(ifp1.m_array_strOutput[0]);

					int prev_x = 0;
					int prev_y = 0;
					float temp_min_x, temp_min_y, temp_max_x, temp_max_y;
					for (int i = 0; i < pt_count; i++)
					{
						temp_count = ifp.ReadOneLineFromFile();
						if (temp_count > 1)
						{
							float x = (atof(ifp.m_array_strOutput[0]) + 3063000.0f) / 25000.0f;
							float y = (atof(ifp.m_array_strOutput[1]) + 3112000.0f) / 25000.0f;
							//float x = atof(ifp.m_array_strOutput[0]);
							//float y = atof(ifp.m_array_strOutput[1]);
							//int i_x = (int)x;
							//int i_y = (int)y;

							if (l_pvOriginalPoints->size() == 0)
							{
								temp_min_x = temp_max_x = x;
								temp_min_y = temp_max_y = y;
								//prev_x = i_x;
								//prev_y = i_y;
								l_pvOriginalPoints->push_back(osg::Vec3(x, y, 0));
							}
							else
							{
								temp_max_x = (temp_max_x < x) ? x : temp_max_x;
								temp_max_y = (temp_max_y < y) ? y : temp_max_y;
								temp_min_x = (temp_min_x > x) ? x : temp_min_x;
								temp_min_y = (temp_min_y > y) ? y : temp_min_y;
								//if (prev_x != i_x || prev_y != i_y)
								//{
								l_pvOriginalPoints->push_back(osg::Vec3(x, y, 0));
								//	prev_x = i_x;
								//	prev_y = i_y;
								//}
							}
						}
					}
					if (!((temp_max_x < minx || temp_min_x > maxx) || (temp_max_y < miny || temp_min_y > maxy)))
					{
						if (l_pvOriginalPoints->size() > 0)
						{
							fprintf_s(fp_out, "%d\n", l_pvOriginalPoints->size());
							for (int i = 0; i < l_pvOriginalPoints->size(); i++)
							{
								fprintf_s(fp_out, "%.2lf,%.2lf\n", l_pvOriginalPoints->at(i).x(), l_pvOriginalPoints->at(i).y());
							}
							//l_pGeometrySurface->setVertexArray(l_pvOriginalPoints);
							//l_pGeometrySurface->setColorArray(map_color);
							//l_pGeometrySurface->setColorBinding(osg::Geometry::BIND_OVERALL);
							//l_pGeometrySurface->setNormalArray(map_normal);
							//l_pGeometrySurface->setNormalBinding(osg::Geometry::BIND_OVERALL);
							//l_pGeometrySurface->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, l_pvOriginalPoints->size()));
							////l_pGeometrySurface->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 4, 4));

							//osgUtil::Tessellator tessellator;
							//tessellator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
							//tessellator.setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);
							//tessellator.retessellatePolygons(*l_pGeometrySurface);

							//l_pGeodeSurface->addDrawable(l_pGeometrySurface);

							osg::ref_ptr<osg::Geometry> l_pGeometryBorder = new osg::Geometry;
							l_pGeometryBorder->setVertexArray(l_pvOriginalPoints);
							l_pGeometryBorder->setColorArray(map_color1);
							l_pGeometryBorder->setColorBinding(osg::Geometry::BIND_OVERALL);
							l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, l_pvOriginalPoints->size()));
							//l_pGeometryBorder->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 4, 4));
							l_pGeometryBorder->getOrCreateStateSet()->setAttribute(new osg::LineWidth(5.0f));

							l_pGeodeSurface->addDrawable(l_pGeometryBorder);
						}
					}

					m_iStatus++;
					UpdateProgress();
					temp_count = ifp1.ReadOneLineFromFile();

					//if (m_iStatus > 100)
					//	break;
				}
				fclose(fp_out);

				//osg::AutoTransform* at = new osg::AutoTransform;
				//at->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
				//at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
				//at->addChild(l_pGeodeSurface);

				map_center->addChild(l_pGeodePoints);
				mOSG->mRoot2->addChild(map_center);
				mOSG->mRoot2->addChild(l_pGeodeSurface);

				EndProgress();
			}
		}
	}
}

void CIRES2View::CheckLand()
{
	char temp_file_path[512];
	sprintf_s(temp_file_path, "%s\\map.bmp", m_strAppPath);
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(temp_file_path);
	int width = image->s();
	int height = image->t();
	for (int i = 0; i < maxMapSizeRow; i++)
	{
		for (int j = 0; j < maxMapSizeCol; j++)
		{
			if (realMap[i][j] < 1)
			{
				osg::Vec4 color = image->getColor(osg::Vec2((float)(i + 211) / (float)width, (float)(j + 107) / (float)height));
				if (color.r() < 0.5f)
				{
					realMap[i][j] = -1;
					realMap_origin[i][j] = -1;
				}
			}
		}
	}
}

void CIRES2View::SaveMap(CString path_name)
{
	CString job_path = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\satellite_to_map.out";
	if (PathFileExists(job_path))
	{
		FILE* fp;
		fopen_s(&fp, job_path, "rt");
		if (fp)
		{
			vector< vector< CString > > map_data;
			COptImportExportBase ifp;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			ifp.m_array_strSplit.push_back(',');
			ifp.m_array_strSplit.push_back('(');
			ifp.m_array_strSplit.push_back(')');
			ifp.ReadOneLineFromFile();
			ifp.ReadOneLineFromFile();
			ifp.ReadOneLineFromFile();
			int temp_count = ifp.ReadOneLineFromFile();
			int max_rows = 0;
			int max_cols = 0;
			if (temp_count > 3)
			{
				max_rows = atoi(ifp.m_array_strOutput[1]);
				max_cols = atoi(ifp.m_array_strOutput[3]);
			}

			int max_col = 0;
			int col_count = ifp.ReadOneLineFromFile();
			int row_count = 0;
			while (col_count > 0 && row_count < max_rows)
			{
				row_count++;
				vector< CString > map_row;
				for (int i = 0; i < col_count; i++)
				{
					map_row.push_back(ifp.m_array_strOutput[i]);
					//m_wndExcelView.QuickSetText(i, row_count - 1, ifp.m_array_strOutput[i]);
					//if (ifp.m_array_strOutput[i] == "*")
					//{
					//	m_wndExcelView.QuickSetBackColor(i, row_count - 1, RGB(0, 255, 0));
					//}
				}
				map_data.push_back(map_row);
				if (col_count > max_col)
				{
					max_col = col_count;
				}
				col_count = ifp.ReadOneLineFromFile();
			}

			osg::Vec2 node_shape[] = {
				osg::Vec2(4, 2), osg::Vec2(5, 2),
				osg::Vec2(4, 3), osg::Vec2(5, 3),
				osg::Vec2(4, 4), osg::Vec2(5, 4), osg::Vec2(2, 4), osg::Vec2(3, 4), osg::Vec2(6, 4), osg::Vec2(7, 4),
				osg::Vec2(4, 5), osg::Vec2(5, 5), osg::Vec2(2, 5), osg::Vec2(3, 5), osg::Vec2(6, 5), osg::Vec2(7, 5),
				osg::Vec2(4, 6), osg::Vec2(5, 6),
				osg::Vec2(4, 7), osg::Vec2(5, 7),
			};
			osg::Vec2 route_shape[] = {
				osg::Vec2(4, 8), osg::Vec2(5, 8),
				osg::Vec2(4, 7), osg::Vec2(5, 7),
				osg::Vec2(4, 6), osg::Vec2(5, 6), osg::Vec2(3, 6), osg::Vec2(6, 6),
				osg::Vec2(4, 5), osg::Vec2(5, 5), osg::Vec2(3, 5), osg::Vec2(6, 5),
				osg::Vec2(4, 4), osg::Vec2(5, 4), osg::Vec2(3, 4), osg::Vec2(6, 4), osg::Vec2(2, 4), osg::Vec2(7, 4),
				osg::Vec2(4, 3), osg::Vec2(5, 3), osg::Vec2(3, 3), osg::Vec2(6, 3), osg::Vec2(2, 3), osg::Vec2(7, 3),
				osg::Vec2(4, 2), osg::Vec2(5, 2), osg::Vec2(3, 2), osg::Vec2(6, 2), osg::Vec2(2, 2), osg::Vec2(7, 2), osg::Vec2(1, 2), osg::Vec2(8, 2),
				osg::Vec2(4, 1), osg::Vec2(5, 1), osg::Vec2(3, 1), osg::Vec2(6, 1), osg::Vec2(2, 1), osg::Vec2(7, 1), osg::Vec2(1, 1), osg::Vec2(8, 1),
			};

			osg::ref_ptr<osg::Image> posterImage = 0;
			posterImage = new osg::Image;
			posterImage->allocateImage(map_data[0].size() * 10, map_data.size() * 10, 1, GL_RGBA, GL_UNSIGNED_BYTE);
			osg::Vec4 color_path = osg::Vec4(1, 0, 0, 1);
			osg::Vec4 color_sea = osg::Vec4(109.0 / 255.0, 207.0 / 255.0, 246.0 / 255.0, 1);
			osg::Vec4 color_ice = osg::Vec4(1, 1, 1, 1);
			osg::Vec4 color_land = osg::Vec4(254.0 / 255, 238.0 / 255.0, 179.0 / 255.0, 1);
			osg::Vec4 color_node = osg::Vec4(0, 0, 0, 1);
			for (int i = 0; i < map_data.size(); i++)
			{
				for (int j = 0; j < map_data[i].size(); j++)
				{
					if (map_data[i][j] == "*")
					{
						for (int k = 0; k < 10; k++)
						{
							for (int l = 0; l < 10; l++)
							{
								posterImage->setColor(color_ice, osg::Vec2((float)(j * 10 + k) / (float)(map_data[i].size() * 10), (float)(i * 10 + l) / (float)(map_data.size() * 10)));
							}
						}

						//for (int k = 0; k < 40; k++)
						//{
						//	posterImage->setColor(color_path, osg::Vec2((float)(nodes_rows[i][j] * 10 + route_shape[k].x()) / (float)(map_data[i].size() * 10), (float)(nodes_cols[i][j] * 10 + route_shape[k].y()) / (float)(maxMapSizeCol * 10)));
						//}
						for (int k = 0; k < 20; k++)
						{
							posterImage->setColor(color_path, osg::Vec2((float)(j * 10 + node_shape[k].x()) / (float)(map_data[i].size() * 10), (float)(i * 10 + node_shape[k].y()) / (float)(map_data.size() * 10)));
						}
					}
					else if (map_data[i][j] == "1")
					{
						for (int k = 0; k < 10; k++)
						{
							for (int l = 0; l < 10; l++)
							{
								posterImage->setColor(color_ice, osg::Vec2((float)(j * 10 + k) / (float)(map_data[i].size() * 10), (float)(i * 10 + l) / (float)(map_data.size() * 10)));
							}
						}
					}
					else if (map_data[i][j] == "0")
					{
						for (int k = 0; k < 10; k++)
						{
							for (int l = 0; l < 10; l++)
							{
								posterImage->setColor(color_sea, osg::Vec2((float)(j * 10 + k) / (float)(map_data[i].size() * 10), (float)(i * 10 + l) / (float)(map_data.size() * 10)));
							}
						}
					}
					else
					{
						for (int k = 0; k < 10; k++)
						{
							for (int l = 0; l < 10; l++)
							{
								posterImage->setColor(color_land, osg::Vec2((float)(j * 10 + k) / (float)(map_data[i].size() * 10), (float)(i * 10 + l) / (float)(map_data.size() * 10)));
							}
						}
					}
				}
			}

			char save_file[512];
			sprintf_s(save_file, path_name);
			osgDB::writeImageFile(*posterImage, save_file);
		}
	}
}

void CIRES2View::ShowMap(CString job_name)
{
	m_strJobName = job_name;
	CString job_path = m_strProjectPath + "\\JOB\\" + job_name + "\\satellite_to_map.out";
	if (PathFileExists(job_path))
	{
		FILE* fp;
		fopen_s(&fp, job_path, "rt");
		if (fp)
		{
			vector< vector< CString > > map_data;
			COptImportExportBase ifp;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			ifp.m_array_strSplit.push_back(',');
			ifp.m_array_strSplit.push_back('(');
			ifp.m_array_strSplit.push_back(')');
			ifp.ReadOneLineFromFile();
			ifp.ReadOneLineFromFile();
			ifp.ReadOneLineFromFile();
			int temp_count = ifp.ReadOneLineFromFile();
			int max_rows = 0;
			int max_cols = 0;
			if (temp_count > 3)
			{
				max_rows = atoi(ifp.m_array_strOutput[1]);
				max_cols = atoi(ifp.m_array_strOutput[3]);
			}

			int max_col = 0;
			int col_count = ifp.ReadOneLineFromFile();
			int row_count = 0;
			while (col_count > 0 && row_count < max_rows)
			{
				row_count++;
				vector< CString > map_row;
				for (int i = 0; i < col_count; i++)
				{
					map_row.push_back(ifp.m_array_strOutput[i]);
					//m_wndExcelView.QuickSetText(i, row_count - 1, ifp.m_array_strOutput[i]);
					//if (ifp.m_array_strOutput[i] == "*")
					//{
					//	m_wndExcelView.QuickSetBackColor(i, row_count - 1, RGB(0, 255, 0));
					//}
				}
				map_data.push_back(map_row);
				if (col_count > max_col)
				{
					max_col = col_count;
				}
				col_count = ifp.ReadOneLineFromFile();
			}

			HideMap();

			osg::ref_ptr<osg::Geode> l_pGeodeSurface = new osg::Geode;
			osg::ref_ptr<osg::Geometry> l_pGeometrySurface = new osg::Geometry;
			osg::ref_ptr<osg::Vec3Array> l_pvOriginalPoints = new osg::Vec3Array;
			osg::ref_ptr<osg::Vec4Array> map_color = new osg::Vec4Array;
			osg::ref_ptr<osg::Vec3Array> map_normal = new osg::Vec3Array;
			map_normal->push_back(osg::Vec3(0, 0, 1));
			osg::Vec4 color_path = osg::Vec4(1, 0, 0, 1);
			osg::Vec4 color_sea = osg::Vec4(109.0/255.0, 207.0/255.0, 246.0/255.0, 1);
			osg::Vec4 color_ice = osg::Vec4(1, 1, 1, 1);
			osg::Vec4 color_land = osg::Vec4(254.0 / 255, 238.0 / 255.0, 179.0 / 255.0, 1);
			osg::Vec4 color_black = osg::Vec4(0, 0, 0, 1);

			for (int i = 0; i < map_data.size(); i++)
			{
				for (int j = 0; j < map_data[i].size() && j < max_col; j++)
				{

					if (map_data[i][j] == "*")
					{
							l_pvOriginalPoints->push_back(osg::Vec3((float)j, (float)i, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 1.0f, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 1.0f, (float)i, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 1.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 1.0f, (float)i + 1.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 1.0f, (float)i + 0.6f, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.0f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.0f, (float)i + 1.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 1.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.6f, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.2f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.2f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.0f, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.8f, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.8f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 1.0f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 1.0f, (float)i + 0.4f, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.8f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 1.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 1.0f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.8f, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.0f, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.0f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.2f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.2f, (float)i + 0.4f, 0));
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.2f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.4f, (float)i + 0.8f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.8f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.6f, (float)i + 0.2f, 0));
							map_color->push_back(color_path);
							map_color->push_back(color_path);
							map_color->push_back(color_path);
							map_color->push_back(color_path);

							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.2f, (float)i + 0.4f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.2f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.8f, (float)i + 0.6f, 0));
							l_pvOriginalPoints->push_back(osg::Vec3((float)j + 0.8f, (float)i + 0.4f, 0));
							map_color->push_back(color_path);
							map_color->push_back(color_path);
							map_color->push_back(color_path);
							map_color->push_back(color_path);
					}
					else
					{
						l_pvOriginalPoints->push_back(osg::Vec3(j, i, 0));
						l_pvOriginalPoints->push_back(osg::Vec3(j, i + 1, 0));
						l_pvOriginalPoints->push_back(osg::Vec3(j + 1, i + 1, 0));
						l_pvOriginalPoints->push_back(osg::Vec3(j + 1, i, 0));
						if (map_data[i][j] == "1")	//	얼음
						{
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
							map_color->push_back(color_ice);
						}
						else if (map_data[i][j] == "0")	//	바다
						{
							map_color->push_back(color_sea);
							map_color->push_back(color_sea);
							map_color->push_back(color_sea);
							map_color->push_back(color_sea);
						}
						else	//	땅
						{
							map_color->push_back(color_land);
							map_color->push_back(color_land);
							map_color->push_back(color_land);
							map_color->push_back(color_land);
						}
					}
				}
			}

			l_pGeometrySurface->setVertexArray(l_pvOriginalPoints);
			l_pGeometrySurface->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, l_pvOriginalPoints->size()));
			l_pGeometrySurface->setColorArray(map_color.get());
			l_pGeometrySurface->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			l_pGeometrySurface->setNormalArray(map_normal);
			l_pGeometrySurface->setNormalBinding(osg::Geometry::BIND_OVERALL);

			l_pGeodeSurface->addDrawable(l_pGeometrySurface);

			osg::AutoTransform* at = new osg::AutoTransform;
			at->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
			at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
			at->addChild(l_pGeodeSurface);

			mOSG->mRoot2->removeChildren(0, mOSG->mRoot2->getNumChildren());
			mOSG->mRoot2->addChild(at);
		}
	}
}

void CIRES2View::ShowOutputSummury(CString job_name)
{
	CString job_path = m_strProjectPath + "\\JOB\\" + job_name + "\\ICE_INPUT.inp";

	char temp_str[1024];
	mOSG->m_widgetOutputSumurryString[0]->setLabel("1. ship size");
	sprintf_s(temp_str, 1024, "    X: Max %.2lfm  Min %.2lfm", bbHull.xMax() * UNIT_TO_M, bbHull.xMin() * UNIT_TO_M);
	mOSG->m_widgetOutputSumurryString[1]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Y: Max %.2lfm  Min %.2lfm", bbHull.yMax() * UNIT_TO_M, bbHull.yMin() * UNIT_TO_M);
	mOSG->m_widgetOutputSumurryString[2]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Z: Max %.2lfm  Min %.2lfm", bbHull.zMax() * UNIT_TO_M, bbHull.zMin() * UNIT_TO_M);
	mOSG->m_widgetOutputSumurryString[3]->setLabel(temp_str);

	mOSG->m_widgetOutputSumurryString[4]->setLabel("2. section info");
	mOSG->m_widgetOutputSumurryString[5]->setLabel("     Draft section info");
	float f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11;
	bool is_bool;
	FILE* fp_draft;
	fopen_s(&fp_draft, m_strProjectPath + "\\JOB\\" + job_name + "\\DRAFT_SECTION.INP", "rt");
	if (fp_draft)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_draft;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f1 = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			is_bool = (atoi(ifp.m_array_strOutput[0]) == 1);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f2 = atof(ifp.m_array_strOutput[0]);
		}
	}
	sprintf_s(temp_str, 1024, "        DRAFT: %.6lfm", f1);
	mOSG->m_widgetOutputSumurryString[6]->setLabel(temp_str);
	if(is_bool)
		mOSG->m_widgetOutputSumurryString[7]->setLabel("        Point Distance");
	else
		mOSG->m_widgetOutputSumurryString[7]->setLabel("        Axis Distance");
	sprintf_s(temp_str, 1024, "     Distance Value: %.6lfm", f2);
	mOSG->m_widgetOutputSumurryString[8]->setLabel(temp_str);

	mOSG->m_widgetOutputSumurryString[9]->setLabel("     Cross section info");
		FILE* fp_cross;
	fopen_s(&fp_cross, m_strProjectPath + "\\JOB\\" + job_name + "\\CROSS_SECTION.INP", "rt");
	if (fp_cross)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp_cross;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f1 = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f2 = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f3 = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			is_bool = (atoi(ifp.m_array_strOutput[0]) == 1);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f4 = atof(ifp.m_array_strOutput[0]);
		}
	}
	sprintf_s(temp_str, 1024, "        Start: %.6lfm", f1);
	mOSG->m_widgetOutputSumurryString[10]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "        End: %.6lfm", f2);
	mOSG->m_widgetOutputSumurryString[11]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "        Interval: %.6lfm", f3);
	mOSG->m_widgetOutputSumurryString[12]->setLabel(temp_str);
	if(is_bool)
		mOSG->m_widgetOutputSumurryString[13]->setLabel("        Point Distance");
	else
		mOSG->m_widgetOutputSumurryString[13]->setLabel("        Axis Distance");
	sprintf_s(temp_str, 1024, "     Distance Value: %.6lfm", f4);
	mOSG->m_widgetOutputSumurryString[14]->setLabel(temp_str);


	mOSG->m_widgetOutputSumurryString[15]->setLabel("3. Material info");

	FILE* fp;
	fopen_s(&fp, m_strProjectPath + "\\JOB\\" + job_name + "\\ICE_INPUT.inp", "rt");
	if (fp)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp;
		ifp.m_array_strSplit.push_back(' ');
		if (ifp.ReadOneLineFromFile() > 0)
		{
			HULL_TYPE = atoi(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 0)
		{
			f2 = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			f1 = atof(ifp.m_array_strOutput[0]);
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			f9 = atof(ifp.m_array_strOutput[0]);
			f10 = atof(ifp.m_array_strOutput[1]);
			f11 = atof(ifp.m_array_strOutput[2]);
		}
		ifp.ReadOneLineFromFile();
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 2)
		{
			f6 = atof(ifp.m_array_strOutput[0]);
			f7 = atof(ifp.m_array_strOutput[1]);
			f8 = atof(ifp.m_array_strOutput[2]);
		}
	}

	fopen_s(&fp, m_strProjectPath + "\\JOB\\" + job_name + "\\ICECOFF_INPUT.inp", "rt");
	if (fp)
	{
		COptImportExportBase ifp;
		ifp.m_fp_input = fp;
		ifp.m_array_strSplit.push_back(' ');
		ifp.m_array_strSplit.push_back(':');
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			f5 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			f4 = atof(ifp.m_array_strOutput[1]);
		}
		if (ifp.ReadOneLineFromFile() > 1)
		{
			f3 = atof(ifp.m_array_strOutput[1]) / 1000000.0f;
		}
	}

	sprintf_s(temp_str, 1024, "     flexural strength: %.6lfm", f1);
	mOSG->m_widgetOutputSumurryString[16]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Friction Coeff.: %.6lfm", f2);
	mOSG->m_widgetOutputSumurryString[17]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     E/sigma: %.6lfm", f3);
	mOSG->m_widgetOutputSumurryString[18]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Icd Density (kg/m3): %.6lfm", f4);
	mOSG->m_widgetOutputSumurryString[19]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Water Density (kg/m3): %.6lfm", f5);
	mOSG->m_widgetOutputSumurryString[20]->setLabel(temp_str);
	
	mOSG->m_widgetOutputSumurryString[21]->setLabel("4. Condition info");
	sprintf_s(temp_str, 1024, "     Initial Ship speed (knots): %.6lfm", f6);
	mOSG->m_widgetOutputSumurryString[22]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Max. Ship speed (knots): %.6lfm", f7);
	mOSG->m_widgetOutputSumurryString[23]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Increments Ship speed (knots): %.6lfm", f8);
	mOSG->m_widgetOutputSumurryString[24]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Initial Thickness (m): %.6lfm", f9);
	mOSG->m_widgetOutputSumurryString[25]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Maximum Thickness (m): %.6lfm", f10);
	mOSG->m_widgetOutputSumurryString[26]->setLabel(temp_str);
	sprintf_s(temp_str, 1024, "     Thickness Increments (m): %.6lfm", f11);
	mOSG->m_widgetOutputSumurryString[27]->setLabel(temp_str);

	if (mOSG->m_bShowSummury == false)
	{
		mOSG->m_bShowSummury = true;
		mOSG->m_WindowManager->addChild(mOSG->m_widgetOutputSumurry);
	}
}

void CIRES2View::OnMove(int x, int y)
{
	CView::OnMove(x, y);

	ResizeControl(x, y);
}

void CIRES2View::SetDlgPoint(float x, float y, float z)
{
	switch (m_iSelectionMode)
	{
	case SELECTION_MAKEDATUM:
	{
		AddDatum(x, y, z);
		ClearFunctions();
	}
	break;
	case SELECTION_TRANSLATION:
	{
		switch (m_iCurrentStatus)
		{
		case 0:
		{
			start_point.set(x, y, z);
			m_pTranslationDlg->SetCaption(" << Select a end point for the translation vector or enter X, Y, Z (m)", "Translation", 3);
			m_pTranslationDlg->SetFocus();
			m_iCurrentStatus++;
		}
		break;
		case 1:
		{
			end_point.set(x, y, z);
			osg::Vec3 diff = end_point - start_point;
			osg::Matrix current_tr = osgHull_Center->getMatrix();
			current_tr.postMultTranslate(diff);
			osgHull_Center->setMatrix(current_tr);
			UpdageHullSize();
			ClearFunctions();
		}
		break;
		}

	}
	break;
	case SELECTION_ROTATION:
	{
		switch (m_iCurrentStatus)
		{
		case 0:
		{
			start_point.set(x, y, z);
			m_pTranslationDlg->SetCaption(" << Select a end point for the rotation axis vector or enter X, Y, Z (m)", "Rotation", 3);
			m_pTranslationDlg->SetFocus();
			m_iCurrentStatus++;
		}
		break;
		case 1:
		{
			end_point.set(x, y, z);
			m_pTranslationDlg->SetCaption(" << Input Rotation Angle (degree)", "Rotation", 1);
			m_pTranslationDlg->SetFocus();
			m_iCurrentStatus++;
		}
		break;
		case 2:
		{
			float angle = x;

			//osg::Matrix current_tr = osgHull_Center->getMatrix();
			//osg::Matrix invert_tr;
			//invert_tr.invert(current_tr);
			//osg::Vec3 center_in_tr = invert_tr.preMult(center_point);
			//osg::Vec3 from = start_point - center_point;
			//from.normalize();
			//osg::Vec3 to = end_point - center_point;
			//to.normalize();
			//osg::Quat quat;
			//quat.makeRotate(from, to);
			//current_tr.setTrans(0, 0, 0);
			//current_tr.preMultRotate(quat);
			//osg::Vec3 new_center = current_tr.preMult(center_in_tr);
			//osg::Vec3 new_diff = center_point - new_center;
			//current_tr.setTrans(new_diff);
			//osgHull_Center->setMatrix(current_tr);
			//UpdageHullSize();
			center_point = end_point - start_point;
			osg::Matrix current_tr = osgHull_Center->getMatrix();
			osg::Matrix invert_tr;
			invert_tr.invert(current_tr);
			osg::Vec3 center_in_tr = invert_tr.preMult(start_point);
			osg::Quat quat;
			quat.makeRotate(osg::DegreesToRadians(angle), center_point);
			current_tr.setTrans(0, 0, 0);
			current_tr.preMultRotate(quat);
			osg::Vec3 new_center = current_tr.preMult(center_in_tr);
			osg::Vec3 new_diff = start_point - new_center;
			current_tr.setTrans(new_diff);
			osgHull_Center->setMatrix(current_tr);
			UpdageHullSize();

			float y_max;
			float new_x = GetXforYMax(y_max);
			CheckDouble();
			UpdageHullSize();

			if (new_x > 0.0f)
			{
				m_fCrossSectionEnd = new_x;
				BREADTH = y_max;
				SaveIceInput();
			}
			m_pTranslationDlg->SetCaption(" << Input Rotation Angle (degree)", "Rotation", 1);
			m_pTranslationDlg->SetFocus();
			//ClearFunctions();
		}
		break;
		}
	}
	break;
	}
}

void CIRES2View::ClearFunctions()
{
	PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
	m_QRemoveChild.push(pf);
	m_pTranslationDlg->ShowWindow(SW_HIDE);
	m_iSelectionMode = SELECTION_NONE;
}

void CIRES2View::OnButtonSetNormal()
{
	if (m_iSelectionMode == SELECTION_NORMAL)
	{
		ClearFunctions();
	}
	else
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QAddChild.push(pf);
		m_pTranslationDlg->ShowWindow(SW_HIDE);
		m_iSelectionMode = SELECTION_NORMAL;
	}
}

void CIRES2View::OnButtonMakeDatum()
{
	if (osgHull->getNumChildren() < 1)
	{
		AfxMessageBox("Import HULL data first.");
		CalculateOutputResult(current_calculate_type, false);
		return;
	}

	PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
	m_QAddChild.push(pf);
	m_iCurrentStatus = 0;
	m_iSelectionMode = SELECTION_MAKEDATUM;
	m_pTranslationDlg->SetCaption(" << Select a point for the datum vector or enter X, Y, Z (m)", "Datum", 3);
	m_pTranslationDlg->ShowWindow(SW_SHOW);
	m_pTranslationDlg->SetFocus();
}

void CIRES2View::SetNormalFromReference(osg::Geometry* ref)
{
	int count = osgHull->getNumChildren();
	//for (int i = 0; i < count; i++)
	if(count > 0)
	{
		osg::Geode* geo = osgHull->getChild(0)->asGeode();
		if (geo)
		{
			int geometry_count = geo->getNumChildren();
			for (int j = 0; j < geometry_count; j++)
			{
				osg::Geometry* geometry = geo->getChild(j)->asGeometry();
				if (geometry == ref)
				{
					vector< bool > checked_status;
					checked_status.resize(geometry_count, false);
					vector< osg::Vec3Array* > check_vertices;
					vector< osg::Vec3Array* > check_normals;
					check_vertices.resize(geometry_count, NULL);
					check_normals.resize(geometry_count, NULL);
					osg::Vec3Array *vertices_ref = (osg::Vec3Array *)geometry->getVertexArray();
					osg::Vec3Array *normal_ref = (osg::Vec3Array *)geometry->getNormalArray();
					check_vertices[j] = vertices_ref;
					check_normals[j] = normal_ref;
					checked_status[j] = true;
					CheckNormal(geo, checked_status, check_vertices, check_normals, j);
				}
			}

			CT2CA pszConvertedAnsiString(m_strProjectPath + "\\hull.osg");
			std::string strStd(pszConvertedAnsiString);
			osg::Node* node = osgHull->getChild(0);
			bool result = osgDB::writeNodeFile(*node, strStd);
		}
	}
}

void CIRES2View::ConvertNormal(osg::Geometry* geometry)
{
	if (geometry)
	{
		osg::Geometry::PrimitiveSetList primitiveList = geometry->getPrimitiveSetList();
		int index1, index2, index3;

		osg::Vec3Array* check_n = (osg::Vec3Array*)geometry->getNormalArray();
		if (check_n)
		{
			for (int j = 0; j < check_n->size(); j++)
			{
				check_n->at(j).set(-check_n->at(j));
			}
			check_n->dirty();

			for (int x = 0; x < primitiveList.size(); x++)
			{
				osg::PrimitiveSet* set = primitiveList[x];
				if (set)
				{
					int numberOfIndices = set->getNumIndices();
					if (set->getMode() == osg::PrimitiveSet::Mode::TRIANGLES)
					{
						for (unsigned int y = 0; y < set->getDrawElements()->getNumIndices() /*numberOfIndices*/; y += 3)
						{
							index1 = set->getDrawElements()->getElement(y); //set->index(y); 
							index2 = set->getDrawElements()->getElement(y + 1); //set->index(y); 
							index3 = set->getDrawElements()->getElement(y + 2); //set->index(y); 

							set->getDrawElements()->setElement(y + 1, index3);
							set->getDrawElements()->setElement(y + 2, index2);
						}
						set->dirty();
					}
				}
			}
			geometry->dirtyBound();
		}
	}
}

void CIRES2View::CheckNormal(osg::Geode* parent_geode, vector< bool >& checked_status, vector< osg::Vec3Array* >& check_vertices, vector< osg::Vec3Array* >& check_normals, int check_id)
{
	osg::Vec3Array* current_vertices = check_vertices[check_id];
	osg::Vec3Array* current_normals = check_normals[check_id];
	vector< int > current_check_index;
	if (current_vertices)
	{
		for (int i = 0; i < checked_status.size(); i++)
		{
			if (checked_status[i] == false)
			{
				osg::Vec3Array* check_v = check_vertices[i];
				osg::Vec3Array* check_n = check_normals[i];
				if (check_v == NULL)
				{
					osg::Geometry* geometry = parent_geode->getChild(i)->asGeometry();
					if (geometry)
					{
						check_v = (osg::Vec3Array *)geometry->getVertexArray();
						check_vertices[i] = check_v;
						check_n = (osg::Vec3Array *)geometry->getNormalArray();
						check_normals[i] = check_n;
					}
				}

				int check_count = 0;
				int back_count = 0;
				if (check_v != NULL)
				{
					for (int j = 0; j < current_vertices->size(); j++)
					{
						for (int k = 0; k < check_v->size(); k++)
						{
							float dist = (current_vertices->at(j) - check_v->at(k)).length2();
							if (dist < 0.1f)
							{
								check_count++;

								float dot = current_normals->at(j) * check_n->at(k);
								if (dot < 0.0f)
								{
									back_count++;
								}
							}
						}
					}

					if (check_count > 0)
					{
						checked_status[i] = true;
						current_check_index.push_back(i);
						if (back_count > 0)
						{
							osg::Geometry* geometry = parent_geode->getChild(i)->asGeometry();
							ConvertNormal(geometry);
						}
					}
				}
			}
		}

		for (int i = 0; i < current_check_index.size(); i++)
		{
			CheckNormal(parent_geode, checked_status, check_vertices, check_normals, current_check_index[i]);
		}
	}
}

void CIRES2View::AddDatumInLocal(osg::Vec3 pos)
{
	osg::Geode* datum = new osg::Geode;
	osg::Vec3Array* datum_vert = new osg::Vec3Array;
	osg::Vec4Array* color1 = new osg::Vec4Array;
	osg::Geometry* datum_geometry = new osg::Geometry;
	osg::MatrixTransform* datum_tr = new osg::MatrixTransform();
	osg::Matrix tr;
	tr.setTrans(pos);
	datum_tr->setMatrix(tr);
	datum_tr->setName("DATUM");

	color1->push_back(osg::Vec4(1.0, 1.0, 1.0, 0.8f));
	datum_vert->push_back(osg::Vec3(-1.0f, 0.0f, 0.0f));
	datum_vert->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	datum_vert->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	datum_vert->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
	datum_vert->push_back(osg::Vec3(0.0f, 0.0f, -1.0f));
	datum_vert->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	datum_geometry->setVertexArray(datum_vert);
	datum_geometry->setColorArray(color1);
	datum_geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	datum_geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, datum_vert->size()));

	datum->addDrawable(datum_geometry);
	osg::StateSet* state1 = datum_geometry->getOrCreateStateSet();
	state1->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::LineWidth* lineWidth1 = new osg::LineWidth(2);
	state1->setAttributeAndModes(lineWidth1, osg::StateAttribute::ON);

	osg::MatrixTransform* frame_tr = new osg::MatrixTransform();
	FrameContainer* frameContainer = new FrameContainer;
	frameContainer->setFrameSize(15.0f);
	frameContainer->setFrame(frame_tr);
	frame_tr->addChild(datum);

	datum_tr->addChild(frameContainer);
	osgHull_Center->addChild(datum_tr);
	m_aDatumInHull.push_back(pos);
}

void CIRES2View::AddDatum(float x, float y, float z)
{
	osg::Vec3 global_pos(x, y, z);
	osg::Matrix current_tr = osgHull_Center->getMatrix();
	osg::Matrix invert_tr;
	invert_tr.invert(current_tr);
	osg::Vec3 center_in_tr = invert_tr.preMult(global_pos);

	AddDatumInLocal(center_in_tr);
}

void CIRES2View::SaveDatumFile()
{
	if (m_aDatumInHull.size() > 0)
	{
		CString file_path = m_strProjectPath + "\\datum.txt";
		FILE* fp;
		fopen_s(&fp, file_path, "wt");
		if (fp)
		{
			osg::Matrix m = osgHull_Center->getMatrix();
			for (int i = 0; i < 16; i++)
			{
				const osg::Matrixd::value_type* ptr = m.ptr();
				if(i == 0)
					fprintf_s(fp, "%lf", ptr[i]);
				else
					fprintf_s(fp, ", %lf", ptr[i]);
			}
			fprintf_s(fp, "\n");

			for (int i = 0; i < m_aDatumInHull.size(); i++)
			{
				fprintf_s(fp, "%lf, %lf, %lf\n", m_aDatumInHull[i].x(), m_aDatumInHull[i].y(), m_aDatumInHull[i].z());
			}

			fclose(fp);
		}
	}
}

void CIRES2View::LoadDatumFile()
{
	CString file_path = m_strProjectPath + "\\datum.txt";
	FILE* fp;
	fopen_s(&fp, file_path, "rt");
	if (fp)
	{
		m_aDatumInHull.clear();
		int count = osgHull_Center->getNumChildren();
		for (int i = 2; i < count; i++)
		{
			osgHull_Center->removeChild(i);
		}

		COptImportExportBase ifp;
		ifp.m_fp_input = fp;
		ifp.m_array_strSplit.push_back(',');

		if (ifp.ReadOneLineFromFile() > 15)
		{
			float in_matrix[16];
			for (int i = 0; i < 16; i++)
			{
				in_matrix[i] = atof(ifp.m_array_strOutput[i]);
			}
			osgHull_Center->setMatrix(osg::Matrix::Matrixd(in_matrix));
		}

		while (ifp.ReadOneLineFromFile() > 2)
		{
			osg::Vec3 pos(atof(ifp.m_array_strOutput[0]), atof(ifp.m_array_strOutput[1]), atof(ifp.m_array_strOutput[2]));
			m_aDatumInHull.push_back(pos);
			AddDatumInLocal(pos);
		}

		ifp.m_fp_input = NULL;
		fclose(fp);
	}
}

void CIRES2View::OnButtonSetUnit(UNIT_MODE um)
{
	m_iCurrentUnitMode = um;
	switch (m_iCurrentUnitMode)
	{
	case UNIT_M:
	{
		UNIT_TO_M = 1.0f;
		M_TO_UNIT = 1.0f;
	}
	break;
	case UNIT_MM:
	{
		UNIT_TO_M = 0.001f;
		M_TO_UNIT = 1000.0f;
	}
	break;
	}
}

void CIRES2View::CheckDouble()
{
	osg::Matrix current_tr = osgHull_Center->getMatrix();
	float x_max = -10000.0f;
	float x_min = 10000.0f;
	int count = osgHull->getNumChildren();
	if (count > 0)
	{
		osg::Geode* geo = osgHull->getChild(0)->asGeode();
		if (geo)
		{
			int geometry_count = geo->getNumChildren();
			for (int j = 0; j < geometry_count; j++)
			{
				osg::Geometry* geometry = geo->getChild(j)->asGeometry();
				osg::Vec3Array *vertices = (osg::Vec3Array *)geometry->getVertexArray();
				for (int k = 0; k < vertices->size(); k++)
				{
					osg::Vec3 current_pt = vertices->at(k);
					if (current_pt.x() > x_max)
					{
						x_max = current_pt.x();
						m_ptXMax = current_pt;
					}

					if (current_pt.x() < x_min)
					{
						x_min = current_pt.x();
						m_ptXMin = current_pt;
					}
				}
			}
			osg::Vec3 max_pt = current_tr.preMult(m_ptXMax);
			osg::Vec3 min_pt = current_tr.preMult(m_ptXMin);

			double prev_offset = m_ptXMax.y() - m_ptXMin.y();
			double current_offset = max_pt.y() - min_pt.y();
			//if (abs(prev_offset - current_offset) > 1.0)
			//{
			//	m_bDoubleCalc = false;
			//}
			//else
			//{
			//	m_bDoubleCalc = true;
			//}
		}
	}
}

float CIRES2View::GetXforYMax(float& y_max)
{
	osg::Matrix current_tr = osgHull_Center->getMatrix();
	int count = osgHull->getNumChildren();
	y_max = -1000.0f;
	float x_for_ymax = -1000.0f;
	if (count > 0)
	{
		osg::Geode* geo = osgHull->getChild(0)->asGeode();
		if (geo)
		{
			int geometry_count = geo->getNumChildren();
			for (int j = 0; j < geometry_count; j++)
			{
				osg::Geometry* geometry = geo->getChild(j)->asGeometry();
				osg::Vec3Array *vertices = (osg::Vec3Array *)geometry->getVertexArray();
				for (int k = 0; k < vertices->size(); k++)
				{
					osg::Vec3 current_pt = current_tr.preMult(vertices->at(k));
					if (current_pt.y() > y_max)
					{
						y_max = current_pt.y();
						x_for_ymax = current_pt.x();
					}
				}
			}
		}
	}
	return x_for_ymax;
}

void CIRES2View::OnButtonCalc()
{
	CDlgCalc pDlg;
	if (pDlg.DoModal() == IDOK)
	{

	}
}

void CIRES2View::OnButtonSetUnit()
{
	int count = osgHull->getNumChildren();
	//for (int i = 0; i < count; i++)
	if(count > 0)
	{
		osg::Geode* geo = osgHull->getChild(0)->asGeode();
		if (geo)
		{
			float x_length = bbHull.xMax() - bbHull.xMin();
			float y_length = bbHull.yMax() - bbHull.yMin();
			float z_length = bbHull.zMax() - bbHull.zMin();
			float max_length = max(x_length, max(y_length, z_length));
			if (max_length > 1000.0f)
			{
				int geometry_count = geo->getNumChildren();
				for (int j = 0; j < geometry_count; j++)
				{
					osg::Geometry* geometry = geo->getChild(j)->asGeometry();
					osg::Vec3Array *vertices = (osg::Vec3Array *)geometry->getVertexArray();
					for (int k = 0; k < vertices->size(); k++)
					{
						vertices->at(k).set(vertices->at(k) / 1000.0f);
					}
					vertices->dirty();
					geometry->dirtyBound();
				}
			}

			SaveCurrentHull();
			
			UpdageHullSize();

			UpdateWaterLineGeo();

			UpdateGlobalAxis(max(max(bbHull.xMax(), bbHull.yMax()), bbHull.zMax()));

			mOSG->OnViewAll();
		}
	}
}

void CIRES2View::OnKeyESC()
{
	ClearFunctions();
}

void CIRES2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		OnKeyESC();
	}

	//if (nChar == VK_UP)
	//{
	//	offset_y += 1000.0f;
	//}
	//if (nChar == VK_DOWN)
	//{
	//	offset_y -= 1000.0f;
	//}
	//if (nChar == VK_LEFT)
	//{
	//	offset_x -= 1000.0f;
	//}
	//if (nChar == VK_RIGHT)
	//{
	//	offset_x += 1000.0f;
	//}
	//osg::Matrix A = osg::Matrix::translate(osg::Vec3f(offset_x, offset_y, 0));
	//map_center->setMatrix(A);

	//char temp_str[200];
	//sprintf_s(temp_str, 200, "%.2lf / %.2lf", offset_x, offset_y);
	//mOSG->m_widgetHullSize[4]->setLabel(temp_str);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIRES2View::ClearProject(CString current_project_path)
{
	m_pMainFrame->m_wndClassView.SetHulllStatus(false);
	m_pMainFrame->m_wndClassView.SetDraftStatus(false);
	m_pMainFrame->m_wndClassView.SetCrossStatus(false);
	m_pMainFrame->m_wndClassView.SetMaterialStatus(false);
	m_pMainFrame->m_wndClassView.SetConditionStatus(false);

	m_pMainFrame->m_wndClassView1.SetHulllStatus(false);
	m_pMainFrame->m_wndClassView1.SetDraftStatus(false);
	m_pMainFrame->m_wndClassView1.SetCrossStatus(false);
	m_pMainFrame->m_wndClassView1.SetMaterialStatus(false);
	//m_pMainFrame->m_wndClassView1.SetConditionStatus(false);

	m_pMainFrame->m_wndClassView2.SetHulllStatus(false);
	m_pMainFrame->m_wndClassView2.SetDraftStatus(false);
	m_pMainFrame->m_wndClassView2.SetCrossStatus(false);
	m_pMainFrame->m_wndClassView2.SetMaterialStatus(false);
	m_pMainFrame->m_wndClassView2.SetEstimationStatus(false);	
	//m_pMainFrame->m_wndClassView2.SetConditionStatus(false);

	m_pMainFrame->m_wndFileView.Clear();
	m_pMainFrame->m_wndClassView.ClearJobList();
	m_pMainFrame->m_wndClassView1.ClearJobList();
	m_pMainFrame->m_wndClassView2.ClearJobList();
	if (m_isCreateFolder)
	{
		if (current_project_path != m_strAppPath)
			DeleteDirectoryFile(current_project_path);
	}
}

void CIRES2View::OnButtonSaveSectionData()
{
	if (m_aWaterLinePointData.size() > 0 && m_aSectionPointDataList.size() > 0)
	{
		CString file_name;
		file_name.Format("section_%.1lf.txt", m_fCrossSectionOffset);
		CFileDialog pDlg(FALSE, "txt", file_name);
		if (pDlg.DoModal() == IDOK)
		{
			FILE* save_file;
			fopen_s(&save_file, pDlg.GetPathName(), "wt");
			if (save_file)
			{
				fprintf_s(save_file, "Water Line  Data : %d\n", m_aWaterLinePointData.size());
				fprintf_s(save_file, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", 
					"x",  "y", "z", "nx", "ny", "nz", "alpha", "beta", "gamma", "FX", "FY", "FZ");
				for (int i = 0; i < m_aWaterLinePointData.size(); i++)
				{
					float X_TEM = GAUS(i + 1, 1);
					float Y_TEM = GAUS(i + 1, 2);
					float Z_TEM = GAUS(i + 1, 3);

					fprintf_s(save_file, "%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n",
						m_aWaterLinePointData[i].pnt.x() * UNIT_TO_M,
						m_aWaterLinePointData[i].pnt.y() * UNIT_TO_M,
						m_aWaterLinePointData[i].pnt.z() * UNIT_TO_M,
						m_aWaterLinePointData[i].normal.x(),
						m_aWaterLinePointData[i].normal.y(),
						m_aWaterLinePointData[i].normal.z(),
						m_aWaterLinePointData[i].angle_alpha,
						m_aWaterLinePointData[i].angle_beta,
						m_aWaterLinePointData[i].angle_gamma,
						X_TEM, Y_TEM, Z_TEM);
					if (i == 0)
					{
						max_y = m_aWaterLinePointData[i].pnt.y() * UNIT_TO_M;
					}
					else
					{
						float current_y = m_aWaterLinePointData[i].pnt.y() * UNIT_TO_M;
						if (current_y > max_y)
						{
							max_y = current_y;
						}
					}
				}
				fprintf_s(save_file, "Max Y value : %lf\n\n", max_y);

				//	빈 데이터 삭제
				int i = m_aSectionPointDataList.size();
				for (int j = i - 1; j > -1; j--)
				{
					if (m_aSectionPointDataList[j].size() < 1)
					{
						m_aSectionPointDataList.erase(m_aSectionPointDataList.begin() + j);
					}
				}

				fprintf_s(save_file, "Section  Data : %d\n", m_aSectionPointDataList.size());


				N_FRAME = m_aSectionPointDataList.size();
				int max_data_count = 0;
				int max_data_index = 0;
				for (int i = 0; i < m_aSectionPointDataList.size(); i++)
				{
					if (max_data_count < m_aSectionPointDataList[i].size())
					{
						max_data_count = m_aSectionPointDataList[i].size();
						max_data_index = i;
					}
				}

				vector< int > section_index;
				section_index.resize(m_aSectionPointDataList.size(), 0);
				for (int i = 0; i < max_data_count; i++)
				{
					double ref_x = m_aSectionPointDataList[max_data_index][i].pnt.x();
					double ref_y = m_aSectionPointDataList[max_data_index][i].pnt.y();
					double ref_z = m_aSectionPointDataList[max_data_index][i].pnt.z();
					for (int j = 0; j < m_aSectionPointDataList.size(); j++)
					{
						double max_offset = 10000000.0;
						for (int k = 0; k < m_aSectionPointDataList[j].size(); k++)
						{
							double current_x = m_aSectionPointDataList[j][k].pnt.x();
							double current_y = m_aSectionPointDataList[j][k].pnt.y();
							double current_z = m_aSectionPointDataList[j][k].pnt.z();

							double current_z_offset = abs(current_z - ref_z) * 10.0;
							double current_y_offset = abs(current_y - ref_y);

							if (max_offset > current_z_offset + current_y_offset)
							{
								section_index[j] = k;
								max_offset = current_z_offset + current_y_offset;
								__X_COOR[j + 1] = m_aSectionPointDataList[j][k].pnt.x();
								Y[j + 1] = m_aSectionPointDataList[j][k].pnt.y();
								Z_COOR[j + 1] = m_aSectionPointDataList[j][k].pnt.z();
								X_NORM[j + 1] = m_aSectionPointDataList[j][k].normal.x();
								Y_NORM[j + 1] = m_aSectionPointDataList[j][k].normal.y();
								Z_NORM[j + 1] = m_aSectionPointDataList[j][k].normal.z();
								__ALPHA[j + 1] = m_aSectionPointDataList[j][k].angle_alpha;
								BETA[j + 1] = m_aSectionPointDataList[j][k].angle_beta;
								GAMMA[j + 1] = m_aSectionPointDataList[j][k].angle_gamma;
							}
						}
					}

					for (int I = 1; I <= N_FRAME; I++)
					{
						__ALPHA[I] = __ALPHA[I] / (180 / PI3);
						BETA[I] = BETA[I] / (180 / PI3);
						GAMMA[I] = GAMMA[I] / (180 / PI3);
					}

					__X_COOR[N_FRAME + 1] = 2.0f * __X_COOR[N_FRAME] - __X_COOR[N_FRAME - 1];
					Y[N_FRAME + 1] = Y[N_FRAME];
					Z_COOR[N_FRAME + 1] = Z_COOR[N_FRAME];
					X_NORM[N_FRAME + 1] = X_NORM[N_FRAME];
					Y_NORM[N_FRAME + 1] = Y_NORM[N_FRAME];
					Z_NORM[N_FRAME + 1] = Z_NORM[N_FRAME];
					__ALPHA[N_FRAME + 1] = __ALPHA[N_FRAME];
					BETA[N_FRAME + 1] = BETA[N_FRAME];
					GAMMA[N_FRAME + 1] = GAMMA[N_FRAME];

					for (int j = 0; j < m_aSectionPointDataList.size(); j++)
					{
						m_aSectionPointDataList[j][section_index[j]].fx = GAUS(j + 1, 1);
						m_aSectionPointDataList[j][section_index[j]].fy = GAUS(j + 1, 2);
						m_aSectionPointDataList[j][section_index[j]].fz = GAUS(j + 1, 3);
					}
				}

				for (int i = 0; i < m_aSectionPointDataList.size(); i++)
				{

					fprintf_s(save_file, "[%d] Section  Point : %d\n", i+1, m_aSectionPointDataList[i].size());
					fprintf_s(save_file, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
						"x", "y", "z", "nx", "ny", "nz", "alpha", "beta", "gamma", "FX", "FY", "FZ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.6lf\t%.6lf\t%.6lf\n",
							m_aSectionPointDataList[i][j].pnt.x() * UNIT_TO_M,
							m_aSectionPointDataList[i][j].pnt.y() * UNIT_TO_M,
							m_aSectionPointDataList[i][j].pnt.z() * UNIT_TO_M,
							m_aSectionPointDataList[i][j].normal.x(),
							m_aSectionPointDataList[i][j].normal.y(),
							m_aSectionPointDataList[i][j].normal.z(),
							m_aSectionPointDataList[i][j].angle_alpha,
							m_aSectionPointDataList[i][j].angle_beta,
							m_aSectionPointDataList[i][j].angle_gamma,
							m_aSectionPointDataList[i][j].fx,
							m_aSectionPointDataList[i][j].fy,
							m_aSectionPointDataList[i][j].fz);
					}
					fprintf_s(save_file, "\n");
				}

				fclose(save_file);
				CalculateOutputResult(current_calculate_type);
			}
		}
	}
	else
	{
		AfxMessageBox("Generate Section Data First.");
	}
}

float CIRES2View::GetEstimatonSpeed(float target_sistance, vector<float>& speed, vector<float>& resistance)
{
	float estimation_speed = 0;
	if (target_sistance > 0.0f)
	{
		if (target_sistance < resistance[0])
		{
			estimation_speed = 0.0f;
		}
		else if (target_sistance > resistance[resistance.size() - 1])
		{
			float resistance_offset = resistance[resistance.size() - 1] - resistance[resistance.size() - 2];
			if (resistance_offset <= 0.0f)
			{
				estimation_speed = speed[resistance.size() - 1];
			}
			else
			{
				float speed_offset = speed[resistance.size() - 1] - speed[resistance.size() - 2];
				float target_offset = target_sistance - resistance[resistance.size() - 1];
				float ratio = target_offset / resistance_offset;
				//int up_speed = round(ratio);
				estimation_speed = speed[resistance.size() - 1] + (speed_offset * ratio);
			}
		}
		else
		{
			for (int i = 0; i < resistance.size(); i++)
			{
				if (target_sistance >= resistance[i] && target_sistance <= resistance[i + 1])
				{
					float resistance_offset = resistance[i + 1] - resistance[i];
					float speed_offset = speed[i + 1] - speed[i];
					float target_offset = target_sistance - resistance[i];
					float ratio = target_offset / resistance_offset;
					estimation_speed = speed[i] + (speed_offset * ratio);
					break;
				}
			}
		}
	}
	return estimation_speed;
}

bool CIRES2View::LoadEstimation2(CString file_path)
{
	if (PathFileExists(file_path))
	{
		ifstream ifs;
		ifs.open(file_path);
		if (ifs.is_open())
		{
			string line;
			if (getline(ifs, line))
			{
				m_fTargetResistance = atof(line.c_str());
			}
			if (getline(ifs, line))
			{
				m_fInitSpeed = atof(line.c_str());
			}
			if (getline(ifs, line))
			{
				m_fMaxSpeed = atof(line.c_str());
			}
			if (getline(ifs, line))
			{
				m_fIncreSpeed = atof(line.c_str());
			}
			if (getline(ifs, line))
			{
				startRow = atoi(line.c_str());
			}
			if (getline(ifs, line))
			{
				startCol = atoi(line.c_str());
			}
			if (getline(ifs, line))
			{
				goalRow = atoi(line.c_str());
			}
			if (getline(ifs, line))
			{
				goalCol = atoi(line.c_str());
			}
			ifs.close();
			return true;
		}
	}
	return false;
}

bool CIRES2View::LoadEstimation(CString file_path)
{
	if (PathFileExists(file_path))
	{
		ifstream ifs;
		ifs.open(file_path);
		if (ifs.is_open())
		{
			m_fConcentration.clear();
			m_fFlexuralStrength.clear();
			m_fIceThickness.clear();
			m_fShipSpeed.clear();
			m_fLongitude.clear();
			m_fLatitude.clear();
			m_fX.clear();
			m_fY.clear();
			m_fedgeX.clear();
			m_fedgeY.clear();

			int row_count = 0;
			int max_col_index = 0;
			string line;
			int max_x = 0;
			int max_y = 0;
			while (getline(ifs, line))
			{
				row_count++;
				int col_index = 0;
				stringstream ss(line);
				string temp;
				while (getline(ss, temp, ','))
				{
					if (!temp.empty())
					{
						switch (col_index)
						{
						case 0:
						{
							m_fConcentration.push_back(atof(temp.c_str()));
						}
						break;
						case 1:
						{
							m_fFlexuralStrength.push_back(atof(temp.c_str()));
						}
						break;
						case 2:
						{
							m_fIceThickness.push_back(atof(temp.c_str()));
						}
						break;
						case 3:
						{
							m_fLongitude.push_back(atof(temp.c_str()));
						}
						break;
						case 4:
						{
							m_fLatitude.push_back(atof(temp.c_str()));
						}
						break;
						case 5:
						{
							double x = atof(temp.c_str());
							m_fX.push_back(x);
							if (x > max_x)
								max_x = x;
						}
						break;
						case 6:
						{
							double y = atof(temp.c_str());
							m_fY.push_back(y);
							if (y > max_y)
								max_y = y;
						}
						break;
						case 7:
						{
							m_fedgeX.push_back(atof(temp.c_str()));
						}
						break;
						case 8:
						{
							m_fedgeY.push_back(atof(temp.c_str()));
						}
						break;
						}
					}
					col_index++;
				}
			}

			m_fExSpeed.resize(max_x + 1, vector< float >(max_y + 1, -1));
			realMap.resize(max_x + 1, vector< int >(max_y + 1, 0));

			ifs.close();

			return true;
		}
	}
	return false;
}

bool CIRES2View::LoadEstimationNodes(CString file_path)
{
	if (PathFileExists(file_path))
	{
		ifstream ifs;
		ifs.open(file_path);
		if (ifs.is_open())
		{
			m_aNodePointX.clear();
			m_aNodePointY.clear();
			string line;
			while (getline(ifs, line))
			{
				stringstream ss(line);
				string temp;
				int col_index = 0;
				while (getline(ss, temp, ','))
				{
					if (!temp.empty())
					{
						switch (col_index)
						{
						case 0:
						{
							m_aNodePointX.push_back(atof(temp.c_str()));
						}
						break;
						case 1:
						{
							m_aNodePointY.push_back(atof(temp.c_str()));
						}
						break;
						}
					}
					col_index++;
				}
			}

			ifs.close();

			return true;
		}
	}
	return false;
}