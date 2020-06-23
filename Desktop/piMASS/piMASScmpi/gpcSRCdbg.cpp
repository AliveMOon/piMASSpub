#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"

#ifdef piMASS_DEBUG
	/// gpcCDR
	// #define cd (pCD=CDsp.CD())[0]
	/// gpcOBJlnk
	#define OBJsrc (pOBs=(gpcOBJlnk*)OBJ.Ux( si, sizeof(gpcOBJlnk)))[0]
	#define OBJdst (pOBd=(gpcOBJlnk*)OBJ.Ux( di, sizeof(gpcOBJlnk)))[0]
#else
	/// gpcCDR
	// #define cd CDsp.CDR().p_cd[0]
	/// gpcOBJlnk
	#define OBJsrc ((gpcOBJlnk*)OBJ.Ux( si, sizeof(gpcOBJlnk)))[0]
	#define OBJdst ((gpcOBJlnk*)OBJ.Ux( di, sizeof(gpcOBJlnk)))[0]
#endif
#define aaOPid gpaaOPid[lnk.y]

static char gpsDBGpub[0x1000];
static const char* gpsTYPsz[] = {
	".b", //0 1
	".w", //1 2
	".l", //2 4
	".q", //3 8
	".x", //4 16
};
static const char* gps68kREG[] = {
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7",
};
static const char* gps68kR[] = {
	"D%d",
	"A%d",

	"(A%d)",
	"(A%d)+",

	"-(A%d)",
	"0x%0.4x(A%d)",
	"0x%0.2x(A%d,%s)",
	"0x%0.4x.w",
	"0x%0.8x.l",
	"0x%0.8x",	//direct data
	"0x%0.4x(PC)",
	"0x%0.2x(PC,%s)",
};
void gpcSRC::SRCmnMILLdbg( I8x2* pOP, gpcLZYdct& dOP, U1 iMN )
{
	if( !this )
		return;
	pDBG->lzyRST();
	if( !SCOOP.nASM() )
		return;
	U4 iOPe = dOP.nIX();
#ifdef piMASS_DEBUG
	gpcOBJlnk	*pOBs, *pOBd;
#endif
	U8 strt = -1;
	char	*pSZ, *pS, *pD, sx, dx;

	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	U4x4 *pL0 = (U4x4*)SCOOP.lnk.p_alloc;
	char *pALL = (char*)SCOOP.pALL;
	U4 iR = -1;
	I4 pc = 0, aPC[0x10];
	for( U4 i = 0, n = SCOOP.nASM(); i < n; i++ )
	{
		I4x4& INS = SCOOP.vASM.pINST(i)[0];
		I4	si = INS.aOB[0],
			di = INS.aOB[1];
		pD = pS = gpsDBGpub;
		sx = dx = 0;
		if( !(INS.msk&0x80) )
		{
		    pD = pS+sprintf( pS, gps68kR[(INS.msk&0x38)>>3], INS.msk&0x7, "D0" )+1;
			//pS = (char*)gps68kREG[INS.msk&0xf];
		}
		else if( si >= 0 )
		{
			gpcOBJlnk& src = OBJsrc;
			sx = src.typ;
			pD = pS + src.strASM( pS, pALL, pM0, pL0 ) + 1;
		} else {
			gpmMcpy( pS, pALL+pM0[-si].iMNi, pM0[-si].iMNn );
			pS[pM0[-si].iMNn]=0;
			pD = pS + pM0[-si].iMNn + 1;
			sx = gpeTYP_STR;
		}
		if( !(INS.msk&0x8000) )
		{
			sprintf( pD, gps68kR[(INS.msk&0x3800)>>11], (INS.msk&0x700)>>8, "D0" );
			//pD = (char*)gps68kREG[(INS.msk&0xf00)>>8];
		}
		else if( di >= 0 )
		{
			gpcOBJlnk& dst = OBJdst;
			dx = dst.typ;
			dst.strASM( pD, pALL, pM0, pL0 );
		} else {
			gpmMcpy( pD, pALL+pM0[-di].iMNi, pM0[-di].iMNn );
			pD[pM0[-di].iMNn]=0;
			dx = gpeTYP_STR;
		}

		switch( INS.pst )
		{
			case gpeOPid_dot:
				for( U4 i = 0; i < 2; i++){ aPC[i] = pc++; }
				pDBG = pDBG->lzyFRMT( strt=-1,
											gpas68k[gpeOPid_dot],
											aPC[0], gpsTYPsz[sx&0x3], si, pS,
											aPC[1]
											//aPC[2], gpsTYPsz[dx&0x3], di, pD,
											//aPC[3]
									);
				break;
			default:
				pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[INS.pst], pc, gpsTYPsz[sx&0x3], pS, pD );
				pc++;
				break;
		}


	}

}
