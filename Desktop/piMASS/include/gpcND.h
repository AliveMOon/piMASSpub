#ifndef GPCND_H
#define GPCND_H

#include "gpcSRC.h"

class gpcND
{

		void** ppND( void )
		{
			return (void**)( pND ? pND->p_alloc : NULL );
		}
	public:

		gpcLAZY	*pND,
				*pLST;
		U4		nLST, xFND, nALLOC;
		void	*pFND;


		gpcND();
		virtual ~gpcND();
		gpcND(const gpcND& other);
		gpcND& operator=(const gpcND& other);



	protected:

	private:
};

#endif // GPCND_H
