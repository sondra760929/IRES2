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
END_MESSAGE_MAP()

int N_FRAME;
int NS_S;
vector< float > S_N;
vector< int > N_BETA;
vector< vector< float > > Y_VAL_ST;
vector< vector< float > > Z_VAL_ST;
vector< vector< float > > BETA_ST;
vector< vector< float > > Sin_Beta;
vector< vector< float > > SLOPE_Y;
vector< vector< float > > SLOPE_Z;
vector< vector< float > > SLOPE;
vector< vector< float > > SLOPE_BETA_DEG;
vector< vector< float > > R_SP;
vector< vector< float > > Z_BUOY;
vector< vector< float > > Y_BUOY;
vector< vector< float > > DIST_ICE;
vector< vector< float > > R_SF;
vector< vector< float > > R_total;

vector< int > N_BUOY;
vector< float > Staion_length_Buoy;
vector< float > GIRTH_LENGTH;
vector< float > N_END_GIRTH;
vector< float > R_BO;

vector< float > X_COOR;
vector< float > Y;
vector< float > Z_COOR;
vector< float > X_NORM;
vector< float > Y_NORM;
vector< float > Z_NORM;
vector< float > ALPHA;
vector< float > BETA;
vector< float > GAMMA;
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
vector< float > VSP;
vector< float > VELOCI;
vector< float > FROUD;
vector< float > SIGMA;
vector< float > THCK;
vector< vector< float > > R_BR;
vector< vector< float > > R_CL;
vector< vector< float > > R_CLEAR;
vector< vector< float > > R_BREAK;
vector< float > R_BOUYA;
vector< vector< float > > DEPTH_BUOY;
vector< float > YH;
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
	, m_pSectionSpinYRot(NULL)
	, m_pSectionSpinZRot(NULL)
	, m_pWaterlineSpinZPos(NULL)
	, m_pWaterlineSpinXRot(NULL)
	, m_pWaterlineSpinYRot(NULL)
	, m_bBowBreaking(true)
{
	// TODO: 여기에 생성 코드를 추가합니다.

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

	return 0;
}


void CIRES2View::OnDestroy()
{
	CView::OnDestroy();

	delete mThreadHandle;
	if (mOSG != 0) delete mOSG;
}


void CIRES2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	mOSG->InitOSG();

	hints = new osg::TessellationHints;
	hints->setDetailRatio(0.5f);

	mThreadHandle = new CRenderingThread(mOSG);
	mThreadHandle->start();

	osgAxis = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgAxis);

	osgHull = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgHull);

	osgWaterline = new osg::MatrixTransform();
	mOSG->mRoot->addChild(osgWaterline);

	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pFrame)
	{
		m_pEditStart = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START));
		m_pEditEnd = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END));
		m_pEditSpace = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE));
		m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));

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
		m_pHULLSpinXRot->EnableSpinButtons(0, 360);
		m_pHULLSpinXRot->SetEditText("0");
		m_iHULLRot[0] = 0;
		m_pHULLSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Y_ANGLE));
		m_pHULLSpinYRot->EnableSpinButtons(0, 360);
		m_pHULLSpinYRot->SetEditText("0");
		m_iHULLRot[1] = 0;
		m_pHULLSpinZRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_HULL_Z_Angle));
		m_pHULLSpinZRot->EnableSpinButtons(0, 360);
		m_pHULLSpinZRot->SetEditText("0");
		m_iHULLRot[2] = 0;

		m_pSectionSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_Y));
		m_pSectionSpinYRot->EnableSpinButtons(0, 360);
		m_pSectionSpinYRot->SetEditText("0");
		m_iSectionRot[1] = 0;
		m_pSectionSpinZRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_SECTION_Z));
		m_pSectionSpinZRot->EnableSpinButtons(0, 360);
		m_pSectionSpinZRot->SetEditText("0");
		m_iSectionRot[2] = 0;

		m_pWaterlineSpinZPos = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Z));
		m_pWaterlineSpinZPos->EnableSpinButtons(-100000000, 100000000);
		m_pWaterlineSpinZPos->SetEditText("0");
		m_iWaterLinePos[2] = 0;

		m_pWaterlineSpinXRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Y));
		m_pWaterlineSpinXRot->EnableSpinButtons(0, 360);
		m_pWaterlineSpinXRot->SetEditText("0");
		m_iWaterLineRot[0] = 0;
		m_pWaterlineSpinYRot = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_SPIN_WATERLINE_Y));
		m_pWaterlineSpinYRot->EnableSpinButtons(0, 360);
		m_pWaterlineSpinYRot->SetEditText("0");
		m_iWaterLineRot[1] = 0;
	}
}


