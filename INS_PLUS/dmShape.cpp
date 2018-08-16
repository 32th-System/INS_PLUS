#include "stdafx.h"
#include "dmShape.h"
Shape shapes[16];
Mask masks[16];

float Asmp(float xl, float xr, std::function<float(float)> F)
{
	float res = 0;
	std::queue<std::pair<std::pair<float, float>, float>> pt;
	pt.push(std::make_pair(std::make_pair(xl, xr), smp(xl, xr, (xl + xr) / 2, F)));
	float x0, x1, x2, T, L, R;
	int i = 500;
	while (!pt.empty() && i>0)
	{
		x0 = pt.front().first.first;
		x1 = pt.front().first.second;
		T = pt.front().second;
		pt.pop();
		x2 = (x0 + x1) / 2;
		L = smp(x0, x2, (x0 + x2) / 2, F);
		R = smp(x2, x1, (x1 + x2) / 2, F);
		if (fabs(L + R - T) <= 50 * EPS)
		{
			res += L + R + (L + R - T) / 50;
		}
		else {
			pt.push(std::make_pair(std::make_pair(x0, x2), L));
			pt.push(std::make_pair(std::make_pair(x2, x1), R));
		}
		i--;
	}
	return res;
}

int dcmp(float x)
{
	if (fabs(x) <= EPS){return 0;}
	else if (x < 0){return -1;}
	return 1;
}

bool Shape::isInside(float a, float b)
{

	float x, y;
	x = a * ixN + b * jxN;
	y = a * iyN + b * jyN;//���(a,b)����
	if (type == 0)//��Բ
	{
		return x * x + y * y <= sx * sx;
	}
	if (type == 1)//ƽ���ı���
	{
		return fabsf(x) <= sx && fabsf(y) <= sy;
	}
	if (type == 2)//·��ͼ��
	{
		int wd = 0, n = pts.size();
		if (n < 3)
		{
			//����
			MessageBox(NULL, _T("����:·��������Ҫ3����"), _T("����INS_PLUS.dll"), MB_OK);
		}
		if (x>sx || y>sy || x<mx || y<my)//�ڰ�Χ���ⲿ
		{
			return 0;
		}
		for (int i = 0,j=0; i < n; i++)
		{
			float x1 = pts[i].first,y1=pts[i].second;
			j = i + 1; if (j == n)j = 0;
			float x2 = pts[j].first, y2 = pts[j].second;//(x1,y1)(x2,y2)Ϊ�������Ե�
			if (fabsf((y-y1)*(x2-x) - (x-x1)*(y2-y)) <= EPS)//�߽��ж�
			{
				return 1;//�߽���=�ڲ�
			}
			int k=dcmp((x2 - x1)*(y - y1) - (y2 - y1)*(x - x1));
			int d1 = dcmp(y1 - y),d2=dcmp(y2-y);
			if (k > 0 && d1 <= 0 && d2 > 0)wd++;
			else if (k < 0 && d2 <= 0 && d1 > 0)wd--;
		}
		if (wd != 0)return 1;
		return 0;
	}
	if (type == 3)//����������
	{
		if (isTransed == 0)
		{
			this->transBz();
		}
		if (x>sx || y>sy || x<mx || y<my)//�ڰ�Χ���ⲿ
		{
			return 0;
		}
		float a, b, c, d, m, n, o, p;
		//���������ߵĽǶȵ�΢��
		int N = funcsBz.size();
		float w=0;//����
		for (int i = 0; i < N; i++)
		{
			a = std::get<0>(funcsBz[i]);
			b = std::get<1>(funcsBz[i]);
			c = std::get<2>(funcsBz[i]);
			d = std::get<3>(funcsBz[i]);
			m = std::get<4>(funcsBz[i]);
			n = std::get<5>(funcsBz[i]);
			o = std::get<6>(funcsBz[i]);
			p = std::get<7>(funcsBz[i]);
			std::function<float(float)> F = [a, b, c, d, m, n, o, p, x, y](float t)
			{
				float xt = a * t * t * t + b * t * t + c * t + d;
				float xt_ = 3 * a * t * t + 2 * b * t + c;
				float yt = m * t * t * t + n * t * t + o * t + p;
				float yt_ = 3 * m * t * t + 2 * n * t + o;
				return ((xt-x) * yt_ - (yt-y)*xt_) / ((xt-x) * (xt - x) + (yt-y) * (yt - y));
			};
			w += Asmp(0, 1, F)/6.28318531f;
		}
		if (fabs(w) <= 0.15)//Ϊ0
		{
			return 0;
		}
		return 1;
	}
	return 0;
}

