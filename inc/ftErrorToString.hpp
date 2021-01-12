#ifndef FT_ERROR_TO_STRING_HPP
#define FT_ERROR_TO_STRING_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

inline const char *ftstrerror(FT_Error error)
{
#undef FTERRORS_H_
#define FT_ERRORDEF(error_code, value, string) case error_code: return string;
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST default: return "Unknown error"; }
#include FT_ERRORS_H
}

#endif //FT_ERROR_TO_STRING_HPP
