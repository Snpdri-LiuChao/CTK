
#include "stdafx.h"
#include "convkml.h"


//************************************
// Method:    MyBreak
// FullName:  MyBreak
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const char * format
// Parameter: ...
//************************************
extern void MyBreak(const char *format, ...)
{
	va_list ap;
	char buffer[1024];

	va_start(ap,format);
	vsprintf_s(buffer,format,ap);
	va_end(ap);
#ifdef VC_EXTRALEAN
	AfxMessageBox((LPCTSTR)buffer);
#else
	printf(buffer);
	getchar();
#endif	
}

/* convert deg-min-sec to degree -----------------------------------------------
* convert degree-minute-second to degree
* args   : double *dms      I   degree-minute-second {deg,min,sec}
* return : degree
*-----------------------------------------------------------------------------*/
extern double dms2deg(const double *dms)
{
	double sign=dms[0]<0.0?-1.0:1.0;
	return sign*(fabs(dms[0])+dms[1]/60.0+dms[2]/3600.0);
}

static double decode_BL(CString str, int ddmmss)
{
	double dms[3]={0.0};
	CString dd,mm,ss;
	int len = -1;
	int dindex = -1;
	int mindex = -1; // index for dms

	len=str.GetLength();

/*--------------------------------------------
* 2017.10.9修改
* 1、增加三种经纬度表达形式： 0- DD:MM:SS.SS
*							   1- DD.DDDD
*							   2- DDMMSSSS	
*---------------------------------------------*/
	switch (ddmmss)
	{
	case 0:{//0- DD:MM:SS.SS		
		//检查格式
		if (-1 == (dindex = str.Find(':'))) {
			MyBreak("经纬度格式(DD:MM:SS.SS)错误:%s", str); 
			break; return 0;
		}

		mindex = str.ReverseFind(':');
		dd = str.Left(dindex);
		mm = str.Mid(dindex + 1, mindex - dindex - 1);
		ss = str.Right(len - mindex - 1);

		dms[0] = atof(dd);
		dms[1] = atof(mm);
		dms[2] = atof(ss);

		return dms2deg(dms);
	}
	case 1:{//1- DD.DDDD
		//检查格式		
		if (-1 == (dindex = str.Find('.'))) {
			MyBreak("经纬度格式(DD.DDDD)错误:%s", str); 
			break; return 0;
		}

		dms[0] = atof(str);
		dms[1] = 0;
		dms[2] = 0;

		return dms2deg(dms);
	}
	case 2:{//2- DD.MMSSSS
		//检查格式
		if (-1 == (dindex = str.Find('.'))) {
			MyBreak("经纬度格式(DD.MMSSSS)错误:%s", str); 
			break; return 0;
		}

		dd = str.Left(dindex);
		mm = str.Mid(dindex+1, 2);
		ss = str.Right(len- dindex-3);

		dms[0] = atof(dd);
		//检查数据正确性：mm
		if ((60 - (dms[1] = atof(mm))) > 0 ) {
			//不执行任何语句
		}
		else {
			MyBreak("请检查数据格式（DD.MMSSSS）或者数值:%s", str); 
			break; return 0;
		}
		//检查数据正确性：ssss
		len = ss.GetLength();
		double k = 1.0;
		int i;

		for (i = 0; i < (len - 2); i++) {
			k = k*10.0;
		}

		if ((60 - (dms[2] = atof(ss) / k)) > 0) {
			//不执行任何语句
			
		}		
		else {
			MyBreak("请检查数据格式（DD.MMSSSS）或者数值:%s", str); 
			break; return 0;
		}
		return dms2deg(dms);
	}
	}
	return 0;
}

static char findfalg(CString &str)
{
	char flag=' ';
	int index = -1;

	if(-1 != str.Find('N')) flag = str.GetAt((index=str.Find('N')));
	if(-1 != str.Find('S')) flag = str.GetAt((index=str.Find('S')));
	if(-1 != str.Find('W')) flag = str.GetAt((index=str.Find('W')));
	if(-1 != str.Find('E')) flag = str.GetAt((index=str.Find('E')));

	if(-1 != index) str.SetAt(index,'\0');

	return flag;
}

