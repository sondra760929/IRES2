// 컴퓨터에서 형식 라이브러리 마법사의 [클래스 추가]를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

#import "E:\\source\\MotionPlanner_Src\\x64\\Release\\MPGraph.ocx" no_namespace
// CMPGraphCollection 래퍼 클래스

class CMPGraphCollection : public COleDispatchDriver
{
public:
	CMPGraphCollection() {} // COleDispatchDriver 기본 생성자를 호출합니다.
	CMPGraphCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMPGraphCollection(const CMPGraphCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 특성
public:

	// 작업
public:


	// IMPGraphCollection 메서드
public:
	long get_Count()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Item(long index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, index);
		return result;
	}
	long get_Selected()
	{
		long result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_Selected(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	LPDISPATCH Add()
	{
		LPDISPATCH result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	void Delete(long index)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, index);
	}

	// IMPGraphCollection 속성
public:

};
