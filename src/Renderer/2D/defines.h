// to 0xAABBGGRR / RR GG BB AA in memory (LE)
#define RGBA_TO_U32_ABGR(r,g,b,a) ((uint8_t(r)) | (uint8_t(g)<<8) | (uint8_t(b)<<16) | (uint8_t(a)<<24))
// to 0xAARRGGBB / BB GG RR AA in memory (LE)
#define RGBA_TO_U32_ARGB(r,g,b,a) ((uint8_t(b)) | (uint8_t(g)<<8) | (uint8_t(r)<<16) | (uint8_t(a)<<24))


/* 
#define RGBA_TO_U32 <insert> 
to determine what order the rgba values should be written in the buffer
defaulted to  0xAARRGGBB (BGRA in memory in little endian) if not defined 
*/
#define RGBA_TO_U32(r,g,b,a)    RGBA_TO_U32_ABGR(r,g,b,a) 


#define RGB_TO_U32(r,g,b)       RGBA_TO_U32(r,g,b,0xff)
#define nRGB_TO_U32(r,g,b)      RGB_TO_U32(uint8_t((r)*255),uint8_t((g)*255),uint8_t((b)*255))
#define nRGBA_TO_U32(r,g,b,a)   RGBA_TO_U32(uint8_t((r)*255),uint8_t((g)*255),uint8_t((b)*255),uint8_t((a)*255))

