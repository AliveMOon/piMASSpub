#ifndef GPCSRCLNK_H
#define GPCSRCLNK_H
#include "piMASS.h"

class gpcAx{
public:
	I8x2 	AN;
	U1		typ, op, x, y;
	gpcAx(){};
	gpcAx& null() { gpmCLR; return *this; }
	gpcAx& OP( U1 o ) { op=o; return *this; }
	gpcAx& ZN( I8x2 zn ) { gpmCLR; AN=zn; typ=4; return *this; } // typ=4 ZN coordináta
	U1 an( const char* pS, U4 nS )
	{
		AN.num = nS;
		AN = pS;
		char *pSi = (char*)pS+AN.num, *pSe;
		typ = !!AN.x;
		if( typ ? (AN.num >= nS) : true )
			return typ;

		AN.num = gpfSTR2I8( pSi, &pSe );
		if(pSe > pSi)
			typ |= 2;

		if( pSe-pS < nS )
		if( *pSe == 'x' || *pSe == 'X' )
		{
			// vector
			switch( pSe[1] )
			{
				case '2':
					x = 2;
					break;

				case '3':
				case '4':
					x = 4;
					break;

				case '5':
				case '6':
				case '7':
				case '8':
					x = 8;
					break;

				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					x = 0x10;
					break;

				default:
					x = 1;
					break;
			}
		}


		return typ;
	}

};
#define iA ((U4x2*)Ai.Ux(iiiA,sizeof(U4x2)))
#define iB ((U4x2*)Bi.Ux(iiiB,sizeof(U4x2)))
#define iC ((U4x2*)Ci.Ux(iiiC,sizeof(U4x2)))

#define iAi iA[0].y
#define iBi iB[0].y
#define iCi iC[0].y

#define A ((gpcAx*)Ao.Ux(iA[0].sum(),sizeof(gpcAx)))
#define B ((gpcAx*)Bo.Ux(iB[0].sum(),sizeof(gpcAx)))
#define C ((gpcAx*)Co.Ux(iC[0].sum(),sizeof(gpcAx)))

#endif // GPCSRCLNK_H
