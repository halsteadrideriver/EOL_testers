#pragma once
#include "global_def.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sys/timeb.h>

extern LPCSTR  DEFAULT_DELIMITER_A;
extern LPCWSTR DEFAULT_DELIMITER_W;

//////////////////////////////////////////////////////////////////////////
class CUtility
{
public:
	CUtility(void);
	virtual ~CUtility(void);

	///
	LPCWSTR _A2CW(LPCSTR  lpszInput);
	LPCSTR  _W2CA(LPCWSTR lpszInput);

	/// Split string into token value.
	/// Token is skipped if started with ';' and '#'.
	/// @param lpszOrg      - [I] String containing token(s).
	/// @param lpsczDelimit - [I] Set of delimiter characters.
	/// @param nCount       - [O] Return the total count of token(s).
	/// @param nBase        - [I] Number base to use.(10 or 16)
	/// @return token(s)
	/// ----
	/// Example A:
	///     const char* lpString = "Hellow   World";
	///     INT  nCount = 0;
	///     CHAR** lppToken = GetTokenStringA(lpString, DEFAULT_DELIMITER_A, nCount);
	///     assert(2 == nCount);
	///     printf("%s\n%s\n", lppToken[0], lppToken[1]);
	///     Hello
	///     World
	/// Example B:
	///     const char* lpString = "0xFE06,0xFE0A, 0xFE50  ; Gain values";
	///     INT  nCount = 0;
	///     INT* pGainTable = GetTokenIntegerA(lpString, DEFAULT_DELIMITER_A, nCount, 16);
	///     assert(3 == nCount);
	///     assert(pGainTable[0] == 0xFE06 && pGainTable[1] == 0xFE0A && pGainTable[2] == 0xFE50);
	LPSTR* GetTokenStringA(LPCSTR  lpszOrg, LPCSTR  lpsczDelimit, INT& nCount);
	LPWSTR* GetTokenStringW(LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount);

	LPSTR* GetSegStringA(LPCSTR lpszOrg, LPCSTR lpsczDelimit, INT& nCount);
	LPWSTR* GetSegStringW(LPWSTR lpszOrg, LPWSTR lpsczDelimit, INT& nCount);

	double* GetTokenDoubleA(LPCSTR  lpszOrg, LPCSTR  lpsczDelimit, INT& nCount, double defValue = INVALID_POSITIVE_DOUBLE_VALUE);
	double* GetTokenDoubleW(LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount, double defValue = INVALID_POSITIVE_DOUBLE_VALUE);

	double* GetSimpleTokenDoubleA(LPCSTR  lpszOrg, LPCSTR  lpsczDelimit, INT& nCount);
	double* GetSimpleTokenDoubleW(LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount);

	INT* GetTokenIntegerA(LPCSTR  lpszOrg, LPCSTR  lpsczDelimit, INT& nCount, INT nBase = 10, INT defValue = INVALID_POSITIVE_INTEGER_VALUE);
	INT* GetTokenIntegerW(LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount, INT nBase = 10, INT defValue = INVALID_POSITIVE_INTEGER_VALUE);

	/*get simpleTokenXXX  -->  Input start$end$step;  e.g. 1$5$1,  output 1,2,3,4,5*/
	INT* GetSimpleTokenIntegerA(LPCSTR  lpszOrg, LPCSTR  lpsczDelimit, INT& nCount, INT nBase = 10);
	INT* GetSimpleTokenIntegerW(LPCWSTR lpszOrg, LPCWSTR lpsczDelimit, INT& nCount, INT nBase = 10);

