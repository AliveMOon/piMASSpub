#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
#define mCD0 (pCD=CDsp.piCD())[0]


#define aaOPid gpaaOPid[OPgrp]
size_t gpCNST::strASM( char* pS, char* pALL, I8x4 *pM0, U4x4 *pL0   ) {
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

gpcO* gpcPIK::fnd( gpcO* pM, gpcO& dot, U4 nmID ) {
	gpcO* pO = NULL;
	if( !pM )
	{
		pO = gpmLZYvali( gpcO, &oLST );
		for( U4 i = 0, n = oLST.nLD(sizeof(*pO)); i < n; i++ )
		{
			if( pO[i].nmID != nmID )
				continue;
			return pO+i;
		}
		return NULL;
	}

	pO = (dot=pM);

	gpcC* pC = gpmLZYvali( gpcC, &cLST );
	if( !pC )
		return NULL;

	pC += dot.cID;
	U4x4	*pCL = pC->pLST;
	U4		*pKD = pC->pKID, szNM,
			ary;
	for( U4 i = 0, n = pC->nLST, iK = 0, off = 0; i < n; i++ )
	{
		szNM = pCL[i].x;
		ary = pCL[i].y;
		if( !ary )
		{
			ary = pCL[i].y = pCL[i].a4x2[1].area();
			if( !ary )
				ary = pCL[i].y = (pCL[i].a4x2[1] = 1).x;
		}
		if( (szNM>>4) != nmID )
		{
			// nem ez!
			szNM &= 0xf;
			if( szNM < gpeCsz_K )
			{
				off += ary*gpaCsz[szNM];
				continue;
			}

			off += ary*gpmLZYvali( gpcC, &cLST )[pKD[iK]].szOF;
			++iK;
			continue;
		}
		dot.nmID = szNM;
		dot.pALL += off;
		dot.an = pCL[i].a4x2[1];
		return &dot;
	}
	return NULL;
}
void gpcSRC::srcCMPLR_tresh( gpcLZYdct& dOP, U1 iSCP ) {
	if( !this )
		return;
	pDBG->lzyRST();

	I8x4 *pM0 = gpmSCP.pMN(), M, Mnx;
	U4x4 *pL0 = gpmSCP.pLNK(); //, aLNK[0x10];
	U4 nM = gpmSCP.nMN(), iOP; //, iOPe = dOP.nIX();

	scpCNST.lzyRST();
	const char *pS;
	const U1 *pSTR;
	U8 nS, nSTR;
	gpcCDsp CDsp; //, Bsp, Csp;
	gpcCD	*pCD;
#ifdef piMASS_DEBUG

	//gpCNST	*pOBJ;
	gpCNST	*pCNSTn, *pCNSTi;

#endif
	I4x2 aFND[0x40];
	U1 nFND, OPgrp;
	gpeALF opALF;
	I4x4 iZNmx = 0;
	gpeCLR clr;

	//for( U4 le = gpmSCP.nLiNK(), l = 0, mNX; l < le; l++ )
	for( U4 nM = gpmSCP.nMN(), m = 0, l; m < nM; m++ )
	{
		clr = (gpeCLR)pM0[m].au4x4[1].aRGBAx4[2].B; // ((pM0[m].rMNclr>>0x10)&0xf);
		if( clr == gpeCLR_red2 )
				continue;

		M = pM0[m]; //pM0[link.x];
		pSTR = NULL;
		if( M.iMNtyp == gpeTYP_STR )
		{
			if( clr != gpeCLR_violet )
				continue;

			pSTR = (nSTR=M.iMNn) ? gpmSCP.p_str+M.iMNi : NULL; //M.iMNindt.a4x2[0];

			mCD0.lnk = -m;
			mCD0.typ = gpeTYP_STR;
			continue;
		}

		U4x4& lnk = pL0[l=M.iMNdct];
		// még nem tudja micsoda kicsoda
		if( !lnk.y )
		{
			pS = gpmSCP.lzyDCT.sSTRix(l, NULL);
			if( !pS )
				continue;
			nS = gpmSCP.lzyDCT.nSTRix(l);
			if( !nS )
				continue;
		}


		switch( clr )
		{
			case gpeCLR_blue2: 	///ABC
				if( lnk.y )
				{
					mCD0.lnk = lnk.y;
					mCD0.typ = scpOBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				mCD0.lnk = lnk.y = (gpeOPid_jsr+gpmSCP.nCNST);
				mCD0.typ = scpOBJadd.typ = scpOBJadd.obj.cdrMILLalf( pS, nS );
				gpmSCP.nCNST++;
				break;
			case gpeCLR_orange:	///NUM
				if( lnk.y )
				{
					mCD0.lnk = lnk.y;
					mCD0.typ = scpOBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				mCD0.lnk = lnk.y = (gpeOPid_jsr+gpmSCP.nCNST);
				mCD0.typ = scpOBJadd.typ = scpOBJadd.obj.cdrMILLnum( pS, nS );
				gpmSCP.nCNST++;
				break;
			case gpeCLR_green2: {///OPER
                    if( !lnk.y ) {
						U4 iOPe = gpeOPid_jsr;
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
							if( mCD0.lnk >= gpeOPid_jsr )
							{
								/// FUNCTION
								/// LENT
								CDsp.LEVup( gpmSCP );
								/// FENT
								break;
							}
                            /// LENT
							mCD0.lnk = lnk.y;
                            CDsp.LEVup( gpmSCP );
                            /// FENT
                            break;
                        case gpeOPid_end:
                        case gpeOPid_dimE:
                        case gpeOPid_brakE:
                            /// FENT
                            CDsp.LEVdwn( gpmSCP, //iOPe,
											gpeOPid_mov ); // (gpeOPid)lnk.y );
                            /// LENT
                            mCD0.lnk = lnk.y;
                            break;
                        default:
                            switch( (gpeOPid)mCD0.lnk )
                            {
                                case gpeOPid_end:
                                case gpeOPid_dimE:
                                case gpeOPid_brakE:
                                    mCD0.pst = (gpeOPid)lnk.y;
                                    CDsp.knead( gpmSCP ); //, iOPe );
                                    break;
                                default:
                                    if( (mCD0.lnk>-1) && (mCD0.lnk<gpeOPid_jsr) )
                                    {
                                        mCD0.pre = (gpeOPid)lnk.y;
                                        break;
                                    }
                                    mCD0.pst = (gpeOPid)lnk.y;
                                    CDsp.knead( gpmSCP);//, iOPe );
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

void gpcSRC::srcBLD( gpcMASS* pMASS ) //, gpcWIN& win )
{
	if( !this )
		return;

	U1 iSCP = 0;

	if( !gpmSCP.nLNK() )
		return;

	srcCMPLR( *pMASS->pOPER(), iSCP, pMASS );
	return;

	/// OFF OLD ---------
	if( pCORE )
		return;

	(pCORE=new gpCORE)->coreLNK(
									gpmLZYvali( I4x4, &gpmSCP.lzyASM ),
									gpmSCP.nASM(),
									gpmSCP.pMN(),
									(char*)gpmSCP.p_str, &scpCNST
								);

}


