
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include "lz4k.h"

#define LZ4K_STATUS_OK				0x0
#define LZ4K_STATUS_NOT_ARG			0x1
#define LZ4K_STATUS_NOT_OPEN		0x2
#define LZ4K_STATUS_NOT_SIZE		0x3
#define LZ4K_STATUS_NOT_MALLOC		0x4
#define LZ4K_STATUS_NOT_READ		0x5
#define LZ4K_STATUS_NOT_CRC			0x6
#define LZ4K_STATUS_NOT_WRITE		0x7

#define LZ4K_MAX_COMPRESS_WINDOW	0xFFFF
#define LZ4K_NEW_FILE				".lz4k"
#define LZ4K_MAGIC					0x6B347A6C
#define LZ4K_VERSION				0x1

/**
 * @brief 
 * 
 * @param crc32 - перваначальное значение crc
 * @param lp - указаьель на таблицу работает с глубиной 1 - не будем делать парлеризацию
 * @param src - указатель на данные для подсчета
 * @param count - сколько данных в src
 * @return uint32_t 
 */
uint32_t	crc32_zlib_stream(uint32_t *table, const void *src, size_t count, uint32_t crc32)
{
	const void		*src_end;
	uint8_t			tempos;

	src_end = src + count;
	while (src < src_end)
	{
		tempos = ((char *)src++)[0];
		tempos = tempos ^ (uint8_t)crc32;
		crc32 = crc32 >> 8;
		crc32 = crc32 ^ table[tempos];
	}
	return (crc32);
}

static int _usage(char *app_name)
{
	dprintf(STDERR_FILENO, "Usage: -c[-b -w]|-d %s [FILE]...\n", app_name);
	return (LZ4K_STATUS_NOT_ARG);
}

static size_t _lz4k_compress_duple(const uint8_t *b, size_t len, size_t *litter_count)
{
	size_t							i;
	size_t							tempos;
	
	i = 0x1;
	while (i < 0x80 && i * 0x2 <= len)
	{
		if (memcmp(&b[0], &b[i], i) == 0)
		{
			tempos = i;
			while (tempos + i <= len && memcmp(&b[0], &b[tempos], i) == 0)
				tempos = tempos + i;
			tempos = tempos - i;
			if (tempos >= LZ4K_MIN_COMPRESS)
			{
				litter_count[0] = i;
				return (tempos);
			}
		}
		i++;
	}
	return (0x0);
}

static uint8_t *_lz4k_compress_set_len(uint8_t *dst, size_t count)
{
	if (count < 0xF)
		return (dst);
	count = count - 0xF;
	while (count >= 0xFF)
	{
		dst++[0] = 0xFF;
		count = count - 0xFF;
	}
	dst++[0] = count;
	return (dst);
}

static size_t _lz4k_compress_match(const uint8_t *w, const uint8_t *b, size_t len, size_t *offset)
{
	uint8_t							litter;
	size_t							i;
	size_t							tempos;
	size_t							best_i;
	size_t							best_offset;

	if (len < LZ4K_MIN_COMPRESS || b - w < LZ4K_MIN_COMPRESS)
		return (0x0);
	best_i = 0x0;
	best_offset = 0x0;
	litter = b[0];
	while (w < b)
	{
		if (w[0] == litter)
		{
			if ((tempos = b - w) > len)
				tempos = len;
			i = 0x1;
			while (i <= tempos && memcmp(&w[0], &b[0], i) == 0)
				i++;
			i--;
			if (best_i < i)
			{
				best_i = i;
				best_offset = b - w;
			}
		}
		w++;
	}
	if (best_i < LZ4K_MIN_COMPRESS)
		return (0x0);
	offset[0] = best_offset;
	return (best_i);
}


