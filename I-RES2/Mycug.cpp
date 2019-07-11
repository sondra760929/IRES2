/***************************************************
****************************************************
Skeleton Class for a Derived MyCug
****************************************************
****************************************************/

#include "stdafx.h"
#include "MainFrm.h"
#include "resource.h"
#include "MyCug.h"
//#include "PatternDBDlg.h"
//#include "DlgPortFields.h"
//#include "DlgSignalField.h"
//#include "DlgConnectPortField.h"
//#include "DlgSetMaterial.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MyCug, CUGCtrl)
	//{{AFX_MSG_MAP(MyCug)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/***************************************************
****************************************************/
MyCug::MyCug()
: m_pParentDlg(0)
{
	/*m_iSortCol = 0;
	m_bSortedAscending = true;*/
}
/***************************************************
****************************************************/
MyCug::~MyCug()
{
	UGXPThemes::CleanUp();
}

/***************************************************
OnSetup
This function is called just after the grid window
is created or attached to a dialog item.
It can be used to initially setup the grid
****************************************************/
void MyCug::OnSetup(){
	//// initialize local variables
	//int rows = 20;
	//int cols = 20;
	//int i,j;
	//CString temp;
	//CUGCell		cell;

	//// set up rows and columns
	//SetNumberRows(rows);
	//SetNumberCols(cols);

	//// fill-in cells with data
	//GetCell(0,1,&cell);
	//for (i = 0; i < cols; i++)
	//	for (j = 0; j < rows; j++) {
	//		temp.Format(_T("%d"),(i+1)*(j+1));
	//		cell.SetText(temp);
	//		SetCell(i,j,&cell);
	//	}

	//// add column headings
	//for (i = 0; i < cols; i++) {
	//	temp.Format(_T("%d"),(i+1));
	//	cell.SetText(temp);
	//	SetCell(i,-1,&cell);
	//}

	//// add row headingsgon
	//for (j = 0; j < rows; j++) {
	//	temp.Format(_T("%d"),(j+1));
	//	cell.SetText(temp);
	//	SetCell(-1,j,&cell);
	//}

	//AdjustComponentSizes();
}

void MyCug::SetType1()
{
	SetNumberRows(5);
	SetNumberCols(8);

	// fill-in cells with data
	CUGCell		cell;
	CString temp;
	GetCell(0, 1, &cell);

	cell.SetText(_T("Material"));
	SetCell(0, -1, &cell);
	cell.SetText(_T("Heat Type"));
	SetCell(1, -1, &cell);
	cell.SetText(_T("Heat Depth"));
	SetCell(2, -1, &cell);
	cell.SetText(_T("Heat Velocity"));
	SetCell(3, -1, &cell);
	cell.SetText(_T("Heat Distance"));
	SetCell(4, -1, &cell);
	cell.SetText(_T("Heat Time"));
	SetCell(5, -1, &cell);
	cell.SetText(_T("Heat Round Type"));
	SetCell(6, -1, &cell);
	cell.SetText(_T("Heat Temperature"));
	SetCell(7, -1, &cell);

	for (int i = 0; i<5; i++)
	{
		temp.Format(_T("section %d"), i + 1);
		cell.SetText(temp);
		SetCell(-1, i, &cell);
	}

	CString data[5][9] = {
		_T("M101"), _T("H1"), _T("20"), _T("10"), _T("20"), _T("10"), _T("3"), _T("80"), _T("20"),
		_T("M102"), _T("H1"), _T("40"), _T("20"), _T("20"), _T("20"), _T("3"), _T("80"), _T("30"),
		_T("M103"), _T("H1"), _T("60"), _T("30"), _T("40"), _T("30"), _T("4"), _T("100"), _T("20"),
		_T("M104"), _T("H1"), _T("80"), _T("40"), _T("40"), _T("40"), _T("4"), _T("100"), _T("30"),
		_T("M105"), _T("H1"), _T("100"), _T("50"), _T("60"), _T("50"), _T("5"), _T("120"), _T("40") };

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<8; j++)
		{
			cell.SetText(data[i][j]);
			SetCell(j, i, &cell);
		}
	}
	AdjustComponentSizes();
	BestFit(-1, 7, 0, UG_BESTFIT_TOPHEADINGS);
}

