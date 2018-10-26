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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIRES2View

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
	ON_COMMAND(ID_BUTTON_SHOW_HIDE_SECTIONS, &CIRES2View::OnButtonShowHideSections)
	ON_COMMAND(ID_BUTTON_SHOW_HIDE_SECTION_CUT, &CIRES2View::OnButtonShowHideSectionCut)
	ON_COMMAND(ID_BUTTON_HIDE_SHOW_WATERLINES, &CIRES2View::OnButtonHideShowWaterlines)
	ON_COMMAND(ID_BUTTON_SHOW_HIDE_WATERLINE_CUT, &CIRES2View::OnButtonShowHideWaterlineCut)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_HIDE_SECTIONS, &CIRES2View::OnUpdateButtonShowHideSections)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_HIDE_SECTION_CUT, &CIRES2View::OnUpdateButtonShowHideSectionCut)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_HIDE_SHOW_WATERLINES, &CIRES2View::OnUpdateButtonHideShowWaterlines)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_HIDE_WATERLINE_CUT, &CIRES2View::OnUpdateButtonShowHideWaterlineCut)
	ON_COMMAND(ID_BUTTON_SAVE_HULL, &CIRES2View::OnButtonSaveHull)
	ON_COMMAND(ID_BUTTON_ANALYSIS, &CIRES2View::OnButtonAnalysis)
	ON_COMMAND(ID_BUTTON_SELECT_SECTION, &CIRES2View::OnButtonSelectSection)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(ID_SPIN_HULL_X_POS, &CIRES2View::OnSpinHullXPos)
	ON_COMMAND(ID_SPIN_HULL_Y_POS, &CIRES2View::OnSpinHullYPos)
	ON_COMMAND(ID_SPIN_HULL_Z_POS, &CIRES2View::OnSpinHullZPos)
	ON_COMMAND(ID_SPIN_HULL_X_ANGLE, &CIRES2View::OnSpinHullXAngle)
	ON_COMMAND(ID_SPIN_SECTION_Z, &CIRES2View::OnSpinSectionZ)
	ON_COMMAND(ID_SPIN_WATERLINE_Z, &CIRES2View::OnSpinWaterlineZ)
	ON_COMMAND(ID_SPIN_WATERLINE_X, &CIRES2View::OnSpinWaterlineX)
	ON_COMMAND(ID_SPIN_WATERLINE_Y, &CIRES2View::OnSpinWaterlineY)
	ON_COMMAND(ID_SPIN_HULL_Y_ANGLE, &CIRES2View::OnSpinHullYAngle)
	ON_COMMAND(ID_SPIN_HULL_Z_Angle, &CIRES2View::OnSpinHullZAngle)
	ON_COMMAND(ID_BUTTON_HULL_POINT_TO_POINT, &CIRES2View::OnButtonHullPointToPoint)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SPIN_SECTION_X, &CIRES2View::OnSpinSectionX)
	ON_COMMAND(ID_SPIN_SECTION_Y, &CIRES2View::OnSpinSectionY)
	ON_COMMAND(ID_BUTTON_Nurbs_Conversion, &CIRES2View::OnButtonNurbsConversion)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_Nurbs_Conversion, &CIRES2View::OnUpdateButtonNurbsConversion)
	ON_COMMAND(ID_BUTTONFront, &CIRES2View::OnButtonfront)
	ON_COMMAND(ID_BUTTONTop, &CIRES2View::OnButtontop)
	ON_COMMAND(ID_BUTTONLeft, &CIRES2View::OnButtonleft)
	ON_COMMAND(ID_BUTTONBack, &CIRES2View::OnButtonback)
	ON_COMMAND(ID_BUTTONRight, &CIRES2View::OnButtonright)
	ON_COMMAND(ID_BUTTONBottom, &CIRES2View::OnButtonbottom)
	ON_COMMAND(ID_BUTTONAxo, &CIRES2View::OnButtonaxo)
	ON_COMMAND(ID_BUTTONReset, &CIRES2View::OnButtonreset)
	ON_COMMAND(ID_BUTTONZoomWin, &CIRES2View::OnButtonzoomwin)
	ON_COMMAND(ID_BUTTONZoomAll, &CIRES2View::OnButtonzoomall)
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
	ON_COMMAND(ID_EDIT_SPACE_WATERLINE, &CIRES2View::OnEditSpaceWaterline)
	ON_COMMAND(ID_CHECK_DISTANCE_WATERLINE, &CIRES2View::OnCheckDistanceWaterline)
	ON_UPDATE_COMMAND_UI(ID_CHECK_DISTANCE_WATERLINE, &CIRES2View::OnUpdateCheckDistanceWaterline)
	ON_COMMAND(ID_CHECK_NUMBER_WATERLINE, &CIRES2View::OnCheckNumberWaterline)
	ON_UPDATE_COMMAND_UI(ID_CHECK_NUMBER_WATERLINE, &CIRES2View::OnUpdateCheckNumberWaterline)
	ON_COMMAND(ID_EDIT_DISTANCE_WATERLINE, &CIRES2View::OnEditDistanceWaterline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DISTANCE_WATERLINE, &CIRES2View::OnUpdateEditDistanceWaterline)
	ON_COMMAND(ID_EDIT_NUMBER_WATERLINE, &CIRES2View::OnEditNumberWaterline)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NUMBER_WATERLINE, &CIRES2View::OnUpdateEditNumberWaterline)
END_MESSAGE_MAP()

int N_FRAME;
int NS_S;
float S_N[500] = { 0 };
int N_BETA[500] = { 0 };
float Y_VAL_ST[500][500] = { 0 };
float Z_VAL_ST[500][500] = { 0 };
float BETA_ST[500][500] = { 0 };
float Sin_Beta[500][500] = { 0 };
float SLOPE_Y[500][500] = { 0 };
float SLOPE_Z[500][500] = { 0 };
float SLOPE[500][500] = { 0 };
float SLOPE_BETA_DEG[500][500] = { 0 };
float R_SP[500][500] = { 0 };
float Z_BUOY[500][500] = { 0 };
float Y_BUOY[500][500] = { 0 };
float DIST_ICE[500][500] = { 0 };
float R_SF[500][500] = { 0 };
float R_total[500][500] = { 0 };

int N_BUOY[500] = { 0 };
float Staion_length_Buoy[500] = { 0 };
float GIRTH_LENGTH[500] = { 0 };
float N_END_GIRTH[500] = { 0 };
float R_BO[500] = { 0 };

float X_COOR[500] = { 0 };
float Y[500] = { 0 };
float Z_COOR[500] = { 0 };
float X_NORM[500] = { 0 };
float Y_NORM[500] = { 0 };
float Z_NORM[500] = { 0 };
float ALPHA[500] = { 0 };
float BETA[500] = { 0 };
float GAMMA[500] = { 0 };
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
float DRAFT, BREADTH;
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
float VSP[500] = { 0 };
float VELOCI[500] = { 0 };
float FROUD[500] = { 0 };
float SIGMA[500] = { 0 };
float THCK[500] = { 0 };
float R_BR[500][500] = { 0 };
float R_CL[500][500] = { 0 };
float R_CLEAR[500][500] = { 0 };
float R_BREAK[500][500] = { 0 };
float R_BOUYA[500] = { 0 };
float DEPTH_BUOY[500][500] = { 0 };
float YH[500] = { 0 };
FILE* fp_4 = NULL;
FILE* fp_5 = NULL;
FILE* fp_6 = NULL;
FILE* fp_7 = NULL;
FILE* fp_8 = NULL;
FILE* fp_9 = NULL;
FILE* fp_10 = NULL;
FILE* fp_15 = NULL;

