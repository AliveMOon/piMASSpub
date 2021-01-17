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
char sGSMreset[] =	{		/// 0
		"AT+CRESET\r\n"		/// CRESET
		"AT+CGMI\r\n"		// Module Manufacture
		"AT+CGMM\r\n"		// Module Modell
		"AT+CGSN\r\n"		// Serial Num
		"AT+SIMEI?\r\n"		//
		"AT+CGMR\r\n"
		"AT+COPS?\r\n"		// provider

		"AT+CREG?\r\n"		// Network registration
		"AT+CPSI?\r\n"		// Inquiring UE system information
		"AT+CNMP=2\r\n"		// Preferred mode selection

		"AT+CGPS=1\r\n"
		"AT+CNMI=2,1\r\n"	// new SMS?
		"AT+CSDVC=1\r\n"	// call handset on
		//"AT+CLVL=4\r\n"	// volume 5
		"AT+CLIP=1\r\n"		// Calling line identification presentation
		"1\r\n"
};
char sGSMidle[] = {		/// 1
	"AT+CSQ\r\n"
	"AT+CGPSINFO\r\n"	// GPS info
	"1\r\n"
};
char sGSMwait[] = {		/// 2
	"AT+CSQ\r\n"
	"AT+CGPSINFO\r\n"	// GPS info
	"w 10000 2\r\n"		// WAIT
	"3\r\n"
};
char sGSMhup[] = {		/// 3
	"AT+CLVL=0\r\n"		// volume 5
	"AT+CSQ\r\n"
	"AT+CHUP\r\n"		// Hang up call
	//"AT+CLIP=1\r\n"
	"1\r\n"
};
char sGSMansw[] = {		/// 4
	"AT+CLVL=4\r\n"		// volume 5
	"ATA\r\n"
	"5\r\n"
};
char sGSMchat[] = {		/// 5
	"AT+CSQ\r\n"
	"AT+CGPSINFO\r\n"	// GPS info
	"5\r\n"
};

