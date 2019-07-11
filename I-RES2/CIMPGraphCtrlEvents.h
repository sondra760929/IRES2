// 컴퓨터에서 형식 라이브러리 마법사의 [클래스 추가]를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

#import "E:\\source\\MotionPlanner_Src\\x64\\Release\\MPGraph.ocx" no_namespace
// CIMPGraphCtrlEvents 래퍼 클래스

class CIMPGraphCtrlEvents : public COleDispatchDriver
{
public:
	CIMPGraphCtrlEvents() {} // COleDispatchDriver 기본 생성자를 호출합니다.
	CIMPGraphCtrlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CIMPGraphCtrlEvents(const CIMPGraphCtrlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// _IMPGraphCtrlEvents 메서드
public:
	void CursorPosition(double X, double Y)
	{
		static BYTE parms[] = VTS_R8 VTS_R8;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, X, Y);
	}
	void TrackModeChanged(long lNewState)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, lNewState);
	}
	void Click()
	{
		InvokeHelper(DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}

	// _IMPGraphCtrlEvents 속성
public:

};
