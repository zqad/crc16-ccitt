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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

uint16_t crc_calculate_table(const void *data, const size_t len);
uint16_t crc_calculate_algorithm(const void *data, const size_t len);

int main(void) {
	int i;
	size_t datalen;
	uint16_t crc_algorithm;
	uint16_t crc_table;
	uint16_t crc_check_algorithm;
	uint16_t crc_check_table;
	/* Use the examples from
	 * http://srecord.sourceforge.net/crc16-ccitt.html */
	char *testdata[] = {
		/* Empty string */
		"",
		"A",
		"123456789",
		/* 256 x "A" */
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
		NULL
	};
	char temp_buffer[300];

	for (i = 0; testdata[i]; i++) {
		printf("Data: '%s'\n", testdata[i]);

		/* Calculate the CRCs */
		datalen = strlen(testdata[i]);
		crc_algorithm = crc_calculate_algorithm(testdata[i], datalen);
		printf("Algorithm-based CRC: 0x%04hX\n", crc_algorithm);
		crc_table = crc_calculate_table(testdata[i], datalen);
		printf("Table-based CRC:     0x%04hX\n", crc_table);

		/* Add the algorithm CRC after the data, note the big endian
		 * byte order */
		memcpy(temp_buffer, testdata[i], datalen);
		temp_buffer[datalen] = crc_algorithm >> 8;
		temp_buffer[datalen + 1] = crc_algorithm & 0xFF;
		/* Re-run the CRC check, but include the CRC */
		crc_check_algorithm = crc_calculate_algorithm(temp_buffer,
				datalen + 2);
		printf("Algorithm-based CRC check: 0x%04hX\n",
				crc_check_algorithm);

		/* Repeat for the table-based CRC function, the first section
		 * of temp_buffer is still intact */
		temp_buffer[datalen] = crc_table >> 8;
		temp_buffer[datalen + 1] = crc_table & 0xFF;
		crc_check_table = crc_calculate_table(temp_buffer,
				datalen + 2);
		printf("Table-based CRC check:     0x%04hX\n", crc_check_table);

		printf("\n");
	}

	return 0;
}
