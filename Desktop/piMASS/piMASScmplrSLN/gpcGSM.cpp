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
char sGPS[] =	"AT+CRESET\r\n"
				"AT+CGMI\r\n"		// Module Manufacture
				"AT+CGMM\r\n"		// Module Modell
				"AT+CGSN\r\n"		// Serial Num
				"AT+CGMR\r\n"
				"AT+COPS?\r\n"		// provider
				;

char sGPSoff[] =
				"AT+CATR?\r\n"
				"AT\r\n"
				"ATE1\r\n"
				"AT+CGMI\r\n"
				"AT+CGMM\r\n"
				"AT+CGSN\r\n"
				"AT+CSUB\r\n"
				"AT+CGMR\r\n"
				"AT+IPREX?\r\n"
				"AT+CSQ\r\n"
				"AT+CICCID\r\n"
				"AT+SPIC\r\n"
				"AT+CPIN?\r\n"
				//"AT+CPIN=2028\r\n"
				//"AT+CPIN?\r\n"
				"AT+SPIC\r\n"
				"AT+CSPN?\r\n"
				"AT+COPS?\r\n"
				"AT+CREG?\r\n"
				"AT+CPSI?\r\n"
				"AT+CNMP=2\r\n"

				"AT+CGPS=1\r\n"
				"AT+CGPS?\r\n"

				"AT+CGPSINFO\r\n"

				"AT+CGPS=0\r\n"

				"AT+CGNSPWR=1\r\n"
				"AT+CGNSTST=1\r\n"
				"AT+CGNSTST=0\r\n"
				"AT+CGNSIF\r\n"
				"AT+CGNSPWR=0\r\n"
										;


char sSER[] = 	"/dev/%s"
				//"/dev/ttyAMA0"