/* static int decodeBLH(CString str, const char flag, double *pos ,int ddmmss)----------------

* 参数：CString str     : 经度 或 纬度 或 高程
		const char flag ：经纬度及高程标识
		double *pos     ：点位数据（存储经纬度及高程）
		int ddmmss      ：角度格式（0- DD:MM:SS.SS; 1- DD.DDDD; 2- DD.MMSSSS ）

* 功能：解析经纬度及高程，返回整型参数。

* 历史：1、Date：2014/05/24 Created by liuchao
		2、Date：2017/10/10 Changed by liuchao
				 增加数值判断，若解析值出来为0，则判断错误

* 已知Bug：1、仅仅用值为零来判断解析正确与否，容易使0°经线和纬线上的点不能进行正常转换。

----------------------------------------------------------------------------------------------*/
static int decodeBLH(CString str, const char flag, double *pos ,int ddmmss)
{
	switch(flag){
	case 'N': { 		
		if (0 == (pos[1] = decode_BL(str, ddmmss)*1.0)){ 
			break; return 0;
		}
		else return 1;
	}

	case 'S': {
		if (0 == (pos[1] = decode_BL(str, ddmmss)*(-1.0))){
			break; return 0;
		}
		else return 1;
	}

	case 'E': {
		if (0 == (pos[0] = decode_BL(str, ddmmss)*1.0)){
			break; return 0;
		}
		else return 1;
	}

	case 'W': { 
		if (0 == (pos[0] = decode_BL(str, ddmmss)*(-1.0))){
			break; return 0;
		}
		else return 1;
	}

	default: {  pos[2] = atof(str);  return 1; }
	}
	return 0;
}

/* add point data --------------------------------------------------------------*/
static int addpoidata(points_t *pois, const point_t *poi)
{
	point_t *poi_data;

	if (pois->nmax <= pois->n ) {
		if (pois->nmax<=0) pois->nmax=1024; else pois->nmax*=2;

		if (!(poi_data=(point_t*)realloc(pois->poi,sizeof(point_t)*pois->nmax))) {
			MyBreak("addpoidata: memalloc error n=%dx%d\n",sizeof(point_t),pois->nmax);
			free(pois->poi); pois->poi=NULL; pois->n=pois->nmax=0;

			return -1;
		}
		pois->poi=poi_data;
	}
	pois->poi[pois->n++]=*poi;

	return 1;
}

