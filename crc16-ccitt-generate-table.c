/*
 * Copyright (c) 2014 Jonas Eriksson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>

/*
 * The basic idea of a pre-generated table is that we, for each leftmost
 * byte in the current CRC16, pre-calculate the XOR pattern with a 0x00 input
 * byte. We can then create the next CRC by using the old rightmost byte as
 * the new leftmost byte, the new data as the rightmost byte, and XOR this new
 * 16-bit word with the contents of the lookup table. We index our lookup
 * table using the old leftmost byte, as it is what controls the XOR
 * operations in the CRC.
 */

int main(void) {
	int i, j;
	int xor_flag;
	uint16_t result;

	/* Print header */
	printf("/*\n"
			" * This file is autogenerated by %s\n"
			" * See http://github.com/zqad/crc16-ccitt"
			" for more information\n"
			" */\n\n", __FILE__);

	printf("#include <stdint.h>\n\n");
	printf("uint16_t crc16_ccitt_table[] = {\n\t");
	for (i = 0; i < 256; i++) {
		result = i << 8;
		for (j = 0; j < 8; j++) {
			/* Flag for XOR if leftmost bit is set */
			xor_flag = result & 0x8000;

			/* Shift CRC */
			result <<= 1;

			/* Perform the XOR */
			if (xor_flag)
				result ^= 0x1021;
		}
		printf("0x%04hX,", result);

		/* Pretty formatting */
		if ((i + 1) % 8)
			printf(" ");
		else if (i != 255)
			printf("\n\t");
	}
	printf("\n};\n");
}