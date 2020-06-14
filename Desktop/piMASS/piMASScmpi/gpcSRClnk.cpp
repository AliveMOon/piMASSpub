#include "gpcSRC.h"
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

	I8x4 *pM0 = (I8x4*)SCOOP.mini.p_alloc, M;
	U4x4 *pL0 = (U4x4*)SCOOP.lnk.p_alloc, aLNK[0x10];
	U4 nM = SCOOP.nMN();
	pDBG->lzyRST();
	const char *pS;
	char *pSi, *pSe;
	U8 nS, nIX = 0;
	U4 nABC = 0, nNUM = 0, nOPER = 0, nBREAK = 0, sw,
		nAN = 0, axFND[0x10], nxFND = 0, iOP, iOPe = mass.OPER.nIX(),
		nADD, nSUB, n;
	I8x2 AN;
	gpcSRC* aAN[0x10];
	gpeALF opALF;
	if( !iOPe )
	{
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
			}

		}
	}

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
				AN.num = nS;
				AN = pS;
				pSi = (char*)pS+AN.num;
				AN.num = gpfSTR2I8( pSi, &pSe );
				if( pSe > pSi )
				{
					// ez AN-nek néz ki
					axFND[nxFND] = mass.getXFNDan( AN );
					nxFND++;
				}
				if( apOUT[3] )
				{
					// létezik a resben?

				}
				nABC++;
				break;
			case gpeCLR_orange:	///NUM
				nNUM++;
				break;
			case gpeCLR_green2: {///OPER
					iOP = mass.OPER.dictMILLfind( (U1*)pS, nS, iOPe );
					opALF = mass.aOP[iOP].alf;
					switch( opALF )
					{
						case gpeALF_INV:

							break;
						case gpeALF_MOV:

							break;
						case gpeALF_STK:

							break;
						case gpeALF_NEWROW:

							break;
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
