//
//  pso.c
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

#include "pso.h"
#include "zrandom.h"

// Modified 15/11/2011, Zhang Jianhua. Delete these global var
// 全局变量
//double c1 = 0.9;
//double c2 = 1.2;

// 开始pso，设定c1和c2参数
// c1调节粒子飞向自身最好位置方向的步长，c2调节粒子向全局最好位置飞行的步长
// c1和c2通常在0~2之间取值

// Modified 15/11/2011, Zhang Jianhua. Delete these global var
// 此函数现在只是占位，不做任何操作，可以不调用
void begin_pso()
{
    
}

///////////////////////////////////////////////////////////////////////
// 粒子群算法主函数
// 参数：
//		in_para: 算法参数
//		result: 计算结果，是一个指针，此函数计算完成后为result参数
//					具体分配内存空间（以及best_particle域）这部分内存需要
//					pso函数的调用者free（包括best_particle域）
//	返回值: 进化迭代的代数
///////////////////////////////////////////////////////////////////////
int pso(IN_PARA_PSO in_para, OUT_PARA_PSO** result)
{
	OUT_PARA_PSO * rst;	// 保存计算结果
    
	double * popular;	// 种群粒子位置
	double * speed;		//种群例子速度
	double * current_fitness;	//size为in_para.pop_number（种群规模）
	// 保存各粒子当前适应值，对应popular中各粒子向量
	double * pbest_pos;	//size为in_para.pop_number * in_para.dimense（种群规模*维数）
	// 保存各个粒子各自的历史最优位置
	double * pbest;		// size为in_para.pop_number（种群规模）
	// 保存各粒子历史最优适应值，对应pbest_pos中各粒子向量
	double * gbest_pos;	// size为in_para.dimense（维数），保存种群的（全局）历史最优位置
	double gbest;		// 种群（全局）历史最优适应值
    
	double * array4fitness;	// 计算适应值时用到的向量，即该向量起始地址
	// 与维数配合使用即可获得该项量的值
    
	int current_gen = 0;
	int no_improve = 0;
	double mutation_range = in_para.val_max - in_para.val_min;
	double current_weight = 0;
	double r1 = 0;
	double r2 = 0;
    
	int i = 0;
	int j = 0;
    
	int best_index = 0;
	double temp = 0;
    
	// 初始化 pbest_pos
	current_fitness = (double *)malloc(in_para.pop_number * sizeof(double));
	// 初始化 pbest_pos
	pbest_pos = (double *)malloc(in_para.dimense * in_para.pop_number * sizeof(double));
	// 初始化 pbest
	pbest = (double *)malloc(in_para.pop_number * sizeof(double));
	// 初始化 gbest_pos
	gbest_pos = (double *)malloc(in_para.dimense * sizeof(double));
    
	// 初始化种群各粒子位置
	popular = init_popular(in_para.dimense, in_para.pop_number, in_para.val_max, in_para.val_min);
	// 初始化种群各粒子速度为0
	speed = init_speed(1, in_para.dimense, in_para.pop_number, in_para.spd_max);
    
	// DEBUG
	// print_popular(in_para.dimense, in_para.pop_number, popular,speed);
    
	// 因为是第一次迭代，将初始种群粒子位置附给pbest_pos
	for (i=0;i<in_para.pop_number;i++)
		for (j=0;j<in_para.dimense;j++)
			*(pbest_pos + i * in_para.dimense + j) = *(popular + i * in_para.dimense + j);
    
	// 计算初始种群各粒子适应值，同时将该值赋给pbest
	for (i = 0;i<in_para.pop_number;i++) {
		array4fitness = popular + i * in_para.dimense;
		current_fitness[i] = in_para.fitness_fun(in_para.dimense, array4fitness, in_para.fitness_data);
		pbest[i] = current_fitness[i];
	}
    
	// DEBUG
	// print_fitness(in_para.pop_number, current_fitness);
    
	// 第一次迭代，首先置gbest为一个很大的值
	gbest = DOUBLE_MAX;
    
	// 保留历史最优值，并记录下这个历史最优位置在current_fitness中的位置
	for (i=0;i<in_para.dimense;i++) {
		if (pbest[i] < gbest) {
			gbest = pbest[i];
			best_index = i;
		}
	}
	// 保留历史最优位置
	for (i=0;i<in_para.dimense;i++)
		gbest_pos[i] = *(pbest_pos + best_index * in_para.dimense + i);
    
	// 进化迭代
	for (current_gen = 1; current_gen < in_para.max_gen;current_gen++) {
		// DEBUG
		// printf("GBEST:%f\t",gbest);
        
		// 计算当前代惯性权重
		current_weight = get_wight(current_gen,in_para.max_gen);
		// 两个U(0,1)随机数
		r1 = rand_double();
		r2 = rand_double();
		for (i=0;i<in_para.pop_number;i++)	 // 对每一个粒子
			for (j=0;j<in_para.dimense;j++) {  //对每一个粒子的每一维
				// 计算下一时刻速度v(t+1)
				temp = current_weight * (*(speed + i * in_para.dimense + j))
                + in_para.c1*r1*( (*(pbest_pos + i * in_para.dimense + j)) - (*(popular + i * in_para.dimense + j)) )
                + in_para.c2*r2*(gbest_pos[j] -  (*(popular + i * in_para.dimense + j)) );
                
				// 计算下一时刻位置x(t+1)
				if (temp > in_para.spd_max)
					temp = in_para.spd_max;
				if (temp < (-1 * in_para.spd_max))
					temp = -1 * in_para.spd_max;
				// 记录v(t+1)
				*(speed + i * in_para.dimense + j) = temp;
				*(popular + i * in_para.dimense + j) = (*(popular + i * in_para.dimense + j)) + temp;
				if (*(popular + i * in_para.dimense + j) > in_para.val_max)
					*(popular + i * in_para.dimense + j) = in_para.val_max;
				if (*(popular + i * in_para.dimense + j) < in_para.val_min)
					*(popular + i * in_para.dimense + j) = in_para.val_min;
                
				// DEBUG
				// printf("Speed:%f\t",temp);
				// printf("Position:%f\n",*(popular + i * in_para.dimense + j));
			}
		// 计算各粒子适应值
		for (i = 0;i<in_para.pop_number;i++) {
			array4fitness = popular + i * in_para.dimense;
			current_fitness[i] = in_para.fitness_fun(in_para.dimense, array4fitness, in_para.fitness_data);
		}
		// 设置pbest_pos及pbest
		for (i=0;i<in_para.pop_number;i++) {
			// 为每个粒子，计算其历史最优值
			if (current_fitness[i] < pbest[i]) {
				// 赋值粒子的历史最优值
				pbest[i] = current_fitness[i];
				// 复制该粒子的历史最优位置
				for (j=0;j<in_para.dimense;j++)
					*(pbest_pos + i * in_para.dimense +j)= *(popular + i * in_para.dimense +j);
			}
		}
        
		// 设置gbest_pos及gbest
		// 保留历史最优值，并记录下这个历史最优位置在current_fitness中的位置
		for (i=0,best_index = -1;i<in_para.pop_number;i++) {
			if (pbest[i] < gbest) {
				gbest = pbest[i];
				best_index = i;
			}
		}
		// 保留历史最优位置
		if (best_index != -1) {	   // gbest改变了
			for (i=0;i<in_para.dimense;i++)
				gbest_pos[i] = *(pbest_pos + best_index * in_para.dimense + i);
			no_improve = 0;	// 重置最大无进化代数
		} else
			no_improve++;	// 最大无进化代数加一
        
		if (in_para.use_break_condition) { // 使用结束条件
			if (gbest < in_para.break_condition)
				break;
		}
        
		// 如果最大无进化代数达到，变异/退出
		if (no_improve > in_para.max_no_improve) {
			if (in_para.max_no_improve_stop)
				break;
			else {
				// DEBUG
				// printf("MUTATE!\n");
				// 计算变异范围，其值为取值范围的1/10
				for (i=0;i<in_para.pop_number;i++)
					for (j=0;j<in_para.dimense;j++) {
						temp = rand_double();
						temp = temp * (mutation_range + mutation_range) - mutation_range;
						*(popular + i*in_para.dimense + j) = *(popular + i*in_para.dimense + j) + temp;
					}
			}
		}
	}
    
	// 为计算结果分配内存
	rst = (OUT_PARA_PSO *)malloc(sizeof(OUT_PARA_PSO));
	rst->dimense = in_para.dimense;
	rst->best_fitness = gbest;
	// 为最优值位置分配内存
	rst->best_particle = (double*)malloc(in_para.dimense * sizeof(double));
	// 赋值最优值位置
	for (i=0;i<in_para.dimense;i++)
		rst->best_particle[i] = gbest_pos[i];
	// 赋值out参数以返回计算结果
	*result = rst;
    
	// 释放动态分配的内存空间
	free(popular);
	free(speed);
	free(current_fitness);
	free(pbest_pos);
	free(pbest);
	free(gbest_pos);
    
	return current_gen;
}

