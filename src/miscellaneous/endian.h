#ifndef ENDIAN_H
#define ENDIAN_H

/*
    http://stackoverflow.com/questions/7184789/does-bit-shift-depend-on-endianness
    http://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func

    0xFFAA
    0xFFAA - Big Endian
    0xAAFF - Little Endian

    0x3f2b
    0x3f2b - Big Endian
    0x2b3f - Little Endian
*/

#include <stdint.h>

/*
int16_t endian_reveal = 1;
#define endian_isbig endian_
#define endian_islittle endian_reveal & 0x00ff;
*/

/* reverse byte order regardless of the machine's endianess */
#define endian16(word) ((word>>8) | (word<<8))
#define endian32(dword) (((dword>>24)&0xff)    |   \
                         ((dword<<8)&0xff0000) |   \
                         ((dword>>8)&0xff00)   |   \
                         ((dword<<24)&0xff000000)) \

/* reverse byte order if on big endian machine */
#define biglittle2(word) \
(((int16_t) *(((int8_t *) &word) + 0)) | \
(((int16_t) *(((int8_t *) &word) + 1)) << 8))
#define biglittle4(dword) \
(((int32_t) *(((int8_t *) &dword) + 0)) |         \
(((int32_t) *(((int8_t *) &dword) + 1)) <<  8)  | \
(((int32_t) *(((int8_t *) &dword) + 2)) << 16)  | \
(((int32_t) *(((int8_t *) &dword) + 3)) << 24))   \

/* reverse byte order if on little endian machine */
#define littlebig2(word) \
(((int16_t) *(((int8_t *) &word) + 1)) | \
(((int16_t) *(((int8_t *) &word) + 0)) << 8))
#define littlebig4(dword) \
(((int32_t) *(((int8_t *) &dword) + 3)) |         \
(((int32_t) *(((int8_t *) &dword) + 2)) <<  8)  | \
(((int32_t) *(((int8_t *) &dword) + 1)) << 16)  | \
(((int32_t) *(((int8_t *) &dword) + 0)) << 24))   \

#endif
