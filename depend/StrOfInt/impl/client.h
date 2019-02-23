/*
	StrOfInt is a header-only library.
	By default, it defines a static function called StrOfInt.
	By #define you may define
		the function name,
		the pointer to char type,
		the integer type,
		the signness of the integer type.
	As such, it may be included more than once.

	StrOfInt is low-level. It does not format the result string.
	It does not null-terminate the result string.
	It will not put any prefix except negative sign for radix 10.
	This design is intentional, to give maximum control to caller.
	e.g. To put string of integer in the middle of a string.
	
	On success:
		The function returns positive value (iFirst) where
		pszBuf[iFirst] will be the first digit char.
	
	On error:
		The function returns negative value such as StrOfInt_errBufBoom.
*/

#ifndef STROFINT_FUNCNAME
#define STROFINT_FUNCNAME  StrOfInt
#endif

#ifndef STROFINT_PSTRTYPE
#define STROFINT_PSTRTYPE  char *
#endif

#ifndef STROFINT_INTTYPE
#define STROFINT_INTTYPE  int
#endif

#ifndef STROFINT_SIGNED
#define STROFINT_SIGNED  (1)
#endif

#ifndef STROFINT_UPPERHEX
#define STROFINT_UPPERHEX  (1)
#endif

#undef  STROFINT__DIGITCHARS
#if (STROFINT_UPPERHEX)
#define STROFINT__DIGITCHARS  "0123456789ABCDEF"
#else
#define STROFINT__DIGITCHARS  "0123456789abcdef"
#endif

#ifndef STROFINT__ERR_ENUM
#define STROFINT__ERR_ENUM
enum {
	StrOfInt_errBufBoom = -1,
	StrOfInt_errBadArg = -2,
	StrOfInt_errBadRadix = -3
};
#endif/* STROFINT__ERR_ENUM */

static int
STROFINT_FUNCNAME(
	STROFINT_PSTRTYPE const pszBuf,
	/* if (< 0) then assume strlen<char_t>(pBuf) */ int nBuf,
	STROFINT_INTTYPE theInt,
	/* 2 (bin) / 8 (oct) / 10 (dec) / 16 (hex) */ int const radix)
{
#if (STROFINT_SIGNED)
	int const isNeg = (theInt < 0);
#endif
	int iStr = 0;
	if (!pszBuf) {
		return StrOfInt_errBadArg;
	}
	if (radix != 2 && radix != 8 && radix != 10 && radix != 16) {
		return StrOfInt_errBadRadix;
	}
	if (nBuf < 0) {
		for (; pszBuf[nBuf]; ++nBuf);
	}
	for (iStr = nBuf; iStr-- > 0;) {
		int iChr = (int)(theInt % radix);
		if (iChr < 0) { iChr *= -1; }
		pszBuf[iStr] = STROFINT__DIGITCHARS[iChr];
		theInt /= (STROFINT_INTTYPE)radix;
		if (theInt == 0) { break; }
	}
	if (iStr < 0) {
		return StrOfInt_errBufBoom;
	}
#if (STROFINT_SIGNED)
	if (isNeg) {
		if (--iStr < 0) {
			return StrOfInt_errBufBoom;
		}
		pszBuf[iStr] = '-';
	}
#endif
	return iStr;
}
