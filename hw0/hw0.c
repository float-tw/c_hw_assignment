#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;

void get_stu_no(char *result);
void nckuhash(const char *stu_no, const char *salt, char *result);
void bytes2hex(BYTE *bytes, size_t count, char *hex);

int main(int argc, char *argv[])
{
	char stu_no[10];
	char hash_value[100];
	const char *salt;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <some_word_you_like>\n", argv[0]);
		return EXIT_FAILURE; /* equivalent to return 1; */
	}
	salt = argv[1];

	get_stu_no(stu_no);
	nckuhash(stu_no, salt, hash_value);
	printf("%s$%s\n", salt, hash_value);
	return EXIT_SUCCESS; /* equivalent to return 0; */
}

void get_stu_no(char *result)
{
	const char *home = getenv("HOME");
	const char *sno_file = ".sno";
	char *filename;
	FILE *file;

	/* compose the full path */
	filename = (char*)malloc(strlen(home) + 1 + strlen(sno_file) + 1);
	sprintf(filename, "%s/%s", home, sno_file);

	/* open file with check if it success */
	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Missing your student number! Contact your TA.\n");
		exit(EXIT_FAILURE);
	}

	/* read file */
	fscanf(file, "%10s", result);

	/* free dynamic allocated memories */
	free(filename);
}

void nckuhash(const char *stu_no, const char *salt, char *result)
{
	const size_t NBYTES = 8;
	BYTE hashsum[NBYTES];
	const char *src;
	size_t i;
	char flag1, flag2;

	// implement hash algorithm
	memset(hashsum, 0, sizeof(hashsum));
	src = salt;
	i = 1;
	flag1 = 0;
	flag2 = 0;
	// using salt to init hashsum
	while(1){
		hashsum[i] = (hashsum[i] ^ (*src)) + (hashsum[(i + NBYTES - 1)%NBYTES]);
		src++, i++;
		if(flag1 && flag2) break;
		if(*src=='\0') {
			src = salt;
			flag1 = 1;
		}
		if(i==NBYTES) {
			i = 0;
			flag2 = 1;
		}
	}
	// start hash
	for(i=0; stu_no[i]; i++)
		hashsum[ i % NBYTES ] += stu_no[i];

	bytes2hex(hashsum, NBYTES, result);
}

void bytes2hex(BYTE *bytes, size_t count, char *hex)
{
	size_t i;
	for (i=0; i < count; i++) {
		sprintf(hex+i*2, "%02x", bytes[i]);
	}
	sprintf(hex+i*2, "%c", 0);
}
