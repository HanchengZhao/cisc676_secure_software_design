/* Generated by re2c 0.9.1-C on Fri Sep 23 17:11:00 2016
 */
#line 1 "./modules/parsers/gas/gas-token.re"
/*
 * GAS-compatible re2c lexer
 *
 *  Copyright (C) 2005-2007  Peter Johnson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of other contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND OTHER CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR OTHER CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <util.h>

#include <libyasm.h>

#include "modules/parsers/gas/gas-parser.h"


#define BSIZE   8192

#define YYCURSOR        cursor
#define YYLIMIT         (s->lim)
#define YYMARKER        (s->ptr)
#define YYFILL(n)       {cursor = fill(parser_gas, cursor);}

#define RETURN(i)       do {s->cur = cursor; parser_gas->tokch = s->tok[0]; \
                         return i;} while (0)

#define SCANINIT()      {s->tok = cursor;}

#define TOK             ((char *)s->tok)
#define TOKLEN          (size_t)(cursor-s->tok)

/* Bridge function to convert byte-oriented parser with line-oriented
 * preprocessor.
 */
static size_t
preproc_input(yasm_parser_gas *parser_gas, /*@out@*/ YYCTYPE *buf,
              size_t max_size)
{
    size_t tot=0;
    while (max_size > 0) {
        size_t n;

        if (!parser_gas->line) {
            parser_gas->line = yasm_preproc_get_line(parser_gas->preproc);
            if (!parser_gas->line)
                return tot; /* EOF */
            parser_gas->linepos = parser_gas->line;
            parser_gas->lineleft = strlen(parser_gas->line) + 1;
            parser_gas->line[parser_gas->lineleft-1] = '\n';
        }

        n = parser_gas->lineleft<max_size ? parser_gas->lineleft : max_size;
        strncpy((char *)buf+tot, parser_gas->linepos, n);

        if (n == parser_gas->lineleft) {
            yasm_xfree(parser_gas->line);
            parser_gas->line = NULL;
        } else {
            parser_gas->lineleft -= n;
            parser_gas->linepos += n;
        }

        tot += n;
        max_size -= n;
    }
    return tot;
}
#if 0
static size_t
fill_input(void *d, unsigned char *buf, size_t max)
{
    return yasm_preproc_input((yasm_preproc *)d, (char *)buf, max);
}
#endif
static YYCTYPE *
fill(yasm_parser_gas *parser_gas, YYCTYPE *cursor)
{
    yasm_scanner *s = &parser_gas->s;
    int first = 0;
    if(!s->eof){
        size_t cnt = s->tok - s->bot;
        if(cnt){
            memmove(s->bot, s->tok, (size_t)(s->lim - s->tok));
            s->tok = s->bot;
            s->ptr -= cnt;
            cursor -= cnt;
            s->lim -= cnt;
        }
        if (!s->bot)
            first = 1;
        if((s->top - s->lim) < BSIZE){
            YYCTYPE *buf = yasm_xmalloc((size_t)(s->lim - s->bot) + BSIZE);
            memcpy(buf, s->tok, (size_t)(s->lim - s->tok));
            s->tok = buf;
            s->ptr = &buf[s->ptr - s->bot];
            cursor = &buf[cursor - s->bot];
            s->lim = &buf[s->lim - s->bot];
            s->top = &s->lim[BSIZE];
            if (s->bot)
                yasm_xfree(s->bot);
            s->bot = buf;
        }
        if((cnt = preproc_input(parser_gas, s->lim, BSIZE)) == 0) {
            s->eof = &s->lim[cnt]; *s->eof++ = '\n';
        }
        s->lim += cnt;
        if (first && parser_gas->save_input) {
            int i;
            YYCTYPE *saveline;
            parser_gas->save_last ^= 1;
            saveline = parser_gas->save_line[parser_gas->save_last];
            /* save next line into cur_line */
            for (i=0; i<79 && &s->tok[i] < s->lim && s->tok[i] != '\n'; i++)
                saveline[i] = s->tok[i];
            saveline[i] = '\0';
        }
    }
    return cursor;
}

static YYCTYPE *
save_line(yasm_parser_gas *parser_gas, YYCTYPE *cursor)
{
    yasm_scanner *s = &parser_gas->s;
    int i = 0;
    YYCTYPE *saveline;

    parser_gas->save_last ^= 1;
    saveline = parser_gas->save_line[parser_gas->save_last];

    /* save next line into cur_line */
    if ((YYLIMIT - YYCURSOR) < 80)
        YYFILL(80);
    for (i=0; i<79 && &cursor[i] < s->lim && cursor[i] != '\n'; i++)
        saveline[i] = cursor[i];
    saveline[i] = '\0';
    return cursor;
}

/* starting size of string buffer */
#define STRBUF_ALLOC_SIZE       128

/* string buffer used when parsing strings/character constants */
static YYCTYPE *strbuf = NULL;

