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
			m_iByteCount = (long)ffind.GetLength();
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
			CString get_string;
			m_strOneLine = "";
			while (fgets(m_str_line_from_file, 1024, m_fp_input) != NULL)
			{
				get_string = CString(m_str_line_from_file);
				get_string.Replace('\n', '\0');
				m_strOneLine += get_string;
				if (get_string.GetLength() < 1023)
				{
					break;
				}
				//m_strOneLine.Format(_T("%ws"), m_str_line_from_file);
				//m_strOneLine.Replace('\n', '\0');

				//fprintf(stderr, "[%d] %s\n", m_iCurrentLineNo, m_str_line_from_file);
			}
			m_array_strOutput.clear();
			m_iSplitCount = SplitString(m_strOneLine, m_array_strSplit, m_array_strOutput);
			m_iCurrentLineNo++;
			WriteLog((LPCTSTR)m_str_line_from_file);
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

int COptImportExportBase::SplitString(CString& str_input, vector< char >& split_chrs_input, vector< CString >& str_output)
{
	str_output.clear();
	vector< int > split_index_array;
	int split_count = (int)split_chrs_input.size();
	int temp_split_index;

	for (int split_index = 0; split_index<split_count; ++split_index)
	{
		temp_split_index = str_input.Find(split_chrs_input[split_index]);
		while (temp_split_index > -1)
		{
			split_index_array.push_back(temp_split_index);
			temp_split_index = str_input.Find(split_chrs_input[split_index], temp_split_index + 1);
		}
	}

	sort(split_index_array.begin(), split_index_array.end());
	split_count = (int)split_index_array.size();
	temp_split_index = -1;
	CString temp_split_str;
	for (int split_index = 0; split_index<split_count; ++split_index)
	{
		if (temp_split_index < split_index_array[split_index])
		{
			temp_split_str = str_input.Mid(temp_split_index + 1, split_index_array[split_index] - temp_split_index - 1);
			temp_split_index = split_index_array[split_index];
			if (temp_split_str != "")
			{
				str_output.push_back(temp_split_str);
			}
		}
	}

	if (temp_split_index < str_input.GetLength())
	{
		temp_split_str = str_input.Mid(temp_split_index + 1, str_input.GetLength() - 1 - temp_split_index);
		if (temp_split_str != "")
		{
			str_output.push_back(temp_split_str);
		}
	}

	return (int)str_output.size();
}

int COptImportExportBase::SplitString(char* str_input, vector< char >& split_chrs_input, vector< CString >& str_output)
{
	str_output.clear();
	int char_index(0);
	char temp_string[512];
	int temp_char_index(0);
	bool b_splitted(false);
	int split_count = (int)split_chrs_input.size();
	while (str_input[char_index] != '\0')
	{
		b_splitted = false;
		for (int split_index = 0; split_index<split_count; ++split_index)
		{
			if (split_chrs_input[split_index] == str_input[char_index])
			{
				b_splitted = true;
				if (temp_char_index > 0)
				{
					temp_string[temp_char_index] = '\0';
					CString temp_str_output(temp_string);
					temp_str_output.Trim();
					str_output.push_back(temp_str_output);
				}
				temp_char_index = 0;
				char_index++;
				break;
			}
		}

		if (b_splitted == false)
		{
			temp_string[temp_char_index++] = str_input[char_index++];
		}
	}

	if (temp_char_index > 0)
	{
		temp_string[temp_char_index] = '\0';
		CString temp_str_output(temp_string);
		temp_str_output.Trim();
		if (temp_str_output != "")
		{
			str_output.push_back(temp_str_output);
		}
	}
	return (int)str_output.size();
}

int COptImportExportBase::SplitString(string& str_input, vector< char >& split_chrs_input, vector< string >& str_output)
{
	str_output.clear();
	vector< int > split_index_array;
	CString str_input_cstring;
	str_input_cstring.Format(TEXT("%s"), str_input.c_str());
	int split_count = (int)split_chrs_input.size();
	int temp_split_index;

	for (int split_index = 0; split_index<split_count; ++split_index)
	{
		temp_split_index = (int)str_input.find_first_of(split_chrs_input[split_index]);
		while (temp_split_index != string::npos)
		{
			split_index_array.push_back(temp_split_index);
			temp_split_index = (int)str_input.find_first_of(split_chrs_input[split_index], temp_split_index + 1);
		}
	}

	sort(split_index_array.begin(), split_index_array.end());
	split_count = (int)split_index_array.size();
	temp_split_index = 0;
	for (int split_index = 0; split_index<split_count; ++split_index)
	{
		if (temp_split_index < split_index_array[split_index])
		{
			CT2CA pszConvertedAnsiString(str_input_cstring.Mid(temp_split_index, split_index_array[split_index] - temp_split_index));
			string temp_split_str(pszConvertedAnsiString);
			temp_split_index = split_index_array[split_index];
			if (temp_split_str != "")
			{
				str_output.push_back(temp_split_str);
			}
		}
	}

	return (int)str_output.size();
}