static uint8_t *lz4k_compress_soled(const uint8_t *b, const uint8_t *e, uint8_t *dst, size_t len_win)
{
	size_t							tempos;
	size_t							offset;
	size_t							count;
	size_t							litter_count;
	const uint8_t					*w;
	const uint8_t					*save;

	w = b;
	save = b;
	while (b < e)
	{
		if ((tempos = (b - w)) > len_win)
			w = w + (tempos - len_win);
		if ((count = _lz4k_compress_match(w, b, e - b, &offset)) != 0x0)
		{
			count = count - (LZ4K_MIN_COMPRESS - 0x1);
			litter_count = (b - save);
			dst++[0] = ((litter_count >= 0xF ? 0xF : litter_count) << 0x4) | (count >= 0xF ? 0xF : count);
			dst = _lz4k_compress_set_len(dst, litter_count);
			memcpy(dst, save, litter_count);
			dst = dst + litter_count;
			dst++[0] = (offset & 0xFF);
			dst++[0] = offset >> 0x8;
			dst = _lz4k_compress_set_len(dst, count);
			b = b + count + (LZ4K_MIN_COMPRESS - 0x1);
			save = b;
		}
		else if ((count = _lz4k_compress_duple(b, e - b, &offset)) != 0)
		{
			count = count - (LZ4K_MIN_COMPRESS - 0x1);
			litter_count = offset + (b - save);
			dst++[0] = ((litter_count >= 0xF ? 0xF : litter_count) << 0x4) | (count >= 0xF ? 0xF : count);
			dst = _lz4k_compress_set_len(dst, litter_count);
			memcpy(dst, save, litter_count);
			dst = dst + litter_count;
			dst++[0] = (offset & 0xFF);
			dst++[0] = offset >> 0x8;
			dst = _lz4k_compress_set_len(dst, count);
			b = b + count + offset + (LZ4K_MIN_COMPRESS - 0x1);
			save = b;
		}
		else
			b++;
	}
	if ((tempos = (b - save)) != 0)
	{
		dst++[0] = ((tempos >= 0xF ? 0xF : tempos) << 0x4);
		dst = _lz4k_compress_set_len(dst, tempos);
		memcpy(dst, save, tempos);
		dst = dst + tempos;
	}
	return (dst);
}

static size_t _lz4k_block_size(size_t len_block)
{
	size_t							count;
	size_t							tempos;

	tempos = len_block;
	count = 0x0;
	while (tempos != 0x0)
	{
		count++;
		tempos = tempos >> 0x8;
	}
	if ((len_block & (count * 0x8)) != 0)
		count++;
	return (count);
}

size_t lz4k_compress_block(const void *src, void *dst, size_t len, size_t len_block, size_t len_win)
{
	const uint8_t					*b;
	uint8_t							*buffer;
	uint8_t							*tmp;
	size_t							step;
	size_t							count;
	size_t							tempos;

	b = src;
	buffer = dst;
	count = _lz4k_block_size(len_block);
	while (len != 0)
	{
		step = (len > len_block) ? len_block : len;
		tmp = buffer;
		buffer = lz4k_compress_soled(b, b + step, buffer + count, len_win);
		tempos = buffer - tmp - count;
		if (tempos > len_block)
			dprintf(STDERR_FILENO, "Warning!!! - The compressed block size 0x%lx has exceeded the block size 0x%lx.\n", tempos, len_block);
		memcpy(tmp, &tempos, count);
		len = len - step;
		b = b + step;
	}
	return ((void *)buffer - dst);
}

size_t lz4k_decompress_soled(const void *src, void *dst, size_t len_src)
{
	const uint8_t					*src_b;
	const uint8_t					*tmp;
	const uint8_t					*src_e;
	uint8_t							*dst_b;
	size_t							count;
	size_t							tempos;
	size_t							count_litter;
	size_t							count_match;
	size_t							offset;
	
	src_b = src;
	dst_b = dst;
	src_e = src_b + len_src;
	while (src_b < src_e)
	{
		count = src_b++[0];
		count_litter = count >> 0x4;
		if (count_litter == 0xF)
		{
			while ((tempos = src_b++[0]) == 0xFF)
				count_litter = count_litter + tempos;
			count_litter = count_litter + tempos;
		}
		memcpy(dst_b, src_b, count_litter);
		dst_b = dst_b + count_litter;
		src_b = src_b + count_litter;
		count_match = count & 0xF;
		if (count_match == 0x0)
			continue ;
		offset = src_b[1] << 0x8 | src_b[0];
		src_b = src_b + 0x2;
		if (count_match == 0xF)
		{
			while ((tempos = src_b++[0]) == 0xFF)
				count_match = count_match + tempos;
			count_match = count_match + tempos;
		}
		count_match = count_match + (LZ4K_MIN_COMPRESS - 0x1);
		tmp = dst_b - offset;
		if (offset < count_match)
		{
			count_match = count_match / offset;
			while (count_match-- != 0)
			{
				memcpy(dst_b, tmp, offset);
				dst_b = dst_b + offset;
			}
		}
		else
		{
			memcpy(dst_b, tmp, count_match);
			dst_b = dst_b + count_match;
		}
		
	}
	return (dst_b - (uint8_t *)dst);
}