/* length of strbuf (including terminating NULL character) */
static size_t strbuf_size = 0;

static void
strbuf_append(size_t count, YYCTYPE *cursor, yasm_scanner *s, int ch)
{
    if (count >= strbuf_size) {
        strbuf = yasm_xrealloc(strbuf, strbuf_size + STRBUF_ALLOC_SIZE);
        strbuf_size += STRBUF_ALLOC_SIZE;
    }
    strbuf[count] = ch;
}

#line 188 "./modules/parsers/gas/gas-token.re"



int
gas_parser_lex(YYSTYPE *lvalp, yasm_parser_gas *parser_gas)
{
    yasm_scanner *s = &parser_gas->s;
    YYCTYPE *cursor = s->cur;
    size_t count;
    YYCTYPE savech;

    /* Handle one token of lookahead */
    if (parser_gas->peek_token != NONE) {
        int tok = parser_gas->peek_token;
        *lvalp = parser_gas->peek_tokval;  /* structure copy */
        parser_gas->tokch = parser_gas->peek_tokch;
        parser_gas->peek_token = NONE;
        return tok;
    }

    /* Catch EOF */
    if (s->eof && cursor == s->eof)
        return 0;

    /* Jump to proper "exclusive" states */
    switch (parser_gas->state) {
        case COMMENT:
            goto comment;
        case SECTION_DIRECTIVE:
            goto section_directive;
        case NASM_FILENAME:
            goto nasm_filename;
        default:
            break;
    }

scan:
    SCANINIT();

    {
	static unsigned char yybm[] = {
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   1,   0,   0,   0,   1,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  1,   0,   0,   0,  12,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,  12,   0, 
	254, 254, 126, 126, 126, 126, 126, 126, 
	 94,  94,   0,   0,   0,   0,   0,   0, 
	  8,  78,  78,  78,  78,  78,  78,  14, 
	 14,  14,  14,  14,  14,  14,  14,  14, 
	 14,  14,  14,  14,  14,  14,  14,  14, 
	 14,  14,  14,   0,   0,   0,   0,  12, 
	  0,  78,  78,  78,  78,  78,  78,  14, 
	 14,  14,  14,  14,  14,  14,  14,  14, 
	 14,  14,  14,  14,  14,  14,  14,  14, 
	 14,  14,  14,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	};

#line 258 "gas-token.c"
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy0;
	++YYCURSOR;
yy0:
	if((YYLIMIT - YYCURSOR) < 5) YYFILL(5);
	yych = *YYCURSOR;
	if(yych <= '/'){
		if(yych <= '#'){
			if(yych <= '\r'){
				if(yych <= '\t'){
					if(yych <= '\b')	goto yy30;
					goto yy26;
				} else {
					if(yych <= '\n')	goto yy28;
					if(yych <= '\f')	goto yy30;
					goto yy26;
				}
			} else {
				if(yych <= ' '){
					if(yych <= '\037')	goto yy30;
					goto yy26;
				} else {
					if(yych <= '!')	goto yy17;
					if(yych <= '"')	goto yy11;
					goto yy24;
				}
			}
		} else {
			if(yych <= '*'){
				if(yych <= '%'){
					if(yych <= '$')	goto yy17;
					goto yy22;
				} else {
					if(yych == '\'')	goto yy9;
					goto yy17;
				}
			} else {
				if(yych <= ','){
					if(yych <= '+')	goto yy5;
					goto yy17;
				} else {
					if(yych <= '-')	goto yy5;
					if(yych <= '.')	goto yy7;
					goto yy18;
				}
			}
		}
	} else {
		if(yych <= 'Z'){
			if(yych <= '<'){
				if(yych <= '9'){
					if(yych >= '1')	goto yy4;
					goto yy2;
				} else {
					if(yych <= ':')	goto yy17;
					if(yych <= ';')	goto yy19;
					goto yy13;
				}
			} else {
				if(yych <= '>'){
					if(yych <= '=')	goto yy17;
					goto yy15;
				} else {
					if(yych <= '?')	goto yy30;
					if(yych <= '@')	goto yy17;
					goto yy21;
				}
			}
		} else {
			if(yych <= 'z'){
				if(yych <= '^'){
					if(yych <= ']')	goto yy30;
					goto yy17;
				} else {
					if(yych == '`')	goto yy30;
					goto yy21;
				}
			} else {
				if(yych <= '|'){
					if(yych <= '{')	goto yy30;
					goto yy17;
				} else {
					if(yych == '~')	goto yy17;
					goto yy30;
				}
			}
		}
	}
yy2:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych <= 'T'){
		if(yych <= 'A'){
			if(yych <= '/'){
				if(yych == '.')	goto yy90;
				goto yy3;
			} else {
				if(yych <= '9')	goto yy90;
				if(yych <= ':')	goto yy81;
				goto yy3;
			}
		} else {
			if(yych <= 'E'){
				if(yych <= 'B')	goto yy85;
				if(yych >= 'D')	goto yy88;
				goto yy3;
			} else {
				if(yych <= 'F')	goto yy86;
				if(yych >= 'T')	goto yy88;
				goto yy3;
			}
		}
	} else {
		if(yych <= 'e'){
			if(yych <= 'a'){
				if(yych == 'X')	goto yy92;
				goto yy3;
			} else {
				if(yych <= 'b')	goto yy85;
				if(yych >= 'd')	goto yy88;
				goto yy3;
			}
		} else {
			if(yych <= 't'){
				if(yych <= 'f')	goto yy86;
				if(yych >= 't')	goto yy88;
				goto yy3;
			} else {
				if(yych == 'x')	goto yy92;
				goto yy3;
			}
		}
	}
yy3:
#line 229 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            lvalp->intn = yasm_intnum_create_dec(TOK);
            s->tok[TOKLEN] = savech;
            RETURN(INTNUM);
        }
