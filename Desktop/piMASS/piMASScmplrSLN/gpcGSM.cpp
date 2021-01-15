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
char sGSMreset[] =	{
		"AT+CRESET\r\n"		/// CRESET
		"AT+CGMI\r\n"		// Module Manufacture
		"AT+CGMM\r\n"		// Module Modell
		"AT+CGSN\r\n"		// Serial Num
		"AT+CGMR\r\n"
		"AT+COPS?\r\n"		// provider

		"AT+CREG?\r\n"		// Network registration
		"AT+CPSI?\r\n"		// Inquiring UE system information
		"AT+CNMP=2\r\n"		// Preferred mode selection

		"AT+CGPS=1\r\n"
		"AT+CGPSCOLD\r\n"
		"1\r\n"

		"AT+CLCC?\r\n"
		"AT+CLCC\r\n"
		"AT+CLIP?\r\n"		// Calling line identification presentation
		"1\r\n"
};
char sGSMidle[] = {
	"AT+CNMI=2,1\r\n"		// new SMS?
		//"AT+CNMI?\r\n"		// new SMS?
	"AT+CGPSINFO\r\n"	// GPS info
	"1\r\n"
};
int aGSMcnt[0x100];
char sGPSoff[] = {
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
										};


char sSER[] = 	"/dev/%s"
				//"/dev/ttyAMA0"
;
class gpcCPSI{
public:
	gpeALF	SysMod, OpMod;
	U4		MCC, MNC, LAC,
			AbsRFChNm, TrkLOAdj,C1,C2,
			FrqBnd, PSC,
			Frq,SSC,ECIO,RSCP, Qual, RxLev, TXPWR, Cpid,TAC,PCellID, EUTRA,
			TransBWcfgDWNlnk,
			TransBWcfgUPlnk,

			SigREFrecPOW,
			SigREFrecQU,
			SigAvrREFratio
			;
	gpcCPSI(){};
	gpcCPSI& operator = ( char* pSat ) {

	}

};
class gpcCNMI{	// New MSG indic to TE
public:
	int mode,mt,bm,ds,bfr;
	gpcCNMI(){};
	gpcCNMI& operator = ( char* pSat ) {
		sscanf( pSat, "%d,%d,%d,%d,%d", &mode, &mt, &bm, &ds, &bfr );
		return *this;
	}
};
class gpcCPMS{	// New MSG indic to TE
public:
	/// SM\",\"SR\",\"ME
	gpeALF aM[4];
	int SMu,SMt,
		SRu,SRt,
		MEu,MEt;
	gpcCPMS(){};
	gpcCPMS& operator = ( char* pSat ) {
		int aI[6], n = sscanf( pSat, "%d,%d,%d,%d,%d,%d", 	aI,aI+1, aI+2, aI+3, aI+4, aI+5);
		for( U1 i = 0; i < n; i+=2 )
		switch( aM[i/2] )
		{
			case gpeALF_SM:
				SMu=aI[i];SMt=aI[i+1];
				break;
			case gpeALF_SR:
				SRu=aI[i];SRt=aI[i+1];
				break;
			case gpeALF_ME:
				MEu=aI[i];MEt=aI[i+1];
				break;

		}

		return *this;
	}
};
class gpcCGPS{	// New MSG indic to TE
public:
	float	lat; char ns;
	float	log; char ew;
	int		dat;
	float	utc, alt, spd;
	int 	curs;
	gpcCGPS(){};
	gpcCGPS& operator = ( char* pSat ) {
		sscanf( pSat, 	"%f,%c,"
						"%f,%c,"
						"%d,%f,%f,%f,%d",
				&lat, &ns,
				&log, &ew,
				&dat, 	&utc, &alt, &spd, 	&curs );
		return *this;
	}
};
class gpcGSM{
public:
	I8x2	sndLAST, is1,
			cmti;
	gpcCPSI	cpsi;
	gpcCPMS	cpms;
	gpcCNMI cnmi;
	gpcCGPS cgps;
	char	sCGMI[0x30],
			sCGMM[0x30],
			sCGMR[0x30],
			sCGSN[0x30],
			sCOPS[0x30];
	I4x2	CREG;
	int 	nCLCC, nCNT, iACTION, iREAD, nCMTI;
	bool	bPINrdy, bSMS, bPB,
			bCGMI, bCGMM, bCGMR, bCGSN, bCOPS,
			bCLIP,
			bCNT0, bCNT1;
	gpcLZY	AT;

