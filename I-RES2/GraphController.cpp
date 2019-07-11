// PictureEx.cpp : implementation file
//
 
#include "stdafx.h"
#include "GraphController.h"
 
// CGraphController
CGraphController::CGraphController( HWND hParentWnd, UINT uiCtrlID  )
{
	m_hParentWnd	= hParentWnd;
	m_uiCtrlID		= uiCtrlID;

	CRect rParentRect;
	GetClientRect( hParentWnd, &rParentRect );

	int nOffset		= 10;
	int nTabHeight = 22;
	m_rGraphPos.left		= nOffset;
	m_rGraphPos.top		= nOffset + nTabHeight;
	m_rGraphPos.right		= (rParentRect.Width() - nOffset);
	m_rGraphPos.bottom	= (rParentRect.Height() - nOffset);

	m_pWnd			= NULL;
	m_pWrapperWnd	= NULL;

	m_pIGraphCtrl				= NULL;
	m_pIElementCollection	= NULL;
	m_pICursorCollection		= NULL;
	m_pIAxis[0]					= NULL;
	m_pIAxis[1]					= NULL;
	Clear_Controller();
	Set_CtrlID( uiCtrlID ); // ID를 이용해 m_pWnd를 생성 하거나 얻어 온다.
}

CGraphController::CGraphController( HWND hParentWnd, UINT uiCtrlID, CRect rGraphPos )
{
	m_hParentWnd	= hParentWnd;
	m_uiCtrlID		= uiCtrlID;

	m_rGraphPos		= rGraphPos; 

	m_pWnd			= NULL;
	m_pWrapperWnd	= NULL;

	m_pIGraphCtrl				= NULL;
	m_pIElementCollection	= NULL;
	m_pICursorCollection		= NULL;
	m_pIAxis[0]					= NULL;
	m_pIAxis[1]					= NULL;

	Clear_Controller();
	Set_CtrlID( uiCtrlID ); // ID를 이용해 m_pWnd를 생성 하거나 얻어 온다.
}
 
CGraphController::~CGraphController()
{
	if( m_pWrapperWnd ) {
		m_pWrapperWnd->DestroyWindow();
		m_pWrapperWnd = NULL;
	}
}

void CGraphController::Clear_Controller()
{
	m_eGraphStyle	= GS_Normal;
	m_eDeviceStyle	= DS_Normal;
	m_eXAxisUnit	= XAU_Time_Degree;

	if( m_pIGraphCtrl ) m_pIGraphCtrl->ClearGraph(); 
}

HWND CGraphController::Get_ParentWnd() 
{
	return m_hParentWnd;
}

CWnd * CGraphController::Get_Wnd() 
{
	return m_pWnd;
}

UINT CGraphController::Get_CtrlID()
{
	return m_uiCtrlID;
}

void CGraphController::Set_CtrlID( UINT uiCtrlID )
{
	HWND		hParentWnd = m_hParentWnd;
	HWND		hWnd = NULL;
	CWnd *	pParentWnd = NULL;
	CWnd *	pWnd = NULL;
	CWnd *	cpWrapperCtrl	= NULL;

	IUnknown *	pIUnknown = NULL;


	if( !uiCtrlID ) return;
	m_uiCtrlID = uiCtrlID;

	if( !hParentWnd ) return;
	pParentWnd = CWnd::FromHandle( hParentWnd );
	pWnd = pParentWnd->GetDlgItem( uiCtrlID );

	if( !pWnd ) 
	{
		if( m_pWrapperWnd ) {
			m_pWrapperWnd->DestroyWindow();
			m_pWrapperWnd = NULL;
		}

		m_pWrapperWnd	= new CWnd(); // MFC provided wrapper for Activex Controls
		m_pWrapperWnd->CreateControl(		__uuidof(MPGraphCtrl), _T(""), WS_VISIBLE, (const RECT) m_rGraphPos
												,	CWnd::FromHandle(hParentWnd), uiCtrlID );

		pParentWnd = CWnd::FromHandle( hParentWnd );
		pWnd = pParentWnd->GetDlgItem( uiCtrlID );
	}

	m_pWnd = pWnd;

	pIUnknown = pWnd->GetControlUnknown(); //weak reference
	pIUnknown->QueryInterface( __uuidof(IMPGraphCtrl), (LPVOID*) &m_pIGraphCtrl );
	pIUnknown->Release(); pIUnknown = NULL;

	m_pIGraphCtrl->get_Elements( &m_pIElementCollection );
	m_pIGraphCtrl->get_Cursors( &m_pICursorCollection );
	m_pIGraphCtrl->get_Axis( HorizontalX, &m_pIAxis[HorizontalX]);
	m_pIGraphCtrl->get_Axis( VerticalY, &m_pIAxis[VerticalY]);

	Set_GraphStyle( m_eGraphStyle );
}

