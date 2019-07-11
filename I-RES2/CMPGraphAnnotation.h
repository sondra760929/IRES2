// ��ǻ�Ϳ��� ���� ���̺귯�� �������� [Ŭ���� �߰�]�� ����Ͽ� ������ IDispatch ���� Ŭ�����Դϴ�.

#import "E:\\source\\MotionPlanner_Src\\x64\\Release\\MPGraph.ocx" no_namespace
// CMPGraphAnnotation ���� Ŭ����

class CMPGraphAnnotation : public COleDispatchDriver
{
public:
	CMPGraphAnnotation() {} // COleDispatchDriver �⺻ �����ڸ� ȣ���մϴ�.
	CMPGraphAnnotation(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMPGraphAnnotation(const CMPGraphAnnotation& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Ư��
public:

	// �۾�
public:


	// IMPGraphAnnotation �޼���
public:
	CString get_LabelCaption()
	{
		CString result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	void put_LabelCaption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	double get_LabelX()
	{
		double result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, nullptr);
		return result;
	}
	void put_LabelX(double newValue)
	{
		static BYTE parms[] = VTS_R8;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	double get_LabelY()
	{
		double result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, nullptr);
		return result;
	}
	void put_LabelY(double newValue)
	{
		static BYTE parms[] = VTS_R8;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	unsigned long get_LabelColor()
	{
		unsigned long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_LabelColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_LabelHorizontal()
	{
		BOOL result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_LabelHorizontal(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	unsigned long get_LabelBkColor()
	{
		unsigned long result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, nullptr);
		return result;
	}
	void put_LabelBkColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}

	// IMPGraphAnnotation �Ӽ�
public:

};
