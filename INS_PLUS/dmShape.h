#pragma once
#include "stdafx.h"
#include "objArg.h"
#include <vector>
#include <utility>
#include <tuple>
#include <queue>
#include <functional>
struct Shape;
struct Mask;
extern Shape shapes[16];
extern Mask masks[16];

float inline smp(float x0, float x1, float xm,std::function<float(float)> F)
{
	return (F(x0) + F(x1) + 4 * F(xm))*(x1 - x0) / 6;
}

float Asmp(float xl, float xr, std::function<float(float)> F);

struct Shape
{
	int type;//shape����ʽ
	std::vector<std::pair<float,float>> pts;//�����ʽΪ�������ʹ��
	std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::pair<float, float>>> ptsBz;//�����ʽΪ����������·����ʹ��
	std::vector<std::tuple<float, float, float, float, float, float, float, float>> funcsBz;//�����ʽΪ����������·����ʹ��
	int isTransed;
	float x,y;//shape������
	float ix, iy, jx, jy;//shapeͼ�εı任
	float ixN, iyN, jxN, jyN;//shapeͼ�ε���
	float sx, sy;//shape�Ĵ�С,�����ʽΪ�������洢������
	float mx, my;//�洢��С����
	Shape() { init(); }
	void init() { type = 0; x = y = iy = jx = 0; ix = jy = sx = sy = 1; transN(); pts.clear(); ptsBz.clear(); isTransed = 0; funcsBz.clear(); mx=my = 0; }
	void inline move(float mx, float my) { x = mx; y = my; }
	void transN()
	{
		float w = ix * jy - jx * iy;
		if (fabsf(w)<EPS)
		{
			//����
			MessageBox(NULL, _T("shape������ʽ����,�⵽��ά���("), _T("����INS_PLUS.dll"), MB_OK);
			w = 1;
		}
		w = 1 / w;
		ixN = jy * w;
		iyN = -iy * w;
		jxN = -jx * w;
		jyN = ix * w;
	}
	void trans(float a, float b, float c, float d)
	{
		float m, n, o, p;
		m = ix * a + iy * c;
		n = ix * b + iy * d;
		o = jx * a + jy * c;
		p = jx * b + jy * d;
		ix = m; iy = n; jx = o; jy = p;
		transN();
	}
	void inline rot(float r){
		trans(cosf(r),sinf(r),-sinf(r),cosf(r));}
	void inline shear(float x, float y){
		trans(1.0f, y, x, 1.0f);}
	void inline zoom(float x, float y){
		trans(x,0,0,y);}
	bool isInside(float a, float b);
	void transBz();
	bool isInsideA(float a, float b)
	{
		return isInside(a - x, b - y);
	}
	std::pair<float, float> transOthPt(float X, float Y) { return std::make_pair(ix*X+jx*Y,iy*X+jy*Y); }
	std::pair<float, float> transOthPtN(float X, float Y) { return std::make_pair(ixN*X + jxN * Y, iyN*X + jyN * Y); }
};

struct Mask
{
	std::vector<std::pair<std::pair<std::shared_ptr<Shape>,Shape*>,int>> shp;
	float x, y, m, n;
	void init() { shp.clear(); x = y = m = n = 0; }
	void addShape(Shape* sh,int type)
	{
		if(type>5 || type <0)//����
			MessageBox(NULL, _T("����:û�ж�Ӧ�Ĳ�������"), _T("����INS_PLUS.dll"), MB_OK);
		shp.push_back(std::make_pair(std::make_pair(nullptr,sh), type));
	}
	void addNewShape(Shape &sh, int type)
	{
		if (type>5 || type <0)//����
			MessageBox(NULL, _T("����:û�ж�Ӧ�Ĳ�������"), _T("����INS_PLUS.dll"), MB_OK);
		std::shared_ptr<Shape> v(new Shape);
		*v = sh;
		shp.push_back(std::make_pair(std::make_pair(v,nullptr), type));
	}
	bool isInside(float x, float y)
	{
		bool isInsd = 1;
		bool esi;
		for (auto &sp : shp)
		{
			if (sp.second != 4)
			{
				if (sp.first.second != nullptr)
				{
					esi = sp.first.second->isInsideA(x, y);
				}else{
					esi = sp.first.first->isInsideA(x, y);
				}
			}
			switch (sp.second)
			{
				//0:ȡ����,1:ȡ����,2:ȡ�(b-shapeN),3:ȡ�(shapeN-b),4:ȡ����(shapeN����Ҫʹ��),5:�ԳƲ�
				//esi = sp.first->isInsideA(x, y);
			case 0:
				isInsd = isInsd && esi; break;
			case 1:
				isInsd = isInsd || esi; break;
			case 2:
				isInsd = (!esi) && isInsd; break;
				isInsd = (!isInsd) && esi; break;
			case 4:
				isInsd = !isInsd; break;
			case 5:
				isInsd = (!(isInsd && esi)) && (esi || isInsd); break;
			default:
				//����
				MessageBox(NULL,_T("����:û�ж�Ӧ�Ĳ�������"),_T("����INS_PLUS.dll"),MB_OK);
			}
		}
		return isInsd;
	}
	bool isInsideA(float a, float b) {
		return isInside(a-x,b-y);
	}
};
std::tuple<int, float, float, float> getRoot3(float a, float b, float c, float d);
std::pair<int, float> getRootBzLine(float ex1, float ey1, float ex2, float ey2, float ax, float bx, float cx, float dx, float ay, float by, float cy, float dy);
std::pair<float, float> reflectLine(float dx, float dy, float vx, float vy);
std::tuple<int, float, float> getLineCrossOverPoint(float x1, float y1, float vx, float vy, float a1, float b1, float a2, float b2);

