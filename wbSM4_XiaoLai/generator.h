#pragma once
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "wbSM4.h"
#include <fstream>
//������Ҫ�õ��ľ���������

//�������
static mat_GF2 P[36];
static mat_GF2 Q[32];
static mat_GF2 E[32];//�ԽǾ���

static mat_GF2 sE[32][4];//����E�ĶԽ��ϵ�С����

//����
static vec_GF2 p[36];
static vec_GF2 p1[36];
static vec_GF2 q[32];
static vec_GF2 e[32];
static vec_GF2 se[32][4];

//�����
static affine_struct B[32], C[32], D[32][3];
//��ʼ������������
void init();

//���ɷ�����ļ�
void creatAffineTable();
//���º������𽫷����д���ļ���
void writeAffineTable(ofstream& file, affine_struct* A,string name);
void writeAffineDTable(ofstream& file, affine_struct D[32][3], string name);

//���ⲿ����д���ļ���
void writeExternalEncode(ofstream& file);
//����8��32���Ĳ��ұ�
void creatLookUpTable(uint* mainKey);


//������Կ�����ɲ��ұ�����ط����
void generator(uint* mainKey);

#endif 