void CIRES2View::OnButtonImportHull()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"IGES Files (*.iges , *.igs)|*.iges; *.igs|All Files (*.*)|*.*||",
		NULL);

	if (dlg.DoModal() == IDOK)
	{
		time_t start_time;
		time(&start_time);
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		TCollection_AsciiString aFileName((const char*)dlg.GetPathName());
		IGESControl_Reader Reader;
		Standard_Integer status = Reader.ReadFile(aFileName.ToCString());
		time_t end_time;
		double diff_time1;
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
				osgHull->addChild(geode);

				osg::BoundingBox bbHull;
				bbHull.expandBy(osgHull->getBound());
				bbHullRadius = osgHull->getBound().radius();
				//osg::ComputeBoundsVisitor cbbv;
				//osgHull->accept(cbbv);
				//osg::BoundingBox bbHull = cbbv.getBoundingBox();

				//CString temp_string;
				//temp_string.Format("%lf, %lf, %lf, %lf, %lf, %lf", bbHull.xMin(), bbHull.xMax(), bbHull.yMin(), bbHull.yMax(), bbHull.zMin(), bbHull.zMax());
				//AfxMessageBox(temp_string);

				int child_no = osgWaterline->getNumChildren();
				if (child_no > 0)
				{
					osgWaterline->removeChildren(0, child_no);
				}

				osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
				osgWaterline->addChild(base_plane);

				osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
				osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
				v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
				v_array->push_back(osg::Vec3(bbHullRadius, -bbHullRadius, 0));
				v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));

				v_array->push_back(osg::Vec3(-bbHullRadius, -bbHullRadius, 0));
				v_array->push_back(osg::Vec3(bbHullRadius, bbHullRadius, 0));
				v_array->push_back(osg::Vec3(-bbHullRadius, bbHullRadius, 0));

				m_iWaterLinePos[0] = bbHull.center().x();
				m_iWaterLinePos[1] = bbHull.center().y();
				m_iWaterLinePos[2] = bbHull.center().z();

				osg::Matrix tr;
				tr.setTrans(bbHull.center());
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
				base_palne_geo->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
				base_plane->addDrawable(base_palne_geo);

				UpdateGlobalAxis(max(max(bbHull.xMax(), bbHull.yMax()), bbHull.zMax()));

				if (m_pEditStart)
				{
					CString temp_string;
					temp_string.Format("%g", bbHull.xMin() / 1000.0f);
					m_pEditStart->SetEditText(temp_string);

					temp_string.Format("%g", bbHull.xMax() / 1000.0f);
					m_pEditEnd->SetEditText(temp_string);

					//temp_string.Format("%g", (m_fBoundingSize[0][1] - m_fBoundingSize[0][0]) / 10.0 / 1000.0f);
					m_pEditSpace->SetEditText("0.5");
				}

			}
		}
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		FitWorld();

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
				if (m_SelectedGeo)
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
	//geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

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

	geode->addDrawable(mOSG->createAxisLabel("X", osg::Vec3(length + 10, 0, 0)));
	geode->addDrawable(mOSG->createAxisLabel("Y", osg::Vec3(0, length + 10, 0)));
	geode->addDrawable(mOSG->createAxisLabel("Z", osg::Vec3(0, 0, length + 10)));

	int count = osgAxis->getNumChildren();
	if (count > 0)
	{
		osgAxis->removeChildren(0, count);
	}

	osgAxis->addChild(geode);
}

void CIRES2View::OnButtonDefineSections()
{
	if (m_pEditStart == NULL)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if (pFrame)
		{
			m_pEditStart = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_START));
			m_pEditEnd = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_END));
			m_pEditSpace = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_SPACE));
			m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));
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

		DefineSections();
	}
}

