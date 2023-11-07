// color order used in buffer by the renderer
// #define er_U32_ABGR
#define er_U32_ARGB



// to 0xAABBGGRR / RR GG BB AA in memory (LE)
#define RGBA_TO_U32_ABGR(r,g,b,a) ((uint8_t(r)) | (uint8_t(g)<<8) | (uint8_t(b)<<16) | (uint8_t(a)<<24))
// to 0xAARRGGBB / BB GG RR AA in memory (LE)
#define RGBA_TO_U32_ARGB(r,g,b,a) ((uint8_t(b)) | (uint8_t(g)<<8) | (uint8_t(r)<<16) | (uint8_t(a)<<24))
// to 0xRRGGBBAA / AA BB GG RR in memory (LE)
#define RGBA_TO_U32_RGBA(r,g,b,a) ((uint8_t(a)) | (uint8_t(b)<<8) | (uint8_t(g)<<16) | (uint8_t(r)<<24))


#define VEC4(r,g,b,a)           (r),(g),(b),(a)
#define NORMALIZE_U8(u8)        ((float)(u8)/0xff)
#define U32_ABGR_TO_VEC4(u32)   VEC4(NORMALIZE_U8((u32) & 0xff), \
                                    NORMALIZE_U8(((u32)>>8) & 0xff), \
                                    NORMALIZE_U8(((u32)>>16) & 0xff), \
                                    NORMALIZE_U8(((u32)>>24) & 0xff))

#define U32_ARGB_TO_VEC4(u32)   VEC4(NORMALIZE_U8(((u32)>>16) & 0xff), \
                                    NORMALIZE_U8(((u32)>>8) & 0xff), \
                                    NORMALIZE_U8(((u32)) & 0xff), \
                                    NORMALIZE_U8(((u32)>>24) & 0xff))

#define U32_RGBA_TO_VEC4(u32)   VEC4(NORMALIZE_U8(((u32)>>24) & 0xff), \
                                    NORMALIZE_U8(((u32)>>16) & 0xff), \
                                    NORMALIZE_U8(((u32)>>8) & 0xff), \
                                    NORMALIZE_U8(((u32)) & 0xff))


/* 
#define RGBA_TO_U32 <insert> 
to determine what order the rgba values should be written in the buffer
defaulted to  0xAARRGGBB (BGRA in memory in little endian) if not defined 
*/

#ifdef er_U32_ABGR
#define RGBA_TO_U32(r,g,b,a)    RGBA_TO_U32_ABGR(r,g,b,a) 
#else 
#define RGBA_TO_U32(r,g,b,a)    RGBA_TO_U32_ARGB(r,g,b,a) 
#endif

#define RGB_TO_U32(r,g,b)       RGBA_TO_U32(r,g,b,0xff)
#define nRGB_TO_U32(r,g,b)      RGB_TO_U32(uint8_t((r)*255),uint8_t((g)*255),uint8_t((b)*255))
#define nRGBA_TO_U32(r,g,b,a)   RGBA_TO_U32(uint8_t((r)*255),uint8_t((g)*255),uint8_t((b)*255),uint8_t((a)*255))

#ifdef er_U32_ABGR
#define U32_TO_VEC4(u32)        U32_ABGR_TO_VEC4(u32)
#else 
#define U32_TO_VEC4(u32)        U32_ARGB_TO_VEC4(u32)
#endif