#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"

#ifdef piMASS_DEBUG
	/// gpcCDR
	// #define cd (pCD=CDsp.CD())[0]
	/// gpcOBJlnk
	#define OBJsrc (pOBs=(gpcOBJlnk*)OBJ.Ux( INS.y, sizeof(gpcOBJlnk)))[0]
	#define OBJdst (pOBd=(gpcOBJlnk*)OBJ.Ux( INS.z, sizeof(gpcOBJlnk)))[0]
#else
	/// gpcCDR
	// #define cd CDsp.CDR().p_cd[0]
	/// gpcOBJlnk
	#define OBJsrc ((gpcOBJlnk*)OBJ.Ux( INS.y, sizeof(gpcOBJlnk)))[0]
	#define OBJdst ((gpcOBJlnk*)OBJ.Ux( INS.z, sizeof(gpcOBJlnk)))[0]
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

	for( U4 i = 0, n = SCOOP.nASM(); i < n; i++ )
	{
		I4x4& INS = ((I4x4*)SCOOP.vASM.Ux( i, sizeof(INS) ))[0];
		pD = pS = gpsDBGpub;
		if( INS.y >= 0 )
		{
			gpcOBJlnk& src = OBJsrc;
			sx = src.typ;
			pD = pS + src.strASM( pS, pALL, pM0, pL0 ) + 1;
		} else {
			gpmMcpy( pS, pALL+pM0[-INS.y].iMNi, pM0[-INS.y].iMNn );
			pS[pM0[-INS.y].iMNn]=0;
			pD = pS + pM0[-INS.y].iMNn + 1;
			sx = gpeTYP_STR;
		}
		if( INS.z >= 0 )
		{
			gpcOBJlnk& dst = OBJdst;
			dx = dst.typ;
			dst.strASM( pD, pALL, pM0, pL0 );
		} else {
			gpmMcpy( pD, pALL+pM0[-INS.z].iMNi, pM0[-INS.z].iMNn );
			pD[pM0[-INS.z].iMNn]=0;
			dx = gpeTYP_STR;
		}

		/*if( iR != INS.w )
		{
			iR = INS.w;
			pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[gpeOPid_mov], i, gpsTYPsz[sx&0x3], pS, gps68kREG[iR&0xf] );
			iR++;
			pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[gpeOPid_mov], i, gpsTYPsz[dx&0x3], pD, gps68kREG[iR&0xf] );
			iR--;
		}*/
		pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[INS.x], i, gpsTYPsz[sx&0x3], pS, pD );


	}

}