void CIRES2View::DefineSections()
{
	//RemoveSection();
	//RemoveSectionPoints();

	char label_txt[255];

	for (int i = 0; i < m_aSectionStart.size(); i++)
	{
		float center_x;
		float center_y = bbHull.center().y();
		float center_z = bbHull.center().z();
		if (m_bBowBreaking)
		{
			center_x = m_aSectionStart[i];
			float prev_x = center_x;
			while (center_x < m_aSectionEnd[i])
			{
				osg::Vec3 center_pnt(center_x, center_y, center_z);
				osgSectionPosList.push_back(center_pnt);

				//osg::Plane section_plane(osg::Vec3(1, 0, 0), osg::Vec3(0, 0, 0));

				osg::ref_ptr<osg::MatrixTransform> base_plane_tr = new osg::MatrixTransform;
				osgSections.push_back(base_plane_tr);
				mOSG->mRoot->addChild(base_plane_tr);

				osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
				base_plane_tr->addChild(base_plane);

				osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
				osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
				v_array->push_back(osg::Vec3(0, -bbHullRadius, -bbHullRadius));
				v_array->push_back(osg::Vec3(0, bbHullRadius, -bbHullRadius));
				v_array->push_back(osg::Vec3(0, bbHullRadius, bbHullRadius));

				v_array->push_back(osg::Vec3(0, -bbHullRadius, -bbHullRadius));
				v_array->push_back(osg::Vec3(0, bbHullRadius, bbHullRadius));
				v_array->push_back(osg::Vec3(0, -bbHullRadius, bbHullRadius));

				m_iWaterLinePos[0] = bbHull.center().x();
				m_iWaterLinePos[1] = bbHull.center().y();
				m_iWaterLinePos[2] = bbHull.center().z();

				osg::Matrix tr;
				tr.setTrans(center_pnt);
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
				base_palne_geo->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
				base_plane->addDrawable(base_palne_geo);

				osgSections.push_back(base_plane_tr);

				sprintf_s(label_txt, 255, "%g", center_x / 1000.0f);
				osgText::Text* text = new  osgText::Text;
				osgText::Font* normal_font = osgText::readFontFile("fonts/arial.ttf");
				text->setFont(normal_font);
				text->setText(label_txt);
				text->setPosition(osg::Vec3(0, -bbHullRadius, bbHullRadius));
				text->setCharacterSize(15.0f);
				text->setFontResolution(20, 20);
				text->setAutoRotateToScreen(true);
				text->setColor(osg::Vec4(0, 0, 0, 1));
				text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
				text->setAlignment(osgText::Text::CENTER_CENTER);
				base_plane->addDrawable(text);

				prev_x = center_x;
				center_x += m_aSectionOffset[i];
			}

			//if ((m_aSectionEnd[i] - prev_x) > 0.0001)
			//{
			//	gp_Pln aFacePlane(gp_Pnt(m_aSectionEnd[i], center_y, center_z), gp_Dir(1.0, 0.0, 0.0));

			//	BRepBuilderAPI_MakeFace aMakeFaceCommand(aFacePlane, -length_z / 1.5, length_z / 1.5, -length_y / 1.5, length_y / 1.5);
			//	TopoDS_Face aShape = aMakeFaceCommand.Face();
			//	Handle(AIS_Shape) myShape = new AIS_Shape(aShape);
			//	myShape->SetTransparency(0.5);
			//	myShape->SetColor(Quantity_NOC_WHITE);
			//	if (m_bDisplaySection)
			//		myAISContext->Display(myShape, Standard_False);
			//	section_list.push_back(myShape);
			//	sectionpln_list.push_back(aFacePlane);

			//	Handle(AIS_TextLabel) aLabel = new AIS_TextLabel();
			//	sprintf_s(label_txt, 255, "%g", center_x);
			//	aLabel->SetText(label_txt);
			//	aLabel->SetPosition(gp_Pnt(center_x, center_y - length_y / 1.5, m_aSectionOffset[1]));
			//	myAISContext->Display(aLabel, Standard_False);
			//	section_label_list.push_back(aLabel);
			//}
		}
		//else
		//{
		//	center_y = m_aSectionStart[i];
		//	Standard_Real prev_y = center_y;
		//	//vector< vector< Handle(AIS_Shape) > > m_aSectionList;
		//	while (center_y < m_aSectionEnd[i])
		//	{
		//		gp_Pnt center(center_x, center_y, center_z);
		//		section_pos_list.push_back(center);
		//		gp_Pln aFacePlane(center, gp_Dir(0.0, 1.0, 0.0));

		//		BRepBuilderAPI_MakeFace aMakeFaceCommand(aFacePlane, -length_z / 1.5, length_z / 1.5, -length_x / 1.5, length_x / 1.5);
		//		TopoDS_Face aShape = aMakeFaceCommand.Face();
		//		Handle(AIS_Shape) myShape = new AIS_Shape(aShape);
		//		myShape->SetTransparency(0.5);
		//		myAISContext->Display(myShape, Standard_False);
		//		gp_Trsf aPistonTrsf;
		//		aPistonTrsf.SetTranslationPart(gp_Vec(gp_Pnt(0, 0, 0), center));
		//		myAISContext->SetLocation(myShape, aPistonTrsf);

		//		section_list.push_back(myShape);
		//		sectionpln_list.push_back(aFacePlane);

		//		Handle(AIS_TextLabel) aLabel = new AIS_TextLabel();
		//		sprintf_s(label_txt, 255, "%g", center_y);
		//		aLabel->SetText(label_txt);
		//		aLabel->SetPosition(gp_Pnt(center_x + length_x / 1.5, center_y, m_aSectionOffset[1]));
		//		myAISContext->Display(aLabel, Standard_False);
		//		section_label_list.push_back(aLabel);

		//		prev_y = center_y;
		//		center_y += m_aSection[2][i];
		//	}

		//	//if ((m_aSectionEnd[i] - prev_y) > 0.0001)
		//	//{
		//	//	gp_Pln aFacePlane(gp_Pnt(m_aSectionEnd[i], center_y, center_z), gp_Dir(0.0, 1.0, 0.0));

		//	//	BRepBuilderAPI_MakeFace aMakeFaceCommand(aFacePlane, -length_z / 1.5, length_z / 1.5, -length_x / 1.5, length_x / 1.5);
		//	//	TopoDS_Face aShape = aMakeFaceCommand.Face();
		//	//	Handle(AIS_Shape) myShape = new AIS_Shape(aShape);
		//	//	myShape->SetTransparency(0.5);
		//	//	myShape->SetColor(Quantity_NOC_WHITE);
		//	//	if (m_bDisplaySection)
		//	//		myAISContext->Display(myShape, Standard_False);
		//	//	section_list.push_back(myShape);
		//	//	sectionpln_list.push_back(aFacePlane);

		//	//	Handle(AIS_TextLabel) aLabel = new AIS_TextLabel();
		//	//	sprintf_s(label_txt, 255, "%g", center_y);
		//	//	aLabel->SetText(label_txt);
		//	//	aLabel->SetPosition(gp_Pnt(center_x + length_x / 1.5, center_y, m_aSectionOffset[1]));
		//	//	myAISContext->Display(aLabel, Standard_False);
		//	//	section_label_list.push_back(aLabel);

		//	//}
		//}

		//m_aSectionList.push_back(section_list);
		//m_aSectionPosList.push_back(section_pos_list);
		//m_aSectionLabelList.push_back(section_label_list);
		//m_aSectionPlnList.push_back(sectionpln_list);

		//myAISContext->UpdateCurrentViewer();
	}
}


void CIRES2View::OnCheckBowBreaking()
{
	m_bBowBreaking = true;
}


void CIRES2View::OnUpdateCheckBowBreaking(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBowBreaking);
}


