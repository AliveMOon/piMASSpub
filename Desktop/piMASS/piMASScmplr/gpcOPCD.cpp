#include "gpcSRC.h"


gpcOPCD::gpcOPCD( const gpcOPCD* pTHIS, const char* pS, char a, char m, I8 i, U8 u, double _d, gpeALF wp, gpeALF ty )
{
	gpmCLR;

	pSTR = (U1*)pS;
	nADD = a;
	nMUL = m;
	d = _d;
	i8 = (this-pTHIS)+i;

	typ = ty;
	wip = wp;
	nDAT = u;


	nSTR = gpmVAN( pSTR,
					gpdPRGsep, //gpsPRG,
					nLEN );

	//return;

	/// off
	if( pSTR[nSTR] == '(' )
		nSTR++;


}


char* gpcCMPL::sLOG( U1* pPUB, char* pTAB, char* sNDAT, gpcLZY* pCMPL )
{
	if( this ? !pPUB : true )
	{
		if( pPUB )
			*pPUB = 0;
		return (char*)pPUB;
	}


	U1	*pTYP = pPUB,
		*pS = pTYP+0x10;

	gpfALF2STR( pTYP, (I8)typ );
	if( wip == gpeALF_REG )
	{
		switch( typ )
		{
			case gpeALF_D:
				sprintf(
							(char*)pS,
							"%0.2d:%0.2d R%d%s%s%f ",
							pCMPL->nPC(), iLEV, iKD, (char*)pTYP, pTAB, d
						);
						break;
			case gpeALF_I:
				sprintf(
							(char*)pS,
							"%0.2d:%0.2d R%d%s%s%lld ",
							pCMPL->nPC(), iLEV, iKD, (char*)pTYP, pTAB, i8
						);
						break;
			default:
				sprintf(
							(char*)pS,
							"%0.2d:%0.2d R%d%s%s%lld ",
							pCMPL->nPC(), 	iLEV, 	iKD,	(char*)pTYP,	pTAB,	u8
							// %0.2d		:%0.2d R%d		%s		%s		%d
						);
			break;
		}
		return (char*)pS;
	}

	char *pSTR = (char*)pCMPL->pPC( mPC )->p_kid->sSTRix( iKD, "Oxo" );

	if( !pSTR )
		*pS = 0;
	else switch ( wip )
	{
		case gpeALF_CONSTR:
		case gpeALF_FUNC:
		case gpeALF_CYCLE:
		//case gpeALF_ARRAY:
			{
				/*if( *pTAB )
					pTAB++;
				if( *pTAB )
					pTAB++;*/
				gpfALF2STR( pTYP, (I8)wip );
				gpcCMPL* pD = pCMPL->pPC( iDEF );
				if( pD )
				{
					char *pDEF = (char*)pCMPL->pPC( pD->mPC )->p_kid->sSTRix( pD->iKD, "Oxo" );
					sprintf(
								(char*)pS, "%0.2d:%0.2d.%0.2d[%0.2d]%s%s.%s\t%s",
										pCMPL->nPC(),
										mPC, iKD, iPC, //iLEV, mPC, iPC,
										pTAB,
										pDEF, pSTR, (char*)pTYP
							);
					break;
				}
			}
		case gpeALF_OPER:
			sprintf(
						(char*)pS, "%0.2d:%0.2d.%0.2d[%0.2d]%s%s\t%s",
								pCMPL->nPC(),
								mPC, iKD, iPC, //iLEV, mPC, iPC,
								pTAB,
								pSTR, (char*)pTYP
					);
			break;
		default:
			if(
					n_dat
					//&& (iPC == pCMPL->nPC()-1)
				)
				sprintf(
							(char*)pS, "%0.2d:%0.2d.%0.2d[%0.2d]0x%x%s%s\t%s.%c",
									pCMPL->nPC(),
									mPC, iKD, iPC, //iLEV, mPC, iPC,
									 i_dat,
									pTAB,
									pSTR, (char*)pTYP, sNDAT[n_dat]
						);
			else
				sprintf(
							(char*)pS, "%0.2d:%0.2d.%0.2d[%0.2d] %s%s\t%s.%c",
									pCMPL->nPC(),
									mPC, iKD, iPC, //iLEV, mPC, iPC,
									pTAB,
									pSTR, (char*)pTYP, sNDAT[n_dat]
						);
			break;
	}

	return (char*)pS;
}
char* gpcCMPL::sASM( U1* pS0, U1* pPUB, char* sNDAT, gpcLZY* pCMPL, gpcCMPL*pA, gpcCMPL*pB )
{
	U1	*pASMop = pPUB;
	char	*psOP = NULL, *psA = NULL, *psB = NULL,
			*pRa	= (char*)pASMop+0x10,
			*pRb	= pRa+0x10,
			*pCOUT	= pRb+0x10;
	*pCOUT = 0;
	gpcCMPL* pM = NULL;
	if( pA->wip == gpeALF_REG )
	{
		sprintf( pRa, "%d", pA->u8 );
		psA = pRa;
	}
	else if( pM = pCMPL->pPC( pA->mPC ) )
	{
		psA = (char*)pM->p_kid->sSTRix( pA->iKD, "-ASM:WARRNNING-" );
	}

	if( !this )
	{
		psOP = NULL;
	}
	else if( pM = pCMPL->pPC( mPC ) )
	{
		psOP = (char*)pM->p_kid->sSTRix( iKD, "-ASM:WARRNNING-" );
	}

	if( pB->wip == gpeALF_REG )
	{
		sprintf( pRb, "%d", pB->u8 );
		psB = pRb;
	}
	else if( pM = pCMPL->pPC( pB->mPC ) )
	{
		psB = (char*)pM->p_kid->sSTRix( pB->iKD, "-ASM:WARRNNING-" );
	}

	gpfALF2STR( pASMop, (I8)typ );
	sprintf(
				pCOUT,
				"%0.2d:%0.2d %s.%c [%0.2d], [%0.2d]		; %s %s %s",
				pCMPL->nPC(), pA->iLEV, pASMop, sNDAT[pA->n_dat], pA->iPC, pB->iPC,
																		(psA ? psA : "?"), (psOP ? psOP : "?"), (psB ? psB : "?")
			);
	return pCOUT;
}



