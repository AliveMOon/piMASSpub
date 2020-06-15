#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

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

			iOP = mass.OPER.dictMILLfind( (U1*)gpasOPER[i], pSe-gpasOPER[i], iOPe );
			if( iOP >= iOPe )
			{
				iOP = iOPe;
				mass.OPER.dictMILLadd( (U1*)gpasOPER[i], pSe-gpasOPER[i] );
				mass.aOP[iOP].num = 14;
				mass.aOP[iOP] = pSe+1;
				iOPe++;
				if( mass.mxOP < mass.aOP[iOP].alf )
					mass.mxOP = mass.aOP[iOP].alf;
			}

		}
		iOPe = mass.OPER.nIX();
	}

	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc, M;
	U4x4 *pL0 = (U4x4*)SCOOP.lnk.p_alloc, aLNK[0x10];
	U4 nM = SCOOP.nMN();
	pDBG->lzyRST();
	const char *pS;
	U8 nS, nIX = 0;
	U4	nABC = 0, nNUM = 0, nOPER = 0, nBREAK = 0, sw,
		nAN = 0, nADD, nSUB, n,
		iiiA = 1, iiiB = 1, iiiC = 1,
		*pAi, *pBi, *pCi;

	gpcLZY Ao,Bo,Co, Ai,Bi,Ci;


	I4x4 iZNmx = 0;
	gpeALF opALF;



	for( U4 le = SCOOP.nLiNK(), l = 0, m = 0; l < le; l++ )
	{
		U4x4& link = pL0[l];
		M = pM0[link.x];
		pS = SCOOP.dct.sSTRix(M.rMNinxt.z, NULL);
		if( !pS )
			continue;
		nS = SCOOP.dct.nSTRix(M.rMNinxt.z);
		if( !nS )
			continue;

		switch( sw = ((M.rMNclr>>0x10)&0xf) )
		{
			case gpeCLR_blue2: 	///ABC
				if( iCi ) { C[0].an(pS,nS); break; }
				if( iBi ) { B[0].an(pS,nS); break; }
				A[0].an(pS,nS);
				break;
			case gpeCLR_orange:	///NUM

				nNUM++;
				break;
			case gpeCLR_green2: {///OPER
					iOP = mass.OPER.dictMILLfind( (U1*)pS, nS, iOPe );
					opALF = mass.aOP[iOP].alf;
					switch( opALF )
					{
						case gpeALF_dot:
							if( iC ? !!C[0].typ : false )
									++iC;
							else if( iB ? !!B[0].typ : false )
									++iB;
							else if( !A[0].typ )
									 ++iA;
							break;
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
						case gpeALF_mov:

							if(iC) {
								if( iC < 2 )
								{
									// Semmi! Csak egy op?
									C[-1].OP(iOP);
									break;
								}

								/// B-t stackelni majd C folytassa B-ként
								++iC;
								iiiB++;
								iBi[0] = U4x2(iBi[-1].sum(),0);

								gpmMcmpOF( B,C-iC, iC );
								iB += iC;

								iiiC++;
								iCi[0] = U4x2(iCi[-1].sum(),0);
								/// X= lehet C-t indítani
								C[0].ZN(iZNmx.a4x2[0]).OP(iOP);
								++iC;
								C[0].null();
								break;
							}
							else if(iB) {
								if( iB < 2 )
								{
									// Semmi! Csak egy op?
									B[-1].OP(iOP);
									break;
								}

								/// A-t stackelni majd B folytassa A-ként
								++iB;
								iiiA++;
								iAi[0] = U4x2(iAi[-1].sum(),0);

								gpmMcmpOF( A,B-iB, iB );
								iA += iB;

								iiiB++;
								iBi[0] = U4x2(iBi[-1].sum(),0);
							} else {
								if(!A[0].typ)
									A[0].ZN( iZNmx.a4x2[0] );
								++iA;
							}
							/// X= lehet B-t indítani
							B[0].ZN(iZNmx.a4x2[0]).OP(iOP);
							++iB;
							B[0].null();
							break;
						case gpeALF_add:
						case gpeALF_inc:
						case gpeALF_sub:
						case gpeALF_dec:
							if( iC ) {

							}
							else if (iB) {


							}
							break;

						/// valamineki vége le kell nulázni az iABC-ket
						case gpeALF_stk:

							iZNmx.x++;
							if( iZNmx.z < iZNmx.x )
							{
								// bővíteni kell

							}
							break;
						case gpeALF_newrow:

							iZNmx.y++;
							break;
						case gpeALF_notLG:
						case gpeALF_LG:
						case gpeALF_inv:


						case gpeALF_comS:
						case gpeALF_comE:

						case gpeALF_com:
						case gpeALF_and:
						case gpeALF_andLG:
						case gpeALF_mul:
						case gpeALF_exp:
						case gpeALF_div:
						case gpeALF_rem:

						case gpeALF_xor:

						case gpeALF_eqLG:
						case gpeALF_neqLG:

						case gpeALF_or:
						case gpeALF_orLG:



						case gpeALF_leLG:
						case gpeALF_ltLG:
						case gpeALF_sl:

						case gpeALF_beLG:
						case gpeALF_bgLG:
						case gpeALF_sr:


						case gpeALF_entry:
						case gpeALF_out:

						case gpeALF_brakS:
						case gpeALF_brakE:
						case gpeALF_dimS:
						case gpeALF_dimE:
						case gpeALF_begin:
						case gpeALF_end:
						case gpeALF_if:
						case gpeALF_else:
						case gpeALF_mail:
						case gpeALF_str:
						default:
							break;
					}
					nOPER++;
                } break;
			case gpeCLR_red2:	///BREAK
				nBREAK++;
                break;
			case gpeCLR_violet:	///STR
			case gpeCLR_green:	///NOTE
			default:
				continue;
		}
	}
}
gpcRES* gpcSRC::SRCmnMILLrun( gpcMASS& mass, gpcWIN& win )
{
	if( !this )
		return NULL;

	return apOUT[3];
}
