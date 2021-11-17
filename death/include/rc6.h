/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc6.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <amatilda@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 08:34:38 by amatilda          #+#    #+#             */
/*   Updated: 2021/06/06 22:52:44 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RC6_H
# define RC6_H

# define RC6_P16			0xB7E1
# define RC6_Q16			0x9E37
# define RC6_P32			0xB7E15163
# define RC6_Q32			0x9E3779B9
# define RC6_P64			0xB7E151628AED2A6B
# define RC6_Q64			0x9E3779B97F4A7C15

# define RC6_W				(0x20)
# define RC6_KEY_MAX		(0x100 / 0x8)
# define RC6_R				(0x14)
# define RC6_B				(0x10)
# define RC6_T				(0x2 * RC6_R + 0x4)

#if RC6_W == 0x10
	# define RC6_WORD		uint16_t
	# define RC6_P			RC6_P16
	# define RC6_Q			RC6_Q16
	# define RC6_LG			0x4
	# define ROTL(x,y)		__rolw(x,y)
	# define ROTR(x,y)		__rorw(x,y)
#elif RC6_W == 0x20
	# define RC6_WORD		uint32_t
	# define RC6_P			RC6_P32
	# define RC6_Q			RC6_Q32
	# define RC6_LG			0x5
	# define ROTL(x,y)		__rold(x,y)
	# define ROTR(x,y)		__rord(x,y)
#elif RC6_W == 0x40
	# define RC6_WORD		uint64_t
	# define RC6_P			RC6_P64
	# define RC6_Q			RC6_Q64
	# define RC6_LG			0x6
	# define ROTL(x,y)		__rolq(x,y)
	# define ROTR(x,y)		__rorq(x,y)
#else
	#error RC6_W
#endif

void rc6_init(const void *key, RC6_WORD *S, size_t len);
void rc6_encrypt(RC6_WORD *S, const RC6_WORD *in, RC6_WORD *out, size_t num_block);
void rc6_decrypt(RC6_WORD *S, const RC6_WORD *in, RC6_WORD *out, size_t num_block);

#endif