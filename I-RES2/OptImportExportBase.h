#pragma once
#include "OptUtil.h"

class COptImportExportBase
{
public:
	COptImportExportBase(bool b_make_log = false);
	~COptImportExportBase(void);

	char m_str_line_from_file[1024];
	FILE* m_fp_input;
	FILE* m_fp_output;
	FILE* m_fp_log;
	CString m_strInputFilePath;
	CString m_strOutputFilePath;
	CString m_strLogFilePath;
	vector< CString > m_array_strOutput;
	vector< char > m_array_strSplit;
	int m_iCurrentLineNo;
	CString m_strErrorString;
	int m_iSplitCount;
	float ReadValue(CString str_format);
	bool OpenInputFile(CString input_file_path);
	bool OpenOutputFile(CString output_file_path);
	bool OpenLogFile(CString output_file_path);
	int ReadOneLineFromFile();
	void ReadUndefinedLinesFromFile(int line_count);
	void ReadDataFromFile(int& param1);
	void ReadDataFromFile(int& param1, int& param2);
	void ReadDataFromFile(int& param1, int& param2, int& param3);
	void ReadDataFromFile(int& param1, int& param2, int& param3, int& param4);
	void ReadDataFromFile(float& param1);
	void ReadDataFromFile(float& param1, float& param2);
	void ReadDataFromFile(float& param1, float& param2, float& param3);
	void ReadDataFromFile(float& param1, float& param2, float& param3, float& param4);
	void ReadDataFromFile(CString& param1);
	void WriteLog(CString str_format);
	void WriteLog(CString str_format, int i_value);
	void WriteLog(CString str_format, CString str_value);
	void WriteLog(CString str_format, float f_value);
	void ReadLinesUntilString(CString until_string);
	bool m_bMakeLog;
	// for progress
	float m_iFileCount;
	float m_iFileIndex;
	float m_iByteCount;
	float m_iByteIndex;
	CString m_strOneLine;
};
