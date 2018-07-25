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

// I-RES2View.h : CIRES2View 클래스의 인터페이스
//

#pragma once
#include "I-RES2Doc.h"
#include "MFC_OSG2.h"

class CIRES2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CIRES2View();
	DECLARE_DYNCREATE(CIRES2View)
	CRenderingThread* mThreadHandle;

// 특성입니다.
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

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CIRES2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // I-RES2View.cpp의 디버그 버전
inline CIRES2Doc* CIRES2View::GetDocument() const
   { return reinterpret_cast<CIRES2Doc*>(m_pDocument); }
#endif

