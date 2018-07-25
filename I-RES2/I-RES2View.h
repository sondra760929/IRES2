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
	osg::ref_ptr< osg::MatrixTransform > osgAxis;
	osg::ref_ptr< osg::MatrixTransform > osgHull;
	osg::ref_ptr< osg::MatrixTransform > osgWaterline;
	vector< osg::MatrixTransform* > osgSections;
	vector< osg::Vec3 > osgSectionPosList;
	void FitWorld();
	osg::BoundingBox bbHull;
	float bbHullRadius;
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

	CMFCRibbonEdit* m_pSectionSpinYRot;
	CMFCRibbonEdit* m_pSectionSpinZRot;

	CMFCRibbonEdit* m_pWaterlineSpinZPos;

	CMFCRibbonEdit* m_pWaterlineSpinXRot;
	CMFCRibbonEdit* m_pWaterlineSpinYRot;

	float m_iHULLPos[3];
	float m_iHULLRot[3];
	float m_iSectionRot[3];
	float m_iWaterLinePos[3];
	float m_iWaterLineRot[3];
	vector< float > m_aSectionStart;
	vector< float > m_aSectionEnd;
	vector< float > m_aSectionOffset;
	bool m_bBowBreaking;

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
};

#ifndef _DEBUG  // I-RES2View.cpp�� ����� ����
inline CIRES2Doc* CIRES2View::GetDocument() const
   { return reinterpret_cast<CIRES2Doc*>(m_pDocument); }
#endif