char gpsOP[] = "=<>-+*/";
I1 gpcCMPL::sOP( char* pS )
{
	*pS = gpsOP[0];
	pS[1] = 0;
	if( this ? !op : true )
		return 2;

	U2 msk = 1, n = op ? (U2)log2( op ) : 0, l = 0, o = 0;

	for( n = sizeof(gpsOP); l < n; l++, msk<<=1 )
	{
		if( op&msk )
		{
			pS[o] = gpsOP[l];
			o++;
		}
	}

	pS[o] = 0;
	o++;
	return o;
}
I1 gpcCMPL::sDST( U1* pPUB, U4 iFND, char* pS0, const char* pTAB, const char* pSTR ) {
	if( !this )
		return 0;
	if( !op )
	{
		*pPUB = 0;
		return 0;
	}
	///sDST
	char	*pOP = (char*)pPUB, *pINFO = pOP,
			o = sOP( pOP );

	sprintf( pINFO = pOP+o, "%0.2d %0.2d[%0.2d:%0.2d]%0.2db %s %s %s %s",
						iLEV, iFND,
						iPC, mPC,
						n_dat,
						pTAB, pS0 ? pS0+iPUB : (char*)"", pOP, pSTR

					);
	return o;
}
gpcCMPL* gpcLZY::pPC( U4 pc, U1* pPUB )
{
	gpcCMPL	**ppC = this ? (gpcCMPL**)p_alloc : NULL;
	if( !ppC )
		return NULL;
	U4 n = nPC();
	U1* pS = pPUB;
	while( pc < n )
	{
		if( !pc )
			return *ppC;
		if( !ppC[pc] )
			return NULL;

		if( pPUB )
			pS = pPUB+ppC[pc]->iPUB;

		//if( !ppC[pc]->iREDIR )
		if( ppC[pc]->iREDIR ? ( pc == ppC[pc]->iREDIR ) : true )
			return ppC[pc];

		pc = ppC[pc]->iREDIR;
	}

	return NULL;
}