void MyCug::SetType2()
{
	SetNumberRows(10);
	SetNumberCols(8);

	// fill-in cells with data
	CUGCell		cell;
	CString temp;
	GetCell(0, 1, &cell);

	cell.SetText(_T("Material"));
	SetCell(0, -1, &cell);
	cell.SetText(_T("Heat Type"));
	SetCell(1, -1, &cell);
	cell.SetText(_T("Heat Depth"));
	SetCell(2, -1, &cell);
	cell.SetText(_T("Heat Velocity"));
	SetCell(3, -1, &cell);
	cell.SetText(_T("Heat Distance"));
	SetCell(4, -1, &cell);
	cell.SetText(_T("Heat Time"));
	SetCell(5, -1, &cell);
	cell.SetText(_T("Heat Round Type"));
	SetCell(6, -1, &cell);
	cell.SetText(_T("Heat Temperature"));
	SetCell(7, -1, &cell);

	for (int i = 0; i<10; i++)
	{
		temp.Format(_T("section %d"), i + 1);
		cell.SetText(temp);
		SetCell(-1, i, &cell);
	}

	CString data[10][9] = {
		_T("M1"), _T("Normal"), _T("10"), _T("20"), _T("20"), _T("10"), _T("1"), _T("80"), _T("20"),
		_T("M2"), _T("Normal"), _T("20"), _T("20"), _T("20"), _T("20"), _T("1"), _T("80"), _T("20"),
		_T("M3"), _T("Normal"), _T("30"), _T("20"), _T("20"), _T("30"), _T("1"), _T("80"), _T("20"),
		_T("M4"), _T("Normal"), _T("40"), _T("20"), _T("40"), _T("40"), _T("1"), _T("100"), _T("20"),
		_T("M5"), _T("Normal"), _T("50"), _T("20"), _T("40"), _T("50"), _T("2"), _T("100"), _T("30"),
		_T("M6"), _T("Normal"), _T("60"), _T("30"), _T("20"), _T("10"), _T("2"), _T("100"), _T("30"),
		_T("M7"), _T("Normal"), _T("70"), _T("30"), _T("20"), _T("20"), _T("2"), _T("120"), _T("30"),
		_T("M8"), _T("Normal"), _T("80"), _T("30"), _T("40"), _T("30"), _T("2"), _T("120"), _T("30"),
		_T("M9"), _T("Normal"), _T("90"), _T("30"), _T("40"), _T("40"), _T("3"), _T("120"), _T("40"),
		_T("M10"), _T("Normal"), _T("100"), _T("50"), _T("40"), _T("50"), _T("3"), _T("150"), _T("40") };

	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<8; j++)
		{
			cell.SetText(data[i][j]);
			SetCell(j, i, &cell);
		}
	}
	AdjustComponentSizes();
	BestFit(-1, 7, 0, UG_BESTFIT_TOPHEADINGS);
}


void MyCug::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if (row > -1)
	{
		StartEdit();
		if (m_pParentDlg)
		{
			m_pParentDlg->SendMessage(WM_GRD_LCLICK_MSG, col, row);
		}
	}
	

}

void MyCug::OnDClicked(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	if (m_pParentDlg)
	{
		m_pParentDlg->SendMessage(WM_GRD_DCLICK_MSG, col, row);
	}
}




void MyCug::OnMouseMove(int col, long row, POINT *point, UINT nFlags, BOOL processed)
{
	if (m_pParentDlg)
	{
		m_pParentDlg->SendMessage(WM_GRD_MOUSEMOVE_MSG, col, row);
	}
}

void MyCug::OnTH_LClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{


	bool temp = updn;
	if (m_pParentDlg && updn)
	{
		m_pParentDlg->SendMessage(WM_GRD_THLCLICK_MSG, col, row);
	}
}
	
void MyCug::OnCharDown(UINT* vcKey, BOOL processed)
{
	StartEdit(*vcKey);
}


int MyCug::OnCellTypeNotify(long ID, int col, long row, long msg, LONG_PTR param)
{
	CUGCell cell;
	int retval = TRUE;

	if (msg == UGCT_DROPLISTPOSTSELECT && m_pParentDlg)
	{
		CString* select_comboItem = (CString*)param;
		m_pParentDlg->SendMessage(WM_GRD_CELL_NOTIFY, col, row);
	}
	return retval;
}