void CIRES2View::OnCheckStern()
{
	m_bBowBreaking = false;
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


void CIRES2View::OnButtonCalculateSectionPoints()
{
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

	osg::Plane water_plane(osg::Vec3(0, 0, 1), osg::Vec3(m_iHULLPos[0], m_iHULLPos[1], m_iHULLPos[2]));


	Standard_Boolean PerformNow = Standard_False;
	BRepBuilderAPI_Transform bbat(m_shapeHull, m_aiHull->LocalTransformation(), true);
	TopoDS_Shape new_hull = bbat.Shape();
	BRepBuilderAPI_Transform bbat1(m_aisWaterLine->Shape(), m_aisWaterLine->LocalTransformation(), true);
	TopoDS_Shape new_waterline = bbat1.Shape();
	BRepAlgoAPI_Section section(new_hull, new_waterline, PerformNow);
	section.ComputePCurveOn1(Standard_True);
	section.Approximation(TopOpeBRepTool_APPROX);
	section.Build();

	m_aisWaterLineResult = new AIS_Shape(section.Shape());
	m_aisWaterLineResult->SetWidth(2.0);
	m_aisWaterLineResult->SetDisplayMode(0);
	m_aisWaterLineResult->SetColor(Quantity_NOC_RED);
	myAISContext->Display(m_aisWaterLineResult, Standard_False);

	RemoveSectionPoints();
	double edge_deflection = 1.0f;
	m_aSectionPointDataList.clear();

	if (m_pEditPointsGap == NULL)
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if (pFrame)
		{
			m_pEditPointsGap = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pFrame->m_wndRibbonBar.FindByID(ID_EDIT_POINT_GAP));
		}
	}

	float points_gap = 0.01f;
	if (m_pEditPointsGap)
	{
		CString temp_string;
		temp_string = m_pEditPointsGap->GetEditText();
		points_gap = atof(temp_string) * 1000.0f;
	}

	if (!m_aiHull.IsNull())
	{
		TopExp_Explorer Ex(new_hull, TopAbs_FACE);
		vector< Handle_Geom_Surface > surface_list;
		vector< TopAbs_Orientation > surface_orient_list;
		while (Ex.More())
		{
			TopoDS_Shape* face = new TopoDS_Shape((Ex.Current()));
			TopoDS_Face f = TopoDS::Face(*face);
			Handle_Geom_Surface Surface = BRep_Tool::Surface(f);
			surface_list.push_back(Surface);
			surface_orient_list.push_back(f.Orientation());
			Ex.Next();
		}

		m_aWaterLinePointData.clear();
		if (!m_aisWaterLineResult.IsNull())
		{
			myAISContext->Remove(m_aisWaterLineResult, Standard_False);
			m_aisWaterLineResult.Nullify();
		}

		m_aisWaterLineResult = CalculateSectionWaterline(new_hull, surface_list, surface_orient_list, new_waterline, 0, m_aWaterLinePointData);
		m_aisWaterLineResult->SetWidth(2.0);
		m_aisWaterLineResult->SetDisplayMode(0);
		m_aisWaterLineResult->SetColor(Quantity_NOC_RED);

		if (m_aisWaterLineResult)
			myAISContext->Display(m_aisWaterLineResult, Standard_False);


		for (int i = 0; i < m_aSectionList.size(); i++)
		{
			vector< Handle(AIS_Shape) > section_list;
			for (int j = 0; j < m_aSectionList[i].size(); j++)
			{
				vector< PointData > section_point_data;
				Handle(AIS_Shape) myShape = m_aSectionList[i][j];

				BRepBuilderAPI_Transform bbat2(myShape->Shape(), myShape->LocalTransformation(), true);
				TopoDS_Shape new_section = bbat2.Shape();


				Handle(AIS_Shape) asection = CalculateSection(new_hull, surface_list, surface_orient_list, new_section, 1, section_point_data, points_gap);
				asection->SetWidth(2.0);
				asection->SetDisplayMode(0);
				asection->SetColor(Quantity_NOC_RED);

				if (section_point_data.size() > 0)
					m_aSectionPointDataList.push_back(section_point_data);

				if (m_bDisplaySectionResult)
					myAISContext->Display(asection, Standard_False);

				section_list.push_back(asection);
			}
			m_aSectionResultList.push_back(section_list);
		}
	}
	myAISContext->UpdateCurrentViewer();

	CalculateOutputResult();
}

