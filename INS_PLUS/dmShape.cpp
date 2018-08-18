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
		if (fabsf(L + R - T) <= 50 * EPS)
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
	if (fabsf(x) <= EPS){return 0;}
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
			std::tie(a, b, c, d, m, n, o, p) = funcsBz[i];
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
		{
			//��Χ����ж�
			a = 6 * xa; b = 2 * xb;
			del = b * b - 2 * a*xc;//delta=b^2-4ac
			if (del >= 0)//delta>0,��ʹ�þ���EPS
			{
				del = sqrtf(del);
				m1 = (-b + del) / a;
				m2 = (-b - del) / a;
				//���ȡ����ֵ��t
				if (m1 >= -EPS && m1-1 <= EPS)
				{
					m1 = xa * m1*m1*m1 + xb * m1*m1 + xc * m1 + xd;
					if (m1 < mx)
						mx = m1;
					if (m1 > sx)
						sx = m1;
				}
				if (m2 >= -EPS && m2 - 1 <= EPS)
				{
					m2 = xa * m2*m2*m2 + xb * m2*m2 + xc * m2 + xd;
					if (m2 < mx)
						mx = m2;
					if (m2 > sx)
						sx = m2;
				}
				//��ò�����������ֵ
			}
			m1 = xa + xb + xc + xd;//t=1ʱ
			m2 = xd;//t=0ʱ
			if (m2 < mx)
				mx = m2;
			if (m2 > sx)
				sx = m2;
			if (m1 < mx)
				mx = m1;
			if (m1 > sx)
				sx = m1;
			//Ȼ����y���ж�>>>>>>>>>>>>>>>>>>>
			a = 6 * ya; b = 2 * yb;
			del = b * b - 2 * a*yc;
			if (del >= 0)
			{
				del = sqrtf(del);
				m1 = (-b + del) / a;
				m2 = (-b - del) / a;
				if (m1 >= -EPS && m1 - 1 <= EPS)
				{
					m1 = ya * m1*m1*m1 + yb * m1*m1 + yc * m1 + yd;
					if (m1 < my)
						my = m1;
					if (m1 > sy)
						sy = m1;
				}
				if (m2 >= -EPS && m2 - 1 <= EPS)
				{
					m2 = ya * m2*m2*m2 + yb * m2*m2 + yc * m2 + yd;
					if (m2 < my)
						my = m2;
					if (m2 > sy)
						sy = m2;
				}
			}
			m1 = ya + yb + yc + yd;
			m2 = yd;
			if (m2 < my)
				my = m2;
			if (m2 > sy)
				sy = m2;
			if (m1 < my)
				my = m1;
			if (m1 > sy)
				sy = m1;
		}
	}
	isTransed = 1;
}

