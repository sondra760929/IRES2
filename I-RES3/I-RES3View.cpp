
// I-RES3View.cpp: CIRES3View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "I-RES3.h"
#endif

#include "I-RES3Doc.h"
#include "I-RES3View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkUnstructuredGrid.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTransform.h"
#include <vtkDoubleArray.h>
#include <vtkCullerCollection.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkQuadricLODActor.h>
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkCellData.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include "IGESControl_Reader.hxx"
#include <BRepMesh_IncrementalMesh.hxx>
#include <GeomLib.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Connect.hxx>
#include <TShort_Array1OfShortReal.hxx>
#include <Poly_PolygonOnTriangulation.hxx>
#include <GCPnts_UniformDeflection.hxx>
#include <IntAna2d_AnaIntersection.hxx>
#include <gp_Lin2d.hxx>
#include "vtkPolyDataNormals.h"
#include "vtkArrowSource.h"
#include "vtkGlyph3D.h"
#include "vtkCubeAxesActor.h"
#include "vtktextproperty.h"
#include "vtkPlane.h"
#include "vtkCutter.h"
#include "vtkPlaneSource.h"
#include "vtkPointSource.h"
#include "vtkStringArray.h"
#include "vtkLabeledDataMapper.h"
#include "vtkCellLocator.h"
//#include "vtkMaskedGlyph3D.h"

// CIRES3View

IMPLEMENT_DYNCREATE(CIRES3View, CView)

BEGIN_MESSAGE_MAP(CIRES3View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIRES3View::OnFilePrintPreview)
	ON_COMMAND(ID_BUTTON_VIEW_FRONT, &CIRES3View::OnViewFront)
	ON_COMMAND(ID_BUTTON_VIEW_REAR, &CIRES3View::OnViewRear)
	ON_COMMAND(ID_BUTTON_VIEW_LEFT, &CIRES3View::OnViewLeft)
	ON_COMMAND(ID_BUTTON_VIEW_RIGHT, &CIRES3View::OnViewRight)
	ON_COMMAND(ID_BUTTON_VIEW_TOP, &CIRES3View::OnViewTop)
	ON_COMMAND(ID_BUTTON_VIEW_BOTTOM, &CIRES3View::OnViewBottom)
	ON_COMMAND(ID_BUTTON_VIEW_ISO, &CIRES3View::OnViewISO)
	ON_COMMAND(ID_BUTTON_VIEW_PERSPECTIVE, &CIRES3View::OnViewPerspective)
	ON_COMMAND(ID_BUTTON_VIEW_ORTHO, &CIRES3View::OnViewOrtho)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CIRES3View 생성/소멸
map< CString, vtkActor* > map_geo;

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

CIRES3View::CIRES3View() noexcept
	: m_bBowBreaking(true)
	, m_bShowSection(true)
	, m_bShowSectionData(true)
	, m_bShowWaterline(true)
	, m_bShowWaterlineData(true)
	, m_bUseDistanceForAxis(false)
	, m_bUseDistanceForAxisWaterline(false)
	, points_gap_waterline(500.0f)
{
	vtk_engine = new CVTKEngine();
}

CIRES3View::~CIRES3View()
{
	delete vtk_engine;
}

BOOL CIRES3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CIRES3View 그리기

void CIRES3View::OnDraw(CDC* /*pDC*/)
{
	CIRES3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CIRES3View 인쇄


void CIRES3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIRES3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CIRES3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CIRES3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CIRES3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIRES3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIRES3View 진단

#ifdef _DEBUG
void CIRES3View::AssertValid() const
{
	CView::AssertValid();
}

void CIRES3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIRES3Doc* CIRES3View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIRES3Doc)));
	return (CIRES3Doc*)m_pDocument;
}
#endif //_DEBUG


// CIRES3View 메시지 처리기


void CIRES3View::OnDestroy()
{

	CView::OnDestroy();
}


BOOL CIRES3View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}


void CIRES3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_iClientWidth = cx;
	m_iClientHeight = cy;
	if (txtHullSize.Get())
	{
		if (txtHullSize->GetBounds())
			txtHullSize->SetDisplayPosition(m_iClientWidth - txtHullSize->GetBounds()[1], m_iClientHeight - txtHullSize->GetBounds()[3]);
		else
			txtHullSize->SetDisplayPosition(m_iClientWidth - 250, m_iClientHeight - 55);
	}
	vtk_engine->OnSize(nType, cx, cy);
}


void CIRES3View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pMainFrame->m_wndDlgToolbar.m_MainToolbar.m_pView = this;
	m_pMainFrame->m_wndClassView.m_pView = this;
	vtk_engine->Init(this);
}

void CIRES3View::OnImportModel()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("IGES Files (*.iges , *.igs)|*.iges; *.igs|I-RES2 Files (*.geo)|*.osgb|All Files (*.*)|*.*||"),
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

				actHull = vtkAssembly::New();
				LoadShapesGeo(aShape, actHull);
				vtk_engine->m_pvtkRenderer->AddActor(actHull);
				actHull->GetBounds(m_dHullSize);
				//	set water line
				m_fDraftValue = (m_dHullSize[5] - m_dHullSize[4]) / 2.0;

				// Create a text actor.
				txtHullSize = vtkSmartPointer<vtkTextActor>::New();
				char output_chrs[1024];
				sprintf_s(output_chrs, "Hull Size\nX : Max %.2lfm    Min %.2lfm\nY : Max %.2lfm    Min %.2lfm\nZ : Max %.2lfm    Min %.2lfm",
					m_dHullSize[1], m_dHullSize[0], m_dHullSize[3], m_dHullSize[2], m_dHullSize[5], m_dHullSize[4]);
				txtHullSize->SetInput(output_chrs);
				vtkTextProperty *txtprop = txtHullSize->GetTextProperty();
				txtprop->SetFontFamilyToArial();
				txtprop->BoldOn();
				txtprop->SetFontSize(16);
				txtprop->ShadowOn();
				txtprop->SetShadowOffset(1, 1);
				txtprop->SetColor(1, 1, 1);
				vtk_engine->m_pvtkRenderer->AddActor(txtHullSize);
				if (txtHullSize->GetBounds())
					txtHullSize->SetDisplayPosition(m_iClientWidth - txtHullSize->GetBounds()[1], m_iClientHeight - txtHullSize->GetBounds()[3]);
				else
					txtHullSize->SetDisplayPosition(m_iClientWidth - 250, m_iClientHeight - 55);

				m_pMainFrame->m_wndClassView.SetHulllStatus(true);
				// Assign actor to the renderer.

				//vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor =
				//	vtkSmartPointer<vtkCubeAxesActor>::New();
				//cubeAxesActor->SetUseTextActor3D(1);
				//cubeAxesActor->SetBounds(current_tr->GetBounds());
				//cubeAxesActor->SetCamera(vtk_engine->m_pvtkRenderer->GetActiveCamera());
				//cubeAxesActor->GetTitleTextProperty(0)->SetColor(1, 0 ,0);
				//cubeAxesActor->GetTitleTextProperty(0)->SetFontSize(48);
				//cubeAxesActor->GetLabelTextProperty(0)->SetColor(1, 0, 0);

				//cubeAxesActor->GetTitleTextProperty(1)->SetColor(0 ,1, 0);
				//cubeAxesActor->GetLabelTextProperty(1)->SetColor(0 ,1, 0);

				//cubeAxesActor->GetTitleTextProperty(2)->SetColor(0, 0 ,1);
				//cubeAxesActor->GetLabelTextProperty(2)->SetColor(0, 0 ,1);

				//cubeAxesActor->DrawXGridlinesOn();
				//cubeAxesActor->DrawYGridlinesOn();
				//cubeAxesActor->DrawZGridlinesOn();
				//cubeAxesActor->SetGridLineLocation(cubeAxesActor->VTK_GRID_LINES_CLOSEST);

				//cubeAxesActor->XAxisMinorTickVisibilityOff();
				//cubeAxesActor->YAxisMinorTickVisibilityOff();
				//cubeAxesActor->ZAxisMinorTickVisibilityOff();

				//cubeAxesActor->SetFlyModeToStaticEdges();

				//vtk_engine->m_pvtkRenderer->AddActor(cubeAxesActor);
				vtk_engine->m_pvtkRenderer->ResetCamera();
				//if (geode->getNumChildren() > 0)
				//{
				//	osgUtil::Optimizer optimizer;
				//	optimizer.optimize(geode, osgUtil::Optimizer::OptimizationOptions::ALL_OPTIMIZATIONS);
				//	optimizer.reset();

				//	PreFrameUpdateData pf(osgHull, NULL);
				//	m_QRemoveChild.push(pf);

				//	PreFrameUpdateData pf1(osgHull, geode);
				//	m_QAddChild.push(pf1);
				//	//osgHull->addChild(geode);

				//	ClearSections();
				//	SetCurrentStep(1);
				//}
			}
		}
		//else//
		//{
		//	time(&start_time);
		//	time(&end_time);
		//	diff_time1 = difftime(end_time, start_time);

		//	CT2CA pszConvertedAnsiString(dlg.GetPathName());
		//	std::string strStd(pszConvertedAnsiString);
		//	osg::Node* geo_node = osgDB::readNodeFile(strStd);
		//	if (geo_node)
		//	{
		//		PreFrameUpdateData pf(osgHull, NULL);
		//		m_QRemoveChild.push(pf);

		//		PreFrameUpdateData pf1(osgHull, geo_node);
		//		m_QAddChild.push(pf1);
		//		//osgHull->addChild(geo_node);

		//		ClearSections();
		//		SetCurrentStep(1);
		//	}
		//}

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