#line 402 "gas-token.c"
yy4:	yych = *++YYCURSOR;
	if(yych <= 'E'){
		if(yych <= ':'){
			if(yych <= '9')	goto yy84;
			goto yy81;
		} else {
			if(yych == 'B')	goto yy77;
			goto yy84;
		}
	} else {
		if(yych <= 'b'){
			if(yych <= 'F')	goto yy79;
			if(yych <= 'a')	goto yy84;
			goto yy77;
		} else {
			if(yych == 'f')	goto yy79;
			goto yy84;
		}
	}
yy5:	yyaccept = 1;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych == '.')	goto yy61;
	if(yych <= '/')	goto yy6;
	if(yych <= '9')	goto yy59;
	goto yy6;
yy6:
#line 303 "./modules/parsers/gas/gas-token.re"
{ RETURN(s->tok[0]); }
#line 431 "gas-token.c"
yy7:	yych = *++YYCURSOR;
	if(yybm[0+yych] & 16) {
		goto yy49;
	}
	goto yy37;
yy8:
#line 310 "./modules/parsers/gas/gas-token.re"
{
            lvalp->str.contents = yasm__xstrndup(TOK, TOKLEN);
            lvalp->str.len = TOKLEN;
            RETURN(ID);
        }
#line 444 "gas-token.c"
yy9:	yych = *++YYCURSOR;
	goto yy10;
yy10:
#line 289 "./modules/parsers/gas/gas-token.re"
{
            goto charconst;
        }
#line 452 "gas-token.c"
yy11:	yych = *++YYCURSOR;
	goto yy12;
yy12:
#line 294 "./modules/parsers/gas/gas-token.re"
{
            goto stringconst;
        }
#line 460 "gas-token.c"
yy13:	yych = *++YYCURSOR;
	if(yych == '<')	goto yy47;
	goto yy14;
yy14:
#line 301 "./modules/parsers/gas/gas-token.re"
{ RETURN(LEFT_OP); }
#line 467 "gas-token.c"
yy15:	yych = *++YYCURSOR;
	if(yych == '>')	goto yy45;
	goto yy16;
yy16:
#line 302 "./modules/parsers/gas/gas-token.re"
{ RETURN(RIGHT_OP); }
#line 474 "gas-token.c"
yy17:	yych = *++YYCURSOR;
	goto yy6;
yy18:	yych = *++YYCURSOR;
	if(yych == '*')	goto yy43;
	if(yych == '/')	goto yy41;
	goto yy6;
yy19:	yych = *++YYCURSOR;
	goto yy20;
yy20:
#line 304 "./modules/parsers/gas/gas-token.re"
{
            parser_gas->state = INITIAL;
            RETURN(s->tok[0]);
        }
#line 489 "gas-token.c"
yy21:	yych = *++YYCURSOR;
	goto yy37;
yy22:	yych = *++YYCURSOR;
	if(yybm[0+yych] & 2) {
		goto yy33;
	}
	goto yy23;
yy23:
#line 407 "./modules/parsers/gas/gas-token.re"
{
            yasm_warn_set(YASM_WARN_UNREC_CHAR,
                          N_("ignoring unrecognized character `%s'"),
                          yasm__conv_unprint(s->tok[0]));
            goto scan;
        }
#line 505 "gas-token.c"
yy24:	yych = *++YYCURSOR;
	goto yy25;
yy25:
#line 389 "./modules/parsers/gas/gas-token.re"
{
            if (parser_gas->is_cpp_preproc)
            {
                RETURN(CPP_LINE_MARKER);
            } else
                goto line_comment;
        }
#line 517 "gas-token.c"
yy26:	yych = *++YYCURSOR;
	goto yy32;
yy27:
#line 398 "./modules/parsers/gas/gas-token.re"
{ goto scan; }
#line 523 "gas-token.c"
yy28:	yych = *++YYCURSOR;
	goto yy29;