void CGraphController::SetWindowPos( CWnd* pWndInsertAfter, CRect rWndPos )
{
	m_rGraphPos = rWndPos;
	m_pWnd->SetWindowPos( pWndInsertAfter, m_rGraphPos.left, m_rGraphPos.top, m_rGraphPos.Width(), m_rGraphPos.Height(), SWP_SHOWWINDOW );
}

CString CGraphController::Get_AxisLabel( Orientation eOrientation )
{
	CString cstrAxisLabel = _T("");


	switch( m_eDeviceStyle ) 
	{
		case DS_Press :
		case DS_Cushion :
		case DS_Press_Slide :
		case DS_Cushion_Blank_Hold :
			if( eOrientation == HorizontalX ) {
				switch( m_eGraphStyle)
				{
					case GS_Displacement :
					case GS_Velocity :
					case GS_Acceleration :
						if(		m_eXAxisUnit == XAU_Time_Degree )	cstrAxisLabel = _T("[t˚]");
						else if(	m_eXAxisUnit == XAU_MG_Angle )		cstrAxisLabel = _T("[deg]");
						break;
				}
			}
			else {
				switch( m_eGraphStyle)
				{
					case GS_Displacement :	{ cstrAxisLabel = _T("[mm]");		break; }
					case GS_Velocity :		{ cstrAxisLabel = _T("[mm/s]");	break; }
					case GS_Acceleration :	{ cstrAxisLabel = _T("[mm/s²]");	break; }
				}
			}	
			break;			
			
		case DS_Press_Motor :
		case DS_Cushion_Motor :
			if( eOrientation == HorizontalX ) {
				switch( m_eGraphStyle )
				{
					case GS_Displacement :
					case GS_Velocity :
					case GS_Acceleration :
					case GS_Torque :
					case GS_Speed :
					case GS_Power :
						if(		m_eXAxisUnit == XAU_Time_Degree )	cstrAxisLabel = _T("[t˚]");
						else if(	m_eXAxisUnit == XAU_MG_Angle )		cstrAxisLabel = _T("[deg]");
						break;
						
					case GS_TN_curve :
						cstrAxisLabel = _T("[rpm]");
						break;
				}
			}
			else {			
				switch( m_eGraphStyle )
				{
					case GS_Displacement :	{ cstrAxisLabel = _T("[deg]");		break; }
					case GS_Velocity :		{ cstrAxisLabel = _T("[deg/s]");		break; }
					case GS_Acceleration :	{ cstrAxisLabel = _T("[deg/s²]");	break; }
					case GS_Torque :			{ cstrAxisLabel = _T("[N·m]");		break; }
					case GS_Speed :			{ cstrAxisLabel = _T("[rpm]");		break; }
					case GS_TN_curve :		{ cstrAxisLabel = _T("[Nm]");			break; }
					case GS_Power :			{ cstrAxisLabel = _T("[KW]");			break; }
				}
			}
			break;

		case DS_Transfer_Crossbar_Rotating :
			if( eOrientation == HorizontalX ) {
				switch( m_eGraphStyle )
				{
					case GS_Displacement :
					case GS_Velocity :
					case GS_Acceleration :
						if(		m_eXAxisUnit == XAU_Time_Degree )	cstrAxisLabel = _T("[t˚]");
						else if(	m_eXAxisUnit == XAU_MG_Angle )		cstrAxisLabel = _T("[deg]");
						break;						
				}
			}
			else {
				switch( m_eGraphStyle )
				{
					case GS_Displacement :	{ cstrAxisLabel = _T("[deg]");		break; }
					case GS_Velocity :		{ cstrAxisLabel = _T("[deg/s]");		break; }
					case GS_Acceleration :	{ cstrAxisLabel = _T("[deg/s²]");	break; }
				}
			}
			break;			
			
		case DS_Transfer :
		case DS_Transfer_Crossbar_Locomotion :
		case DS_Transfer_Crossbar_Feeding :
		case DS_Transfer_Crossbar_Lifting :

		case DS_Transfer_Tool_Locomotion :
		case DS_Transfer_Tool_Feeding :
		case DS_Transfer_Tool_Lifting :
		case DS_Transfer_Tool_Clamping :

			if( eOrientation == HorizontalX ) {
				switch( m_eGraphStyle )
				{
					case GS_Locomotion :
						cstrAxisLabel = _T("Feeding[mm]");
						break;												
						
					case GS_Displacement :
					case GS_Velocity :
					case GS_Acceleration :
						if(		m_eXAxisUnit == XAU_Time_Degree )	cstrAxisLabel = _T("[t˚]");
						else if(	m_eXAxisUnit == XAU_MG_Angle )		cstrAxisLabel = _T("[deg]");
						break;						
				}
			}
			else {
				switch( m_eGraphStyle )
				{
					case GS_Locomotion :		{ cstrAxisLabel = _T("Lifting[mm]");	break; }
					case GS_Displacement :	{ cstrAxisLabel = _T("[mm]");				break; }
					case GS_Velocity :		{ cstrAxisLabel = _T("[mm/s]");			break; }
					case GS_Acceleration :	{ cstrAxisLabel = _T("[mm/s²]");			break; }
				}
			}
			break;
			
		case DS_Transfer_Motor_R1_Motor :
		case DS_Transfer_Motor_L_Motor :
		case DS_Transfer_Motor_R2_Motor :
		case DS_Transfer_Motor_R3_Motor :

		case DS_Transfer_Motor_Feeding_Motor :
		case DS_Transfer_Motor_Lifting_Motor :
		case DS_Transfer_Motor_Clamping_Motor :

			if( eOrientation == HorizontalX ) {
				switch( m_eGraphStyle )
				{
					case GS_Displacement :
					case GS_Velocity :
					case GS_Acceleration :
					case GS_Torque :
					case GS_Speed :
					case GS_Power :
						if(		m_eXAxisUnit == XAU_Time_Degree )	cstrAxisLabel = _T("[t˚]");
						else if(	m_eXAxisUnit == XAU_MG_Angle )		cstrAxisLabel = _T("[deg]");
						break;
						
					case GS_TN_curve :
						cstrAxisLabel = _T("[rpm]");
						break;
				}
			}
			else {			
				switch( m_eGraphStyle )
				{
					case GS_Displacement :	{ cstrAxisLabel = _T("[deg]");		break; }
					case GS_Velocity :		{ cstrAxisLabel = _T("[deg/s]");		break; }
					case GS_Acceleration :	{ cstrAxisLabel = _T("[deg/s²]");	break; }
					case GS_Torque :			{ cstrAxisLabel = _T("[N·m]");		break; }
					case GS_Speed :			{ cstrAxisLabel = _T("[rpm]");		break; }
					case GS_TN_curve :		{ cstrAxisLabel = _T("[Nm]");			break; }
					case GS_Power :			{ cstrAxisLabel = _T("[KW]");			break; }
				}
			}
			break;			
	}

	return cstrAxisLabel;
}	

