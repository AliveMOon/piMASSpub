#ifndef GPCND_H
#define GPCND_H

#include "gpcSRC.h"

class gpcND
{
	gpcLAZY	*pND,
			*pLST;
	U4		nLST, nALLOC;
	U8		alf;
	void	*pFND;

	void** ppND( void )
	{
		return (void**)( pND ? pND->p_alloc : NULL );
	}
public:




	gpcND();
	virtual ~gpcND();
	gpcND(const gpcND& other);
	gpcND& operator=(const gpcND& other);



protected:

private:
};

#endif // GPCND_H
