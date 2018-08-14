# INS_PLUS
more ins in th15

##被修改的exe部分:
	;0048CB06
	00429276


###区段:.BUF	(00526000)
	ins跳转表	00526000-00526500;给dll写入
	{
		00526000	INS_Switch
		00526004	switchDefault
		;如果自定义dll,则SWITCH_DEFAULT和SWITCH类似,当有自定义的ins代码需要写(即00526004有值)时调用,调用规则同SWITCH
		;SWITCH语句将push以下的值以接受:
		;eax:ins号,esi:当前执行到的地方的指针,edx:当前单位的指针,edi:obj内一个值,很重要.jpg
		;示例:
		;bool switch(int eax,int esi,int edx,int edi){if(成功)retern 1;else return 0;}
		;如果成功匹配则返回1,否则返回0
		;;00526400	断点的结构储存地址(ins_2160使用)
	}
	错误指示位	00526510-00526520;暂定
	
	缓存区		00526600-00526FFC;暂定

	
	
###区段:.TXT	(00525000)	;包涵写入dll不方便的语句
00525000	SWITCH		增强的INS_Switch功能


###dll:INS_PLUS.dll

	2000系:计算补充
	2100系:指针运算,内嵌asm
	2200系:字符串调整
	2300系:自机调控
	2400系:单位调控
	2500系:弹幕调控&图形处理
	2600系:
##2000:
~	2000 int n=1:
		castArgumentInStackToInt()	cstArgStS
		将栈顶下数第n个数强转换成int格式,无变量则修改栈顶
~	2001 int n=1:
		castArgumentInStackToFloat()	cstArgStF
		将栈顶下数第n个数强转换成float格式,无变量则修改栈顶
~	2002 int n=1:
		changeFlagOfArgumentInStackToInt()	chgFlgArgStS
		将栈顶下数第n个数的flag改为int而不影响dw值,无变量则修改栈顶
~	2003 int n=1:
		changeFlagOfArgumentInStackToFloat()	chgFlgArgStF
		将栈顶下数第n个数的flag改为float而不影响dw值,无变量则修改栈顶
~	2004 :
		pushESP()	pushESP
		将栈顶偏移量压入堆栈顶
~	2005 int n:
		copyAndPush()	cpPush
		将[-n]复制一遍压入栈顶
~	2006 int n:
		moveArgumentInStackToCESPAndPop()	movStArgPop
		将栈顶下数第n个数修改成栈顶的那个数并弹出[-1],类型转为[-n]的
~	2007 int n:
		subESP()	subESP
		将栈顶向下移动n个单位(1单位=8byte),负数为上移
~	2013 :
		opRadianToDegree() opRadDeg
		栈顶=[-1]*180/pi
~	2014 :
		opDegreeToRadian() opDegRad
		栈顶=[-1]*pi/180
~	2015 :
		operatorLn()	opLn
		栈顶=ln([-1.0f])
~	2016 :
		operatorLog()	opLog
		栈顶=log([-1.0f])(log(2,[-1.0f]))
~	2017 :
		operatorPowFloat()	opPowF
		栈顶=pow([-2.0f],[-1.0f])
~	2018 :
		operatorPowInt()	opPowS
		(快速幂)栈顶=pow([-2],[-1])
~	2019 :
		operatorSgnInt()	opSgnS
		栈顶=sgn([-1])
~	2020 :
		operatorSgnFloat()	opSgnF
		栈顶=sgn([-1.0f])
~	2021 :
		operatorTan()	opTan
		栈顶=tan([-1.0f])
~	2022 :
		operatorAsin()	opAsin
		栈顶=arcsin([-1.0f])
~	2023 :
		operatorAcos()	opAcos
		栈顶=arccos([-1.0f])
~	2024 :
		operatorAtan()	opaTan
		栈顶=arctan([-1.0f])
~	2025 float x,float y:
		pushAtan2()	pushAtan2
		push atan2(x,y),需要注意参数是正的,即很多情况下=atan(y/x)
~	2026:
		opSar()	opSar
		栈顶=[-2]>>[-1](不改变栈顶flag)
~	2027:
		opShr()		
		栈顶=[-2]>>>[-1](不改变栈顶flag)(逻辑右移)
~	2028:
		opShl()	opShl
		栈顶=[-2]<<[-1](不改变栈顶flag)
~	2029 dw x:
		pushLow()	pushL
		push入x的低4字节
~	2030 dw x:
		pushHigh()	pushH
		push入x的高4字节