bool CIRES3View::LoadShapesGeo(const TopoDS_Shape& aShape, vtkAssembly* geode)
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
		//vtkAssembly* geo = CurveToGeometry(s, m_fEdgeDeflection);
		//if (geo)
		//{
		//	geode->AddPart(geo);
		//}
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

		vtkAssembly* geo = FaceToGeometry(f, m_fFaceDeflection);
		if (geo != NULL)
		{
			//geo->setDataVariance(osg::Object::DYNAMIC);
			//geo->setUseDisplayList(false);
			//geo->setUseVertexBufferObjects(true);

			//osg::StateSet *ss = geo->getOrCreateStateSet();
			//osg::Material *mtl = new osg::Material();
			//mtl->setDiffuse(osg::Material::FRONT_AND_BACK,
			//	osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			//ss->setAttributeAndModes(mtl, osg::StateAttribute::ON |
			//	osg::StateAttribute::OVERRIDE);

			geode->AddPart(geo);
		}

		TopExp_Explorer Ex4(f, TopAbs_EDGE);
		while (Ex4.More())
		{
			TopoDS_Edge s = TopoDS::Edge((Ex4.Current()));
			//geo = CurveToGeometry(s, f);
			//if (geo)
			//{
			//	//osg::StateSet *ss = geo->getOrCreateStateSet();
			//	//osg::Material *mtl = new osg::Material();
			//	//mtl->setDiffuse(osg::Material::FRONT_AND_BACK,
			//	//	osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
			//	//ss->setAttributeAndModes(mtl, osg::StateAttribute::ON |
			//	//	osg::StateAttribute::OVERRIDE);

			//	geode->AddPart(geo);
			//}

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

vtkAssembly* CIRES3View::FaceToGeometry(const TopoDS_Face& aFace, float face_deflection)
{
	//pface->m_Shape = aFace;
	vtkAssembly* sub_geo = NULL;
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
		sub_geo = vtkAssembly::New();
		vtkPolyData *cube = vtkPolyData::New();
		vtkPoints *points = vtkPoints::New();
		vtkCellArray *cells = vtkCellArray::New();
		float pt[3];
		double cN[3];
		vtkIdType ids[3];

		//sub_geo = new osg::Geometry;
		//osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
		//osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array(PointNum);
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
			pt[0] = V1.X() / 1000.0;
			pt[1] = V1.Y() / 1000.0;
			pt[2] = V1.Z() / 1000.0;
			points->InsertPoint(i - 1, pt);
		}

		vtkSmartPointer<vtkDoubleArray> normalsArray =
			vtkSmartPointer<vtkDoubleArray>::New();
		normalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
		normalsArray->SetNumberOfTuples(PointNum);

		Standard_Boolean hasUV = aPoly->HasUVNodes();
		Handle_Geom_Surface Surface = BRep_Tool::Surface(aFace);
		if (aPoly->HasNormals())
		{
			const TShort_Array1OfShortReal& nors = aPoly->Normals();
			if (aFace.Orientation() == TopAbs_REVERSED)
			{
				for (i = 0; i < PointNum; i++)
				{
					cN[0] = -nors(3 * i);
					cN[1] = -nors(3 * i + 1);
					cN[2] = -nors(3 * i + 2);
					normalsArray->SetTuple(i, cN);
				}
			}
			else
			{
				for (i = 0; i < PointNum; i++)
				{
					cN[0] = nors(3 * i);
					cN[1] = nors(3 * i + 1);
					cN[2] = nors(3 * i + 2);
					normalsArray->SetTuple(i, cN);
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
					cN[0] = -Nor.X();
					cN[1] = -Nor.Y();
					cN[2] = -Nor.Z();
					normalsArray->SetTuple(i - UVNodes.Lower(), cN);
				}
				else
				{
					cN[0] = Nor.X();
					cN[1] = Nor.Y();
					cN[2] = Nor.Z();
					normalsArray->SetTuple(i - UVNodes.Lower(), cN);
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
					gp_XYZ vv = v1 ^ v2;
					Standard_Real mod = vv.Modulus();

					if (mod < Tol) continue;

					eqPlan += vv / mod;
				}

				//Standard_Real modmax = eqPlan.Modulus();

				//if(modmax > Tol) Nor(i) = gp_Dir(eqPlan);
				//else Nor(i) = gp_Dir(0., 0., 1.);

				if (aFace.Orientation() == TopAbs_REVERSED)
				{
					cN[0] = -eqPlan.X();
					cN[1] = -eqPlan.Y();
					cN[2] = -eqPlan.Z();
					normalsArray->SetTuple(i - Nodes.Lower(), cN);
				}
				else
				{
					cN[0] = eqPlan.X();
					cN[1] = eqPlan.Y();
					cN[2] = eqPlan.Z();
					normalsArray->SetTuple(i - Nodes.Lower(), cN);
				}

			}
		}

		//osg::ref_ptr<osg::DrawElementsUInt> de =
		//	new osg::DrawElementsUInt(GL_TRIANGLES, 0);

		for (int i = 1; i <= TriNum; i++)
		{
			Standard_Integer N1, N2, N3;
			Triangles(i).Get(N1, N2, N3);
			//de->push_back(N1 - 1);
			//de->push_back(N2 - 1);
			//de->push_back(N3 - 1);
			ids[0] = N1 - 1;
			ids[1] = N2 - 1;
			ids[2] = N3 - 1;

			cells->InsertNextCell(3, ids);
		}

		cube->SetPoints(points);
		points->Delete();
		cube->SetPolys(cells);
		cells->Delete();
		cube->GetPointData()->SetNormals(normalsArray);

		vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
		cubeMapper->SetInputData(cube);
		cubeMapper->StaticOn();

		//vtkQuadricLODActor *cubeActor = vtkQuadricLODActor::New();
		vtkActor *cubeActor = vtkActor::New();
		//cubeActor->DeferLODConstructionOn();
		//cubeActor->StaticOn();

		//cubeActor->GetProperty()->SetOpacity(0.5);

		cubeActor->SetMapper(cubeMapper);
		sub_geo->AddPart(cubeActor);

		polyHull.push_back(cube);

		//vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
		//normalGenerator->SetInputData(cube);
		//normalGenerator->ComputePointNormalsOn();
		//normalGenerator->ComputeCellNormalsOff();
		//normalGenerator->SetSplitting(0); //I want exactly one normal per vertex
		//normalGenerator->Update();

		//vtkSmartPointer<vtkPolyDataMapper> mapperNormals =
		//	vtkSmartPointer<vtkPolyDataMapper>::New();

		////I chose arrows as representation
		//vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
		//arrow->Update();

		////use the output of vtkPolyDataNormals as input for the glyph3d
		//vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
		//glyph->SetInputData(normalGenerator->GetOutput());
		//glyph->SetSourceConnection(arrow->GetOutputPort());
		////glyph->SetScaleFactor(0.2);
		//glyph->OrientOn();
		//glyph->SetVectorModeToUseNormal();
		//glyph->Update();

		//mapperNormals->SetInputConnection(glyph->GetOutputPort());

		////now follows standard code which could be taken from almost any example…
		//vtkSmartPointer<vtkActor> actorNormals =
		//	vtkSmartPointer<vtkActor>::New();
		//actorNormals->SetMapper(mapperNormals);

		//vtk_engine->m_pvtkRenderer->AddActor(actorNormals);
	}
	catch (Standard_Failure e)
	{
		AfxMessageBox(e.GetMessageString());
		sub_geo = NULL;
	}
	return sub_geo;
}

