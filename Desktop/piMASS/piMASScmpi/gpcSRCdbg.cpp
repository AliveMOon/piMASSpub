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
	char	*pSZ, *pS, *pD, *pDe, sx, dx, *pC, *pCe;

	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	U4x4 *pL0 = (U4x4*)SCOOP.lnk.p_alloc;
	char *pALL = (char*)SCOOP.pALL;
	U4 iR = -1;
	I4 pc = 0, aPC[0x10];
	//I8 i8 = 0;
	for( U4 i = 0, n = SCOOP.nASM(); i < n; i++ )
	{
		I4x4& ins = SCOOP.vASM.pINST(i)[0];
		pDe = pS = (pD = (pCe=pC=gpsDBGpub)+0x400)+0x400;
		*pDe = *pD = *pC = 0;
		gpeEA ADRmod;
		U1 Xn, Yn;
		U1x4 opw=ins.op;



		for( U1 i = 0; i < 2; i++ )
		{
			bool bOB = false;
			Xn = i ? opw.z : opw.y;
			Yn = i ? opw.w>>4 : opw.w;
			switch( ADRmod=(gpeEA)(Xn>>3) )
			{
				case gpeEA_Dn:
				case gpeEA_An:
				case gpeEA_IAnI:
				case gpeEA_IAnIp:
				case gpeEA_sIAnI:
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], Xn&7 )+1;
					break;
				case gpeEA_d16IAnI:
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i], Xn&7 )+1;
					break;
				case gpeEA_d16IAnDnI:
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i], Xn&7, gps68kREG[Yn&0xf] )+1;
					bOB = true;
					break;
				case gpeEA_d16IPcI:
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i] )+1;
					bOB = true;
					break;
				case gpeEA_d16IPcDnI:
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i], gps68kREG[Yn&0xf] )+1;
					bOB = true;
					break;
				case gpeEA_num:
				default:
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i] )+1;
					bOB = true;
					break;
			}

			if( bOB )
			{
				I4 o = ins.aOB[i];
				if( o >= 0 )
				{
					gpcOBJlnk& obj = ((gpcOBJlnk*)OBJ.Ux( o, sizeof(gpcOBJlnk)))[0];
					pCe += obj.strASM( pCe, pALL, pM0, pL0 ) + 1;
				} else {
					o *= -1;
					gpmMcpy( pCe, pALL+pM0[o].iMNi, pM0[o].iMNn );
					pS[pM0[o].iMNn]=0;
					pCe += pM0[o].iMNn + 1;
				}
			}

			if(i)
				continue;
			pD = pDe;
			*pD = 0;
		}

		switch( U1 pst = ins.op&0xff )
		{
			case gpeOPid_dot:
				pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[pst], pc );
				pc++;
				break;

			default:
				pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[pst], pc, gpsTYPsz[sx&0x3], pS, pD, *pC?";":"", pC );
				pc++;
				break;
		}
	}
}
