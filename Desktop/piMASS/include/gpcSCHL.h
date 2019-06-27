#ifndef GPESCHL_H_INCLUDED
#define GPESCHL_H_INCLUDED

#include "piMASS.h"
//#include "gpcSRC.h"

class gpeSCHL
{

	gpcLAZY	*paLZY,
			*pLST, *pFND;
	I8	nLST, nCLASS,
		idFND, ixFND;

	gpcLAZY** ppCLASS( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		gpcLAZY** ppC = NULL;
		if( paLZY )
			ppC = (gpcLAZY**)paLZY->p_alloc;

		nCLASS = ppC ? paLZY->n_load/sizeof(pFND) : 0;

		if( nCLASS <= ix )
		{
			I8	nALL = paLZY ? paLZY->n_alloc/sizeof(pFND) : 0,
				nADD;
			if( nALL <= ix )
			{
				nALL = ix+1;
				nADD = nALL-nCLASS;

				U8 s = -1;

				paLZY = paLZY->lazy_add( NULL, nADD*sizeof(pFND), s );
				ppC = NULL;
				if( paLZY )
					ppC = (gpcLAZY**)paLZY->p_alloc;
				else
					return NULL;

				if( !ppC )
					return NULL;

				gpmZn( ppC+nCLASS, nADD );
				nCLASS = paLZY->n_load/sizeof(pFND) ;
			}
		}
		return ppC+ix;
	}
	gpeSCHL( I8 id )
	{
		gpmCLR;
		I8 n = 0;
		pLST = pLST->tree_add( id, n );
		if( !pLST )
			return;

		nLST = n;
	}
public:
	gpeSCHL( void )
	{
		gpmCLR;
	}

	gpcLAZY* pGET( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		gpcLAZY** ppC = ppCLASS( ix );
		if( !ppC )
			return NULL;

		return *ppC;
	}
	gpcLAZY** ppGET( I8 ix )
	{
		if( ix >= nLST )
			return NULL;

		return ppCLASS( ix );
	}

	I8 fnd( I8 id )
	{
		if( id ? !this : true )
			return nLST;

		if( nLST )
		if( idFND == id )
			return ixFND;

		return pLST->tree_fnd(id, nLST);
	}
	gpcLAZY* p_fnd( I8 id, I8& ix )
	{
		ix = 0;
		if( id ? !this : true )
			return NULL;
		if( !nLST )
			return 0;

		if( idFND == id )
		{
			ix = ixFND;
			return pFND;
		}

		ix = pLST->tree_fnd(id, nLST);
		if(ix >= nLST)
			return NULL;

		gpcLAZY** ppC = ppCLASS( ix );
		if( ppC ? !*ppC : true )
		{
			ix = nLST;
			return NULL;
		}

		ixFND = ix;
		idFND = id;
		return pFND = *ppC;
	}
	gpeSCHL* add( I8 id, I8& ix, I8& n )
	{
		if( !id )
		{
			ix = n = (this ? nLST: 0);
			return this;
		}
		if( !this )
		{
			gpeSCHL* pSCHL = new gpeSCHL(id); ///
			ix = 0;
			n = 1;
			return pSCHL;
		}

		ix = pLST->tree_fnd( id, nLST );
		if( ix < nLST )
		{
			n = nLST;
			return this;
		}

		pLST = pLST->tree_add(id, nLST);
		n = nLST;
		/*if( ix >= nLST ) // is és nLST - ha továbbra is egyenlő akkor nem tudta hozzá adni
			return this;

		ppCLASS( ix );
		if( nCLASS > ix )
			return this;

		U8 s = -1;
		nALLOC = nLST;
		pKIDS = pKIDS->lazy_add( NULL, sizeof(*pKIDS), s );*/
		return this;
	}
};


#endif // GPESCHL_H_INCLUDED