yy29:
#line 400 "./modules/parsers/gas/gas-token.re"
{
            if (parser_gas->save_input)
                cursor = save_line(parser_gas, cursor);
            parser_gas->state = INITIAL;
            RETURN(s->tok[0]);
        }
#line 534 "gas-token.c"
yy30:	yych = *++YYCURSOR;
	goto yy23;
yy31:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy32;
yy32:	if(yybm[0+yych] & 1) {
		goto yy31;
	}
	goto yy27;
yy33:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy34;
yy34:	if(yybm[0+yych] & 2) {
		goto yy33;
	}
	goto yy35;
yy35:
#line 327 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            if (parser_gas->is_nasm_preproc && strcmp(TOK+1, "line") == 0) {
                s->tok[TOKLEN] = savech;
                RETURN(NASM_LINE_MARKER);
            }

            switch (yasm_arch_parse_check_regtmod
                    (p_object->arch, TOK+1, TOKLEN-1, &lvalp->arch_data)) {
                case YASM_ARCH_REG:
                    s->tok[TOKLEN] = savech;
                    RETURN(REG);
                case YASM_ARCH_REGGROUP:
                    s->tok[TOKLEN] = savech;
                    RETURN(REGGROUP);
                case YASM_ARCH_SEGREG:
                    s->tok[TOKLEN] = savech;
                    RETURN(SEGREG);
                default:
                    break;
            }
            yasm_error_set(YASM_ERROR_GENERAL,
                           N_("Unrecognized register name `%s'"), s->tok);
            s->tok[TOKLEN] = savech;
            lvalp->arch_data = 0;
            RETURN(REG);
        }
#line 583 "gas-token.c"
yy36:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy37;
yy37:	if(yybm[0+yych] & 4) {
		goto yy36;
	}
	if(yych != '@')	goto yy8;
	goto yy38;
yy38:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy39;
yy39:	if(yybm[0+yych] & 8) {
		goto yy38;
	}
	goto yy40;
yy40:
#line 317 "./modules/parsers/gas/gas-token.re"
{
            /* if @ not part of ID, move the scanner cursor to the first @ */
            if (!((yasm_objfmt_base *)p_object->objfmt)->module->id_at_ok)
                cursor = (unsigned char *)strchr(TOK, '@');
            lvalp->str.contents = yasm__xstrndup(TOK, TOKLEN);
            lvalp->str.len = TOKLEN;
            RETURN(ID);
        }
#line 611 "gas-token.c"
yy41:	yych = *++YYCURSOR;
	goto yy42;
yy42:
#line 396 "./modules/parsers/gas/gas-token.re"
{ goto line_comment; }
#line 617 "gas-token.c"
yy43:	yych = *++YYCURSOR;
	goto yy44;
yy44:
#line 388 "./modules/parsers/gas/gas-token.re"
{ parser_gas->state = COMMENT; goto comment; }
#line 623 "gas-token.c"
yy45:	yych = *++YYCURSOR;
	goto yy46;
yy46:
#line 300 "./modules/parsers/gas/gas-token.re"
{ RETURN(RIGHT_OP); }
#line 629 "gas-token.c"
yy47:	yych = *++YYCURSOR;
	goto yy48;
yy48:
#line 299 "./modules/parsers/gas/gas-token.re"
{ RETURN(LEFT_OP); }
#line 635 "gas-token.c"
yy49:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	goto yy50;
yy50:	if(yybm[0+yych] & 16) {
		goto yy49;
	}
	if(yych <= 'E'){
		if(yych <= '.'){
			if(yych == '$')	goto yy36;
			if(yych >= '.')	goto yy36;
			goto yy51;
		} else {
			if(yych <= '?')	goto yy51;
			if(yych <= '@')	goto yy38;
			if(yych <= 'D')	goto yy36;
			goto yy52;
		}
	} else {
		if(yych <= '`'){
			if(yych <= 'Z')	goto yy36;
			if(yych == '_')	goto yy36;
			goto yy51;
		} else {
			if(yych == 'e')	goto yy52;
			if(yych <= 'z')	goto yy36;
			goto yy51;
		}
	}
yy51:
#line 266 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            lvalp->flt = yasm_floatnum_create(TOK);
            s->tok[TOKLEN] = savech;
            RETURN(FLTNUM);
        }
#line 674 "gas-token.c"
yy52:	yyaccept = 2;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych <= ','){
		if(yych == '+')	goto yy55;
		goto yy37;
	} else {
		if(yych <= '-')	goto yy55;
		if(yych <= '/')	goto yy37;
		if(yych >= ':')	goto yy37;
		goto yy53;
	}
yy53:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy54;
yy54:	if(yych <= '?'){
		if(yych <= '-'){
			if(yych == '$')	goto yy36;
			goto yy51;
		} else {
			if(yych <= '.')	goto yy36;
			if(yych <= '/')	goto yy51;
			if(yych <= '9')	goto yy53;
			goto yy51;
		}
	} else {
		if(yych <= '^'){
			if(yych <= '@')	goto yy38;
			if(yych <= 'Z')	goto yy36;
			goto yy51;
		} else {
			if(yych == '`')	goto yy51;
			if(yych <= 'z')	goto yy36;
			goto yy51;
		}
	}
