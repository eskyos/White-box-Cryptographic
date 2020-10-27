#include "generator.h"

void init()
{
	//�������ɿ������������

	//���ɿ������
	for (int i = 0; i < 36; i++)
	{
		creatInvMat(P[i], 32);
		if (i < 32)
		{
			creatInvMat(Q[i], 32);
			for (int j = 0; j < 4; j++)
				creatInvMat(sE[i][j], 8);
		}
	}
	//���ɶԽǾ���E
	for (int i = 0; i < 32; i++)
		creatDiagMat(E[i], sE[i]);

	//�����������
	for (int i = 0; i < 36; i++)
	{
		random(p[i], 32);
		p1[i] = p[i];
		if (i < 32)
		{
			random(q[i], 32);
			random(e[i], 32);

			//��e�����и����sE����ĳ�������
			se[i][0] = getPartVector(e[i], 0, 8);
			se[i][1] = getPartVector(e[i], 8, 16);
			se[i][2] = getPartVector(e[i], 16, 24);
			se[i][3] = getPartVector(e[i], 24, 32);
		}
	}

}

//������ṹB��Cд���ļ���
void writeAffineTable(ofstream& file, affine_struct* A, string name)
{
	uint affineMatrix[32][32];
	uint affineVector[32];

	//�����������е�Ԫ��ת��Ϊuint������ʽ
	for (int i = 0; i < 32; i++)
	{
		affineVector[i] = vec2uint(A[i].vector);
		for (int j = 0; j < 32; j++)
			affineMatrix[i][j] = vec2uint(A[i].matrix[j]);
	}

	//��ʼ���ļ�д������
	file << endl;

	//д������
	file << "uint " << name << "_vector[32] = {";
	for (int i = 0; i < 32; i++)
	{
		if (i != 31)
			file << affineVector[i] << ", ";
		else
			file << affineVector[i] << "};";
	}
	file << endl;
	
	//д�����
	file << "uint " << name << "_matrix[32][32] = {" << endl;
	for (int i = 0; i < 32; i++)
	{
		if (i != 31)
		{
			file << "	{";
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << affineMatrix[i][j] << ", ";
				else
				{
					file << affineMatrix[i][j] << "}," << endl;
				}
			}
		}
		else
		{
			file << "	{";
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << affineMatrix[i][j] << ", ";
				else
				{
					file << affineMatrix[i][j] << "}" << endl;
					file << "};" << endl;
				}
			}
		}
	}
	file << endl;
}

//������Dд���ļ�
void writeAffineDTable(ofstream& file, affine_struct D[32][3], string name)
{
	uint affineMartix[96][32];
	uint affineVector[96];

	//������ת��Ϊuint������ʽ
	int num = 0;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 3; j++)
			affineVector[num++] = vec2uint(D[i][j].vector);
	}

	//������ת��Ϊuint������ʽ
	int numM = 0;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 32; k++)
				affineMartix[numM][k] = vec2uint(D[i][j].matrix[k]);
			numM++;
		}
	}


	//����������д���ļ�
	file << endl;
	file << "uint " << name << "_vector[96] = {";
	for (int i = 0; i < 96; i++)
	{
		if (i != 95)
			file << affineVector[i] << ", ";
		else
			file << affineVector[i] << "};" << endl;
	}

	//������д���ļ�
	file << endl;
	file << "uint " << name << "_matrix[96][32] = {" << endl;
	for (int i = 0; i < 96; i++)
	{
		if (i != 95)
		{
			file << "	{";
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << affineMartix[i][j] << ", ";
				else
					file << affineMartix[i][j] << "}," << endl;
			}
		}
		else
		{
			file << "    {";
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << affineMartix[i][j] << ", ";
				else
					file << affineMartix[i][j] << "}" << endl << "};";
			}
		}
	}
}

//���������������д���ļ���
void creatAffineTable()
{

//���з����ļ���
	//�������B
	for (int i = 0; i < 32; i++)
	{
		B[i].matrix = P[i + 4] * inv(Q[i]);
		B[i].vector = P[i + 4] * inv(Q[i]) * q[i];
	}

	//�������C
	for (int i = 0; i < 32; i++)
	{
		C[i].matrix = P[i + 4] * inv(P[i]);
		C[i].vector = P[i + 4] * inv(P[i]) * p[i] + p1[i + 4];
	}

	//�������D
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			D[i][j].matrix = E[i] * inv(P[i + j + 1]);
			D[i][j].vector = E[i] * inv(P[i + j + 1]) * p[i + j + 1] + e[i];
		}
	}

//�������������
	ofstream outFile;
	outFile.open("affineTable.h",ios::app);
	outFile << "typedef unsigned int uint;" << endl;

	//������B��C��D����ͷ�ļ��У���uint����ʽ��֯����������
	writeAffineTable(outFile, B, "B");
	writeAffineTable(outFile, C, "C");
	writeAffineDTable(outFile, D, "D");
	
	outFile.close();
}