~	2031 :
		opModFloat()	opModF
		栈顶=[-2.0f]%[-1.0f](浮点数取余,负数会先改为正数)
~	2032 :
		opAbsInt()	opAbsS()
		栈顶=abs([-1])
~	2033 :
		opAbsFloat()	opAbsF()
		栈顶=abs([-1.0f])
~	2034 float& result,float x,int type,float progress
		translateArgument()	transArg
		进行type类型的变换,变换完成度为%progress,该变换与ins_405中关于时间的变换相同,result存储结果(也就是result=x*TYPE(progress),TYPE为变换的函数)
~	2035 int n:
		compareSaveInt()	cmpSvS
		n大于零,保留[-1],[-2]中的较大数,小于0保留较小数
~	2036 int n:
		compareSaveFloat()	cmpSvF
		n大于零,保留[-1.0f],[-2.0f]中的较大数,小于0保留较小数
~	2037 :
		castBoolEX()	cstBlEX	
		将[-1]:{0,!0}映射成{-1,1}(如果[-1]为0则[-1]=-1,[-1]不为0则为1,flag强制变为int)
~	2038 int a int b:
		castBoolIntEx()	cstBlPHS
		将[-1]:{0,!0}映射成{a,b},a,b均为int
~	2039 float a float b:
		castBoolFloatEx()	cstBlPHF
		将[-1]:{0,!0}映射成{a,b},a,b均为float
	;注:凡有关vector的计算都按float计算,默认从栈顶向下先y后x,复数先虚部后实部
~	2050 :
		opVectorAdd()	opVecAdd
		将堆栈中两个向量相加
~	2051 :
		opVectorSub()	opVecSub
		将堆栈中两个向量相减
~	2052 :
		opVectorScalarMul()	opVecSclMul
		向量的数乘,[-1.0f]为数,[-2.0f],[-3.0f]为向量
~	2053 :
		opVectorScalarDiv()	opVecSclDiv
		向量的数除,[-1.0f]为数,[-2.0f],[-3.0f]为向量
~	2054 :
		opVectorDot()	opVecDot
		向量的点积
~	2055 :
		opVectorCross()	opVecCrs
		向量的叉积
~	2056 :
		opVectorModulus()	opVecMod
		向量的模长
~	2057 :
		opVectorAngle()	opVecAng
		向量的角度,类似于atan2(ins2025)([-2.0f],[-1.0f])
~	2058 :
		opVectorNeg()	opVecNeg
		将vector取反
~	2059 :
		opVectorStandardization()	opVecStd
		将vector转换成单位向量
~	2060 :
		opVectorEqual()	opVecEql
		判断两向量是否相等,有一定的误差允许
~	2061 :
		opVectorTranslation()	opVecTrans
		将向量的x和y颠倒
~	2062 :
		opVectorRotate()	opVecRot
		将向量(x,y)([-3.0f],[-2.0f])顺时针旋转[-1.0f]弧度
~	2063 :
		opVectorToPol()	opVecToPol
		将vector(x,y)转换成(θ,r)的格式
~	2064 :
		opVectorToRec()	opVecToRec
		与2062相反
~	2070 :
		opComplexRealPart()	opCpxReP
		获得复数的实部,注意这里不仅仅是减少esp,同时也将类型修改为了float
~	2071 :
		opComplexImaginaryPart()	opCpxImP
		获得复数的虚部
~	2072 :
		opComplexConjugate()	opCpxConj
		获得共轭复数
~	2073 :
		opComplexMul()	opCpxMul
		复数的乘法
~	2074 :
		opComplexDiv()	opCpxDiv
		复数的除法
##2100:
	
~	2100 int* p=(AddressOfStackTop):
		pushDereferencedInt()	pushCntS
		塞入*p到栈顶(不填写则将栈顶替换成*(栈顶))
~	2101 float* p=(AddressOfStackTop):
		pushDereferencedFloat()	pushCntF
		塞入*p到栈顶(不填写则将栈顶替换成*(栈顶))
~	2102 int* p=&[-2]:
		popInt()	popS()
		弹出栈顶的数赋值到*p,确保栈顶为int,否则则将栈顶的数字转换为int存入,如果不指明则按照*[-2]=[-1]来填写,并弹出[-2]
~	2103 float* p=&[-2]:
		popFloat()	popF()
		弹出栈顶的数赋值到*p,确保栈顶为float,否则则将栈顶的数字转换为float存入,如果不指明则按照*[-2]=[-1]来填写,并弹出[-2]