/* static int decodepoi( CString linestr,point_t *poi, int formate ,int ddmmss)----------------

* 参数：CString linestr : 文件行字符串
		point_t *poi    ：点位保存结构体
		int formate     ：ESWN 格式开关  //2017.10.10 作废
		int ddmmss      ：角度格式（0- DD:MM:SS.SS; 1- DD.DDDD; 2- DD.MMSSSS ）

* 功能：解析坐标，返回整型参数。

* 历史：1、Date：2014/05/24 Created by liuchao
		2、Date：2017/10/10 Changed by liuchao
				 增加 bool H_Flag 用于标记是否含有高程列
				 利用 findflag 函数替代 formate 变量
				 
* 已知Bug：
----------------------------------------------------------------------------------------------*/
static int decodepoi( CString linestr,point_t *poi ,int ddmmss)
{
	int index=-1,len,i;

	bool H_Flag = FALSE;			//是否含有高程列的标识：FALSE- 无；TRUE- 有。

	CString str;
	
	//将字符串中所有字母大写，并获取长度
	linestr.MakeUpper();
	len  = linestr.GetLength();

	/* 解析点名 */	
	index= linestr.Find(',');

	if ( -1 == index )  { 
		MyBreak("Input File Formate Error:\n%s",linestr); return FALSE;
	}

	str = linestr.Left(index);

	for ( i=0; i<str.GetLength(); i++) 	poi->name[i]=str[i];

	poi->name[i]='\0';

	/* 解析坐标 */
	
	//解析纬度：B	
	//获取纬度
	linestr = linestr.Right(len-index-1);
	index = linestr.Find(',');
	str = linestr.Left(index);

	if (' ' == findfalg(str)) {
		if(!decodeBLH(str,'N',poi->pos,ddmmss)) {
			MyBreak("decodeBLH failed:%s - B",poi->name);
			return FALSE;
		}
	}
	else{		
		if(!decodeBLH(str,findfalg(str),poi->pos,ddmmss)) {
			MyBreak("decodeBLH failed:%s - %s ",poi->name,findfalg(str));
			return FALSE;
		}
	}

	
	//解析经度：L		
	//获取经度
	len = linestr.GetLength();
	linestr = linestr.Right(len-index-1);
	
	//根据逗号索引判断str的赋值，并标记 H_Flag 用于高程读取
	/* ----------------------------------------------------------------------
	*  经度后面还有高程列则应该还有逗号‘，’，如果没有，则没有逗号。
	* ---------------------------------------------------------------------*/
	if (-1 == (index = linestr.Find(',')))	{ str = linestr; H_Flag = FALSE; }
	else { str = linestr.Left(index); H_Flag = TRUE; }


	if (' ' == findfalg(str)) {
		if (!decodeBLH(str, 'E', poi->pos, ddmmss)) {
			MyBreak("decodeBLH failed:%s - L",poi->name);
			return FALSE;
		}
	}
	else{
		if(!decodeBLH(str,findfalg(str),poi->pos,ddmmss)) {
			MyBreak("decodeBLH failed:%s - %s ",poi->name,findfalg(str));
			return FALSE;
		}
	}

	/* 解析高程：H */
	if (!H_Flag) { poi->pos[2] = 0.0; return TRUE; }  // 无高程数据

	else{//根据逗号判断有高程数据，获取高程数据
		len = linestr.GetLength();
		linestr = linestr.Right(len - index - 1);
		str = linestr;

		//考虑虽有逗号但是没有高程数据
		if ("" == str) { poi->pos[2] = 0.0; return TRUE; }
		else{
			if (!decodeBLH(str, findfalg(str), poi->pos, ddmmss)) {
				MyBreak("decodeBLH failed:%s - %s ", poi->name, findfalg(str));
				return FALSE;
			}
		}
		/*
		//有高程数据
		if (' ' == findfalg(str)) {
			if (!decodeBLH(str, ' ', poi->pos, ddmmss)) {
				MyBreak("decodeBLH failed:%s - H", poi->name);
				return FALSE;
			}
		}
		else{
			if (!decodeBLH(str, findfalg(str), poi->pos, ddmmss)) {
				MyBreak("decodeBLH failed:%s - %s ", poi->name, findfalg(str));
				return FALSE;
			}
		}*/
	}

	return TRUE;
}
/* static int readfile(const char *infile, points_t *pois, int formate ,int ddmmss)-------------

* 参数：const char *infile : 坐标原始文件
        points_t *pois     ：点位保存结构体
	    int formate        ：ESWN 格式开关  //2017.10.10 作废
	    int ddmmss         ：角度格式（0- DD:MM:SS.SS; 1- DD.DDDD; 2- DD.MMSSSS ）
 

* 功能：读取文件，返回整型参数stat

* 历史：1、Date：2014/05/24 Created by liuchao
        2、Date：2013/12/31 Changed by liuchao
	             Fixed the bug: Recieve blank
        3、Date：2017/10/10 Changed by liuchao
	             增加 int count 用于记录 points_t *pois 数目
				 移除读取文件字符串 linestr 中的空格
				 Fixed bug: 参数 n 记录文件行数不正确的问题
* 已知Bug：
----------------------------------------------------------------------------------------------*/
static int readfile(const char *infile, points_t *pois ,int ddmmss)
{   
	CStdioFile CSFin;	

	CString linestr;

	int count = 0;				// pois数目
	int i,n = 0, stat = 0;
	
	point_t *poi;

	//预先给 poi 分配空间
	if ( !(poi=(point_t *)malloc(sizeof(point_t)*1024)) ) return 0;

	// CSFin 以只读模式打开输入文件 infile
	if ( !CSFin.Open(infile,CFile::modeRead) ){ MyBreak("file open error:%s",infile); return 0;}

	//按行读取文件
	while ( CSFin.ReadString(linestr) )	{

		//2017.10.10 changed by liuchao：移除linestr中的空格
		linestr.Remove(' ');

		//跳过“#”注释行
		if ( 0 == linestr.Find('#') ) { n++; continue; }

		/* Date: 2013.12.31; Fixed the bug: Recieve blank*/
		if ( "" == linestr ) { n++; continue;	}

		//解析坐标
		if ( !decodepoi(linestr,poi+count,ddmmss)){
			MyBreak("decodepoi error: line %d",n+1);
			break;
		}
		n++; count++;
	}

	for (i=0;i<count;i++){
		/* save point */
		if ( (stat=addpoidata(pois,poi+i))<0 ) break;	
	}
	
	free (poi);
	CSFin.Close();

	return stat;
}