// CIRES2View 생성/소멸

CIRES2View::CIRES2View()
	: mOSG(0L)
	, m_pEditStart(NULL)
	, m_pEditEnd(NULL)
	, m_pEditSpace(NULL)
	, m_pHULLSpinXPos(NULL)
	, m_pHULLSpinYPos(NULL)
	, m_pHULLSpinZPos(NULL)
	, m_pHULLSpinXRot(NULL)
	, m_pHULLSpinYRot(NULL)
	, m_pHULLSpinZRot(NULL)
	, m_pSectionSpinXRot(NULL)
	, m_pSectionSpinYRot(NULL)
	, m_pSectionSpinZRot(NULL)
	, m_pWaterlineSpinZPos(NULL)
	, m_pWaterlineSpinXRot(NULL)
	, m_pWaterlineSpinYRot(NULL)
	, m_bBowBreaking(true)
	, m_bShowSection(true)
	, m_bShowSectionData(true)
	, m_bShowWaterline(true)
	, m_bShowWaterlineData(true)
	, m_iCurrentStep(0)
	, m_bSetCenterPoint(false)
	, m_bSetFlipNormal(false)
	, m_bSelectWindow(false)
	, m_bInitialize(false)
	, m_bUseDistanceForAxis(false)
	, m_bUseDistanceForAxisWaterline(false)
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
}

CIRES2View::~CIRES2View()
{
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
	osgHull_Center->addChild(geode);

	double pyramidBaseZ = 100.0f;
	double outerBaseRadius = 50.0f;
	osg::Vec4 color(1, 1, 0, 1);
	//	x axis
	osg::Cone* cone = new osg::Cone(osg::Vec3(950.0f, 0.0f, 0.0f), outerBaseRadius, pyramidBaseZ);
	osg::Quat rotation;
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(1, 0, 0));
	cone->setRotation(rotation);
	osg::ShapeDrawable* shape = new osg::ShapeDrawable(cone);
	shape->setColor(color);
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createXAxis(1000.0f, pyramidBaseZ, outerBaseRadius, color));

	//	 y axis
	cone = new osg::Cone(osg::Vec3(0.0f, 950.0f, 0.0f), outerBaseRadius, pyramidBaseZ);
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0, 1, 0));
	cone->setRotation(rotation);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(color);
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createYAxis(1000.0f, pyramidBaseZ, outerBaseRadius, color));

	//	 z axis
	cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 950.0f), outerBaseRadius, pyramidBaseZ);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(color);
	geode->addDrawable(shape);
	geode->addDrawable(mOSG->createZAxis(1000.0f, pyramidBaseZ, outerBaseRadius, color));

	geode->addDrawable(mOSG->createAxisLabel("X", osg::Vec3(1000.0f + 100, 0, 0), color));
	geode->addDrawable(mOSG->createAxisLabel("Y", osg::Vec3(0, 1000.0f + 100, 0), color));
	geode->addDrawable(mOSG->createAxisLabel("Z", osg::Vec3(0, 0, 1000.0f + 100), color));


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

	osgWaterline = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgWaterline);

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
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pFrame)
	{
		m_pEditStart = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START));
		m_pEditEnd = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END));
		m_pEditSpace = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE));
		//m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));
		m_pEditPointsDistance = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE));
		m_pEditPointsNumber = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER));

		m_pEditStartWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START_WATERLINE));
		m_pEditEndWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END_WATERLINE));
		m_pEditSpaceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE_WATERLINE));
		m_pEditPointsDistanceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE_WATERLINE));
		m_pEditPointsNumberWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER_WATERLINE));

		m_pHULLSpinXPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_X_POS));
		m_pHULLSpinXPos->EnableSpinButtons(-100000000, 100000000);
		m_pHULLSpinXPos->SetEditText("0");
		m_iHULLPos[0] = 0;
		m_pHULLSpinYPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Y_POS));
		m_pHULLSpinYPos->EnableSpinButtons(-100000000, 100000000);
		m_pHULLSpinYPos->SetEditText("0");
		m_iHULLPos[1] = 0;
		m_pHULLSpinZPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Z_POS));
		m_pHULLSpinZPos->EnableSpinButtons(-100000000, 100000000);
		m_pHULLSpinZPos->SetEditText("0");
		m_iHULLPos[2] = 0;

		m_pHULLSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_X_ANGLE));
		m_pHULLSpinXRot->EnableSpinButtons(-360, 360);
		m_pHULLSpinXRot->SetEditText("0");
		m_iHULLRot[0] = 0;
		m_pHULLSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Y_ANGLE));
		m_pHULLSpinYRot->EnableSpinButtons(-360, 360);
		m_pHULLSpinYRot->SetEditText("0");
		m_iHULLRot[1] = 0;
		m_pHULLSpinZRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Z_Angle));
		m_pHULLSpinZRot->EnableSpinButtons(-360, 360);
		m_pHULLSpinZRot->SetEditText("0");
		m_iHULLRot[2] = 0;

		m_pSectionSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_X));
		m_pSectionSpinXRot->EnableSpinButtons(-360, 360);
		m_pSectionSpinXRot->SetEditText("0");
		m_iSectionRot[0] = 0;
		m_pSectionSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_Y));
		m_pSectionSpinYRot->EnableSpinButtons(-360, 360);
		m_pSectionSpinYRot->SetEditText("0");
		m_iSectionRot[1] = 0;
		m_pSectionSpinZRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_Z));
		m_pSectionSpinZRot->EnableSpinButtons(-360, 360);
		m_pSectionSpinZRot->SetEditText("0");
		m_iSectionRot[2] = 0;

		m_pWaterlineSpinZPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Z));
		m_pWaterlineSpinZPos->EnableSpinButtons(-100000000, 100000000);
		m_pWaterlineSpinZPos->SetEditText("0");
		m_iWaterLinePos[2] = 0;

		m_pWaterlineSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_X));
		m_pWaterlineSpinXRot->EnableSpinButtons(-360, 360);
		m_pWaterlineSpinXRot->SetEditText("0");
		m_iWaterLineRot[0] = 0;
		m_pWaterlineSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Y));
		m_pWaterlineSpinYRot->EnableSpinButtons(-360, 360);
		m_pWaterlineSpinYRot->SetEditText("0");
		m_iWaterLineRot[1] = 0;
	}

	m_DlgProgress = new CDlgProgress();
	m_DlgProgress->Create(IDD_DIALOG_PROGRESS);

	OnButtonzoomall();
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
			TCollection_AsciiString aFileName((const char*)dlg.GetPathName());
			IGESControl_Reader Reader;
			Standard_Integer status = Reader.ReadFile(aFileName.ToCString());
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

					ClearSections();
					SetCurrentStep(1);
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

				ClearSections();
				SetCurrentStep(1);
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
		//		temp_string.Format("%g", bbHull.xMin() / 1000.0f);
		//		m_pEditStart->SetEditText(temp_string);

		//		temp_string.Format("%g", bbHull.xMax() / 1000.0f);
		//		m_pEditEnd->SetEditText(temp_string);

		//		//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 / 1000.0f);
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
			osg::Geometry* geo = CurveToGeometry(s, m_fEdgeDeflection);
			if (geo)
			{
				geode->addChild(geo);
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
	mOSG->OnViewIso();
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
	//if (sel_type == OPT_PATH || sel_type == OPT_TAG || sel_type == OPT_FRAME)
	//{
	//	osgUtil::PolytopeIntersector::Intersections intersections;
	//	y = mOSG->getViewer()->getCamera()->getViewport()->height() - y;
	//	double w = 5.0f;
	//	double h = 5.0f;
	//	osg::ref_ptr< osgUtil::PolytopeIntersector > picker = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x - w, y - h, x + w, y + h);
	//	osgUtil::IntersectionVisitor iv(picker.get());
	//	mOSG->getViewer()->getCamera()->accept(iv);
	//	if (picker->containsIntersections())
	//	{
	//		intersections = picker->getIntersections();
	//		//if (view->computeIntersections(x,y,intersections))
	//		//{
	//		for (osgUtil::PolytopeIntersector::Intersections::iterator hitr = intersections.begin();
	//			hitr != intersections.end();
	//			++hitr)
	//		{
	//			const osg::NodePath& nodePath = hitr->nodePath;
	//			unsigned int idx = nodePath.size();
	//			while (idx--){
	//				osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(nodePath[idx]);
	//				if (mt == NULL)
	//					continue;

	//				if (m_mapGeoToEntity2.find(mt) != m_mapGeoToEntity2.end())
	//				{
	//					if (m_mapGeoToEntity2[mt]->GetType() == sel_type || OPT_ELEMENT == sel_type)
	//					{
	//						return m_mapGeoToEntity2[mt];
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//{
		osgUtil::LineSegmentIntersector::Intersections intersections;
		y = mOSG->getViewer()->getCamera()->getViewport()->height() - y;
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

	osg::LineWidth* lineWidth = new osg::LineWidth(2);
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

	if (m_pEditStart == NULL)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if (pFrame)
		{
			m_pEditStart = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START));
			m_pEditEnd = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END));
			m_pEditSpace = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE));
			//m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));
			m_pEditPointsDistance = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE));
			m_pEditPointsNumber = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER));

			m_pEditStartWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START_WATERLINE));
			m_pEditEndWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END_WATERLINE));
			m_pEditSpaceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE_WATERLINE));
			m_pEditPointsDistanceWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_DISTANCE_WATERLINE));
			m_pEditPointsNumberWaterline = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_NUMBER_WATERLINE));
		}
	}

	if (m_pEditStart)
	{
		CString temp_string;
		m_aSectionStart.clear();
		m_aSectionEnd.clear();
		m_aSectionOffset.clear();

		temp_string = m_pEditStart->GetEditText();
		m_aSectionStart.push_back(atof(temp_string) * 1000.0f);

		temp_string = m_pEditEnd->GetEditText();
		m_aSectionEnd.push_back(atof(temp_string) * 1000.0f);

		temp_string = m_pEditSpace->GetEditText();
		m_aSectionOffset.push_back(atof(temp_string) * 1000.0f);

		ClearSections();
		DefineSections();
	}
}

