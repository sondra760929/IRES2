#include "StdAfx.h"
#include "OptImportExportBase.h"
//#include "OptSubObject.h"
//#include "OptCell.h"
//#include "HTools.h"

COptImportExportBase::COptImportExportBase(bool b_make_log)
: m_fp_input(NULL)
, m_fp_output(NULL)
, m_fp_log(NULL)
, m_iSplitCount(0)
, m_iFileCount(0)
, m_iFileIndex(0)
, m_iByteCount(0)
, m_iByteIndex(0)
, m_bMakeLog(b_make_log)
{
	m_array_strSplit.push_back(' ');
}

COptImportExportBase::~COptImportExportBase(void)
{
	if(m_fp_input)
	{
		fclose(m_fp_input);
		m_fp_input = NULL;
	}

	if(m_fp_output)
	{
		fclose(m_fp_output);
		m_fp_output = NULL;
	}

	if(m_fp_log)
	{
		fclose(m_fp_log);
		m_fp_log = NULL;
	}
}

bool COptImportExportBase::OpenInputFile(CString input_file_path)
{
	if(m_fp_input)
	{
		fclose(m_fp_input);
		m_fp_input = NULL;
	}

	CFileFind ffind;
	if(ffind.FindFile(input_file_path))
	{
		ffind.FindNextFile();
		_tfopen_s(&m_fp_input, input_file_path, _T("r"));
		if(m_fp_input)
		{
			m_strInputFilePath = input_file_path;
			m_iByteCount = ffind.GetLength();
			m_iByteIndex = 0;
			m_iCurrentLineNo = 0;
			if(m_bMakeLog)
			{
				OpenLogFile(input_file_path);
			}

			return true;
		}
	}
	return false;
}

bool COptImportExportBase::OpenOutputFile(CString output_file_path)
{
	//if(m_fp_output)
	//{
	//	fclose(m_fp_output);
	//	m_fp_output = NULL;
	//}

	//m_fp_output = fopen(H_ASCII_TEXT(output_file_path), "w");
	//if(m_fp_output)
	//{
	//	m_strOutputFilePath = output_file_path;
	//	if(m_bMakeLog)
	//	{
	//		OpenLogFile(output_file_path);
	//	}

	//	return true;
	//}

	return false;
}

bool COptImportExportBase::OpenLogFile(CString output_file_path)
{
	if(m_fp_log)
	{
		fclose(m_fp_log);
		m_fp_log = NULL;
	}

	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	sprintf_s(path_buffer, "%s", output_file_path);
	_splitpath_s( path_buffer, drive, dir, fname, ext ); // C4996

	CString log_file_path;
	log_file_path.Format(_T("%s%s%s.log"), drive, dir, fname);

	_tfopen_s(&m_fp_log, log_file_path, _T("r"));
	if (m_fp_log)
	{
		m_strLogFilePath = log_file_path;
		return true;
	}

	return false;
}

int COptImportExportBase::ReadOneLineFromFile()
{
	MSG msg;
	try
	{
		while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if (m_fp_input)
		{
			m_iSplitCount = -1;
			if (fgets(m_str_line_from_file, 1024, m_fp_input) != NULL)
			{
				m_strOneLine = CString(m_str_line_from_file);
				//m_strOneLine.Format(_T("%ws"), m_str_line_from_file);
				m_strOneLine.Replace('\n', '\0');
				m_array_strOutput.clear();
				m_iSplitCount = COptUtil::SplitString(m_str_line_from_file, m_array_strSplit, m_array_strOutput);
				m_iCurrentLineNo++;
				WriteLog((LPCTSTR)m_str_line_from_file);

				//fprintf(stderr, "[%d] %s\n", m_iCurrentLineNo, m_str_line_from_file);
			}
		}
	}
	catch (int execption)
	{
		m_iSplitCount = -1;
	}
	return m_iSplitCount;
}

void COptImportExportBase::ReadDataFromFile(int& param1)
{
	if(ReadOneLineFromFile() != 1)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _ttoi(m_array_strOutput[0]);
}

void COptImportExportBase::ReadDataFromFile(int& param1, int& param2)
{
	if(ReadOneLineFromFile() != 2)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _ttoi(m_array_strOutput[0]);
	param2 = _ttoi(m_array_strOutput[1]);
}

void COptImportExportBase::ReadDataFromFile(int& param1, int& param2, int& param3)
{
	if(ReadOneLineFromFile() != 3)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _ttoi(m_array_strOutput[0]);
	param2 = _ttoi(m_array_strOutput[1]);
	param3 = _ttoi(m_array_strOutput[2]);
}

