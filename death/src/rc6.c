/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc6.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <amatilda@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 08:41:44 by amatilda          #+#    #+#             */
/*   Updated: 2021/06/21 05:29:31 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <x86intrin.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "rc6.h"

void rc6_init(const void *key, RC6_WORD *S, size_t len)
{
	RC6_WORD						L[RC6_KEY_MAX / sizeof(RC6_WORD)];
	RC6_WORD						x;
	RC6_WORD						y;
	size_t							k;
	size_t							n;
	size_t							i;
	size_t							j;
	size_t							c;

	if (len > RC6_KEY_MAX)
		len = RC6_KEY_MAX;
	memcpy(&L[0], key, len);
	memset((uint8_t *)&L[0] + len, 0x0, RC6_KEY_MAX - len);
	if (len == 0)
		c = 1;
	else
		c = ((len + (sizeof(RC6_WORD) - 1)) & (0 - sizeof(RC6_WORD))) / sizeof(RC6_WORD);
	S[0] = RC6_P;
	i = 1;
	while (i <= (RC6_T - 1))
	{
		S[i] = S[i - 1] + RC6_Q;
		i++;
	}
	x = 0;
	y = 0;
	i = 0;
	j = 0;
	k = 0;
	n = (RC6_T > c) ? RC6_T : c;
	n = n * 3;
	while (k < n)
	{
		x = ROTL((S[i] + x + y), 3);
		y = ROTL((L[j] + x + y), x + y);
		S[i] = x;
		L[j] = y;
		i = (i + 1) % RC6_T;
		j = (j + 1) % c;
		k++;
	}
}


void rc6_encrypt(RC6_WORD *S, const RC6_WORD *in, RC6_WORD *out, size_t num_block)
{
	RC6_WORD						a;
	RC6_WORD						b;
	RC6_WORD						c;
	RC6_WORD						d;
	RC6_WORD						t;
	RC6_WORD						x;
	RC6_WORD						u;
	size_t							i;

	while (num_block-- != 0)
	{
		a = in[0];
		b = in[1] + S[0];
		c = in[2];
		d = in[3] + S[1];
		i = 2;
		while (i <= (2 * RC6_R))
		{
			t = ROTL(b * (2 * b + 1), RC6_LG);
			u = ROTL(d * (2 * d + 1), RC6_LG);
			a = ROTL(a ^ t, u) + S[i];
			c = ROTL(c ^ u, t) + S[i + 1];
			x = a;
			a = b;
			b = c;
			c = d;
			d = x;
			i += 2;
		}
		out[0] = a + S[2 * RC6_R + 2];
		out[1] = b;
		out[2] = c + S[2 * RC6_R + 3];
		out[3] = d;
		out += 4;
		in += 4;
	}
}

void rc6_decrypt(RC6_WORD *S, const RC6_WORD *in, RC6_WORD *out, size_t num_block)
{
	RC6_WORD						a;
	RC6_WORD						b;
	RC6_WORD						c;
	RC6_WORD						d;
	RC6_WORD						t;
	RC6_WORD						x;
	RC6_WORD						u;
	size_t							i;

	while (num_block-- != 0)
	{
		a = in[0] - S[2 * RC6_R + 2];
		b = in[1];
		c = in[2] - S[2 * RC6_R + 3];
		d = in[3];
		i = (2 * RC6_R);
		while (i >= 2)
		{
			x = d;
			d = c;
			c = b;
			b = a;
			a = x;
			u = ROTL(d * (2 * d + 1), RC6_LG);
			t = ROTL(b * (2 * b + 1), RC6_LG);
			c = ROTR(c - S[i + 1], t) ^ u;
			a = ROTR(a - S[i], u) ^ t;
			i -= 2;
		}
		out[0] = a;
		out[1] = b - S[0];
		out[2] = c;
		out[3] = d - S[1];
		out += 4;
		in += 4;
	}
}

#define RC6_STATUS_CODE_OK									0x0
#define RC6_STATUS_CODE_ERROR_STREAM						0x1
#define RC6_STATUS_CODE_ERROR_BUFFER_STREAM					0x2
#define RC6_STATUS_CODE_ERROR_NOT_FIND_ARG					0x3
#define RC6_STATUS_CODE_ERROR_INVALID_NUMBER_ARG			0x4
#define RC6_STATUS_CODE_ERROR_INVALID_FILE_ARG				0x5
#define RC6_STATUS_CODE_ERROR_INVALID_OPEN					0x6
#define RC6_STATUS_CODE_ERROR_MEMORY						0x7
#define RC6_STATUS_CODE_ERROR_STAT							0x8
#define RC6_STATUS_CODE_ERROR_FILE_RANG						0x9
#define RC6_STATUS_CODE_ERROR_ALING							0xA


#define RC6_ARG_START									"rc6_start: "
#define RC6_ARG_END										"rc6_end: "
#define RC6_ARG_KEY										"rc6_key: "
#define RC6_ARG_KEY_LEN									"rc6_key_len: "
#define RC6_ARG_BASE									"rc6_base: "

static size_t _findNumber(const char *find, const char *needle, size_t len)
{
	size_t					number;

	if ((find = strstr(find, needle)) == 0x0)
	{
		dprintf(STDERR_FILENO, "Argument %s won't find\n", needle);
		exit(RC6_STATUS_CODE_ERROR_NOT_FIND_ARG);
	}
	find = find + len;
	errno = 0x0;
	number = strtol(find, 0x0, 0x10);
	if (errno != 0x0)
	{
		dprintf(STDERR_FILENO, "Not a valid number - %s\n", needle);
		exit(RC6_STATUS_CODE_ERROR_INVALID_NUMBER_ARG);
	}
	return (number);
}

int	main(int argc, char **argv)
{
	static char			buff[0x1000];
	RC6_WORD			S[RC6_T];
	struct stat			st;
	ssize_t				count;
	size_t				rc6_start;
	size_t				rc6_end;
	size_t				rc6_key;
	size_t				rc6_base;
	size_t				rc6_key_len;
	int					fd;
	void				*map;
	size_t				len;

	if (argc != 0x2)
	{
		dprintf(STDERR_FILENO, "Invalid arguments\n");
		return (RC6_STATUS_CODE_ERROR_INVALID_FILE_ARG);
	}
	if ((count = read(STDIN_FILENO, &buff[0x0], sizeof(buff))) == -1)
	{
		dprintf(STDERR_FILENO, "%s\n", strerror(errno));
		return (RC6_STATUS_CODE_ERROR_STREAM);
	}
	if ((size_t)count >= sizeof(buff))
	{
		dprintf(STDERR_FILENO, "Too long text in the pipe\n");
		return (RC6_STATUS_CODE_ERROR_BUFFER_STREAM);
	}
	buff[count] = 0x0;
	rc6_start = _findNumber(&buff[0x0], RC6_ARG_START,(sizeof(RC6_ARG_START) - 0x1));
	rc6_end = _findNumber(&buff[0x0], RC6_ARG_END, (sizeof(RC6_ARG_END) - 0x1));
	rc6_key = _findNumber(&buff[0x0], RC6_ARG_KEY, (sizeof(RC6_ARG_KEY) - 0x1));
	rc6_key_len = _findNumber(&buff[0x0], RC6_ARG_KEY_LEN, (sizeof(RC6_ARG_KEY_LEN) - 0x1));
	rc6_base = _findNumber(&buff[0x0], RC6_ARG_BASE, (sizeof(RC6_ARG_BASE) - 0x1));
	rc6_key = rc6_key + rc6_base;
	rc6_end = rc6_end + rc6_base;
	rc6_start = rc6_start + rc6_base;
	if ((fd = open(argv[0x1], O_RDWR)) == -1)
	{
		dprintf(STDERR_FILENO, "%s\n", strerror(errno));
		return (RC6_STATUS_CODE_ERROR_INVALID_OPEN);
	}
	if (fstat(fd, &st) == -1)
	{
		dprintf(STDERR_FILENO, "%s\n", strerror(errno));
		return (RC6_STATUS_CODE_ERROR_STAT);
	}
	len = st.st_size;
	if ((map = mmap(0x0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x0)) == MAP_FAILED)
	{
		close(fd);
		dprintf(STDERR_FILENO, "%s\n", strerror(errno));
		return (RC6_STATUS_CODE_ERROR_MEMORY);
	}
	close(fd);
	if (len < rc6_key || len < rc6_start || len < rc6_end || len < (rc6_key_len + rc6_key))
	{
		munmap(map, len);
		dprintf(STDERR_FILENO, "The file size does not match the arguments.\n");
		return (RC6_STATUS_CODE_ERROR_FILE_RANG);
	}
	if ((rc6_start & 0xF) != 0x0 || (rc6_end & 0xF) != 0x0)
	{
		munmap(map, len);
		dprintf(STDERR_FILENO, "The data for encryption is not aligned.\n");
		return (RC6_STATUS_CODE_ERROR_ALING);
	}
	rc6_init(map + rc6_key, &S[0], rc6_key_len);
	rc6_encrypt(&S[0], map + rc6_start, map + rc6_start, ((rc6_end - rc6_start) / (sizeof(RC6_WORD) * 4)));
	// rc6_decrypt(&S[0], map + rc6_start, map + rc6_start, ((rc6_end - rc6_start) / (sizeof(RC6_WORD) * 4)));
	munmap(map, len);
	return (RC6_STATUS_CODE_OK);
}


/*
struct test_struct
{
	int keylen;
	unsigned char key[32];
	unsigned int pt[4];
	unsigned int ct[4];
} tests[] =
{
	{ 16,   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00000000, 0x00000000, 0x00000000, 0x00000000},
		{0x36a5c38f, 0x78f7b156, 0x4edf29c1, 0x1ea44898},
	},

	{ 16,   {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78},
		{0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce},
		{0x2f194e52, 0x23c61547, 0x36f6511f, 0x183fa47e},
	},

	{ 24,   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00000000, 0x00000000, 0x00000000, 0x00000000},
		{0xcb1bd66c, 0x38300b19, 0x163f8a4e, 0x82ae9086},
	},

	{ 24,   {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
		 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0},
		{0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce},
		{0xd0298368, 0x0405e519, 0x2ae9521e, 0xd49152f9},
	},

	{ 32,   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00000000, 0x00000000, 0x00000000, 0x00000000},
		{0x05bd5f8f, 0xa85fd110, 0xda3ffa93, 0xc27e856e},
	},

	{ 32,   {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
		 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0,
		 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe},
		{0x35241302, 0x79685746, 0xbdac9b8a, 0xf1e0dfce},
		{0x161824c8, 0x89e4d7f0, 0xa116ad20, 0x485d4e67},
	},

	{       0,
	}
};

int	main(int argc, char **argv)
{
	RC6_WORD						out[4];
	RC6_WORD						S[RC6_T];

	int i;
	struct test_struct *p;

	
	for (p = tests, i = 1; p->keylen; p++, i++)
	{
		rc6_init(p->key, &S[0], p->keylen);
		rc6_encrypt(&S[0], p->pt, out, (sizeof(p->pt) / (sizeof(RC6_WORD) * 4)));
		printf("Test   %d:  %08x %08x %08x %08x\n",
			i, out[0], out[1], out[2], out[3]);
		printf("Should be: %08x %08x %08x %08x\n",
			p->ct[0], p->ct[1], p->ct[2], p->ct[3]);
		rc6_decrypt(&S[0], out, out, (sizeof(out) / (sizeof(RC6_WORD) * 4)));
		printf("Plain:     %08x %08x %08x %08x\n",
			out[0], out[1], out[2], out[3]);
		printf("Should be: %08x %08x %08x %08x\n\n",
			p->pt[0], p->pt[1], p->pt[2], p->pt[3]);
	}
	return (0);
}
*/