int MyCug::OnEditKeyDown(int col, long row, CWnd *edit, UINT *vcKey)
{
	CString text;
	CUGCell cell;
	if (col > -1 && row > -1)
	{
		if (*vcKey == VK_DELETE)
		{
			if (m_pParentDlg)
			{
				m_pParentDlg->SendMessage(WM_GRD_DELETE_KEY, col, row);
			}

		}
	}
	

	return 0;
}


int MyCug::OnKeyDown(int col, long row, CWnd *edit, LPCTSTR string, BOOL cancelFlag)
{

	return 0;
}

int MyCug::OnEditStart(int col, long row, CWnd **edit)
{
	if (row <= -1)
	{
		return false;
	}

	if (m_sMyCugName != "ConnectEditorField")
	{

		CUGCell cell;
		GetCellIndirect(col, row, &cell);
		DWORD dwStyle = 0;

		if (cell.GetCellType() == UGCT_DROPLIST)
		{
			return FALSE;
		}
		else
		{
			((CEdit*)*edit)->ModifyStyle(ES_UPPERCASE, 0);
			((CEdit*)*edit)->ModifyStyle(ES_LOWERCASE, 0);
			((CEdit*)*edit)->ModifyStyle(ES_PASSWORD, 0);
		}
		origin_name = cell.GetText();
		return true;
	}

}
//(int col,long row,CWnd *edit,UINT *vcKey)
int MyCug::OnEditVerify(int col, long row, CWnd *edit, UINT *vcKey)
{
	
	if (row <= -1)
	{
		return false;
	}

	CUGCell type_cell;
	CUGCell value_cell;
	
	CString string;

	if (m_sMyCugName == "SignalField")
	{
		if (col == 2)
		{
			string += (char)*vcKey;
		}
	}
	else if (m_sMyCugName == "PortField")
	{
		if (col == 0 || col == 3)
		{
			string += (char)*vcKey;
		}
	}
	else if (m_sMyCugName == "MaterialField")
	{
		if (col == 1 || col == 2)
		{
			if ((int)*vcKey >= 48 && (int)*vcKey <= 57)
			{
				string += (char)*vcKey;
			}
			else
			{
				return false;
			}
			
		}
	}
	return TRUE;
}

int MyCug::OnEditFinish(int col, long row, CWnd * edit, LPCTSTR string, BOOL cancelFlag)
{
	UNREFERENCED_PARAMETER(edit);

	
	///*if (string != origin_name)
	//{*/
	//	int result;
	//	int result2;

	//	CUGCell cell;
	//	GetCellIndirect(col, row, &cell);
	//	if (cancelFlag != TRUE)
	//	{
	//		if (m_sMyCugName == "SignalField")
	//		{
	//			CDlgSignalField* dlg_sigfield = (CDlgSignalField*)m_pParentDlg;
	//			result = dlg_sigfield->PortNameCheck(string);
	//			result2 = dlg_sigfield->SignalNameCheck(string);
	//			if (result == 1 && result == 1)
	//			{
	//				dlg_sigfield->SetRowData(col, row, string);
	//				return TRUE;
	//			}
	//			else
	//			{
	//				edit->SetWindowTextA(origin_name);
	//				return false;
	//			}
	//		}
	//		else if (m_sMyCugName == "PortField")
	//		{
	//			//port fields
	//			CDlgPortFields* dlg_portfield = (CDlgPortFields*)m_pParentDlg;
	//			result = dlg_portfield->PortNameCheck(string);
	//			result2 = dlg_portfield->SignalNameCheck(string);
	//			if (result > -1 && result2 > -1)
	//			{
	//				//같은 이름이 있으면
	//				edit->SetWindowTextA(origin_name);
	//				return false;
	//			}
	//			else
	//			{
	//				dlg_portfield->SetRowData(col, row, string);
	//				return TRUE;
	//			}
	//		}
	//		else if (m_sMyCugName == "MaterialField")
	//		{
	//			CDlgSetMaterial* dlg_material = (CDlgSetMaterial*)m_pParentDlg;
	//			dlg_material->SetRowData(col, row, string);
	//			return TRUE;
	//		}
	//		else
	//		{
	//			return true;
	//		}
	//	}
	//}
	//else
	//{
	//	//입력된 이름이 중복될 경우 기존이름으로 셀에 입력 후 반영.
	//	return true;
	//}
	
	return FALSE;
	
}