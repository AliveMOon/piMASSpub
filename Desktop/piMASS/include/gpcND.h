#ifndef GPCND_H
#define GPCND_H

#include "gpcSRC.h"

class gpcND
{
public:
	gpeALF	alfIS;
	U8		typeIS;
protected:
	gpcLAZY	*pND,
			*pLST;
	U4		nLST, nALLOC;

	gpeALF	alfND;
	gpcND	*pFND;

	gpcND** ppND( void )
	{
		return (gpcND**)( pND ? pND->p_alloc : NULL );
	}
public:
	gpcND* fnd( gpeALF alf )
	{
		if(!this)
			return NULL;

		if( nLST )
		if( alfND == alf )
			return pFND;

		U8 iFND = pLST->tree_fnd((U8)alf, nLST);
		if(iFND >= nLST)
			return NULL;

		gpcND** ppS = ppND();
		if( ppS ? !ppS[iFND] : true )
			return pFND;

		pFND = ppS[iFND];
		alfND = alf;

		return pFND;
	}

	gpcND();
	virtual ~gpcND();
	gpcND(const gpcND& other);
	gpcND& operator=(const gpcND& other);



protected:

private:
};

#endif // GPCND_H