void CIRES2View::ClearSections()
{
	for each(auto section in osgSections)
	{
		PreFrameUpdateData pf(mOSG->mRoot, section);
		m_QRemoveChild.push(pf);
	}
	for each(auto section in osgSectionsData)
	{
		PreFrameUpdateData pf(mOSG->mRoot, section);
		m_QRemoveChild.push(pf);
	}

	osgSections.clear();
	osgSectionsData.clear();
	osgSectionPosList.clear();
	osgSectionEnable.clear();

	ClearSectionPoints();
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

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr = new osg::MatrixTransform;
				osgSections.push_back(base_plane_tr);

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr1 = new osg::MatrixTransform;
				osgSectionsData.push_back(base_plane_tr1);

				osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
				base_plane_tr->addChild(base_plane);

				osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
				osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
				v_array->push_back(osg::Vec3(0, -bbLength[1], -bbLength[2]));
				v_array->push_back(osg::Vec3(0, bbLength[1], -bbLength[2]));
				v_array->push_back(osg::Vec3(0, bbLength[1], bbLength[2]));

				v_array->push_back(osg::Vec3(0, -bbLength[1], -bbLength[2]));
				v_array->push_back(osg::Vec3(0, bbLength[1], bbLength[2]));
				v_array->push_back(osg::Vec3(0, -bbLength[1], bbLength[2]));

				osg::Matrix tr;
				osg::Quat q(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
				tr.setTrans(center_pnt);
				tr.setRotate(q);
				base_plane_tr->setMatrix(tr);

				osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
				n_array->push_back(osg::Vec3(1, 0, 0));

				base_palne_geo->setVertexArray(v_array);
				osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
				cross_color->push_back(osg::Vec4(1, 1, 0.5, 0.5));
				base_palne_geo->setColorArray(cross_color.get());
				base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
				base_palne_geo->setNormalArray(n_array.get());
				base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

				osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
				de->push_back(0);
				de->push_back(1);
				de->push_back(2);
				de->push_back(3);
				de->push_back(4);
				de->push_back(5);
				base_palne_geo->addPrimitiveSet(de.get());

				osg::StateSet *ss = base_palne_geo->getOrCreateStateSet();
				ss->setMode(GL_BLEND, osg::StateAttribute::ON);
				osg::ref_ptr<osg::Depth> depth = new osg::Depth;
				depth->setWriteMask(true);
				ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
				ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				ss->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

				base_plane->addDrawable(base_palne_geo);

				sprintf_s(label_txt, 255, "%.2lf", center_pnt.x() / 1000.0f);
				osgText::Text* text = new  osgText::Text;
				osgText::Font* normal_font = osgText::readFontFile("fonts/arial.ttf");
				text->setFont(normal_font);
				text->setText(label_txt);
				text->setPosition(osg::Vec3(0, -bbLength[1], bbLength[2]));
				text->setCharacterSize(15.0f);
				text->setFontResolution(20, 20);
				text->setAutoRotateToScreen(true);
				text->setColor(osg::Vec4(0, 0, 0, 1));
				text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
				text->setAlignment(osgText::Text::CENTER_CENTER);
				base_plane->addDrawable(text);

				PreFrameUpdateData pf(mOSG->mRoot, base_plane_tr);
				m_QAddChild.push(pf);
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

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr = new osg::MatrixTransform;
				osgSections.push_back(base_plane_tr);

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr1 = new osg::MatrixTransform;
				osgSectionsData.push_back(base_plane_tr1);

				osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
				base_plane_tr->addChild(base_plane);

				osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
				osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
				v_array->push_back(osg::Vec3(-bbLength[0], 0, -bbLength[2]));
				v_array->push_back(osg::Vec3(bbLength[0], 0, -bbLength[2]));
				v_array->push_back(osg::Vec3(bbLength[0], 0, bbLength[2]));

				v_array->push_back(osg::Vec3(-bbLength[0], 0, -bbLength[2]));
				v_array->push_back(osg::Vec3(bbLength[0], 0, bbLength[2]));
				v_array->push_back(osg::Vec3(-bbLength[0], 0, bbLength[2]));

				osg::Matrix tr;
				osg::Quat q(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
				tr.setTrans(center_pnt);
				tr.setRotate(q);
				base_plane_tr->setMatrix(tr);

				osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
				n_array->push_back(osg::Vec3(0, 1, 0));

				base_palne_geo->setVertexArray(v_array);
				osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
				cross_color->push_back(osg::Vec4(1, 1, 0.5, 0.5));
				base_palne_geo->setColorArray(cross_color.get());
				base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
				base_palne_geo->setNormalArray(n_array.get());
				base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

				osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
				de->push_back(0);
				de->push_back(1);
				de->push_back(2);
				de->push_back(3);
				de->push_back(4);
				de->push_back(5);
				base_palne_geo->addPrimitiveSet(de.get());

				osg::StateSet *ss = base_palne_geo->getOrCreateStateSet();
				ss->setMode(GL_BLEND, osg::StateAttribute::ON);
				osg::ref_ptr<osg::Depth> depth = new osg::Depth;
				depth->setWriteMask(true);
				ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
				ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				ss->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

				base_plane->addDrawable(base_palne_geo);

				sprintf_s(label_txt, 255, "%.2lf", center_pnt.y() / 1000.0f);
				osgText::Text* text = new  osgText::Text;
				osgText::Font* normal_font = osgText::readFontFile("fonts/arial.ttf");
				text->setFont(normal_font);
				text->setText(label_txt);
				text->setPosition(osg::Vec3(bbLength[0], 0, bbLength[2]));
				text->setCharacterSize(15.0f);
				text->setFontResolution(20, 20);
				text->setAutoRotateToScreen(true);
				text->setColor(osg::Vec4(0, 0, 0, 1));
				text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
				text->setAlignment(osgText::Text::CENTER_CENTER);
				base_plane->addDrawable(text);

				PreFrameUpdateData pf(mOSG->mRoot, base_plane_tr);
				m_QAddChild.push(pf);
				PreFrameUpdateData pf1(mOSG->mRoot, base_plane_tr1);
				m_QAddChild.push(pf1);
				//mOSG->mRoot->addChild(base_plane_tr);
				//mOSG->mRoot->addChild(base_plane_tr1);
			}
		}
	}

	SetCurrentStep(2);
	ClearSectionPoints();
}