///////////////////////////////////////////////////////////////////////
// 初始化种群
// 参数：number
//		dimense: 问题维数
//		pop_number: 种群规模
//		val_max: 各维取值的最大值
//		val_min:  各维取值的最小值
//	返回值: 一个二维double数组，pop_number行，dimense列，每一行对应一个粒子；
//				这部分内存动态分配，需要调用者（pso主函数）使用完毕后释放.
//				这个二维数组在主函数执行过程中将用于存放各粒子
///////////////////////////////////////////////////////////////////////
double* init_popular(int dimense, int pop_number, double val_max, double val_min)
{
	double * p = NULL;
	int i = 0;
	int j = 0;
    
	p = (double *)malloc(dimense * pop_number * sizeof(double));	//为二维数组分配内存空间
	rand_multi_double( dimense * pop_number, p);
	for (i=0;i<pop_number;i++)
		for (j=0;j<dimense;j++)
			*(p + i * dimense + j) = (*(p + i * dimense + j)) * (val_max - val_min) + val_min;
    
	return p;
}

///////////////////////////////////////////////////////////////////////
// 初始化种群
// 参数：number
//		dimense: 问题维数
//		pop_number: 种群规模
//		spd_max: 速度最大值
//	返回值: 一个二维double数组，pop_number行，dimense列，每一行对应一个粒子；
//				这部分内存动态分配，需要调用者（pso主函数）使用完毕后释放.
//				这个二维数组在主函数执行过程中将用于存放各粒子各维速度
///////////////////////////////////////////////////////////////////////
double* init_speed(int is_zero, int dimense, int pop_number, double spd_max)
{
	double spd_min = -1 * spd_max;
	double * p = NULL;
	int i = 0;
	int j = 0;
    
	p = (double *)malloc(dimense * pop_number * sizeof(double));	//为二维数组分配内存空间
	rand_multi_double( dimense * pop_number, p);
	for (i=0;i<pop_number;i++)
		for (j=0;j<dimense;j++) {
			if (is_zero)
				*(p + i * dimense + j) = 0;
			else
				*(p + i * dimense + j) = (*(p + i * dimense + j)) * (spd_max - spd_min) + spd_min;
		}
    
	return p;
}

