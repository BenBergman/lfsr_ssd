#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define NELEMS(x) (sizeof(x) / sizeof(x[0]))
#define BIT(x) (0b00000001 << (x))

char *map[][3][3] = {
	{
		{ " ", " ", " " },
		{ " ", " ", " " },
		{ " ", " ", " " }
	},
	{
		{ " ", "━", " " },
		{ " ", " ", " " },
		{ " ", " ", " " }
	},
	{
		{ " ", " ", "╻" },
		{ " ", " ", "╹" },
		{ " ", " ", " " }
	},
	{
		{ " ", "━", "╻" },
		{ " ", " ", "╹" },
		{ " ", " ", " " }
	},
	{
		{ " ", " ", " " },
		{ " ", " ", "╻" },
		{ " ", " ", "╹" }
	},
	{
		{ " ", "━", " " },
		{ " ", " ", "╻" },
		{ " ", " ", "╹" }
	},
	{
		{ " ", " ", "╻" },
		{ " ", " ", "╏" },
		{ " ", " ", "╹" }
	},
	{
		{ " ", "━", "╻" },
		{ " ", " ", "╏" },
		{ " ", " ", "╹" }
	},
	{
		{ "╻", "━", "╻" },
		{ "╏", "━", "╏" },
		{ "╹", "━", "╹" }
	},
	{
		{ "╻", "━", "╻" },
		{ "╏", "━", "╏" },
		{ "╹", "━", "╹" }
	},
	{
		{ "╻", "━", "╻" },
		{ "╏", "━", "╏" },
		{ "╹", "━", "╹" }
	},
	{
		{ "╻", "━", "╻" },
		{ "╏", "━", "╏" },
		{ "╹", "━", "╹" }
	}
};

char *get_ssd_char(uint8_t b, int row, int col)
{
	switch (row) {
		case 0:
			switch (col) {
				case 0:
					return (b & BIT(5)) ? "╻" : " ";
					break;
				case 1:
					return (b & BIT(0)) ? "━" : " ";
					break;
				case 2:
					return (b & BIT(1)) ? "╻" : " ";
					break;
				default:
					return " ";
					break;
			}
			break;
		case 1:
			switch (col) {
				case 0:
					if (b & BIT(5) && b & BIT(4)) {
						return "╏";
					} else if (b & BIT(5)) {
						return "╹";
					} else if (b & BIT(4)) {
						return "╻";
					} else {
						return " ";
					}
					break;
				case 1:
					return (b & BIT(6)) ? "━" : " ";
					break;
				case 2:
					if (b & BIT(1) && b & BIT(2)) {
						return "╏";
					} else if (b & BIT(1)) {
						return "╹";
					} else if (b & BIT(2)) {
						return "╻";
					} else {
						return " ";
					}
					break;
				default:
					return " ";
					break;
			}
			break;
		case 2:
			switch (col) {
				case 0:
					return (b & BIT(4)) ? "╹" : " ";
					break;
				case 1:
					return (b & BIT(3)) ? "━" : " ";
					break;
				case 2:
					return (b & BIT(2)) ? "╹" : " ";
					break;
				case 3:
					return (b & BIT(7)) ? "•" : " ";
				default:
					break;
			}
			break;
		default:
			break;
	}
	return " ";
}

char *bytes2ssd(const uint8_t *b, int len)
{
	char *ssd = malloc(28 * sizeof(char) * len);

	for (int row = 0; row < 3; row++) {
		for (int byte = 0; byte < len; byte++) {
			for (int col = 0; col < 4; col++) {
				//strcat(ssd, map[b[byte]][row][col]);
				strcat(ssd, get_ssd_char(b[byte], row, col));
			}
		}
		strcat(ssd, "\n");
	}
	return ssd;
}

char *byte2ssd(uint8_t b)
{
	uint8_t bytes[] = {b};
	return bytes2ssd(bytes, 1);
}

char *ssdcat(const char *a, const char *b)
{
	char *new_ssd = malloc(sizeof(a) + sizeof(b));

	if (a == NULL && b == NULL) {
		return NULL;

	} else if (a == NULL) {
		strcpy(new_ssd, b);
		return new_ssd;

	} else {
		strcpy(new_ssd, a);
		return new_ssd;
	}

	char *a_tok, *b_tok;
	char *a_save, *b_save;

	char *a_cpy = malloc(sizeof(a));
	char *b_cpy = malloc(sizeof(b));

	strcpy(a_cpy, a);
	strcpy(b_cpy, b);

	a_tok = strtok_r (a_cpy, "\n", &a_save);
	b_tok = strtok_r (b_cpy, "\n", &b_save);

	while (a_tok != NULL && b_tok != NULL) {
		strcat(new_ssd, a_tok);
		strcat(new_ssd, b_tok);
		strcat(new_ssd, "\n");

		a_tok = strtok_r (NULL, "\n", &a_save);
		b_tok = strtok_r (NULL, "\n", &b_save);
	}

	return new_ssd;
}

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