/* output track --------------------------------------------------------------
* Date：2017/10/10 Changed by liuchao
        将 pois->poi[i].pos[2]*0.0 中的 0.0 去掉
----------------------------------------------------------------------------*/

static void outtrack(FILE *f, const points_t *pois, const char *color)
{
	int i;

	fprintf(f,"<Placemark>\n");
	fprintf(f,"<name>Rover Track</name>\n");
	fprintf(f,"<Style>\n");
	fprintf(f,"<LineStyle>\n");
	fprintf(f,"<color>%s</color>\n",color);
	fprintf(f,"</LineStyle>\n");
	fprintf(f,"</Style>\n");
	fprintf(f,"<LineString>\n");
	fprintf(f,"<coordinates>\n");
	for (i=0;i<pois->n;i++) {
		fprintf(f,"%.9f,%.9f,%.3f\n",pois->poi[i].pos[0],pois->poi[i].pos[1],
			pois->poi[i].pos[2]);//将 pois->poi[i].pos[2]*0.0 中的 0.0 去掉
	}
	fprintf(f,"</coordinates>\n");
	fprintf(f,"</LineString>\n");
	fprintf(f,"</Placemark>\n");
}

/* output point --------------------------------------------------------------
将 pois->poi[i].pos[2] * 0.0 中的 0.0 去掉
----------------------------------------------------------------------------*/
static void outpoint(FILE *fp,const double *pos, const char *label, int style)
{
	fprintf(fp,"<Placemark>\n");
	if (*label) fprintf(fp,"<name>%s</name>\n",label);
	fprintf(fp,"<styleUrl>#P%d</styleUrl>\n",style);
	
	fprintf(fp,"<Point>\n");
	fprintf(fp,"<coordinates>%.9f,%.9f,%.3f</coordinates>\n",pos[0],
		pos[1],pos[2]);//将 pois->poi[i].pos[2]*0.0 中的 0.0 去掉
	fprintf(fp,"</Point>\n");
	fprintf(fp,"</Placemark>\n");
}
/* save kml file -------------------------------------------------------------*/
static int savekml(const char *outfile, const points_t *pois, int tcolor, int pcolor)
{
	FILE *fp_out;
	int i,qcolor[]={0,1,2,5,4,3,0};
	char *color[]={
		"ffffffff","ff008800","ff00aaff","ff0000ff","ff00ffff","ffff00ff"
	};
	
	if (!(fp_out=fopen(outfile,"w+"))) {
		MyBreak("file open error : %s\n",outfile);
		return 0;
	}
	
	fprintf(fp_out,"%s\n%s\n",HEADKML1,HEADKML2);
	fprintf(fp_out,"<Document>\n");
	for (i=0;i<6;i++) {
		fprintf(fp_out,"<Style id=\"P%d\">\n",i);
		fprintf(fp_out,"  <IconStyle>\n");
		fprintf(fp_out,"    <color>%s</color>\n",color[i]);
		fprintf(fp_out,"    <scale>%.1f</scale>\n",i==0?SIZR:SIZP);
		fprintf(fp_out,"    <Icon><href>%s</href></Icon>\n",MARKICON);
		fprintf(fp_out,"  </IconStyle>\n");
		fprintf(fp_out,"</Style>\n");
	}

	if (tcolor>0) {
		outtrack(fp_out,pois,color[tcolor-1]);
	}

	if (pcolor>0) {
		fprintf(fp_out,"<Folder>\n");
		fprintf(fp_out,"  <name>Rover Position</name>\n");
		for (i=0;i<pois->n;i++) {
			outpoint(fp_out,pois->poi[i].pos,pois->poi[i].name,pcolor-1);
		}
		fprintf(fp_out,"</Folder>\n");
	}

	fprintf(fp_out,"</Document>\n");
	fprintf(fp_out,"</kml>\n");
	fclose(fp_out);
	return 1;
}

extern int convkml(const char *infile, const char *outfile,
	int tcolor, int pcolor, int ddmmss)
{
	points_t pois={0};

	if ( !readfile(infile,&pois,ddmmss) ){
		MyBreak("file read failed: \n%s",infile);
		return 0;
	}

	return savekml(outfile, &pois, tcolor, pcolor);

}