int aGSMcnt[0x100];

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
			default: break;
		}

		return *this;
	}
};
class gpcCLIP {
public:
	char sSTR[0x200];
	I8 aI[0x10];
	gpcCLIP(){};
	gpcCLIP& operator = ( char* pSat ) {
		U4 i = 0, n;
		U8 nLEN;
		gpmCLR;
		for( pSat += gpmNINCS(pSat," \r\n\t:+"); *pSat; pSat += gpmNINCS(pSat," \r\n\t:+") ) {
			pSat++;
			switch(pSat[-1]){
				case '\"':
					n = gpmVAN(pSat,"\"",nLEN);
					aI[i] = gpfSTR2I8(gpmMcpy( sSTR+i*0x20, pSat, n ));
					pSat += n+1;
					break;
				case ',':
					i++;
					aI[i] = gpfSTR2I8(pSat,&pSat,"," );
					break;
				default: break;
			}
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
class gpcCREG {
public:
	I4x2 	reg;
	int		stat,lac,ci;
	gpcCREG(){};
	gpcCREG& operator = ( char* pSat ) {
		sscanf( pSat, "%d,%x,%x", stat, lac, ci );
		return *this;
	}
};
class gpcGSM {
public:
	I8x2	sndLAST, aIS[2];
	I4x2	aMnMx[gpeAT_N];
	gpcCPSI	cpsi;
	gpcCPMS	cpms;
	gpcCNMI cnmi;
	gpcCGPS cgps;
	//gpcCLIP clip;
	char	sCGMI[0x30],
			sCGMM[0x30],
			sCGMR[0x30],
			sCGSN[0x30],
			sCOPS[0x30],
			sIMEI[0x10];
	gpcCREG	CREG;
	I4x2 	CSQ;
	int 	nCLCC, nCNT, iACTION, iREAD,
			nCMTI, nCLIP, nRING, iW,
			iCMTI, iCLIP;
	bool	bPINrdy, bSMS, bPB,

			bATA,

			bCGMI, bCGMM, bCGMR, bCGSN, bCOPS,
			bCLIP,
			bCNT0, bCNT1;
	gpcLZY	AT, clip, cmti;

	gpcGSM(){gpmCLR;};
	I8x2* pAT( int& nAT, char* pSat, int nSat ) {
		nAT = AT.nAT( pSat, nSat );
		if( !nAT )
			return NULL;
		return (I8x2*)AT.p_alloc;
	}
	int nAT( I8x2* &pAT0, int& iAT, int& iQ1, int& iOK, int& iNO, int& iERR, int& iCMTI, int& iCLIP, int& iRING, int& iMISS, char* pSat, int nSat ) {
		if( !&pAT0 )
			return 0;
		int nAT;
		pAT0 = pAT( nAT, pSat, nSat );

		aMnMx->x = -1;
		gpfMset( (&aMnMx->x)+1, gpeAT_N*2-1, (&aMnMx->x), sizeof(aMnMx->x) );

		iQ1 = iOK = iNO = iERR = iCMTI = iCLIP = iRING = iMISS = -1;
		gpeAT j,x;
		for( int i = 0; i < nAT; i++ ){
			switch( pAT0[i].alf ){
				case gpeALF_NO: 		j = gpeAT_no; break;
				case gpeALF_OK: 		j = gpeAT_ok; break;
				case gpeALF_AT:			j = gpeAT_at; break;
				case gpeALF_ERROR: 		j = gpeAT_err; break;
				case gpeALF_CMTI: 		j = gpeAT_cmti; break;
				case gpeALF_CLIP: 		j = gpeAT_clip; break;
				case gpeALF_RING: 		j = gpeAT_ring; break;
				case gpeALF_MISSING:
				case gpeALF_MISSED: 	j = gpeAT_miss; break;
				default: j = gpeAT_N;
						break;
			}
			switch( pSat[pAT0[i].num] ){
				case '?': 	x = gpeAT_noQ;
							if(i!=1)
								break;
							iQ1 = 1;
							break;
				case '+': x = gpeAT_noP; break;
				case '&': x = gpeAT_noA; break;
				case ':': x = gpeAT_noPP; break;
				default: x = gpeAT_no; break;
			}
			if( x )
				pAT0[i].num += gpmNINCS( pSat+pAT0[i].num, " \r\n\t?+&:" );
			if( j >= gpeAT_N )
				continue;

			if(aMnMx[j].x < 0)
				aMnMx[j].x = i;
			aMnMx[j].y = i;
			if( !x )
				continue;

			if(aMnMx[x+j].x < 0)
				aMnMx[x+j].x = i;
			aMnMx[x+j].y = i;
		}
		iAT = aMnMx[gpeAT_atP].x;	// AT+ első
		iOK = aMnMx[gpeAT_ok].y;	// utolso
		iNO = aMnMx[gpeAT_no].y;	// utolso
		iERR = aMnMx[gpeAT_err].y;	// utolso
		iCMTI = aMnMx[gpeAT_cmtiPP].x;	// : első (PP)
		iCLIP = aMnMx[gpeAT_clipPP].x;	// : első (PP)
		iRING = aMnMx[gpeAT_ring].x;	// első
		iMISS = aMnMx[gpeAT_miss].x;	// első

		aIS[0] = pAT0[0];

		if( nAT > 1 )
			aIS[1].alf = pAT0[1].alf; // pAT[1].alf;
		else
			aIS[1].alf = gpeALF_null;
		return aIS[1].num = nAT;
/*
		if( pSat ? !nSat : true )
			return 0;
		U8 nLEN;
		int nAT = 0, nCLIPpp= 0;
		I8x2* pATnAT;
		char* pSatI = pSat, *pSatE = pSat+nSat, aN[]=" ";
		for( 	pSatI += gpmNINCS(pSatI," \r\n\t:+,");
				pSatI < pSatE;
				pSatI += gpmNINCS(pSatI," \r\n\t:+,"), nAT++ ) {

			if( !*pSatI )
				break;

			pATnAT = (I8x2*)AT.Ux(nAT,sizeof(*pAT));
			pATnAT->y = pSatE-pSatI;
			*pATnAT = pSatI;
			if( !pATnAT->alf ) {
				nAT--;
				pSatI += gpmVAN(pSatI,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",nLEN);
				continue;
			}
			pSatI+=pATnAT->y;
			pATnAT->y = pSatI-pSat;

			switch( pATnAT->alf ){
				case gpeALF_OK: 	iOK = nAT;break;
				case gpeALF_NO: 	iNO = nAT;break;
				case gpeALF_ERROR: 	iERR = nAT;break;
				case gpeALF_CMTI: 	iCMTI = nAT;break;
				case gpeALF_CLIP: 	if(iCLIP < 0)
									{
										iCLIP = nAT;
										break;
									}

									if( *pSatI == ':' ) {
										nCLIPpp++;
										if( nCLIPpp > 1 )
											break;
									}

									iCLIP = nAT;
									break;
				case gpeALF_RING: 	iRING = nAT;break;
				default: break;
			}


			switch( *pSatI ){
				case '?':
					if( nAT == 1 ) iQ1 = 1;
				case '+':
				case '&':
				case ':':
					*aN = *pSatI;
					pATnAT->y+=gpmNINCS(pSatI, aN);
				default: break;
			}
		}
		I8x2* p_at = ((I8x2*)AT.Ux(0,sizeof(*p_at)));
*/
	}
	char* answCNMI( char* pANSW ) {
		return pANSW+sprintf( pANSW, "AT+CMGF=1\r\n" );
	}
	char* answCMGF( char* pANSW ) {
		cpms.aM[0] = cpms.aM[1] = cpms.aM[2] = gpeALF_SM;
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
	int baud = 115200, aI[0x20],
		iAT = 0, nAT=0,// nWT = 0, nWok = 0,
		iQ1 = 0, iOK = 0, iERR = 0, iNO = 0, iCMTI = 0, iCLIP = 0, iRING = 0, iMISS = 0, pin = 2028; //5779; //2028; //16550;
	U8 s = -1, nLEN;
	I8x2 *pAT, aWT[0x10];
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

			pOUT = pOUT->lzyADD( sGSMwait, gpmSTRLEN(sGSMwait), (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMhup, gpmSTRLEN(sGSMhup), (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMansw, gpmSTRLEN(sGSMansw), (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMchat, gpmSTRLEN(sGSMchat), (s=-1), -1 );
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
	pGSM->bATA =
	pGSM->bCNT0 =
	pGSM->bCNT1 = false;

	//bool bFREEinp = true;
	if( pSat ) {
		if(bSTDcout){std::cout << pSat <<std::endl;}
		nAT = pGSM->nAT( pAT, iAT, iQ1, iOK, iNO, iERR, iCMTI, iCLIP, iRING, iMISS, pSat, nINP );

		U4 at = 0;
		for( ; at < nAT; at++ ) {
			pAT[at].an2str( sOUT );
			if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
		}
		if(bSTDcout){std::cout << "nAT:" << at << std::endl;}

//-------------------------------------------------------------------------------------
		///-------------------------------------------------------------------\\
		/// CALL RING
		while( iRING > -1 ) {
			pGSM->nRING++;
			U8 a = pAT[iRING].num-7, b = pAT[iRING].num+3, sub = b-a;
			pINP->lzySUB( a, sub );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			nAT = pGSM->nAT( pAT, iAT, iQ1, iOK, iNO, iERR, iCMTI, iCLIP, iRING, iMISS, pSat, nINP );
			at = 0;
			for( ; at < nAT; at++ ) {
				pAT[at].an2str( sOUT );
				if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
			}
			if(bSTDcout){std::cout << "nAT:" << at << std::endl;}
		}
		/// CALL CLIP Calling line identification presentation
		while( pGSM->nRING && (iCLIP>-1) ) {
			if( pGSM->nRING > 0 )
				pGSM->nRING--;

			gpcCLIP* pCn = (gpcCLIP*)pGSM->clip.Ux( pGSM->nCLIP, sizeof(*pCn) ), *pC0;
			if( pCn ){
				pC0 = pCn-pGSM->nCLIP;
				*pCn = pSat + pAT[iCLIP].num;
				I4 j = pGSM->nCLIP;
				for( U4 i = 0; i<pGSM->nCLIP; i++ ){
					if( pC0[i].aI[0] != pCn->aI[0] )
						continue;

					pC0[j=i].aI[0xf]++;
					i=pGSM->nCLIP;
				}
				if( j >= pGSM->nCLIP )
					pGSM->nCLIP++;
			}

			U8 a = pAT[iCLIP].num, b = gpmVAN(pSat+a,"\r\n", nLEN), a8 = a-8;
			pINP->lzySUB( a8, b+8 );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			nAT = pGSM->nAT( pAT, iAT, iQ1, iOK, iNO, iERR, iCMTI, iCLIP, iRING, iMISS, pSat, nINP );
			at = 0;
			for( ; at < nAT; at++ ) {
				pAT[at].an2str( sOUT );
				if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
			}
			if(bSTDcout){std::cout << "nAT:" << at << std::endl;}
			//bFREEinp = false;
		}
		while( iMISS>-1 ) {
			char	*pNs = pSat + pAT[iMISS].num-6, *pNe = pSat + pAT[iMISS+2].num;
			I8 pnum = gpfSTR2I8( pNe, &pNe );

			gpcCLIP* pCn = (gpcCLIP*)pGSM->clip.Ux( pGSM->nCLIP, sizeof(*pCn) ), *pC0;
			if( pCn ){
				pC0 = pCn-pGSM->nCLIP;
				I4 j = pGSM->nCLIP;
				for( U4 i = 0; i<pGSM->nCLIP; i++ ){
					if( pC0[i].aI[0] != pnum )
						continue;
					pC0[i].aI[0xf] = 0;
					pGSM->nCLIP--;
					if( i < pGSM->nCLIP )
						continue;
					pC0[i] = pC0[pGSM->nCLIP];
					i--;
				}

			}

			pINP->lzySUB( s = pNs-pSat, pNe-pNs );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			nAT = pGSM->nAT( pAT, iAT, iQ1, iOK, iNO, iERR, iCMTI, iCLIP, iRING, iMISS, pSat, nINP );
			at = 0;
			for( ; at < nAT; at++ ) {
				pAT[at].an2str( sOUT );
				if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
			}
			if(bSTDcout){std::cout << "nAT:" << at << std::endl;}
		}
		/// CALL
		///-------------------------------------------------------------------//
 //-------------------------------------------------------------------------------------
		///-------------------------------------------------------------------\\
		/// SMS New MSG indication || AT+CNMI=2,1 . . . .... +CMTI:
		while( iCMTI > -1 ) {
			I8x2* pCMIT = (I8x2*)pGSM->cmti.Ux( pGSM->nCMTI, sizeof(*pCMIT) );
			if( pCMIT ) {
				pCMIT->alf = pAT[iCMTI+1].alf;
				pCMIT->num = gpfSTR2I8( pSat+pAT[iCMTI+1].num+2 );
				pGSM->nCMTI++;
				if( pCMIT->num > -1 )
					pGSM->iREAD = pCMIT->num;
			}

			U8 a = pAT[iCMTI].num-8, b = (pAT[iCMTI+2].num-3);
			pINP->lzySUB( a, b-a );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			//gpmMcpy( pSat+a, pSat+b, nINP-b+1 );
			//nINP -= sub;

			nAT = pGSM->nAT( pAT, iAT, iQ1, iOK, iNO, iERR, iCMTI, iCLIP, iRING, iMISS, pSat, nINP );
			at = 0;
			for( ; at < nAT; at++ ) {
				pAT[at].an2str( sOUT );
				if(bSTDcout){ std::cout << at <<"."<< sOUT << " "; }
			}
			if(bSTDcout){std::cout << "nAT:" << at << std::endl;}
			//bFREEinp = false;
		}
		/// SMS
		///-------------------------------------------------------------------//

		switch( pGSM->aIS[0].alf ) {
			case gpeALF_ATA: {
					if( iOK < 0 )
						break;
					nAT = 0;
					pGSM->bCNT0 = true;
				} break;
			case gpeALF_CREG: {

					pGSM->CREG = pSat+pAT[0].num;
					nAT = 0;
					pGSM->bCNT0 = true;
				} break;
			default: {
				switch( pGSM->aIS[1].alf ) {
					case gpeALF_SIMEI: {
							if( iOK < 0 )
								break;
							gpmMcpy( pGSM->sIMEI, pSat+pAT[2].num,0xf )[0xf] = 0;
							nAT = 0;
							pGSM->bCNT0 = true;
						} break;
					case gpeALF_CSQ: {
							if( iOK < 0 )
								break;

							sscanf( pSat+pAT[2].num, "%d,%d", &pGSM->CSQ.x, &pGSM->CSQ.y);
							nAT = 0;
							pGSM->bCNT0 = true;
						} break;
					case gpeALF_CLIP:
						nAT = 0;
						pGSM->bCLIP = true;
						break;
					case gpeALF_CLVL:
						nAT = 0;
						pGSM->bCNT0 = true;
						break;
					case gpeALF_CSDVC:
						nAT = 0;
						pGSM->bCNT0 = true;
						break;
					case gpeALF_CMTI: {
							if( iOK < 0 )
								break;

							sprintf( sANSW, "AT+CLCC\r\n" );
							nAT = 0;
							pGSM->bCNT0 = true;
						} break;
					case gpeALF_CNMI: 			/// New MSG indic to TE
						if( nAT < 4 ) {			//2,1,0,0,0
							if( iOK < 0 )
								break;
						}
						nAT = 0;
						if(iQ1>0)
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
						if( iOK < 0 )
							break;
						nAT = 0;
						pGSM->answCMGR(sANSW, 1);
						pGSM->bCNT0 = true;
						break;
					case gpeALF_CPMS:
						if( iOK < 0 )
							break;
						pGSM->cpms = pSat+pAT[5].num;
						nAT = 0;
						pGSM->iREAD = gpmMAX( 0, pGSM->cpms.SMu-1 );
						pGSM->answCSCS(sANSW, "GSM");
						pGSM->bCNT0 = true;
						break;
					case gpeALF_CSCS:
						if( iOK < 0 )
							break;
						nAT = 0;
						pGSM->answCMGR(sANSW, pGSM->iREAD );
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
						if( nAT >= 6 )
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
						if( iQ1>0 ) {
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
						if( iOK < 0 ) {
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
						pGSM->CREG.reg.x = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
						pGSM->CREG.reg.y = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
						if(iQ1>0) {
							if( pGSM->CREG.reg.y == 2 )
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
						if(iQ1>0) {
							pGSM->nCLCC = gpfSTR2U8( pSat+pAT[2].num+1 ); //, &pSnum );
							if( pGSM->nCLCC ) sprintf( sANSW, "AT+CLCC\r\n" );
							nAT = 0;
							break;
						}
						nAT = 0;
					} break;

					default: break;
				}
			} break;
		}
	}

	if( nAT )
		return iCNT;


	/// SEND -------------------------------------
	//if( bFREEinp )
	//U8 s = 0;
	if( iAT > 0 )
		pINP->lzySUB(s=0,pAT[iAT].num-3);
	else
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

		pS = (char*)pOUT->p_alloc+iCNT;
		pSe = pS + gpmVAN( pS, "\r\n", nLEN );
		if( pSe-pS < 1 )
			return iCNT;
		U8 ms, act, oACT = pGSM->iACTION;
		while( *pS != 'A' ) {
			switch( *pS ) {
				case 'w':
				case 'W':
					ms = gpfSTR2U8(pS, &pS);
					act = gpfSTR2U8(pS, &pS);

					if( pGSM->iW >= pWIN->mSEC.x ) {
						iCNT = aGSMcnt[pGSM->iACTION = act];
						pS = (char*)pOUT->p_alloc + iCNT;
						break;
					} else if( !pGSM->iW ) {
						pGSM->iW = ms + pWIN->mSEC.x;
						iCNT = aGSMcnt[pGSM->iACTION = act];
						pS = (char*)pOUT->p_alloc + iCNT;
						break;
					}
					// következő sor
					pS += gpmVAN( pS, "\r\n", nLEN );
					pS += gpmNINCS( pS, "\r\n" );
					break;
				default: {
						switch( pGSM->iACTION = gpfSTR2U8(pS) ) {
							case 1:
								if( pGSM->iCLIP < pGSM->nCLIP ) {
									pGSM->iACTION = 2;
									iCNT = aGSMcnt[pGSM->iACTION];
									pS = (char*)pOUT->p_alloc+iCNT;

									pGSM->iCLIP++;
									break;
								}
							case 5:
								if( pGSM->iCMTI < pGSM->nCMTI ) {
									I8x2* pCMTI = (I8x2*)pGSM->cmti.Ux(pGSM->iCMTI,sizeof(*pCMTI));
									pS = pGSM->answCMGR( sANSW, pCMTI->num );

									pGSM->iCMTI++;
									break;
								}
							default:
								iCNT = aGSMcnt[pGSM->iACTION];
								pS = (char*)pOUT->p_alloc+iCNT;
								break;
						}
					} break;
			}
			pSe = pS + gpmVAN( pS, "\r\n", nLEN );
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