void Shape::transBz()
{
	if (type != 3)
	{
		MessageBox(NULL, _T("����:���Ǳ��������ߵ�ͼ��ת��"), _T("����INS_PLUS.dll"), MB_OK);
	}
	int n = ptsBz.size();
	if (n < 1)
	{
		MessageBox(NULL, _T("����:��������������̫��"), _T("����INS_PLUS.dll"), MB_OK);
	}
	std::pair<float, float> pt0, pt1, pt2,pt3;
	float xa, xb, xc, xd, ya, yb, yc, yd, del, m1, m2, a, b;
	for (int i = 0; i < n; i++)
	{
		/*
		cx = 3 * ( x1 - x0 )
		bx = 3 * ( x2 - x1 ) - cx
		ax = x3 - x0 - cx - bx
		cy = 3 * ( y1 - y0 )
		by = 3 * ( y2 - y1 ) - cy
		ay = y3 - y0 - cy - by
		*/
		int j = i + 1; if (j == n)j = 0;
		pt0 = std::get<0>(ptsBz[i]);
		pt1 = std::get<2>(ptsBz[i]);
		pt2 = std::get<1>(ptsBz[j]);
		pt3 = std::get<0>(ptsBz[j]);
		xd = pt0.first;
		xc = 3 * (pt1.first - xd);
		xb = 3 * (pt2.first - pt1.first) - xc;
		xa = pt3.first - pt0.first - xc - xb;
		yd = pt0.second;
		yc = 3 * (pt1.second - yd);
		yb = 3 * (pt2.second - pt1.second) - yc;
		ya = pt3.second - pt0.second - yc - yb;
		funcsBz.push_back(std::make_tuple(xa, xb, xc, xd, ya, yb, yc, yd));
		//x=f(n)=xa t^3+xb t^2+xc t+xd
		//y=g(n)=ya t^3+yb t^2+yc t+yd
		//t in [0,1]
		//f'(n)=(3*xa)t^2+(2*xb)t+xc
		//g'(n)=(3*ya)t^2+(2*yb)t+yc
		a = 6 * xa; b = 2 * xb;
		del = b * b - 2 * a*xc;//delta=b^2-4ac
		if (del >= 0)//delta>0,��ʹ�þ���EPS
		{
			del = sqrtf(del);
			m1 = (-b + del) / a;
			m2 = (-b - del) / a;
			//���ȡ����ֵ��t
			if (m1 >= 0 && m1 <= 1)
			{
				m1 = xa * m1*m1*m1 + xb * m1*m1 + xc * m1 + xd;
				if (m1 < mx)
					mx = m1;
				else if (m1 > sx)
					sx = m1;
			}
			if (m2 >= 0 && m2 <= 1)
			{
				m2 = xa * m2*m2*m2 + xb * m2*m2 + xc * m2 + xd;
				if (m2 < mx)
					mx = m2;
				else if (m2 > sx)
					sx = m2;
			}
			//��ò�����������ֵ
		}
		m1 = xa + xb + xc + xd;//t=1ʱ
		m2 = xd;//t=0ʱ
		if (m2 < mx)
			mx = m2;
		else if (m2 > sx)
			sx = m2;
		if (m1 < mx)
			mx = m1;
		else if (m1 > sx)
			sx = m1;
		//Ȼ����y���ж�>>>>>>>>>>>>>>>>>>>
		a = 6 * ya; b = 2 * yb;
		del = b * b - 2 * a*yc;
		if (del >= 0)
		{
			del = sqrtf(del);
			m1 = (-b + del) / a;
			m2 = (-b - del) / a;
			if (m1 >= 0 && m1 <= 1)
			{
				m1 = ya * m1*m1*m1 + yb * m1*m1 + yc * m1 + yd;
				if (m1 < my)
					my = m1;
				else if (m1 > sy)
					sy = m1;
			}
			if (m2 >= 0 && m2 <= 1)
			{
				m2 = ya * m2*m2*m2 + yb * m2*m2 + yc * m2 +yd;
				if (m2 < my)
					my = m2;
				else if (m2 > sy)
					sy = m2;
			}
		}
		m1 = ya + yb + yc + yd;
		m2 = yd;
		if (m2 < my)
			my = m2;
		else if (m2 > sy)
			my = m2;
		if (m1 < my)
			my = m1;
		else if (m1 > sy)
			sy = m1;
	}
	isTransed = 1;
}