yy55:	yych = *++YYCURSOR;
	if(yych <= '/')	goto yy56;
	if(yych <= '9')	goto yy57;
	goto yy56;
yy56:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy3;
	case 6:	goto yy91;
	case 3:	goto yy51;
	case 4:	goto yy66;
	case 5:	goto yy87;
	case 1:	goto yy6;
	case 2:	goto yy8;
	}
yy57:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy58;
yy58:	if(yych <= '/')	goto yy51;
	if(yych <= '9')	goto yy57;
	goto yy51;
yy59:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
	goto yy60;
yy60:	if(yych == '.')	goto yy65;
	if(yych <= '/')	goto yy56;
	if(yych <= '9')	goto yy59;
	goto yy56;
yy61:	yych = *++YYCURSOR;
	if(yych <= '/')	goto yy56;
	if(yych >= ':')	goto yy56;
	goto yy62;
yy62:	yyaccept = 3;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	goto yy63;
yy63:	if(yych <= 'D'){
		if(yych <= '/')	goto yy51;
		if(yych <= '9')	goto yy62;
		goto yy51;
	} else {
		if(yych <= 'E')	goto yy64;
		if(yych != 'e')	goto yy51;
		goto yy64;
	}
yy64:	yych = *++YYCURSOR;
	if(yych <= ','){
		if(yych == '+')	goto yy55;
		goto yy56;
	} else {
		if(yych <= '-')	goto yy55;
		if(yych <= '/')	goto yy56;
		if(yych <= '9')	goto yy57;
		goto yy56;
	}
yy65:	yyaccept = 4;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych <= 'D'){
		if(yych <= '/')	goto yy66;
		if(yych <= '9')	goto yy67;
		goto yy66;
	} else {
		if(yych <= 'E')	goto yy69;
		if(yych == 'e')	goto yy69;
		goto yy66;
	}
yy66:
#line 273 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            lvalp->flt = yasm_floatnum_create(TOK);
            s->tok[TOKLEN] = savech;
            RETURN(FLTNUM);
        }
#line 788 "gas-token.c"
yy67:	yyaccept = 3;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	goto yy68;
yy68:	if(yych <= 'D'){
		if(yych <= '/')	goto yy51;
		if(yych <= '9')	goto yy67;
		goto yy51;
	} else {
		if(yych <= 'E')	goto yy73;
		if(yych == 'e')	goto yy73;
		goto yy51;
	}
yy69:	yych = *++YYCURSOR;
	if(yych <= ','){
		if(yych != '+')	goto yy56;
		goto yy70;
	} else {
		if(yych <= '-')	goto yy70;
		if(yych <= '/')	goto yy56;
		if(yych <= '9')	goto yy71;
		goto yy56;
	}
yy70:	yych = *++YYCURSOR;
	if(yych <= '/')	goto yy56;
	if(yych >= ':')	goto yy56;
	goto yy71;
yy71:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy72;
yy72:	if(yych <= '/')	goto yy66;
	if(yych <= '9')	goto yy71;
	goto yy66;
yy73:	yych = *++YYCURSOR;
	if(yych <= ','){
		if(yych != '+')	goto yy56;
		goto yy74;
	} else {
		if(yych <= '-')	goto yy74;
		if(yych <= '/')	goto yy56;
		if(yych <= '9')	goto yy75;
		goto yy56;
	}
yy74:	yych = *++YYCURSOR;
	if(yych <= '/')	goto yy56;
	if(yych >= ':')	goto yy56;
	goto yy75;
yy75:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy76;
yy76:	if(yych <= '/')	goto yy51;
	if(yych <= '9')	goto yy75;
	goto yy51;
yy77:	yych = *++YYCURSOR;
	goto yy78;
yy78:
#line 379 "./modules/parsers/gas/gas-token.re"
{
            /* build local label name */
            lvalp->str.contents = yasm_xmalloc(30);
            lvalp->str.len =
                sprintf(lvalp->str.contents, "L%c\001%lu", s->tok[0],
                        parser_gas->local[s->tok[0]-'0']);
            RETURN(ID);
        }
#line 857 "gas-token.c"
yy79:	yych = *++YYCURSOR;
	goto yy80;
yy80:
#line 369 "./modules/parsers/gas/gas-token.re"
{
            /* build local label name */
            lvalp->str.contents = yasm_xmalloc(30);
            lvalp->str.len =
                sprintf(lvalp->str.contents, "L%c\001%lu", s->tok[0],
                        parser_gas->local[s->tok[0]-'0']+1);
            RETURN(ID);
        }
#line 870 "gas-token.c"
yy81:	yych = *++YYCURSOR;
	goto yy82;