BOOL CGraphController::Set_AxisLabel( Orientation eOrientation )
{
	BOOL bRtnVal = FALSE;
	CString cstrAxisLabel = _T("");

	cstrAxisLabel = Get_AxisLabel( eOrientation );
	if( m_pIGraphCtrl ) {
		if( m_pIAxis[eOrientation] ) {
			m_pIAxis[eOrientation]->put_Label( ((_bstr_t) cstrAxisLabel) );
			bRtnVal = TRUE;
		}
	}

	return bRtnVal;
}

BOOL CGraphController::Set_AxisLabel( Orientation eOrientation, CString strAxisLabel )
{
	BOOL bRtnVal = FALSE;

	if( m_pIGraphCtrl ) {
		if( m_pIAxis[eOrientation] ) {
			m_pIAxis[eOrientation]->put_Label( ((_bstr_t) strAxisLabel) );
			bRtnVal = TRUE;
		}
	}

	return bRtnVal;
}	

BOOL CGraphController::Get_GraphStyle( GraphStyle * pGraphStyle )
{
	BOOL bRtnVal = FALSE;


	if( m_pIGraphCtrl ) {
		*pGraphStyle = m_pIGraphCtrl->GetGraphStyle();
		m_eGraphStyle = *pGraphStyle;
		bRtnVal  = TRUE;
	}
	else {
		*pGraphStyle = GS_Normal;
	}

	return bRtnVal;
}

BOOL CGraphController::Set_GraphStyle( GraphStyle eGraphStyle )
{
	BOOL bRtnVal = FALSE;


	if( m_pIGraphCtrl ) 
	{
		m_pIGraphCtrl->PutGraphStyle( eGraphStyle );
		m_eGraphStyle = eGraphStyle;

		// Graph Style이 변경 되면 Axis Lable도 변경해 주자.
		Set_AxisLabel( HorizontalX );
		Set_AxisLabel( VerticalY );

		bRtnVal  = TRUE;
	}

	return bRtnVal;
}

BOOL CGraphController::Get_DeviceStyle( DeviceStyle_Type * pDeviceStyle )
{
	BOOL bRtnVal = TRUE;


	*pDeviceStyle = m_eDeviceStyle;
	return bRtnVal;
}