std::tuple<int, float, float, float> getRoot3(float a, float b, float c, float d)
{
	using namespace std;
	float A, B, C, del, eax, ecx, edx;
	if (a == 0)//2 Order
	{
		del = c * c - 4 * b*d;
		if (del<0)
			return make_tuple(0, 0, 0, 0);
		if (del == 0)
			return make_tuple(1, -c / (2 * b), 0, 0);
		del = sqrtf(del);
		return make_tuple(2, (-c + del) / (2 * b), (-c - del) / (2 * b), 0);
	}
	A = b * b - 3 * a*c;
	B = b * c - 9 * a*d;
	C = c * c - 3 * b*d;
	if (A == 0 && B == 0)
		return make_tuple(1, -b / (3 * a), 0, 0);
	del = B * B - 4 * A*C;
	if (del>0)
	{
		ecx = powf(A*b + 3 * a*((-B + sqrtf(del)) / 2), 1.0f / 3.0f);
		edx = powf(A*b + 3 * a*((-B - sqrtf(del)) / 2), 1.0f / 3.0f);
		return make_tuple(1, (-b - ecx - edx) / (3 * a), 0, 0);
	}
	if (del == 0)
	{
		ecx = B / A;
		return make_tuple(2, (-b / a) + ecx, -ecx / 2, 0);
	}
	//if(del<0)
	edx = (2 * A*b - 3 * a*B) / (2 * sqrtf(A)*A);
	ecx = acosf(edx) / 3;
	edx = 1.732051f*sinf(ecx);
	return make_tuple(3,
		(-b - 2 * sqrtf(A)*cosf(ecx)) / (3 * a),
		(-b + sqrtf(A)*(cosf(ecx) + edx)) / (3 * a),
		(-b + sqrtf(A)*(cosf(ecx) - edx)) / (3 * a)
	);
}


std::pair<int, float> getRootBzLine(float ex1, float ey1, float ex2, float ey2, float ax, float bx, float cx, float dx, float ay, float by, float cy, float dy)
{
	using namespace std;
	float A, B, C, D, lA, lB, lC;
	lA = ey2 - ey1;
	lB = ex1 - ex2;
	lC = -ex1 * (ey2 - ey1) + ey1 * (ex2 - ex1);
	A = ax * lA + ay * lB;
	B = bx * lA + by * lB;
	C = cx * lA + cy * lB;
	D = dx * lA + dy * lB + lC;
	//equa Ax^3+Bx^2+Cx+D=0
	int i;
	float x1, x2, x3;
	tie(i, x1, x2, x3) = getRoot3(A, B, C, D);
	switch (i)
	{
	case 2:
	case 3:
	chooseI2:
		if (x1 - 1 <= EPS && x1 >= -EPS)//x1 in Bz
		{
			A = ax * x1*x1*x1 + bx * x1*x1 + cx * x1 + dx;
			B = ay * x1*x1*x1 + by * x1*x1 + cy * x1 + dy;
			A = (A - ex1) / (ex2 - ex1);
			B = (B - ey1) / (ey2 - ey1);
			if ((A - 1 <= EPS && A >= -EPS) || (B - 1 <= EPS && B >= -EPS))
			{
			chooseI32://~x1,?x2,??x3
				if (x2 - 1 <= EPS && x2 >= -EPS)//x2 in Bz
				{
					C = ax * x2*x2*x2 + bx * x2*x2 + cx * x2 + dx;
					D = ay * x2*x2*x2 + by * x2*x2 + cy * x2 + dy;
					C = (C - ex1) / (ex2 - ex1);
					D = (D - ey1) / (ey2 - ey1);
					if ((C - 1 <= EPS && C >= -EPS) || (D - 1 <= EPS && D >= -EPS))
					{
						//~x1,~x2,??x3
						//x1:(~A,~B),x2:(~C,~D)
						if (A>C || B>D)//x1>x2
							x1 = x2;	//!x1,~x2,??x3
						if (i == 3)
						{
							i--;
							x2 = x3;
							goto chooseI32;
						}
						return make_pair(1, x1);
					}
				}//~x1,!x2,??x3
				if (i == 3)
				{
					i--;
					x2 = x3;
					goto chooseI32;
				}
				return make_pair(1, x1);
			}
		}
		if (i == 3)//!x1,?x2,?x3
		{
			i--;
			x1 = x3;
			goto chooseI2;
		}
		x1 = x2;//!x1,?x2,!x3
		i--;
	case 1:
		if (x1 - 1 <= EPS && x1 >= -EPS)//in Bz
		{
			A = ax * x1*x1*x1 + bx * x1*x1 + cx * x1 + dx;
			B = ay * x1*x1*x1 + by * x1*x1 + cy * x1 + dy;
			A = (A - ex1) / (ex2 - ex1);
			B = (B - ey1) / (ey2 - ey1);
			if ((A - 1 <= EPS && A >= -EPS) || (B - 1 <= EPS && B >= -EPS))//in line
				return make_pair(1, x1);
		}
	case 0:
		return make_pair(0, 0);

	}
}