yy82:
#line 357 "./modules/parsers/gas/gas-token.re"
{
            /* increment label index */
            parser_gas->local[s->tok[0]-'0']++;
            /* build local label name */
            lvalp->str.contents = yasm_xmalloc(30);
            lvalp->str.len =
                sprintf(lvalp->str.contents, "L%c\001%lu", s->tok[0],
                        parser_gas->local[s->tok[0]-'0']);
            RETURN(LABEL);
        }
#line 885 "gas-token.c"
yy83:	++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
	goto yy84;
yy84:	if(yych == '.')	goto yy65;
	if(yych <= '/')	goto yy3;
	if(yych <= '9')	goto yy83;
	goto yy3;
yy85:	yych = *++YYCURSOR;
	if(yybm[0+yych] & 128) {
		goto yy104;
	}
	goto yy78;
yy86:	yyaccept = 5;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych == '+')	goto yy96;
	if(yych == '-')	goto yy96;
	goto yy97;
yy87:
#line 280 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            lvalp->flt = yasm_floatnum_create(TOK+2);
            s->tok[TOKLEN] = savech;
            RETURN(FLTNUM);
        }
#line 913 "gas-token.c"
yy88:	yyaccept = 5;
	yych = *(YYMARKER = ++YYCURSOR);
	if(yych == '+')	goto yy96;
	if(yych == '-')	goto yy96;
	goto yy97;
yy89:	yyaccept = 6;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
	goto yy90;
yy90:	if(yybm[0+yych] & 32) {
		goto yy89;
	}
	if(yych == '.')	goto yy65;
	if(yych <= '/')	goto yy91;
	if(yych <= '9')	goto yy59;
	goto yy91;
yy91:
#line 247 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            lvalp->intn = yasm_intnum_create_oct(TOK);
            s->tok[TOKLEN] = savech;
            RETURN(INTNUM);
        }
#line 940 "gas-token.c"
yy92:	yych = *++YYCURSOR;
	if(yybm[0+yych] & 64) {
		goto yy93;
	}
	goto yy56;
yy93:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy94;
yy94:	if(yybm[0+yych] & 64) {
		goto yy93;
	}
	goto yy95;
yy95:
#line 256 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            /* skip 0 and x */
            lvalp->intn = yasm_intnum_create_hex(TOK+2);
            s->tok[TOKLEN] = savech;
            RETURN(INTNUM);
        }
#line 964 "gas-token.c"
yy96:	yyaccept = 5;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	goto yy97;
yy97:	if(yych <= '9'){
		if(yych == '.')	goto yy98;
		if(yych <= '/')	goto yy87;
		goto yy96;
	} else {
		if(yych <= 'E'){
			if(yych <= 'D')	goto yy87;
			goto yy100;
		} else {
			if(yych == 'e')	goto yy100;
			goto yy87;
		}
	}
yy98:	yyaccept = 5;
	YYMARKER = ++YYCURSOR;
	if((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	goto yy99;
yy99:	if(yych <= 'D'){
		if(yych <= '/')	goto yy87;
		if(yych <= '9')	goto yy98;
		goto yy87;
	} else {
		if(yych <= 'E')	goto yy100;
		if(yych != 'e')	goto yy87;
		goto yy100;
	}
yy100:	yych = *++YYCURSOR;
	if(yych <= ','){
		if(yych != '+')	goto yy56;
		goto yy101;
	} else {
		if(yych <= '-')	goto yy101;
		if(yych <= '/')	goto yy56;
		if(yych <= '9')	goto yy102;
		goto yy56;
	}
yy101:	yych = *++YYCURSOR;
	if(yych <= '/')	goto yy56;
	if(yych >= ':')	goto yy56;
	goto yy102;
yy102:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy103;
yy103:	if(yych <= '/')	goto yy87;
	if(yych <= '9')	goto yy102;
	goto yy87;
yy104:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy105;
yy105:	if(yybm[0+yych] & 128) {
		goto yy104;
	}
	goto yy106;
yy106:
#line 238 "./modules/parsers/gas/gas-token.re"
{
            savech = s->tok[TOKLEN];
            s->tok[TOKLEN] = '\0';
            lvalp->intn = yasm_intnum_create_bin(TOK+2);
            s->tok[TOKLEN] = savech;
            RETURN(INTNUM);
        }
#line 1035 "gas-token.c"
}
}
#line 413 "./modules/parsers/gas/gas-token.re"


    /* C-style comment; nesting not supported */