void CIRES2View::OnCheckBowBreaking()
{
	m_bBowBreaking = true;
	if (m_bBowBreaking)
		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
	else
		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));

	osg::Vec3 diff(0, 0, - bbLength[2] * 2.0f);
	for (int i = 0; i < osgSections.size(); i++)
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
		osgSections[i]->setMatrix(m);
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
	for (int i = 0; i < osgSections.size(); i++)
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
		osgSections[i]->setMatrix(m);
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
								if ((pd.pnt.z() / 1000.0f <= m_fDraftValue) && ((use_start_end == false) || (pd.pnt.x() <= start_pos && pd.pnt.x() >= end_pos)))
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
						float current_x = floor(section_line[j][0].x() / 1000.0f) * 1000.0f + point_distance;
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
								if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
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
								if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
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
								if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
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
								if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
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
						float current_z = floor(section_line[j][0].z() / 1000.0f) * 1000.0f - point_distance;
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
								if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
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
		v_array->push_back(pd.pnt + (pd.normal * 500.0f));
	}
	base_palne_geo->setVertexArray(v_array);
	osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
	cross_color->push_back(osg::Vec4(1, 0, 0, 1));
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

void CIRES2View::ClearSectionPoints()
{
	m_aWaterLine.clear();
	m_aWaterLinePointData.clear();

	m_aSectionLine.clear();
	m_aSectionPointDataList.clear();

	PreFrameUpdateData pf(osgWaterlineSection, NULL);
	m_QRemoveChild.push(pf);

	for each(auto section in osgSectionsData)
	{
		PreFrameUpdateData pf(section, NULL);
		m_QRemoveChild.push(pf);
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
	if (m_bUseDistanceForAxis)
	{
		if (m_pEditPointsDistance)
		{
			CString temp_string;
			temp_string = m_pEditPointsDistance->GetEditText();
			points_gap = atof(temp_string) * 1000.0f;
		}
	}
	else
	{
		if (m_pEditPointsNumber)
		{
			CString temp_string;
			temp_string = m_pEditPointsNumber->GetEditText();
			points_gap = atof(temp_string) * 1000.0f;
		}
	}

	if (m_bUseDistanceForAxisWaterline)
	{
		if (m_pEditPointsDistanceWaterline)
		{
			CString temp_string;
			temp_string = m_pEditPointsDistanceWaterline->GetEditText();
			points_gap_waterline = atof(temp_string) * 1000.0f;
		}
	}
	else
	{
		if (m_pEditPointsNumberWaterline)
		{
			CString temp_string;
			temp_string = m_pEditPointsNumberWaterline->GetEditText();
			points_gap_waterline = atof(temp_string) * 1000.0f;
		}
	}

	if (m_pEditStartWaterline)
	{
		CString temp_string;
		temp_string = m_pEditStartWaterline->GetEditText();
		m_fWaterlineStartPos = atof(temp_string) * 1000.0f;
	}
	if (m_pEditEndWaterline)
	{
		CString temp_string;
		temp_string = m_pEditEndWaterline->GetEditText();
		m_fWaterlineEndPos = atof(temp_string) * 1000.0f;
	}

	if (m_pEditSpaceWaterline)
	{
		CString temp_string;
		temp_string = m_pEditSpaceWaterline->GetEditText();
		m_fDraftValue = atof(temp_string);
	}

	//if (m_pEditPointsGap)
	//{
	//	CString temp_string;
	//	temp_string = m_pEditPointsGap->GetEditText();
	//	points_gap = atof(temp_string) * 1000.0f;
	//}

	if (osgHull->getNumChildren() < 1)
	{
		AfxMessageBox("Import HULL data first.");
		CalculateOutputResult(false);
		return;
	}
	else if (osgSectionPosList.size() < 1)
	{
		AfxMessageBox("Define Sections first.");
		CalculateOutputResult(false);
		return;
	}

	fopen_s(&fp_4, m_strAppPath + "\\ICE_INPUT.inp", "rt");
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

	ClearSectionPoints();

	m_aWaterLine.clear();
	m_aSectionLine.clear();

	//fopen_s(&FileLog, "d:\log.txt", "wt");
	m_iTotal = (osgSectionPosList.size() + 1) * 100;
	m_iStatus = 0;
	m_strStatus.Format("Calculate Section Points : Waterline");
	BeginProgress();

	osg::Matrix m;
	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iWaterLinePos);
	m.setRotate(q);
	osg::Vec3 n(0, 0, 1);
	n = m.preMult(n) - m_iWaterLinePos;

	CalculateSectionWaterline(n, m_iWaterLinePos, 0, m_aWaterLinePointData, m_bUseDistanceForAxisWaterline, points_gap_waterline, m_aWaterLine, true, m_fWaterlineStartPos, m_fWaterlineEndPos);
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
	SetCurrentStep(3);
}

