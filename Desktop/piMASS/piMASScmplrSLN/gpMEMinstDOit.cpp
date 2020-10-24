#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
I4 gpMEM::instDOitSLMP( gpcGT* pGT ) {
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	pGT->GTcnct( pWIN );
	if( cnt == pGT->iCNT )
		return cnt;

	std::cout << stdALU "SLMP" << pGT->iCNT;

	gpOBJ	*pOi = pOBJ(gpeALF_RINP),
			*pOo = pOBJ(gpeALF_ROUT);
	I8x2 an;
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
			an.x = *(U4*)pU1;
			an.y = an.x&0xff;
			an.x >>= 0x10;

			for( U1 i = 0; i < 2; i++ )
			{
				xfnd = pMASS->getXFNDan( an+I8x2( i, 0 ) );
				pS2 = xfnd ? pMASS->srcFND( xfnd ) : NULL;
				if( pS2 )
				{
					pS2->pMINI = pS2->pMINI->lzyFRMT( s=0, "\r\n" ); //\r\n" );
					pS2->pMINI = pS2->pMINI->lzyROBnDstat( s=0, *pROBnD, i, "" );
				}
			}
		}
	}

	if( !pOo )
		return pGT->iCNT;

	std::cout << stdALU "rOUT" << std::endl;
	pU1 = pSRC->srcMEMiPC( pOo->iPC, gpeCsz_l );
	if( !pU1 )
		return pGT->iCNT;
	an.x = *(U4*)pU1;
	an.y = an.x&0xff;
	an.x >>= 0x10;

	xfnd = pMASS->getXFNDan( an );
	pS2 = xfnd ? pMASS->srcFND( xfnd ) : NULL;
	if( !pS2 )
		return pGT->iCNT;
	gpcLZY *pLZYhex = pWIN->piMASS->GTlzyALL.LZY(gpdGTlzyIDinp(pGT->TnID));
	if( !pLZYhex->nLD() )
		return pGT->iCNT;
	pS2->pMINI = pS2->pMINI->lzyFRMT( s=0, "\r\n" ); //\r\n" );
	pS2->pMINI = pS2->pMINI->lzyHEXl( s = 0, pLZYhex->p_alloc, pLZYhex->n_load, false );

	return pGT->iCNT;
}
#define gpdCTRL (pCTRL?pCTRL:(pCTRL = new gpCTRL))

#define gpdGL (pGL?pGL:(pGL = new gpGL))
#define gpdGLapPIC gpdGL->apPIC
#define gpdGLaPICid gpdGL->aPICid
#define gpdGLpCAM (gpdGL->pCAM?gpdGL->pCAM:(gpdGL->pCAM = new gpcPICAM))
I4 gpMEM::instDOit( gpOBJ& obj, U1* pU1 )
{
	bool bCID = this ? !pU1 : true;
	gpcGT* pGT;
	I4 cID = -1;
	if( obj.cAN != gpeCsz_a )
	{
		return cID;
	}
	switch( obj.AN.alf )
	{
		/// --------------------------------------------------------------------------
		/// LISTENER
		case gpeALF_TELNET: {								cID = gpeCsz_L; if( bCID ) break;
			I4 port = *(U4*)pU1;
			if( port < 1 ) break;
			pGT = pMASS->GTacpt.GT( obj.AN.alf, port );
			if( !pGT ) break;
			std::cout << stdALU " TNET" << std::endl;
			pGT->GTlst( pWIN, pMASS->GTcnct );
		} break;
		/// --------------------------------------------------------------------------
		/// CONNECT
		case gpeALF_SLMP: {									cID = gpeCsz_b; if( bCID ) break;
				pGT = pMASS->GTcnct.GT( obj.AN.alf, pU1, 0 );
				instDOitSLMP( pGT );
			} break;
		case gpeALF_SYNC: {									cID = gpeCsz_b; if( bCID ) break;
			pGT = pMASS->GTcnct.GT( obj.AN.alf, pU1, 0 );
			if( !pGT ) break;
			pGT->GTcnct( pWIN );
		} break;
		/// --------------------------------------------------------------------------
		/// SRC CTRL
		case gpeALF_UNSEL: {								cID = gpeCsz_L; if( bCID ) break;
			if( (*pU1)&1 )
				pSRC->bSW |= gpeMASSunselMSK;
			else
				pSRC->bSW &= ~gpeMASSunselMSK;
		} break;
		case gpeALF_AGAIN: {								cID = gpeCsz_l; if( bCID ) break;
			msRUN = *(U4*)pU1;
			if( !msRUN ) break;
			msRUN += pWIN->mSEC.x;
		} break;
		case gpeALF_NAME: {									cID = gpeCsz_b; if( bCID ) break;
			gpmSTRCPY( gpdCTRL->sNAME, pU1 );
		} break;
		/// --------------------------------------------------------------------------
		/// periFERI
		case gpeALF_CAM: {									cID = gpeCsz_l; if( bCID ) break;
			U4 iCAM = *(U4*)pU1;
			if( !iCAM )
				break;
			if( gpdGLapPIC[0] = pMASS->PIC.PIC( I8x2(obj.AN.alf,iCAM) ) ) {
				/*if( !gpdGP->pCAM )
					gpdGP->pCAM = new gpcPICAM;*/
				U1* pRGB = gpdGLapPIC[0]->getPIX( gpdGLpCAM, pWIN->mSEC.y ); //50 );
				if( pRGB )
				{
					if( gpdGLpCAM->bGD() )
					{
						gpmDEL( gpdGLapPIC[0]->pPACK );
						pWIN->pSYNwin =
						pWIN->pSYNwin->syncADD(
												gpcSYNC(
															gpeNET4_0PIC,
															gpdGLapPIC[0]->id+1,
															pWIN->mSEC.y,
															INVALID_SOCKET, 0
														),
												pWIN->msSYN
											);
					}

					if( !pWIN->pPICbg )
					{
						pWIN->pPICbg = gpdGLapPIC[0];
					}
				}
				else if( gpdGLapPIC[0]->pSHR )
						pWIN->pPICbg = gpdGLapPIC[0];
			}
		} break;
		/// GFX
		case gpeALF_PIC: {								cID = gpeCsz_b; if( bCID ) break;
			U1* pS = pU1;
			I4 picID 	= obj.bSTR()
						? pMASS->PIC.alfFND( (pS+=gpmNINCS(pS," \t\"")) )
						: *(I4*)pU1;
			gpcPIC* pPIC = pMASS->PIC.PIC( picID );
			if( pPIC )
			{
				gpdGLapPIC[0] = pMASS->PIC.PIC( picID );
				gpdGLaPICid[0] = picID+1;
				break;
			}
			if( !*pS )
				break;

			I8x2 alfN(0,14);

			alfN = pS;
			alfN.num = gpfSTR2U8( pS+alfN.num, &pS );
			if( !(gpdGLapPIC[0] = pMASS->PIC.PIC( alfN )) )
				break;

		} break;
		default:
			break;
	}
	return cID;
}
