#include "gpcSRC.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
//#define C Csp.CDR().p_cd[0]
//#define B Bsp.CDR().p_cd[0]
#define A Asp.CDR().p_cd[0]
//#define dC C.deep
//#define dB B.deep
#define dA A.deep


//#define iC Csp.CDR().p_sp[0].x
//#define iB Bsp.CDR().p_sp[0].x
#define iA Asp.CDR().p_sp[0].x
#define OBJ SCOOP.obj
#define OBJadd ((gpcOBJlnk*)OBJ.Ux( SCOOP.nOBJ, sizeof(gpcOBJlnk)))[0]
#define OBJget ((gpcOBJlnk*)OBJ.Ux( (A.obj-iOPe), sizeof(gpcOBJlnk)))[0]
void gpcSRC::SRCmnMILLcdr( I8x2* pOP, gpcLZYdct& dOP, U1 iMN )
{
	if( !this )
		return;
	pDBG->lzyRST();

	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc, M;
	U4x4 *pL0 = (U4x4*)SCOOP.lnk.p_alloc; //, aLNK[0x10];
	U4 nM = SCOOP.nMN(), iOP, iOPe = dOP.nIX();

	OBJ.lzyRST();
	const char *pS;
	U8 nS;

	gpcOBJlnk* pOBJ;


	gpeALF opALF;
	I4x4 iZNmx = 0;
	gpcCDRsp Asp, Bsp, Csp;

	for( U4 le = SCOOP.nLiNK(), l = 0, m = 0; l < le; l++ )
	{
		U4x4& link = pL0[l];
		M = pM0[link.x];


		// még nem tud ja micsoda kicsoda
		if( !M.rMNlnk )
		{
			pS = SCOOP.dct.sSTRix(M.rMNinxt.z, NULL);
			if( !pS )
				continue;
			nS = SCOOP.dct.nSTRix(M.rMNinxt.z);
			if( !nS )
				continue;
		}

		switch( (M.rMNclr>>0x10)&0xf )
		{
			case gpeCLR_blue2: 	///ABC
				if( M.rMNlnk )
				{
					A.obj = M.rMNlnk;
					A.typ = OBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				A.obj = M.rMNlnk = (iOPe+SCOOP.nOBJ);
				A.typ = OBJadd.typ = OBJadd.obj.cdrMILLalf( pS, nS );
				SCOOP.nOBJ++;
				break;
			case gpeCLR_orange:	///NUM
				if( M.rMNlnk )
				{
					A.obj = M.rMNlnk;
					A.typ = OBJget.typ;
					break;
				}
				// ez a mini még nincs feldolgozva
				A.obj = M.rMNlnk = (iOPe+SCOOP.nOBJ);
				A.typ = OBJadd.typ = OBJadd.obj.cdrMILLnum( pS, nS );
				SCOOP.nOBJ++;
				break;
			case gpeCLR_green2: {///OPER
					if( !M.rMNlnk ) {
						M.rMNlnk = dOP.dctMILLfnd( (U1*)pS, nS, iOPe );
						///!!! ez majd kell, ha mert több operátor is lesz egy pS-ben
						U4 n = dOP.nSTRix(M.rMNlnk);
						if( !M.rMNlnk )
							break;
					}

					opALF = pOP[M.rMNlnk].alf;
					if( A.obj < iOPe )
					{
						A.pre = M.rMNlnk;
					}
					else switch( opALF )
					{

					///{  &= *= **= /= //= %= ^= |= += -= <<= >>= =  }-----------------------------------------------
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
							A.post = M.rMNlnk;
							++iA;
							break;
					///{  + ++ - -- | ^  }-----------------------------------------------
						case gpeALF_add:
						case gpeALF_inc:
						case gpeALF_sub:
						case gpeALF_dec:
						case gpeALF_or:		// a vagy a log. össze adás
						case gpeALF_xor:	// a vagy a log. össze adás majd *-1
							A.post = M.rMNlnk;
							++iA;
							break;

					///{  ~ * ** / % & ! !!  }-----------------------------------------------
						case gpeALF_inv:	// végülis ez *-1-gyel
						case gpeALF_and:	// és log. szorzás
						case gpeALF_mul:	// szorzás
						case gpeALF_exp:	// sokszor szorzás
						case gpeALF_div:	// reciprokkal szorzás
						case gpeALF_rem:	// osztás maradéka
						case gpeALF_sl:		// << *2^n
						case gpeALF_sr:		// << /2^n
						case gpeALF_notLG:
						case gpeALF_LG:
							A.post = M.rMNlnk;
							++dA;
							++iA;
							break;
					///{  .  }-----------------------------------------------
						case gpeALF_dot:
							A.post = M.rMNlnk;
							++iA;
							break;
					///{  ~> :: ( [ { ?  }-----------------------------------------------
						case gpeALF_entry:
						case gpeALF_out:
						case gpeALF_brakS:
						case gpeALF_dimS:
						case gpeALF_begin:
						case gpeALF_if:
							A.post = M.rMNlnk;
							++dA;
							++iA;
							break;

					///{  && == != || <= < >= >  }-----------------------------------------------
						case gpeALF_andLG:
						case gpeALF_neqLG:
						case gpeALF_orLG:
						case gpeALF_eqLG:
						case gpeALF_leLG:
						case gpeALF_ltLG:
						case gpeALF_beLG:
						case gpeALF_bgLG:
							A.post = M.rMNlnk;
							++dA;
							++iA;
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

						case gpeALF_else:
						case gpeALF_comE:
						case gpeALF_brakE:
						case gpeALF_dimE:
						case gpeALF_end:
						case gpeALF_mail:

						case gpeALF_str:
						case gpeALF_comS:
						case gpeALF_com:
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

			iOP = mass.OPER.dctMILLfnd( (U1*)gpasOPER[i], pSe-gpasOPER[i], iOPe );
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

	return apOUT[3];
}