void CIRES2View::CalculateOutputResult(bool refresh)
{
	//S_N.clear();
	//N_BETA.clear();
	//Y_VAL_ST.clear();
	//Z_VAL_ST.clear();
	//BETA_ST.clear();
	//X_COOR.clear();
	//Y.clear();
	//Z_COOR.clear();
	//X_NORM.clear();
	//Y_NORM.clear();
	//Z_NORM.clear();
	//ALPHA.clear();
	//BETA.clear();
	//GAMMA.clear();

	if (refresh)
	{
		//	결과 파일 저장
		FILE* save_file;
		if (m_aWaterLinePointData.size() > 0)
		{
			fopen_s(&save_file, m_strAppPath + "\\WATERLINE_OUTSIDE.inp", "wt");
			if (save_file)
			{
				fprintf_s(save_file, "%d\n", m_aWaterLinePointData.size());
				for (int i = 0; i < m_aWaterLinePointData.size(); i++)
				{
					fprintf_s(save_file, "   %.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\n",
						m_aWaterLinePointData[i].pnt.x() / 1000.0f,
						m_aWaterLinePointData[i].pnt.y() / 1000.0f,
						m_aWaterLinePointData[i].pnt.z() / 1000.0f,
						m_aWaterLinePointData[i].normal.x(),
						m_aWaterLinePointData[i].normal.y(),
						m_aWaterLinePointData[i].normal.z(),
						m_aWaterLinePointData[i].angle_alpha,
						m_aWaterLinePointData[i].angle_beta,
						m_aWaterLinePointData[i].angle_gamma);
					if (i == 0)
					{
						max_y = m_aWaterLinePointData[i].pnt.y() / 1000.0f;
					}
					else
					{
						float current_y = m_aWaterLinePointData[i].pnt.y() / 1000.0f;
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

			fopen_s(&save_file, m_strAppPath + "\\FRAME.inp", "wt");
			if (save_file)
			{
				fprintf_s(save_file, "%d\n", m_aSectionPointDataList.size());
				for (int i = 0; i < m_aSectionPointDataList.size(); i++)
				{
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   %.3lf   %d\n", m_aSectionPointDataList[i][0].pnt.x() / 1000.0f, m_aSectionPointDataList[i].size());
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %.3lf", abs(m_aSectionPointDataList[i][j].pnt.y()) / 1000.0f);
					}
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %.3lf", m_aSectionPointDataList[i][j].pnt.z() / 1000.0f);
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
	fopen_s(&fp_4, m_strAppPath + "\\ICE_INPUT.inp", "rt");
	fopen_s(&fp_6, m_strAppPath + "\\ECHO.OUT", "wt");
	fopen_s(&fp_7, m_strAppPath + "\\ice_result.OUT", "wt");
	fopen_s(&fp_8, m_strAppPath + "\\IMSI.OUT", "wt");
	fopen_s(&fp_9, m_strAppPath + "\\ICECOFF_INPUT.inp", "rt");
	fopen_s(&fp_10, m_strAppPath + "\\SELECT MODULE.INP", "rt");

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

	WRITE_OUT();

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

	ShellExecute(NULL, "open", m_strAppPath + "\\ice_result.OUT", NULL, NULL, SW_SHOW);
}

void CIRES2View::CAL_COND()
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

	fopen_s(&fp_9, m_strAppPath + "\\ICECOFF_INPUT.inp", "rt");
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

	RHO0 = RHO - RHOL;

	PI3 = acos(-1.0f);
	float SV = (VE - VS) / VI + 1.0f;
	if (VS == VE) SV = 1.0f;
	NV = (int)SV;

	if (fp_8)
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
		if (fp_8)
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


	if (fp_6)
	{
		fprintf_s(fp_6, "     SPEED            = %lf  Knots \n", VS);
		fprintf_s(fp_6, "     BREADTH          = %lf\n", BREADTH);
		fprintf_s(fp_6, "     FRICTIONAL COEFF.= %e\n", FG);
	}
}

void CIRES2View::READ_HULL(int ID)
{
	//	READ_HULL
	if (ID == 1)
	{
		fopen_s(&fp_5, m_strAppPath + "\\WATERLINE_OUTSIDE.inp", "rt");
	}
	else if (ID == 2)
	{
		fopen_s(&fp_5, m_strAppPath + "\\WATERLINE_INSIDE.inp", "rt");
	}
	else if (ID == 3)
	{
		fopen_s(&fp_5, m_strAppPath + "\\WATERLINE_MIDDLE.inp", "rt");
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
					X_COOR[i+1] = (atof(ifp.m_array_strOutput[0]));
					Y[i+1] = (atof(ifp.m_array_strOutput[1]));
					Z_COOR[i+1] = (atof(ifp.m_array_strOutput[2]));
					X_NORM[i+1] = (atof(ifp.m_array_strOutput[3]));
					Y_NORM[i+1] = (atof(ifp.m_array_strOutput[4]));
					Z_NORM[i+1] = (atof(ifp.m_array_strOutput[5]));
					ALPHA[i+1] = (atof(ifp.m_array_strOutput[6]));
					BETA[i+1] = (atof(ifp.m_array_strOutput[7]));
					GAMMA[i+1] = (atof(ifp.m_array_strOutput[8]));
				}
			}

			//N_FRAME = X_COOR.size();
		}
		ifp.m_fp_input = NULL;
	}

	fprintf_s(fp_6, "THE NUMBER OF FRAMES = %d\n", N_FRAME);
	fprintf_s(fp_6, "X_COOR(I)        Y(I)    Z_COOR(I)    X_NORM(I)    Y_NORM(I)    Z_NORM(I)     ALPHA(I)      BETA(I)     GAMMA(I)\n");
	for (int I = 1; I <= N_FRAME; I++)
	{
		fprintf_s(fp_6, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
			X_COOR[I], Y[I], Z_COOR[I], X_NORM[I], Y_NORM[I], Z_NORM[I], ALPHA[I], BETA[I], GAMMA[I]);

		ALPHA[I] = ALPHA[I] / (180 / PI3);
		BETA[I] = BETA[I] / (180 / PI3);
		GAMMA[I] = GAMMA[I] / (180 / PI3);
	}

	X_COOR[N_FRAME + 1] = 2.0f * X_COOR[N_FRAME] - X_COOR[N_FRAME - 1];
	Y[N_FRAME + 1] = Y[N_FRAME];
	Z_COOR[N_FRAME + 1] = Z_COOR[N_FRAME];
	X_NORM[N_FRAME + 1] = X_NORM[N_FRAME];
	Y_NORM[N_FRAME + 1] = Y_NORM[N_FRAME];
	Z_NORM[N_FRAME + 1] = Z_NORM[N_FRAME];
	ALPHA[N_FRAME + 1] = ALPHA[N_FRAME];
	BETA[N_FRAME + 1] = BETA[N_FRAME];
	GAMMA[N_FRAME + 1] = GAMMA[N_FRAME];

	fclose(fp_5);
	fp_5 = NULL;
	vector< float > element(10);

	for (int II = 1; II <= N_FRAME; II++)
	{
		float TEM_I = min(1.0f, max(-1.0f, (Z_NORM[II] * Z_NORM[II] + Y_NORM[II] * Y_NORM[II])));
		float TEM_J = min(1.0f, max(-1.0f, (X_NORM[II] * Y_NORM[II])));
		ALPHA[II] = abs(acos(TEM_I));
		fprintf_s(fp_6, "   %d   %lf   %lf\n", II, ALPHA[II] * 180 / PI3, BETA[II] * 180 / PI3);
	}

	if (ID == 1)
	{
		fopen_s(&fp_15, m_strAppPath + "\\FRAME.inp", "rt");
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
				R_BR[IH][IS] = (R_Lamda * SIGMA[IS] * THCK[IH] * THCK[IH] / (1.93 * Eta_1)) * Y[I];
			}
		}
	}
}
void CIRES2View::CLEARING1()
{
	//YH.resize(N_FRAME + 10);
	for (int I = 1; I <= N_FRAME; I++)
	{
		YH[I] = tan(ALPHA[I]);
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
			B13 = 2.0 * RHOL * GG * THCK[IH] * BREADTH * FROUD[IVS];
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
					if (Y_VAL_ST[KK][I3] >= BREADTH / 2.)
					{
						DIST_ICE[KK][I3] = 0.;
					}
				}
				GIRTH_LENGTH[KK] = GIRTH_LENGTH[KK] + DIST_ICE[KK][I3];
				N_END_GIRTH[KK] = I3 - 1;
				if (GIRTH_LENGTH[KK] >= BREADTH / 2.)
				{
					N_END_GIRTH[KK] = I3 - 1;
					//break;
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

		R_BO[IH] = (R_SP_TOTAL + R_SF_TOTAL) * 2;
	}
}