	BOOL    CreateMultiDirectoryA(LPCSTR  lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	BOOL    CreateMultiDirectoryW(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	BOOL	ASCII2Bin(const BYTE* pBuffer, BYTE* pResultBuffer, unsigned long ulSize);
	BOOL	Bin2ASCII(const BYTE* pBuffer, BYTE* pResultBuffer, unsigned long ulSize);

	unsigned short Convert16(unsigned short src) const;
	unsigned int Convert32(unsigned int Src) const;

	/// Trims all leading and trailing whitespace from the string
	std::string& trimA(std::string& str);
	std::wstring& trimW(std::wstring& str);

	std::string  GetAbsoluteFilePathA(LPCSTR  lpszFilePath);
	std::wstring GetAbsoluteFilePathW(LPCWSTR lpszFilePath);

	BOOL IsExistFileA(LPCSTR lpszFileFullPath);
	/*
	cout << replace_all(string("12212"), "12", "21") << endl;               // "22211"
	cout << replace_all_distinct(string("12212"), "12", "21") << endl;      // "21221"
	*/
	std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value);
	std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
	std::wstring& replace_all_w(std::wstring& str, const std::wstring& old_value, const std::wstring& new_value);
	std::wstring& replace_all_distinct_w(std::wstring& str, const std::wstring& old_value, const std::wstring& new_value);

	double safe_strtod(const char* nptr, double defValue = INVALID_POSITIVE_DOUBLE_VALUE);
	double safe_wcstod(const wchar_t* nptr, double defValue = INVALID_POSITIVE_DOUBLE_VALUE);
	long   safe_strtol(const char* nptr, int base, long defValue = INVALID_POSITIVE_INTEGER_VALUE);
	long   safe_wcstol(const wchar_t* nptr, int base, long defValue = INVALID_POSITIVE_INTEGER_VALUE);

private:
	std::vector<CHAR>   m_vecStringA;
	std::vector<LPSTR>  m_vecTokenA;
	std::vector<WCHAR>  m_vecStringW;
	std::vector<LPWSTR> m_vecTokenW;
	std::vector<double> m_vecDouble;
	std::vector<INT>    m_vecInteger;
	LPSTR               m_lpOutputStrA;
	LPWSTR              m_lpOutputStrW;
};

#if defined (UNICODE) ||  defined (_UNICODE)
#define GetTokenString          GetTokenStringW
#define GetTokenDouble          GetTokenDoubleW
#define GetTokenInteger         GetTokenIntegerW
#define trim                    trimW
#define CreateMultiDirectory    CreateMultiDirectoryW
#define DEFAULT_DELIMITER       DEFAULT_DELIMITER_W
#define _T2CW(x)                (x)
#define _T2CA(x)                _W2CA(x)
#define _A2CT(x)                _A2CW(x)
#define _W2CT(x)                (x)
#define GetSimpleTokenDouble    GetSimpleTokenDoubleW
#define GetSimpleTokenInteger   GetSimpleTokenIntegerW
#define safe_tcstod             safe_strtod
#define safe_tcstol             safe_strtol
#else
#define GetTokenString          GetTokenStringA
#define GetTokenDouble          GetTokenDoubleA
#define GetTokenInteger         GetTokenIntegerA
#define trim                    trimA
#define CreateMultiDirectory    CreateMultiDirectoryA
#define DEFAULT_DELIMITER       DEFAULT_DELIMITER_A
#define _T2CW(x)                _A2CW(x)
#define _T2CA(x)                (x)
#define _A2CT(x)                (x)
#define _W2CT(x)                _W2CA(x)
#define GetSimpleTokenDouble    GetSimpleTokenDoubleA
#define GetSimpleTokenInteger   GetSimpleTokenIntegerA
#define safe_tcstod             safe_wcstod
#define safe_tcstol             safe_wcstol
#endif

class  CLinearEquation
{
public:
	CLinearEquation();
	CLinearEquation(double k, double b);

	CLinearEquation(double k, double x1, double y1);
	CLinearEquation(double x1, double y1, double x2, double y2);
	~CLinearEquation();

	double operator()(double x);
	double x2y(double x);
	double y2x(double y);
	double k;
	double b;
};
//

// 可替代GetTickCount, GetTickCount精度只有15ms，超过49.7天有复位问题
class CSPTimer sealed
{
public:
	CSPTimer(void)
	{
		Reset();
	}

	CSPTimer(const CSPTimer& rhs)
	{
		m_begin = rhs.GetTime();
	}

	CSPTimer& operator=(const CSPTimer& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		m_begin = rhs.GetTime();
		return *this;
	}

	~CSPTimer(void)
	{
	}

public:
	BOOL IsTimeOut(time_t millsec)
	{
		timeb  end;
		ftime(&end);
		time_t diff = (end.time - m_begin.time) * 1000 + (end.millitm - m_begin.millitm);
		return diff > millsec;
	}

	time_t GetTimeSpan(BOOL bMillseconds = TRUE)
	{
		timeb  end;
		ftime(&end);
		if (bMillseconds)
		{
			return (end.time - m_begin.time) * 1000 + (end.millitm - m_begin.millitm);
		}
		else
		{
			return end.time - m_begin.time;
		}
	}

	void Reset(void)
	{
		ftime(&m_begin);
	}

	const timeb& GetTime(void) const
	{
		return m_begin;
	}

private:
	timeb m_begin;
};
