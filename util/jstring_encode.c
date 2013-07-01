/**
 * jstring_encode.c
 *
 * Defines the jagex string coding method
 */
#include <util/jstring_encode.h>

#include <string.h>

char enc_table[] = {'_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
					'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
					't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
					'3', '4', '5', '6', '7', '8', '9'};

/**
 * Encodes a string up to 12 chars to a long using jagex'
 * encoding method.
 */
long jstring_encode(char* s) {
	long encoded = 0;
	for (size_t i = 0; i < strlen(s) && i < 12; i++) {
		encoded *= 37;
		if (s[i] >= 'A' && s[i] <= 'Z') {
			encoded += (1 + s[i]) - 65;
		} else if (s[i] >= 'a' && s[i] <= 'z') {
			encoded += (1 + s[i]) - 97;
		} else if (s[i] >= '0' && s[i] <= '9') {
			encoded += (27 + s[i]) - 48;
		}
	}
	while (encoded % 37 == 0 && encoded != 0) {
		encoded /= 37;
	}
	return encoded;
}

/**
 * Decodes a string up to 12 chars from a long using jagex'
 * encoding method.
 */
void jstring_decode(long l, char* buf) {
	if (l <= 0 || l >= 0x5b5b57f8a98a5dd1) { // Why this max?
		strcpy(buf, "invalid_name");
		return;
	}
	if (!(l % 37)) {
		strcpy(buf, "invalid_name");
		return;
	}
	int i = 0;
	while (l) {
		long tmp = l;
		l /= 37;
		buf[11 - i++] = enc_table[tmp - (l * 37)];
	}
}
