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

// I-RES2View.h : CIRES2View Ŭ������ �������̽�
//

#pragma once
#include "I-RES2Doc.h"
#include "MFC_OSG2.h"
#include "CDlgProgress.h"
class PointData
{
public:
	osg::Vec3 pnt;
	osg::Vec3 normal;
	bool alpha_exist;
	double angle_alpha;
	osg::Vec3 pnt_alpha;
	bool beta_exist;
	double angle_beta;
	osg::Vec3 pnt_beta;
	bool gamma_exist;
	double angle_gamma;
	osg::Vec3 pnt_gamma;

	PointData()
	{
		alpha_exist = false;
		beta_exist = false;
		gamma_exist = false;
		angle_alpha = 0;
		angle_beta = 0;
		angle_gamma = 0;
	}
};

class PreFrameUpdateData
{
public:
	osg::ref_ptr< osg::Group > parent_node;
	osg::ref_ptr< osg::Node > child_node;

	PreFrameUpdateData()
	{
		parent_node = NULL;
		child_node = NULL;
	}

	PreFrameUpdateData(osg::Group* p_node, osg::Node* c_node)
	{
		parent_node = p_node;
		child_node = c_node;
	}
};

class CIRES2View : public CView
{
protected: // serialization������ ��������ϴ�.
	CIRES2View();
	DECLARE_DYNCREATE(CIRES2View)
	CRenderingThread* mThreadHandle;

// Ư���Դϴ�.
public:
	CIRES2Doc* GetDocument() const;
	cOSG2* mOSG;
	osg::TessellationHints* hints;
	bool LoadShapesGeo(const TopoDS_Shape& aShape, osg::Geode* goede);
	osg::Geometry* FaceToGeometry(const TopoDS_Face& aFace, float face_deflection);
	osg::Geometry* CurveToGeometry(const TopoDS_Edge& s, const TopoDS_Face& f);
	osg::Geometry* CurveToGeometry(const TopoDS_Edge& s, float edge_deflection);
	void FindLimits(const Adaptor3d_Curve& aCurve,
		const Standard_Real  aLimit,
		Standard_Real&       First,
		Standard_Real&       Last);
	osg::ref_ptr< osg::MatrixTransform > osgSelectPoint;
	osg::ref_ptr< osg::MatrixTransform > osgAxis;
	osg::ref_ptr< osg::MatrixTransform > osgHull;
	osg::ref_ptr< osg::MatrixTransform > osgHull_Center;
	osg::ref_ptr< osg::MatrixTransform > osgWaterline;
	osg::ref_ptr< osg::MatrixTransform > osgWaterlineSection;
	vector< osg::ref_ptr<osg::MatrixTransform> > osgSections;
	vector< osg::ref_ptr<osg::MatrixTransform> > osgSectionsData;
	vector< osg::Vec3 > osgSectionPosList;
	vector< bool > osgSectionEnable;
	osg::Quat osgSectionRotation;
	int m_iCurrentStep;
	void FitWorld();
	osg::BoundingBox bbHull;
	float bbLength[3];
	//float bbHullRadius;
	void SetCenterView(double x, double y);
	osg::Node* OnSelectPoint(double x, double y, osg::Vec3d& hit_pt);
	osg::Vec3 m_vecSelectedNormal;
	osg::Geometry* m_SelectedGeo;
	osg::Vec3 m_geoFace[3];
	osg::Vec3 m_geoEdge[3];
	osg::Vec3 m_geoNormal;
	osg::Vec3 m_geoVertex;
	void UpdateGlobalAxis(float length);
	void DefineSections();
	void CAL_COND();
	void READ_HULL(int ID);
	void SEL_MODE1();
	void BREAKING1();
	void CLEARING1();
	void BOUYANCY1();
	void WRITE_OUT();
	float GAUS(int II, int IOP);
	void CalculateOutputResult(bool refresh = true);
	void CalculateSectionWaterline(osg::Vec3 plane_normal, osg::Vec3 plane_point, int align_axis, vector< PointData >& section_point_data, bool check_point_distance, float point_distance, vector< vector< osg::Vec3 > >& section_line);
	void GetNormal(osg::Drawable* geo, PointData& pd);
	void AddSectionDataGeo(vector< PointData >& pt_list, osg::Group* group);
	void AddSectionGeo(vector< vector< osg::Vec3 > >& pt_list, osg::Group* group);
	void ClearSections();
	void ClearSectionPoints();
	void SetCurrentStep(int i_step);
	bool m_bSetCenterPoint;
	bool m_bSetFlipNormal;
	CPoint m_ptStart;
	bool m_bSelectWindow;
	void UnSetCenterPoint();
	void UnSetFlipNormal();
	osg::ref_ptr< osg::Camera > m_cameraStatus;
	osg::ref_ptr< osg::Vec3Array > m_ptSelectionRect;
	osg::ref_ptr< osg::Geometry > m_geometrySelectionRect;
	osg::ref_ptr< osg::Geode > m_goedeRectangle;
	void SetSelectionWindow(CPoint start, CPoint end);
	bool m_bInitialize;
	//FILE* FileLog;
	bool m_bProgressing;
	CString m_strStatus;
	int m_iStatus;
	int m_iTotal;
	CDlgProgress* m_DlgProgress;
	void BeginProgress();
	void EndProgress();
	time_t start_time, end_time;
	void UpdateProgress();

