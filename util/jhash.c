#include <util/jhash.h>

long string_jhash(char* s) {
	long hash = 0;
	for (int i = 0; i < strlen(s) && i < 12; i++) {
		hash *= 37;
		if (s[i] >= 'A' && s[i] <= 'Z') {
			hash += (1 + s[i]) - 65;
		} else if (s[i] >= 'a' && s[i] <= 'z') {
			hash += (1 + s[i]) - 97;
		} else if (s[i] >= '0' && s[i] <= '9') {
			hash += (27 + s[i]) - 48;
		}
	}
	while (hash % 37 == 0 && hash != 0) {
		hash /= 37;
	}
	return hash;
}
