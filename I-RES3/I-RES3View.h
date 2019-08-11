
// I-RES3View.h: CIRES3View 클래스의 인터페이스
//

#pragma once

#include "vtkMFCWindow.h"
#include "vtkRenderer.h"
#include "vtkAssembly.h"
#include "VTKEngine.h"
#include "vtkTextActor.h"

class CMainFrame;
class CIRES3Doc;

class OPoint3D
{
public:
	~OPoint3D() {};

	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.

	/*! The default constructor. */
	OPoint3D() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	OPoint3D(float X, float Y, float Z = 0.0f) : x(X), y(Y), z(Z) {};

	/*! This constructor accepts a pointer to an OPoint3D object. */
	OPoint3D(OPoint3D const * p) : x(p->x), y(p->y), z(p->z) {};

	/*! This constructor accepts an OPoint3D reference. */
	OPoint3D(OPoint3D const & p) : x(p.x), y(p.y), z(p.z) {};

	/*!  This method packs an OPoint3D object with coordinate values.  */
	void Set(float X, float Y, float Z = 0.0f) { x = X; y = Y; z = Z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D.  */
	void Set(OPoint3D *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D.  */
	void Set(OPoint3D const *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D that is passed by reference.  */
	void Set(OPoint3D &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D that is passed by reference.  */
	void Set(const OPoint3D &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method increases/decreases the coordinate values of an existing OPoint3D object.
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(float X, float Y, float Z = 0.0){ x += X; y += Y; z += Z; };

	OPoint3D const operator -(const OPoint3D &p2) const
	{
		return OPoint3D(x - p2.x, y - p2.y, z - p2.z);
	}

	OPoint3D const operator +(const OPoint3D &p2) const
	{
		return OPoint3D(x + p2.x, y + p2.y, z + p2.z);
	}

	OPoint3D const operator *(float const rhs) const
	{
		return OPoint3D(x*rhs, y*rhs, z*rhs);
	}

	OPoint3D const operator /(float const rhs) const
	{
		return OPoint3D(x / rhs, y / rhs, z / rhs);
	}

	OPoint3D const & operator += (OPoint3D const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	OPoint3D const & operator -= (OPoint3D const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	OPoint3D const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	OPoint3D const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(OPoint3D const & rhs) const
	{
		return Equal(&rhs);
	}

	bool operator !=(OPoint3D const & rhs) const
	{
		return !Equal(&rhs);
	}


	/*!\def OPoint3D_EPSILON */
#define OPoint3D_EPSILON (1e-5f)  //!<  OPoint3D_EPSILON


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(OPoint3D const *p, float epsi = OPoint3D_EPSILON) const {
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(OPoint3D const &p, float epsi = OPoint3D_EPSILON) const {
		return Equal(&p, epsi);
	};
};

class PointData
{
public:
	OPoint3D pnt;
	OPoint3D normal;
	bool alpha_exist;
	double angle_alpha;
	OPoint3D pnt_alpha;
	bool beta_exist;
	double angle_beta;
	OPoint3D pnt_beta;
	bool gamma_exist;
	double angle_gamma;
	OPoint3D pnt_gamma;

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

class CIRES3View : public CView
{
protected: // serialization에서만 만들어집니다.
	CIRES3View() noexcept;
	DECLARE_DYNCREATE(CIRES3View)

// 특성입니다.
public:
	CIRES3Doc* GetDocument() const;
	//vtkMFCWindow* m_pvtkMFCWindow;
	//vtkRenderer* m_pvtkRenderer;
	CMainFrame* m_pMainFrame;
	CVTKEngine* vtk_engine;
// 작업입니다.
public:
	bool LoadShapesGeo(const TopoDS_Shape& aShape, vtkAssembly* goede);
	vtkAssembly* FaceToGeometry(const TopoDS_Face& aFace, float face_deflection);
	//vtkAssembly* CurveToGeometry(const TopoDS_Edge& s, const TopoDS_Face& f);
	//vtkAssembly* CurveToGeometry(const TopoDS_Edge& s, float edge_deflection);
	void FindLimits(const Adaptor3d_Curve& aCurve,
		const Standard_Real  aLimit,
		Standard_Real&       First,
		Standard_Real&       Last);
	vtkSmartPointer<vtkTextActor> txtHullSize;
	vector< vtkActor* > actWaterLine;
	vtkSmartPointer<vtkActor> actWaterPlane;
	bool GetNormal(PointData& pd);
	double m_dHullSize[6];
	vtkAssembly* actHull;
	vector< vtkPolyData* > polyHull;
	map< vtkPolyData*, vtkPolyData* > polyHullToNormal;
	int m_iClientWidth, m_iClientHeight;

	vector< float > m_aSectionStart;
	vector< float > m_aSectionEnd;
	vector< float > m_aSectionOffset;
	bool m_bBowBreaking;
	bool m_bShowSection;
	bool m_bShowSectionData;
	bool m_bShowWaterline;
	bool m_bShowWaterlineData;
	bool m_bUseDistanceForAxis;
	bool m_bUseDistanceForAxisWaterline;
	float m_fDraftValue;
	float points_gap_waterline;

	void CalculateWaterSectionPoint();
	void CalculateSectionPoint(OPoint3D plane_normal, OPoint3D plane_point, int align_axis, vector< PointData >& section_point_data, bool check_point_distance, float point_distance, vector< vtkActor* >& act_line, vtkSmartPointer<vtkActor>& act_plane, bool use_start_end = false, float start_pos = 0, float end_pos = 0);
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
	virtual ~CIRES3View();
	void OnImportModel();

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
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnViewFront();
	afx_msg void OnViewRear();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewBottom();
	afx_msg void OnViewISO();
	afx_msg void OnViewPerspective();
	afx_msg void OnViewOrtho();
};

#ifndef _DEBUG  // I-RES3View.cpp의 디버그 버전
inline CIRES3Doc* CIRES3View::GetDocument() const
   { return reinterpret_cast<CIRES3Doc*>(m_pDocument); }
#endif