void COptImportExportBase::ReadDataFromFile(int& param1, int& param2, int& param3, int& param4)
{
	if(ReadOneLineFromFile() != 4)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _ttoi(m_array_strOutput[0]);
	param2 = _ttoi(m_array_strOutput[1]);
	param3 = _ttoi(m_array_strOutput[2]);
	param4 = _ttoi(m_array_strOutput[3]);
}

void COptImportExportBase::ReadDataFromFile(float& param1)
{
	if(ReadOneLineFromFile() != 1)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _tstof(m_array_strOutput[0]);
}

void COptImportExportBase::ReadDataFromFile(float& param1, float& param2)
{
	if(ReadOneLineFromFile() != 2)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _tstof(m_array_strOutput[0]);
	param2 = _tstof(m_array_strOutput[1]);
}

void COptImportExportBase::ReadDataFromFile(float& param1, float& param2, float& param3)
{
	if(ReadOneLineFromFile() != 3)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _tstof(m_array_strOutput[0]);
	param2 = _tstof(m_array_strOutput[1]);
	param3 = _tstof(m_array_strOutput[2]);
}

void COptImportExportBase::ReadDataFromFile(float& param1, float& param2, float& param3, float& param4)
{
	if(ReadOneLineFromFile() != 4)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = _tstof(m_array_strOutput[0]);
	param2 = _tstof(m_array_strOutput[1]);
	param3 = _tstof(m_array_strOutput[2]);
	param4 = _tstof(m_array_strOutput[3]);
}

void COptImportExportBase::ReadDataFromFile(CString& param1)
{
	if(ReadOneLineFromFile() != 1)
	{
		//throw COUNT_IN_LINE_ERROR;
	}
	param1 = m_array_strOutput[0];
}

void COptImportExportBase::ReadUndefinedLinesFromFile(int line_count)
{
	if(m_fp_input)
	{
		if(m_fp_log)
		{
			fprintf(m_fp_log, "%d : 아래로 %d 줄 무시\n", m_iCurrentLineNo, line_count);
		}
		for(int i=0; i<line_count; ++i)
		{
			if(fgets(m_str_line_from_file, 1024, m_fp_input) != NULL)
			{
				m_iCurrentLineNo++;
				if(m_fp_log)
				{
					fprintf(m_fp_log, "%d : => %s", m_iCurrentLineNo, m_str_line_from_file);
				}
			}
		}
	}
}

void COptImportExportBase::WriteLog(CString str_format)
{
	if(m_fp_log)
	{
		fprintf(m_fp_log, "%d : %s", m_iCurrentLineNo, str_format);
	}
	//TRACE("%d : %s", m_iCurrentLineNo, str_format);
}

float COptImportExportBase::ReadValue(CString str_format)
{
	int index = str_format.Find(_T("e"));
	if (index > 0)
	{
		float result_value = _tstof(str_format.Left(index));
		str_format.Delete(0, index+1);
		float result_exp = _tstof(str_format);
		return result_value * pow(10.0f, result_exp);
	}
	return _tstof(str_format);
}

void COptImportExportBase::WriteLog(CString str_format, int i_value)
{
	if(m_fp_log)
	{
		fprintf(m_fp_log, "%d : ", m_iCurrentLineNo);
		fprintf(m_fp_log, (str_format), i_value);
	}
	//TRACE("%d : ", m_iCurrentLineNo);
	//TRACE(str_format, i_value);
}

void COptImportExportBase::WriteLog(CString str_format, CString str_value)
{
	if(m_fp_log)
	{
		fprintf(m_fp_log, "%d : ", m_iCurrentLineNo);
		fprintf(m_fp_log, (str_format), str_value);
	}
	//TRACE("%d : ", m_iCurrentLineNo);
	//TRACE(str_format, str_value);
}

void COptImportExportBase::WriteLog(CString str_format, float f_value)
{
	if(m_fp_log)
	{
		fprintf(m_fp_log, "%d : ", m_iCurrentLineNo);
		fprintf(m_fp_log, (str_format), f_value);
	}
	//TRACE("%d : ", m_iCurrentLineNo);
	//TRACE(str_format, f_value);
}

void COptImportExportBase::ReadLinesUntilString(CString until_string)
{
	while( ReadOneLineFromFile() > 0 && m_array_strOutput[0] != until_string)
	{
	}
}