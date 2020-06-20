#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];



#ifdef piMASS_DEBUG
	/// gpcCDR
	#define cd (pCD=CDsp.CD())[0]
	/// gpcOBJlnk
	#define OBJget (pOBi=(gpcOBJlnk*)OBJ.Ux( (cd.obj-iOPe), sizeof(gpcOBJlnk)))[0]
	#define OBJadd (pOBn=(gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#else
	/// gpcCDR
	#define cd CDsp.CDR().p_cd[0]
	/// gpcOBJlnk
	#define OBJget ((gpcOBJlnk*)OBJ.Ux( (cd.obj-iOPe), sizeof(gpcOBJlnk)))[0]
	#define OBJadd ((gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#endif
#define aaOPid gpaaOPid[lnk.y]
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
			n = sprintf( pS, "0x%0.8llx.0x%0.8llx", obj.x, obj.y );
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
#ifdef piMASS_DEBUG
	gpcCD	*pCD;

	gpcOBJlnk	*pOBJ;
	gpcOBJlnk	*pOBn, *pOBi;

#endif
	I4x2 aFND[0x40];
	U1 nFND;
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

			cd.obj = m ;
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
				if( lnk.y )
				{
					cd.obj = lnk.y;
					cd.typ = OBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				cd.obj = lnk.y = (iOPe+SCOOP.nOBJ);
				cd.typ = OBJadd.typ = OBJadd.obj.cdrMILLalf( pS, nS );
				SCOOP.nOBJ++;
				break;
			case gpeCLR_orange:	///NUM
				if( lnk.y )
				{
					cd.obj = lnk.y;
					cd.typ = OBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				cd.obj = lnk.y = (iOPe+SCOOP.nOBJ);
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

					opALF = pOP[lnk.y].alf;
					if( cd.obj < iOPe )
					{
						cd.pre = lnk.y;
					}
					else switch( opALF )
					{

					///{  &= *= **= /= //= %= ^= |= += -= <<= >>= =  }-----------------------------------------------
						case gpeALF_mov:
						case gpeALF_andM:
						case gpeALF_mulM:
						case gpeALF_expM:
						case gpeALF_divM:
						case gpeALF_rootM:
						case gpeALF_remM:
						case gpeALF_xorM:
						case gpeALF_orM:
						case gpeALF_addM:
						case gpeALF_subM:
						case gpeALF_slM:
						case gpeALF_srM:
							cd.pst = lnk.y;
							++CDsp;
							break;
					///{  + ++ - -- | ^  }-----------------------------------------------
						case gpeALF_add:
						case gpeALF_inc:
						case gpeALF_sub:
						case gpeALF_dec:
						case gpeALF_or:		// avagy a log. össze adás
						case gpeALF_xor:	// avagy a log. össze adás majd *-1
							cd.pst = lnk.y;
							++CDsp;
							break;

					///{  ~ * ** / % & ! !!  }-----------------------------------------------
						case gpeALF_inv:	// végül is ez *-1-gyel
						case gpeALF_and:	// és log. szorzás
						case gpeALF_mul:	// szorzás
						case gpeALF_exp:	// sokszor szorzás
						case gpeALF_div:	// reciprokkal szorzás
						case gpeALF_rem:	// osztás maradéka
						case gpeALF_sl:		// << *2^n
						case gpeALF_sr:		// << /2^n
						case gpeALF_notLG:
						case gpeALF_LG:
							cd.pst = lnk.y;
							++CDsp;
							break;
					///{  .  }-----------------------------------------------
						case gpeALF_dot:
							cd.pst = lnk.y;
							++CDsp;
							break;



					///{  && == != || <= < >= >  }-----------------------------------------------
					/// cmp utasítás lényegében kivonás,
					/// csak nem a különbség van eltárólva hanem a különbség kondiciója
						case gpeALF_andLG:
						case gpeALF_neqLG:
						case gpeALF_orLG:
						case gpeALF_eqLG:
						case gpeALF_leLG:
						case gpeALF_ltLG:
						case gpeALF_beLG:
						case gpeALF_bgLG:
							cd.pst = lnk.y;
							++CDsp;
							break;

					///{  ~> :: ( [ { ?  }-----------------------------------------------
						case gpeALF_entry:
						case gpeALF_out:
						case gpeALF_brakS:
						case gpeALF_dimS:
						case gpeALF_begin:
						case gpeALF_if:
							cd.pst = lnk.y;
							++CDsp;
							break;

					/// valamineki vége
					/// szegyük vissza a PULCSIT!
						case gpeALF_newrow:
						case gpeALF_stk: {
								nFND = CDsp.opi( aFND, aaOPid, sizeof(aaOPid) );
								aFND->median( nFND, aFND+nFND+1 );
								for( U4 up = CDsp.iCD, dwn = aFND[0].iCD; up > dwn; --up )
								{
									U4x4& INS = ((U4x4*)SCOOP.vASM.Ux( SCOOP.nASM(), sizeof(INS) ))[0];
									INS = U4x4( pCD[-1].pst, pCD[0].obj-iOPe, pCD[-1].obj-iOPe );
									--CDsp;
								}
							} break;

						case gpeALF_brakE: {
								//static gpeOPid lst[] = {gpeOPid_brakS};
								nFND = CDsp.opi( aFND, aaOPid, sizeof(aaOPid) );
								aFND->median( nFND, aFND+nFND+1 );

							} break;
						case gpeALF_dimE:{
								//static gpeOPid lst[] = {gpeOPid_dimS};
								nFND = CDsp.opi( aFND, aaOPid, sizeof(aaOPid) );
								aFND->median( nFND, aFND+nFND+1 );

							} break;

						case gpeALF_else:{
								//static gpeOPid lst[] = {gpeOPid_if};
								nFND = CDsp.opi( aFND, aaOPid, sizeof(aaOPid) );
								aFND->median( nFND, aFND+nFND+1 );
							} break;

						case gpeALF_end:{
								//static gpeOPid lst[] = {gpeOPid_begin};
								nFND = CDsp.opi( aFND, aaOPid, sizeof(aaOPid) );
								aFND->median( nFND, aFND+nFND+1 );

							} break;

						case gpeALF_str:
						case gpeALF_comS:
						case gpeALF_com:
						case gpeALF_comE:
						case gpeALF_mail:

						default:
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
	if( !iOPe ) {
		mass.mxOP = gpeALF_null;
		/// ha nincsen még kitöltve az OPER lista feltöltjük
		for( U4 i = 0, ie = gpmN(gpasOPER); i < ie; i++ )
		{
			pSe = strchr( (char*)gpasOPER[i], ' ' );
			if( !pSe )
				continue;
			U4 n = pSe-gpasOPER[i];
			iOP = mass.OPER.dctMILLfnd( (U1*)gpasOPER[i], pSe-gpasOPER[i], iOPe );
			if( iOP < iOPe )
			if( mass.OPER.nSTRix(iOP) != n )
				iOP = iOPe;

			if( iOP >= iOPe )
			{
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

	SRCmnMILLcdr( mass.aOP, mass.OPER, 0 );


}
gpcRES* gpcSRC::SRCmnMILLrun( gpcMASS& mass, gpcWIN& win )
{
	if( !this )
		return NULL;

	SRCmnMILLdbg( mass.aOP, mass.OPER, 0 );
	return apOUT[3];
}
