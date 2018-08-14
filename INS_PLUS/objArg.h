#pragma once
#include "stdafx.h"
int autoGetArgIntA(DWORD * _obj, int NeedArgN);
int autoGetArgIntB(DWORD ptNowObj, int NeedArgN);
int* autoGetPtrArgIntA(DWORD * _obj, int NeedArgN);
float autoGetArgFloatA(DWORD * _obj, int NeedArgN);
float autoGetArgFloatB(DWORD ptNowObj, int NeedArgN);
int* autoGetPtrArgIntB(DWORD ptNowObj, int NeedArgN);
float* autoGetPtrArgFloatA(DWORD * _obj, int NeedArgN);
float* autoGetPtrArgFloatB(DWORD ptNowObj, int NeedArgN);

inline int getPtrObj(DWORD * obj);//ͨ��_obj���ptObj
inline int* getPtrEsp(DWORD ptNowObj);//���ջ����ַ,��βע��
inline int* getPtrArgSt(DWORD ptNowObj, int ArgCount);//���ջ����ĳ���ĵ�ַ
inline float getFloatArgSt(DWORD ptNowObj, int ArgCount);//���ջ����ĳ��
inline int getIntArgSt(DWORD ptNowObj, int ArgCount);//���ջ����ĳ��
inline void autoSubESP(DWORD ptNowObj, int Arg);//��esp����
inline int* getValueOfESP(DWORD ptNowObj);//��ȡesp�ĵ�ַ


inline int getPtrObj(DWORD * obj)//ͨ��_obj���ptObj
{
	return *(_DWORD *)(obj[0x112E] + 0xC);
}

inline int* getPtrEsp(DWORD ptNowObj)//���ջ����ַ
{
	return (int*)(0x10 + *(_DWORD *)(ptNowObj + 0x100C) + ptNowObj);
}

inline int* getPtrArgSt(DWORD ptNowObj, int ArgCount)//���ջ����ĳ���ĵ�ַ
{
	return (int*)(0x10 + *(_DWORD *)(ptNowObj + 0x100C) + ptNowObj + 8 * ArgCount);
}

inline float getFloatArgSt(DWORD ptNowObj, int ArgCount)//���ջ����ĳ��
{
	int address = (int)getPtrArgSt(ptNowObj, ArgCount);
	char flag = *(BYTE*)(address - 4);
	if (flag == 'i')
	{
		return (float)(*(int*)address);
	}
	return *(float*)address;
}

inline int getIntArgSt(DWORD ptNowObj, int ArgCount)//���ջ����ĳ��
{
	int address = (int)getPtrArgSt(ptNowObj, ArgCount);
	char flag = *(BYTE*)(address - 4);
	if (flag == 'i')
	{
		return *(int*)address;
	}
	return (int)(*(float*)address);
}

inline void autoSubESP(DWORD ptNowObj, int Arg)//��esp����
{
	*getValueOfESP(ptNowObj) -= Arg * 8;
}

inline int* getValueOfESP(DWORD ptNowObj)//��ȡesp�ĵ�ַ
{
	return (int*)(ptNowObj + 0x100C);
}