//���ⲿ����д���ļ���
void writeExternalEncode()
{
	ofstream file;
	//���ⲿ����д�뵽�ļ���
	file.open("externalEncode.h", ios::app);
	file << "typedef unsigned int uint;" << endl;

	//��д���������
	file << "uint IN[4][32] = {" << endl;
	for (int i = 0; i < 4; i++)
	{
		file << "    {";
		if (i != 3)
		{
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << vec2uint(P[i][j]) << ", ";
				else
					file << vec2uint(P[i][j]) << "}," << endl;
			}
		}
		else
		{
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << vec2uint(P[i][j]) << ", ";
				else
					file << vec2uint(P[i][j]) << "}" << endl << "};";
			}
		}
	}
	file << endl;
	//д���������ĳ�������
	file << "uint IN_vec[4] = {";
	for (int i = 0; i < 4; i++)
	{
		if (i != 3)
			file << vec2uint(p[i]) << ", ";
		else
			file << vec2uint(p[i]) << "};" << endl;
	}

	//д���������
	file << "uint OUT[4][32] = {" << endl;
	for (int i = 0; i < 4; i++)
	{
		file << "    {";
		if (i != 3)
		{
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << vec2uint(P[32 + i][j]) << ", ";
				else
					file << vec2uint(P[32 + i][j]) << "}," << endl;
			}
		}
		else
		{
			for (int j = 0; j < 32; j++)
			{
				if (j != 31)
					file << vec2uint(P[32 + i][j]) << ", ";
				else
					file << vec2uint(P[32 + i][j]) << "}" << endl << "};";
			}
		}
	}

	//д���������ĳ�������
	file << endl;
	file << "uint OUT_vec[4] = {";
	for (int i = 32; i < 36; i++)
	{
		if (i != 35)
			file << vec2uint(p[i]) << ", ";
		else
			file << vec2uint(p[i]) << "};" << endl;
	}

	file.close();
}

//�������ұ�
void createLookUpTable(uint* mainKey)
{
	//������M���룬�þ�������L����������
	mat_GF2 L_M;
	random(L_M, 32, 32);
	for (int i = 0; i < 32; i++)
		L_M[i] = uint2vec(M_usefor_L[i]);

	//��������Կ��������ת��Ϊ������ʽ�����������
	vec_GF2 vecKey[32];
	uint* roundKey = new uint[32];
	genRoundKey(mainKey, roundKey);
	for (int i = 0; i < 32; i++)
		vecKey[i] = uint2vec(roundKey[i]);
	delete[] roundKey;

	uint TABLE[128][256];
	int tableNum = 0;
	for (int r = 0; r < 32; r++)
	{
		mat_GF2 R = Q[r] * L_M;
		mat_GF2 sR[4];
		matSlice(8, R, sR);

		vec_GF2 Y;
		random(Y, 32);
		clear(Y);

		for (int j = 0; j < 4; j++)
		{
			byte plaintext = 0;
			for (int k = 0; k < 256; k++)
			{
				Y = sR[j] * S_box(inv(sE[r][j]) * (byte2vec(plaintext) +
					getPartVector(e[r], j * 8, j * 8 + 8) )+
					getPartVector(vecKey[r], j * 8, j * 8 + 8));
				if (j == 3)
					Y += q[r];
				TABLE[tableNum][k] = vec2uint(Y);
				plaintext++;
			}
			tableNum++;
		}
	}




	//�����ɵĲ��ұ�д���ļ���
	ofstream tableFile;
	tableFile.open("table.h", ios::app);
	tableFile << "typedef unsigned int uint;" << endl;


	tableFile << endl;
	tableFile << "uint TABLE[128][256] = {" << endl;
	for (int i = 0; i < 128; i++)
	{
		tableFile << "    {";
		if (i != 127)
		{
			for (int j = 0; j < 256; j++)
			{
				if (j != 255)
					tableFile << TABLE[i][j] << ", ";
				else
					tableFile << TABLE[i][j] << "}," << endl;
			}
		}
		else
		{
			for (int j = 0; j < 256; j++)
			{
				if (j != 255)
					tableFile << TABLE[i][j] << ", ";
				else
					tableFile << TABLE[i][j] << "}" << endl << "};";
			}
		}
	}
	tableFile.close();
}

void generator(uint* mainKey)
{
	//���ȳ�ʼ�����еľ���������
	init();

	//���ɷ������д���ļ�
	creatAffineTable();

	//���ⲿ����д���ļ�
	writeExternalEncode();

	//�������ɵľ���������������Լ���������Կ���ɲ��ұ�
	createLookUpTable(mainKey);
}