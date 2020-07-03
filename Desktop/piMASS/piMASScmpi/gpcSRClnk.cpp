#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
#define cd (pCD=CDsp.CD())[0]

	/// gpcOBJlnk
#ifdef piMASS_DEBUG
	#define OBJget (pOBi=(gpcOBJlnk*)OBJ.Ux( (cd.lnk-iOPe), sizeof(gpcOBJlnk)))[0]
	#define OBJadd (pOBn=(gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#else
	#define OBJget ((gpcOBJlnk*)OBJ.Ux( (cd.obj-iOPe), sizeof(gpcOBJlnk)))[0]
	#define OBJadd ((gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#endif
#define aaOPid gpaaOPid[OPgrp]
size_t gpcOBJlnk::strASM( char* pS, char* pALL, I8x4 *pM0, U4x4 *pL0   )
{
	char sBstr[] = "FFFFffffFFFFffff",
		*pBD, nB = sizeof(sBstr)-1;
	size_t n;
	switch( typ )
	{
		case gpeTYP_sA8:
			return gpfALF2STR( pS, obj.alf );
		case gpeTYP_sA8N:
			return obj.an2str( pS );
		case gpeTYP_U1:
			return sprintf( pS, "0x%0.2x", obj.uy );
		case gpeTYP_U2:
			return sprintf( pS, "0x%0.4x", obj.uy );
		case gpeTYP_U4:
			return sprintf( pS, "0x%0.8x", obj.uy );
		case gpeTYP_U8:
			return sprintf( pS, "0x%0.16llx", obj.uy );

		case gpeTYP_I1:
			return sprintf( pS, "0x%.3d", obj.num );
		case gpeTYP_I2:
			return sprintf( pS, "0x%.6d", obj.num );
		case gpeTYP_I4:
			return sprintf( pS, "0x%.9d", obj.num );
		case gpeTYP_I8:
			return sprintf( pS, "0x%.12lld", obj.num );

		case gpeTYP_D:
		case gpeTYP_F:
			return sprintf( pS, "%0.7f", obj.dy );
		case gpeTYP_STR:
			n = pM0[obj.uy].iMNn;
			if( n > nB )
				n = nB;
			gpmMcpy( sBstr, pALL+(pM0[obj.uy]).iMNi, n );
			sBstr[n]=0;
			return sprintf( pS, "\"%s...\"", sBstr );
		default:
			n = sprintf( pS, "0x%0.8llx,0x%0.8llx", obj.x, obj.y );
			break;
	}
	return n;
}