void inline INS_2500(DWORD ptNowObj); 
void inline INS_2501(DWORD ptNowObj);
void inline INS_2502(DWORD ptNowObj);
void inline INS_2503(DWORD ptNowObj);
void inline INS_2504(DWORD ptNowObj);
void inline INS_2505(DWORD ptNowObj);
void inline INS_2506(DWORD ptNowObj);
void inline INS_2507(DWORD ptNowObj);
void inline INS_2508(DWORD ptNowObj);
void inline INS_2509(DWORD ptNowObj);
void inline INS_2510(DWORD ptNowObj);
void inline INS_2511(DWORD ptNowObj);
void inline INS_2512(DWORD ptNowObj);
void inline INS_2513(DWORD ptNowObj);
void inline INS_2514(DWORD ptNowObj);
void inline INS_2515(DWORD ptNowObj);
void inline INS_2516(DWORD ptNowObj);
void inline INS_2517(DWORD ptNowObj);
void inline INS_2518(DWORD ptNowObj);
void inline INS_2519(DWORD ptNowObj);
void inline INS_2520(DWORD ptNowObj);
void inline INS_2521(DWORD ptNowObj);
void inline INS_2522(DWORD ptNowObj);
void inline INS_2523(DWORD ptNowObj);
void inline INS_2524(DWORD ptNowObj);
void inline INS_2525(DWORD ptNowObj);

void inline INS_2500(DWORD ptNowObj) {
	if(autoGetArgIntB(ptNowObj, 0)!=-1)
		shapes[autoGetArgIntB(ptNowObj, 0)].init();
	else
	{
		for(int i=0;i<16;i++)
			shapes[i].init();
	}
};

void inline INS_2501(DWORD ptNowObj){
	int t = autoGetArgIntB(ptNowObj, 1);
	if (t > 3 || t < 0) {
		//����
		MessageBox(NULL, _T("����:ins_2501,ʹ����δ֪����״"), _T("����INS_PLUS.dll"), MB_OK);
	}
	int w = autoGetArgIntB(ptNowObj, 0);
	shapes[w].type = t;
	if (t == 2 || t == 3)
	{
		shapes[w].sx = shapes[w].sy = -1000000;
		shapes[w].mx = shapes[w].my = 1000000;
	}
};

void inline INS_2502(DWORD ptNowObj) {
	int a = autoGetArgIntB(ptNowObj, 0);
	switch (shapes[a].type)
	{
	case 0://Բ
		shapes[a].sx = autoGetArgFloatB(ptNowObj, 1);
		break;
	case 1://����
		shapes[a].sx = autoGetArgFloatB(ptNowObj, 1);
		shapes[a].sy = autoGetArgFloatB(ptNowObj, 2);
		break;
	default:
		//����
		MessageBox(NULL, _T("����:ins_2502,�Դ������״��ֵ"), _T("����INS_PLUS.dll"), MB_OK);
		break;
	}
};

void inline INS_2503(DWORD ptNowObj) {
	int a = autoGetArgIntB(ptNowObj, 0);
	shapes[a].move(autoGetArgFloatB(ptNowObj, 1), autoGetArgFloatB(ptNowObj, 2));
};

