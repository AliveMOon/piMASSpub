#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
void gpMEM::funPRINT()
{
	I4	nI4 = pD[7]-pA[7],
		*pI4 = (I4*)pUn(pA[7], nI4 ), nCPY;
	nI4 /= gpaCsz[gpeCsz_l];

	U8 nLEN, s;
	I4x2 AB;
	gpeCsz cAB;
	I4 i = nI4-1;
	gpPTR *pPi = (gpPTR*)pUn(pI4[i],sizeof(gpPTR));

	char	sFRMT[0x100], *pNUM,
			*pS = pPi ? (char*)pUn(pPi->iPC, pPi->sOF() ) : NULL,
			*pSe = pS;

	gpcLZY* pPRNT = NULL;

	nCPY = gpmVAN( pSe, "%", nLEN );
	while(nCPY) {


		s = -1;
		pPRNT = pPRNT->lzyADD( pSe, nCPY, s, -1 );
		pSe += nCPY;
		if( *pSe != '%' )
			break;
		// %7.3x--------------------------
		// %%%
		nCPY = gpmNINCS( pSe, "%" );
		if( !(nCPY&1) ) {
			// páros
			nCPY += gpmVAN( pSe+nCPY, "%", nLEN );
			continue;
		}
		pSe += nCPY;
		AB = strtol( pSe, &pSe, 10 );
		if( *pSe == '.' ) {
			pSe++;
			AB.y = strtol( pSe, &pSe, 10 );
		}

		pSe++;
		nCPY = gpmVAN( pSe, "%", nLEN );

		i--;
		if( i < 0 )
			continue;

		/*if( AB.area() )
			sFRMT[0] = sprintf( sFRMT+1, "%%%d.%d%c", (int)AB.x, (int)AB.y, pSe[-1] );
		else
			sFRMT[0] = sprintf( sFRMT+1, "%%%c", pSe[-1] );*/

		pPi = pPTRu1(pI4[i]);
		U1* pU1 = pPi->pU1(this);
		if( !pU1)
			continue;
		switch( pSe[-1] )
		{
			case 's':
			case 'S':{
					U4 l = gpmSTRLEN(pU1+1);
					if( l > 0 )
						l--;

					//sFRMT[0] = sprintf( sFRMT+1, "%%%ds", l );
					pPRNT = pPRNT->lzyADD( pU1+1, l, (s=-1) );
				} break;
			default: {
				if( AB.area() )
					sFRMT[0] = sprintf( sFRMT+1, "%%%d.%d%c", (int)AB.x, (int)AB.y, pSe[-1] );
				else
					sFRMT[0] = sprintf( sFRMT+1, "%%%c", pSe[-1] );
				switch( pPi->cID() ) {
					case gpeCsz_Q: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(I8*)pU1 ); break;
					case gpeCsz_q: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(U8*)pU1 ); break;
					case gpeCsz_L: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(I4*)pU1 ); break;
					case gpeCsz_l: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(U4*)pU1 ); break;
					case gpeCsz_W: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(I2*)pU1 ); break;
					case gpeCsz_w: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(U2*)pU1 ); break;
					case gpeCsz_B: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(I1*)pU1 ); break;
					case gpeCsz_b: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(U1*)pU1 ); break;
					case gpeCsz_f: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(float*)pU1 ); break;
					case gpeCsz_d: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(double*)pU1 ); break;
					case gpeCsz_4: pPRNT = pPRNT->lzyFRMT( (s=-1), sFRMT+1, *(U4*)pU1 ); break;
					default: break;
				}
			} break;

		}

	}

	pS = pPRNT ? (char*)pPRNT->p_alloc : NULL;
	nCPY = gpmSTRLEN(pS);


	gpPTR *pPhr = pPTR(pA[0]);

	I4 nA = nALL(pPhr->iPC);
	if( nA < nCPY ) {
		if( nA )
			iFREE( pPhr->iPC );
		pPhr->iPC = iALL( nCPY );
	}
	nA = nALL(pPhr->iPC);
	pPhr->cID(gpeCsz_b);
	I4x2 dm(nCPY,1);
	pPhr->d2D( dm );

	U1* pDST = pPhr->pU1( this );
	if( pDST )
		gpmMcpy( pDST, pS, nCPY )[nCPY] = 0;

	if(bSTDcout_jsr)
	{std::cout	<< stdCYAN << "funPRINT:"
				<< stdALU << (pDST?(char*)pDST:"?") << std::endl;}


}
