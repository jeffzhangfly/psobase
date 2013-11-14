//
//  zrandom.c
//  psobase
//
// Copyright (c) 2011, Jianhua Zhang
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this
// list of conditions and the following disclaimer in the documentation and/or other
// materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

#include "zrandom.h"

// 产生num个[0,1]之间的double随机数，保存在result中
// result内存空间需要在调用该函数前分配
void rand_multi_double(int num, double* result)
{
	int i = 0;
	unsigned int seed = get_seed();
    
	srand(seed);
	for (i=0;i<num;i++)
	{
		result[i]   =   rand()   /   (double)(RAND_MAX);
		//////////////////////////////////////////////////////////
		// DEBUG
		//printf("random: %f\n", result[i]);
		// End of DEBUG
		//////////////////////////////////////////////////////////
	}
}

// 产生一个[0,1]的double随机数
double rand_double(void)
{
	unsigned int seed = get_seed();
    
	srand(seed);
	return rand()   /   (double)(RAND_MAX);
}

// 产生num个[min,max]的int随机数，保存在result中
// result内存空间需要在调用该函数前分配
void rand_multi_int(int num, int* result, int min, int max)
{
	int i = 0;
	int range = max - min;
	unsigned int seed = get_seed();
	int temp = 0;
    
	srand(seed);
	for (i=0;i<num;i++) {
		temp  =   rand()   %   range;
		result[i] = temp + min;
	}
}

// 产生一个[min,max]的int随机数
int rand_int( int min, int max)
{
	int range = max - min;
	unsigned int seed = get_seed();
	int temp = 0;
    
	srand(seed);
	temp  =   rand()   %   range;
	return temp + min;
}

// 获得随机数种子，一个纳秒级的无符号整型值
unsigned int get_seed(void)
{
#ifdef _MAC
    unsigned int val;
    __asm__ volatile("rdtsc" : "=A" (val));
    return val;
#elif defined _LINUX
    __asm__ ("RDTSC");
#elif defined _WIN32
    unsigned __int64 val;
    val = __rdtsc();
	return val;
#endif
}

