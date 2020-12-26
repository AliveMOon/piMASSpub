#include "gpcSRC.h"
#include "gpcSRCinst.h"
#include "gpcSRClnk.h"
#include "gpccrs.h"
#include <wiringSerial.h>
/*
-lraspicam
-lvcos
-lbcm_host
-lmmal -lmmal_core
-lmmal_util
-lmmal_vc_client
*/
extern U1 gpaALFsub[];
extern char gpaALF_H_sub[];
extern char gpsPUB[0x1000];
char sGPS[] =	"AT\r\n"
				"ATE1\r\n"
				"AT+CGMI\r\n"
				"AT+CGMM\r\n"
;
char sSER[] = 	"/dev/ttyS0"
				//"/dev/ttyAMA0"
;
I8 gpcGT::GTgsm( gpcWIN* pWIN ) {
	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? iCNT : 0;
	gpcGT* pGT = this;
	U1 sOUT[0x100], *pS, *pSe;
	int baud = 115200; //16550;
	U8 s = -1, nLEN;
	if( socket == INVALID_SOCKET )
	//if( (socket=(SOCKET)serialOpen("/dev/ttyS0",baud)) <0 ){
	if( (socket=(SOCKET)serialOpen( sSER, baud )) < 0 ) {
		if(bSTDcout){std::cout << stdALU "GSM ERR:" << strerror(errno) << std::endl;}

		msGTdie = pWIN->mSEC.x + 5000;
		return iCNT;
	} else {
		pOUT = pOUT->lzyADD( sGPS, gpmSTRLEN(sGPS), (s=0), -1 );
	}

	int c, nR = serialDataAvail(socket), n = pINP->nLD();
	if( nR > 0 )
	{
		nR = read( socket, sOUT, nR );
		if( nR > 0 )
			pINP = pINP->lzyADD( sOUT, nR, s=-1, -1 );
		/*c = serialGetchar(socket);
		while( c >= 0 ) {
			pINP = pINP->lzyADD( &c, 1, s=-1, -1 );
			c = serialGetchar(socket);
		}*/
	}
	if(bSTDcout){std::cout << stdALU "GSM c:" << c << stdALU "iCNT:" << iCNT <<std::endl;}
	pS = pINP ? pINP->p_alloc+n : NULL;
	/// RECV -------------------------------------
	if( pS )
	{
		pSe = pS;
		pSe += gpmVAN( pSe, "\r\n", nLEN );
		if( !*pSe )
			return iCNT;
		pSe += gpmNINCS( pSe, "\r\n" );
		if(bSTDcout){std::cout << stdALU "GSM msg:" << pINP->p_alloc; }
		gpmDEL(pINP);

		pS = pOUT ? pOUT->p_alloc+iCNT : NULL;
		if( pS )
		{
			pSe = pS;
			pSe += gpmNINCS( pSe, "\r\n" );
			iCNT += pSe-pS;
		}
	}
	/// SEND -------------------------------------
	pS = pOUT ? pOUT->p_alloc+iCNT : NULL;
	if( !pS )
		return iCNT;
	pSe = pS;
	pSe += gpmVAN( pSe, "\r\n", nLEN );
	if( pSe-pS < 1 )
		return iCNT;
	iCNT += pSe-pS;
	pSe += gpmNINCS( pSe, "\r\n" );
	//gpmMcpy(sOUT, pS, pSe-pS )[pSe-pS]=0;
	int nW = 0;
	while( pS < pSe ) {
		nW = write(socket,pS,pSe-pS);
		pS += nW;
	}
	//serialPuts( socket, (char*)sOUT );
	return iCNT;
}

I4 gpMEM::instDOitGSM( gpcGT* pGT ) {
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	pGT->GTgsm( pWIN );
	if( cnt == pGT->iCNT )
		return cnt;

#ifdef stdON
	if(bSTDcout){std::cout << stdALU "GSM" << pGT->iCNT;}
#endif

	return pGT->iCNT;
}
