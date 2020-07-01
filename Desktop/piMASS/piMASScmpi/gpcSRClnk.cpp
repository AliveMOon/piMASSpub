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

gpcRES* gpcSRC::SRCmnMILLrun( gpcMASS& mass, gpcWIN& win ) {
	if( !this )
		return NULL;

	SRCmnMILLdbg( mass.aOP, mass.OPER, 0 );
	if( !pDBG->nLD() )
		return NULL;

	static const U1 iMN=0;
	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc;
	U1 Xn, Yn, bOB = false;
	gpeEA m;
	I8	aR[0x10],
		*pA = aR, *pD = pA+0x8;
	gpmZ(aR);
	pA[7] = pA[6] = 0x400;
	gpcLZY aMEM[0x8];

	char *pALL = (char*)SCOOP.pALL, *apSTR[2];
	I4	anSTR[2];
	gpcOBJlnk* aOB[2];

	for( I4x4* pPCs = SCOOP.vASM.pINST(0), *pPC = pPCs, *pPCe = pPC+SCOOP.nASM();
			pPC < pPCe; pPC++ )
	{
		U1x4& op = *(U1x4*)pPC->op;
		if( !op.x ) // nop
			continue;

		for( U1 i = 0; i < 2; i++, bOB = false ) {
			Xn = i ? op.z : op.y;
			m = (gpeEA)(Xn>>3);
			switch( m ) {
				case gpeEA_OFF:
					continue;
				case gpeEA_Dn:
				case gpeEA_An:
				case gpeEA_IAnI:
				case gpeEA_IAnIp:
				case gpeEA_sIAnI:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], Xn&7 )+1;
					break;
				case gpeEA_d16IAnI:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i], Xn&7 )+1;
					break;
				case gpeEA_d16IAnDnI:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i], Xn&7, gps68kREG[Yn&0xf] )+1;
					Yn = i ? op.w>>4 : op.w&0xf;
					bOB = true;
					break;
				case gpeEA_d16IPcI:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i] )+1;
					bOB = true;
					break;
				case gpeEA_d16IPcDnI:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i], gps68kREG[Yn&0xf] )+1;
					Yn = i ? op.w>>4 : op.w&0xf;
					bOB = true;
					break;
				case gpeEA_num:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i] )+1;
					bOB = true;
					break;
				default:
					//pDe = pDe+sprintf( pDe, gps68kADRmod[ADRmod], ins.aOB[i] )+1;
					bOB = true;
					break;
			}
			aOB[i] = NULL;
			apSTR[i] = NULL;
			anSTR[i] = 0;
			if( bOB ) {
				I4 ob = pPC->aOB[i];
				if( ob >= 0 )
				{
					aOB[i] = (gpcOBJlnk*)OBJ.Ux( ob, sizeof(gpcOBJlnk));
					//pCe += obj.strASM( pCe, pALL, pM0, pL0 ) + 1;
				} else {
					ob *= -1;
					apSTR[i] = pALL+pM0[ob].iMNi;
					anSTR[i] = pM0[ob].iMNn;
					//gpmMcpy( pCe, pALL+pM0[o].iMNi, pM0[o].iMNn );
					//pS[pM0[o].iMNn]=0;
					//pCe += pM0[o].iMNn + 1;
				}
			}
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
			case gpeOPid_mul:{

				} continue;
			case gpeOPid_add:{


				} continue;
			case gpeOPid_sub:{


				} continue;
		}


	}

	return apOUT[3];
}
