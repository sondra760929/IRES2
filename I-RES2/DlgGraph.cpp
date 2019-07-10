// DlgGraph.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgGraph.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CDlgGraph, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgGraph 메시지 처리기


BOOL CDlgGraph::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgGraph::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);


}
