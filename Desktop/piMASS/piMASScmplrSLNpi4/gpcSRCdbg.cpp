#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"

#ifdef piMASS_DEBUG
	/// gpcCDR
	// #define cd (pCD=CDsp.CD())[0]
	/// gpCNST
	#define OBJsrc (pOBs=(gpCNST*)OBJ.Ux( si, sizeof(gpCNST)))[0]
	#define OBJdst (pOBd=(gpCNST*)OBJ.Ux( di, sizeof(gpCNST)))[0]
#else
	/// gpcCDR
	// #define cd CDsp.CDR().p_cd[0]
	/// gpCNST
	#define OBJsrc ((gpCNST*)OBJ.Ux( si, sizeof(gpCNST)))[0]
	#define OBJdst ((gpCNST*)OBJ.Ux( di, sizeof(gpCNST)))[0]
#endif
#define aaOPid gpaaOPid[lnk.y]

static char gpsDBGpub[0x1000];

void gpcSRC::srcDBG( gpcLZYdct& dOP, U1 iSCP )
{
	if( !this )
		return;
	pDBG->lzyRST();
	if( !gpmSCP.nASM() )
		return;
	//U4 iOPe = dOP.nIX();
#ifdef stdOFF
	std::cout << stdDBG " DBG" stdRESET << std::endl;
#endif // stdOFF

#ifdef piMASS_DEBUG
	gpCNST	*pOBs, *pOBd;
#endif
	U8 strt = -1;
	char	*pSZ, *pS, *pD, *pDe,
			*pC, *pCe;
	//gpeEAsz aSZ[2];

	I8x4 *pM0 = gpmSCP.pMN();
	U4x4 *pL0 = gpmSCP.pLNK(); //lzyLNK.p_alloc;
	char *pALL = (char*)gpmSCP.p_str;
	U4 iR = -1;
	I4 pc = 0, aPC[0x10];
	//I8 i8 = 0;
	for( U4 i = 0, n = gpmSCP.nASM(); i < n; i++ )
	{
		I4x4& ins = gpmSCP.lzyASM.pINST(i)[0];
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
					pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i] )+1;
					bOB = true;
					break;
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
					gpCNST& obj = ((gpCNST*)scpCNST.Ux( o, sizeof(gpCNST)))[0];
					pCe += obj.strASM( pCe, pALL, pM0, pL0 ) + 1;
				} else {
					o *= -1;
					gpmMcpy( pCe, pALL+pM0[o].iMNi, pM0[o].iMNn );
					pCe += pM0[o].iMNn;
					*pCe = 0;
					pCe++;
				}
			}

			if(i)
				continue;
			pD = pDe;
			*pD = 0;
		}

		switch( U1 pst = (ins.op&0xff) )
		{
			case gpeOPid_nop:
			case gpeOPid_dot:
				pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[pst], pc );
				pc++;
				break;
			case gpeOPid_jsr:
				pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[pst], pc, *pC?pC:"?" );
				pc++;
				break;
			default:
				pDBG = pDBG->lzyFRMT( strt=-1, gpas68k[pst], pc, gpasCsz[ins.cID], pS, pD, *pC?";":"", pC );
				pc++;
				break;
		}
	}
}
