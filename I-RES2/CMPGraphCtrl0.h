// CMPGraphCtrl0.h  : Microsoft Visual C++로 만든 ActiveX 컨트롤 래퍼 클래스의 선언입니다.

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CMPGraphCtrl0

class CMPGraphCtrl0 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CMPGraphCtrl0)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xCAC3B4DD, 0x754A, 0x4C86, { 0xAE, 0xB8, 0xBE, 0xCB, 0x11, 0x13, 0xA6, 0xA2 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext = nullptr)
	{
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
				BSTR bstrLicKey = nullptr)
	{
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey);
	}

// 특성입니다.
public:

// 작업입니다.
public:

	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(DISPID_CAPTION, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	void put_Appearance(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	short get_Appearance()
	{
		short result;
		InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, nullptr);
		return result;
	}
	unsigned long get_AxisColor()
	{
		unsigned long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_AxisColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	unsigned long get_GridColor()
	{
		unsigned long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_GridColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	unsigned long get_LabelColor()
	{
		unsigned long result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_LabelColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_LabelFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_LabelFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_TickFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_TickFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_TitleFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_TitleFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_IdentFont()
	{
		LPDISPATCH result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_IdentFont(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_ElementIdentify()
	{
		BOOL result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_ElementIdentify(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_TrackMode()
	{
		long result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_TrackMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_ControlFramePicture()
	{
		LPDISPATCH result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_ControlFramePicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_PlotAreaPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void put_PlotAreaPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	unsigned long get_ControlFrameColor()
	{
		unsigned long result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_ControlFrameColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	unsigned long get_PlotAreaColor()
	{
		unsigned long result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_PlotAreaColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_FrameStyle()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_FrameStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH get_Annotations()
	{
		LPDISPATCH result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Elements()
	{
		LPDISPATCH result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Cursors()
	{
		LPDISPATCH result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Axis(long eOrientation)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, eOrientation);
		return result;
	}
	void SetRange(double xmin, double xmax, double ymin, double ymax)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, xmin, xmax, ymin, ymax);
	}
	void GetRange(double * xmin, double * xmax, double * ymin, double * ymax)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, xmin, xmax, ymin, ymax);
	}
	void AutoRange()
	{
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void GetAutoRange(double * xmin, double * xmax, double * ymin, double * ymax)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, xmin, xmax, ymin, ymax);
	}
	void CopyToClipboard()
	{
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void PrintGraph()
	{
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void ClearGraph()
	{
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void ShowProperties()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void SaveAs(LPCTSTR szFilename)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, szFilename);
	}
	long get_GraphStyle()
	{
		long result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_GraphStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_AnimationMode()
	{
		BOOL result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_AnimationMode(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	void GetAnimationPoint(double * XElementValue)
	{
		static BYTE parms[] = VTS_PR8 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, XElementValue);
	}
	void SetAnimationPoint(double XElementValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, XElementValue);
	}
	void GetAnimationIndex(unsigned long * XElementIndex)
	{
		static BYTE parms[] = VTS_PUI4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, XElementIndex);
	}
	void SetAnimationIndex(unsigned long XElementIndex)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, XElementIndex);
	}
	void GetSlideMaxVelocity(double * dSlideMaxVelocity)
	{
		static BYTE parms[] = VTS_PR8 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dSlideMaxVelocity);
	}
	void SetSlideMaxVelocity(double dSlideMaxVelocity)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dSlideMaxVelocity);
	}
	BOOL get_GuideBarEnabled()
	{
		BOOL result;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_GuideBarEnabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	void GetTnCurveEllipseOrign(double * dpRatedSpeed, double * dpRatedTorque)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dpRatedSpeed, dpRatedTorque);
	}
	void SetTnCurveEllipseOrign(double dRatedSpeed, double dRatedTorque)
	{
		static BYTE parms[] = VTS_R8 VTS_R8 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dRatedSpeed, dRatedTorque);
	}
	void SetLocomotionLoadingPoints(VARIANT newXVals)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, &newXVals);
	}
	void ClearToolTipText()
	{
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}


};
