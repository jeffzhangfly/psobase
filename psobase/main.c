//
//  main.c
//  psobase
//
// Copyright (c) 2013, Jianhua Zhang
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

#include <stdio.h>
#include <math.h>
#include "pso.h"


/////////////////////////////////////////////////////////////////
// 说明:
//		适应值计算自定义函数, 此处计算(Latex数学公式):
//          \sum_{i=1}^(dimense){x_{i}^2}
//      换为具体优化问题的适应值函数即可
// 参数:
//		dimense: 问题维数
//		vals: 向量各个分量
//		data: 无, 计算适应值仅需要向量, 不需要外部数据
//	返回值: \sum_{i=1}^(dimense){x_{i}^2}
/////////////////////////////////////////////////////////////////
double fitness_test(int dimense, double * vals, void * data)
{
    double sum = 0;
    for (int i=0; i<dimense; i++) {
        // (*(vals + i))即vals的第i个分量
        sum += (*(vals + i)) * (*(vals + i));
    }
	
	return sum;
}

/////////////////////////////////////////////////////////////////
// 说明:
//		适应值计算自定义函数, 此处计算
//          levyno5 (最优: x=(－1.3068,－1.4248)，f(x)=－176.1375)
//      主要说明对于多峰的复杂优化问题, 标准PSO陷入局部最优,
//      但对于工程上大部分情况只需要满意解即可, 所以也是有意义的.
//      需要精度更高, 更准确的最优解, 可以针对问题改进算法或使用其他随机优化算法
// 参数:
//		dimense: 问题维数
//		vals: 向量各个分量
//		data: 无, 计算适应值仅需要向量, 不需要外部数据
//	返回值: f(x)
/////////////////////////////////////////////////////////////////
//
double fitness_levyno5(int dimense, double * vals, void * data)
{
    double result = 0;
    double tmp1 = 0;
    double tmp2 = 0;
    for (int i = 1; i <= 5; i++) {
        tmp1 = tmp1 + i * cos( (i-1) * (*(vals + 1)) + i);
        tmp2 = tmp2 + i * cos( (i+1) * (*(vals + 2)) + i);
    }
    result = tmp1 * tmp2 + ((*(vals + 1)) + 1.42513) * ((*(vals + 1)) + 1.42513) + ((*(vals + 2)) + 0.80032) * ((*(vals + 2)) + 0.80032);
    return result;
}

// 简单优化问题PSO示例
void pso_calc_test()
{
    IN_PARA_PSO in_para;	// pso入口参数
	OUT_PARA_PSO* result = NULL;
	int generation = 0;		// pso计算完成后这个变量保存进化代数
    
    int dimense = 5;
    /////////////////////////////////////////////////////////////////
    // pso入口参数定义
    in_para.dimense = dimense;	// 维数
	// 根据判断矩阵维数确定pso参数
	in_para.pop_number = 100;	// 种群规模
	in_para.max_gen = 100;		// 最大进化代数
	in_para.max_no_improve = in_para.max_gen;	// 最大无进化代数
    
	in_para.max_no_improve_stop = 10;	// 达到最到无进化代数后停止，不变异
	in_para.val_max = 10;	// 各维搜索空间最大值为10
	in_para.val_min = -10;	// 给位搜索空间最小值为-10
	in_para.spd_max = 0.2;	// pso中的最大速度设置为0.2
    
	in_para.fitness_fun = fitness_test;	// 适应值函数为fitness_test, 不同优化问题变此即可
	
    in_para.use_break_condition = 0;	// 不使用结束条件
	in_para.c1 = 0.9;   // PSO参数c1
	in_para.c2 = 1.2;   // PSO参数c2
    
	// 如果适应值计算需要用到数据, 自定义一个struct并初始化数据, 赋值给in_para.fitness_data
	// some_dt = (XX_DATA*)malloc(sizeof(XX_DATA));
	// some_dt->dimense = in_para.dimense;
	// some_dt->data = matrix;
	// in_para.fitness_data = ahp_dt;	// 适应值函数所需data
    
    /////////////////////////////////////////////////////////////////
    // pso迭代
    generation = pso(in_para,&result);	// pso迭代进化
    
    /////////////////////////////////////////////////////////////////
    // 显示优化结果
    printf("\n========================================\n");
    printf("= Simple test:\n");
    printf("= Iteration: %d\n", generation);
    printf("= Best Fitness: %8f\n",result->best_fitness);
    printf("= Best Position:\n");
    for (int i=0; i<dimense; i++) {
        printf("= %d: %8f\n", i+1, result->best_particle[i]);
    }
    printf("========================================\n");
    
    /////////////////////////////////////////////////////////////////
    // 释放优化结果内存
    end_pso(result);
    
    // 如果适应值计算需要用到数据, 释放之
    // free(some_dt->position);
	// free(some_dt);	// 释放fitness的data（ahp_dt）
}

// benchmark函数levyno5示例
void pso_calc_levyno5()
{
    IN_PARA_PSO in_para;	// pso入口参数
	OUT_PARA_PSO* result = NULL;
	int generation = 0;		// pso计算完成后这个变量保存进化代数
    
    int dimense = 2;
    /////////////////////////////////////////////////////////////////
    // pso入口参数定义
    in_para.dimense = dimense;	// 维数
	// 根据判断矩阵维数确定pso参数
	in_para.pop_number = 100;	// 种群规模
	in_para.max_gen = 100;		// 最大进化代数
	in_para.max_no_improve = in_para.max_gen;	// 最大无进化代数
    
	in_para.max_no_improve_stop = 10;	// 达到最到无进化代数后停止，不变异
	in_para.val_max = 0;	// 各维搜索空间最大值为0
	in_para.val_min = -3;	// 给位搜索空间最小值为-10
	in_para.spd_max = 0.1;	// pso中的最大速度设置为0.1
    
	in_para.fitness_fun = fitness_levyno5;	// 适应值函数为fitness_levyno5, 不同优化问题变此即可
	
    in_para.use_break_condition = 0;	// 不使用结束条件
	in_para.c1 = 0.9;   // PSO参数c1
	in_para.c2 = 1.2;   // PSO参数c2
    
	// 如果适应值计算需要用到数据, 自定义一个struct并初始化数据, 赋值给in_para.fitness_data
	// some_dt = (XX_DATA*)malloc(sizeof(XX_DATA));
	// some_dt->dimense = in_para.dimense;
	// some_dt->data = matrix;
	// in_para.fitness_data = ahp_dt;	// 适应值函数所需data
    
    /////////////////////////////////////////////////////////////////
    // pso迭代
    generation = pso(in_para,&result);	// pso迭代进化
    
    /////////////////////////////////////////////////////////////////
    // 显示优化结果
    printf("\n========================================\n");
    printf("= LevyNO5:\n");
    printf("= Iteration: %d\n", generation);
    printf("= Best Fitness: %8f\n",result->best_fitness);
    printf("= Best Position:\n");
    for (int i=0; i<dimense; i++) {
        printf("= %d: %8f\n", i+1, result->best_particle[i]);
    }
    printf("========================================\n");
    
    /////////////////////////////////////////////////////////////////
    // 释放优化结果内存
    end_pso(result);
    
    // 如果适应值计算需要用到数据, 释放之
    // free(some_dt->position);
	// free(some_dt);	// 释放fitness的data（ahp_dt）
}

/////////////////////////////////////////////////////////////////
// 说明:
//
/////////////////////////////////////////////////////////////////
int main(int argc, const char * argv[])
{
    pso_calc_test();
    pso_calc_levyno5();
    return 0;
}
