// ��ǻ�Ϳ��� ���� ���̺귯�� �������� [Ŭ���� �߰�]�� ����Ͽ� ������ IDispatch ���� Ŭ�����Դϴ�.

#import "E:\\source\\MotionPlanner_Src\\x64\\Release\\MPGraph.ocx" no_namespace
// CIMPGraphCtrlEvents ���� Ŭ����

class CIMPGraphCtrlEvents : public COleDispatchDriver
{
public:
	CIMPGraphCtrlEvents() {} // COleDispatchDriver �⺻ �����ڸ� ȣ���մϴ�.
	CIMPGraphCtrlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CIMPGraphCtrlEvents(const CIMPGraphCtrlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Ư��
public:

	// �۾�
public:


	// _IMPGraphCtrlEvents �޼���
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

	// _IMPGraphCtrlEvents �Ӽ�
public:

};