void CIRES2View::CalculateOutputResult(bool refresh)
{
	S_N.clear();
	N_BETA.clear();
	Y_VAL_ST.clear();
	Z_VAL_ST.clear();
	BETA_ST.clear();
	X_COOR.clear();
	Y.clear();
	Z_COOR.clear();
	X_NORM.clear();
	Y_NORM.clear();
	Z_NORM.clear();
	ALPHA.clear();
	BETA.clear();
	GAMMA.clear();

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
					fprintf_s(save_file, "   %lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
						m_aWaterLinePointData[i].pnt.X() / 1000.0f,
						m_aWaterLinePointData[i].pnt.Y() / 1000.0f,
						m_aWaterLinePointData[i].pnt.Z() / 1000.0f,
						m_aWaterLinePointData[i].normal.X(),
						m_aWaterLinePointData[i].normal.Y(),
						m_aWaterLinePointData[i].normal.Z(),
						m_aWaterLinePointData[i].angle_alpha,
						m_aWaterLinePointData[i].angle_beta,
						m_aWaterLinePointData[i].angle_gamma);
					if (i == 0)
					{
						max_y = m_aWaterLinePointData[i].pnt.Y() / 1000.0f;
					}
					else
					{
						float current_y = m_aWaterLinePointData[i].pnt.Y() / 1000.0f;
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
					fprintf_s(save_file, "   %lf   %d\n", m_aSectionPointDataList[i][0].pnt.X() / 1000.0f, m_aSectionPointDataList[i].size());
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %lf", abs(m_aSectionPointDataList[i][j].pnt.Y()) / 1000.0f);
					}
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %lf", m_aSectionPointDataList[i][j].pnt.Z() / 1000.0f);
					}
					fprintf_s(save_file, "\n");
					fprintf_s(save_file, "   ");
					for (int j = 0; j < m_aSectionPointDataList[i].size(); j++)
					{
						fprintf_s(save_file, "   %lf", m_aSectionPointDataList[i][j].angle_beta);
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
	R_CLEAR.assign(R_CL.begin(), R_CL.end());
	R_BREAK.assign(R_BR.begin(), R_BR.end());
	R_BOUYA.assign(R_BO.begin(), R_BO.end());

	if (HULL_TYPE == 2)
	{
		R_CL.clear();
		R_BR.clear();
		READ_HULL(2);
		//SEL_MODE2();
		for (int i = 0; i < R_CLEAR.size(); i++)
		{
			for (int j = 0; j < R_CLEAR[i].size(); j++)
			{
				R_CLEAR[i][j] = R_CLEAR[i][j] + R_CL[i][j];
			}
		}
		for (int i = 0; i < R_BREAK.size(); i++)
		{
			for (int j = 0; j < R_BREAK[i].size(); j++)
			{
				R_BREAK[i][j] = R_BREAK[i][j] + R_BR[i][j];
			}
		}
	}

	if (HULL_TYPE == 3)
	{
		R_CL.clear();
		R_BR.clear();
		READ_HULL(3);
		//SEL_MODE2();
		for (int i = 0; i < R_CLEAR.size(); i++)
		{
			for (int j = 0; j < R_CLEAR[i].size(); j++)
			{
				R_CLEAR[i][j] = R_CLEAR[i][j] + R_CL[i][j];
			}
		}
		for (int i = 0; i < R_BREAK.size(); i++)
		{
			for (int j = 0; j < R_BREAK[i].size(); j++)
			{
				R_BREAK[i][j] = R_BREAK[i][j] + R_BR[i][j];
			}
		}
	}

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

	VSP.resize(NV);
	VELOCI.resize(NV);
	FROUD.resize(NV);
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

	SIGMA.resize(NSIGMA);
	for (int ISIG = 1; ISIG <= NSIGMA; ISIG++)
	{
		SIGMA[ISIG - 1] = SIGMAP + SSIGMA*(ISIG - 1);
	}

	THCK.resize(NH);
	for (int IH = 1; IH <= NH; IH++)
	{
		THCK[IH - 1] = HH + SH*(IH - 1);
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
			while (ifp.ReadOneLineFromFile() > 7)
			{
				X_COOR.push_back(atof(ifp.m_array_strOutput[0]));
				Y.push_back(atof(ifp.m_array_strOutput[1]));
				Z_COOR.push_back(atof(ifp.m_array_strOutput[2]));
				X_NORM.push_back(atof(ifp.m_array_strOutput[3]));
				Y_NORM.push_back(atof(ifp.m_array_strOutput[4]));
				Z_NORM.push_back(atof(ifp.m_array_strOutput[5]));
				ALPHA.push_back(atof(ifp.m_array_strOutput[6]));
				BETA.push_back(atof(ifp.m_array_strOutput[7]));
				GAMMA.push_back(atof(ifp.m_array_strOutput[8]));
			}
		}
		ifp.m_fp_input = NULL;
	}

	fprintf_s(fp_6, "THE NUMBER OF FRAMES = %d\n", N_FRAME);
	fprintf_s(fp_6, "X_COOR(I)        Y(I)    Z_COOR(I)    X_NORM(I)    Y_NORM(I)    Z_NORM(I)     ALPHA(I)      BETA(I)     GAMMA(I)\n");
	for (int I = 1; I <= N_FRAME; I++)
	{
		fprintf_s(fp_6, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n",
			X_COOR[I - 1], Y[I - 1], Z_COOR[I - 1], X_NORM[I - 1], Y_NORM[I - 1], Z_NORM[I - 1], ALPHA[I - 1], BETA[I - 1], GAMMA[I - 1]);

		ALPHA[I - 1] = ALPHA[I - 1] / (180 / PI3);
		BETA[I - 1] = BETA[I - 1] / (180 / PI3);
		GAMMA[I - 1] = GAMMA[I - 1] / (180 / PI3);
	}
	X_COOR.push_back(2.0f * X_COOR[N_FRAME - 1] - X_COOR[N_FRAME - 2]);
	Y.push_back(Y[N_FRAME - 1]);
	Z_COOR.push_back(Z_COOR[N_FRAME - 1]);
	X_NORM.push_back(X_NORM[N_FRAME - 1]);
	Y_NORM.push_back(Y_NORM[N_FRAME - 1]);
	Z_NORM.push_back(Z_NORM[N_FRAME - 1]);
	ALPHA.push_back(ALPHA[N_FRAME - 1]);
	BETA.push_back(BETA[N_FRAME - 1]);
	GAMMA.push_back(GAMMA[N_FRAME - 1]);
	fclose(fp_5);
	fp_5 = NULL;

	for (int II = 0; II < N_FRAME; II++)
	{
		float TEM_I = min(1.0, max(-1., (Z_NORM[II] * Z_NORM[II] + Y_NORM[II] * Y_NORM[II])));
		float TEM_J = min(1.0, max(-1., (X_NORM[II] * Y_NORM[II])));
		ALPHA[II] = abs(acos(TEM_I));
		fprintf_s(fp_6, "   %d   %lf   %lf\n", II + 1, ALPHA[II] * 180 / PI3, BETA[II] * 180 / PI3);
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
				for (int i = 0; i < NS_S; i++)
				{
					ifp.ReadOneLineFromFile();
					if (ifp.ReadOneLineFromFile() > 1)
					{
						S_N.push_back(atof(ifp.m_array_strOutput[0]));
						N_BETA.push_back(atoi(ifp.m_array_strOutput[1]));
						ifp.ReadOneLineFromFile();
						vector< float > temp_array;
						for (int j = 0; j < N_BETA[i]; j++)
						{
							temp_array.push_back(atof(ifp.m_array_strOutput[j]));
						}
						Y_VAL_ST.push_back(temp_array);
						temp_array.clear();
						ifp.ReadOneLineFromFile();
						for (int j = 0; j < N_BETA[i]; j++)
						{
							temp_array.push_back(atof(ifp.m_array_strOutput[j]));
						}
						Z_VAL_ST.push_back(temp_array);
						temp_array.clear();
						ifp.ReadOneLineFromFile();
						for (int j = 0; j < N_BETA[i]; j++)
						{
							temp_array.push_back(atof(ifp.m_array_strOutput[j]));
						}
						BETA_ST.push_back(temp_array);
					}
				}
			}
			ifp.m_fp_input = NULL;
		}

		Sin_Beta.resize(NS_S);
		for (int KK = 0; KK < NS_S; KK++)
		{
			fprintf_s(fp_6, "   %lf,   %d\n", S_N[KK], N_BETA[KK]);
			for (int I = 0; I < N_BETA[KK]; I++)
			{
				fprintf_s(fp_6, "   %lf", Y_VAL_ST[KK][I]);
			}
			fprintf_s(fp_6, "\n");
			for (int I = 0; I < N_BETA[KK]; I++)
			{
				fprintf_s(fp_6, "   %lf", Z_VAL_ST[KK][I]);
			}
			fprintf_s(fp_6, "\n");
			for (int I = 0; I < N_BETA[KK]; I++)
			{
				fprintf_s(fp_6, "   %lf", BETA_ST[KK][I]);
			}
			fprintf_s(fp_6, "\n");

			Sin_Beta[KK].resize(N_BETA[KK]);
			for (int I = 0; I < N_BETA[KK]; I++)
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
	for (int IA = 1; IA <= N_FRAME - 1; IA++)
	{
		X_TEM = GAUS(IA, 1);
		Y_TEM = GAUS(IA, 2);
		Z_TEM = GAUS(IA, 3);
		Sum_X = Sum_X + X_TEM;
		Sum_Y = Sum_Y + Y_TEM;
		Sum_Z = Sum_Z + Z_TEM;
	}

	float Eta_1 = Sum_Z / Sum_X;
	R_BR.clear();
	vector< float > sigmas(NSIGMA);
	R_BR.resize(NH, sigmas);
	for (int I = 1; I <= N_FRAME; I++)
	{
		for (int IH = 1; IH <= NH; IH++)
		{
			for (int IS = 1; IS <= NSIGMA; IS++)
			{
				float R_Lamda = pow(((3.0 * RHO * GG) / (E_young * pow(THCK[IH - 1], 3.0))), (1.0 / 4.0));
				R_BR[IH - 1][IS - 1] = (R_Lamda * SIGMA[IS - 1] * THCK[IH - 1] * THCK[IH - 1] / (1.93 * Eta_1)) * Y[I - 1];
			}
		}
	}
}
void CIRES2View::CLEARING1()
{
	YH.clear();
	for (int I = 1; I <= N_FRAME; I++)
	{
		YH.push_back(tan(ALPHA[I - 1]));
	}
	float B5 = 0.0;
	float B6 = 0.0;
	float B5_TEM = 0.0;
	float B6_TEM = 0.0;
	float SUM_5, SUM_6;
	float B13 = 0.0;
	float B14 = 0.0;
	float B15 = 0.0;

	for (int IA = 1; IA <= N_FRAME - 1; IA++)
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
	R_CL.clear();
	vector< float > sigmas(NV);
	R_CL.resize(NH, sigmas);
	for (int IVS = 1; IVS <= NV; IVS++)
	{
		for (int IH = 1; IH <= NH; IH++)
		{
			B13 = 2.0 * RHOL * GG * THCK[IH - 1] * BREADTH * FROUD[IVS - 1];
			B14 = B13 * XK3H1;
			B15 = B13 * XK3H2 *FG;
			float RV = B5 * B14;
			float RFV = B6 * B15;
			R_CL[IH - 1][IVS - 1] = RV + RFV;
		}
	}
}

