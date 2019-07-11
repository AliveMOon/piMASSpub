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

	{ gpaOPCi,	"new",		0, 0, 0, 0,					0.0, gpeALF_OPER, gpeALF_NEW },
	{ gpaOPCi,	"del",		0, 0, 0, 0,					0.0, gpeALF_OPER, gpeALF_DEL },

	{ gpaOPCi,	"SYS",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"GT",		0, 0, 0, 0,					0.0, gpeALF_CLASS },
	{ gpaOPCi,	"PIC",		0, 0, 0, 0,					0.0, gpeALF_CLASS },


};
char* gpasOPER[] = {


	"! inv",	"!! LG",	"!= neqLG",

	"/* comS", 	"*/ comE",	"// com",


	"& and", 	"&& andLG",	"&= andM",
	"* mul", 	"** exp",	"*= mulM",
							"**= expM",
	"/ div", 	"/= divM",	"//= rootM",
	"% rem", 	"%= remM",
	"^ xor",	"^= xorM",


	"= mov", 	"== eqLG",
	"| or", 	"|| orLG",	"|= orM",
	"+ add", 	"++ inc",	"+= addM",
	"- sub", 	"++ dec",	"-= subM",


	"<= leLG",	"< ltLG", 	"<< sl", 	"<<= slM",
	">= beLG",	"> bgLG", 	">> sr", 	">>= srM",

	". tag", 	"-> entry", ":: out"
	"( brakS",	") brakE",
	"[ dimS", 	"] dimE",
	"{ begin", 	"} end",
	"? if",		": else",
	"@ mail"
};

U1* gpcMASS::reset( U1* pSTR0 )
{
	gpmZ( asPRG );
	if( !pPUB )
	{

		pPUB = (U1*)strcpy( (char*)pSTR0, "false" );

		U8 s8;
		PC.reset( &CMPL, pPUB );
		aPC[0] = 0;
		gpcCMPL* pPC = PC.pPC( &CMPL, 0 );
		//pPC->i_str = 0;
		pPUB += pPC->n_str+1;
		U4 j = 1, nS;
		U1 *pS, *pE;
		for( U4 i = 0, n = gpmN(gpasOPER), nSTR; i < n; i++ )
		{
			pE = (U1*)strchr( gpasOPER[i], ' ' );
			if( !pE )
				continue;
			nS = pE-(U1*)gpasOPER[i];
			pS = (U1*)gpmMEMCPY( pPUB, gpasOPER[i], nS );
			pS[nS] = 0;
			pPUB += nS+1;

			PC.pPC( &CMPL, 0 )->cmpl_add( &CMPL, pS, nS );
			pPC = PC.pPC( &CMPL, j );
			if( !pPC )
				continue;

			pE++;
			nS = strlen( (char*)pE ); //pE-(U1*)gpasOPER[i];
			pS = (U1*)gpmMEMCPY( pPUB, pE, nS );

			//pPC->i_str = pS-pSTR0;
			//pPC->n_str = nS;
			pPC->typ = gpfSTR2ALF( pPUB, pPUB+nS, NULL );
			pPC->wip = gpeALF_OPER;
			pPUB += nS+1;
			j++;
		}
		nOP = j;
		for( U4 i = 1, iN = gpmN(gpaOPCi), nwLEV = iLEV; i < iN; i++ )
		{
			gpcOPCD opcd = gpaOPCi[i];
			nS = opcd.nSTR;
			pS = opcd.pSTR;
			gpmMEMCPY( pPUB, pS, nS );
			pPUB[nS] = 0;
			pPUB += nS+1;

			PC.pPC( &CMPL, 0 )->cmpl_add( &CMPL, pS, nS );
			pPC = PC.pPC( &CMPL, j );
			if( !pPC )
				continue;
			pPC->i_str = pPUB-pSTR0;
			//pPC->n_str = gpaOPCi[i].nSTR;

			//pPUB += pPC->n_str+1;
			pPC->typ = opcd.typ;
			pPC->wip = opcd.wip;
			pPC->n_dat = opcd.nDAT;
			j++;
		}


		iLEV++;
	}
	rstLEV = iLEV;
	return pPUB;
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
I1 gpcCMPL::sDST( U1* pPUB, U4 iFND, char* p_str0, char* pTAB, char* pSTR )
{
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
						pTAB, p_str0 ? p_str0+i_str : "", pOP, pSTR

					);
	return o;
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
gpcCMPL* gpcCMPL::pLIST( U1* pSTR0, U1* pPUB, gpcLAZY* pCMPL, char c )
{
	if( !pSTR0 )
		return NULL;
	if( !pPUB )
		pPUB = pSTR0;

	*pPUB = 0;
	if( this ? !pCMPL : true )
		return this;

	U4 n = nKID();
	if( !n )
	{
		if( iINI )
			pPC( pCMPL, iINI )->pLIST( pSTR0, pPUB, pCMPL, 'i' );
		else if( iDEF )
			pPC( pCMPL, iDEF )->pLIST( pSTR0, pPUB, pCMPL, 'd' );
		return this;
	}

	gpcCMPL	*p_stuff, *p_def;
	if( c )
		pPUB += sprintf(
							(char*)pPUB, "%0.2d %s[%0.2d]%c ",
							iLEV,
							pSTR0+i_str,
							iPC,
							c
						);

	for( U4 i = 0; i < n; i++ )
	{
		p_stuff = pPC( pCMPL, iKID( pCMPL, i ) );
		if( !p_stuff  )
			continue;

		p_def = pPC( pCMPL, p_stuff->iDEF ? p_stuff->iDEF : p_stuff->iDEC );

		pPUB += sprintf(
							(char*)pPUB, "%s[%0.2d]%s,",
							pSTR0+p_def->i_str,
							p_stuff->iPC,
							pSTR0+p_stuff->i_str
						);

	}
	*pPUB = 0;
	return this;
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

U4 gpcCMPL::cmpl_best( gpcLAZY* pCMPL, U1* pS, U4 nS )
{
	if( this ? !pCMPL : true )
		return 0;

	gpcCMPL** ppC = (gpcCMPL**)(pCMPL->n_load ? pCMPL->p_alloc : NULL);
	if( !ppC )
		return 0;

	gpcCMPL* pC = this; //*pCC = pPC( pCMPL, 0 );
	U4 ifPC, nPC, iBST = 0, nBST = 0;

	while( pC )
	{
		ifPC = pC->p_kid->dict_find( pS, nS, nPC );

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
