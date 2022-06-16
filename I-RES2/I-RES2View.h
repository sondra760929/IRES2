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
#include "CDlgProgress.h"
#include "DlgTranslation.h"
class CMainFrame;

enum MOUSE_SELECTION_MODE
{
	SELECTION_NONE = 0,
	SELECTION_CENTER,
	SELECTION_WINDOW,
	SELECTION_NORMAL,
	SELECTION_TRANSLATION,
	SELECTION_ROTATION,
	SELECTION_MAKEDATUM
};

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
	double fx;
	double fy;
	double fz;

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
protected: // serialization에서만 만들어집니다.
	CIRES2View();
	DECLARE_DYNCREATE(CIRES2View)
	CRenderingThread* mThreadHandle;

// 특성입니다.
public:
	CIRES2Doc* GetDocument() const;
	cOSG2* mOSG;
	CMainFrame* m_pMainFrame;
	float DRAFT, BREADTH;
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
	//osg::ref_ptr< osg::MatrixTransform > osgWaterline;
	osg::ref_ptr< osg::MatrixTransform > osgWaterlineSection;
	//vector< osg::ref_ptr<osg::MatrixTransform> > osgSections;
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
	void CAL_COND(bool save_file = true);
	void READ_HULL(int ID);
	void READ_ICE_INPUT();
	void READ_ICECOFF_INPUT();
	void CALC_ATTAINABLE_SPEED();
	void CALC_SATELLITE(float concentration, float flexural_strength, float ice_thickness, bool save_file = true);
	void SEL_MODE1();
	void BREAKING1();
	void CLEARING1();
	void BOUYANCY1();
	void WRITE_OUT();
	float GAUS(int II, int IOP);
	float GAUS2(int II, int IOP);
	int current_calculate_type = 0;
	void CalculateOutputResult(int type, bool refresh = true);
	void CalculateSectionWaterline(osg::Vec3 plane_normal, osg::Vec3 plane_point, int align_axis, vector< PointData >& section_point_data, bool check_point_distance, float point_distance, vector< vector< osg::Vec3 > >& section_line, bool use_start_end = false, float start_pos = 0, float end_pos = 0);
	bool GetNormal(osg::Drawable* geo, PointData& pd);
	void AddSectionDataGeo(vector< PointData >& pt_list, osg::Group* group);
	void AddSectionGeo(vector< vector< osg::Vec3 > >& pt_list, osg::Group* group);
	void ClearSections();
	void ClearProject(CString current_project_path);
	float GetXforYMax(float& y_max);
	//void ClearSectionPoints();
	//void SetCurrentStep(int i_step);
	CPoint m_ptStart;
	void UnSetCenterPoint();
	void UnSetFlipNormal();
	osg::ref_ptr< osg::Camera > m_cameraStatus;
	osg::ref_ptr< osg::Vec3Array > m_ptSelectionRect;
	osg::ref_ptr< osg::Geometry > m_geometrySelectionRect;
	osg::ref_ptr< osg::Geode > m_goedeRectangle;
	vector< float > m_fConcentration;
	vector< float > m_fFlexuralStrength;
	vector< float > m_fIceThickness;
	vector< float > m_fShipSpeed;
	vector< float > m_fLongitude;
	vector< float > m_fLatitude;
	vector< float > m_fX;
	vector< float > m_fY;
	vector< float > m_fedgeX;
	vector< float > m_fedgeY;
	vector< vector< float > > m_fExSpeed;
	vector< vector< int > > realMap;
	int startRow = 110;							// 시작 지점 행 설정
	int startCol = 183;							// 시작 지점 열 설정
	int goalRow = 127;							// 도착 지점 행 설정
	int goalCol = 82;							// 도착 지점 열 설정

	int maxMapSizeRow;
	int maxMapSizeCol;
	void SpeedDecision();
	float m_fTargetResistance;
	float m_fInitSpeed;
	float m_fMaxSpeed;
	float m_fIncreSpeed;
	float m_fEstimationSpeed;
	void SetSelectionWindow(CPoint start, CPoint end);
	bool m_bInitialize;
	//FILE* FileLog;
	bool m_bProgressing;
	CString m_strStatus;
	bool m_bConditionConstant;
	int m_iStatus;
	int m_iTotal;
	CDlgProgress* m_DlgProgress;
	void BeginProgress();
	void EndProgress();
	time_t start_time, end_time;
	void UpdateProgress();
	float m_fWaterlineStartPos;
	float m_fWaterlineEndPos;
	//float m_fDraftValue;
	float m_fWaterlinePointGap;
	void CalculateWaterSectionPoint();
	void CalculateSectionPoint();
	bool CreateJob(CString job_name, int type);
	bool SelectJob(CString job_name);
	bool DeleteJob(CString job_name);
	void LoadIceInput();
	void LoadDraftSectionSetting();
	void SaveDraftSectionSetting();
	void LoadCrossSectionSetting();
	void SaveCrossSectionSetting();
	void RunExecute(CString command_string);
	void OnButtonTranslate();
	void OnButtonRotate();
	void OnButtonSetNormal();
	void OnButtonMakeDatum();
	void OnButtonToggleOblique();
	MOUSE_SELECTION_MODE m_iSelectionMode;
	int m_iCurrentStatus;
	osg::Vec3 m_ptXMin;
	osg::Vec3 m_ptXMax;
	void CheckDouble();
	bool m_bDoubleCalc;
	//CMFCRibbonEdit* m_pEditStart;
	//CMFCRibbonEdit* m_pEditEnd;
	//CMFCRibbonEdit* m_pEditSpace;
	////CMFCRibbonEdit* m_pEditPointsGap;
	//CMFCRibbonEdit* m_pEditPointsDistance;
	//CMFCRibbonEdit* m_pEditPointsNumber;

	//CMFCRibbonEdit* m_pEditStartWaterline;
	//CMFCRibbonEdit* m_pEditEndWaterline;
	//CMFCRibbonEdit* m_pEditSpaceWaterline;
	//CMFCRibbonEdit* m_pEditPointsDistanceWaterline;
	//CMFCRibbonEdit* m_pEditPointsNumberWaterline;

	//CMFCRibbonEdit* m_pHULLSpinXPos;
	//CMFCRibbonEdit* m_pHULLSpinYPos;
	//CMFCRibbonEdit* m_pHULLSpinZPos;

	//CMFCRibbonEdit* m_pHULLSpinXRot;
	//CMFCRibbonEdit* m_pHULLSpinYRot;
	//CMFCRibbonEdit* m_pHULLSpinZRot;

	//CMFCRibbonEdit* m_pSectionSpinXRot;
	//CMFCRibbonEdit* m_pSectionSpinYRot;
	//CMFCRibbonEdit* m_pSectionSpinZRot;

	//CMFCRibbonEdit* m_pWaterlineSpinZPos;

	//CMFCRibbonEdit* m_pWaterlineSpinXRot;
	//CMFCRibbonEdit* m_pWaterlineSpinYRot;
	void ClearWaterLine();
	void ClearCrossSectionLine();

	float m_fCrossSectionStart;
	float m_fCrossSectionEnd;
	float m_fCrossSectionOffset;
	float m_fCrossSectionPointGap;
	//osg::Vec3 m_iHULLPos;
	//osg::Vec3 m_iHULLRot;
	osg::Vec3 m_iSectionRot;
	//osg::Vec3 m_iWaterLinePos;
	//osg::Vec3 m_iWaterLineRot;
	//void UpdateHullPos();
	//void UpdateWaterlinePos();
	void ShowOutputSummury(CString job_name);
	void HideOutputSummury();
	void ShowMap(CString job_name);
	void HideMap();
	vector< float > m_aSectionStart;
	vector< float > m_aSectionEnd;
	vector< float > m_aSectionOffset;
	bool m_bBowBreaking;
	//bool m_bShowSection;
	bool m_bShowSectionData;
	//bool m_bShowWaterline;
	bool m_bShowWaterlineData;
	bool m_bUseDistanceForAxis;
	bool m_bUseDistanceForAxisWaterline;

	int screen_width;
	int screen_height;
	bool m_bViewEdge;
	vector< vector< osg::Vec3 > > m_aWaterLine;
	vector< PointData> m_aWaterLinePointData;
	vector< vector< vector< osg::Vec3 > > > m_aSectionLine;
	vector< vector< PointData > > m_aSectionPointDataList;
	void PreFrameUpdate();
	queue < PreFrameUpdateData > m_QAddChild;
	queue < PreFrameUpdateData > m_QRemoveChild;
	// 작업입니다.
	bool m_isCreateFolder;
	vector< float > SATELLITE_DATA[10];
	vector< vector< float > > ESTIMATION_DATA[10];

	CDlgTranslation* m_pTranslationDlg;
	void OnKeyESC();
	void SetDlgPoint(float x, float y, float z);
	osg::Vec3 start_point;
	osg::Vec3 end_point;
	osg::Vec3 center_point;
	osg::Vec3 selection_point;
	void ResizeControl(int cx, int cy);
	void ClearFunctions();
	void ConvertNormal(osg::Geometry* geometry);
	void SetNormalFromReference(osg::Geometry* ref);
	void CheckNormal(osg::Geode* parent_geode, vector< bool >& checked_status, vector< osg::Vec3Array* >& check_vertices, vector< osg::Vec3Array* >& check_normals, int check_id);
	void AddDatum(float x, float y, float z);
	vector< osg::Vec3 > m_aDatumInHull;
	void SaveDatumFile();
	void LoadDatumFile();
	void AddDatumInLocal(osg::Vec3 pos);
	void OnButtonSetUnit(UNIT_MODE um);
	void OnButtonCalc();
	void OnButtonSetUnit();
	void SaveCurrentHull();
	void UpdageHullSize();
	void UpdateWaterLineGeo();
	void SaveIceInput();
	void OnButtonSaveSectionData();
	float GetEstimatonSpeed(float target_sistance, vector<float>& speed, vector<float>& resistance);
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
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonSaveImage();
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
	//afx_msg void OnButtonShowHideSections();
	afx_msg void OnButtonShowHideSectionCut();
	//afx_msg void OnButtonHideShowWaterlines();
	afx_msg void OnButtonShowHideWaterlineCut();
	//afx_msg void OnUpdateButtonShowHideSections(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonShowHideSectionCut(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateButtonHideShowWaterlines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonShowHideWaterlineCut(CCmdUI *pCmdUI);
	afx_msg void OnButtonSaveHull();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonAnalysis();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSelectSection();
	//afx_msg void OnSpinHullXPos();
	//afx_msg void OnSpinHullYPos();
	//afx_msg void OnSpinHullZPos();
	//afx_msg void OnSpinHullXAngle();
	//afx_msg void OnSpinSectionZ();
	//afx_msg void OnSpinWaterlineZ();
	//afx_msg void OnSpinWaterlineX();
	//afx_msg void OnSpinWaterlineY();
	//afx_msg void OnSpinHullYAngle();
	//afx_msg void OnSpinHullZAngle();
	afx_msg void OnButtonHullPointToPoint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnSpinSectionX();
	//afx_msg void OnSpinSectionY();
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
	afx_msg void OnButtonViewPerspective();
	afx_msg void OnButtonViewOrtho();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCheckDistanceForAxis();
	afx_msg void OnUpdateCheckDistanceForAxis(CCmdUI *pCmdUI);
	afx_msg void OnCheckDistance();
	afx_msg void OnUpdateCheckDistance(CCmdUI *pCmdUI);
	afx_msg void OnCheckNumber();
	afx_msg void OnUpdateCheckNumber(CCmdUI *pCmdUI);
	afx_msg void OnEditDistance();
	afx_msg void OnUpdateEditDistance(CCmdUI *pCmdUI);
	afx_msg void OnEditNumber();
	afx_msg void OnUpdateEditNumber(CCmdUI *pCmdUI);
	afx_msg void OnEditStartWaterline();
	afx_msg void OnEditEndWaterline();
	//afx_msg void OnEditSpaceWaterline();
	afx_msg void OnCheckDistanceWaterline();
	afx_msg void OnUpdateCheckDistanceWaterline(CCmdUI *pCmdUI);
	afx_msg void OnCheckNumberWaterline();
	afx_msg void OnUpdateCheckNumberWaterline(CCmdUI *pCmdUI);
	afx_msg void OnEditDistanceWaterline();
	afx_msg void OnUpdateEditDistanceWaterline(CCmdUI *pCmdUI);
	afx_msg void OnEditNumberWaterline();
	afx_msg void OnUpdateEditNumberWaterline(CCmdUI *pCmdUI);
	afx_msg void OnButtonDefineSectionsWaterline();
	afx_msg void OnButtonDefine();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // I-RES2View.cpp의 디버그 버전
inline CIRES2Doc* CIRES2View::GetDocument() const
   { return reinterpret_cast<CIRES2Doc*>(m_pDocument); }
#endif