void inline INS_2504(DWORD ptNowObj) {
	shapes[autoGetArgIntB(ptNowObj, 0)].rot(autoGetArgFloatB(ptNowObj, 1));
};

void inline INS_2505(DWORD ptNowObj) {
	shapes[autoGetArgIntB(ptNowObj, 0)].zoom(autoGetArgFloatB(ptNowObj, 1), autoGetArgFloatB(ptNowObj, 2));
};

void inline INS_2506(DWORD ptNowObj) {
	shapes[autoGetArgIntB(ptNowObj, 0)].shear(autoGetArgFloatB(ptNowObj, 1), autoGetArgFloatB(ptNowObj, 2));
};

void inline INS_2507(DWORD ptNowObj) {
	int a = autoGetArgIntB(ptNowObj, 0);
	shapes[a].ix = autoGetArgFloatB(ptNowObj, 1);
	shapes[a].iy = autoGetArgFloatB(ptNowObj, 2);
	shapes[a].jx = autoGetArgFloatB(ptNowObj, 3);
	shapes[a].jy = autoGetArgFloatB(ptNowObj, 4);
	shapes[a].transN();
};

void inline INS_2508(DWORD ptNowObj) {
	shapes[autoGetArgIntB(ptNowObj, 0)].trans(autoGetArgFloatB(ptNowObj, 1), autoGetArgFloatB(ptNowObj, 2),
		autoGetArgFloatB(ptNowObj, 3), autoGetArgFloatB(ptNowObj, 4));
};

void inline INS_2509(DWORD ptNowObj) {
	float *x=autoGetPtrArgFloatB(ptNowObj, 1);
	float *y=autoGetPtrArgFloatB(ptNowObj, 2);
	int t = autoGetArgIntB(ptNowObj, 0);
	float a = *x, b = *y;
	float m = shapes[t].ix,n= shapes[t].iy, o= shapes[t].jx, p= shapes[t].jy;
	*x = a * m + b * o;
	*y = a * n + b * p;
};

void inline INS_2510(DWORD ptNowObj) {
	float *x = autoGetPtrArgFloatB(ptNowObj, 1);
	float *y = autoGetPtrArgFloatB(ptNowObj, 2);
	int t = autoGetArgIntB(ptNowObj, 0);
	float a = *x, b = *y;
	float m = shapes[t].ixN, n = shapes[t].iyN, o = shapes[t].jxN, p = shapes[t].jyN;
	*x = a * m + b * o;
	*y = a * n + b * p;
};

void inline INS_2511(DWORD ptNowObj) {
	shapes[autoGetArgIntB(ptNowObj, 0)] = shapes[autoGetArgIntB(ptNowObj, 1)];
};

void inline INS_2512(DWORD ptNowObj) {
	int t = autoGetArgIntB(ptNowObj, 0);
	float a = autoGetArgFloatB(ptNowObj, 1) - shapes[t].x;
	float b = autoGetArgFloatB(ptNowObj, 2) - shapes[t].y;
	if (shapes[t].isInside(a, b))//������
	{
		int pt = (int)getPtrEsp(ptNowObj);
		*(int*)pt = 1;
		*(BYTE*)(pt - 4) = 'i';
		autoSubESP(ptNowObj, -1);
	}else{
		int pt = (int)getPtrEsp(ptNowObj);
		*(int*)pt = 0;
		*(BYTE*)(pt - 4) = 'i';
		autoSubESP(ptNowObj, -1);
	}
};

void inline INS_2513(DWORD ptNowObj) {
	int w = autoGetArgIntB(ptNowObj, 0);
	*autoGetPtrArgFloatB(ptNowObj, 1) = shapes[w].ix;
	*autoGetPtrArgFloatB(ptNowObj, 2) = shapes[w].iy;
	*autoGetPtrArgFloatB(ptNowObj, 3) = shapes[w].jx;
	*autoGetPtrArgFloatB(ptNowObj, 4) = shapes[w].jy;
};

void inline INS_2514(DWORD ptNowObj) {
	int w = autoGetArgIntB(ptNowObj, 0);
	*autoGetPtrArgFloatB(ptNowObj, 1) = shapes[w].ixN;
	*autoGetPtrArgFloatB(ptNowObj, 2) = shapes[w].iyN;
	*autoGetPtrArgFloatB(ptNowObj, 3) = shapes[w].jxN;
	*autoGetPtrArgFloatB(ptNowObj, 4) = shapes[w].jyN;
};

