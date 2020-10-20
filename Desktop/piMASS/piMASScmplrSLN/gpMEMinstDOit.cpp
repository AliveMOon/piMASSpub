#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
I4 gpMEM::instDOitSLMP( gpcGT* pGT )
{
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	pGT->GTcnct( pWIN );
	if( cnt == pGT->iCNT )
		return cnt;

	std::cout << stdALU "SLMP" << pGT->iCNT;

	gpOBJ	*pOi = pOBJ(gpeALF_RINP),
			*pOo = pOBJ(gpeALF_ROUT);
	I8x4 anRio;
	U1* pU1;
	gpcSRC* pS2;
	U4 xfnd;
	U8 s;
	gpcLZY *pLZYinp = pMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID));
	if( !pLZYinp )
		return pGT->iCNT;

	gpcROBnD *pROBnD = gpmLZYvali( gpcROBnD, pLZYinp );
	if( !pROBnD )
		return pGT->iCNT;

	if( pOi )
	{
		std::cout << stdALU "rINP" << std::endl;
		pU1 = pSRC->srcMEMiPC( pOi->iPC, gpeCsz_l );
		if( pU1 )
		{
			anRio.a8x2[0].x = *(U4*)pU1;
			anRio.a8x2[0].y = anRio.a8x2[0].x&0xff;
			anRio.a8x2[0].x >>= 0x10;
			for( U1 i = 0; i < 2; i++ )
			{
				xfnd = pMASS->getXFNDan( anRio.a8x2[0]+I8x2( i, 0 ) );
				pS2 = xfnd ? pMASS->srcFND( xfnd ) : NULL;
				if( pS2 )
				{
					pS2->pMINI = pS2->pMINI->lzyFRMT( s=0, "\r\n" ); //\r\n" );
					pS2->pMINI = pS2->pMINI->lzyROBnDstat( s=0, *pROBnD, i, "" );
					//pS2->pMINI = pS2->pMINI->lzyRST()->lzyHEXl( s = 0, pLZYin->p_alloc, pLZYin->n_load );
				}
			}
		}
	}

	if( !pOo )
		return pGT->iCNT;

	std::cout << stdALU "rOUT" << std::endl;
	pU1 = pSRC->srcMEMiPC( pOo->iPC, gpeCsz_l );

	return pGT->iCNT;
}
I4 gpMEM::instDOit( gpOBJ& obj, U1* pU1 )
{
	bool bCID = this ? !pU1 : true;
	I4 cID = -1;
	switch( obj.AN.alf )
	{
		case gpeALF_TELNET: {
				cID = gpeCsz_L;
				if( bCID ) break;
				I4 port = *(U4*)pU1;
				if( port < 0 )
					break;
				gpcGT* pGT = pMASS->GTacpt.GT( obj.AN.alf, port );
				if( !pGT )
					break;
				std::cout << stdALU " TNET" << std::endl;
				pGT->GTlst( pWIN, pMASS->GTcnct );
			} break;
		case gpeALF_SLMP: {
				cID = gpeCsz_L;
				if( bCID ) break;
				gpcGT* pGT = pMASS->GTcnct.GT( obj.AN.alf, pU1, 0 );
				instDOitSLMP( pGT );
				break;
				/*if( !pGT )
					break;

				I4 cnt = pGT->iCNT;
				pGT->GTcnct( pWIN );
				if( cnt == pGT->iCNT )
					break;
				std::cout << stdALU " SLMP" << std::endl;*/

			} break;
		case gpeALF_AGAIN:	cID = gpeCsz_l;
			if( bCID ) break;

			msRUN = *(U4*)pU1;
			if( !msRUN )
				break;
			msRUN += pWIN->mSEC.x;
			break;
		default:
			break;
	}
	return cID;
}
