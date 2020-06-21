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
	for( U4 i = 0, n = SCOOP.nASM(); i < n; i++ )
	{
		U4x4& INS = ((U4x4*)SCOOP.vASM.Ux( i, sizeof(INS) ))[0];
		gpcOBJlnk& src = OBJsrc;
		gpcOBJlnk& dst = OBJdst;
		dx = dst.typ&0x3;
		sx = src.typ&0x3;
		pS = gpsDBGpub;
		pD = pS + src.strASM( pS, pALL, pM0, pL0 ) + 1;
		dst.strASM( pD, pALL, pM0, pL0 );

		pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[INS.x], i, gpsTYPsz[sx], pS, pD );


	}

}
