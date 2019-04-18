//#include "LogManager.h"
//#include <time.h>
//
//
//LogMgr::LogMgr(UINT32 nTarget, LPSTR szFilename)
//{
//	m_nTarget = nTarget;
//
//	if (nTarget & ZF_LOG_TARGET_FILE)
//		lstrcpy(m_szFilename, szFilename);
//	else
//		m_szFilename[0] = NULL;
//
//	if (nTarget & ZF_LOG_TARGET_WINDOW)
//		CreateLogWindow();
//	else
//		m_hwnd = NULL;
//
//	Log("F-Killer Logging Started...");
//}
//
//LogMgr::~LogMgr()
//{
//	DestroyWindow(m_hwnd);
//}
//
//void LogMgr::CreateLogWindow()
//{
//	int			x, y, cx, cy;
//	WNDCLASS	wc;
//	RECT		rc;
//
//	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//	wc.lpfnWndProc = (WNDPROC)LogMgr::WndProcZFLog;
//	wc.cbClsExtra = 0;									// No Extra Window Data
//	wc.cbWndExtra = 0;									// No Extra Window Data
//	wc.hInstance = GetModuleHandle(NULL);
//	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
//	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = "ZLogWindow";
//
//	RegisterClass(&wc);
//
//	cx = ZF_LOG_WINDOW_CX;
//	cy = ZF_LOG_WINDOW_CY;
//	x = GetSystemMetrics(SM_CXFULLSCREEN) - cx;
//	y = 0;
//
//	LPCWSTR wndName(L"LogWindow");
//
//	m_hwnd = CreateWindow("LogWindow", ZF_LOG_WINDOW_TITLE, WS_POPUP | WS_CAPTION, x, y, cx, cy, NULL, NULL, GetModuleHandle(NULL), NULL);
//	GetClientRect(m_hwnd, &rc);
//	m_hwndList = CreateWindow("LogWindow", "", WS_CHILD | WS_VSCROLL, 0, 0, rc.right, rc.bottom, m_hwnd, NULL, GetModuleHandle(NULL), NULL);
//	ShowWindow(m_hwnd, SW_SHOW);
//	ShowWindow(m_hwndList, SW_SHOW);
//}
//
//int LogMgr::Log(LPSTR fmt, ...)
//{
//	char	buff[1024];
//	char	date[128];
//	char	time[128];
//
//	_strdate(date);
//	_strtime(time);
//	vsprintf(buff, fmt, (char *)(&fmt + 1));
//
//	// Console에 출력할 경우 
//	if (m_nTarget & ZF_LOG_TARGET_CONSOLE)
//	{
//		printf("(date[%s] time[%s]) : %s\n", date, time, buff);
//	}
//
//	// Log File에 출력할 경우
//	if (m_nTarget & ZF_LOG_TARGET_FILE)
//	{
//		FILE*	fp = NULL;
//		fp = fopen(m_szFilename, "a+");
//		if (fp)
//		{
//			fprintf(fp, "(date[%s] time[%s]) : %s\n", date, time, buff);
//			fclose(fp);
//		}
//	}
//
//	// Log Window에 출력할 경우 
//	if (m_nTarget & ZF_LOG_TARGET_WINDOW)
//	{
//		SendMessage(m_hwndList, LB_ADDSTRING, 0, (LPARAM)buff);
//		UINT32 n = SendMessage(m_hwndList, LB_GETCOUNT, 0, 0L) - 1;
//		SendMessage(m_hwndList, LB_SETCURSEL, (WPARAM)n, 0L);
//	}
//	return 1;
//}
//
//LRESULT CALLBACK
//LogMgr::WndProcZFLog(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
//
///**-----------------------------------------------------------------------------
// * Status정보 출력
// *------------------------------------------------------------------------------
// */
//void LogMgr::LogStatus(void)
//{
//	g_pLog->Log("Wireframe:%d", 1);
//	g_pLog->Log("HideFrustum:%d", 2);
//	g_pLog->Log("LockFrustum:%d", 3);
//}
//
//
///**-----------------------------------------------------------------------------
// * FPS(Frame Per Second)출력
// *------------------------------------------------------------------------------
// */
//void LogMgr::LogFPS(void)
//{
//	static DWORD	nTick = 0;
//	static DWORD	nFPS = 0;
//
//	/// 1초가 지났는가?
//	if (GetTickCount() - nTick > 1000)
//	{
//		nTick = GetTickCount();
//		/// FPS값 출력
//		g_pLog->Log("FPS:%d", nFPS);
//
//		nFPS = 0;
//		LogStatus();	/// 상태정보를 여기서 출력(1초에 한번)
//		return;
//	}
//	nFPS++;
//}