void inline INS_2515(DWORD ptNowObj)
{
	int w = autoGetArgIntB(ptNowObj, 0);
	if (shapes[w].type != 2)
	{//����
		MessageBox(NULL, _T("�������ͼ������·����"), _T("����INS_PLUS.dll"), MB_OK);
		return;
	}
	float x= autoGetArgFloatB(ptNowObj, 1), y= autoGetArgFloatB(ptNowObj, 2);
	shapes[w].pts.push_back(std::make_pair(x,y));//¼��������
	if (x> shapes[w].sx)
	{
		shapes[w].sx = x;
	}
	if (x < shapes[w].mx)
	{
		shapes[w].mx = x;
	}
	if (y > shapes[w].sy)
	{
		shapes[w].sy = y;
	}
	if (y < shapes[w].my)
	{
		shapes[w].my = y;
	}
	//��ͼ����Ϊ��Χ�崢��
}

void inline INS_2516(DWORD ptNowObj)
{
	int w = autoGetArgIntB(ptNowObj, 0);
	if (shapes[w].type != 3)
	{//����
		MessageBox(NULL, _T("�������ͼ�����ӱ�������"), _T("����INS_PLUS.dll"), MB_OK);
		return;
	}
	shapes[w].isTransed = 0;//�Ƿ�ת��=0
	shapes[w].ptsBz.push_back(std::make_tuple(
		std::make_pair(autoGetArgFloatB(ptNowObj, 1),autoGetArgFloatB(ptNowObj, 2)),
		std::make_pair(autoGetArgFloatB(ptNowObj, 3), autoGetArgFloatB(ptNowObj, 4)),
		std::make_pair(autoGetArgFloatB(ptNowObj, 5), autoGetArgFloatB(ptNowObj, 6))));
}

void inline INS_2517(DWORD ptNowObj)
{
	shapes[autoGetArgIntB(ptNowObj, 0)].transBz();
}

void inline INS_2518(DWORD ptNowObj)
{
	int t = autoGetArgIntB(ptNowObj, 0);
	int pt = (int)getPtrEsp(ptNowObj);
	
	switch (shapes[t].type)
	{
	case 0:
		*(int*)pt = 1; break;
	case 1:
		*(int*)pt = 4; break;
	case 2:
		*(int*)pt =shapes[t].pts.size(); break;
	case 3:
		*(int*)pt = shapes[t].ptsBz.size(); break;
	}
	*(BYTE*)(pt - 4) = 'i';
	autoSubESP(ptNowObj, -1);
}

void inline INS_2519(DWORD ptNowObj)
{
	int a=autoGetArgIntB(ptNowObj, 0);
	int N;
	float t= autoGetArgFloatB(ptNowObj, 2);
	float x, y;
	switch (shapes[a].type)
	{
	case 0://��Բ
		x = cosf(t*6.283185482f)*shapes[a].sx;
		y = sinf(t*6.283185482f)*shapes[a].sx;
		break;
	case 1://��
		N = autoGetArgIntB(ptNowObj, 1)%4; 
		switch (N)
		{
		case 3://��
			y = -shapes[a].sy;
			x = (2*t-1)*shapes[a].sx;
			break;
		case 2://��
			x = shapes[a].sx;
			y = (2 * t - 1)*shapes[a].sy;
			break;
		case 1://��
			y = shapes[a].sy;
			x = (1 - 2 * t)*shapes[a].sx;
			break;
		case 0://��
			x = -shapes[a].sx;
			y = (1 - 2 * t)*shapes[a].sy;
			break;
		default:
			x = y = 0;
			MessageBox(NULL, _T("�߶�ȡֵ��С����"), _T("����INS_PLUS.dll"), MB_OK); break;
		}
		break;
	case 2://·��ͼ��
		if (shapes[a].pts.size() < 2)
		{
			MessageBox(NULL, _T("·��������̫��,����Ϊ2"), _T("����INS_PLUS.dll"), MB_OK);
			x = y = 0; break;
		}{//ȡֵ
			N = autoGetArgIntB(ptNowObj, 1) % shapes[a].pts.size();
			float x1, y1, x2, y2;
			x1 = shapes[a].pts[N].first;
			y1 = shapes[a].pts[N].second;
			N += 1;
			N = N % shapes[a].pts.size();
			x2 = shapes[a].pts[N].first;
			y2 = shapes[a].pts[N].second;
			//����ĸ��������
			x = (x2 - x1)*t + x1;
			y = (y2 - y1)*t + y1;
		}
		break;
	case 3:
		if (!shapes[a].ptsBz.size())
		{
			MessageBox(NULL, _T("������·��������̫��,����Ϊ1"), _T("����INS_PLUS.dll"), MB_OK);
			x = y = 0; break;
		}
		N = autoGetArgIntB(ptNowObj, 1) % shapes[a].ptsBz.size();
		if (!shapes[a].isTransed)
		{
			shapes[a].transBz();
		}{
			float xa, xb, xc, xd, ya, yb, yc, yd;
			xa = std::get<0>(shapes[a].funcsBz[N]);
			xb = std::get<1>(shapes[a].funcsBz[N]);
			xc = std::get<2>(shapes[a].funcsBz[N]);
			xd = std::get<3>(shapes[a].funcsBz[N]);
			ya = std::get<4>(shapes[a].funcsBz[N]);
			yb = std::get<5>(shapes[a].funcsBz[N]);
			yc = std::get<6>(shapes[a].funcsBz[N]);
			yd = std::get<7>(shapes[a].funcsBz[N]);
			x = t * t*t*xa + t * t*xb + t * xc + xd;
			y = t * t*t*ya + t * t*yb + t * yc + yd;
		}
		break;
	default:
		MessageBox(NULL, _T("ͼ�δ���"), _T("����INS_PLUS.dll"), MB_OK); break;
	}
	//�������Ա任
	float X = x, Y = y;
	float m = shapes[a].ix, n = shapes[a].iy, o = shapes[a].jx, p = shapes[a].jy;
	x = X * m + Y * o;
	y = X * n + Y * p;
	//���ϱ���λ��
	x += shapes[a].x;
	y += shapes[a].y;
	//��ջ
	int pt = (int)getPtrEsp(ptNowObj);
	*(float*)pt = y;
	*(BYTE*)(pt - 4) = 'f';
	pt += 8;
	*(float*)pt = x;
	*(BYTE*)(pt - 4) = 'f';
	autoSubESP(ptNowObj, -2);
}

