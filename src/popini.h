#ifndef POPINI_H_FILE
#	define POPINI_H_FILE

#	include <stddef.h>

/* Define PopIniSig enum signal types. */
typedef enum PopIniSig_{
	PopIniSig_ParseError= 1,
	PopIniSig_ParseError_MissingBracket= 2,
	PopIniSig_ParseError_MissingEqual= 3,
	PopIniSig_MemoryError= 4,
	PopIniSig_CopyError= 5,
	PopIniSig_IniError= 6,
	PopIniSig_IniError_CommentDef= 7,
	PopIniSig_IniError_InvalidSpace= 8
} PopIniSig;

/* Define PopIniSig function type. */
typedef char (*PopIniSigFunc)(const PopIniSig signal,const long unsigned line,const char *lineStr);

/* Allows setting of a signal's function callback. */
void PopIniSig_OnSignal(PopIniSig signal,PopIniSigFunc func);

#endif