	gpcGSM(){gpmCLR;};
	int nAT( I8x2* &pAT, int& iOK, int& iNO, int& iERR, int& iCMTI, bool& bQ1, char* pSat, int nSat ) {
		if( !&pAT )
			return 0;
		iOK = iNO = iERR = iCMTI = -1;
		if( pSat ? !nSat : true )
			return 0;
		U8 nLEN;
		int nAT = 0;
		I8x2* pATnAT;
		char* pSatI = pSat, *pSatE = pSat+nSat, aN[]=" ";
		for( pSatI += gpmNINCS(pSatI," \r\n\t:+,"); pSatI < pSatE; pSatI += gpmNINCS(pSatI," \r\n\t:+,"), nAT++ ) {
			if( !*pSat )
				break;
			pATnAT = (I8x2*)AT.Ux(nAT,sizeof(*pAT));
			pATnAT->y = pSatE-pSatI;
			*pATnAT = pSatI;
			if( !pATnAT->alf ) {
				nAT--;
				pSatI += gpmVAN(pSatI,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",nLEN);
				continue;
			}
			switch( pATnAT->alf ){
				case gpeALF_OK: 	iOK = nAT;break;
				case gpeALF_NO: 	iNO = nAT;break;
				case gpeALF_ERROR: 	iERR = nAT;break;
				case gpeALF_CMTI: 	iCMTI = nAT;break;
				default: break;
			}
			pSatI+=pATnAT->y;

			pATnAT->y = pSatI-pSat;
			switch( *pSatI ){
				case '?':
					if( nAT == 1 ) bQ1 = true;
				case '+':
				case '&':
				case ':':
					*aN = *pSatI;
					pATnAT->y+=gpmNINCS(pSatI, aN);
				default: break;
			}
		}
		if( &pAT )
			pAT = ((I8x2*)AT.Ux(0,sizeof(*pAT)));

		if( nAT > 1 )
			is1.alf = ((I8x2*)AT.Ux(1,sizeof(*pAT)))->alf; // pAT[1].alf;
		else
			is1.alf = gpeALF_null;
		return is1.num = nAT;
	}
	char* answCNMI( char* pANSW ) {
		return pANSW+sprintf( pANSW, "AT+CMGF=1\r\n" );
	}
	char* answCMGF( char* pANSW ) {
		m1 = m2 = m3 = gpeALF_SM;
		return pANSW+sprintf( pANSW, "AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n" );// "AT+CMGL=\"ALL\"\r\n" );
	}
	char* answCMGR( char* pANSW, int i ) {
		return pANSW+sprintf( pANSW, "AT+CMGR=%d\r\n", i );
	}
	char* answCSCS( char* pANSW, char* pS ) {
		return pANSW+sprintf( pANSW, "AT+CSCS=\"%s\"\r\n", pS );
	}
};
gpcGSM gsmZERO;

I8 gpcGT::GTgsm( gpcWIN* pWIN ) {
	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? iCNT : 0;
	gpcGT* pGT = this;
	char	sOUT[0x1000], sANSW[0x1000],
			*pS, *pSe,
			*pSat, *pSatI, *pSatE,
			*pSnum;
	int baud = 115200, aI[0x20], nAT=0,// nWT = 0, nWok = 0,
		iOK = 0, iERR = 0, iNO = 0, iCMTI = 0, pin = 2028; //5779; //2028; //16550;
	U8 s = -1, nLEN;
	I8x2 *pAT, aWT[0x10];
	bool bQ1 = false;
	if( socket == INVALID_SOCKET ) {
		sprintf( sOUT, sSER, s_ip );
		//if( (socket=(SOCKET)serialOpen("/dev/ttyS0",baud)) <0 ){
		if( (socket=(SOCKET)serialOpen( sOUT, baud )) < 0 ) {
			if(bSTDcout){std::cout << stdALU "GSM ERR:" << strerror(errno) << std::endl;}

			msGTdie = pWIN->mSEC.x + 3000;
			return iCNT;
		} else {
			int nCNT = 0;
			pOUT = pOUT->lzyADD( &gsmZERO, sizeof(gsmZERO), (s=0), -1 );
			iCNT = pOUT->nLD();
			pOUT = pOUT->lzyADD( sGSMreset, gpmSTRLEN(sGSMreset), (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMidle, gpmSTRLEN(sGSMidle), (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			gpcGSM	*pGSM = (gpcGSM*)pOUT->p_alloc;
			aGSMcnt[nCNT] = pOUT->nLD();
			++nCNT;
			pGSM->nCNT = nCNT;
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
	*sANSW = 0;
	pGSM->bCNT0 =
	pGSM->bCNT1 = false;

	if( pSat ) {
		if(bSTDcout){std::cout << pSat <<std::endl;}
		nAT = pGSM->nAT( pAT, iOK, iNO, iERR, iCMTI, bQ1, pSat, nINP );

		U4 at = 0;
		for( ; at < nAT; at++ ) {
			pAT[at].an2str( sOUT );
			if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
		}
		if(bSTDcout){std::cout << "nAT:" << at << std::endl;}
		while( iCMTI > -1 ) {
			pGSM->nCMTI++;
			pGSM->cmti.alf = pAT[iCMTI+1].alf;
			pGSM->cmti.num = gpfSTR2U8( pSat+pAT[iCMTI+1].num+2 );
			int a = pAT[iCMTI].num-8, b = (pAT[iCMTI+2].num-3), sub = b-a;
			gpmMcpy( pSat+a, pSat+b, nINP-b+1 );
			nINP -= sub;

			nAT = pGSM->nAT( pAT, iOK, iNO, iERR, iCMTI, bQ1, pSat, nINP );
			at = 0;
			for( ; at < nAT; at++ ) {
				pAT[at].an2str( sOUT );
				if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
			}
			if(bSTDcout){std::cout << "nAT:" << at << std::endl;}
		}

		switch( pGSM->is1.alf ) {
			case gpeALF_CMTI: {
				if( iOK < 0 ) {
					break;
				}
				sprintf( sANSW, "AT+CLCC\r\n" );
				nAT = 0;
				pGSM->bCNT0 = true;
			} break;
			case gpeALF_CNMI:			/// New MSG indic to TE
				if( nAT < 4 ) {			//2,1,0,0,0
					if( !iOK )
					break;
				}
				nAT = 0;
				if(bQ1)
					pGSM->cnmi = pSat+pAT[2].num;
				pGSM->answCNMI(sANSW);
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CMGF:
				nAT = 0;
				pGSM->answCMGF(sANSW);
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CMGL:
				if( iOK < 0 ) {
					break;
				}
				nAT = 0;
				pGSM->answCMGR(sANSW, 1);
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CPMS:
				if( iOK < 0 ) {
					break;
				}
				pGSM->cpms = pSat+pAT[5].num;
				nAT = 0;

				pGSM->answCSCS(sANSW, "GSM");
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CSCS:
				if( iOK < 0 ) {
					break;
				}
				nAT = 0;
				pGSM->answCMGR(sANSW, pGSM->iREAD);
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CMGR:
				if( iOK < 3 ) {
					break;
				}
				pGSM->iREAD++;
				nAT = 0;
				pGSM->bCNT0 = true;
				break;


			case gpeALF_CGPS:
				nAT = 0;
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CGPSCOLD:
				nAT = 0;
				pGSM->bCNT0 = true;
				break;
			case gpeALF_CGPSINFO:
				pGSM->cgps = pSat+pAT[2].num;
				nAT = 0;
				pGSM->bCNT0 = true;
				break;


			case gpeALF_CRESET: {
					if( nAT < 7 ) {
						break;
					}
					switch( pAT[4].alf ) {
						case gpeALF_CPIN: {
								switch( pAT[5].alf ){
									case gpeALF_SIM: {
											switch( pAT[6].alf ){
												case gpeALF_PIN: {
														sprintf( sANSW, "AT+CPIN=%d\r\n", pin );
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

				if( pAT[2].alf == gpeALF_CPIN )
				if( bQ1 ) {
					switch( pAT[3].alf ) {
						case gpeALF_SIM: {
								switch( pAT[4].alf ){
									case gpeALF_PIN: {
											sprintf( sANSW, "AT+CPIN=%d\r\n", pin );
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
				for( U4 i = iOK; i < nAT; i++ ){
					switch( pAT[i].alf ){
						case gpeALF_CPIN:
							pGSM->bPINrdy = (pAT[i+1].alf==gpeALF_READY);
							break;
						case gpeALF_SMS:
							pGSM->bSMS = (pAT[i+1].alf==gpeALF_DONE);
							break;
						case gpeALF_PB:
							pGSM->bPB = (pAT[i+1].alf==gpeALF_DONE);
							break;
						default: break;
					}

				}
				sprintf( sANSW, "AT+CPIN?\r\n" );
				nAT = 0;
			} break;
			case gpeALF_CGMI: {
				gpmSTRCPY( pGSM->sCGMI, pSat+pAT[1].num+gpmNINCS(pSat+pAT[1].num, " \t\r\n") );
				pGSM->bCGMI = true;
				nAT = 0;
			} break;
			case gpeALF_CGMM: {
				gpmSTRCPY( pGSM->sCGMM, pSat+pAT[1].num+gpmNINCS(pSat+pAT[1].num, " \t\r\n") );
				pGSM->bCGMM = true;
				nAT = 0;
			} break;
			case gpeALF_CGMR: {
				gpmSTRCPY( pGSM->sCGMR, pSat+pAT[1].num+gpmNINCS(pSat+pAT[1].num, " \t\r\n") );
				pGSM->bCGMR = true;
				nAT = 0;
			} break;
			case gpeALF_COPS: {
				gpmSTRCPY( pGSM->sCOPS, pSat+pAT[1].num+gpmNINCS(pSat+pAT[1].num, " \t\r\n") );
				pGSM->bCOPS= true;
				nAT = 0;
			} break;
			case gpeALF_CGSN: {
				gpmSTRCPY( pGSM->sCGSN, pSat+pAT[1].num+gpmNINCS(pSat+pAT[1].num, " \t\r\n") );
				pGSM->bCGSN = true;
				nAT = 0;
			} break;
			case gpeALF_CPSI: {
				if( iOK < 0 )
				{

					break;
				}
				pGSM->cpsi.SysMod = pAT[3].alf;
				pGSM->cpsi.OpMod = pAT[4].alf;
				/*char *pSnum = pSat+pAT[4].num+1;
				pGSM->cpsi.MCC 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.MNC 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.LAC 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.AbsRFChNm 		= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.TrkLOAdj 		= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.C1 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.C2 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.FrqBnd 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.PSC 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.Frq 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.SSC 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.ECIO 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.RSCP 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.Qual 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.RxLev 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.TXPWR 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.Cpid 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.TAC 				= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.PCellID 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.EUTRA 			= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.TransBWcfgDWNlnk = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.TransBWcfgUPlnk 	= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.SigREFrecPOW 	= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.SigREFrecQU 		= gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->cpsi.SigAvrREFratio	= gpfSTR2U8( pSnum, &pSnum ); pSnum++;*/

				nAT = 0;
				pGSM->bCNT0 = true;
			} break;
			case gpeALF_CREG: {
				if( iOK < 0 ){
					break;
				}

				char *pSnum = pSat+pAT[2].num;
				pGSM->CREG.x = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				pGSM->CREG.y = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
				if( bQ1 ) {
					if( pGSM->CREG.y == 2 )
						sprintf( sANSW, "AT+CREG=2\r\n" );
				}
				nAT = 0;
				pGSM->bCNT0 = true;
			} break;
			case gpeALF_CNMP: {
				nAT = 0;
				pGSM->bCNT1 = true;
			} break;
			case gpeALF_CLCC: {
				if(bQ1) {
					pGSM->nCLCC = gpfSTR2U8( pSat+pAT[2].num+1 ); //, &pSnum );
					if( pGSM->nCLCC ) sprintf( sANSW, "AT+CLCC\r\n" );
					nAT = 0;
					break;
				}
				nAT = 0;
			} break;
			case gpeALF_CLIP: {
				nAT = 0;
				pGSM->bCLIP = true;
			} break;
			default: break;
		}
	} else if( pGSM->nCMTI ) {
		pGSM->nCMTI = 0;
		sprintf( sANSW, "AT+CMGL=\"ALL\"\r\n" );
		nAT = 0;
		pGSM->bCNT0 = true;
	}

	if( nAT )
		return iCNT;


	/// SEND -------------------------------------
	gpmDEL(pINP);
	if( *sANSW ) {
		pS = sANSW;
		pSe = pS;
		pSe += gpmVAN( pSe, "\r\n", nLEN );
		if( pSe-pS < 1 )
			return iCNT;
		pSe+=2;
		if(bSTDcout){std::cout << stdRUN << sANSW <<std::endl;}
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

			case gpeALF_CLCC: bGD = pGSM->nCLCC; break;
			case gpeALF_CLIP: bGD = pGSM->bCLIP; break;	//Calling line identification presentation

			case gpeALF_CNMP:
				bGD = pGSM->bCNT1; break;
			case gpeALF_CPSI:
			default: bGD = pGSM->bCNT0; break;
		}
		if( bGD )
			iCNT = pGSM->sndLAST.num;
		pS = pOUT ? (char*)pOUT->p_alloc+iCNT : NULL;

		pSe = pS;
		pSe += gpmVAN( pSe, "\r\n", nLEN );
		if( pSe-pS < 1 )
			return iCNT;
		if( *pS != 'A' ) {
			iCNT = aGSMcnt[pGSM->iACTION = gpfSTR2U8(pS)];
			pS = (char*)pOUT->p_alloc+iCNT;
			pSe = pS;
			pSe += gpmVAN( pSe, "\r\n", nLEN );
		}
		pGSM->sndLAST.num = pSe-pS;
		iCNT += pGSM->sndLAST.num;
		pGSM->sndLAST.num-=3;
		pGSM->sndLAST=pS+3;

		pSe += gpmNINCS( pSe, "\r\n" );
		pGSM->sndLAST.num = pSe-(char*)pOUT->p_alloc;
		gpmMcpy( sANSW, pS, pSe-pS )[pSe-pS] = 0;
		if(bSTDcout){std::cout << stdRUN << sANSW <<std::endl;}
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
