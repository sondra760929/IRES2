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
