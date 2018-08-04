// CDlgProgress.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "CDlgProgress.h"
#include "afxdialogex.h"


// CDlgProgress 대화 상자

IMPLEMENT_DYNAMIC(CDlgProgress, CDialog)

CDlgProgress::CDlgProgress(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PROGRESS, pParent)
{

}

CDlgProgress::~CDlgProgress()
{
}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_stStatus);
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
END_MESSAGE_MAP()


// CDlgProgress 메시지 처리기
