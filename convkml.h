#ifndef CONVKML_H

#define CONVKML_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

/* constants -----------------------------------------------------------------*/

#define SIZP     1.2            /* mark size of rover positions */
#define SIZR     1.8            /* mark size of reference position */
#define TINT     30.0           /* time label interval (sec) */
#define MAXNUM   128           /* maxnum of arry */
#define HEADKML1 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
#define HEADKML2 "<kml xmlns=\"http://earth.google.com/kml/2.1\">"
#define MARKICON "http://maps.google.com/mapfiles/kml/pal2/icon18.png"

/* struct --------------------------------------------------------------------*/
typedef struct {
	char name[MAXNUM];
	double pos[3]; /* coordnates: B,L,H */
}point_t;

typedef struct{
	int n,nmax;
	point_t *poi;
}points_t;

/* function ------------------------------------------------------------------*/
extern int convkml(const char *infile, const char *outfile,	int tcolor, 
	int pcolor,int ddmmss);
extern void MyBreak(const char *format, ...);
extern double dms2deg(const double *dms);


#endif