bool xnor(bool a, bool b)
{
	if (a == b)
		return 0;

	else
		return 1;
}

void shift_bytes(uint8_t *b)
{
	bool new = xnor(xnor(b[24] & BIT(5), b[24] & BIT(3)), xnor(b[24] & BIT(2), b[24] & BIT(0)));
	for (int i = 24; i > 0; i--) {
		b[i] >>= 1;
		if (b[i - 1] & BIT(0)) {
			b[i] += 128;
		}
	}
	b[0] >>= 1;
	if (new) {
		b[0] += 128;
	}
}

int main(int argc, const char *argv[])
{
//	// ╏━╹╻
//	printf( "╻━╻╻━╻╻━╻╻━╻\n"
//			"╏━╏╏━╏╏━╏╏━╏\n"
//			"╹━╹╹━╹╹━╹╹━╹\n");
//
//	uint8_t bytes[64];
//
//	for (int i = 0; i < NELEMS(bytes); i++) {
//		bytes[i] = i;
//		printf("%3d ", bytes[i]);
//	}
//
//	char *ssd = bytes2ssd(bytes, NELEMS(bytes));
//	printf("\n%s", ssd);
//
//	for (int i = 0; i < NELEMS(bytes); i++) {
//		bytes[i] = i + 64;
//		printf("%3d ", bytes[i]);
//	}
//
//	ssd = bytes2ssd(bytes, NELEMS(bytes));
//	printf("\n%s", ssd);
//
//	for (int i = 0; i < NELEMS(bytes); i++) {
//		bytes[i] = i + 128;
//		printf("%3d ", bytes[i]);
//	}
//
//	ssd = bytes2ssd(bytes, NELEMS(bytes));
//	printf("\n%s", ssd);
//
//	for (int i = 0; i < NELEMS(bytes); i++) {
//		bytes[i] = i + 192;
//		printf("%3d ", bytes[i]);
//	}
//
//	ssd = bytes2ssd(bytes, NELEMS(bytes));
//	printf("\n%s", ssd);
//
//	uint8_t msg[] = {0x6D, 0x76, 0x30, 0x71, 0x78, 0x6E};
//	printf("%s", bytes2ssd(msg, NELEMS(msg)));
//	for (int i = 0; i < NELEMS(msg); i++) {
//		printf("%s ", byte_to_binary(msg[i]));
//	}
//	printf("\n");

	if (argc > 1) {
		uint8_t args[argc - 1];
		char *tmp;
		for (int i = 1; i < argc; i++) {
			args[i - 1] = strtol(argv[i], &tmp, 2);
		}
		printf("%s", bytes2ssd(args, NELEMS(args)));

		for (int i = 0; i < NELEMS(args); i++) {
			printf("%s ", byte_to_binary(args[i]));
		}
		printf("\n");

		shift_bytes(args);

		printf("%s", bytes2ssd(args, NELEMS(args)));

		for (int i = 0; i < NELEMS(args); i++) {
			printf("%s ", byte_to_binary(args[i]));
		}
		printf("\n");

//		shift_bytes(args);
//
//		printf("%s", bytes2ssd(args, NELEMS(args)));
//
//		for (int i = 0; i < NELEMS(args); i++) {
//			printf("%s ", byte_to_binary(args[i]));
//		}
//		printf("\n");
//
//		shift_bytes(args);
//
//		printf("%s", bytes2ssd(args, NELEMS(args)));
//
//		for (int i = 0; i < NELEMS(args); i++) {
//			printf("%s ", byte_to_binary(args[i]));
//		}
//		printf("\n");
//
//		shift_bytes(args);
//
//		printf("%s", bytes2ssd(args, NELEMS(args)));
//
//		for (int i = 0; i < NELEMS(args); i++) {
//			printf("%s ", byte_to_binary(args[i]));
//		}
//		printf("\n");
//		printf("01101101 01110110 00110000 01110001 01111000 01101110")
		printf("199      191      183      175      167      159      151      143      135      127      119      111      103      95       87       79       71       63       55       47       39       31       23       15       7      0\n"
		       "^-- 5 XNOR 3 XNOR 2 XNOR 0\n");
	}

	return 0;
}
