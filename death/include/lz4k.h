#ifndef LZ4K_H
#define LZ4K_H

typedef struct						lz4k_s
{
	uint32_t						crc32_zlib;
	uint32_t						magic;
	uint64_t						compressed_size;
	uint64_t						uncompressed_size;
	uint32_t						block_size;
	uint8_t							version;
	uint8_t							mode;
	uint8_t							reserved[0x2];
}									lz4k_t;

#define LZ4K_MIN_COMPRESS			0x4

#define LZ4K_MODE_SOLED				0x1
#define LZ4K_MODE_BLOCK				0x2

#endif