//vtkAssembly* CIRES3View::CurveToGeometry(const TopoDS_Edge& s, const TopoDS_Face& f)
//{
//	TopLoc_Location aLoc;
//	gp_Trsf myTransf;
//
//	Handle(Poly_Triangulation) aPolyTria = BRep_Tool::Triangulation(f, aLoc);
//	if (!aLoc.IsIdentity()) {
//		myTransf = aLoc.Transformation();
//	}
//
//	if (aPolyTria.IsNull()) return NULL;
//
//	// this holds the indices of the edge's triangulation to the actual points
//	Handle(Poly_PolygonOnTriangulation) aPoly = BRep_Tool::PolygonOnTriangulation(s, aPolyTria, aLoc);
//	if (aPoly.IsNull())
//		return NULL;
//
//	// getting size and create the array
//	Standard_Integer nbNodesInFace = aPoly->NbNodes();
//
//	const TColStd_Array1OfInteger& indices = aPoly->Nodes();
//	const TColgp_Array1OfPnt& Nodes = aPolyTria->Nodes();
//
//	osg::Geometry* sub_geo = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
//
//	gp_Pnt V;
//	int pos = 0;
//	// go through the index array
//	for (Standard_Integer i = indices.Lower(); i <= indices.Upper(); i++) {
//		V = Nodes(indices(i));
//		V.Transform(myTransf);
//		v_array->push_back(osg::Vec3((float)(V.X()), (float)(V.Y()), (float)(V.Z())));
//	}
//
//	sub_geo->setVertexArray(v_array);
//
//	sub_geo->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, nbNodesInFace));
//	osg::LineWidth* lineWidth = new osg::LineWidth(2);
//	sub_geo->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
//	sub_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//
//	return sub_geo;
//}
//
//vtkAssembly* CIRES3View::CurveToGeometry(const TopoDS_Edge& s, float edge_deflection)
//{
//	try {
//		BRepAdaptor_Curve aCurve(s);
//		int nbintervals = 1;
//		Standard_Real U1, U2;
//		FindLimits(aCurve, 500, U1, U2);
//
//		if (aCurve.GetType() == GeomAbs_BSplineCurve) {
//			nbintervals = aCurve.NbKnots() - 1;
//			nbintervals = Max(1, nbintervals / 3);
//		}
//
//		osg::Geometry* sub_geo = new osg::Geometry;
//		osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
//
//		//Standard_Boolean isPrimArrayEnabled = Graphic3d_ArrayOfPrimitives::IsEnable() && !drawCurve;
//		switch (aCurve.GetType()) {
//		case GeomAbs_Line:
//		{
//			gp_Pnt p = aCurve.Value(U1);
//			v_array->push_back(osg::Vec3(p.X(), p.Y(), p.Z()));
//
//			p = aCurve.Value(U2);
//			v_array->push_back(osg::Vec3(p.X(), p.Y(), p.Z()));
//		}
//		break;
//		default:
//		{
//			int NbP = 0;
//
//			GCPnts_UniformDeflection discretizer;
//			discretizer.Initialize(aCurve, edge_deflection, U1, U2);//don't forget parameters!!!		  Standard_Real U;
//			if (discretizer.IsDone())
//			{
//				NbP = discretizer.NbPoints();
//				Standard_Real U;
//				Standard_Integer N = Max(2, NbP*nbintervals);
//				Standard_Real DU = (U2 - U1) / (N - 1);
//				gp_Pnt p;
//
//				for (Standard_Integer i = 1; i <= N; i++) {
//					U = U1 + (i - 1)*DU;
//					p = aCurve.Value(U);
//					v_array->push_back(osg::Vec3(p.X(), p.Y(), p.Z()));
//				}
//			}
//		}
//		break;
//		}
//		sub_geo->setVertexArray(v_array);
//
//		sub_geo->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, v_array->size()));
//		osg::LineWidth* lineWidth = new osg::LineWidth(2);
//		sub_geo->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
//		sub_geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//
//		return sub_geo;
//
//	}
//	catch (...)
//	{
//	}
//	return NULL;
//}