size_t lz4k_decompress_block(const void *src, void *dst, size_t len_src, size_t len_block)
{
	const uint8_t					*b;
	uint8_t							*buffer;
	size_t							step;
	size_t							count;
	size_t							tempos;

	b = src;
	buffer = dst;
	count = _lz4k_block_size(len_block);
	while (len_src != 0)
	{
		step = 0x0;
		memcpy(&step, b, count);
		tempos = lz4k_decompress_soled(b + count, buffer, step);
		buffer = buffer + tempos;
		len_src = len_src - step - count;
		b = b + step + count;
	}
return (buffer - (uint8_t *)dst);
}

static uint32_t _crc32_table[0x100] =
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

static int _decompess(char *app_name, int argc, char **argv)
{
	printf("%s: not implemented...", app_name);
	return (0x0);
	(void)argc;
	(void)argv;
}

static int _compess(char *app_name, int argc, char **argv)
{
	char						**b;
	char						**e;
	char						*tmp;
	static char					*arg_b = "0x1000";
	static char					*arg_w = "0x1000";
	size_t						block;
	size_t						window;
	int							fd;
	char						*file_name;
	struct stat					st;
	off_t						size_file;
	off_t						compressed_size;
	char						*file_name_new;
	uint8_t						*dst;
	uint8_t						*src;
	size_t						tempos;
	uint32_t					crc32_src;
	time_t						time1;
	time_t						time2;
	size_t						mode;
	static lz4k_t				lz4k =
	{
		.crc32_zlib = 0x0,
		.magic = LZ4K_MAGIC,
		.compressed_size = 0x0,
		.uncompressed_size = 0x0,
		.block_size = 0x0,
		.version = LZ4K_VERSION,
		.reserved = {0x0}
	};

	if (argc < 0x3)
		return (_usage(app_name));
	b = &argv[0x2];
	e = &argv[argc];
	mode = LZ4K_MODE_SOLED;
	while (b < e)
	{
		tmp = b[0];
		if (tmp[0] != '-')
			break ;
		b++;
		if (tmp[0x1] == '-' && tmp[0x2] == 0x0)
			break ;
		if (strlen(tmp) != 0x2 && b == e)
			return (_usage(app_name));
		switch (toupper(tmp[0x1]))
		{
				case 'M':
					if ((e - b ) == 0x1)
						return (_usage(app_name));
					tmp = b[0];
					if (strcmp("soled", tmp) == 0x0)
						mode = LZ4K_MODE_SOLED;
					else if (strcmp("block", tmp) == 0x0)
						mode = LZ4K_MODE_BLOCK;
					else
						return (_usage(app_name));
					break ;
				case 'B':
					arg_b = b[0];
					break ;
				case 'W':
					arg_w = b[0];
					break ;
				default:
					return (_usage(app_name));
					break ;
		}
		b++;
	}
	errno = 0x0;
	block = strtol(arg_b, 0x0, 0x10);
	if (errno != 0x0)
		return (_usage(app_name));
	window = strtol(arg_w, 0x0, 0x10);
	if (errno != 0x0)
		return (_usage(app_name));
	if (window > LZ4K_MAX_COMPRESS_WINDOW)
		return (_usage(app_name));
	if ((e - b ) == 0 || (e - b) != 1)
		return (_usage(app_name));
	file_name = b[0];
	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		dprintf(STDERR_FILENO, "%s: Could not open file: %s\n", app_name, file_name);
		return (LZ4K_STATUS_NOT_OPEN);
	}
	if (fstat(fd, &st) != 0)
	{
		close(fd);
		dprintf(STDERR_FILENO, "%s: Failed to get file size: %s\n", app_name, file_name);
		return (LZ4K_STATUS_NOT_SIZE);
	}
	if ((size_file = st.st_size) == 0x0)
	{
		close(fd);
		dprintf(STDERR_FILENO, "%s: File has zero size: %s\n", app_name, file_name);
		return (LZ4K_STATUS_NOT_SIZE);
	}
	if ((src = malloc(size_file)) == 0x0)
	{
		close(fd);
		dprintf(STDERR_FILENO, "%s: Failed to allocate memory.\n", app_name);
		return (LZ4K_STATUS_NOT_MALLOC);
	}
	if ((dst = malloc(size_file + (size_file >> 0x1))) == 0x0)
	{
		close(fd);
		free(src);
		dprintf(STDERR_FILENO, "%s: Failed to allocate memory.\n", app_name);
		return (LZ4K_STATUS_NOT_MALLOC);
	}
	tempos = strlen(file_name);
	if ((file_name_new = malloc(tempos + sizeof(LZ4K_NEW_FILE))) == 0x0)
	{
		close(fd);
		free(src);
		free(dst);
		dprintf(STDERR_FILENO, "%s: Failed to allocate memory.\n", app_name);
		return (LZ4K_STATUS_NOT_MALLOC);
	}
	memcpy(file_name_new, file_name, tempos);
	memcpy(&file_name_new[tempos], LZ4K_NEW_FILE, sizeof(LZ4K_NEW_FILE));
	if (read(fd, src, size_file) == -1)
	{
		close(fd);
		free(src);
		free(dst);
		dprintf(STDERR_FILENO, "%s: Could not read the file: %s\n", app_name, file_name);
		return (LZ4K_STATUS_NOT_READ);
	}
	close(fd);
	time(&time1);
	crc32_src = crc32_zlib_stream(_crc32_table, src, size_file, 0xFFFFFFFF);
	if (mode == LZ4K_MODE_BLOCK)
		compressed_size = lz4k_compress_block(src, dst, size_file, block, window);
	else
		compressed_size = lz4k_compress_soled(src, src + size_file , dst, window) - dst;
	lz4k.uncompressed_size = size_file;
	lz4k.compressed_size = compressed_size;
	lz4k.block_size = block;
	lz4k.mode = mode;
	time(&time2);
	printf("Timing compresing: %.0f seconds.\n", difftime(time2, time1));
	printf("Unpacked file size: %llu bytes.\n", (long long unsigned int)size_file);
	printf("Packed data size: %llu bytes.\n", (long long unsigned int)compressed_size);
	printf("Compression ratio: %3.2f%%.\n\n", (double)(compressed_size  / (double)(size_file / 100)));
	time(&time1);
	src[0] = src[0] ^ src[0];
	if (mode == LZ4K_MODE_BLOCK)
		size_file = lz4k_decompress_block(dst, src, compressed_size, block);
	else
		size_file = lz4k_decompress_soled(dst, src, compressed_size);
	if (crc32_zlib_stream(_crc32_table, src, size_file, 0xFFFFFFFF) != crc32_src)
	{
		dprintf(STDERR_FILENO, "%s: Checksum does not match when unpacking.\n", app_name);
		return (LZ4K_STATUS_NOT_CRC);
	}
	printf("Timing uncompresing: %.0f seconds.\n", difftime(time2, time1));
	time(&time2);
	if ((fd = open(file_name_new, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) == -1)
	{
		free(src);
		free(dst);
		dprintf(STDERR_FILENO, "%s: Could not open file: %s\n", app_name, file_name_new);
		return (LZ4K_STATUS_NOT_OPEN);
	}
	crc32_src = crc32_zlib_stream(_crc32_table, &lz4k.magic, (sizeof(lz4k) - sizeof(lz4k.crc32_zlib)), 0xFFFFFFFF);
	lz4k.crc32_zlib = crc32_zlib_stream(_crc32_table, dst, (sizeof(lz4k) - sizeof(lz4k.crc32_zlib)), crc32_src) ^ 0xFFFFFFFF;
	if (write(fd, &lz4k, sizeof(lz4k)) == -1)
	{
		close(fd);
		free(src);
		free(dst);
		dprintf(STDERR_FILENO, "%s: Could not write the file: %s\n", app_name, file_name_new);
		return (LZ4K_STATUS_NOT_WRITE);
	}
	if (write(fd, dst, compressed_size) == -1)
	{
		close(fd);
		free(src);
		free(dst);
		dprintf(STDERR_FILENO, "%s: Could not write the file: %s\n", app_name, file_name_new);
		return (LZ4K_STATUS_NOT_WRITE);
	}
	close(fd);
	free(src);
	free(dst);
	return (LZ4K_STATUS_OK);
}

int	main(int argc, char **argv)
{
	char						*app_name;
	char						*tmp;

	app_name = argv[0x0];
	if (argc < 2)
		return (_usage(app_name));
	tmp = argv[0x1];
	if (strlen(tmp) != 0x2 || tmp[0x0] != '-')
		return (_usage(app_name));
	switch (toupper(tmp[0x1]))
	{
		case 'C':
			return (_compess(app_name, argc, argv));
			break ;
		case 'D':
			return (_decompess(app_name, argc, argv));
			break ;
		default:
			
			break ;
	}
	return (_usage(app_name));
}