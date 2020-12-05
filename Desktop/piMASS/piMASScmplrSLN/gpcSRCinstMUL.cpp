#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];

gpBLK* gpcSRC::srcINSTmul( char* pS, gpBLK *pBLKm, gpBLK* pBLK ) {
	if( !pBLKm )
		pBLKm = lzyBLOCK.pSTPdwn( pBLK->bIDm );
    I4  nR = pBLK->nROW();
    if(!nR)
        return pBLKm;

	gpPTR 	*pPa, *pPb = NULL;
    gpROW	*pR0 = pBLK->pROW(),
            *pRa = pR0;

	for( gpROW* pRe = pRa+nR; pRa < pRe; pPb = pPa, pRa++ ) {
		pPa = SRCpPTR( pS, *pRa );
		if( !pPb )
			continue;

	}
	return pBLKm;
}
