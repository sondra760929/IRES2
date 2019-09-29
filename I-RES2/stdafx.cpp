// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// I-RES2.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

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
	strcpy_s(strUTF8, 1024, in.c_str());// �̰� ��� ��Ƽ����Ʈ���� UTF8�̶�� �������ּ��� -_-;;
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
	//strcpy_s(strUTF8, 1024, in.c_str());// �̰� ��� ��Ƽ����Ʈ���� UTF8�̶�� �������ּ��� -_-;;
	//if (strlen(strUTF8) > 0)
	//{
	//	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), NULL, NULL);
	//	MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), strUnicode, nLen);

	//	string strMulti = CW2A(strUnicode);
	//	return strMulti;
	//}
	//return "";
}