float ALP(float X, int II)
{
	float A = X_COOR[1] - X_COOR[II];
	float B = X_COOR[1] - X_COOR[II+1];
	float DXX = X_COOR[II] - X_COOR[II + 1];
	return (ALPHA[II] * (B - X) + ALPHA[II + 1] * (X - A)) / DXX;
}

float BT(float X, int II)
{
	float A = X_COOR[1] - X_COOR[II];
	float B = X_COOR[1] - X_COOR[II+1];
	float DXX = X_COOR[II] - X_COOR[II+1];
	return (BETA[II] * (B - X) + BETA[II + 1] * (X - A)) / DXX;
}

float YY(float X, int II)
{
	float A = X_COOR[1] - X_COOR[II];
	float B = X_COOR[1] - X_COOR[II + 1];
	float DXX = X_COOR[II] - X_COOR[II + 1];
	return (Y[II] * (B - X) + Y[II + 1] * (X - A)) / DXX;
}

float YYH(float X, int II)
{
	float A = X_COOR[1] - X_COOR[II];
	float B = X_COOR[1] - X_COOR[II + 1];
	float DXX = X_COOR[II] - X_COOR[II + 1];
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
	float W[] = {0,  .2955242247, .2692667193, .2190863625, .1494513491, .0666713443 };
	float X[] = {0,  .1488743389, .4333953941, .6794095682, .8650633666, .9739065285 };

	float A = X_COOR[1] - X_COOR[II];
	float B = X_COOR[1] - X_COOR[II+1];
	float XM = 0.5*(B + A);
	float XR = 0.5*(B - A);
	float SS = 0;
	float DX = 0;
	switch (IOP)
	{
	case 1:
	{
			  for (int J = 1; J <= 5; J++)
			  {
				  DX = XR*X[J];
				  SS = SS + W[J] * (FFX(XM + DX, II) + FFX(XM - DX, II));
			  }
	}
		break;
	case 2:
	{
			  for (int J = 1; J <= 5; J++)
			  {
				  DX = XR*X[J];
				  SS = SS + W[J] * (FY(XM + DX, II) + FY(XM - DX, II));
			  }
	}
		break;
	case 3:
	{
			  for (int J = 1; J <= 5; J++)
			  {
				  DX = XR*X[J];
				  SS = SS + W[J] * (FFZ(XM + DX, II) + FFZ(XM - DX, II));
			  }
	}
		break;
	case 15:
	{
			   for (int J = 1; J <= 5; J++)
			   {
				   DX = XR*X[J];
				   SS = SS + W[J] * (FB5(XM + DX, II) + FB5(XM - DX, II));
			   }
	}
		break;
	case 16:
	{
			   for (int J = 1; J <= 5; J++)
			   {
				   DX = XR*X[J];
				   SS = SS + W[J] * (FB6(XM + DX, II) + FB6(XM - DX, II));
			   }
	}
		break;
	}

	SS = XR*SS;
	return SS;
}

void CIRES2View::WRITE_OUT()
{
	float R_TOTAL;
	fprintf_s(fp_7, "   Vs(kts)     Hi(m) sigf(kPa)  R_br(kN)  R_cl(kN)  R_bu(kN)   R_i(kN)\n");
	for (int IV = 1; IV <= NV; IV++)
	{
		for (int IS = 1; IS <= NSIGMA; IS++)
		{
			for (int IH = 1; IH <= NH; IH++)
			{
				fprintf_s(fp_8, " IV = %d   SIGMA = %d   THICK = %d\n", IV, IS, IH);
				R_TOTAL = R_BREAK[IH][IS] + R_CLEAR[IH][IV] + R_BOUYA[IH];
				fprintf_s(fp_7, "%9.2lf%10.2lf%10.2lf%10.1lf%10.1lf%10.1lf%10.1lf\n",
					VSP[IV], THCK[IH], SIGMA[IS] / 1000.0f, R_BREAK[IH][IS] / 1000.0f, R_CLEAR[IH][IV] / 1000.0f, R_BOUYA[IH] / 1000.0f, R_TOTAL / 1000.0f);
			}
		}
	}
}

void CIRES2View::OnButtonShowHideSections()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_bShowSection)
	{
		for (int i = 0; i < osgSections.size(); i++)
		{
			PreFrameUpdateData pf(mOSG->mRoot, osgSections[i]);
			m_QRemoveChild.push(pf);
			//mOSG->mRoot->removeChild(osgSections[i]);
		}
		m_bShowSection = false;
	}
	else
	{
		for (int i = 0; i < osgSections.size(); i++)
		{
			PreFrameUpdateData pf(mOSG->mRoot, osgSections[i]);
			m_QAddChild.push(pf);
			//mOSG->mRoot->addChild(osgSections[i]);
		}
		m_bShowSection = true;
	}
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


void CIRES2View::OnButtonHideShowWaterlines()
{
	UnSetFlipNormal();
	UnSetCenterPoint();

	if (m_bShowWaterline)
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgWaterline);
		m_QRemoveChild.push(pf);
		//mOSG->mRoot->removeChild(osgWaterline);
		m_bShowWaterline = false;
	}
	else
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgWaterline);
		m_QAddChild.push(pf);
		//mOSG->mRoot->addChild(osgWaterline);
		m_bShowWaterline = true;
	}
}


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


void CIRES2View::OnUpdateButtonShowHideSections(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSection);
}


void CIRES2View::OnUpdateButtonShowHideSectionCut(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSectionData);
}


void CIRES2View::OnUpdateButtonHideShowWaterlines(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowWaterline);
}


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

