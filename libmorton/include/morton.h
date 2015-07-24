// Libmorton - fast methods to encode/decode morton codes
#ifndef MORTON_H_
#define MORTON_H_

#include <stdint.h>
#include "morton_lookup_tables.h"

using namespace std;

// encode a given (x,y,z) coordinate to a 64-bit morton code
inline uint64_t mortonEncode(uint32_t x, uint32_t y, uint32_t z);
// decode a given morton code into X,Y,Z coordinates
inline uint64_t mortonDecode(uint64_t morton, uint32_t& x, uint32_t& y, uint32_t& z);
inline uint32_t mortonDecode_X(uint64_t morton);
inline uint32_t mortonDecode_Y(uint64_t morton);
inline uint32_t mortonDecode_Z(uint64_t morton);

// encoding with lookup table
inline uint64_t mortonEncode_LUT(const uint32_t x, const uint32_t y, const uint32_t z){
	uint64_t answer = 0;
	answer = encode_morton256_z[(z >> 16) & 0xFF ] |
			 encode_morton256_y[(y >> 16) & 0xFF] |
			 encode_morton256_x[(x >> 16) & 0xFF];
	answer = answer << 48 |
			 encode_morton256_z[(z >> 8) & 0xFF] |
			 encode_morton256_y[(y >> 8) & 0xFF] |
			 encode_morton256_x[(x >> 8) & 0xFF];
	answer = answer << 24 |
			 encode_morton256_z[(z)& 0xFF] |
			 encode_morton256_y[(y)& 0xFF] |
			 encode_morton256_x[(x)& 0xFF];
	return answer;
}

inline void mortonDecode_LUT(const uint64_t morton, uint32_t& x, uint32_t& y, uint32_t& z){
	x = 0;
	y = 0;
	z = 0;
	x = x | decode_morton512_x[morton & 0x1ff] 
		| (decode_morton512_x[((morton >> 9) & 0x1ff)] << 3)
		| (decode_morton512_x[((morton >> 18) & 0x1ff)] << 6)
		| (decode_morton512_x[((morton >> 27) & 0x1ff)] << 9)
		| (decode_morton512_x[((morton >> 36) & 0x1ff)] << 12)
		| (decode_morton512_x[((morton >> 46) & 0x1ff)] << 15)
		| (decode_morton512_x[((morton >> 54) & 0x1ff)] << 18);
	y = y | decode_morton512_y[morton & 0x1ff]
		| (decode_morton512_y[((morton >> 9) & 0x1ff)] << 3)
		| (decode_morton512_y[((morton >> 18) & 0x1ff)] << 6)
		| (decode_morton512_y[((morton >> 27) & 0x1ff)] << 9)
		| (decode_morton512_y[((morton >> 36) & 0x1ff)] << 12)
		| (decode_morton512_y[((morton >> 46) & 0x1ff)] << 15)
		| (decode_morton512_y[((morton >> 54) & 0x1ff)] << 18);
	z = z | decode_morton512_z[morton & 0x1ff]
		| (decode_morton512_z[((morton >> 9) & 0x1ff)] << 3)
		| (decode_morton512_z[((morton >> 18) & 0x1ff)] << 6)
		| (decode_morton512_z[((morton >> 27) & 0x1ff)] << 9)
		| (decode_morton512_z[((morton >> 36) & 0x1ff)] << 12)
		| (decode_morton512_z[((morton >> 46) & 0x1ff)] << 15)
		| (decode_morton512_z[((morton >> 54) & 0x1ff)] << 18);

	// can probably make this faster by using lzc intrinsic! 

	// For Microsoft compilers use _BitScanForward & _BitScanReverse.
	// For GCC use __builtin_ffs, __builtin_clz, __builtin_ctz.

	//uint64_t part = morton & 0x1ff;
	//x = x | decode_morton512_x[part];
	//y = y | decode_morton512_y[part];
	//z = z | decode_morton512_z[part];
	//part = (morton >> 9) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 3);
	//y = y | (decode_morton512_y[part] << 3);
	//z = z | (decode_morton512_z[part] << 3);
	//part = (morton >> 18) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 6);
	//y = y | (decode_morton512_y[part] << 6);
	//z = z | (decode_morton512_z[part] << 6);
	//part = (morton >> 27) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 9);
	//y = y | (decode_morton512_y[part] << 9);
	//z = z | (decode_morton512_z[part] << 9);
	//part = (morton >> 27) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 9);
	//y = y | (decode_morton512_y[part] << 9);
	//z = z | (decode_morton512_z[part] << 9);
	//part = (morton >> 36) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 12);
	//y = y | (decode_morton512_y[part] << 12);
	//z = z | (decode_morton512_z[part] << 12);
	//part = (morton >> 46) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 15);
	//y = y | (decode_morton512_y[part] << 15);
	//z = z | (decode_morton512_z[part] << 15);
	//part = (morton >> 46) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 15);
	//y = y | (decode_morton512_y[part] << 15);
	//z = z | (decode_morton512_z[part] << 15);
	//part = (morton >> 54) & 0x1ff;
	//x = x | (decode_morton512_x[part] << 18);
	//y = y | (decode_morton512_y[part] << 18);
	//z = z | (decode_morton512_z[part] << 18);
}

// define default methods
inline uint64_t mortonEncode(const uint32_t x, const uint32_t y, const uint32_t z){
	return mortonEncode_LUT(x, y, z);
}

inline uint64_t mortonDecode(const uint64_t morton, uint32_t& x, uint32_t& y, uint32_t& z){
	mortonDecode_LUT(morton, x, y, z);
}

#endif // MORTON_H_