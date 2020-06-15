#include "gpcSRC.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
class gpcAx{
public:
	I8x2 	AN;
	U1		typ, op, x;
	gpcAx(){};
	gpcAx& null() { gpmCLR; return *this; }
	gpcAx& OP( U1 o ) { op=o; return *this; }
	gpcAx& ZN( I8x2 zn ) { gpmCLR; AN=zn; typ=4; return *this; } // typ=4 ZN coordináta
	U1 an( const char* pS, U4 nS )
	{
		AN.num = nS;
		AN = pS;
		char *pSi = (char*)pS+AN.num, *pSe;
		typ = !!AN.x;
		if( typ ? (AN.num >= nS) : true )
			return typ;

		AN.num = gpfSTR2I8( pSi, &pSe );
		if(pSe > pSi)
			typ |= 2;

		if( pSe-pS < nS )
		if( *pSe == 'x' || *pSe == 'X' )
		{
			// vector
			switch( pSe[1] )
			{
				case '2':
					x = 2;
					break;

				case '3':
				case '4':
					x = 4;
					break;

				case '5':
				case '6':
				case '7':
				case '8':
					x = 8;
					break;

				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					x = 0x10;
					break;

				default:
					x = 1;
					break;
			}
		}


		return typ;
	}

};
#define iA (((U4x2*)Ai.Ux(iiiA,sizeof(U4x2)))[0])
#define iB (((U4x2*)Bi.Ux(iiiB,sizeof(U4x2)))[0])
#define iC (((U4x2*)Ci.Ux(iiiC,sizeof(U4x2)))[0])

#define iAi iA.y
#define iBi iB.y
#define iCi iC.y

#define A0 ((gpcAx*)A.Ux(iA.x,sizeof(gpcAx)))[ 0]
#define A1 ((gpcAx*)A.Ux(iA.x,sizeof(gpcAx)))[-1]
#define B0 ((gpcAx*)B.Ux(iB.x,sizeof(gpcAx)))[ 0]
#define B1 ((gpcAx*)B.Ux(iB.x,sizeof(gpcAx)))[-1]
#define C0 ((gpcAx*)C.Ux(iC.x,sizeof(gpcAx)))[ 0]
#define C1 ((gpcAx*)C.Ux(iC.x,sizeof(gpcAx)))[-1]


void gpcSRC::SRCmnMILLlnk( gpcMASS& mass, gpcWIN& win )
{
	if( !this )
		return;

	U1 iMN = 0;

	if( !SCOOP.nLiNK() )
		return;

	char *pSi, *pSe;
	U4 iOP, iOPe = mass.OPER.nIX();
	if( !iOPe )
	{
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

	gpcLZY A,B,C, Ai,Bi,Ci;


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
				if( iCi )
				{
					C0.an(pS,nS);
					break;
				}
				if( iBi )
				{
					B0.an(pS,nS);
					break;
				}


				A0.an(pS,nS);
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
							if( iCi )
							{
								if( C0.typ )
									++iC;
							}
							else if( iBi )
							{
								if( B0.typ )
									++iB;
							} else
								if( A0.typ )
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
							/// X= lehet B-t indítani
							if(iCi)
							{

							}
							else if(iBi)
							{
								if( iBi < 2 )
								{
									// Semmi! Csak egy op?
									B1.OP(iOP);
									break;
								}

								// eltároljuk a 'A' cél és 'B' forrás



							}
							else if(!iAi)
							{
								A0.ZN( iZNmx.a4x2[0] );
								++iA;
							}
							B0.ZN(iZNmx.a4x2[0]).OP(iOP);
							++iB;
							B0.null();
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

						case gpeALF_add:
						case gpeALF_inc:

						case gpeALF_sub:
						case gpeALF_dec:

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
