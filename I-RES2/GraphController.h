#pragma once
 
// CGraphController

#include "CMPGraphCtrl.h"
#include "math.h"

#if 0
typedef enum
{
	GS_Normal,
 
	GS_Displacement, 
	GS_Velocity,
	GS_Acceleration,

	GS_Torque,
	GS_Speed,
	GS_TN_curve,

	GS_Power,
	
	GS_Locomotion,
	
	GS_MAX
} GraphStyle;
#endif

typedef enum
{
	DS_Normal,
  
	DS_Press,
	DS_Press_Slide,
	DS_Press_Motor,
	
	DS_Cushion,
	DS_Cushion_Blank_Hold,
	DS_Cushion_Motor,
	
	DS_Transfer,
	DS_Transfer_Crossbar_Locomotion,
	DS_Transfer_Crossbar_Feeding,
	DS_Transfer_Crossbar_Lifting,
	DS_Transfer_Crossbar_Rotating,

	DS_Transfer_Tool_Locomotion,
	DS_Transfer_Tool_Feeding,
	DS_Transfer_Tool_Lifting,
	DS_Transfer_Tool_Clamping,

	DS_Transfer_Motor_R1_Motor,
	DS_Transfer_Motor_L_Motor,
	DS_Transfer_Motor_R2_Motor,
	DS_Transfer_Motor_R3_Motor,

	DS_Transfer_Motor_Feeding_Motor,
	DS_Transfer_Motor_Lifting_Motor,
	DS_Transfer_Motor_Clamping_Motor,
	
	DS_MAX
} DeviceStyle_Type;

typedef enum
{
	XAU_Time_Degree,
	XAU_MG_Angle,
	XAU_MAX
} XAxisUnit_Type;

#if 0
typedef enum { HorizontalX, VerticalY } Orientation;
#endif
 
class CGraphController
{
private:
protected:
	CRect		m_rGraphPos;

	HWND		m_hParentWnd;
	UINT		m_uiCtrlID;
	CWnd *	m_pWnd;
	CWnd *	m_pWrapperWnd;

	GraphStyle			m_eGraphStyle;
	DeviceStyle_Type	m_eDeviceStyle;
	XAxisUnit_Type		m_eXAxisUnit;

public:	
	IMPGraphCtrl *				m_pIGraphCtrl;
	IMPGraphCollection *		m_pIElementCollection;
	IMPGraphCollection *		m_pICursorCollection;
	IMPGraphAxis *				m_pIAxis[2];
	
private:
protected:
public:
	CGraphController(HWND hParendWnd, UINT uiCtrlID);
	CGraphController( HWND hParentWnd, UINT uiCtrlID, CRect rGraphPos  );
	virtual ~CGraphController();
    
	void Clear_Controller();
	HWND Get_ParentWnd(); 
	CWnd * Get_Wnd();
	UINT Get_CtrlID();
	void Set_CtrlID( UINT uiCtrlID );

	void SetWindowPos( CWnd* pWndInsertAfter, CRect rWndPos );
	CString Get_AxisLabel( Orientation eOrientation = VerticalY);
	BOOL Set_AxisLabel( Orientation eOrientation = VerticalY);
	BOOL Set_AxisLabel( Orientation eOrientation, CString strAxisLabel );

	BOOL Set_GraphStyle( GraphStyle eGraphStyle );
	BOOL Get_GraphStyle( GraphStyle * pGraphStyle );

	BOOL Get_DeviceStyle( DeviceStyle_Type * pDeviceStyle );
	BOOL Set_DeviceStyle( DeviceStyle_Type eDeviceStyle );

	BOOL Get_XAxisUnit( XAxisUnit_Type * pXAxisUnit );
	BOOL Set_XAxisUnit( XAxisUnit_Type eXAxisUnit );

	BOOL Set_GraphElement( CArray<double, double&> * pElementsX, CArray<double, double&> * pElementsY );
};