void CIRES2View::PreFrameUpdate()
{
	//mOSG->ResizeToolbar(screen_width, screen_height);

	while (!m_QRemoveChild.empty())
	{
		PreFrameUpdateData pd = m_QRemoveChild.front();
		if (pd.parent_node != NULL)
		{
			if (pd.child_node != NULL)
			{
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
		if(pd.parent_node != NULL && pd.child_node != NULL)
			pd.parent_node->addChild(pd.child_node);
		m_QAddChild.pop();
		
		if (pd.parent_node == osgHull)
		{
			//bbHull.expandBy(osgHull->getBound());
			//bbHullRadius = osgHull->getBound().radius();
			osg::ComputeBoundsVisitor cbbv;
			osgHull_Center->accept(cbbv);
			bbHull = cbbv.getBoundingBox();

			//CString temp_string;
			//temp_string.Format("%lf, %lf, %lf, %lf, %lf, %lf", bbHull.xMin(), bbHull.xMax(), bbHull.yMin(), bbHull.yMax(), bbHull.zMin(), bbHull.zMax());
			//AfxMessageBox(temp_string);

			int child_no = osgWaterline->getNumChildren();
			if (child_no > 0)
			{
				osgWaterline->removeChildren(0, child_no);
			}

			osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;

			osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
			osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
			//v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
			//v_array->push_back(osg::Vec3(bbHullRadius, -bbHullRadius, 0));
			//v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));

			//v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
			//v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));
			//v_array->push_back(osg::Vec3(-bbHullRadius, bbHullRadius, 0));

			bbLength[0] = (bbHull.xMax() - bbHull.xMin()) * 0.6f;
			bbLength[1] = (bbHull.yMax() - bbHull.yMin()) * 0.6f;
			bbLength[2] = (bbHull.zMax() - bbHull.zMin()) * 0.6f;
			v_array->push_back(osg::Vec3(-bbLength[0], -bbLength[1], 0));
			v_array->push_back(osg::Vec3(bbLength[0], -bbLength[1], 0));
			v_array->push_back(osg::Vec3(bbLength[0], bbLength[1], 0));

			v_array->push_back(osg::Vec3(-bbLength[0], -bbLength[1], 0));
			v_array->push_back(osg::Vec3(bbLength[0], bbLength[1], 0));
			v_array->push_back(osg::Vec3(-bbLength[0], bbLength[1], 0));

			m_iWaterLinePos.set(bbHull.center().x(), bbHull.center().y(), bbHull.center().z());
			//UpdateWaterlinePos();

			osg::Matrix tr;
			tr.setTrans(m_iWaterLinePos);
			osgWaterline->setMatrix(tr);

			osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
			n_array->push_back(osg::Vec3(0, 0, 1));

			base_palne_geo->setVertexArray(v_array);
			osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
			cross_color->push_back(osg::Vec4(0.8, 0.8, 1.0, 0.5));
			base_palne_geo->setColorArray(cross_color.get());
			base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
			base_palne_geo->setNormalArray(n_array.get());
			base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

			osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
			de->push_back(0);
			de->push_back(1);
			de->push_back(2);
			de->push_back(3);
			de->push_back(4);
			de->push_back(5);
			base_palne_geo->addPrimitiveSet(de.get());

			osg::StateSet *ss = base_palne_geo->getOrCreateStateSet();
			ss->setMode(GL_BLEND, osg::StateAttribute::ON);
			osg::ref_ptr<osg::Depth> depth = new osg::Depth;
			depth->setWriteMask(true);
			ss->setAttributeAndModes(depth.get(), osg::StateAttribute::ON);
			ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			ss->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

			base_plane->addDrawable(base_palne_geo);

			//PreFrameUpdateData pf(osgWaterline, base_plane);
			//m_QAddChild.push(pf);
			osgWaterline->addChild(base_plane);

			UpdateGlobalAxis(max(max(bbHull.xMax(), bbHull.yMax()), bbHull.zMax()));

			SetTimer(1, 10, NULL);
			//if (m_pEditStart)
			//{
			//	CString temp_string;
			//	temp_string.Format("%g", bbHull.xMin() / 1000.0f);
			//	m_pEditStart->SetEditText(temp_string);

			//	temp_string.Format("%g", bbHull.xMax() / 1000.0f);
			//	m_pEditEnd->SetEditText(temp_string);

			//	//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 / 1000.0f);
			//	m_pEditSpace->SetEditText("0.5");
			//}

		}
	}
}

void CIRES2View::SetCurrentStep(int i_step)
{
	mOSG->m_widgetOPTType[m_iCurrentStep]->hide();
	m_iCurrentStep = i_step;
	mOSG->m_widgetOPTType[m_iCurrentStep]->show();
	mOSG->ResizeToolbar(screen_width, screen_height);
}

void CIRES2View::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (m_pEditStart)
		{
			CString temp_string;
			temp_string.Format("%g", bbHull.xMax() / 1000.0f);
			m_pEditStart->SetEditText(temp_string);
			m_pEditStartWaterline->SetEditText(temp_string);

			temp_string.Format("%g", bbHull.xMin() / 1000.0f);
			m_pEditEnd->SetEditText(temp_string);
			m_pEditEndWaterline->SetEditText(temp_string);

			//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 / 1000.0f);
			m_pEditSpace->SetEditText("0.5");

			UpdateWaterlinePos();
			
			FitWorld();
		}

		KillTimer(1);
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
		CalculateOutputResult(false);
		return;
	}

	CalculateOutputResult();
	SetCurrentStep(4);
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
	}
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


void CIRES2View::OnSpinHullXPos()
{
	CString str_x = m_pHULLSpinXPos->GetEditText();
	str_x.Replace(",", "");
	m_iHULLPos[0] = atof(str_x) * 1000.0f;
	osg::Matrix m;
	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iHULLPos);
	m.setRotate(q);
	osgHull_Center->setMatrix(m);
}


void CIRES2View::OnSpinHullYPos()
{
	CString str_x = m_pHULLSpinYPos->GetEditText();
	str_x.Replace(",", "");
	m_iHULLPos[1] = atof(str_x) * 1000.0f;
	osg::Matrix m;
	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iHULLPos);
	m.setRotate(q);
	osgHull_Center->setMatrix(m);
}


void CIRES2View::OnSpinHullZPos()
{
	CString str_x = m_pHULLSpinZPos->GetEditText();
	str_x.Replace(",", "");
	m_iHULLPos[2] = atof(str_x) * 1000.0f;
	osg::Matrix m;
	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iHULLPos);
	m.setRotate(q);
	osgHull_Center->setMatrix(m);
}


void CIRES2View::OnSpinHullXAngle()
{
	CString str_x = m_pHULLSpinXRot->GetEditText();
	str_x.Replace(",", "");
	m_iHULLRot[0] = osg::DegreesToRadians(atof(str_x));
	osg::Matrix m;
	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iHULLPos);
	m.setRotate(q);
	osgHull_Center->setMatrix(m);
}



void CIRES2View::OnSpinSectionX()
{
	CString str_x = m_pSectionSpinXRot->GetEditText();
	str_x.Replace(",", "");
	m_iSectionRot[0] = osg::DegreesToRadians(atof(str_x));
	if(m_bBowBreaking)
		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
	else
		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));

	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
	for (int i = 0; i < osgSections.size(); i++)
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
		osgSections[i]->setMatrix(m);
	}
}

void CIRES2View::OnSpinSectionY()
{
	CString str_x = m_pSectionSpinYRot->GetEditText();
	str_x.Replace(",", "");
	m_iSectionRot[1] = osg::DegreesToRadians(atof(str_x));
	if (m_bBowBreaking)
		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
	else
		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));

	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
	for (int i = 0; i < osgSections.size(); i++)
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
		osgSections[i]->setMatrix(m);
	}
}

void CIRES2View::OnSpinSectionZ()
{
	CString str_x = m_pSectionSpinZRot->GetEditText();
	str_x.Replace(",", "");
	m_iSectionRot[2] = osg::DegreesToRadians(atof(str_x));
	if (m_bBowBreaking)
		osgSectionRotation = osg::Quat(0, osg::Vec3(1, 0, 0), m_iSectionRot[1], osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));
	else
		osgSectionRotation = osg::Quat(m_iSectionRot[0], osg::Vec3(1, 0, 0), 0, osg::Vec3(0, 1, 0), m_iSectionRot[2], osg::Vec3(0, 0, 1));

	osg::Vec3 diff(0, 0, -bbLength[2] * 2.0f);
	for (int i = 0; i < osgSections.size(); i++)
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
		osgSections[i]->setMatrix(m);
	}
}


void CIRES2View::OnSpinWaterlineZ()
{
	CString str_z = m_pWaterlineSpinZPos->GetEditText();
	str_z.Replace(",", "");
	m_iWaterLinePos[2] = atof(str_z) * 1000.0f;

	osg::Matrix m;
	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iWaterLinePos);
	m.setRotate(q);
	osgWaterline->setMatrix(m);
}


