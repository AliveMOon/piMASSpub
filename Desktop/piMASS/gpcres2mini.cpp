#include "gpcres.h"
extern U1 gpaALFadd[];
//extern U1 gpsTABrun[];
U1	gpsTABrun[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",
	*gppTABrun = gpsTABrun + sizeof(gpsTABrun)-1; //pmSTRLEN( (char*)gpsTABrun );


gpcLAZY* gpcRES::res2mini( gpcLAZY* pLZY, U1* pBUFF, gpcRES* pMOM, U4 deep )
{
	if( !this )
		return pLZY;

    if( deep ? !pLZY : false )
		return NULL;

	if( pLZY )
		pLZY->lzy_reset();
	else
		pLZY = new gpcLAZY;

	if( !pLZY )
		return NULL;

	U8 s = -1;
	U1* pB = pBUFF+0x100, *p_sep;
	for( U4 a = 0; a < nA; a++ )
	{
		ALU( a );
		if( (gpeALF)(a+1) != alu.alf )
		{
			pB = pB + gpfALF2STR( pBUFF, alu.alf ) + 1;
			pLZY->lzyFRMT( s = -1,	"%s"
									//"%s."
									"%s=" ,//" // [%dx%d]sof(%d) ",
									gppTABrun-deep,
									//alu.typ.typ2str(pBUFF+0x10),
									pBUFF //, alu.AN.x, alu.AN.y, alu.nLOAD()
									);
		} else {
			*pBUFF = 0;
			pB = pBUFF;
			pLZY->lzyFRMT( s = -1, nA > 1 ? "%s=" : "%s", gppTABrun-deep );
		}



		U4x2 	//X = alu.xDIM(),
				xy;
		U1* pD = (U1*)alu.pDAT;
		if( !pD )
		{
			pLZY->lzyFRMT( s = -1, "\r\n" );
			continue;
		}
		if( alu.typ().x & 0x10 )
		{
			pLZY->lzyFRMT( s = -1, "%s\"%s\"", gppTABrun-deep, pD );
			if( nA > 1 )
				pLZY->lzyFRMT( s = -1, ";\r\n" );
			continue;
		}
		U4 		nAN = alu.AN().z, d, xp1, xe,
				nB	= alu.typ().w,
				//nX	= X.area(),
				nXB	= alu.txySOF(); //txySOF(); // nX*nB;

		U4x2	T = alu.trafo(); //  alu.txySOF(), nXB*alu.AN.a4x2[0].x );
		//U4		d =  (a.xy-sub)*T;

		for( xy.y = 0; xy.y < alu.AN().y; xy.y++ )
		{
			pLZY->lzyFRMT( s = -1, "%s", gppTABrun-deep );
			for( xy.x = 0, xe = alu.AN().x; xy.x < xe; xy.x = xp1 )
			{
				xp1 = xy.x+1;
				p_sep = (U1*)((xp1 < xe) ? "," : "");
				d = xy*T;
				/*if( d > nAN*nXB )
					d %= nAN*nXB;*/

				if( alu.typ().x&0x40 )
				{
					if( nB > 4 )
						pLZY->lzyFRMT( s = -1, "%f%s", ((double*)(pD+d))[0], p_sep );
					else
						pLZY->lzyFRMT( s = -1, "%f%s", ((float*)(pD+d))[0], p_sep );
					continue;
				}
				if( alu.typ().x&0x80 )
				{
					if( nB > 2 )
					{
						if( nB > 4 )
							pLZY->lzyFRMT( s = -1, "%lld%s", ((I8*)(pD+d))[0], p_sep  );
						else
							pLZY->lzyFRMT( s = -1, "%d%s", ((I4*)(pD+d))[0], p_sep );
						continue;
					}
					if( nB > 1 )
						pLZY->lzyFRMT( s = -1, "%lld%s", ((I2*)(pD+d))[0], p_sep  );
					else
						pLZY->lzyFRMT( s = -1, "%d%s", ((I1*)(pD+d))[0], p_sep );

					continue;
				}
				if( nB > 2 )
				{
					if( nB > 4 )
						pLZY->lzyFRMT( s = -1, "%lld%s", ((U8*)(pD+d))[0], p_sep  );
					else
						pLZY->lzyFRMT( s = -1, "%d%s", ((U4*)(pD+d))[0], p_sep );
					continue;
				}
				if( nB > 1 )
					pLZY->lzyFRMT( s = -1, "%d%s", ((U2*)(pD+d))[0], p_sep );
				else
					pLZY->lzyFRMT( s = -1, "%d%s", ((U1*)(pD+d))[0], p_sep );

			}
			if( nA > 1 )
				pLZY->lzyFRMT( s = -1, ";\r\n" );
		}
	}
	/// ha nem DEBUG legyen pMOM == NULL és akor nem írja ki a pISA-t
	if( !pMOM )
		return pLZY;

	/*bool b_asg = false;
	for( U4 i = 0; i < nISA.x; i++ )
	{

		if( !pLZY )
			continue;
		b_asg = pISA[i].isa.aISA[1] == '=';
		if( b_asg )
			pLZY->lzyFRMT( s = -1, "\r\n%s", gppTABrun-deep );

        switch( pISA[i].isa.aISA[0] )
        {

			case gpeISA_trg:{
					pLZY->lzyFRMT( s = -1, "\r\n%s[%d:%d]", gppTABrun-deep, pISA[i].an.x, pISA[i].an.y );
				} break;
 			case gpeISA_u8: {
					pLZY->lzyFRMT( s = -1, "%lld", pISA[i].an.u8 );
				} break;
			case gpeISA_d8: {
					pLZY->lzyFRMT( s = -1, "%f", pISA[i].an.d8 );
				} break;
			case gpeISA_an: {
					gpfALF2STR( sBUFF, pISA[i].an.a4 );
					pLZY->lzyFRMT( s = -1, "%s%d", sBUFF, pISA[i].an.n4 );
				} break;
			case gpeISA_anFUN: {
					gpfALF2STR( sBUFF, pISA[i].an.a4 );
					pLZY->lzyFRMT( s = -1, "%s%d(", sBUFF, pISA[i].an.n4 );
				} break;

			case gpeISA_tag: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzyFRMT( s = -1, "#%s", sBUFF );
				} break;

			case gpeISA_var: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzyFRMT( s = -1, "%s%s", b_asg ? "." : "", sBUFF );
				} break;
			case gpeISA_FUN: {
					gpfALF2STR( sBUFF, pISA[i].an.var );
					pLZY->lzyFRMT( s = -1, "%s(", sBUFF );
					//pOUT = pISA[i].pRES->run( pOUT, pLZY, pMASS, pSRC, this, deep+1 );
				} break;
			case gpeISA_str: {
					pLZY->lzyFRMT( s = -1, "\"%s\"", pISA[i].an.y ? pISA[i].an.aSTR[0] : (U1*)"" );
				} break;
			case gpeISA_nop:
			default: {
				} break;
        }
        pLZY->lzyFRMT( s = -1, "%c", pISA[i].isa.aISA[1] >= ' ' ? pISA[i].isa.aISA[1] : '_' );

	}*/
    return  pLZY;
}