~	2104 void* p=&[-2]:
		popArgument()	popArg
		弹出栈顶的数赋值到*p,不管是否为整数或浮点,如果不指明则按照*[-2]=[-1]来填写,并弹出[-2]
~	2105 DWORD p:
		pushAddressOfArgument()	pushAddrArg
		将p的指针压入堆栈,当作int型处理
~	2106 int n:
		pushAddressOfArgumentInStack() pushAddrStArg
		将[-n]的指针压入堆栈
~	2107 int* p,int sz [sizeof(type p)],int offset
		pushPtrOffsetInt() pushPtrOfsS
		将*[p+offset*sz]压入堆栈
~	2108 float* p,int sz [sizeof(type p)],int offset
		pushPtrOffsetFloat() pushPtrOfsF
		将*[p+offset*sz]压入堆栈
		;例如2106(0x0040000,4,0x1)就是压入0x0040004入栈
~	2109 void* p,int num,dword type,int isAddOffset
		pushMultiArgument()	pushMtArg
		将*p后的num个数(dw)压入堆栈,按照type格式压,type从低位到高位指定是否为float,为1为float,isAddOffset为常量,指定是否在语句块运行完成后将p增加sizeof(dword)*num,(如果p为const则请填0),一次最多压32个值
		;例如:
		;ins_2109(0x0040000,31,0x48800008,1)就是将0x004000到0x004007C这31个变量压入堆栈,且第4,24,28,31个数字为float
~	2110 void* p,int num,dword type
		popMultiArgument()	popMtArg
		与2109相反,将堆栈中的数赋值入*p,不同2109,没有第四个参数
		;例如:
		;ins_2110(0x0040000,31,0x48800008)就是将0x004000到0x004007C这31个变量从堆栈取出,0x004007C为[-1],...[0x004000]为[-31]
~	2111 void &p,int num,dword type
		pushMultiVariable()	pushMtVar
		将&p与之后的变量共num个push入栈,p按照int型输入,即$p,基础类型为type,1 float,0 int
		;例如:var point,point_1, ins_2111(point,2,2)能够将point按int,point_1按float送入
~	2112 void &p,int num,dword type
		pushMultiVariable()	pushMtVar
		将&p与之后的变量共num个pop出栈栈,基础类型为type,1 float,0 int
		;例如:var point,point_1, ins_2112(point,2,2)能够将[-1]赋值point_1,[-2]赋值point
~	2113 int* p,int sz [sizeof(type p)],int offset
		popPtrOffsetInt() popPtrOfsS
		将[-1] pop出 *[p+offset*sz]
~	2114 float* p,int sz [sizeof(type p)],int offset
		popPtrOffsetFloat() popPtrOfsF
		将[-1] pop出 *[p+offset*sz]
		
~	2150 string str1,string str2
		getAPI()	getAPI
		获取str1.dll内的str2 API的地址并塞入堆栈
~	2151 int (*func)(DWORD* _obj,DWORD ptObj)
		callSpecialFunction() callSpecFunc
		call一个特定的函数,为_fastcall类型,ecx=_obj,edx=ptObj
##2200:
	
	
##2300:
~	2300:
		getPlayerKeyState()	getPlKeySt
		push入当前按键状态
		(0x0不按,0x1按z,0x2按x,0x8按shift,0x10上,0x20下,0x40左,0x80右,0x100按esc,0x100按ctrl,0xA00按C(为啥乘了10),0x10000按q,40000按p或home,0x200000按R,0x100000按d,8000000按enter)
~	2306 :
		PlayerSuicide()	plScd
		让自机自杀(无视无敌时间)
~	2307 float x,float y:
		setPlayerPosition()  setPlPos
		将自机坐标修改为(x,y)
~	2308 int t:
		setPlayerInvincibilityTime()  setPlIvcTm
		修改无敌时间为t帧
~	2309 int l:
		setLife()	setPlLf
		修改生命数为l
~	2310 int b:
		setBomb()	setPlBm
		修改Bomb数为b
~	2311 int p:
		setPlayerPower()	setPlP
		修改p数为p
		
##2400:
~	2400 int (*func)(DWORD _obj):
		setSpecialFunction() setSpecFunc
		设置specFunction,相当于一个特定的632,func为特定函数的指针,为fastcall(或thiscall)接受
	
	Msg的list最多容纳200条消息,超过后会自动删除消息链最后的msg(最先发出的)
