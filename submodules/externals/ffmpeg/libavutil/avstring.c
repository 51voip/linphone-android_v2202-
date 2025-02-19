/*
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 * Copyright (c) 2007 Mans Rullgard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "avstring.h"
#include "mem.h"

int av_strstart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && *pfx == *str) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr)
        *ptr = str;
    return !*pfx;
}

int av_stristart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && toupper((unsigned)*pfx) == toupper((unsigned)*str)) {
        pfx++;
        str++;
    }
    if (!*pfx && ptr)
        *ptr = str;
    return !*pfx;
}

char *av_stristr(const char *s1, const char *s2)
{
    if (!*s2)
        return s1;

    do {
        if (av_stristart(s1, s2, NULL))
            return s1;
    } while (*s1++);

    return NULL;
}

size_t av_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = 0;
    while (++len < size && *src)
        *dst++ = *src++;
    if (len <= size)
        *dst = 0;
    return len + strlen(src) - 1;
}

size_t av_strlcat(char *dst, const char *src, size_t size)
{
    size_t len = strlen(dst);
    if (size <= len + 1)
        return len + strlen(src);
    return len + av_strlcpy(dst + len, src, size - len);
}

size_t av_strlcatf(char *dst, size_t size, const char *fmt, ...)
{
    int len = strlen(dst);
    va_list vl;

    va_start(vl, fmt);
    len += vsnprintf(dst + len, size > len ? size - len : 0, fmt, vl);
    va_end(vl);

    return len;
}

char *av_d2str(double d)
{
    char *str= av_malloc(16);
    if(str) snprintf(str, 16, "%f", d);
    return str;
}

#define WHITESPACES " \n\t"

char *av_get_token(const char **buf, const char *term)
{
    char *out = av_malloc(strlen(*buf) + 1);
    char *ret= out, *end= out;
    const char *p = *buf;
    if (!out) return NULL;
    p += strspn(p, WHITESPACES);

    while(*p && !strspn(p, term)) {
        char c = *p++;
        if(c == '\\' && *p){
            *out++ = *p++;
            end= out;
        }else if(c == '\''){
            while(*p && *p != '\'')
                *out++ = *p++;
            if(*p){
                p++;
                end= out;
            }
        }else{
            *out++ = c;
        }
    }

    do{
        *out-- = 0;
    }while(out >= end && strspn(out, WHITESPACES));

    *buf = p;

    return ret;
}

#ifdef TEST

#undef printf

int main(void)
{
    int i;

    printf("Testing av_get_token()\n");
    {
        const char *strings[] = {
            "''",
            "",
            ":",
            "\\",
            "'",
            "    ''    :",
            "    ''  ''  :",
            "foo   '' :",
            "'foo'",
            "foo     ",
            "  '  foo  '  ",
            "foo\\",
            "foo':  blah:blah",
            "foo\\:  blah:blah",
            "foo\'",
            "'foo :  '  :blahblah",
            "\\ :blah",
            "     foo",
            "      foo       ",
            "      foo     \\ ",
            "foo ':blah",
            " foo   bar    :   blahblah",
            "\\f\\o\\o",
            "'foo : \\ \\  '   : blahblah",
            "'\\fo\\o:': blahblah",
            "\\'fo\\o\\:':  foo  '  :blahblah"
        };

        for (i=0; i < FF_ARRAY_ELEMS(strings); i++) {
            const char *p= strings[i];
            printf("|%s|", p);
            printf(" -> |%s|", av_get_token(&p, ":"));
            printf(" + |%s|\n", p);
        }
    }

    return 0;
}

#endif /* TEST */
