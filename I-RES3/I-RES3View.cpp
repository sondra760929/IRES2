
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

	vtk_engine->OnSize(nType, cx, cy);
}


void CIRES3View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pMainFrame->m_wndDlgToolbar.m_MainToolbar.m_pView = this;

	vtk_engine->Init(this);
}

void CIRES3View::OnImportModel()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("IGES Files (*.iges , *.igs)|*.iges; *.igs|I-RES2 Files (*.osgb)|*.osgb|All Files (*.*)|*.*||"),
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

				vtkAssembly* current_tr = vtkAssembly::New();
				LoadShapesGeo(aShape, current_tr);

				vtk_engine->m_pvtkRenderer->AddActor(current_tr);

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
			pt[0] = V1.X();
			pt[1] = V1.Y();
			pt[2] = V1.Z();
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
		cubeActor->SetMapper(cubeMapper);

		vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
		normalGenerator->SetInputData(cube);
		normalGenerator->ComputePointNormalsOn();
		normalGenerator->ComputeCellNormalsOff();
		normalGenerator->SetSplitting(0); //I want exactly one normal per vertex
		normalGenerator->Update();

		vtkSmartPointer<vtkPolyDataMapper> mapperNormals =
			vtkSmartPointer<vtkPolyDataMapper>::New();

		//I chose arrows as representation
		vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
		arrow->Update();

		//use the output of vtkPolyDataNormals as input for the glyph3d
		vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
		glyph->SetInputData(normalGenerator->GetOutput());
		glyph->SetSourceConnection(arrow->GetOutputPort());
		glyph->OrientOn();
		glyph->SetVectorModeToUseNormal();
		glyph->Update();

		mapperNormals->SetInputConnection(glyph->GetOutputPort());

		//now follows standard code which could be taken from almost any example…
		vtkSmartPointer<vtkActor> actorNormals =
			vtkSmartPointer<vtkActor>::New();
		actorNormals->SetMapper(mapperNormals);

		sub_geo->AddPart(cubeActor);
		vtk_engine->m_pvtkRenderer->AddActor(actorNormals);
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