void CIRES3View::FindLimits(const Adaptor3d_Curve& aCurve,
	const Standard_Real  aLimit,
	Standard_Real&       First,
	Standard_Real&       Last)
{
	try {
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

void CIRES3View::OnViewFront()
{
	vtk_engine->OnViewFront();
}

void CIRES3View::OnViewRear()
{
	vtk_engine->OnViewRear();
}

void CIRES3View::OnViewLeft()
{
	vtk_engine->OnViewLeft();
}

void CIRES3View::OnViewRight()
{
	vtk_engine->OnViewRight();
}

void CIRES3View::OnViewTop()
{
	vtk_engine->OnViewTop();
}

void CIRES3View::OnViewBottom()
{
	vtk_engine->OnViewBottom();
}

void CIRES3View::OnViewISO()
{
	vtk_engine->OnViewISO();
}

void CIRES3View::OnViewPerspective()
{
	vtk_engine->OnViewPerspective();
}

void CIRES3View::OnViewOrtho()
{
	vtk_engine->OnViewOrtho();
}

bool sort_curves_x(OPoint3D  i, OPoint3D  j)
{
	return (i.x > j.x);
}

bool sort_curves_y(OPoint3D  i, OPoint3D j)
{
	return (i.y > j.y);
}

bool sort_curves_z(OPoint3D i, OPoint3D j)
{
	return (i.z > j.z);
}

void CIRES3View::CalculateWaterSectionPoint()
{
	vector< PointData > section_points;
	CalculateSectionPoint(OPoint3D(0, 0, 1), OPoint3D(0, 0, m_fDraftValue), 0, section_points, m_bUseDistanceForAxisWaterline, points_gap_waterline, actWaterLine, actWaterPlane);
	//CalculateSectionPoint(OPoint3D(0, 0, 1), OPoint3D(0, 0, m_fDraftValue), 0, section_points, m_bUseDistanceForAxisWaterline, points_gap_waterline, actWaterLine, actWaterPlane, true, m_fWaterlineStartPos, m_fWaterlineEndPos);
}

bool GetCellNormals(vtkPolyData* polydata, int cell_id, double* normal)
{
	std::cout << "Looking for cell normals..." << std::endl;

	// Count points
	vtkIdType numCells = polydata->GetNumberOfCells();
	std::cout << "There are " << numCells << " cells." << std::endl;

	// Count triangles
	vtkIdType numPolys = polydata->GetNumberOfPolys();
	std::cout << "There are " << numPolys << " polys." << std::endl;

	////////////////////////////////////////////////////////////////
	// Double normals in an array
	vtkDoubleArray* normalDataDouble =
		vtkDoubleArray::SafeDownCast(polydata->GetCellData()->GetArray("Normals"));

	if (normalDataDouble)
	{
		int nc = normalDataDouble->GetNumberOfTuples();
		normalDataDouble->GetTypedTuple(cell_id, normal);
		std::cout << "There are " << nc
			<< " components in normalDataDouble" << std::endl;
		return true;
	}

	////////////////////////////////////////////////////////////////
	// Double normals in an array
	vtkFloatArray* normalDataFloat =
		vtkFloatArray::SafeDownCast(polydata->GetCellData()->GetArray("Normals"));

	if (normalDataFloat)
	{
		int nc = normalDataFloat->GetNumberOfTuples();
		float test_normal[3];
		normalDataFloat->GetTypedTuple(cell_id, test_normal);
		normal[0] = test_normal[0];
		normal[1] = test_normal[1];
		normal[2] = test_normal[2];
		std::cout << "There are " << nc
			<< " components in normalDataFloat" << std::endl;
		return true;
	}

	////////////////////////////////////////////////////////////////
	// Point normals
	vtkDoubleArray* normalsDouble =
		vtkDoubleArray::SafeDownCast(polydata->GetCellData()->GetNormals());

	if (normalsDouble)
	{
		normal[0] = normalsDouble->GetComponent(cell_id, 0);
		normal[1] = normalsDouble->GetComponent(cell_id, 1);
		normal[2] = normalsDouble->GetComponent(cell_id, 2);
		std::cout << "There are " << normalsDouble->GetNumberOfComponents()
			<< " components in normalsDouble" << std::endl;
		return true;
	}

	////////////////////////////////////////////////////////////////
	// Point normals
	vtkFloatArray* normalsFloat =
		vtkFloatArray::SafeDownCast(polydata->GetCellData()->GetNormals());

	if (normalsFloat)
	{
		normal[0] = normalsDouble->GetComponent(cell_id, 0);
		normal[1] = normalsDouble->GetComponent(cell_id, 1);
		normal[2] = normalsDouble->GetComponent(cell_id, 2);
		std::cout << "There are " << normalsFloat->GetNumberOfComponents()
			<< " components in normalsFloat" << std::endl;
		return true;
	}

	/////////////////////////////////////////////////////////////////////
	// Generic type point normals
	vtkDataArray* normalsGeneric = polydata->GetCellData()->GetNormals(); //works
	if (normalsGeneric)
	{
		std::cout << "There are " << normalsGeneric->GetNumberOfTuples()
			<< " normals in normalsGeneric" << std::endl;

		//double testDouble[3];
		normalsGeneric->GetTuple(0, normal);

		//std::cout << "Double: " << testDouble[0] << " "
		//	<< testDouble[1] << " " << testDouble[2] << std::endl;

		// Can't do this:
		/*
		float testFloat[3];
		normalsGeneric->GetTuple(0, testFloat);

		std::cout << "Float: " << testFloat[0] << " "
				  << testFloat[1] << " " << testFloat[2] << std::endl;
		*/
		return true;
	}


	// If the function has not yet quit, there were none of these types of normals
	std::cout << "Normals not found!" << std::endl;
	return false;

}
bool CIRES3View::GetNormal(PointData& pd)
{
	double max_distance = 10000000.0;
	int current_poly_index = -1;
	vtkIdType current_cell_id;
	int current_sub_id;
	double current_cell_point[3];
	for (int i = 0; i < polyHull.size(); i++)
	{
		vtkSmartPointer<vtkCellLocator> cellLocator =
			vtkSmartPointer<vtkCellLocator>::New();
		cellLocator->SetDataSet(polyHull[i]);
		cellLocator->BuildLocator();

		double testPoint[3] = { pd.pnt.x, pd.pnt.y, pd.pnt.z };

		//Find the closest points to TestPoint
		double closestPoint[3];//the coordinates of the closest point will be returned here
		double closestPointDist2; //the squared distance to the closest point will be returned here
		vtkIdType cellId; //the cell id of the cell containing the closest point will be returned here
		int subId; //this is rarely used (in triangle strips only, I believe)
		cellLocator->FindClosestPoint(testPoint, closestPoint, cellId, subId, closestPointDist2);
		if (max_distance > closestPointDist2)
		{
			max_distance = closestPointDist2;
			current_poly_index = i;
			current_cell_id = cellId;
			current_sub_id = subId;
			current_cell_point[0] = closestPoint[0];
			current_cell_point[1] = closestPoint[1];
			current_cell_point[2] = closestPoint[2];
		}
	}

	double pt_normal[3];
	bool hasCellNormals = false;
	if (polyHullToNormal.find(polyHull[current_poly_index]) != polyHullToNormal.end())
	{
		hasCellNormals = GetCellNormals(polyHullToNormal[polyHull[current_poly_index]], current_cell_id, pt_normal);
	}
	else
	{
		hasCellNormals = GetCellNormals(polyHull[current_poly_index], current_cell_id, pt_normal);
		if (!hasCellNormals)
		{
			std::cout << "No cell normals were found. Computing normals..." << std::endl;

			// Generate normals
			vtkPolyDataNormals* normalGenerator = vtkPolyDataNormals::New();
			normalGenerator->SetInputData(polyHull[current_poly_index]);
			normalGenerator->ComputePointNormalsOff();
			normalGenerator->ComputeCellNormalsOn();
			normalGenerator->Update();
			/*
			// Optional settings
			normalGenerator->SetFeatureAngle(0.1);
			normalGenerator->SetSplitting(1);
			normalGenerator->SetConsistency(0);
			normalGenerator->SetAutoOrientNormals(0);
			normalGenerator->SetComputePointNormals(1);
			normalGenerator->SetComputeCellNormals(0);
			normalGenerator->SetFlipNormals(0);
			normalGenerator->SetNonManifoldTraversal(1);
			*/

			//polyHull[current_poly_index] = normalGenerator->GetOutput();
			polyHullToNormal[polyHull[current_poly_index]] = normalGenerator->GetOutput();
			// Try to read normals again
			hasCellNormals = GetCellNormals(polyHullToNormal[polyHull[current_poly_index]], current_cell_id, pt_normal);

			std::cout << "On the second try, has cell normals? " << hasCellNormals << std::endl;
		}
	}

	if (hasCellNormals)
	{
		pd.normal.Set(pt_normal[0], pt_normal[1], pt_normal[2]);
		try	//	for alpha
		{
			gp_Lin2d lin0(gp_Pnt2d(pd.pnt.x, pd.pnt.y), gp_Dir2d(pd.normal.y, -pd.normal.x));
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
						pd.pnt_alpha.Set(Inters.Point(1).Value().X(), Inters.Point(1).Value().Y(), pd.pnt.z);
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
			gp_Lin2d lin0(gp_Pnt2d(pd.pnt.y, pd.pnt.z), gp_Dir2d(pd.normal.z, -pd.normal.y));
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
						pd.pnt_beta.Set(pd.pnt.x, Inters.Point(1).Value().X(), Inters.Point(1).Value().Y());
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
			gp_Lin2d lin0(gp_Pnt2d(pd.pnt.x, pd.pnt.z), gp_Dir2d(pd.normal.z, -pd.normal.x));
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
						pd.pnt_gamma.Set(Inters.Point(1).Value().X(), pd.pnt.y, Inters.Point(1).Value().Y());
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

	//osg::Geometry* geom = geo->asGeometry();
	//if (geom)
	//{
	//	osg::Vec3Array *vertices = (osg::Vec3Array *)geom->getVertexArray();
	//	osg::Vec3Array *normals = (osg::Vec3Array *)geom->getNormalArray();
	//	osg::Geometry::PrimitiveSetList primitiveList = geom->getPrimitiveSetList();
	//	osg::Matrix world(osg::computeLocalToWorld(geo->getParentalNodePaths()[0]));

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

	//	for (int x = 0; x <primitiveList.size(); x++)
	//	{
	//		//fprintf(FileLog, "GetNormal [%d / %d] cl : %lf, sl : %lf, rl : %lf\n", x, primitiveList.size(), pd.pnt.x(), pd.pnt.y(), pd.pnt.z());

	//		osg::PrimitiveSet *set = primitiveList[x];
	//		int numberOfIndices = set->getNumIndices();
	//		if (set->getMode() == osg::PrimitiveSet::Mode::TRIANGLES)
	//		{
	//			for (unsigned int y = 0; y < set->getDrawElements()->getNumIndices() /*numberOfIndices*/; y+=3)
	//			{
	//				index1 = set->getDrawElements()->getElement(y); //set->index(y); 
	//				if (index1 < vertices->size())
	//				{
	//					p1 = vertices->at(index1) * world;
	//				}
	//				index2 = set->getDrawElements()->getElement(y +1); //set->index(y); 
	//				if (index2 < vertices->size())
	//				{
	//					p2 = vertices->at(index2) * world;
	//				}
	//				index3 = set->getDrawElements()->getElement(y +2); //set->index(y); 
	//				if (index3 < vertices->size())
	//				{
	//					p3 = vertices->at(index3) * world;
	//				}

	//				//	Same Side Technique
	//				bool ss1 = same_side(pd.pnt, p1, p2, p3);
	//				bool ss2 = same_side(pd.pnt, p2, p1, p3);
	//				bool ss3 = same_side(pd.pnt, p3, p1, p2);
	//				//fprintf(FileLog, "GetNormal \t %d, %d, %d >> %d, %d, %d\n", index1, index2, index3, ss1, ss2, ss3);
	//				if (ss1 && ss2 && ss3)
	//				{
	//					v1 = p2 - p1;
	//					v3 = p3 - p2;
	//					osg::Vec3 normal = v1 ^ v3;
	//					//osg::Vec3 normal = normals->at(index1) + normals->at(index2) + normals->at(index3);
	//					normal.normalize();
	//					pd.normal = normal;

	//					try	//	for alpha
	//					{
	//						gp_Lin2d lin0(gp_Pnt2d(pd.pnt.x(), pd.pnt.y()), gp_Dir2d(pd.normal.y(), -pd.normal.x()));
	//						gp_Lin2d lin1(gp_Pnt2d(0, 0), gp_Dir2d(1, 0));
	//						IntAna2d_AnaIntersection Inters;
	//						Inters.Perform(lin0, lin1);
	//						if (Inters.IsDone())
	//						{
	//							if (Inters.IsDone())
	//							{
	//								if (!Inters.IdenticalElements() && !Inters.ParallelElements())
	//								{
	//									pd.alpha_exist = true;
	//									pd.pnt_alpha = osg::Vec3(Inters.Point(1).Value().X(), Inters.Point(1).Value().Y(), pd.pnt.z());
	//									pd.angle_alpha = abs(lin0.Angle(lin1) * 180.0 / M_PI);
	//									if (pd.angle_alpha > 90)
	//									{
	//										pd.angle_alpha = 180.0f - pd.angle_alpha;
	//									}
	//								}
	//							}
	//						}
	//					}
	//					catch (Standard_Failure e)
	//					{
	//						//AfxMessageBox(e.GetMessageString());
	//					}

	//					try	//	for beta
	//					{
	//						gp_Lin2d lin0(gp_Pnt2d(pd.pnt.y(), pd.pnt.z()), gp_Dir2d(pd.normal.z(), -pd.normal.y()));
	//						gp_Lin2d lin1(gp_Pnt2d(0, 0), gp_Dir2d(0, 1));
	//						IntAna2d_AnaIntersection Inters;
	//						Inters.Perform(lin0, lin1);
	//						if (Inters.IsDone())
	//						{
	//							if (Inters.IsDone())
	//							{
	//								if (!Inters.IdenticalElements() && !Inters.ParallelElements())
	//								{
	//									pd.beta_exist = true;
	//									pd.pnt_beta = osg::Vec3(pd.pnt.x(), Inters.Point(1).Value().X(), Inters.Point(1).Value().Y());
	//									pd.angle_beta = abs(lin0.Angle(lin1) * 180.0 / M_PI);
	//									if (pd.angle_beta > 90)
	//									{
	//										pd.angle_beta = 180.0f - pd.angle_beta;
	//									}
	//								}
	//							}
	//						}
	//					}
	//					catch (Standard_Failure e)
	//					{
	//						//AfxMessageBox(e.GetMessageString());
	//					}

	//					try	//	for gamma
	//					{
	//						gp_Lin2d lin0(gp_Pnt2d(pd.pnt.x(), pd.pnt.z()), gp_Dir2d(pd.normal.z(), -pd.normal.x()));
	//						gp_Lin2d lin1(gp_Pnt2d(0, 0), gp_Dir2d(0, 1));
	//						IntAna2d_AnaIntersection Inters;
	//						Inters.Perform(lin0, lin1);
	//						if (Inters.IsDone())
	//						{
	//							if (Inters.IsDone())
	//							{
	//								if (!Inters.IdenticalElements() && !Inters.ParallelElements())
	//								{
	//									pd.gamma_exist = true;
	//									pd.pnt_gamma = osg::Vec3(Inters.Point(1).Value().X(), pd.pnt.y(), Inters.Point(1).Value().Y());
	//									pd.angle_gamma = abs(lin0.Angle(lin1) * 180.0 / M_PI);
	//									if (pd.angle_gamma > 90)
	//									{
	//										pd.angle_gamma = 180.0f - pd.angle_gamma;
	//									}
	//								}
	//							}
	//						}
	//					}
	//					catch (Standard_Failure e)
	//					{
	//						//AfxMessageBox(e.GetMessageString());
	//					}
	//					return true;
	//				}
	//			}
	//		}
	//	}
	//}
	//pd.normal = osg::Vec3(0, 0, 0);
	return false;
}

void CIRES3View::CalculateSectionPoint(OPoint3D plane_normal, OPoint3D plane_point, int align_axis, vector< PointData >& section_point_data, bool check_point_distance, float point_distance, vector< vtkActor* >& act_line, vtkSmartPointer<vtkActor>& act_plane, bool use_start_end, float start_pos, float end_pos)
{
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	//plane->SetNormal(0, 0, 1);
	//plane->SetOrigin(0, 0, m_fDraftValue);
	plane->SetNormal(plane_normal.x, plane_normal.y, plane_normal.z);
	plane->SetOrigin(plane_point.x, plane_point.y, plane_point.z);

	for (int i = 0; i < act_line.size(); i++)
	{
		vtk_engine->m_pvtkRenderer->RemoveActor(act_line[i]);
		act_line[i]->Delete();
	}
	act_line.clear();

	vector< vector< OPoint3D > > section_line;
	for (int i = 0; i < polyHull.size(); i++)
	{
		vector< OPoint3D > loop_pt;
		vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
		cutter->SetInputData(polyHull[i]);
		cutter->SetCutFunction(plane);
		cutter->GenerateValues(1, 0, 0);

		vtkPolyData* poly = cutter->GetOutput();
		cutter->Update();
		vtkPoints* pts = poly->GetPoints();
		int point_count = pts->GetNumberOfPoints();
		if (point_count > 0)
		{
			double bounds[6];
			for (int j = 0; j < point_count; j++)
			{
				loop_pt.push_back(OPoint3D(pts->GetPoint(j)[0], pts->GetPoint(j)[1], pts->GetPoint(j)[2]));
				if (j == 0)
				{
					bounds[0] = pts->GetPoint(j)[0];
					bounds[1] = pts->GetPoint(j)[0];
					bounds[2] = pts->GetPoint(j)[1];
					bounds[3] = pts->GetPoint(j)[1];
					bounds[4] = pts->GetPoint(j)[2];
					bounds[5] = pts->GetPoint(j)[2];
				}
				else
				{
					if (bounds[0] > pts->GetPoint(j)[0])
						bounds[0] = pts->GetPoint(j)[0];
					if (bounds[1] < pts->GetPoint(j)[0])
						bounds[1] = pts->GetPoint(j)[0];
					if (bounds[2] > pts->GetPoint(j)[1])
						bounds[2] = pts->GetPoint(j)[1];
					if (bounds[3] < pts->GetPoint(j)[1])
						bounds[3] = pts->GetPoint(j)[1];
					if (bounds[4] > pts->GetPoint(j)[2])
						bounds[4] = pts->GetPoint(j)[2];
					if (bounds[5] < pts->GetPoint(j)[2])
						bounds[5] = pts->GetPoint(j)[2];
				}
			}

			double length[3];
			length[0] = bounds[1] - bounds[0];
			length[1] = bounds[3] - bounds[2];
			length[2] = bounds[5] - bounds[4];
			if (length[0] > length[1])
			{
				if (length[0] > length[2])
				{
					//	length[0]
					sort(loop_pt.begin(), loop_pt.end(), sort_curves_x);
				}
				else
				{
					//	length[2]
					sort(loop_pt.begin(), loop_pt.end(), sort_curves_z);
				}
			}
			else
			{
				if (length[1] > length[2])
				{
					//	length[1]
					sort(loop_pt.begin(), loop_pt.end(), sort_curves_y);
				}
				else
				{
					//	length[2]
					sort(loop_pt.begin(), loop_pt.end(), sort_curves_z);
				}
			}

			section_line.push_back(loop_pt);

			vtkSmartPointer<vtkPolyDataMapper> cutterMapper =
				vtkSmartPointer<vtkPolyDataMapper>::New();
			cutterMapper->SetInputConnection(cutter->GetOutputPort());
			cutterMapper->ScalarVisibilityOff();

			// Create cut actor
			vtkActor* actWater = vtkActor::New();
			actWater->GetProperty()->SetColor(1, 1, 1);
			actWater->GetProperty()->SetLineWidth(2);
			actWater->SetMapper(cutterMapper);
			vtk_engine->m_pvtkRenderer->AddActor(actWater);
			act_line.push_back(actWater);
		}
	}

	vtkSmartPointer<vtkPlaneSource> planeSource =
		vtkSmartPointer<vtkPlaneSource>::New();
	planeSource->SetOrigin(0, 0, m_fDraftValue);
	planeSource->SetPoint1(m_dHullSize[1], 0, m_fDraftValue);
	planeSource->SetPoint2(0, m_dHullSize[3], m_fDraftValue);
	planeSource->Update();
	vtkSmartPointer<vtkPolyDataMapper> planeMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	planeMapper->SetInputConnection(planeSource->GetOutputPort());

	if (act_plane.Get())
	{
		vtk_engine->m_pvtkRenderer->RemoveActor(act_plane);
	}
	act_plane = vtkSmartPointer<vtkActor>::New();
	act_plane->GetProperty()->SetColor(1, 1, 0);
	act_plane->GetProperty()->SetOpacity(0.50);
	act_plane->GetProperty()->EdgeVisibilityOn();
	act_plane->GetProperty()->SetEdgeColor(0, 0, 0);
	act_plane->GetProperty()->SetLineWidth(2.0);
	act_plane->SetMapper(planeMapper);
	vtk_engine->m_pvtkRenderer->AddActor(act_plane);

	//FILE* fp = NULL;
	//fopen_s(&fp, "e:\\check.txt", "wt");
	vector< OPoint3D > result_pt;
	//if (fp)
	//{
	//	fprintf(fp, "> %d\n", section_line.size());
	//}
	if (section_line.size() > 0)
	{
		//for (int i = 0; i < section_line.size(); i++)
		//{
		//	for (int j = 0; j < section_line[i].size(); j++)
		//	{
		//		fprintf(fp, "[%d/%d] %.2lf, %.2lf, %.2lf\n", i, j, section_line[i][j].x, section_line[i][j].y, section_line[i][j].z);
		//	}
		//}

		result_pt.assign(section_line[0].begin(), section_line[0].end());
		section_line.erase(section_line.begin());

		//if (fp)
		//{
		//	fprintf(fp, "set first line > %d\n", section_line.size());
		//}

		double p0[3], p1[3], pp0[3], pp1[3];
		while (section_line.size() > 0)
		{
			double max_distance = 100000.0;
			double current_distance;
			int check_index = -1;
			int check_type = -1;

			//fprintf(fp, "check result pts [%.2lf, %.2lf, %.2lf] - [%.2lf, %.2lf, %.2lf]\n", result_pt[0].x, result_pt[0].y, result_pt[0].z, result_pt[result_pt.size() - 1].x, result_pt[result_pt.size() - 1].y, result_pt[result_pt.size() - 1].z);
			p0[0] = result_pt[0].x;
			p0[1] = result_pt[0].y;
			p0[2] = result_pt[0].z;
			p1[0] = result_pt[result_pt.size() - 1].x;
			p1[1] = result_pt[result_pt.size() - 1].y;
			p1[2] = result_pt[result_pt.size() - 1].z;
			for (int i = 0; i < section_line.size(); i++)
			{
				int pt_count = section_line[i].size();

				//if (fp)
				//{
				//	fprintf(fp, "check pts [%d/%d] [%.2lf, %.2lf, %.2lf] - [%.2lf, %.2lf, %.2lf]\n", i, section_line.size(),
				//		section_line[i][0].x, section_line[i][0].y, section_line[i][0].z, section_line[i][pt_count - 1].x, section_line[i][pt_count - 1].y, section_line[i][pt_count - 1].z);
				//}

				pp0[0] = section_line[i][0].x;
				pp0[1] = section_line[i][0].y;
				pp0[2] = section_line[i][0].z;
				pp1[0] = section_line[i][pt_count - 1].x;
				pp1[1] = section_line[i][pt_count - 1].y;
				pp1[2] = section_line[i][pt_count - 1].z;

				current_distance = vtkMath::Distance2BetweenPoints(p0, pp0);
				if (current_distance < max_distance)
				{
					max_distance = current_distance;
					check_index = i;
					check_type = 0;
				}

				current_distance = vtkMath::Distance2BetweenPoints(p0, pp1);
				if (current_distance < max_distance)
				{
					max_distance = current_distance;
					check_index = i;
					check_type = 1;
				}

				current_distance = vtkMath::Distance2BetweenPoints(p1, pp0);
				if (current_distance < max_distance)
				{
					max_distance = current_distance;
					check_index = i;
					check_type = 2;
				}

				current_distance = vtkMath::Distance2BetweenPoints(p1, pp1);
				if (current_distance < max_distance)
				{
					max_distance = current_distance;
					check_index = i;
					check_type = 3;
				}
			}

			if (check_index > -1)
			{
				//if (fp)
				//{
				//	fprintf(fp, "distance : %.2lf, [%d] [%d]\n", max_distance, check_index, check_type);
				//}
				int pt_count = section_line[check_index].size();
				switch (check_type)
				{
				case 0:
				{
					for (int j = 0; j < pt_count; j++)
					{
						result_pt.insert(result_pt.begin(), section_line[check_index][j]);
					}
				}
				break;
				case 1:
				{
					for (int j = 0; j < pt_count; j++)
					{
						result_pt.insert(result_pt.begin(), section_line[check_index][pt_count - j - 1]);
					}
				}
				break;
				case 2:
				{
					for (int j = 0; j < pt_count; j++)
					{
						result_pt.push_back(section_line[check_index][j]);
					}
				}
				break;
				case 3:
				{
					for (int j = 0; j < pt_count; j++)
					{
						result_pt.push_back(section_line[check_index][pt_count - j - 1]);
					}
				}
				break;
				}
				section_line.erase(section_line.begin() + check_index);
			}
		}

		//if (check_point_distance)
		//{
			double remain_length = 0;
			for (int i = 1; i < result_pt.size(); i++)
			{
				p0[0] = result_pt[i - 1].x;
				p0[1] = result_pt[i - 1].y;
				p0[2] = result_pt[i - 1].z;
				p1[0] = result_pt[i].x;
				p1[1] = result_pt[i].y;
				p1[2] = result_pt[i].z;
				double current_length = vtkMath::Distance2BetweenPoints(p0, p1);	//	두점 사이 거리
				double current_remove_length = 0;
				OPoint3D dir = result_pt[i] - result_pt[i - 1];
				dir /= current_length;
				while (current_length - current_remove_length > remain_length)	//	점 사이 거리가 남은 거리보다 크다
				{
					PointData pd;
					current_remove_length += remain_length;	//	남은 거리 빼기
					pd.pnt = result_pt[i - 1] + (dir * current_remove_length);
					if ((pd.pnt.z <= m_fDraftValue) && ((use_start_end == false) || (pd.pnt.x <= start_pos && pd.pnt.x >= end_pos)))
					{
						if (GetNormal(pd))
						{
							section_point_data.push_back(pd);
						}
					}
					remain_length = point_distance;
				}
				remain_length -= (current_length - current_remove_length);
			}
			if (remain_length > 0)
			{
				PointData pd;
				pd.pnt = result_pt[result_pt.size() - 1];
				if ((pd.pnt.z <= m_fDraftValue) && ((use_start_end == false) || (pd.pnt.x <= start_pos && pd.pnt.x >= end_pos)))
				{
					if (GetNormal(pd))
					{
						section_point_data.push_back(pd);
					}
				}
			}
		//}
		//else
		//{
		//	//	waterline
		//	for (int j = 0; j < section_line.size(); j++)
		//	{
		//		current_count++;
		//		m_iStatus = prev_count + (current_count / total_count) * 100;
		//		UpdateProgress();
		//		if (section_line[j].size() > 0)
		//		{
		//			osg::Drawable* geo = loop_geo[section_line[j][0].x()];
		//			float current_x = floor(section_line[j][0].x() / 1000.0f) * 1000.0f + point_distance;
		//			while (current_x > section_line[j][0].x())
		//			{
		//				current_x -= point_distance;
		//			}

		//			if (use_start_end)
		//			{
		//				if (current_x > start_pos)
		//					current_x = start_pos;
		//			}

		//			for (int i = 1; i < section_line[j].size(); i++)
		//			{
		//				while (section_line[j][i].x() <= current_x && ((use_start_end == false) || (current_x >= end_pos)))
		//				{
		//					PointData pd;
		//					float ratio = (current_x - section_line[j][i - 1].x()) / (section_line[j][i].x() - section_line[j][i - 1].x());
		//					osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
		//					vec *= ratio;
		//					pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
		//					if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
		//					{
		//						if (GetNormal(geo, pd))
		//							section_point_data.push_back(pd);
		//					}
		//					current_x -= point_distance;
		//				}
		//			}
		//		}
		//	}
		//}

	}

	//float p[3];
	//int point_count = result_pt.size();

	//vtkSmartPointer<vtkPointSource> pointSource =
	//	vtkSmartPointer<vtkPointSource>::New();
	//pointSource->SetNumberOfPoints(point_count);
	//pointSource->Update();
	//vtkPoints* points = pointSource->GetOutput()->GetPoints();

	//for (int i = 0; i < point_count; i++)
	//{
	//	//fprintf(fp, "[%d/%d] %.2lf, %.2lf, %.2lf\n", i, result_pt.size(), result_pt[i].x, result_pt[i].y, result_pt[i].z);
	//	p[0] = result_pt[i].x;
	//	p[1] = result_pt[i].y;
	//	p[2] = result_pt[i].z;
	//	points->SetPoint(i, p);
	//}
	////fclose(fp);

	//// Create a mapper and actor
	//vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//pointMapper->SetInputConnection(pointSource->GetOutputPort());

	//vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
	//pointActor->SetMapper(pointMapper);
	//pointActor->GetProperty()->SetPointSize(10);
	//pointActor->GetProperty()->SetColor(1, 1, .4);

	//vtkSmartPointer<vtkLabeledDataMapper> labelMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
	//labelMapper->SetInputConnection(pointSource->GetOutputPort());
	//vtkSmartPointer<vtkActor2D> labelActor = vtkSmartPointer<vtkActor2D>::New();
	//labelActor->SetMapper(labelMapper);

	//// Add the actor to the scene
	//vtk_engine->m_pvtkRenderer->AddActor(pointActor);
	//vtk_engine->m_pvtkRenderer->AddActor(labelActor);
	
	//map< double, osg::Drawable* > loop_geo;
	//osg::Plane water_plane(plane_normal, plane_point);
	//osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(water_plane, osg::Polytope());
	////intersector->enter(*dynamic_cast<osg::Node*>(m_mtScan.get()));
	//osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	//osgUtil::IntersectionVisitor            _intersectionVisitor;
	//_intersectionVisitor.reset();
	//_intersectionVisitor.setIntersector(intersector.get());

	//osgHull_Center.get()->accept(_intersectionVisitor);
	//int prev_count = m_iStatus;
	//int total_count = 0;
	//int current_count = 0;
	//typedef osgUtil::PlaneIntersector::Intersection::Polyline Polyline;
	//if (!intersections.empty())
	//{
	//	total_count = intersections.size() * 3;

	//	osgUtil::PlaneIntersector::Intersections::iterator itr;
	//	for (itr = intersections.begin();
	//		itr != intersections.end();
	//		++itr)
	//	{
	//		osgUtil::PlaneIntersector::Intersection& intersection = *itr;
	//		current_count++;
	//		m_iStatus = prev_count + (current_count / total_count) * 100;
	//		UpdateProgress();
	//		if (intersection.matrix.valid())
	//		{
	//			// osg::notify(osg::NOTICE)<<"  transforming "<<std::endl;
	//			// transform points on polyline 
	//			for (Polyline::iterator pitr = intersection.polyline.begin();
	//				pitr != intersection.polyline.end();
	//				++pitr)
	//			{
	//				*pitr = (*pitr) * (*intersection.matrix);
	//			}

	//			// matrix no longer needed.
	//			intersection.matrix = 0;
	//		}
	//	}

	//	for (itr = intersections.begin();
	//		itr != intersections.end();
	//		++itr)
	//	{
	//		current_count++;
	//		m_iStatus = prev_count + (current_count / total_count) * 100;
	//		UpdateProgress();
	//		//	more에서 문제가 있어서 중간 포인트 정리하는 과정을 생략한다.
	//		//map< CString, int > temp_exist;
	//		CString str_exist;
	//		vector< osg::Vec3 > temp_loop;
	//		osg::Vec3 temp_pt;
	//		osgUtil::PlaneIntersector::Intersection& intersection = *itr;
	//		Polyline::iterator pitr = intersection.polyline.begin();
	//		for (;
	//			pitr != intersection.polyline.end();
	//			++pitr)
	//		{
	//			temp_pt = *pitr;
	//			//str_exist.Format("%.0lf-%.0lf-%.0lf", temp_pt.x() * 100.0f, temp_pt.y() * 100.0f, temp_pt.z() * 100.0f);
	//			//if (temp_exist.find(str_exist) == temp_exist.end())
	//			//{
	//			temp_loop.push_back(temp_pt);
	//			//	temp_exist[str_exist] = 1;
	//			//}

	//		}

	//		switch (align_axis)
	//		{
	//		case 0:
	//		{
	//			if (temp_loop[0].x() < temp_loop[temp_loop.size() - 1].x()) std::reverse(temp_loop.begin(), temp_loop.end());
	//			loop_geo[temp_loop[0].x()] = intersection.drawable.get();
	//		}
	//		break;
	//		case 1:
	//		{
	//			if (temp_loop[0].y() < temp_loop[temp_loop.size() - 1].y()) std::reverse(temp_loop.begin(), temp_loop.end());
	//			loop_geo[temp_loop[0].y()] = intersection.drawable.get();
	//		}
	//		break;
	//		case 2:
	//		{
	//			if (temp_loop[0].z() > temp_loop[temp_loop.size() - 1].z()) std::reverse(temp_loop.begin(), temp_loop.end());
	//			loop_geo[temp_loop[0].z()] = intersection.drawable.get();
	//		}
	//		break;
	//		}
	//		section_line.push_back(temp_loop);
	//	}

	//	switch (align_axis)
	//	{
	//	case 0:
	//	{
	//		sort(section_line.begin(), section_line.end(), sort_curves_x);
	//		if (check_point_distance)
	//		{
	//			float remain_length = 0;
	//			for (int j = 0; j < section_line.size(); j++)
	//			{
	//				current_count++;
	//				m_iStatus = prev_count + (current_count / total_count) * 100;
	//				UpdateProgress();
	//				if (section_line[j].size() > 0)
	//				{
	//					osg::Drawable* geo = loop_geo[section_line[j][0].x()];

	//					for (int i = 1; i < section_line[j].size(); i++)
	//					{
	//						osg::Vec3 dir = section_line[j][i] - section_line[j][i - 1];
	//						float current_length = dir.length();
	//						float step_length = 0;
	//						dir.normalize();
	//						//fprintf(stderr, "before while current_length : %lf, step_length : %lf, remain_length : %lf\n", current_length, step_length, remain_length);
	//						while (current_length - step_length > remain_length)
	//						{
	//							PointData pd;
	//							step_length += remain_length;
	//							pd.pnt = section_line[j][i - 1] + (dir * step_length);
	//							if ((pd.pnt.z() / 1000.0f <= m_fDraftValue) && ((use_start_end == false) || (pd.pnt.x() <= start_pos && pd.pnt.x() >= end_pos)))
	//							{
	//								if (GetNormal(geo, pd))
	//									section_point_data.push_back(pd);
	//							}
	//							remain_length = point_distance;
	//							//fprintf(stderr, "in while current_length : %lf, step_length : %lf, remain_length : %lf (%.2lf, %.2lf, %.2lf)\n", current_length, step_length, remain_length, pd.pnt.x(), pd.pnt.y(), pd.pnt.z());
	//						}

	//						remain_length = remain_length - (current_length - step_length);
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			//	waterline
	//			for (int j = 0; j < section_line.size(); j++)
	//			{
	//				current_count++;
	//				m_iStatus = prev_count + (current_count / total_count) * 100;
	//				UpdateProgress();
	//				if (section_line[j].size() > 0)
	//				{
	//					osg::Drawable* geo = loop_geo[section_line[j][0].x()];
	//					float current_x = floor(section_line[j][0].x() / 1000.0f) * 1000.0f + point_distance;
	//					while (current_x > section_line[j][0].x())
	//					{
	//						current_x -= point_distance;
	//					}

	//					if (use_start_end)
	//					{
	//						if (current_x > start_pos)
	//							current_x = start_pos;
	//					}

	//					for (int i = 1; i < section_line[j].size(); i++)
	//					{
	//						while (section_line[j][i].x() <= current_x && ((use_start_end == false) || (current_x >= end_pos)))
	//						{
	//							PointData pd;
	//							float ratio = (current_x - section_line[j][i - 1].x()) / (section_line[j][i].x() - section_line[j][i - 1].x());
	//							osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
	//							vec *= ratio;
	//							pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
	//							if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
	//							{
	//								if (GetNormal(geo, pd))
	//									section_point_data.push_back(pd);
	//							}
	//							current_x -= point_distance;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;
	//	case 1:
	//	{
	//		sort(section_line.begin(), section_line.end(), sort_curves_y);
	//		if (check_point_distance)
	//		{
	//			//	sections
	//			float remain_length = 0;
	//			for (int j = 0; j < section_line.size(); j++)
	//			{
	//				current_count++;
	//				m_iStatus = prev_count + (current_count / total_count) * 100;
	//				UpdateProgress();
	//				if (section_line[j].size() > 0)
	//				{
	//					osg::Drawable* geo = loop_geo[section_line[j][0].y()];

	//					//fprintf(FileLog, "lines %d\n", j);
	//					for (int i = 1; i < section_line[j].size(); i++)
	//					{
	//						osg::Vec3 dir = section_line[j][i] - section_line[j][i - 1];
	//						float current_length = dir.length();
	//						float step_length = 0;
	//						dir.normalize();
	//						//fprintf(FileLog, "[%d / %d] cl : %lf, sl : %lf, rl : %lf\n", i, section_line[j].size(), current_length, step_length, remain_length);
	//						while (current_length - step_length > remain_length)
	//						{
	//							PointData pd;
	//							step_length += remain_length;
	//							//fprintf(FileLog, "add > \tcl : %lf, \tsl : %lf, \trl : %lf ", current_length, step_length, remain_length);
	//							pd.pnt = section_line[j][i - 1] + (dir * step_length);
	//							if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
	//							{
	//								if (GetNormal(geo, pd))
	//									section_point_data.push_back(pd);
	//							}
	//							remain_length = point_distance;
	//						}

	//						remain_length -= (current_length - step_length);
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			for (int j = 0; j < section_line.size(); j++)
	//			{
	//				current_count++;
	//				m_iStatus = prev_count + (current_count / total_count) * 100;
	//				UpdateProgress();
	//				if (section_line[j].size() > 0)
	//				{
	//					osg::Drawable* geo = loop_geo[section_line[j][0].y()];
	//					int temp_count = section_line[j][0].y() / point_distance;
	//					float current_y = (float)(temp_count + 1) * point_distance;
	//					//float current_y = floor(section_line[j][0].y() / 100.0f) * 100.0f + point_distance;
	//					while (current_y > section_line[j][0].y())
	//					{
	//						current_y -= point_distance;
	//					}

	//					for (int i = 1; i < section_line[j].size(); i++)
	//					{
	//						while (section_line[j][i].y() <= current_y)
	//						{
	//							PointData pd;
	//							float ratio = (current_y - section_line[j][i - 1].y()) / (section_line[j][i].y() - section_line[j][i - 1].y());
	//							osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
	//							vec *= ratio;
	//							pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
	//							if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
	//							{
	//								if (GetNormal(geo, pd))
	//									section_point_data.push_back(pd);
	//							}
	//							current_y -= point_distance;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;
	//	case 2:
	//	{
	//		sort(section_line.begin(), section_line.end(), sort_curves_z);
	//		if (check_point_distance)
	//		{
	//			for (int j = 0; j < section_line.size(); j++)
	//			{
	//				current_count++;
	//				m_iStatus = prev_count + (current_count / total_count) * 100;
	//				UpdateProgress();
	//				if (section_line[j].size() > 0)
	//				{
	//					osg::Drawable* geo = loop_geo[section_line[j][0].z()];

	//					float remain_length = 0;
	//					for (int i = 1; i < section_line[j].size(); i++)
	//					{
	//						osg::Vec3 dir = section_line[j][i] - section_line[j][i - 1];
	//						float current_length = dir.length();
	//						float step_length = remain_length;
	//						dir.normalize();
	//						while (current_length - step_length < remain_length)
	//						{
	//							PointData pd;
	//							step_length += remain_length;
	//							pd.pnt = section_line[j][i - 1] + (dir * step_length);
	//							if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
	//							{
	//								if (GetNormal(geo, pd))
	//									section_point_data.push_back(pd);
	//							}
	//							remain_length = point_distance;
	//						}

	//						remain_length = point_distance - (current_length - step_length);
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			for (int j = 0; j < section_line.size(); j++)
	//			{
	//				current_count++;
	//				m_iStatus = prev_count + (current_count / total_count) * 100;
	//				UpdateProgress();
	//				if (section_line[j].size() > 0)
	//				{
	//					osg::Drawable* geo = loop_geo[section_line[j][0].z()];
	//					float current_z = floor(section_line[j][0].z() / 1000.0f) * 1000.0f - point_distance;
	//					while (current_z < section_line[j][0].z())
	//					{
	//						current_z += point_distance;
	//					}

	//					for (int i = 1; i < section_line[j].size(); i++)
	//					{
	//						while (section_line[j][i].z() >= current_z)
	//						{
	//							PointData pd;
	//							float ratio = (current_z - section_line[j][i - 1].z()) / (section_line[j][i].z() - section_line[j][i - 1].z());
	//							osg::Vec3 vec = section_line[j][i] - section_line[j][i - 1];
	//							vec *= ratio;
	//							pd.pnt = osg::Vec3(section_line[j][i - 1].x() + vec.x(), section_line[j][i - 1].y() + vec.y(), section_line[j][i - 1].z() + vec.z());
	//							if (pd.pnt.z() / 1000.0f <= m_fDraftValue)
	//							{
	//								if (GetNormal(geo, pd))
	//									section_point_data.push_back(pd);
	//							}
	//							current_z += point_distance;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;
	//	}
	//}
	//else
	//{
	//	osg::notify(osg::NOTICE) << "No intersections found." << std::endl;
	//}
	//m_iStatus = prev_count + 100;
}