std::tuple<int, float, float, float> getRoot3(float a, float b, float c, float d)
{
	using namespace std;
	float A, B, C, del, ecx, edx, eax, esi;
	if (a == 0)//2 Order
	{
		del = c * c - 4 * b*d;
		if (del<0)
			return make_tuple(0, 0.0f, 0.0f, 0.0f);
		if (del == 0)
			return make_tuple(1, -c / (2 * b), 0.0f, 0.0f);
		del = sqrtf(del);
		return make_tuple(2, (-c + del) / (2 * b), (-c - del) / (2 * b), 0.0f);
	}
	A = b * b - 3 * a*c;
	B = b * c - 9 * a*d;
	C = c * c - 3 * b*d;
	if (A == 0 && B == 0)
		return make_tuple(1, -b / (3 * a), 0.0f, 0.0f);
	del = B * B - 4 * A*C;
	if (del>0)
	{
		ecx = A * b + 3 * a*((-B + sqrtf(del)) / 2);
		eax = ecx>0 ? 1.0f : -1.0f;
		ecx = powf(fabsf(ecx), 1.0f / 3.0f);
		edx = A * b + 3 * a*((-B - sqrtf(del)) / 2);
		esi = edx>0 ? 1.0f: -1.0f;
		edx = powf(fabsf(edx), 1.0f / 3.0f);
		ecx *= eax;
		edx *= esi;
		return make_tuple(1, (-b - ecx - edx) / (3 * a), 0.0f, 0.0f);
	}
	if (del == 0)
	{
		ecx = B / A;
		return make_tuple(2, (-b / a) + ecx, -ecx / 2, 0.0f);
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

#define EPS2 0.5
std::pair<int, float> getRootBzLine(float ex1, float ey1, float ex2, float ey2, float ax, float bx, float cx, float dx, float ay, float by, float cy, float dy)
{
	using namespace std;
	float A, B, C, D, lA, lB, lC;
	lA = ey2;
	lB = -ex2;
	lC = -ex1 * ey2 + ey1 * ex2;//������(ex2,ey2)*t+(ex1,ey1)����ʽ
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
			A = (A - ex1) / ex2;
			B = (B - ey1) / ey2;
			if ((A - 1 <= EPS2 && A >= -EPS2) || (B - 1 <= EPS2  && B >= -EPS2))
			{
			chooseI32://~x1,?x2,??x3
				if (x2 - 1 <= EPS && x2 >= -EPS)//x2 in Bz
				{
					C = ax * x2*x2*x2 + bx * x2*x2 + cx * x2 + dx;
					D = ay * x2*x2*x2 + by * x2*x2 + cy * x2 + dy;
					C = (C - ex1) / ex2;
					D = (D - ey1) / ey2;
					if ((C - 1 <= EPS2 && C >= -EPS2) || (D - 1 <= EPS2 && D >= -EPS2))
					{
						//~x1,~x2,??x3,x1:(~A,~B),x2:(~C,~D)
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
			A = (A - ex1) / ex2;
			B = (B - ey1) / ey2;
			if ((A - 1 <= EPS2 && A >= -EPS2) || (B - 1 <= EPS2 && B >= -EPS2))//in line
				return make_pair(1, x1);
		}
	case 0:
		return make_pair(0, 0.0f);
	}
	return make_pair(0, 0.0f);
}
#undef EPS2
std::pair<float, float> reflectLine(float dx, float dy, float vx, float vy)
{
	float lenD2 = dx*dx + dy * dy,lenV2 = vx*vx + vy * vy;
	float div1MulLen = 1/sqrtf(lenD2*lenV2);
	float sint,cost,sin2t,cos2t;
	sint = (vx * dy - vy * dx)*div1MulLen;
	cost = (vx*dx + vy * dy)*div1MulLen;
	sin2t = 2 * sint*cost;
	cos2t = 2 * cost * cost - 1;
	return std::make_pair(cos2t*vx - sin2t * vy, sin2t*vx + cos2t * vy);
}

std::tuple<int, float,float> getLineCrossOverPoint(float x1, float y1,float vx,float vy,float a1,float b1,float a2,float b2)
{
	float eax;
	eax = vx * b2 - vy * a2;
	if(fabsf(eax)<EPS)//�߶�ƽ��
	{
		return std::make_tuple(0, 0.0f,0.0f);
	}
	eax = 1 / eax;
	float u1, u2, t1, t2;
	u1 = x1 - a1; u2 = y1 - b1;
	t1 = (a2 * u2 - b2 * u1)*eax;
	t2 = (vx * u2 - vy * u1)*eax;
	if (t1 - 1 < EPS && t1 > -EPS && t2 - 1 < EPS && t2 > -EPS)
	{
		return std::make_tuple(1, t1, t2);
	}
	return std::make_tuple(-1, t1, t2);//ֱ���ཻ�����߶�
}

int reflectDm(int B, float& x, float& y, float& vx, float& vy, float* ptvx,float* ptvy,float* ptr)
{
	float dx, dy, sx, sy, rtx, bx, by;
	bx = vx; by = vy;
	sx = shapes[B].x; sy = shapes[B].y;
	auto p = shapes[B].transOthPtN(x - sx, y - sy);
	x = p.first; y = p.second;
	p = shapes[B].transOthPtN(vx, vy);
	vx = p.first; vy = p.second;
	//���߶��û���������
	switch (shapes[B].type)
	{
	case 0://Բ
	{
		auto root = getRoot3(0, vx*vx + vy * vy, 2 * vx*x + 2 * y*vy, x*x + y * y - shapes[B].sx*shapes[B].sx);
		//���Բ��ֱ�ߵĽ���
		switch (std::get<0>(root))
		{
		case 2:
			if (std::get<1>(root) < 0 || std::get<1>(root)-1>1)
			{
				//���x1���Ϸ�
				if (std::get<2>(root) < 0 || std::get<2>(root)  > 1)
				{
					//���x2���Ϸ�
					return 0;
				}
				rtx = std::get<2>(root);
			}else{
				//x1�Ϸ�
				if (std::get<2>(root) >= 0 && std::get<2>(root)  <= 1 && std::get<2>(root) < std::get<1>(root))
				{
					rtx = std::get<2>(root);//x2�Ϸ��ұ�x1��
				}else{
					rtx = std::get<1>(root);//x2��֮����x1��
				}
			}
			break;
		case 1:
			if (std::get<1>(root) < -EPS || std::get<1>(root) - 1>EPS)
			{
				//���x1���Ϸ�
				return 0;
			}
			rtx = std::get<1>(root);
			break;
		default:
			//ʧЧ
			return 0;
		}
		dx = rtx * vy + y; dy = -(rtx * vx + x);//���б�ʵ�����
	}
	break;
	case 1://��
	{
		float re = 2.0f, esi, edi;
		esi = shapes[B].sx; edi = shapes[B].sy;
		std::tuple<int, float, float> root;
		root = getLineCrossOverPoint(x, y, vx, vy, -esi, -edi, 2 * esi, 0);//��
		if (std::get<0>(root) == 1 && re > std::get<1>(root))
		{
			re = std::get<1>(root); dx = 1; dy = 0;
		}
		root = getLineCrossOverPoint(x, y, vx, vy, -esi, edi, 2 * esi, 0);//��
		if (std::get<0>(root) == 1 && re > std::get<1>(root))
		{
			re = std::get<1>(root); dx = 1; dy = 0;
		}
		root = getLineCrossOverPoint(x, y, vx, vy, -esi, -edi, 0, 2 * edi);//��
		if (std::get<0>(root) == 1 && re > std::get<1>(root))
		{
			re = std::get<1>(root); dx = 0;  dy = 1;
		}
		root = getLineCrossOverPoint(x, y, vx, vy, esi, -edi, 0, 2 * edi);//��
		if (std::get<0>(root) == 1 && re > std::get<1>(root))
		{
			re = std::get<1>(root); dx = 0; dy = 1;
		}
		if (re == 2)
		{
			//ʧЧ
			return 0;
		}
	}
	break;
	case 2://����߶�
	{
		int n = shapes[B].pts.size(), tn = -1, sn = -1;
		float rt = 2.0f;
		float lx, ly, lvx, lvy;
		std::tuple<int, float, float> root;
		int i, j;
		for (i = 0, j = 1; i < n; ++i, ++j)
		{
			if (j == n)
				j = 0;
			lx = shapes[B].pts[i].first;
			ly = shapes[B].pts[i].second;
			lvx = shapes[B].pts[j].first - lx;
			lvy = shapes[B].pts[j].second - ly;
			root = getLineCrossOverPoint(x, y, vx, vy, lx, ly, lvx, lvy);
			if (std::get<0>(root) == 1 && rt > std::get<1>(root))
			{
				rt = std::get<1>(root);
				tn = i;
				sn = j;
			}
		}
		if (tn == -1)
			return 0;//ʧЧ
		dx = shapes[B].pts[sn].first - shapes[B].pts[tn].first;
		dy = shapes[B].pts[sn].second - shapes[B].pts[tn].second;
	}
	break;
	case 3://����������
	{
		int n = shapes[B].funcsBz.size(), tn = -1;
		float w = 0.0f, t, lnd = 999999.0f, ld = 0.0f;
		float Ax, Bx, Cx, Dx, Ay, By, Cy, Dy;
		float xa, ya;
		for (int k = 0; k<n; k++)
		{
			std::tie(Ax, Bx, Cx, Dx, Ay, By, Cy, Dy) = shapes[B].funcsBz[k];
			auto root = getRootBzLine(x, y, vx, vy, Ax, Bx, Cx, Dx, Ay, By, Cy, Dy);
			if (root.first != 0)
			{
				t = root.second;
				xa = Ax * t*t*t + Bx * t*t + Cx * t + Dx - x;
				ya = Ay * t*t*t + By * t*t + Cy * t + Dy - y;
				ld = ya * ya + xa * xa;//�ж��ĸ��������
				if (ld<lnd)
				{
					tn = k;
					w = t;
					lnd = ld;
				}
			}
		}
		//������,�п���û�н���(��Ϊbz�ı߽��ж�)
		if (tn == -1)//ʧЧ
			return 0;
		std::tie(Ax, Bx, Cx, Dx, Ay, By, Cy, Dy) = shapes[B].funcsBz[tn];
		//��
		dx = 3 * Ax*t*t + 2 * Bx*t + Cx;
		dy = 3 * Ay*t*t + 2 * By*t + Cy;
	}
	break;
	default:
		return 0;
	}
	p = shapes[B].transOthPt(dx, dy);//��dx,dy���ԭ����
	dx = p.first; dy = p.second;
	p = reflectLine(dx, dy, bx, by);
	*ptvx = p.first;
	*ptvy = p.second;
	*ptr = atan2f(p.second, p.first);
	return 1;//ִ�гɹ�
}
