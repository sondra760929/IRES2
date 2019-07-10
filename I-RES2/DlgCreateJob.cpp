// DlgCreateJob.cpp: 구현 파일
//

#include "stdafx.h"
#include "I-RES2.h"
#include "DlgCreateJob.h"
#include "afxdialogex.h"


// CDlgCreateJob 대화 상자

IMPLEMENT_DYNAMIC(CDlgCreateJob, CDialog)

CDlgCreateJob::CDlgCreateJob(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CREATE_JOB, pParent)
	, m_strJobName(_T(""))
{

}

CDlgCreateJob::~CDlgCreateJob()
{
}

void CDlgCreateJob::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strJobName);
}


BEGIN_MESSAGE_MAP(CDlgCreateJob, CDialog)
END_MESSAGE_MAP()


// CDlgCreateJob 메시지 처리기
