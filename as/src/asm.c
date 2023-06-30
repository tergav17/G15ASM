/*
 * asm.c 
 *
 * assembler guts
 */
#include "asm.h"
#include "sio.h"

char token_buf[TOKEN_BUF_SIZE];

/*
 * skips past all of the white space to a token
 */
void asm_wskip()
{
	char comment;
	
	comment = 0;
	while ((sio_peek() <= ' ' || sio_peek() == ';' || comment) && sio_peek() != '\n' && sio_peek() != -1)
		if (sio_next() == ';') comment = 1;
}

/*
 * tests if a character is a alpha (aA - zZ or underscore)
 *
 * in = character to test
 * returns true (1) or false (0)
 */
char asm_alpha(char in)
{
	return (in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z') || in == '_';
}

/*
 * tests if a character is a alpha
 *
 * in = character to test
 * returns true (1) or false (0)
 */
char asm_num(char in)
{
	return (in >= '0' && in <= '9');
}

/*
 * reads the next token in from the source, buffers if needed, and returns type
 * white space will by cycled past, both in front and behind the token
 */
char asm_token_read() 
{
	char c, out;
	int i;

	// skip all leading white space
	asm_wskip();
	
	// peek and check type
	out = c = sio_peek();
	if (asm_alpha(c))
		out = 'a';
	else if (asm_num(c))
		out = '0';
	
	if (out == 'a' || out == '0') {
			// scan in the buffer if needed
		i = 0;
		while (asm_num(c) || asm_alpha(c)) {
			if (i < TOKEN_BUF_SIZE - 1)
				token_buf[i++] = c;
			
			sio_next();
			c = sio_peek();
		}
		token_buf[i] = 0;
	} else {
		sio_next();
	}
	
	// correct for new lines
	if (out == '\n') out = 'n';
	
	// skip more whitespace 
	asm_wskip();
	
	return out;
}

/*
 * expects a specific symbol
 * some symbols have special cases for ignoring trailing or leading line breaks
 *
 * c = symbol to expect
 */
void asm_expect(char c)
{
	char tok;
	
	if (c == '}') {
		while (sio_peek() == '\n')
			asm_token_read();
	}
	
	tok = asm_token_read();
	
	if (tok != c) {
		asm_error ("unexpected character");
	}
	
	if (c == '{' || c == ',') {
		while (sio_peek() == '\n')
			asm_token_read();
	}
}

/*
 * consumes an end of line
 */
void asm_eol()
{
	char tok;
	
	tok = asm_token_read();
	if (tok != 'n' && tok != -1)
		asm_error("expected end of line");
}

/*
 * skips to and consumes an end of line
 */
void asm_skip()
{
	char tok;
	
	do {
		tok = asm_token_read();
	} while (tok != 'n' && tok != -1);
}

void asm_assemble(char flagv) {
	
}