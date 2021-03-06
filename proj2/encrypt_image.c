/*
 * Image encryption tool.
 * 
 * Usage: encrypt_image [-e|-d] [-v|-a|-t] input_image output_image
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ppm.h"

#define MODULE 256

/* Function prototypes. */
void vigenere(unsigned char *buf, int len);
void affine_enc(unsigned char *buf, int len);
void affine_dec(unsigned char *buf, int len);
void tea_enc(unsigned char *buf, int len);
void tea_dec(unsigned char *buf, int len);

/* Program arguments. */
FILE *input, *output;

/* Main program. */
int main(int argc, char *argv[]) {
	int encrypt = 0;
	pic image;

	/* Check number of arguments. */
	if (argc != 5) {
		fprintf(stderr,
				"\nUsage: encrypt_image [-e|-d] [-v|-a|-t] input_image output_image\n");
		exit(1);
	}
	/* Check mode. */
	if (argv[1][0] != '-' || strlen(argv[1]) != 2) {
		fprintf(stderr,
				"\nUse -e to encrypt and -d to decrypt..\n");
		exit(1);
	}
	encrypt = (argv[1][1] == 'e');
	/* Check algorithm. */
	if (argv[2][0] != '-' || strlen(argv[1]) != 2) {
		fprintf(stderr, "\nUnknown algorithm, use [-v|-a|-t].\n");
		exit(1);
	}
	/* Check files. */
	if ((input = fopen(argv[3], "rb")) == NULL) {
		fprintf(stderr, "\nN�o � poss�vel ler arquivo de entrada.\n");
		exit(1);
	}
	if ((output = fopen(argv[4], "wb")) == NULL) {
		fprintf(stderr, "\nN�o � poss�vel gravar em arquivo de sa�da.\n");
		exit(1);
	}

	if (ppm_read(input, &image) == 0) {
		fprintf(stderr, "\nN�o � poss�vel ler image de entrada.\n");
		exit(1);
	}

	/* Pass control to chosen algorithm. */
	switch (argv[2][1]) {
		case 'v':
			vigenere(image.pix, image.nx * image.ny * 3);
			break;
		case 'a':
			if (encrypt)
				affine_enc(image.pix, image.nx * image.ny * 3);
			else
				affine_dec(image.pix, image.nx * image.ny * 3);
			break;
		case 't':
            if (encrypt)
                    tea_enc(image.pix, image.nx * image.ny * 3);
            else
                    tea_dec(image.pix, image.nx * image.ny * 3);
            break;
	}

	if (ppm_write(output, &image) == 0) {
		fprintf(stderr, "\nN�o � poss�vel gravar image de sa�da.\n");
		exit(1);
	}

	fclose(input);
	fclose(output);
}

/* Reads the key from user imput, returns it's size */
/*int getKey(unsigned char *key;) {
	int i, length;
	unsigned int byte;
	
	printf("Key size: ");
	if (scanf("%d", &length) != 1) {
		fprintf(stderr, "\nInvalid key size.\n");
		exit(1);
	}
	key = (char *)malloc((length + 1) * (sizeof(char)));
	getchar();

	printf("Encryption key: ");
	for (i = 0; i < length; i++) {
		if (scanf("%u", &byte) != 1) {
			fprintf(stderr, "\nError reading encryption key.\n");
			exit(1);
		}
		key[i] = byte % 256;
	}
	key[length] = '\0';

	return lenght;
} */

/* Implements the Vigenere cipher. */
void vigenere(unsigned char *buf, int len) {
	int i, length;
	unsigned int byte;
	unsigned char *key;

	printf("Key size: ");
	if (scanf("%d", &length) != 1) {
		fprintf(stderr, "\nInvalid key size.\n");
		exit(1);
	}
	key = (char *)malloc((length + 1) * (sizeof(char)));
	getchar();

	printf("Encryption key: ");
	for (i = 0; i < length; i++) {
		if (scanf("%u", &byte) != 1) {
			fprintf(stderr, "\nError reading encryption key.\n");
			exit(1);
		}
		key[i] = byte % 256;
	}
	key[length] = '\0';

	for (i = 0; i < len; i++) {
		buf[i] = buf[i] ^ key[i % length];
	}
	free(key);
}

/* Reads the value of the key ab from the user */
void getAffineKey(unsigned char *key_a, unsigned char *key_b) {
	unsigned int byte;

	printf("Encryption key part A: ");
	if (scanf("%u", &byte) != 1) {
		fprintf(stderr, "\nError reading encryption key part A.\n");
		exit(1);
	} else {
		*key_a = byte % 256;
	}
	printf("Encryption key part B: ");
	if (scanf("%u", &byte) != 1) {
		fprintf(stderr, "\nError reading encryption key part B.\n");
		exit(1);
	} else {
		*key_b = byte % 256;
	}
}

/* Greatest Common Divisor */
gcd(int a, int b)
{
  int aux;
  while(a != 0) {
	aux = a; 
	a = b%a;  
	b = aux;
  }
  return b;
}

/* Implements the affine encryption */
void affine_enc(unsigned char *buf, int len) {
	int i, length;
	unsigned int byte;
	unsigned char key_a, key_b;

	getAffineKey(&key_a, &key_b);

	if(gcd(key_a, MODULE) != 1) {
		fprintf(stderr, "\nError: key A %d and module value %d are not coprimes.\n", key_a, MODULE);
		exit(1);
	}

	for (i = 0; i < len; i++) {
		buf[i] = (buf[i] * key_a + key_b) % MODULE;
	}
}


/* Implements the affine decryption */
void affine_dec(unsigned char *buf, int len) {
	int i, length;
	unsigned int byte;
	unsigned char key_a, key_b;
	getAffineKey(&key_a, &key_b);

	if(gcd(key_a, MODULE) != 1) {
		fprintf(stderr, "\nError: key A %d and module value %d are not coprimes.\n", key_a, MODULE);
		exit(1);
	}

	for (i = 0; i < len; i++) {
		buf[i] = ((buf[i] - key_b) / key_a) % MODULE;
	}
}


/* Implements the TEA encryption */
void tea_enc(unsigned char *buf, int len) {

	
}

/* Implements the TEA decryption */
void tea_dec(unsigned char *buf, int len) {

	
}