	CMFCRibbonEdit* m_pEditStart;
	CMFCRibbonEdit* m_pEditEnd;
	CMFCRibbonEdit* m_pEditSpace;
	CMFCRibbonEdit* m_pEditPointsGap;

	CMFCRibbonEdit* m_pHULLSpinXPos;
	CMFCRibbonEdit* m_pHULLSpinYPos;
	CMFCRibbonEdit* m_pHULLSpinZPos;

	CMFCRibbonEdit* m_pHULLSpinXRot;
	CMFCRibbonEdit* m_pHULLSpinYRot;
	CMFCRibbonEdit* m_pHULLSpinZRot;

	CMFCRibbonEdit* m_pSectionSpinXRot;
	CMFCRibbonEdit* m_pSectionSpinYRot;
	CMFCRibbonEdit* m_pSectionSpinZRot;

	CMFCRibbonEdit* m_pWaterlineSpinZPos;

	CMFCRibbonEdit* m_pWaterlineSpinXRot;
	CMFCRibbonEdit* m_pWaterlineSpinYRot;

	osg::Vec3 m_iHULLPos;
	osg::Vec3 m_iHULLRot;
	osg::Vec3 m_iSectionRot;
	osg::Vec3 m_iWaterLinePos;
	osg::Vec3 m_iWaterLineRot;
	void UpdateHullPos();
	void UpdateWaterlinePos();

	vector< float > m_aSectionStart;
	vector< float > m_aSectionEnd;
	vector< float > m_aSectionOffset;
	bool m_bBowBreaking;
	bool m_bShowSection;
	bool m_bShowSectionData;
	bool m_bShowWaterline;
	bool m_bShowWaterlineData;

	int screen_width;
	int screen_height;

	vector< vector< osg::Vec3 > > m_aWaterLine;
	vector< PointData> m_aWaterLinePointData;
	vector< vector< vector< osg::Vec3 > > > m_aSectionLine;
	vector< vector< PointData > > m_aSectionPointDataList;
	void PreFrameUpdate();
	queue < PreFrameUpdateData > m_QAddChild;
	queue < PreFrameUpdateData > m_QRemoveChild;
	// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CIRES2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnButtonImportHull();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnButtonDefineSections();
	afx_msg void OnCheckBowBreaking();
	afx_msg void OnUpdateCheckBowBreaking(CCmdUI *pCmdUI);
	afx_msg void OnCheckStern();
	afx_msg void OnUpdateCheckStern(CCmdUI *pCmdUI);
	afx_msg void OnEditStart();
	afx_msg void OnEditEnd();
	afx_msg void OnEditSpace();
	afx_msg void OnEditPointGap();
	afx_msg void OnButtonCalculateSectionPoints();
	afx_msg void OnButtonShowHideSections();
	afx_msg void OnButtonShowHideSectionCut();
	afx_msg void OnButtonHideShowWaterlines();
	afx_msg void OnButtonShowHideWaterlineCut();
	afx_msg void OnUpdateButtonShowHideSections(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonShowHideSectionCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonHideShowWaterlines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonShowHideWaterlineCut(CCmdUI *pCmdUI);
	afx_msg void OnButtonSaveHull();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonAnalysis();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSelectSection();
	afx_msg void OnSpinHullXPos();
	afx_msg void OnSpinHullYPos();
	afx_msg void OnSpinHullZPos();
	afx_msg void OnSpinHullXAngle();
	afx_msg void OnSpinSectionZ();
	afx_msg void OnSpinWaterlineZ();
	afx_msg void OnSpinWaterlineX();
	afx_msg void OnSpinWaterlineY();
	afx_msg void OnSpinHullYAngle();
	afx_msg void OnSpinHullZAngle();
	afx_msg void OnButtonHullPointToPoint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSpinSectionX();
	afx_msg void OnSpinSectionY();
	afx_msg void OnButtonNurbsConversion();
	afx_msg void OnUpdateButtonNurbsConversion(CCmdUI *pCmdUI);
	afx_msg void OnButtonfront();
	afx_msg void OnButtontop();
	afx_msg void OnButtonleft();
	afx_msg void OnButtonback();
	afx_msg void OnButtonright();
	afx_msg void OnButtonbottom();
	afx_msg void OnButtonaxo();
	afx_msg void OnButtonreset();
	afx_msg void OnButtonzoomwin();
	afx_msg void OnButtonzoomall();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // I-RES2View.cpp�� ����� ����
inline CIRES2Doc* CIRES2View::GetDocument() const
   { return reinterpret_cast<CIRES2Doc*>(m_pDocument); }
#endif

