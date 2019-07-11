// DlgGraph.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgGraph.h"
#include "afxdialogex.h"
#include "OptImportExportBase.h"
#include "CMPGraphCollection.h"
#include "CMPGraphElement.h"
// CDlgGraph 대화 상자

IMPLEMENT_DYNAMIC(CDlgGraph, CDialog)

CDlgGraph::CDlgGraph(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_GRAPH, pParent)
{

}

CDlgGraph::~CDlgGraph()
{
}

void CDlgGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MPGRAPHCTRL1, m_ctrlGraph);
}


BEGIN_MESSAGE_MAP(CDlgGraph, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgGraph 메시지 처리기


BOOL CDlgGraph::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlGraph.put_GraphStyle(1);
	CMPGraphCollection elements(m_ctrlGraph.get_Elements());
	CMPGraphElement element1(elements.Add());
	CMPGraphElement element2(elements.Add());
	CMPGraphElement element3(elements.Add());
	CMPGraphElement element4(elements.Add());
	element1.put_LineColor(RGB(255, 0, 0));
	element2.put_LineColor(RGB(0, 255, 0));
	element3.put_LineColor(RGB(0, 0, 255));
	element4.put_LineColor(RGB(255, 255, 0));
	//m_pGraphController->Set_GraphStyle( GS_Displacement );
	//m_pGraphController->Set_DeviceStyle( DS_Transfer_Crossbar_Feeding );
	//m_pGraphController->Set_XAxisUnit( XAU_MG_Angle );
	CString job_file;
	job_file = m_strProjectPath + "\\JOB\\" + m_strJobName + "\\ice_result.OUT";
	if (PathFileExists(job_file))
	{
		FILE* fp;
		fopen_s(&fp, job_file, "rt");
		if (fp)
		{
			COptImportExportBase ifp;
			ifp.m_fp_input = fp;
			ifp.m_array_strSplit.push_back(' ');
			int row_count = 0;
			float prev_x = 0;
			if (ifp.ReadOneLineFromFile() > 6)
			{
			}
			while (ifp.ReadOneLineFromFile() > 6)
			{
				float x = atof(ifp.m_array_strOutput[1]);
				if (x < prev_x)
				{

				}

				element1.PlotXY(x, atof(ifp.m_array_strOutput[3]));
				element2.PlotXY(x, atof(ifp.m_array_strOutput[4]));
				element3.PlotXY(x, atof(ifp.m_array_strOutput[5]));
				element4.PlotXY(x, atof(ifp.m_array_strOutput[6]));
				prev_x = x;
			}
		}
	}
	m_ctrlGraph.AutoRange();

	//IDispatchPtr			pIDisPatch = NULL;
	//IMPGraphElement *		pIGraphElement = NULL;
	//
	//
	//CRect	rGraphPos;
	//rGraphPos.left		= 0;
	//rGraphPos.top		= 0;
	//rGraphPos.right	= 400;
	//rGraphPos.bottom	= 400;

	//if( !m_pGraphController ) m_pGraphController = new CGraphController( this->m_hWnd, IDC_MPGRAPHCTRL1 );
	//m_pGraphController->SetWindowPos( CWnd::FromHandle(this->m_hWnd), rGraphPos );

	//m_pGraphController->Set_GraphStyle( GS_Displacement );
	//m_pGraphController->Set_DeviceStyle( DS_Transfer_Crossbar_Feeding );
	//m_pGraphController->Set_XAxisUnit( XAU_MG_Angle );
	//

	//pIDisPatch = m_pGraphController->m_pIElementCollection->Add();
	//pIDisPatch->QueryInterface( __uuidof(IMPGraphElement), (LPVOID*) &pIGraphElement );
	//pIDisPatch->Release(); pIDisPatch = NULL;

	//pIGraphElement->put_Name(_bstr_t("sin"));
 //
	//{
	//	COleSafeArray arrx, arry;
	//	arrx.CreateOneDim(VT_R8, 100);
	//	arry.CreateOneDim(VT_R8, 100);
	//	long i;
	//	for(i=0; i<100; i++)
	//	{
	//		double x, y;
	//		x = i/10.;
	//		y = sin(x);

	//		arrx.PutElement(&i, &x);
	//		arry.PutElement(&i, &y);
	//	}

	//	pIGraphElement->Plot(COleVariant(arrx), COleVariant(arry));
	//}

	//m_pGraphController->m_pIGraphCtrl->AutoRange();
	////m_pGraphController->m_pIGraphCtrl->SetRange(-2.0, 11.0, -1.5, 1.5);

	//m_pGraphController->m_pIGraphCtrl->put_AnimationMode( TRUE );
	//m_pGraphController->m_pIGraphCtrl->SetAnimationPoint( 5.0 );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgGraph::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_ctrlGraph)
	{
		m_ctrlGraph.MoveWindow(0, 0, cx, cy);
	}
}
