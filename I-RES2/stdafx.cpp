// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// I-RES2.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


CString m_strAppPath;
CString m_strEnvPath;
CString m_strProjectPath;
bool m_bUseOutline = false;
bool m_bUseVBO = false;
bool m_bUseShadow = false;
COLORREF m_colorUp = RGB(0, 0, 0);
COLORREF m_colorDown = RGB(200, 200, 200);
//COLORREF m_colorUp = RGB(171, 255, 255);
//COLORREF m_colorDown = RGB(5, 9, 100);
float m_fFaceDeflection = 10.0f;
float m_fEdgeDeflection = 10.0f;
vector< CString > m_aAnalysisInput;
vector< CString > m_aAnalysisOutput;
vector< CString > m_aAnalysisCommand;
vector< CString > m_aAnalysisPGM;
CIRES2View* m_pCurrentView = NULL;
float UNIT_TO_M = 0.001f;
float M_TO_UNIT = 1000.0f;
UNIT_MODE m_iCurrentUnitMode = UNIT_MM;

void SetButtons(CMFCButton* btn, UINT img, CString tooltip)
{
	CImage btn_image1;
	btn_image1.LoadFromResource(AfxGetInstanceHandle(), img);
	HBITMAP hBit1;
	hBit1 = btn_image1.Detach();

	btn->SetImage(hBit1, TRUE, hBit1);

	btn->SetTooltip(tooltip);
	btn->EnableFullTextTooltip(FALSE);
}

void char_to_utf8(char* strMultibyte, char* out)
{
	memset(out, 0, 512);
	wchar_t strUni[512] = { 0, };
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), strUni, nLen);

	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), out, nLen, NULL, NULL);
}

void lpctstr_to_utf8(LPCTSTR in, char* out)
{
	char    strMultibyte[512] = { 0, };
	strcpy_s(strMultibyte, 512, CT2A(in));
	char_to_utf8(strMultibyte, out);
}

wstring utf_to_unicode(string in)
{
	wstring return_string;
	wchar_t strUnicode[1024] = { 0, };
	char    strUTF8[1024] = { 0, };
	strcpy_s(strUTF8, 1024, in.c_str());// 이건 사실 멀티바이트지만 UTF8이라고 생각해주세요 -_-;;
	if (strlen(strUTF8) > 0)
	{
		int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, (int)strlen(strUTF8), NULL, NULL);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8, (int)strlen(strUTF8), strUnicode, nLen);

		wstring strMulti(strUnicode);
		return strMulti;
	}
	return return_string;
}

string utf_to_multibyte(string in)
{
	string strMulti;
	wstring strUni = utf_to_unicode(in);
	strMulti.assign(strUni.begin(), strUni.end());
	return strMulti;
	//string strMulti(CW2A(utf_to_unicode(in)));
	//wchar_t strUnicode[1024] = { 0, };
	//char    strUTF8[1024] = { 0, };
	//strcpy_s(strUTF8, 1024, in.c_str());// 이건 사실 멀티바이트지만 UTF8이라고 생각해주세요 -_-;;
	//if (strlen(strUTF8) > 0)
	//{
	//	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), NULL, NULL);
	//	MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), strUnicode, nLen);

	//	string strMulti = CW2A(strUnicode);
	//	return strMulti;
	//}
	//return "";
}