// 输入数组size和数组，返回数组中最小的值
double get_min(int num, double * arrays)
{
	double min = arrays[0];
	int i = 0;
	for (;i<num;i++) {
		if (arrays[i] < min)
			min = arrays[i];
	}
	return min;
}

// 计算惯性权重
// current_gen: 当前代数
// max_gen: 最大进化代数
double get_wight(int current_gen,int max_gen)
{
	double temp = 0.9 - ((double)current_gen / (double)max_gen) * 0.5;
	return temp;
}

// 释放计算结果的堆内存，每次调用完pso函数必须调用这个函数
void end_pso(OUT_PARA_PSO * rst)
{
	free(rst->best_particle);
	free(rst);
}

//////////////////////////////////////////////////////////////////////////
// DEBUG FUNCTIONS
// 控制台输出popular各粒子位置及各维速度，仅用于DEBUG
void print_popular(int dimense, int pop_number, double * popular, double * speed)
{
	int i = 0;
	int j = 0;
	printf("##############################################\n");
	printf("BEGIN OF PRINT POPULAR\n");
	for (i=0;i<pop_number;i++) {
		printf("\nParticle NO: (%d)\n",i);
		printf("Particle Position:  ");
		for (j=0;j<dimense;j++)
			printf("%f  ",*(popular + i*dimense + j));
		printf("\nParticle Speed:  ");
		for (j=0;j<dimense;j++)
			printf("%f  ",*(speed + i*dimense + j));
		printf("\n");
	}
	printf("\nEND OF PRINT POPULAR\n");
	printf("##############################################\n");
}

// 控制台输出popular各粒子适应值，仅用于DEBUG
void print_fitness(int pop_number, double * fitness)
{
	int i = 0;
	printf("##############################################\n");
	printf("BEGIN OF PRINT FITNESS\n");
	for (i=0;i<pop_number;i++) {
		printf("\nParticle NO: (%d)\t ",i);
		printf("Particle Fitness:  %f\n",fitness[i]);
	}
	printf("\nEND OF PRINT FITNESS\n");
	printf("##############################################\n");
}
// END OF DEBUG FUNCTIONS
//////////////////////////////////////////////////////////////////////////
