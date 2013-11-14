//
//  pso.h
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

#ifndef psobase_pso_h
#define psobase_pso_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "defines.h"

// 适应值函数指针类型
// 参数：问题维数和粒子向量
typedef double (*FITNESS_FUN)(int dimense, double * fitness_fun, void * fitness_data);

typedef struct in_para_pso {
	// para c1
	double c1;
	// para c2
	double c2;
	//	dimense: 问题维数
	int dimense;
	//	pop_number: 种群规模
	int pop_number;
	//	max_gen: 最大进化代数
	int max_gen;
	//	max_no_improve: 最大无进化代数
	int max_no_improve;
	// max_no_improve_stop: 最大无进化代数达到后是否进化停止
	int max_no_improve_stop;	//1: 达到后停止；0: 达到后变异种群
	//	val_max: 各维取值的最大值. TODO:如果希望各维取值范围不同, 将此项改为double数组并修改相应条件判断代码
	double val_max;
	//	val_min:  各维取值的最小值. TODO:如果希望各维取值范围不同, 将此项改为double数组并修改相应条件判断代码
	double val_min;
	//	spd_max: 最大速度. TODO:如果希望各维最大速度不同, 将此项改为double数组并修改相应条件判断代码
	double spd_max;
	// fitness_fun: 适应值函数
	FITNESS_FUN fitness_fun;
	// 适应值函数所需用到的数据
	void * fitness_data;
	// 是否使用结束条件
	int use_break_condition;
	// 结束条件，fitness小于这个值阶数进化迭代
	double break_condition;
}IN_PARA_PSO;

typedef struct out_para_pso {
	// 问题维数，主要用于确定best_position数组的大小
	int dimense;
	// 最优适应值
	double best_fitness;
	// 最优适应值对应的粒子
	double * best_particle;
}OUT_PARA_PSO;

EXPORT void begin_pso();

EXPORT int pso(IN_PARA_PSO in_para, OUT_PARA_PSO** result);

EXPORT void end_pso(OUT_PARA_PSO * rst);

#ifdef DEBUG
EXPORT
#endif
double* init_popular(int dimense, int pop_number, double val_max, double val_min);

#ifdef DEBUG
EXPORT
#endif
double* init_speed(int is_zero, int dimense, int pop_number, double spd_max);

#ifdef DEBUG
EXPORT
#endif
double get_min(int num, double * arrays);

#ifdef DEBUG
EXPORT
#endif
double get_wight(int current_gen,int max_gen);

////////////////////////////////////////////////////////////////////////
// DEBUG
void print_popular(int dimense, int pop_number, double * popular, double * speed);
void print_fitness(int pop_number, double * fitness);

#endif
