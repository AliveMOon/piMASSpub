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

	return;

	/// off
	if( pSTR[nSTR] == '(' )
		nSTR++;


}

static const gpcOPCD gpaOPCi[] = {
	// this,	pS,			a, m, i, nDT,				d,		wip, typ;
	{ gpaOPCi,	"false", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_zero },
	{ gpaOPCi,	"true", 	0, 0, 0, sizeof(U1), 		0.0, gpeALF_TRUE },

	{ gpaOPCi,	"U1", 		0, 0, 0, sizeof(U1), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"U2", 		0, 0, 0, sizeof(U2), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"U4", 		0, 0, 0, sizeof(U4), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"U8", 		0, 0, 0, sizeof(U8), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"I1", 		0, 0, 0, sizeof(I1), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"I2", 		0, 0, 0, sizeof(I2), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"I4", 		0, 0, 0, sizeof(I4), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"I8", 		0, 0, 0, sizeof(I8), 		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"F4", 		0, 0, 0, sizeof(float),		0.0, gpeALF_DEF, gpeALF_CLASS },
	{ gpaOPCi,	"F8", 		0, 0, 0, sizeof(double),	0.0, gpeALF_DEF, gpeALF_CLASS },

	{ gpaOPCi,	"sizeof(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_SIZEOF },
	{ gpaOPCi,	"if(", 		0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_IF },
	{ gpaOPCi,	"for(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_FOR },
	{ gpaOPCi,	"while(", 	0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_WHILE },
	{ gpaOPCi,	"switch(",	0, 0, 0, 0,					0.0, gpeALF_FUNC, gpeALF_SWITCH },

	{ gpaOPCi,	"break",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_BREAK },
	{ gpaOPCi,	"continue",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_CONTINUE },
	{ gpaOPCi,	"return",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_RETURN },
	{ gpaOPCi,	"discard",	0, 0, 0, 0,					0.0, gpeALF_SYS, gpeALF_BREAK },

	{ gpaOPCi,	"class",	0, 0, 0, 0,					0.0, gpeALF_DEC, gpeALF_CLASS },
	{ gpaOPCi,	"pub",		0, 0, 0, 0,					0.0, gpeALF_CLASS, gpeALF_CTRL },
	{ gpaOPCi,	"prot",		0, 0, 0, 0,					0.0, gpeALF_CLASS, gpeALF_CTRL },

	{ gpaOPCi,	"new",		0, 0, 0, 0,					0.0, gpeALF_NEW, gpeALF_MEM },
	{ gpaOPCi,	"del",		0, 0, 0, 0,					0.0, gpeALF_DEL, gpeALF_MEM },

	{ gpaOPCi,	"SYS",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"GT",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"PIC",		0, 0, 0, 0,					0.0, gpeALF_CLASS },


};


U1* gpcMASS::reset( U1* pSTR )
{
	gpmZ( asPRG );
	if( !pPUB )
	{

		pPUB = (U1*)strcpy( (char*)pSTR, "false" );

		U8 s8;
		PC.reset( &CMPL, pPUB );
		aPC[0] = 0;
		gpcCMPL* pPC = PC.pPC( &CMPL, 0 );
		//pPC->i_str = 0;
		pPUB += pPC->n_str+1;

		for( U4 i = 1, iN = gpmN(gpaOPCi), nwLEV = iLEV; i < iN; i++ )
		{
			gpmMEMCPY( pPUB, gpaOPCi[i].pSTR, gpaOPCi[i].nSTR );
			pPUB[gpaOPCi[i].nSTR] = 0;

			PC.pPC( &CMPL, 0 )->cmpl_add( &CMPL, pPUB, gpaOPCi[i].nSTR );
			pPC = PC.pPC( &CMPL, i );
			if( !pPC )
				continue;
			pPC->i_str = pPUB-pSTR;
			//pPC->n_str = gpaOPCi[i].nSTR;

			pPUB += pPC->n_str+1;
			pPC->typ = gpaOPCi[i].typ;
			pPC->wip = gpaOPCi[i].wip;
			pPC->n_dat = gpaOPCi[i].nDAT;
		}
		iLEV++;
	}
	rstLEV = iLEV;
	return pPUB;
}
U4 gpcCMPL::iKID( gpcLAZY* pCMPL, U4 i )
{
	if( !this )
		return nPC(pCMPL);

	U4 n = p_iPC ? p_iPC->n_load/sizeof(U4) : 0;
	if( i >= n )
		return nPC( pCMPL );

	return ((U4*)p_iPC->p_alloc)[i];
}
U1* gpcCMPL::pLIST( gpcLAZY* pCMPL, U1* pSTR0, U1* pPUB )
{
	*pPUB = 0;

	U4 n = nKID();
	if( !n )
		return pPUB;

	gpcCMPL	*p_stuff, *p_def;
	for( U4 i = 0; i < n; i++ )
	{
		p_stuff = pPC( pCMPL, iKID( pCMPL, i ) );
		if( !p_stuff  )
			continue;
		p_def = pPC( pCMPL, p_stuff->iDEF );


		pPUB += true ?	sprintf(
									(char*)pPUB, "%0.2d:%s:%s,",
									p_stuff->iPC, pSTR0+p_def->i_str,
									pSTR0+p_stuff->i_str
								)
						:
						sprintf(
									(char*)pPUB, "%0.2d:%s:%0.2d:%s,",
									p_stuff->iPC, pSTR0+p_def->i_str,
									p_stuff->n_dat, pSTR0+p_stuff->i_str
								);
	}
	*pPUB = 0;
	return pPUB;
}
U4 gpcCMPL::cmpl_find( gpcLAZY* pCMPL, U1* pS, U4 nS )
{
	if( this ? !pCMPL : true )
		return 0;

	gpcCMPL** ppC = (gpcCMPL**)(pCMPL->n_load ? pCMPL->p_alloc : NULL);
	if( !ppC )
		return 0;

	gpcCMPL* pC = this;
	U4 ifPC, nPC, *pPC = NULL;

	while( pC )
	{
		ifPC = pC->p_kid->dict_find( pS, nS, nPC );

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
gpcLAZY* gpcCMPL::cmpl_add( gpcLAZY* pCMPL, U1* pS, U4 nS )
{

	U4 fnd = cmpl_find( pCMPL, pS, nS );
	if( fnd )
		return pCMPL;

	if( this ? !pCMPL : true )
		return pCMPL;

	U4 w = p_kid->nIX();
	p_kid = p_kid->dict_add( pS, nS );
	if( w >= p_kid->nIX() )
		return pCMPL;

	U8 iPC8 = -1;

	pCMPL = pCMPL->lazy_add( NULL, sizeof(this), iPC8 );
	gpcCMPL	**ppC = (gpcCMPL**)pCMPL->p_alloc;
	if( !ppC )
	{
		return pCMPL;
	}
	gpcCMPL* pC = new gpcCMPL( iPC );
	if( !pC )
		return pCMPL;

	pC->iPC = iPC8/sizeof(this);
	pC->n_str = nS;
	ppC[pC->iPC] = pC;

	p_iPC = p_iPC->lazy_add( &pC->iPC, sizeof(pC->iPC), iPC8 = -1 );

	return pCMPL;
}