void inline INS_2520(DWORD ptNowObj)
{
	int t = autoGetArgIntB(ptNowObj, 0);
	if (t == -1)
	{
		for (int i = 0; i < 16; i++)
			masks[i].init();
		return;
	}
	masks[t].init();
}

void inline INS_2521(DWORD ptNowObj)
{
	int type = autoGetArgIntB(ptNowObj, 2);
	if (type == 4)
	{
		masks[autoGetArgIntB(ptNowObj, 0)].shp.push_back(std::make_pair(std::make_pair(nullptr,nullptr), type));
		return;
	}
	masks[autoGetArgIntB(ptNowObj, 0)].addShape(&shapes[autoGetArgIntB(ptNowObj, 1)], type);
}

void inline INS_2522(DWORD ptNowObj)
{
	int type = autoGetArgIntB(ptNowObj, 2);
	if (type == 4)
	{
		masks[autoGetArgIntB(ptNowObj, 0)].shp.push_back(std::make_pair(std::make_pair(nullptr, nullptr), type));
		return;
	}
	masks[autoGetArgIntB(ptNowObj, 0)].addNewShape(shapes[autoGetArgIntB(ptNowObj, 1)], type);
}

void inline INS_2523(DWORD ptNowObj)
{
	int m = autoGetArgIntB(ptNowObj, 0);
	masks[m].x=autoGetArgFloatB(ptNowObj,1);
	masks[m].y=autoGetArgFloatB(ptNowObj,2);
}

void inline INS_2524(DWORD ptNowObj)
{
	int m = autoGetArgIntB(ptNowObj, 0);
	if (masks[m].isInside(autoGetArgFloatB(ptNowObj, 1)-masks[m].x, autoGetArgFloatB(ptNowObj, 2)-masks[m].y))//������
	{
		int pt = (int)getPtrEsp(ptNowObj);
		*(int*)pt = 1;
		*(BYTE*)(pt - 4) = 'i';
		autoSubESP(ptNowObj, -1);
	}
	else {
		int pt = (int)getPtrEsp(ptNowObj);
		*(int*)pt = 0;
		*(BYTE*)(pt - 4) = 'i';
		autoSubESP(ptNowObj, -1);
	}
}

void inline INS_2525(DWORD ptNowObj)
{
	int m = autoGetArgIntB(ptNowObj, 0);
	masks[m].m = autoGetArgFloatB(ptNowObj, 1);
	masks[m].n = autoGetArgFloatB(ptNowObj, 2);
}