comment:
    SCANINIT();

    {

#line 1047 "gas-token.c"
{
	YYCTYPE yych;
	goto yy107;
	++YYCURSOR;
yy107:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	if(yych == '\n')	goto yy111;
	if(yych != '*')	goto yy113;
	goto yy109;
yy109:	yych = *++YYCURSOR;
	if(yych == '/')	goto yy114;
	goto yy110;
yy110:
#line 429 "./modules/parsers/gas/gas-token.re"
{
            if (cursor == s->eof)
                return 0;
            goto comment;
        }
#line 1068 "gas-token.c"
yy111:	yych = *++YYCURSOR;
	goto yy112;
yy112:
#line 423 "./modules/parsers/gas/gas-token.re"
{
            if (parser_gas->save_input)
                cursor = save_line(parser_gas, cursor);
            RETURN(s->tok[0]);
        }
#line 1078 "gas-token.c"
yy113:	yych = *++YYCURSOR;
	goto yy110;
yy114:	yych = *++YYCURSOR;
	goto yy115;
yy115:
#line 421 "./modules/parsers/gas/gas-token.re"
{ parser_gas->state = INITIAL; goto scan; }
#line 1086 "gas-token.c"
}
}
#line 434 "./modules/parsers/gas/gas-token.re"


    /* Single line comment. */
line_comment:
    {
	static unsigned char yybm[] = {
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128,   0, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	};

#line 1130 "gas-token.c"
{
	YYCTYPE yych;
	goto yy116;
yy117:	++YYCURSOR;
yy116:
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy118;
yy118:	if(yybm[0+yych] & 128) {
		goto yy117;
	}
	goto yy119;
yy119:
#line 439 "./modules/parsers/gas/gas-token.re"
{ goto scan; }
#line 1146 "gas-token.c"
}
}
#line 440 "./modules/parsers/gas/gas-token.re"


    /* .section directive (the section name portion thereof) */
section_directive:
    SCANINIT();

    {
	static unsigned char yybm[] = {
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,  64,   0,   0,   0,  64,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	 64,   0,   0,   0, 128,   0,   0,   0, 
	  0,   0,   0,   0,   0, 128, 128,   0, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128,   0,   0,   0,   0,   0,   0, 
	  0, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128,   0,   0,   0,   0, 128, 
	  0, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	};

#line 1192 "gas-token.c"
{
	YYCTYPE yych;
	goto yy120;
	++YYCURSOR;
yy120:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	if(yych <= '$'){
		if(yych <= '\r'){
			if(yych <= '\t'){
				if(yych <= '\b')	goto yy132;
				goto yy126;
			} else {
				if(yych <= '\n')	goto yy130;
				if(yych <= '\f')	goto yy132;
				goto yy126;
			}
		} else {
			if(yych <= '!'){
				if(yych == ' ')	goto yy126;
				goto yy132;
			} else {
				if(yych <= '"')	goto yy124;
				if(yych <= '#')	goto yy132;
				goto yy122;
			}
		}
	} else {
		if(yych <= '@'){
			if(yych <= '.'){
				if(yych <= '+')	goto yy132;
				if(yych <= ',')	goto yy128;
				goto yy122;
			} else {
				if(yych <= '/')	goto yy132;
				if(yych >= ':')	goto yy132;
				goto yy122;
			}
		} else {
			if(yych <= '_'){
				if(yych <= 'Z')	goto yy122;
				if(yych <= '^')	goto yy132;
				goto yy122;
			} else {
				if(yych <= '`')	goto yy132;
				if(yych >= '{')	goto yy132;
				goto yy122;
			}
		}
	}
yy122:	yych = *++YYCURSOR;
	goto yy137;
yy123:
#line 447 "./modules/parsers/gas/gas-token.re"
{
            lvalp->str.contents = yasm__xstrndup(TOK, TOKLEN);
            lvalp->str.len = TOKLEN;
            parser_gas->state = INITIAL;
            RETURN(ID);
        }
#line 1253 "gas-token.c"
yy124:	yych = *++YYCURSOR;
	goto yy125;
yy125:
#line 454 "./modules/parsers/gas/gas-token.re"
{ goto stringconst; }
#line 1259 "gas-token.c"
yy126:	yych = *++YYCURSOR;
	goto yy135;
yy127:
#line 456 "./modules/parsers/gas/gas-token.re"
{ goto section_directive; }
#line 1265 "gas-token.c"
yy128:	yych = *++YYCURSOR;
	goto yy129;
yy129:
#line 458 "./modules/parsers/gas/gas-token.re"
{
            parser_gas->state = INITIAL;
            RETURN(s->tok[0]);
        }
#line 1274 "gas-token.c"
yy130:	yych = *++YYCURSOR;
	goto yy131;
yy131:
#line 463 "./modules/parsers/gas/gas-token.re"
{
            if (parser_gas->save_input)
                cursor = save_line(parser_gas, cursor);
            parser_gas->state = INITIAL;
            RETURN(s->tok[0]);
        }
#line 1285 "gas-token.c"
yy132:	yych = *++YYCURSOR;
	goto yy133;
yy133:
#line 470 "./modules/parsers/gas/gas-token.re"
{
            yasm_warn_set(YASM_WARN_UNREC_CHAR,
                          N_("ignoring unrecognized character `%s'"),
                          yasm__conv_unprint(s->tok[0]));
            goto section_directive;
        }
#line 1296 "gas-token.c"
yy134:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy135;
yy135:	if(yybm[0+yych] & 64) {
		goto yy134;
	}
	goto yy127;
yy136:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy137;
yy137:	if(yybm[0+yych] & 128) {
		goto yy136;
	}
	goto yy123;
}
}
#line 476 "./modules/parsers/gas/gas-token.re"


    /* filename portion of nasm preproc %line */