;
class gpcGSM{
public:
	I8x2	sndLAST;
	char	sCGMI[0x30],
			sCGMM[0x30],
			sCGMR[0x30],
			sCGSN[0x30],
			sCOPS[0x30];
	bool	bPINrdy,
			bCGMI, bCGMM, bCGMR, bCGSN, bCOPS;
	gpcGSM(){gpmCLR;};

};
gpcGSM gsmZERO;
I8 gpcGT::GTgsm( gpcWIN* pWIN ) {
	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? iCNT : 0;
	gpcGT* pGT = this;
	char	sOUT[0x100], sANSW[0x100],
			*pS, *pSe,
			*pSat, *pSatI, *pSatE;
	int baud = 115200, aI[0x20], nAT=0, nWT = 0, nWok = 0; //16550;
	U8 s = -1, nLEN;
	I8x2 aAT[0x10], aWT[0x10];
	if( socket == INVALID_SOCKET ) {
		sprintf( sOUT, sSER, s_ip );
		//if( (socket=(SOCKET)serialOpen("/dev/ttyS0",baud)) <0 ){
		if( (socket=(SOCKET)serialOpen( sOUT, baud )) < 0 ) {
			if(bSTDcout){std::cout << stdALU "GSM ERR:" << strerror(errno) << std::endl;}

			msGTdie = pWIN->mSEC.x + 3000;
			return iCNT;
		} else {
			pOUT = pOUT->lzyADD( &gsmZERO, sizeof(gsmZERO), (s=0), -1 );
			iCNT = pOUT->nLD();
			pOUT = pOUT->lzyADD( sGPS, gpmSTRLEN(sGPS), (s=-1), -1 );
		}
	}
	gpcGSM	*pGSM = (gpcGSM*)pOUT->p_alloc;
	int nR = serialDataAvail(socket), nINP = pINP->nLD();
	if( nR > 0 )
	{
		nR = read( socket, sOUT, nR );
		if( nR > 0 )
			pINP = pINP->lzyADD( sOUT, nR, s=-1, -1 );
		nINP = pINP->nLD();
	}

	if(bSTDcout){std::cout << stdALU "iCNT:" << iCNT <<std::endl;}

	pSat = pINP ? (char*)pINP->p_alloc : NULL;
	/// RECV -------------------------------------
	sANSW[0] = 0;
	if( pSat ) {
		pSatE = (pSatI=pSat) + nINP;
		nAT = 0;
		for( pSatI += gpmNINCS(pSatI," \r\n\t:+,"); pSatI < pSatE; pSatI += gpmNINCS(pSatI," \r\n\t:+,"), nAT++ ) {
			aAT[nAT].y = pSatE-pSatI;
			aAT[nAT] = pSatI;
			if( !aAT[nAT].alf ) {
				nAT--;
				pSatI += gpmVAN(pSatI,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",nLEN);
				continue;
			}
			pSatI+=aAT[nAT].y;

			aAT[nAT].y = pSatI-pSat;
			if( *pSatI == '+' )
				aAT[nAT].y++;
		}
		U4 at = 0;
		for( ; at < nAT; at++ ) {
			aAT[at].an2str( sOUT );
			if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
		}
		if(bSTDcout){std::cout << "nAT:" << at << std::endl;}

		if( nAT > 1 ) {
			switch( aAT[1].alf ) {
				case gpeALF_CRESET: {
						if( nAT < 7 )
							break;
						switch( aAT[4].alf ) {
							case gpeALF_CPIN: {
									switch( aAT[5].alf ){
										case gpeALF_SIM: {
												switch( aAT[6].alf ){
													case gpeALF_PIN: {
															sprintf( sANSW, "AT+CPIN=%d\r\n", 2028 );
														} break;
													case gpeALF_READY:
														pGSM->bPINrdy = true;
														break;
													default: break;
												}
											} break;
										case gpeALF_READY:
											pGSM->bPINrdy = true;
											break;
										default: break;
									}
								} break;
							default:
								break;
						}

						nAT = 0;
					} break;
				case gpeALF_CPIN: {
					if( nAT < 5 )
						break;

					if( aAT[2].alf == gpeALF_CPIN )
					if( pSat[aAT[1].num] == '?' ) {
						switch( aAT[3].alf ) {
							case gpeALF_SIM: {
									switch( aAT[4].alf ){
										case gpeALF_PIN: {
												sprintf( sANSW, "AT+CPIN=%d\r\n", 2028 );
											} break;
										case gpeALF_READY:
											pGSM->bPINrdy = true;
											break;
										default: break;
									}
								} break;
							case gpeALF_READY:
								pGSM->bPINrdy = true; /// ** PIN READY
								break;
							default: break;
						}
						nAT = 0;
						break;
					}

					if( nAT < 9 )
						break;

					sprintf( sANSW, "AT+CPIN?\r\n" );
					nAT = 0;
				} break;
				case gpeALF_CGMI: {
					gpmSTRCPY( pGSM->sCGMI, pSat+aAT[1].num+gpmNINCS(pSat+aAT[1].num, " \t\r\n") );
					pGSM->bCGMI = true;
					nAT = 0;
				} break;
				case gpeALF_CGMM: {
					gpmSTRCPY( pGSM->sCGMM, pSat+aAT[1].num+gpmNINCS(pSat+aAT[1].num, " \t\r\n") );
					pGSM->bCGMM = true;
					nAT = 0;
				} break;
				case gpeALF_CGMR: {
					gpmSTRCPY( pGSM->sCGMR, pSat+aAT[1].num+gpmNINCS(pSat+aAT[1].num, " \t\r\n") );
					pGSM->bCGMR = true;
					nAT = 0;
				} break;
				case gpeALF_COPS: {
					gpmSTRCPY( pGSM->sCOPS, pSat+aAT[1].num+gpmNINCS(pSat+aAT[1].num, " \t\r\n") );
					pGSM->bCOPS= true;
					nAT = 0;
				} break;
				case gpeALF_CGSN: {
					gpmSTRCPY( pGSM->sCGSN, pSat+aAT[1].num+gpmNINCS(pSat+aAT[1].num, " \t\r\n") );
					pGSM->bCGSN = true;
					nAT = 0;
				} break;

				default:
					break;
			}

		}

	}
	if( nWok < nAT )
		return iCNT;


	/// SEND -------------------------------------
	gpmDEL(pINP);
	if( sANSW[0] ) {
		pS = sANSW;
		pSe = pS;
		pSe += gpmVAN( pSe, "\r\n", nLEN );
		if( pSe-pS < 1 )
			return iCNT;
		pSe+=2;

	} else {
		//pS = pOUT ? (char*)pOUT->p_alloc+iCNT : NULL;
		if( !pOUT ) // !pS )
			return iCNT;
		bool bGD = false;
		switch(pGSM->sndLAST.alf){
			case gpeALF_CRESET: bGD = pGSM->bPINrdy; break;
			case gpeALF_CGMI: bGD = pGSM->bCGMI; break;
			case gpeALF_CGMM: bGD = pGSM->bCGMM; break;
			case gpeALF_CGMR: bGD = pGSM->bCGMR; break;
			case gpeALF_CGSN: bGD = pGSM->bCGSN; break;
			case gpeALF_COPS: bGD = pGSM->bCOPS; break;
			default: break;
		}
		if( bGD )
			iCNT = pGSM->sndLAST.num;
		pS = pOUT ? (char*)pOUT->p_alloc+iCNT : NULL;

		pSe = pS;
		pSe += gpmVAN( pSe, "\r\n", nLEN );
		if( pSe-pS < 1 )
			return iCNT;
		pGSM->sndLAST.num = pSe-pS;
		iCNT += pGSM->sndLAST.num;
		pGSM->sndLAST.num-=3;
		pGSM->sndLAST=pS+3;

		pSe += gpmNINCS( pSe, "\r\n" );
		pGSM->sndLAST.num = pSe-(char*)pOUT->p_alloc;
	}

	int nW = 0;
	while( pS < pSe ) {
		nW = write(socket,pS,pSe-pS);
		pS += nW;
	}
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