~	2410 int handleObj,int id,int a,int b,int c,float x,float y,float z:
		postMessage()	pstMsg
		向指定obj发送一个消息,标识符int id,该消息能接受a,b,c,x,y,z最多六个值,前三个使用int型发送,后三个使用float型发送(可以用强制转换,指针等突破),标识符id不能为0(非强制规定)
~	2411 int& id,int& a,int& b,int& c,float& x,float& y,float& z:
		getMessage()	getMsg
		接受发送给该obj的消息,如果当前没有消息则id=0,如果有些消息不用六个值可以将这些值中不用的设为一个常数,就不会读取
~	2412 int& id,int& a,int& b,int& c,float& x,float& y,float& z:
		waitMessage()	wtMsg
		类似2411,但是当没有消息时会回退到上一个ins(该ins必须为没有参数的空ins(占地0x10 byte),直到出现消息
~	2413 int& id,int& a,int& b,int& c,float& x,float& y,float& z:
		popFrontMessage()	popFrtMsg
		直接捕获消息链最前的消息(最后发出的消息),无消息则id=0
~	2414 int& id,int& a,int& b,int& c,float& x,float& y,float& z:
		popMessage()	popMsg
		直接捕获消息链最后的消息(最先发出的消息),无消息则id=0
~	2415 int sdObj,int& id,int& a,int& b,int& c,float& x,float& y,float& z:
		catchMessage()	catchMsg
		捕获由sdObj发出的消息中最先发送的一条,捕获失败则id=0
~	2416:
		clearMessage()	clrMsg
		清除所有消息链里面的消息
~	2417 int id:
		clearMessage()	clrMsg
		清除消息链里向id发送的Msg(可以放在死尸弹的call里面)
~	2418 :
		getSizeMessageList()	getSzMsgLst
		获取消息链的长度

##2500:
	;注:图形中的矩阵并不完善
~	2500 int a
		initShape()	initShp
		将某个图形初始化,a取0-15,如果取-1则全部初始化一遍(如果不合法 不会 报错)
~	2501 int a,int b
		setShapeType()	setShpTp
		设置某图形的样式,0椭圆,1平行四边形,2路径图形,3贝塞尔路径,(对于ins_2512),默认为0
~	2502 int a,float x,float y
		setShape()	setShp
		设置图形的基本形状,对于椭圆圆只有x生效为半径,对于平行四边形为「半」长宽,创建出来之后为一个圆和矩形,需要后续变换才能改变为椭圆,平行四边形
~	2503 int a,float x,float y
		setShapePosition() setShpPos
		将某个图形中心移动到(x,y)
~	2504 int a,float x
		rotateShape()	rotShp
		将某个图形旋转x rad
~	2505 int a,float x,float y
		zoomShape() zmShp
		将某个图形进行缩放
~	2506 int a,float x,float y
		shearShape()	shrShp
		将某个图形进行切变变换,x轴剪切x,y轴剪切y
~	2507 int a,float ix,float iy,float jx,float jy
		setShapeTransform()	setShpTrans
		直接设置图形的变换
~	2508 int a,float ix,float iy,float jx,float jy
		transformShape()	transShp
		将图形进行线性变换
~	2509 int a,float& x,float& y
		getTransfromed()	getTrans
		将(x,y)当作图形的一部分进行变换,返回变换结果
~	2510 int a,float& x,float& y
		getInverseTransfromed()	getInvTrans
		将(x,y)当作图形的一部分进行逆变换,返回变换结果
~	2511 int a,int b
		copyShape()	cpyShp
		将图形b复制到图形a
~	2512 int a,float x,float y
		isInShape()	isInShp
		确定(x,y)是否在某个图形内部(包括边缘),返回0则在外部,1在内部,如果为路径图形,使用转角法判定,如果为贝塞尔曲线则必须已经标准化过,否则进行一次标准化再判定,需要注意贝塞尔曲线的判定较为不精确且速度较慢(需要进行积分),不建议用来创建过于复杂的图形
~	2513 int a,float& xi,float& yi,float& xj,float& yj
		getShapeMatrix()	getShpMat
		返回Shape的矩阵{{xi,yi},{xj,yj}}
~	2514 int a,float& xi,float& yi,float& xj,float& yj
		getShapeMatrixInversion()	getShpMatInv
		返回Shape的矩阵的逆{{Ixi,Iyi},{Ixj,Iyj}}
~	2515 int a,float x,float y
		addPathPoint()	addPthPt
		向路径图形中加入一个新点(x,y),对于一个路径图形,至少需要三个点才能闭合,默认是闭合图形,即不需要输入两次第一个点.
~	2516 int a,float x1,float y1,float x2,float y2,float x3,float y3
		addBezierPoint()	addBzrPt
		向贝塞尔路径中加入point(x1,y1)其入锚点指向(x2,y2),出锚点指向(x3,y3),对于一个贝塞尔图形,至少只要一个点就能视为闭合图形
~	2517 int a
		standardizeBezierShape()	stdBzrShp
		对一个贝塞尔图形进行标准化,即将两个用{(x1,y1),(x2,y2),(x3,y3)}表示的锚点转换为用{{ax,bx,cx,dx}{ay,by,cy,dy}}表示的方程
~	2518 int a
		getEdgeNum()	getEdgNum	
		获得某图形的边数,方形永远输出4,椭圆永远输出1
~	2519 int a,int n,float t
		getPointPosition()	getPtPos
		获得图形第n条边上的t点的坐标,t∈[0,1],n从0开始,如果过大则会进行取模,贝塞尔曲线需要标准化之后才能求(否则则进行一次标准化),先push y,再push x
	
~	2520 int b
		initMask()	initMsk
		对一个遮罩/弹幕场进行初始化,清空信息,默认覆盖整个舞台,b取0-15,b=-1则全部初始化
~	2521 int b,int type,int shapeN
		boolForMask()	boolMsk
		对遮罩b使用shapeN图形进行type类布尔运算,0:取交集,1:取并集,2:取差集(b-shapeN),3:取差集(shapeN-b),4:取补集(shapeN不需要使用),5:对称差,录入时采取引用的措施,即之后修改对应的shape会造成改变,本质上是将shape[N]复制到maskB的vector中去
~	2522 int b,int type,int shapeN
		boolForMaskC()	boolMskC
		同2521,但是录入时采取复制的措施,即修改shape不会造成改变
~	2523 int b,float x,float y
		moveMask()	movMsk
		移动遮罩b到(x,y)
~	2524 int b,float x,float y
		isInMask()	isInMsk
		确定(x,y)是否在遮罩内,1在内部,0在外部
~	2525 int b,float m,float n
		setMaskVirtualPosition()	setMskVrtlPos
		设置遮罩的虚位置,不影响判断是否在遮罩内
	
	;标号:指弹幕第一个变换(序号=0)的第二个值的大小,建议用大数,比如100000以上的数
	;因此处理的时候可以在第一个变换处
	;写mode:-2147483648(0,n,-999999.0f,-999999.0f)
~	2530 int b,int n,int t,int w
		setDanmakuTransformationID()	setDmTransID
		对于弹幕标号为n的弹幕,如果在b遮罩内且变换序号不等于t且小于w则修改其变换序号到t(需要等到当前变换完成之后才有效)
~	2531 int b,int n,int t,int ismw,int mode,int x,int y,float r,float s
		setDanmakuTransformationShort()	setDmTransA
		修改处在b遮罩内标号n弹幕的变换,除了第一个参数之外同ins_609()
~	2532 int b,int n,int t,int ismw,int mode,int x,int y,int w,int z,float r,float s,float m,float n
		setDanmakuTransformationLong()	setDmTransB
		修改处在b遮罩内标号n弹幕的变换,除了第一个参数之外同ins_610()
~	2533 int b,int n,float r
		setDanmakuRotationEX()	setDmRotEX
		修改处在b遮罩内标号n的弹幕,使其朝向遮罩的虚位置(m,n)并再旋转r rad
~	2534 int b,int n
		setDanmakuPositionEX()	setDmPosEX
		修改处在b遮罩内标号n的弹幕,使其位置(x,y)增加遮罩虚位置(m,n)的值
~	2535 int b,int n,float f
		setDanmakuSpeedEX()	setDmSpdEX
		修改处在b遮罩内标号n的弹幕,改变其速度,改变的依据为当前弹幕的速度(vx,vy)乘以遮罩b虚位置(m,n)到弹幕位置(x,y)的距离的平方的倒数乘以f,即(vx,vy)+=f/((m-x)^2+(n-y)^2)*(x-m,y-n)
~	2536 int b,int n,int m
		setDanmakuDynamicID()	setDmDnmcID
		修改b遮罩内标号n的弹幕的标号为m