BOOL CGraphController::Set_DeviceStyle( DeviceStyle_Type eDeviceStyle )
{
	BOOL bRtnVal = TRUE;


	m_eDeviceStyle = eDeviceStyle;

	// Device Style이 변경 되면 Axis Lable도 변경해 주자.
	Set_AxisLabel( HorizontalX );
	Set_AxisLabel( VerticalY );

	return bRtnVal;
}

BOOL CGraphController::Get_XAxisUnit( XAxisUnit_Type * pXAxisUnit )
{
	BOOL bRtnVal = TRUE;


	*pXAxisUnit = m_eXAxisUnit;
	return bRtnVal;
}

BOOL CGraphController::Set_XAxisUnit( XAxisUnit_Type eXAxisUnit )
{
	BOOL bRtnVal = TRUE;

		
	m_eXAxisUnit = eXAxisUnit;

	// Axis Unit이 변경 되면 Axis Lable도 변경해 주자.
	Set_AxisLabel( HorizontalX );
	Set_AxisLabel( VerticalY );

	return bRtnVal;
}

BOOL CGraphController::Set_GraphElement( CArray<double, double&> * pElementsX, CArray<double, double&> * pElementsY )
{
	BOOL	bRtnVal			= TRUE;
	UINT	uiElementCount	= 0;

	IDispatchPtr			pIDisPatch = NULL;
	IMPGraphElement *		pIGraphElement = NULL;


	if( !pElementsX || !pElementsY ) return FALSE;
	if( pElementsX->GetCount() <= pElementsY->GetCount() )	uiElementCount = pElementsX->GetCount();
	else																		uiElementCount = pElementsY->GetCount();
	
	if( !m_pIElementCollection ) return FALSE;
	pIDisPatch = m_pIElementCollection->Add();
	pIDisPatch->QueryInterface( __uuidof(IMPGraphElement), (LPVOID*) &pIGraphElement );
	pIDisPatch->Release(); pIDisPatch = NULL;

	if( uiElementCount < 1 ) { bRtnVal = FALSE; return bRtnVal; }

	COleSafeArray arrx, arry;
	arrx.CreateOneDim( VT_R8, uiElementCount );
	arry.CreateOneDim( VT_R8, uiElementCount );

	double ValueX, ValueY;
	for( long lIdx = 0; lIdx < uiElementCount; lIdx++ ) 
	{
		ValueX = pElementsX->ElementAt( lIdx );
		ValueY = pElementsY->ElementAt( lIdx );

		arrx.PutElement( &lIdx, &ValueX );
		arry.PutElement( &lIdx, &ValueY );
	}

	pIGraphElement->put_Name(_bstr_t("sin"));
	pIGraphElement->Plot(COleVariant(arrx), COleVariant(arry));

	m_pIGraphCtrl->AutoRange();

	// 속도 / 가속도 Graph일 경우 V max / -V max 보이도록 Range를 설정 한다.
	// younghyun.oh 2016-04-16 block {
	// 모든 Graph의 Range에 여유 분을 주자.
	#if 0
	if( (m_eGraphStyle == GS_Velocity) || (m_eGraphStyle == GS_Acceleration) ) 
	#endif
	// younghyun.oh 2016-04-16 block }

	{
		double	dHMin, dHMax;
		double	dRangeWidth,	dRangeWidthOffset;

		double	dVMin, dVMax;
		double	dRangeHeight,	dRangeHeightOffset;
		double	dAutoRangeX[2], dAutoRangeY[2];

		m_pIGraphCtrl->GetAutoRange( &dAutoRangeX[0], &dAutoRangeX[1], &dAutoRangeY[0], &dAutoRangeY[1]);

		dHMin = dAutoRangeX[0]; dHMax = dAutoRangeX[1];
		dRangeWidth = fabs(dHMax - dHMin);
		dRangeWidthOffset = dRangeWidth / 10.0l;

		dVMin = dAutoRangeY[0]; dVMax = dAutoRangeY[1];
		dRangeHeight = fabs(dVMax - dVMin);
		dRangeHeightOffset = dRangeHeight / 10.0l;

		m_pIGraphCtrl->SetRange( (dHMin - dRangeWidthOffset),		(dHMax + dRangeWidthOffset)
										,(dVMin - dRangeHeightOffset),	(dVMax + dRangeHeightOffset) );
	}

// for debugging.
#if 0
	double AutonRange[4];
	m_pIGraphCtrl->GetAutoRange( &AutonRange[0], &AutonRange[1], &AutonRange[2], &AutonRange[3] );

	double nRange[4];
	m_pIGraphCtrl->GetRange( &nRange[0], &nRange[1], &nRange[2], &nRange[3] );
#endif

	return bRtnVal;
}
