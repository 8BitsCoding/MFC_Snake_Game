﻿
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_draw_image.Create(801, 601, 32);
	m_draw_dc.Attach(m_draw_image.GetDC());


	m_draw_dc.SetDCBrushColor(RGB(0, 0, 0));

	srand((unsigned int)time(NULL));
	int x, y;
	for (int i = 0; i < 300; i++) {
		x = rand() % 80;
		y = rand() % 60;
		if(m_table[y][x] == 0) m_table[y][x] = RGB(255, 0, 0);
		else i--;
	}

	//m_table[m_pos.y][m_pos.x] = RGB(0, 255, 0);
	m_count_map[m_pos.y][m_pos.x] = m_eat_count + 1;


	SetTimer(1, 200, NULL);

	DrawMap();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::DrawMap()
{
	m_draw_dc.SelectStockObject(DC_BRUSH);
	m_draw_dc.SelectStockObject(DC_PEN);

	m_draw_dc.SetDCPenColor(RGB(48, 48, 12));

	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++)
		{
			if (m_count_map[y][x] > 0) {
				m_count_map[y][x]--;
				if (m_count_map[y][x] == 0) m_table[y][x] = 0;
			}
			m_draw_dc.SetDCBrushColor(m_table[y][x]);
			m_draw_dc.Rectangle(x * 10, y * 10, x * 10 + 11, y * 10 + 11);
		}
	}

}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.


	if (IsIconic())
	{
		

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_draw_image.Draw(dc, 0, 0);
		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
	KillTimer(1);
}

void CMFCApplication1Dlg::GameOver() {
	KillTimer(1);
	MessageBox(L"미션을 완료하지 못했습니다.", L"Game Over!!", MB_ICONSTOP);
}

void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		//m_table[m_pos.y][m_pos.x] = 0;

		if (m_direction == 0) m_pos.x--;
		else if (m_direction == 1) m_pos.y--;
		else if (m_direction == 2) m_pos.x++;
		else m_pos.y++;

		if (m_pos.x >= 0 && m_pos.x <= 79 && m_pos.y >= 0 && m_pos.y <= 59) {
			if (m_table[m_pos.y][m_pos.x] == RGB(255, 0, 0)) {
				m_eat_count++;
				SetDlgItemInt(IDC_EAT_COUNT_EDIT, m_eat_count);
			}
			else if (m_table[m_pos.y][m_pos.x]) {
				GameOver();
				return;
			}
			m_table[m_pos.y][m_pos.x] = RGB(0, 255, 0);
			m_count_map[m_pos.y][m_pos.x] = m_eat_count + 2;
			DrawMap();
			CClientDC dc(this);
			m_draw_image.Draw(dc, 0, 0);
		}
		else {
			// 게임 오버
			GameOver();
		}


	}
	else {
		CDialogEx::OnTimer(nIDEvent);
	}
}


BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam >= VK_LEFT && pMsg->wParam <= VK_DOWN) {
			int old_direction = m_direction;
			m_direction = pMsg->wParam - VK_LEFT;
			if (old_direction > m_direction) {
				if ((old_direction - m_direction) == 2) m_direction = old_direction;
			}
			else {
				if ((m_direction - old_direction) == 2) m_direction = old_direction;
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
