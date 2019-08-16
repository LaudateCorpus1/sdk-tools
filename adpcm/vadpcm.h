#ifndef VADPCM_H
#define VADPCM_H

#include <stdio.h>

typedef signed char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#ifdef __sgi
#  define BSWAP16(x)
#  define BSWAP32(x)
#else
#  define BSWAP16(x) x = __builtin_bswap16(x);
#  define BSWAP32(x) x = __builtin_bswap32(x);
#endif

typedef struct {
    u32 ckID;
    u32 ckSize;
} ChunkHeader;

typedef struct {
    u32 ckID;
    u32 ckSize;
    u32 formType;
} Chunk;

typedef struct {
    s16 numChannels;
    u16 numFramesH;
    u16 numFramesL;
    s16 sampleSize;
    s16 sampleRate[5]; // 80-bit float
    u16 compressionTypeH;
    u16 compressionTypeL;
} CommonChunk;

typedef struct {
    s16 MarkerID;
    u16 positionH;
    u16 positionL;
} Marker;

typedef struct {
    s16 playMode;
    s16 beginLoop;
    s16 endLoop;
} Loop;

typedef struct {
    s8 baseNote;
    s8 detune;
    s8 lowNote;
    s8 highNote;
    s8 lowVelocity;
    s8 highVelocity;
    s16 gain;
    Loop sustainLoop;
    Loop releaseLoop;
} InstrumentChunk;

typedef struct {
    s32 offset;
    s32 blockSize;
} SoundDataChunk;

typedef struct {
    s16 version;
    s16 order;
    s16 nEntries;
} CodeChunk;

typedef struct
{
    u32 start;
    u32 end;
    u32 count;
    s16 state[16];
} ALADPCMloop;

// vpredictor.c
s32 readcodebook(FILE *fhandle, s32 ****table, s32 *order, s32 *npredictors);
s32 readaifccodebook(FILE *fhandle, s32 ****table, s16 *order, s16 *npredictors);
s32 inner_product(s32 length, s32 *v1, s32 *v2);

// quant.c

// vdecode.c
void vdecodeframe(FILE *ifile, s32 *outp, s32 order, s32 ***coefTable);

// vencode.c

// util.c
u32 readbits(u32 nbits, FILE *ifile);
char *ReadPString(FILE *ifile);
s32 lookupMarker(s32 *sample, s16 loopPoint, Marker *markers, s32 nmarkers);
ALADPCMloop *readlooppoints(FILE *ifile, s16 *nloops);

// sampleio.c
void writeout(FILE *outfd, s32 size, s32 *l_out, s32 *r_out, s32 chans);

#endif