gpcCMPL* gpcLZY::pSPARE( U4 pc, gpeALF sw, U1* pS )
{
	gpcCMPL	**ppC = this ? (gpcCMPL**)p_alloc : NULL;
	if( !ppC )
		return NULL;
	U4 n = nPC();
	while( pc < n )
	{
		if( !pc )
			return *ppC;
		if( !ppC[pc] )
			return NULL;

		if( ppC[pc]->iSPARE ? (pc != ppC[pc]->iSPARE) : false )
		if( sw ? ppC[pc]->wip == sw : true )
		{

			pc = ppC[pc]->iSPARE;
			continue;
		}

		if( ppC[pc]->iREDIR ? ( pc == ppC[pc]->iREDIR ) : true )
			return ppC[pc];

		pc = ppC[pc]->iREDIR;
	}

	return NULL;
}
U4 gpcLZY::nPC( void )
{
	if( !this )
		return 0;

	return n_load/sizeof(gpcCMPL*);
}
U4 gpcCMPL::iKID( gpcLZY* pCMPL, U4 i )
{
	if( !this )
		return pCMPL->nPC();

	U4 n = p_iPC ? p_iPC->n_load/sizeof(U4) : 0;
	if( i >= n )
		return pCMPL->nPC();;

	return ((U4*)p_iPC->p_alloc)[i];
}
gpcCMPL* gpcCMPL::sKIDlst( U1* pS0, U1* pPUB, gpcLZY* pCMPL, char c )
{
	if( !pS0 )
		return NULL;
	if( !pPUB )
		pPUB = pS0;

	*pPUB = 0;
	if( this ? !pCMPL : true )
		return this;

	U4 n = nKID();
	if( !n )
	{
		if( iINI )
			pCMPL->pPC( iINI )->sKIDlst( pS0, pPUB, pCMPL, 'i' );
		else if( iDEF )
			pCMPL->pPC( iDEF )->sKIDlst( pS0, pPUB, pCMPL, 'd' );
		return this;
	}

	gpcCMPL	*p_stuff, *p_def;
	if( c )
		pPUB += sprintf(
							(char*)pPUB, "%0.2d %s[%0.2d]%c ",
							iLEV,
							pS0+iPUB,
							iPC,
							c
						);

	for( U4 i = 0; i < n; i++ )
	{
		p_stuff = pCMPL->pPC( iKID( pCMPL, i ) );
		if( !p_stuff  )
			continue;

		p_def = pCMPL->pPC( p_stuff->iDEF ? p_stuff->iDEF : p_stuff->iDEC );

		pPUB += sprintf(
							(char*)pPUB, "%s[%0.2d]%s,",
							pS0+p_def->iPUB,
							p_stuff->iPC,
							pS0+p_stuff->iPUB
						);

	}
	*pPUB = 0;
	return this;
}
U4 gpcCMPL::cmpl_find( gpcLZY* pCMPL, U1* pS, U4 nS )
{
	if( this ? !pCMPL : true )
		return 0;

	gpcCMPL** ppC = (gpcCMPL**)(pCMPL->n_load ? pCMPL->p_alloc : NULL);
	if( !ppC )
		return 0;

	gpcCMPL* pC = ppC[iPC]; // this;
	U4 ifPC, nPC, *pPC = NULL;

	while( pC )
	{
		ifPC = pC->p_kid->dctFND( pS, nS, nPC );

		if( ifPC >= nPC )
		{
			if( pC->mPC == pC->iPC )
				return 0;
			pC = ppC[pC->mPC];
			continue;
		}
		U4x4 *p_ix0 = ((U4x4*)pC->p_kid->ix.p_alloc);
		if( p_ix0[ifPC].y != nS )
		{
			if( pC->mPC == pC->iPC )
				return 0;
			pC = ppC[pC->mPC];
			continue;
		}

		if( pC->p_iPC ? !pC->p_iPC->p_alloc : true )
			return 0;

		return ((U4*)pC->p_iPC->p_alloc)[ifPC];
	}
	return 0;
}

U4 gpcCMPL::cmpl_best( gpcLZY* pCMPL, U1* pS, U4 nS )
{
	if( this ? !pCMPL : true )
		return 0;

	gpcCMPL** ppC = (gpcCMPL**)(pCMPL->n_load ? pCMPL->p_alloc : NULL);
	if( !ppC )
		return 0;

	gpcCMPL* pC = ppC[iPC]; //this; //*pCC = pCMPL->pPC( 0 );
	U4 ifPC, nPC, iBST = 0, nBST = 0;

	while( pC )
	{
		ifPC = pC->p_kid->dctFND( pS, nS, nPC );

		if( ifPC >= nPC )
		{
			if( pC->mPC == pC->iPC )
				return iBST;
			pC = ppC[pC->mPC];
			continue;
		}
		U4x4 *p_ix0 = ((U4x4*)pC->p_kid->ix.p_alloc);
		if( p_ix0[ifPC].y != nS )
		{
			if( p_ix0[ifPC].y > nBST )
			if( pC->p_iPC ? !!pC->p_iPC->p_alloc : false )
			{
				nBST = p_ix0[ifPC].y;
				iBST = ((U4*)pC->p_iPC->p_alloc)[ifPC];
			}

			if( pC->mPC == pC->iPC )
				return iBST;

			pC = ppC[pC->mPC];
			continue;
		}

		if( pC->p_iPC ? !pC->p_iPC->p_alloc : true )
			return iBST;

		return ((U4*)pC->p_iPC->p_alloc)[ifPC];
	}
	return iBST;
}

gpcLZY* gpcCMPL::cmpl_add( gpcLZY* pCMPL, U1* pS, U4 nS )
{

	U4 fnd = cmpl_find( pCMPL, pS, nS );
	if( fnd )
		return pCMPL;

	if( this ? !pCMPL : true )
		return pCMPL;

	U4 w = p_kid->nIX();
	p_kid = p_kid->dctADD( pS, nS );
	if( w >= p_kid->nIX() )
		return pCMPL;

	U8 iPC8 = -1;

	pCMPL = pCMPL->lzyADD( NULL, sizeof(this), iPC8 );
	gpcCMPL	**ppC = (gpcCMPL**)pCMPL->p_alloc;
	if( !ppC )
	{
		return pCMPL;
	}
	gpcCMPL* pC = new gpcCMPL( iPC );
	if( !pC )
		return pCMPL;
	bool b_rst = !iPC8;

	pC->iPC = iPC8/sizeof(this);
	pC->n_str = nS;
	ppC[pC->iPC] = pC;

	p_iPC = p_iPC->lzyADD( &pC->iPC, sizeof(pC->iPC), iPC8 = -1 );
	pC->iKD = iPC8/sizeof(pC->iPC);
	if( b_rst)
		pC->iLEV = 0;
	else
		pC->iLEV = iLEV+1;
	return pCMPL;
}