nasm_filename:
    strbuf = yasm_xmalloc(STRBUF_ALLOC_SIZE);
    strbuf_size = STRBUF_ALLOC_SIZE;
    count = 0;

nasm_filename_scan:
    SCANINIT();

    {
	static unsigned char yybm[] = {
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0, 128,   0,   0,   0, 128,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	128,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 
	};

#line 1363 "gas-token.c"
{
	YYCTYPE yych;
	goto yy138;
	++YYCURSOR;
yy138:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	if(yych <= '\f'){
		if(yych <= '\b')	goto yy144;
		if(yych <= '\t')	goto yy142;
		if(yych >= '\v')	goto yy144;
		goto yy140;
	} else {
		if(yych <= '\r')	goto yy142;
		if(yych == ' ')	goto yy142;
		goto yy144;
	}
yy140:	yych = *++YYCURSOR;
	goto yy141;
yy141:
#line 488 "./modules/parsers/gas/gas-token.re"
{
            strbuf_append(count++, cursor, s, '\0');
            lvalp->str.contents = (char *)strbuf;
            lvalp->str.len = count;
            parser_gas->state = INITIAL;
            RETURN(STRING);
        }
#line 1392 "gas-token.c"
yy142:	yych = *++YYCURSOR;
	goto yy147;
yy143:
#line 496 "./modules/parsers/gas/gas-token.re"
{ goto nasm_filename_scan; }
#line 1398 "gas-token.c"
yy144:	yych = *++YYCURSOR;
	goto yy145;
yy145:
#line 498 "./modules/parsers/gas/gas-token.re"
{
            if (cursor == s->eof) {
                strbuf_append(count++, cursor, s, '\0');
                lvalp->str.contents = (char *)strbuf;
                lvalp->str.len = count;
                parser_gas->state = INITIAL;
                RETURN(STRING);
            }
            strbuf_append(count++, cursor, s, s->tok[0]);
            goto nasm_filename_scan;
        }
#line 1414 "gas-token.c"
yy146:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	goto yy147;
yy147:	if(yybm[0+yych] & 128) {
		goto yy146;
	}
	goto yy143;
}
}
#line 509 "./modules/parsers/gas/gas-token.re"


    /* character constant values */
charconst:
    /*TODO*/

    /* string constant values */
stringconst:
    strbuf = yasm_xmalloc(STRBUF_ALLOC_SIZE);
    strbuf_size = STRBUF_ALLOC_SIZE;
    count = 0;

stringconst_scan:
    SCANINIT();

    {

#line 1443 "gas-token.c"
{
	YYCTYPE yych;
	goto yy148;
	++YYCURSOR;
yy148:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	if(yych == '"')	goto yy152;
	if(yych != '\\')	goto yy154;
	goto yy150;
yy150:	yych = *++YYCURSOR;
	if(yych != '\n')	goto yy155;
	goto yy151;
yy151:
#line 547 "./modules/parsers/gas/gas-token.re"
{
            if (cursor == s->eof) {
                yasm_error_set(YASM_ERROR_SYNTAX,
                               N_("unexpected end of file in string"));
                lvalp->str.contents = (char *)strbuf;
                lvalp->str.len = count;
                RETURN(STRING);
            }
            strbuf_append(count++, cursor, s, s->tok[0]);
            goto stringconst_scan;
        }
#line 1470 "gas-token.c"
yy152:	yych = *++YYCURSOR;
	goto yy153;
yy153:
#line 539 "./modules/parsers/gas/gas-token.re"
{
            strbuf_append(count, cursor, s, '\0');
            yasm_unescape_cstring(strbuf, &count);
            lvalp->str.contents = (char *)strbuf;
            lvalp->str.len = count;
            RETURN(STRING);
        }
#line 1482 "gas-token.c"
yy154:	yych = *++YYCURSOR;
	goto yy151;
yy155:	yych = *++YYCURSOR;
	goto yy156;
yy156:
#line 526 "./modules/parsers/gas/gas-token.re"
{
            if (cursor == s->eof) {
                yasm_error_set(YASM_ERROR_SYNTAX,
                               N_("unexpected end of file in string"));
                lvalp->str.contents = (char *)strbuf;
                lvalp->str.len = count;
                RETURN(STRING);
            }
            strbuf_append(count++, cursor, s, '\\');
            strbuf_append(count++, cursor, s, s->tok[1]);
            goto stringconst_scan;
        }
#line 1501 "gas-token.c"
}
}
#line 558 "./modules/parsers/gas/gas-token.re"

}