void gpcSRC::SRCmnMILLcdr( I8x2* pOP, gpcLZYdct& dOP, U1 iMN )
{
	if( !this )
		return;
	pDBG->lzyRST();

	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc, M, Mnx;
	U4x4 *pL0 = (U4x4*)SCOOP.lnk.p_alloc; //, aLNK[0x10];
	U4 nM = SCOOP.nMN(), iOP, iOPe = dOP.nIX();
	OBJ.lzyRST();
	const char *pS;
	const U1 *pSTR;
	U8 nS, nSTR;
	gpcCDsp CDsp; //, Bsp, Csp;
	gpcCD	*pCD;
#ifdef piMASS_DEBUG

	gpcOBJlnk	*pOBJ;
	gpcOBJlnk	*pOBn, *pOBi;

#endif
	I4x2 aFND[0x40];
	U1 nFND, OPgrp;
	gpeALF opALF;
	I4x4 iZNmx = 0;
	gpeCLR clr;

	//for( U4 le = SCOOP.nLiNK(), l = 0, mNX; l < le; l++ )
	for( U4 nM = SCOOP.nMN(), m = 0, l; m < nM; m++ )
	{
		clr = (gpeCLR)((pM0[m].rMNclr>>0x10)&0xf);
		if( clr == gpeCLR_red2 )
				continue;

		M = pM0[m]; //pM0[link.x];
		pSTR = NULL;
		if( M.iMNtyp == gpeTYP_STR )
		{
			if( clr != gpeCLR_violet )
				continue;

			pSTR = (nSTR=M.iMNn) ? SCOOP.pALL+M.iMNi : NULL; //M.iMNinlt.a4x2[0];

			cd.lnk = -m;
			cd.typ = gpeTYP_STR;
			continue;
		}

		U4x4& lnk = pL0[l=M.iMNlnk];
		// még nem tud ja micsoda kicsoda
		if( !lnk.y )
		{
			pS = SCOOP.dct.sSTRix(l, NULL);
			if( !pS )
				continue;
			nS = SCOOP.dct.nSTRix(l);
			if( !nS )
				continue;
		}


		switch( clr )
		{
			case gpeCLR_blue2: 	///ABC
				//cd.pst = gpeOPid_mul;
				if( lnk.y )
				{
					cd.lnk = lnk.y;
					cd.typ = OBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				cd.lnk = lnk.y = (iOPe+SCOOP.nOBJ);
				cd.typ = OBJadd.typ = OBJadd.obj.cdrMILLalf( pS, nS );
				SCOOP.nOBJ++;
				break;
			case gpeCLR_orange:	///NUM
				//cd.pst = gpeOPid_mul;
				if( lnk.y )
				{
					cd.lnk = lnk.y;
					cd.typ = OBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				cd.lnk = lnk.y = (iOPe+SCOOP.nOBJ);
				cd.typ = OBJadd.typ = OBJadd.obj.cdrMILLnum( pS, nS );
				SCOOP.nOBJ++;
				break;
			case gpeCLR_green2: {///OPER
                    if( !lnk.y ) {
						lnk.y = dOP.dctMILLfnd( (U1*)pS, nS, iOPe );
						///!!! ez majd kell, ha mert több operátor is lesz egy pS-ben
						U4 n = dOP.nSTRix(lnk.y);
						if( !lnk.y )
							break;
					}
					//if( (cd.lnk<0) ? false : (cd.lnk<iOPe) ) {
					switch( (gpeOPid)lnk.y )
                    {
                        case gpeOPid_brakS:
                        case gpeOPid_dimS:
                        case gpeOPid_begin:
							if( cd.lnk >= gpeOPid_n )
							{
								/// FUNCTION
								/// LENT
								CDsp.LEVup( SCOOP );
								/// FENT
								break;
							}
                            /// LENT
							cd.lnk = lnk.y;

                            CDsp.LEVup( SCOOP );
                            /// FENT
                            break;
                        case gpeOPid_end:
                        case gpeOPid_dimE:
                        case gpeOPid_brakE:
                            /// FENT
                            CDsp.LEVdwn( SCOOP, iOPe );
                            /// LENT
                            cd.lnk = lnk.y;
                            break;
                        default:
                            switch( (gpeOPid)cd.lnk )
                            {
                                case gpeOPid_end:
                                case gpeOPid_dimE:
                                case gpeOPid_brakE:
                                    cd.pst = (gpeOPid)lnk.y;
                                    CDsp.knead( SCOOP, iOPe );
                                    break;
                                default:
                                    if( (cd.lnk>-1) && (cd.lnk<iOPe) )
                                    {
                                        cd.pre = (gpeOPid)lnk.y;
                                        break;
                                    }
                                    cd.pst = (gpeOPid)lnk.y;
                                    CDsp.knead( SCOOP, iOPe );
                                    break;
                            }
                            break;
                    }
				} break;
			case gpeCLR_red2:	///BREAK
				break;
			case gpeCLR_violet:	///STR
			case gpeCLR_green:	///NOTE
			default:
				continue;
		}
	}
}
void gpcSRC::SRCmnMILLlnk( gpcMASS& mass, gpcWIN& win )
{
	if( !this )
		return;

	U1 iMN = 0;

	if( !SCOOP.nLiNK() )
		return;

	char *pSi, *pSe;
	U4 iOP, iOPe = mass.OPER.nIX();
	/// -------------------------------------------------------
	/// OPER
	/// -------------------------------------------------------
	if( !iOPe ) {
		mass.mxOP = gpeALF_null;
		/// ha nincsen még kitöltve az OPER lista feltöltjük
		pSe = strchr( (char*)gpasOPER[0], ' ' );
		mass.OPER.dctMILLadd( (U1*)gpasOPER[0], pSe-gpasOPER[0] );
		mass.aOP[0].num = 14;
		mass.aOP[0] = pSe+1;
		mass.mxOP = mass.aOP[0].alf;
		iOPe++;

		for( U4 i = 1, ie = gpmN(gpasOPER); i < ie; i++ ) {
			pSe = strchr( (char*)gpasOPER[i], ' ' );
			if( !pSe )
				continue;
			U4 n = pSe-gpasOPER[i], nn = 0;
			iOP = 0;
			iOP = mass.OPER.dctMILLfnd( (U1*)gpasOPER[i], pSe-gpasOPER[i], iOPe );
			if( iOP < iOPe )
			if( mass.OPER.nSTRix(iOP) < n )
				iOP = iOPe;

			if( iOP >= iOPe ) {
				iOP = iOPe;
				mass.OPER.dctMILLadd( (U1*)gpasOPER[i], pSe-gpasOPER[i] );
				mass.aOP[iOP].num = 14;
				mass.aOP[iOP] = pSe+1;
				iOPe++;
				if( mass.mxOP < mass.aOP[iOP].alf )
					mass.mxOP = mass.aOP[iOP].alf;
			}

		}
		iOPe = mass.OPER.nIX();
	}
	/// -------------------------------------------------------

	SRCmnMILLcdr( mass.aOP, mass.OPER, 0 );


}

gpcRES* gpcSRC::SRCmnMILLrun( gpcMASS* pMASS, gpcWIN* pWIN ) {
	if( !this )
		return NULL;

	SRCmnMILLdbg( pMASS->aOP, pMASS->OPER, 0 );
	if( !pDBG->nLD() )
		return NULL;

	static const U1 iMN=0;
	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	U1	//Xs,
		Ys, Xd, Yd, bOBs = false, bOBd = false;
	gpeEA mS, mD;
	char	*pALL = (char*)SCOOP.pALL,
			*p_STR[2];
	I4		anSTR[2];

	//gpcRES	aRES[0x8];
	gpcLZY	aMEM[0x10];
	gpcREG	aA[8],
			aD[8+2], *pSRC, *pDST;
	aA[7] = aA[6] = 0x400;
	I4	a;

	for( I4x4* pPCs = (I4x4*)SCOOP.vASM.p_alloc, *pPC = pPCs, *pPCe = pPC+SCOOP.nASM();
			pPC < pPCe; pPC++ )
	{
		U1x4 op = //*(U1x4*)&
					pPC->op;
		if( !op.x ) // nop
			continue;

		std::cout << pPC-pPCs << std::endl;
		mS = (gpeEA)(op.y>>3);
		switch( mS ) {
			case gpeEA_OFF:
				pSRC = NULL;
				continue;
			case gpeEA_Dn:
				pSRC = aD+(op.y&7);
				break;
			case gpeEA_An:
				pSRC = aA+(op.y&7);
				break;
			case gpeEA_IAnI:
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( aA[op.y&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_IAnIp:
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( aA[op.y&7].i8(),sizeof(*aA)) );
				++aA[op.y&7];
				break;
			case gpeEA_sIAnI:
				--aA[op.y&7];
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( aA[op.y&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_d16IAnI:
				a = aA[op.y&7].i8() + pPC->aOB[0];
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IAnDnI:
				a = aA[op.y&7].i8() + pPC->aOB[0] + aD[op.w&7].i8();
				pSRC = (gpcREG*)( aMEM[op.y&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IPcI:
				/*a = (pPC-pPCs)+pPC->aOB[0];
				pSRC = &(aRES[op.y&7].ALU(a));
				break;*/
			case gpeEA_d16IPcDnI:
				pSRC = NULL;
				break;
				/*a = (pPC-pPCs)+pPC->aOB[0]+aD[op.w&7].i8();
				pSRC = &(aRES[op.y&7].ALU(a));
				break;*/
			case gpeEA_num:
			default:{
				pSRC = &(aD[8]=*pPC->aOB);
				/*if( ob >= 0 )
					NUM = (gpcOBJlnk*)OBJ.Ux( ob, sizeof(gpcOBJlnk));
				else {
					ob *= -1;
					if(pM0[ob].iMNn)
					{
						NUM = ? pALL+pM0[ob].iMNi
					} else
						NUM.zero();

				}*/

			} break;
		}

		mD = (gpeEA)(op.z>>3);
		switch( mD ) {
			case gpeEA_OFF:
				pDST = NULL;
				continue;
			case gpeEA_Dn:
				pDST = aD+(op.z&7);
				break;
			case gpeEA_An:
				pDST = aA+(op.z&7);
				break;
			case gpeEA_IAnI:
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( aA[op.z&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_IAnIp:
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( aA[op.z&7].i8(),sizeof(*aA)) );
				++aA[op.z&7];
				break;
			case gpeEA_sIAnI:
				--aA[op.z&7];
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( aA[op.z&7].i8(),sizeof(*aA)) );
				break;
			case gpeEA_d16IAnI:
				a = aA[op.z&7].i8() + pPC->aOB[0];
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IAnDnI:
				a = aA[op.z&7].i8() + pPC->aOB[0] + aD[op.w>>4].i8();
				pDST = (gpcREG*)( aMEM[op.z&7].Ux( a,sizeof(*aA)) );
				break;
			case gpeEA_d16IPcI:
			case gpeEA_d16IPcDnI:
				pDST = NULL;
				break;
				/*a = (pPC-pPCs)+pPC->aOB[0]+aD[op.w&7].i8();
				pSRC = &(aRES[op.y&7].ALU(a));
				break;*/
			case gpeEA_num:
			default:{
				pDST = &(aD[9]=*pPC->aOB);
				/*if( ob >= 0 )
					NUM = (gpcOBJlnk*)OBJ.Ux( ob, sizeof(gpcOBJlnk));
				else {
					ob *= -1;
					if(pM0[ob].iMNn)
					{
						NUM = ? pALL+pM0[ob].iMNi
					} else
						NUM.zero();

				}*/

			} break;
		}

		if(op.x>=gpeOPid_n)
		{
			// jsr;
			continue;
		}

		switch( gpaOPgrp[op.x] )
		{
			case gpeOPid_entry:{
					switch( (gpeOPid)op.x )
					{
						case gpeOPid_dot:
							// find OBJ
							continue;
						default:
							continue;
					}
				} continue;
			case gpeOPid_mov:{
					if( !pDST || !pSRC )
						continue;

					*pDST = *pSRC;
				} continue;
			case gpeOPid_mul:{
					if( !pDST || !pSRC )
						continue;

					*pDST *= *pSRC;
				} continue;
			case gpeOPid_add:{
					if( !pDST || !pSRC )
						continue;

					*pDST += *pSRC;
				} continue;
			case gpeOPid_sub:{
					if( !pDST || !pSRC )
						continue;

					*pDST -= *pSRC;
				} continue;
		}


	}
	U4 i = 0x400, ie = aA[6].i8();
	while( i >= ie )
	{
		i--;

	}
	return apOUT[3];
}
