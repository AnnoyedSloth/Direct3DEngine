//#pragma once
//#ifndef _ZFLOG_H_
//#define _ZFLOG_H_
//
//#include <windows.h>
//#include <stdio.h>
//
//#define ZF_LOG_TARGET_CONSOLE			0x00000001
//#define ZF_LOG_TARGET_FILE				0x00000002
//#define ZF_LOG_TARGET_WINDOW			0x00000004
//#define ZF_LOG_TARGET_ALL				(ZF_LOG_TARGET_CONSOLE|ZF_LOG_TARGET_FILE|ZF_LOG_TARGET_WINDOW)
//#define ZF_LOG_WINDOW_TITLE				"ZEngine F-Killer"
//#define ZF_LOG_WINDOW_CX				300
//#define ZF_LOG_WINDOW_CY				600
//
//class LogMgr
//{
//	unsigned int	m_nTarget;
//	char			m_szFilename[MAX_PATH];
//	HWND			m_hwnd;
//	HWND			m_hwndList;
//public:
//	LogMgr(UINT32 nTarget, LPSTR szFilename = NULL);
//	~LogMgr();
//	void	CreateLogWindow();
//	int		Log(LPSTR fmt, ...);
//
//	void LogStatus(void);
//	void LogFPS(void);
//
//private:
//	static	LRESULT CALLBACK
//		LogMgr::WndProcZFLog(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//};
//
//extern LogMgr*		g_pLog;
//
//#endif // _ZFLOG_H_