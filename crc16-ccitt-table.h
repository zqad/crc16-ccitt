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

static inline uint16_t crc16_ccitt_init(void) {
	return 0xFFFF;
}

extern uint16_t crc16_ccitt_table[];

static inline uint16_t crc16_ccitt_update(uint8_t byte, uint16_t crc) {

	/* The data in the table constructed by running the algorithmic CRC
	 * with inital CRC (crc & 0xFF00), and the input data 0x00. The
	 * resulting CRC can be found in the table at (crc >> 8).
	 *
	 * Since the operation is performed on a zero rightmost half of the
	 * CRC, and a zero input byte, we can construct the new CRC by usin
	 * the rightmost half of the current CRC as the new leftmost half, and
	 * the byte as the leftmost half. We then XOR this construction with
	 * the contents of the table at position (crc >> 8). */
	crc = ((crc << 8) | byte) ^ crc16_ccitt_table[crc >> 8];

	return crc;
}

static inline uint16_t crc16_ccitt_finalize(uint16_t crc) {
	int i;

	/* Augment 16 zero-bits */
	for (i = 0; i < 2; i++) {
		crc = crc16_ccitt_update(0, crc);
	}

	return crc;
}