void CIRES2View::OnSpinWaterlineX()
{
	CString str_z = m_pWaterlineSpinXRot->GetEditText();
	str_z.Replace(",", "");
	m_iWaterLineRot[0] = osg::DegreesToRadians(atof(str_z));

	osg::Matrix m;
	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iWaterLinePos);
	m.setRotate(q);
	osgWaterline->setMatrix(m);
}


void CIRES2View::OnSpinWaterlineY()
{
	CString str_z = m_pWaterlineSpinYRot->GetEditText();
	str_z.Replace(",", "");
	m_iWaterLineRot[1] = osg::DegreesToRadians(atof(str_z));

	osg::Matrix m;
	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iWaterLinePos);
	m.setRotate(q);
	osgWaterline->setMatrix(m);
}


void CIRES2View::OnSpinHullYAngle()
{
	CString str_x = m_pHULLSpinYRot->GetEditText();
	str_x.Replace(",", "");
	m_iHULLRot[1] = osg::DegreesToRadians(atof(str_x));
	osg::Matrix m;
	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iHULLPos);
	m.setRotate(q);
	osgHull_Center->setMatrix(m);
}


void CIRES2View::OnSpinHullZAngle()
{
	CString str_x = m_pHULLSpinZRot->GetEditText();
	str_x.Replace(",", "");
	m_iHULLRot[2] = osg::DegreesToRadians(atof(str_x));
	osg::Matrix m;
	osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iHULLPos);
	m.setRotate(q);
	osgHull_Center->setMatrix(m);
}


void CIRES2View::OnButtonHullPointToPoint()
{
	UnSetFlipNormal();

	PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
	m_QAddChild.push(pf);
	m_bSetCenterPoint = true;
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
	if (m_bSelectWindow)
	{
		SetSelectionWindow(m_ptStart, point);
	}
	else if (m_bSetCenterPoint)
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
	else if (m_bSetFlipNormal)
	{
		osg::Vec3d hit_pt;
		osg::Node* p_element = OnSelectPoint(point.x, point.y, hit_pt);
		if (m_SelectedGeo)
		{
			osg::Matrix m;
			m.setTrans(hit_pt);
			TRACE("%lf, %lf, %lf\n", hit_pt.x(), hit_pt.y(), hit_pt.z());
			osgSelectPoint->setMatrix(m);
		}
	}

	CView::OnMouseMove(nFlags, point);
}


void CIRES2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bSelectWindow)
	{
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
		mOSG->SetCruise(false);
		m_ptStart = point;
		SetSelectionWindow(m_ptStart, point);
		PreFrameUpdateData pf(mOSG->mRoot, m_cameraStatus);
		m_QAddChild.push(pf);
	}
	else if (m_bSetCenterPoint)
	{
		osg::Matrix current_tr = osgHull_Center->getMatrix();
		osg::Matrix inv_tr;
		inv_tr.invert(current_tr);
		osg::Vec3 diff = inv_tr.preMult(m_geoVertex);
		//osg::Vec3 diff = m_geoVertex - m_iHULLPos;
		m_iHULLPos = m_geoVertex;

		osg::Matrix m;
		osg::Quat q(m_iHULLRot[0], osg::Vec3(1, 0, 0), m_iHULLRot[1], osg::Vec3(0, 1, 0), m_iHULLRot[2], osg::Vec3(0, 0, 1));
		m.setTrans(m_iHULLPos);
		m.setRotate(q);
		osgHull_Center->setMatrix(m);

		m.makeIdentity();
		m.setTrans(osgHull->getMatrix().getTrans() - diff);
		osgHull->setMatrix(m);

		UpdateHullPos();

		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QRemoveChild.push(pf);
		m_bSetCenterPoint = false;
	}
	else if (m_bSetFlipNormal)
	{
		if (m_SelectedGeo)
		{
			osg::Vec3Array *vertices = (osg::Vec3Array *)m_SelectedGeo->getVertexArray();
			osg::Vec3Array *normals = (osg::Vec3Array *)m_SelectedGeo->getNormalArray();
			if (vertices != NULL && normals != NULL)
			{
				osg::Geometry::PrimitiveSetList primitiveList = m_SelectedGeo->getPrimitiveSetList();

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

				for (int i = 0; i < normals->size(); i++)
				{
					normals->at(i).set(-normals->at(i));
				}
				normals->dirty();

				for (int x = 0; x < primitiveList.size(); x++)
				{
					osg::PrimitiveSet *set = primitiveList[x];
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
				m_SelectedGeo->dirtyBound();
			}
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CIRES2View::UpdateHullPos()
{
	if (m_pHULLSpinXPos)
	{
		CString temp_string;
		temp_string.Format("%.2lf", m_iHULLPos[0] / 1000.0f);
		m_pHULLSpinXPos->SetEditText(temp_string);
		temp_string.Format("%.2lf", m_iHULLPos[1] / 1000.0f);
		m_pHULLSpinYPos->SetEditText(temp_string);
		temp_string.Format("%.2lf", m_iHULLPos[2] / 1000.0f);
		m_pHULLSpinZPos->SetEditText(temp_string);
	}
}

void CIRES2View::UpdateWaterlinePos()
{
	if (m_pWaterlineSpinZPos)
	{
		CString temp_string;
		temp_string.Format("%.2lf", m_iWaterLinePos[2] / 1000.0f);
		m_pWaterlineSpinZPos->SetEditText(temp_string);
		m_pEditSpaceWaterline->SetEditText(temp_string);
	}
}

void CIRES2View::OnButtonNurbsConversion()
{
	UnSetCenterPoint();

	if (m_bSetFlipNormal)
	{
		UnSetFlipNormal();
	}
	else
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QAddChild.push(pf);
		m_bSetFlipNormal = true;
	}
}


void CIRES2View::OnUpdateButtonNurbsConversion(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bSetFlipNormal);
}

void CIRES2View::UnSetCenterPoint()
{
	if (m_bSetCenterPoint)
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QRemoveChild.push(pf);
		m_bSetCenterPoint = false;
	}
}

void CIRES2View::UnSetFlipNormal()
{
	if (m_bSetFlipNormal)
	{
		PreFrameUpdateData pf(mOSG->mRoot, osgSelectPoint);
		m_QRemoveChild.push(pf);
		m_bSetFlipNormal = false;
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


void CIRES2View::OnButtonreset()
{
	FitWorld();
}


void CIRES2View::OnButtonzoomwin()
{
	m_bSelectWindow = true;
}


void CIRES2View::OnButtonzoomall()
{
	FitWorld();
}


void CIRES2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bSelectWindow)
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		mOSG->SetCruise(true);
		PreFrameUpdateData pf(mOSG->mRoot, m_cameraStatus);
		m_QRemoveChild.push(pf);
		m_bSelectWindow = false;
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


void CIRES2View::OnEditSpaceWaterline()
{
	CString str_z = m_pEditSpaceWaterline->GetEditText();
	str_z.Replace(",", "");
	m_iWaterLinePos[2] = atof(str_z) * 1000.0f;

	osg::Matrix m;
	osg::Quat q(m_iWaterLineRot[0], osg::Vec3(1, 0, 0), m_iWaterLineRot[1], osg::Vec3(0, 1, 0), m_iWaterLineRot[2], osg::Vec3(0, 0, 1));
	m.setTrans(m_iWaterLinePos);
	m.setRotate(q);
	osgWaterline->setMatrix(m);
}


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
