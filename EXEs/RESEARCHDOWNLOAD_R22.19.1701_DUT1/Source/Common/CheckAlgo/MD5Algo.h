/******************************************************************************
*  @Copyright:     (C) 2019 UNISOC Technologies Co.,Ltd. All Rights Reserved.
*  @Filename:      MD5Algo.h
*  @Author:        MES Team
*  @Date:          2019/6/3
*  @Description:
*****************************************************************************/
#pragma once
/************************************************************************/
/*                                                        MD5													             */
/************************************************************************/
#define _MD5_H_
#define R_memset(x, y, z) memset(x, y, z)
#define R_memcpy(x, y, z) memcpy(x, y, z)
#define R_memcmp(x, y, z) memcmp(x, y, z)

typedef unsigned long UINT4;
typedef unsigned char *POINTER;

typedef struct 
{
	UINT4 state[4];  
	UINT4 count[2];
	unsigned char buffer[64];  
} MD5_CTX;

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) {(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac);(a) = ROTATE_LEFT ((a), (s));(a) += (b);}
#define GG(a, b, c, d, x, s, ac) {(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac);(a) = ROTATE_LEFT ((a), (s));(a) += (b);}
#define HH(a, b, c, d, x, s, ac) {(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac);(a) = ROTATE_LEFT ((a), (s));(a) += (b);}
#define II(a, b, c, d, x, s, ac) {(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac);(a) = ROTATE_LEFT ((a), (s));(a) += (b);}

static unsigned char PADDING[64] = 
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
class CMD5Algo
{
public:
	CMD5Algo(void);
	virtual ~CMD5Algo(void);

	void MD5_Init(MD5_CTX*);
	void MD5_Update(MD5_CTX *, unsigned char *, unsigned int);
	void MD5_Final(unsigned char [16], MD5_CTX *);
	void MD5_Transform(UINT4 [4], unsigned char [64]);
	void Encode(unsigned char *, UINT4 *, unsigned int);
	void Decode(UINT4 *, unsigned char *, unsigned int);
};