void CIRES2View::BOUYANCY1()
{
	DEPTH_BUOY.clear();
	SLOPE_Y.clear();
	SLOPE_Z.clear();
	SLOPE.clear();
	SLOPE_BETA_DEG.clear();
	Z_BUOY.clear();
	Y_BUOY.clear();
	DIST_ICE.clear();
	R_SP.clear();
	R_SF.clear();
	Staion_length_Buoy.resize(NS_S);
	GIRTH_LENGTH.resize(NS_S);
	N_END_GIRTH.resize(NS_S);
	for (int KK = 1; KK <= NS_S; KK++)
	{
		vector< float > i6(N_BETA[KK - 1]);
		DEPTH_BUOY.push_back(i6);
		SLOPE_Y.push_back(i6);
		SLOPE_Z.push_back(i6);
		SLOPE.push_back(i6);
		SLOPE_BETA_DEG.push_back(i6);
		Z_BUOY.push_back(i6);
		Y_BUOY.push_back(i6);
		DIST_ICE.push_back(i6);
		R_SP.push_back(i6);
		R_SF.push_back(i6);
		for (int I6 = 1; I6 <= N_BETA[KK - 1]; I6++)
		{
			DEPTH_BUOY[KK - 1][I6 - 1] = (DRAFT - Z_VAL_ST[KK - 1][I6 - 1]) + (Z_VAL_ST[KK - 1][I6 - 1] - Z_VAL_ST[KK - 1][I6 - 1]) / 2.0;
		}
	}

	int NO_BUOY;
	N_BUOY.resize(NS_S);
	for (int KK = 1; KK <= NS_S; KK++)
	{
		NO_BUOY = 6;
		for (int I5 = 2; I5 <= N_BETA[KK - 1]; I5++)
		{
			SLOPE_Y[KK - 1][I5 - 2] = abs(abs(Y_VAL_ST[KK - 1][I5 - 1]) - abs(Y_VAL_ST[KK - 1][I5 - 2]));
			SLOPE_Z[KK - 1][I5 - 2] = abs(Z_VAL_ST[KK - 1][I5 - 1] - Z_VAL_ST[KK - 1][I5 - 2]);
			if (SLOPE_Z[KK - 1][I5 - 2] == 0.0f)
			{
				SLOPE[KK - 1][I5 - 2] = 0.0f;
			}
			else
			{
				SLOPE[KK - 1][I5 - 2] = SLOPE_Y[KK - 1][I5 - 2] / SLOPE_Z[KK - 1][I5 - 2];
			}

			SLOPE_BETA_DEG[KK - 1][I5 - 2] = atan(SLOPE[KK - 1][I5 - 2]) * (180 / PI3);
			if (NO_BUOY == 6)
			{
				N_BUOY[KK - 1] = I5 - 1;
				if (SLOPE_BETA_DEG[KK - 1][I5 - 2] >= 80.0f)
				{
					NO_BUOY = 5;
				}
			}
		}
	}

	R_BO.resize(NH);
	for (int IH = 1; IH <= NH; IH++)
	{
		float R_sf = 0.;
		float R_sf_Tem = 0.;
		R_SP[0][0] = 0.;
		float R_sp_Tem = 0.;

		float B_STATION = 0.;
		float X_LPP = 0.;
		float DEP_MEAN = 0.;
		float BETA_MEAN_1 = 0.;
		float GIRTH_IMSI = 0.;
		int I_FINISH;
		for (int KK = 1; KK <= NS_S - 1; KK++)
		{
			Staion_length_Buoy[KK - 1] = abs(S_N[KK] - S_N[KK - 1]);
			GIRTH_LENGTH[KK - 1] = 0.;
			I_FINISH = 5;
			for (int I3 = 1; I3 >= N_BETA[KK - 1]; I3++)
			{
				Z_BUOY[KK - 1][I3 - 1] = abs(Z_VAL_ST[KK - 1][I3] - Z_VAL_ST[KK - 1][I3 - 1]);
				Y_BUOY[KK - 1][I3 - 1] = abs(Y_VAL_ST[KK - 1][I3] - Y_VAL_ST[KK - 1][I3 - 1]);

				if (Y_BUOY[KK - 1][I3 - 1] == 0)
				{
					DIST_ICE[KK - 1][I3 - 1] = 0;

					if (I_FINISH == 5)
					{
						N_END_GIRTH[KK - 1] = I3 - 1;
						I_FINISH = 3;
					}
				}
				else if (Y_BUOY[KK - 1][I3 - 1] > 0)
				{
					DIST_ICE[KK - 1][I3 - 1] = sqrt(pow(Y_BUOY[KK - 1][I3 - 1], 2.0) + pow(Z_BUOY[KK - 1][I3 - 1], 2.0));
					if (Y_VAL_ST[KK - 1][I3 - 1] >= BREADTH / 2.)
					{
						DIST_ICE[KK - 1][I3 - 1] = 0.;
					}
				}
				GIRTH_LENGTH[KK - 1] = GIRTH_LENGTH[KK - 1] + DIST_ICE[KK - 1][I3 - 1];
				N_END_GIRTH[KK - 1] = I3 - 1;
				if (GIRTH_LENGTH[KK - 1] >= BREADTH / 2.)
				{
					N_END_GIRTH[KK - 1] = I3 - 1;
					break;
				}
			}
		}

		float R_SP_TOTAL = 0.;
		float R_SF_TOTAL = 0.;

		for (int KK = 1; KK <= NS_S - 1; KK++)
		{
			float R_SP_ST = 0.;
			float R_SF_ST = 0.;

			for (int I4 = 1; I4 <= N_END_GIRTH[KK - 1]; I4++)
			{
				R_SP[KK - 1][I4 - 1] = GG * RHO0 * DIST_ICE[KK - 1][I4 - 1] * THCK[IH - 1] * DEPTH_BUOY[KK - 1][I4 - 1] * Staion_length_Buoy[KK - 1];
				R_SF[KK - 1][I4 - 1] = GG * RHO0 * Staion_length_Buoy[KK - 1] * DIST_ICE[KK - 1][I4 - 1] * THCK[IH - 1] * Sin_Beta[KK - 1][I4 - 1] * FG;
				R_SF_ST = R_SF_ST + R_SF[KK - 1][I4 - 1];
				R_SP_ST = R_SP_ST + R_SP[KK - 1][I4 - 1];
			}
			R_SF_TOTAL = R_SF_TOTAL + R_SF_ST;
			R_SP_TOTAL = R_SP_TOTAL + R_SP_ST;
		}

		if (NS_S > 1)
			R_SP_TOTAL = R_SP_TOTAL / (S_N[0] - S_N[NS_S - 1]);
		else if (NS_S > 0)
			R_SP_TOTAL = R_SP_TOTAL / S_N[0];

		R_BO[IH - 1] = (R_SP_TOTAL + R_SF_TOTAL) * 2;
	}
}

float ALP(float X, int II)
{
	float A = X_COOR[0] - X_COOR[II - 1];
	float B = X_COOR[0] - X_COOR[II];
	float DXX = X_COOR[II - 1] - X_COOR[II];
	return (ALPHA[II - 1] * (B - X) + ALPHA[II] * (X - A)) / DXX;
}

float BT(float X, int II)
{
	float A = X_COOR[0] - X_COOR[II - 1];
	float B = X_COOR[0] - X_COOR[II];
	float DXX = X_COOR[II - 1] - X_COOR[II];
	return (BETA[II - 1] * (B - X) + BETA[II] * (X - A)) / DXX;
}

float YY(float X, int II)
{
	float A = X_COOR[0] - X_COOR[II - 1];
	float B = X_COOR[0] - X_COOR[II];
	float DXX = X_COOR[II - 1] - X_COOR[II];
	return (Y[II - 1] * (B - X) + Y[II] * (X - A)) / DXX;
}

float YYH(float X, int II)
{
	float A = X_COOR[0] - X_COOR[II - 1];
	float B = X_COOR[0] - X_COOR[II];
	float DXX = X_COOR[II - 1] - X_COOR[II];
	return (YH[II - 1] * (B - X) + YH[II] * (X - A)) / DXX;
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
	float W[] = { .2955242247, .2692667193, .2190863625, .1494513491, .0666713443 };
	float X[] = { .1488743389, .4333953941, .6794095682, .8650633666, .9739065285 };

	float A = X_COOR[0] - X_COOR[II - 1];
	float B = X_COOR[0] - X_COOR[II];
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
				  DX = XR*X[J - 1];
				  SS = SS + W[J - 1] * (FFX(XM + DX, II) + FFX(XM - DX, II));
			  }
	}
		break;
	case 2:
	{
			  for (int J = 1; J <= 5; J++)
			  {
				  DX = XR*X[J - 1];
				  SS = SS + W[J - 1] * (FY(XM + DX, II) + FY(XM - DX, II));
			  }
	}
		break;
	case 3:
	{
			  for (int J = 1; J <= 5; J++)
			  {
				  DX = XR*X[J - 1];
				  SS = SS + W[J - 1] * (FFZ(XM + DX, II) + FFZ(XM - DX, II));
			  }
	}
		break;
	case 15:
	{
			   for (int J = 1; J <= 5; J++)
			   {
				   DX = XR*X[J - 1];
				   SS = SS + W[J - 1] * (FB5(XM + DX, II) + FB5(XM - DX, II));
			   }
	}
		break;
	case 16:
	{
			   for (int J = 1; J <= 5; J++)
			   {
				   DX = XR*X[J - 1];
				   SS = SS + W[J - 1] * (FB6(XM + DX, II) + FB6(XM - DX, II));
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
	fprintf_s(fp_7, "   Vs(kts)\t  Hi(m)\t\t sigf(kPa)\t R_br(kN)\tR_cl(kN)\tR_bu(kN)\t  R_i(kN)\n");
	for (int IV = 1; IV <= NV; IV++)
	{
		for (int IS = 1; IS <= NSIGMA; IS++)
		{
			for (int IH = 1; IH <= NH; IH++)
			{
				fprintf_s(fp_8, " IV = %d   SIGMA = %d   THICK = %d\n", IV, IS, IH);
				R_TOTAL = R_BREAK[IH - 1][IS - 1] + R_CLEAR[IH - 1][IV - 1] + R_BOUYA[IH - 1];
				fprintf_s(fp_7, " %lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
					VSP[IV - 1], THCK[IH - 1], SIGMA[IS - 1] / 1000.0f, R_BREAK[IH - 1][IS - 1] / 1000.0f, R_CLEAR[IH - 1][IV - 1] / 1000.0f, R_BOUYA[IH - 1] / 1000.0f, R_TOTAL / 1000.0f);
			}
		}
	}
}