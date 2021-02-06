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
		"AT+CGPS=1\r\n"
		//"AT+CGPSCOLD\r\n"
		//"AT+CGPSAUTO=1\r\n"

		"AT+CGMI\r\n"		// Module Manufacture
		"AT+CGMM\r\n"		// Module Modell
		"AT+CGSN\r\n"		// Serial Num
		"AT+CGMR\r\n"		// Firmware revision
		//"AT+SIMEI?\r\n"	// IMEI num

		"1\r\n"
};
char sGSMreg[] = {		/// 1	REG
	"AT+CREG?\r\n"		/// Network registration
	"AT+COPS?\r\n"		// Operator selection
	"AT+CPSI?\r\n"		// Inquiring UE system information
	"AT+CNMP=2\r\n"		// Preferred mode selection //AUTO

	//"AT+CSDVC=3" 		// Switch voice channel device	1hndst 3spkr

	"AT+CNMI=2,1\r\n"	// New message indications to TE
	"AT+CLIP=1\r\n"		// Calling line identification presentation

	"2\r\n"
};
char sGSMidle[] = {		/// 2	IDLE
	"AT+CSQ\r\n"
	"AT+CGPSINFO\r\n"	// GPS info
	"AT+CREG?\r\n"		/// Network registration
	"2\r\n"
};
char sGSMwait[] = {		/// 3	WAIT ATA
	"AT+CSQ\r\n"
	"AT+CGPSINFO\r\n"	// GPS info
	"w 3 5\r\n"			// WAIT
	"4\r\n"				// 4 	VOICE CALL END
};
char sGSMhup[] = {		/// 4 	VOICE CALL END
	"AT+CLVL=0\r\n"		// volume 5
	"AT+CSQ\r\n"
	"AT+CHUP\r\n"		// Hang up call
	//"AT+CLIP=1\r\n"
	"2\r\n"
};
char sGSMansw[] = {		/// 5 	VOICE CALL BEGIN
	"AT+CLVL=4\r\n"		// volume 5
	"ATA\r\n"
	"6\r\n"				// 6 CHAT
};
char sGSMchat[] = {		/// 6	VOICE CALL chat
	"AT+CSQ\r\n"
	"AT+CGPSINFO\r\n"	// GPS info
	"6\r\n"				// 6 CHAT
};
char sGSMcall[] = {		/// 7 	VOICE CALL BEGIN
	"AT+CSDVC=1\r\n"
	"AT+CLVL=4\r\n"		// volume 5
	"ATD%s\r\n"
	"6\r\n"				// 6 CHAT
};
typedef enum gpeGPSact:U4
{
	gpeGPSact_RESET,
	gpeGPSact_REG,
	gpeGPSact_IDLE,
	gpeGPSact_WAIT,
	gpeGPSact_HUP,
	gpeGPSact_ANSW,
	gpeGPSact_CHAT,
	gpeGPSact_CALL,
	gpeGPSact_N,
} gpeGPSactU4;
#define gpdACTn gpeGPSact_N

char* asACTclr[] = {
	stdRESET,			// RESTE
	stdCYAN,			// REG
	stdYELLOW,			// IDLE
	stdPURPLE,			// WAIT
	stdRED,				// end
	stdGREEN,			// begin
	stdBLUE,			// chat
	stdPURPLE,			// CALL
};
char* asACTname[] = {
				stdYELLOW "-+INIT+-" stdRESET	" REG - IDLE - WAIT - BEGI - CHAT - END - CALL"			stdRESET,	// 0
	stdRESET	"- INIT " stdCYAN "-+REG+-" stdRESET	" IDLE - WAIT - BEGI - CHAT - END - CALL"		stdCYAN,	// 1
	stdRESET	"- INIT - REG"	stdPURPLE " -+IDLE+-" stdRESET	" WAIT - BEGI - CHAT - END - CALL"		stdPURPLE,	// 2
	stdRESET	"- INIT - REG - IDLE"	stdWHITE " -+WAIT+-" stdRESET	" BEGI - CHAT - END - CALL"		stdWHITE,	// 3
	stdRESET	"- INIT - REG - IDLE - WAIT - BEGI - CHAT"	stdRED " -+END+-" stdRESET	" CALL"			stdRED,		// 4
	stdRESET	"- INIT - REG - IDLE - WAIT"	stdGREEN " -+BEGI+-" stdRESET	" CHAT - END - CALL"	stdGREEN,	// 5
	stdRESET	"- INIT - REG - IDLE - WAIT - BEGI"	stdBLUE " -+CHAT+-" stdRESET	" END - CALL"		stdBLUE,	// 6
	stdRESET	"- INIT - REG - IDLE - WAIT - BEGI - CHAT - END "	stdPURPLE " -+CALL" 				stdPURPLE,	// 7

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
	float 	curs;
	gpcCGPS(){};
	gpcCGPS& operator = ( char* pSat ) {
		sscanf( pSat, 	"%f,%c,"
						"%f,%c,"
						"%d,%f,%f,%f,%f",
				&lat, &ns,
				&log, &ew,
				&dat, 	&utc, &alt, &spd, 	&curs );
		return *this;
	}
};
class gpcCREG {
public:
	int		n, stat,lac,ci;
	gpcCREG(){};
	gpcCREG& operator = ( char* pSat ) {
		gpmCLR;
		U8 nLEN;
		U4 nCOM = 0, nS = gpmVAN(pSat, "\r\n", nLEN );
		for( U4 i = 0; i < nS; i++ )
			if( pSat[i] == ',' ) nCOM++;
		stat = gpfSTR2I8(pSat, &pSat );
		if( !nCOM )
			return *this;

		pSat++;
		switch( nCOM )
		{
			case 1:
				sscanf( pSat, "%x", lac );
				break;
			case 2:
				sscanf( pSat, "%x %x", lac, ci );
				break;
			default:
				break;
		}
		return *this;
	}
};
static const gpeALF aALFok[] = {
	gpeALF_AT,
	gpeALF_ATA,
	gpeALF_ATD,
	gpeALF_OK,
	gpeALF_ERROR,
	gpeALF_PLUS,
	gpeALF_DONE,
	gpeALF_READY,
};
static const gpeALF aALFmost[] = {
	gpeALF_AT,
	gpeALF_ATA,
	gpeALF_ATD,
	gpeALF_OK,
	gpeALF_ERROR,
	gpeALF_PLUS,
	gpeALF_DONE,
	gpeALF_READY,
	gpeALF_RDY,
};
static const gpeALF aALFat[] = {
	gpeALF_AT,
	gpeALF_ATD,
	gpeALF_ATA,
	gpeALF_CPIN,
	gpeALF_CREG,
	gpeALF_CLIP,
	gpeALF_CMTI,
	gpeALF_VOICE,
	gpeALF_MISSED,
	gpeALF_RDY,
	gpeALF_SMS,
	gpeALF_PB,
};

static const gpeALF aALFvoice[] = {
	gpeALF_begin,
	gpeALF_end,
};

static const gpeALF aALFampm[] = {
	gpeALF_AM,
	gpeALF_PM,
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
	char	sSTAT[0x400],
			sSTR[0x400],
			sPUB[0x400],
			sOUT[0x100],
			sCGMI[0x30],
			sCGMM[0x30],
			sCGMR[0x30],
			sCGSN[0x30],
			sCOPS[0x30],
			sIMEI[0x10];

	I8		aNUM[0x10];
	gpcCREG	CREG;
	I4x2 	CSQ;
	int 	nCLCC, nCNT, iACTION, iREAD,
			nCMTI, nCLIP, nRING, iW, nATA,
			iCMTI, iCLIP, iCREG0;
	bool	bGSMrdy, bPINrdy, bGPS,
			bSMS, bSMSfull,
			bPB,

			bATA,

			bCGMI, bCGMM, bCGMR, bCGSN, bCOPS,
			bCLIP,
			bCNT0, bCNT1;
	gpcLZY	AT, clip, cmti;

	gpcGSM(){gpmCLR;};
	I8x2* pAT( int& nAT, char* pS, int nS, const char* pFILT = " \r\n\t:+," ) {
		nAT = AT.nAT( pS, nS, pFILT );
		if( !nAT )
			return NULL;
		return (I8x2*)AT.p_alloc;
	}
	int nAT( I8x2* &pAT0,
						int& iAT, int& iQ1, int& iOK, int& iNO, int& iERR,
						int& iCMTI, int& iCLIP,
						int& iRING, int& iMISS,
						int& iVOICE, int& iCALL,
						int& iBEGIN, int& iEND,

						int& iMRK, int& iCREG,

						char* pSat, int nSat ) {
		if( !&pAT0 )
			return 0;
		int nAT;
		pAT0 = pAT( nAT, pSat, nSat );

		aMnMx->x = -1;
		gpfMset( (&aMnMx->x)+1, gpeAT_N*2-1, (&aMnMx->x), sizeof(aMnMx->x) );

		iQ1 = iOK = iNO = iERR = iCMTI = iCLIP = iRING = iMISS = iVOICE = iCALL = iBEGIN = iEND = iMRK = -1;
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
				case gpeALF_VOICE: 		j = gpeAT_voice; break;
				case gpeALF_CALL: 		j = gpeAT_call; break;
				case gpeALF_begin: 		j = gpeAT_begin; break;
				case gpeALF_end: 		j = gpeAT_end; break;
				case gpeALF_MRK:
							j = gpeAT_mrk; break;
				case gpeALF_CREG:
							j = gpeAT_creg; break;
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
		iAT		= aMnMx[gpeAT_atP].x;		// AT+ első
		iOK		= aMnMx[gpeAT_ok].y;		// utolso
		iNO		= aMnMx[gpeAT_no].y;		// utolso
		iERR	= aMnMx[gpeAT_err].y;		// utolso
		iCMTI	= aMnMx[gpeAT_cmtiPP].x;	// : első (PP)
		iCLIP	= aMnMx[gpeAT_clipPP].x;	// : első (PP)
		iRING	= aMnMx[gpeAT_ring].x;		// első
		iMISS	= aMnMx[gpeAT_miss].x;		// első
		iVOICE	= aMnMx[gpeAT_voice].x;		// első
		iCALL	= aMnMx[gpeAT_call].x;		// első
		iBEGIN	= aMnMx[gpeAT_begin].x;		// első
		iEND 	= aMnMx[gpeAT_end].x;		// első
		iMRK 	= aMnMx[gpeAT_mrk].x;		// első
		iCREG 	= aMnMx[gpeAT_creg].x;		// első
		if( !nAT )
		{
			gpmZ(aIS);
			return 0;
		}
		aIS[0] = pAT0[0];

		if( nAT > 1 )
			aIS[1].alf = pAT0[1].alf; // pAT[1].alf;
		else
			aIS[1].alf = gpeALF_null;
		return aIS[1].num = nAT;

	}
	gpcLZY* answSTAT( gpcLZY* pANSW ) {
		U8 s;
		pANSW = pANSW->lzyFRMT( s=-1,	"ACT:%s"
										"\r\n",
										asACTname[iACTION%gpdACTn] );
		if( aNUM[1] ) {
			pANSW = pANSW->lzyFRMT( s=-1, "call: +%0.11lld last: +%0.11lld\r\n", aNUM[1], aNUM[2] );
		} else if( aNUM[2] ) {
			pANSW = pANSW->lzyFRMT( s=-1, "last: +%d\r\n", aNUM[2] );
		}
		int utc = cgps.utc;
		pANSW = pANSW->lzyFRMT( s=-1, 	" lat: %c %6.4f lng: %c %6.4f\r\n"
										" alt:    %6.4f spd:    %6.4f\r\n"
										" dat: %d\ttim: %d:%d %d\r\n"
										"\r\n",
										cgps.ns, cgps.lat, cgps.ew, cgps.log,
										cgps.alt, cgps.spd,
										cgps.dat, utc/10000, (utc%10000)/100, utc%100

								);
		return pANSW->lzyFRMT( s = -1, sSTAT );
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
	void so( I8x2* pAT, int nAT ) {
		if(!bSTDcout)
			return;
		int at = 0;
		for( ; at < nAT; at++ ) {
			switch(pAT[at].alf){
				case gpeALF_PLUS:
					sprintf( sOUT, "+%lld", pAT[at].num );
					break;
				case gpeALF_CM:
					sprintf( sOUT, ",%lld", pAT[at].num );
					break;
				case gpeALF_MRK:
					sprintf( sOUT, "\"%lld\r\n", pAT[at].num);
					break;
				default:
					pAT[at].an2str( sOUT );
					break;
			}
			std::cout << at <<"."<< sOUT << " ";
		}
		std::cout << "nAT:" << at << std::endl;
	}

	int answCPIN( char* pANSW, int* pSUB, char* pS, I8x2 *pAT, int nAT, int iPLUS, int pin ) {
		*pANSW = 0;

		switch( pAT[iPLUS+1].alf ) {
			case gpeALF_ERROR: {
					sprintf( pANSW, "AT+CPIN?\r\n", pin );
					//pSUB[1] = pAT[nAT].y;
					nAT = 0;
				} break;
			case gpeALF_OK: nAT = 0; break;
			case gpeALF_READY: {
					bPINrdy = true; //pSUB[1] = pAT[nAT].y;
					nAT = 0;
				} break;
			case gpeALF_SIM: {
					switch( pAT[iPLUS+2].alf ) {
						case gpeALF_READY: {
								//pSUB[1] = pAT[nAT].y;
								nAT = 0;
							} break;
						case gpeALF_PIN: {
								sprintf( pANSW, "AT+CPIN=%d\r\n", pin );
								//pSUB[1] = pAT[nAT].y;
								nAT = 0;
							} break;
						default: break;
					}
				} break;
			default: break;
		}

		return nAT;
	}

	int answ2CPIN2( char* pANSW, I8x2 *pAT, int nAT, int pin ) {
		*pANSW = 0;
		switch( pAT[0].alf ) {
			case gpeALF_READY:
				bPINrdy = true;
				break;
			case gpeALF_SIM: {
					switch( pAT[1].alf ) {
						case gpeALF_READY:
							bPINrdy = true;
							break;
						case gpeALF_PIN:
							sprintf( pANSW, "AT+CPIN=%d\r\n", pin );
							break;
						default: break;
					}
				} return 1;
			default: break;
		}
		return 0;
	}
	int answ2reg( char* pANSW, I8x2 *pAT, int nAT, char* pS, int n ) {
		*pANSW = 0;

		int	iOK = pAT->aALFfnd( aALFmost, nAT, gpmN(aALFmost)),
			iCR = 0, nCM = 0;

		switch( pS[pAT->num] ){
			case '?':
				if( iOK >= nAT )
					return nAT;
				iCR = pAT[1].alfFND(gpeALF_CREG, nAT-1)+1;
				break;
			case '=':
				if( iOK >= nAT )
					return nAT;
				sprintf( pANSW, "AT+CREG?\r\n" );
				return iOK;
			case ':':
			default:
				break;
		}

		if( iCR > iOK )
			iOK = iCR + pAT[iCR].aALFfnd( aALFmost, nAT-iCR, gpmN(aALFmost));
		for( U4 i = iCR; i < iOK; i++ ) {
			if( pAT[i].alf == gpeALF_CM )
				nCM++;
		}

		pS+=pAT[iCR].num+1;
		char *pSTOP = ",\r\n \t";
		switch( nCM ){
			case 0:
				CREG.n 		= gpfSTR2I8( pS, &pS, pSTOP ); pS++;

				break;
			case 1:
				CREG.n 		= gpfSTR2I8( pS, &pS, pSTOP ); pS++;
				CREG.stat	= gpfSTR2I8( pS, &pS, pSTOP ); pS++;
				break;
			case 2:	// 2
				CREG.n 		= 2;
				CREG.stat	= gpfSTR2I8( pS, &pS, pSTOP ); pS++;
				CREG.lac	= gpfSTR2I8( pS, &pS, pSTOP, true ); pS++;
				CREG.ci		= gpfSTR2I8( pS, &pS, pSTOP, true ); pS++;
				break;
			case 3:
				CREG.n 		= gpfSTR2I8( pS, &pS, pSTOP ); pS++;
				CREG.stat	= gpfSTR2I8( pS, &pS, pSTOP ); pS++;
				CREG.lac	= gpfSTR2I8( pS, &pS, pSTOP, true ); pS++;
				CREG.ci		= gpfSTR2I8( pS, &pS, pSTOP, true ); pS++;
				break;
		}

		if( (CREG.n==n) && (CREG.stat==1) )
			return iOK;

		sprintf( pANSW, ((CREG.n==n) ? "AT+CREG?\r\n" : "AT+CREG=%d\r\n"), n );
		return iOK;
	}

	int answCLIP( char* pANSW, I8x2 *pAT, int nAT, char* pS ) {
		*pANSW = 0;

		int iM = pAT->alfFND( gpeALF_MRK, nAT ), iCRLF = iM;
		if( iM >= nAT )
			return nAT;
		iCRLF = iM + pAT[iM].alfFND( gpeALF_CRLF, nAT-iM );
		gpcCLIP		*pCn = (gpcCLIP*)clip.Ux( nCLIP, sizeof(*pCn) ),
					*pC0 = pCn - nCLIP;

		*pCn = pS+pAT[iM].num-1;

		I4 j = nCLIP;
		for( U4 i = 0; i<nCLIP; i++ ){
			if( pC0[i].aI[0] != pCn->aI[0] )
				continue;

			pC0[j=i].aI[0xf]++;
			break;
		}
		if( j < nCLIP )
			return iCRLF;

		nCLIP++;
		return iCRLF;
	}
	int answCLIPend( char* pANSW, I8 iNUM  ) {
		if( !iNUM )
			return nCLIP;
		*pANSW = 0;
		gpcCLIP		*pCn = (gpcCLIP*)clip.Ux( nCLIP, sizeof(*pCn) ),
					*pC0 = pCn - nCLIP;
		for( U4 i = 0; i<nCLIP; i++ ) {
			if( iNUM > 0 )
			if( pC0[i].aI[0] != iNUM )
				continue;

			nCLIP--;
			if( nCLIP > i ) {
				pC0[i] = pC0[nCLIP];
				i--;
			}

		}
		if( iW )
			iW = 0;
		return nCLIP;
	}
	int answ2gps( char* pANSW, I8x2 *pAT, int nAT, char* pS ) {
		*pANSW = 0;
		int iOK = pAT->alfFND( gpeALF_OK, nAT ); //pAT->aALFfnd( aALFok, nAT, gpmN(aALFok) );
		if(iOK>nAT)
			return nAT;
		char	*pSi, *pSe, *pSl;

		for( int i = 0, j = 0; i<=iOK; i++ ) {
			pSe = pS+pAT[i].y;
			if( *pSe == ':' ) {
				pSe++;
				pSi = pSe;
			}

			if(i < iOK)
			if( pAT[i].alf != gpeALF_CM )
				continue;

			switch(j) {
				case 0: {
						pSl = pSi;
						double d0 = gpmSTR2D( pSi );
						if(pSl==pSi) {
							pSl = NULL;
							break;
						}
						cgps.lat = d0;
					} break;
				default:
					if( pSl )
					switch(j) {
						case 1:
							cgps.ns = *pSi;
							break;
						case 2:
							cgps.log = gpmSTR2D( pSi );
							break;
						case 3:
							cgps.ew = *pSi;
							break;
						case 4:
							cgps.dat = gpfSTR2I8( pSi );
							break;
						case 5:
							cgps.utc = gpmSTR2D( pSi );
							break;
						case 6:
							cgps.alt = gpmSTR2D( pSi );
							break;
						case 7:
							cgps.spd = gpmSTR2D( pSi );
							break;
						case 8:
							cgps.curs = gpmSTR2D( pSi );
							i = iOK;
							break;
						default: break;
					}
					break;
			}
			pSi = pSe;
			j++;
		}

		return iOK;
	}

	int answ2CPIN( char* pANSW, I8x2 *pAT, int nAT, int pin ) {
		*pANSW = 0;
		switch( pAT[0].alf ) {
			case gpeALF_READY: {
					bPINrdy = true; //pSUB[1] = pAT[nAT].y;
				} break;
			case gpeALF_SIM: {
					switch( pAT[1].alf ) {
						case gpeALF_READY:
							bPINrdy = true; //pSUB[1] = pAT[nAT].y;
							break;
						case gpeALF_PIN:
							sprintf( pANSW, "AT+CPIN=%d\r\n", pin );
							break;
						default: break;
					}
				} return 2;
			default: break;
		}
		int iOK = pAT->alfFND(gpeALF_OK, nAT);
		if( iOK > 1 )
			return 1;
		return 0;
	}
	int answ2CREG( char* pANSW, I8x2 *pAT, int nAT, char* pSat ) {
		*pANSW = 0;
		pSat += pAT->num;
		if( *pSat == '?' )
			return 0;

		int iOK = pAT->alfFND(gpeALF_OK, nAT), nCM = 0;
		for( U4 i = 0; i < iOK; i++ ){
			if( pAT[i].alf == gpeALF_CM )
				nCM++;
		}


		switch( nCM ){
			case 0:
				CREG.n = gpfSTR2I8( pSat, &pSat ); pSat++;
				break;
			case 1:
				CREG.n = gpfSTR2I8( pSat, &pSat ); pSat++;
				CREG.stat = gpfSTR2I8( pSat, &pSat ); pSat++;
				break;
			default:
				CREG.stat = gpfSTR2I8( pSat, &pSat ); pSat++;
				CREG.lac = gpfSTR2I8( pSat, &pSat, NULL, true ); pSat++;
				CREG.ci = gpfSTR2I8( pSat, &pSat, NULL, true ); pSat++;
				break;
		}
		if( CREG.stat != 1 )
			if( CREG.n == 2 )
				sprintf( pANSW, "AT+CREG?\r\n" );
			else
				sprintf( pANSW, "AT+CREG=2\r\n" );

		if( iOK > 1 )
			return 1;
		return 0;
	}
};
gpcGSM gsmZERO;
gpcLZY* gpcGT::GTgsmOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR, U4 mSEC ) {
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );
	gpcGSM	*pGSM = pOUT ? (gpcGSM*)pOUT->p_alloc : NULL;
	if( !pGSM )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iNUM = 0, nNUM, iE = 2;
	I8x2 an;
	double d8;
	gpeALF alf = gpeALF_null;
	bool bSTAT = false;
	for(
			 pSTR += gpmNINCS( pSTR, " \t\a\r\n;," );
			*pSTR;
			 pSTR += gpmNINCS( pSTR, " \t\a\r\n;," )
		) {
		an.num = pEND-(pCOM = pSTR);
		an = pCOM;
		pSTR += an.num;
		pNUM = NULL;
		if( an.alf ) {
			comA = *(U4*)pCOM;
			iNUM = gpeDRCos_NONS;
			switch( an.alf ) {
				case gpeALF_STAT:
					bSTAT = true;
					break;
				case gpeALF_CALL:
					pNUM = pSTR;
					pGSM->aNUM[1] = 0;
					iNUM = 1;
					bSTAT = true;
					break;
				case gpeALF_HUP:
					pNUM = NULL;
					pGSM->aNUM[1] = 0;
					iNUM = 0;
					bSTAT = true;
					break;
				default:
					break;

			}
		}
		if( !pNUM )
			continue;

		pGSM->aNUM[iNUM] = gpfSTR2I8( pNUM, &pSTR );
		break;
	}
	if(bSTAT)
		return pGSM->answSTAT( pANS );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}
I8 gpcGT::GTgsm( gpcWIN* pWIN ) {
	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? iCNT : 0;

	U8 nLEN, s;
	int baud = 115200, pin = 2028;
	gpcGSM	*pGSM = pOUT ? (gpcGSM*)pOUT->p_alloc : NULL;
	char	*pANSW = NULL, sGO[0x100];

	if( socket == INVALID_SOCKET ) {
		sprintf( sGO, sSER, s_ip );
		if( (socket=(SOCKET)serialOpen( sGO, baud )) < 0 ) {
			if(bSTDcout){std::cout << stdALU "GSM ERR:" << strerror(errno) << std::endl;}
			msGTdie = pWIN->mSEC.x + 3000;
			return iCNT;
		}
		else if( !pOUT ){
			int nCNT = 0;
			pOUT = pOUT->lzyADD( pGSM ? pGSM : &gsmZERO, sizeof(gsmZERO), (s=0), -1 );
			iCNT = pOUT->nLD();
			/// 0 CRESET
			pOUT = pOUT->lzyADD( sGSMreset, gpmSTRLEN(sGSMreset)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 1 CREG
			pOUT = pOUT->lzyADD( sGSMreg, gpmSTRLEN(sGSMreg)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 2 IDLE
			pOUT = pOUT->lzyADD( sGSMidle, gpmSTRLEN(sGSMidle)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 2 WAIT ATA
			pOUT = pOUT->lzyADD( sGSMwait, gpmSTRLEN(sGSMwait)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 4 	VOICE CALL END
			pOUT = pOUT->lzyADD( sGSMhup, gpmSTRLEN(sGSMhup)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 5 	VOICE CALL BEGIN
			pOUT = pOUT->lzyADD( sGSMansw, gpmSTRLEN(sGSMansw)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 6	VOICE CALL chat
			pOUT = pOUT->lzyADD( sGSMchat, gpmSTRLEN(sGSMchat)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 7	VOICE CALL
			pOUT = pOUT->lzyADD( sGSMcall, gpmSTRLEN(sGSMcall)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// n
			pGSM = (gpcGSM*)pOUT->p_alloc;
			aGSMcnt[nCNT] = pOUT->nLD();
			++nCNT;
			pGSM->nCNT = nCNT;
		}
	}

	pANSW = (char*)pOUT->Ux( aGSMcnt[pGSM->nCNT], 0x1000 );
	if(!pANSW)
		return 0;

	char	*pS, *pSi, *pSe,
			// *pSat = NULL,
			*pCLR, *pALL = (char*)pOUT->p_alloc;
	int	nR = serialDataAvail(socket), nINP = pINP->nLD(),
		nAT = 0, aSUB[2], iA, iA2,
		aN[0x10],
		aP[2], iOK, iCLIP, nCM,
		ms = 20000;

	if( nR > 0 ) {
		nR = read( socket, pANSW, nR );
		if( nR > 0 )
			pINP = pINP->lzyADD( pANSW, nR, s=-1, -1 );
		nINP = pINP->nLD();
	}

	*pANSW = 0;
	pS = nINP ? (char*)pINP->p_alloc : NULL;
	I8x2* pAT = pGSM->pAT( nAT, pS, nINP, " \t:" ); //, " \r\n\t:," );
	while( nAT ) {
		gpmZ(aSUB);
		if(bSTDcout){std::cout
								<< asACTname[pGSM->iACTION%gpdACTn]
								<< " pS:\r\n"
								<< pS <<std::endl;}
		pGSM->so( pAT, nAT );

		iA = pAT->aALFfnd(aALFat, nAT, gpmN(aALFat));
		if( iA >= nAT )
			iA = 0;
		switch( pAT[iA].alf ) {
			case gpeALF_AT: {
				if( (iA2 = iA+2) >= nAT )
					break;
				aSUB[1] = aSUB[0] = pAT[iA].y-2;	// AT
				switch( pAT[iA2].alf ) {

					/// --------------------- CRESET BEGIN ----------------------------	//
					case gpeALF_CRESET: 												///
						iA2 = iA2 + pAT[iA2].aALFfnd(  aALFat, nAT-iA2, gpmN(aALFat));	///
						if( pAT[iA2].alf == gpeALF_PLUS ){
							iA2++;
							iA2 = iA2 + pAT[iA2].aALFfnd(  aALFat, nAT-iA2, gpmN(aALFat));

						}

						switch( pAT[iA2].alf ) {
							case gpeALF_CREG:
								aSUB[0] = pAT[iA2].y-5;
								iA2 += pGSM->answ2reg( pANSW, pAT+iA2, nAT-iA2, pS, 2 );
								aSUB[1] = pAT[iA2].y;
								if( pAT[iA2].alf != gpeALF_RDY )
									break;
							case gpeALF_RDY:
								if( pAT[iA].y-2 == aSUB[0] )
									aSUB[0] = pAT[iA2].y-3;
								pGSM->bGSMrdy = true;
								aSUB[1] = pAT[iA2].y;
								break;
							case gpeALF_CPIN:
							default:
								if( iA2 >= nAT ) {
									return iCNT;
								}
						}
						if( !pGSM->bGSMrdy )
							return iCNT;

						if( aSUB[1] > aSUB[0] )
							break;
					case gpeALF_CPIN: 													///
						iA2++;															///
						iA2 += pGSM->answ2CPIN2( pANSW, pAT+iA2, nAT-iA2, pin );		///
						aSUB[1] = pAT[iA2].y;											///
						break;															///
					/// --------------------- CRESET END ----------------------------	//

					case gpeALF_CREG: {
						iA2 += pGSM->answ2reg( pANSW, pAT+iA2, nAT-iA2, pS, 2 );
						if( iA2 >= nAT )
							break;

						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CLIP: {
						iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CMTI: {
						iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CGPSINFO: {
						//iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						iA2 += pGSM->answ2gps( pANSW, pAT+iA2, nAT-iA2, pS ); // cgps = pS+pAT[2].num;
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CSQ: {
						iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CGPS: {
						iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CGMR: {
						iA2+=2;
						iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					default:
						iA2 += pAT[iA2].aALFfnd( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
						break;
				}
			} break;
			case gpeALF_ATA:{
				aSUB[1] = aSUB[0] = pAT[iA].y-3;	// ATA
				pGSM->iW = 0;
				pGSM->nATA = gpmMAX(pGSM->nATA, pGSM->nCLIP);
				if( pGSM->nATA ) {
					aSUB[1] = pAT[iA].y;
					break;
				}
				iA += pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;

			} break;
			case gpeALF_ATD:{
				aSUB[1] = aSUB[0] = pAT[iA].y-3;	// ATA
				pGSM->iW = 0;
				iA++;
				iA += pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				if( iA >= nAT )
					break;
				aSUB[1] = pAT[iA].y;

			} break;
			case gpeALF_CSDVC: {
				aSUB[1] = aSUB[0] = pAT[iA].y-5;	// CSDVC
				iA++;
				iA += pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				if( iA >= nAT )
					break;
				aSUB[1] = pAT[iA].y;
				break;
			}
			case gpeALF_CPIN:{
				aSUB[0] = pAT[iA].y-5;	// CPIN
				iA++;
				iA += pGSM->answ2CPIN2( pANSW, pAT+iA, nAT-iA, pin );
				aSUB[1] = pAT[iA].y;
			} break;
			case gpeALF_CREG:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//+CREG
				iA2 = iA + pGSM->answ2reg( pANSW, pAT+iA, nAT-iA, pS, 2 );
				aSUB[1] = pAT[iA2].y;
			} break;
			case gpeALF_CLIP:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//+CLIP
				iA += pGSM->answCLIP( pANSW, pAT+iA, nAT-iA, pS );
						//pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
			} break;
			case gpeALF_CMTI:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//+CMTI
				iA += pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
			} break;
			case gpeALF_RDY: {
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-3);	// RDY
				pGSM->bGSMrdy = true;
				aSUB[1] = pAT[iA].y;
			}  break;
			case gpeALF_BUSY: {
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-3);	// RDY
				if( pGSM->aNUM[1] ) {
					pGSM->aNUM[2] = pGSM->aNUM[1];
					pGSM->aNUM[1] = 0;
				}
				aSUB[1] = pAT[iA].y;
			}  break;
			case gpeALF_VOICE:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//VOICE
				iA2 = iA+1;
				iA2 += pAT[iA2].aALFfnd(aALFvoice, nAT-iA2, gpmN(aALFvoice));
				switch( pAT[iA2].alf ) {
					case gpeALF_begin: {

					} break;
					case gpeALF_end: {
						I8 nCLIP = pGSM->nCLIP;
						if(	pGSM->answCLIPend( pANSW, -1 ) != nCLIP ) {
							pGSM->iW = 0;
							iCNT = aGSMcnt[pGSM->iACTION = 4];
						}
						if( pGSM->aNUM[1] ) {
							pGSM->aNUM[2] = pGSM->aNUM[1];
							pGSM->aNUM[1] = 0;
						}

					} break;
					default: break;
				}
				aSUB[1] = pAT[iA2].y;
			} break;
			case gpeALF_MISSED:{
				aSUB[1] = aSUB[0] = pAT[iA].y-6;	//MISSED
				iA2 = iA+1;
				iA2 += pAT[iA2].aALFfnd(aALFampm, nAT-iA2, gpmN(aALFampm));
				if( iA2 >= nAT )
					break;
				iA2 += pAT[iA2].alfFND( gpeALF_PLUS, nAT-iA2 );
				pSi = pS + pAT[iA2].y;
				I8 iNUM = gpfSTR2I8( pSi, &pSi );
				int nC = pGSM->nCLIP;
				if( iNUM )
				if(	pGSM->answCLIPend( pANSW, iNUM ) != nC ) {
					iCNT = aGSMcnt[pGSM->iACTION = 4];	//CLIP end
					pGSM->iW = 0;
				}
				iA2 += pAT[iA2].alfFND( gpeALF_CRLF, nAT-iA2 );
				aSUB[1] = pAT[iA2].y;
			} break;
			case gpeALF_SMS:
				aSUB[1] = aSUB[0] = pAT[iA].y-3;	//SMS
				iA++;
				switch( pAT[iA].alf ){
					case gpeALF_DONE:
						pGSM->bSMS = true;
						break;
					case gpeALF_FULL:
						pGSM->bSMSfull = true;
						break;
					default: break;
				}
				aSUB[1] = pAT[iA].y;
				break;
			case gpeALF_PB:
				aSUB[1] = aSUB[0] = pAT[iA].y-2;	//PB
				iA++;
				pGSM->bPB = (pAT[iA].alf==gpeALF_DONE);
				aSUB[1] = pAT[iA].y;
				break;
			case gpeALF_COPS:
				aSUB[1] = aSUB[0] = pAT[iA].y-4;	//COPS
				iA++;
				iA += pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
				break;
			default:
				if(iA >= nAT)
					iA = 0;
				aSUB[1] = aSUB[0] = gpmMAX( 0, pAT[iA].y-(alfLEN(pAT[iA].alf)+1));	//+CMTI
				iA += pAT[iA].aALFfnd( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
				break;

		}

		if( aSUB[1] > aSUB[0] ) {
			aSUB[1] += gpmNINCS( pS+aSUB[1], " \r\n\t" );
			nR = serialDataAvail(socket);
			if( nR > 0 ) {
				nR = read( socket, pANSW, nR );
				if( nR > 0 )
					pINP = pINP->lzyADD( pANSW, nR, s=-1, -1 );
				//nINP = pINP->nLD();
			}
			s = aSUB[0];
			pINP->lzySUB(s, aSUB[1]-aSUB[0]);
			nINP = pINP->nLD();
			pS = nINP ? (char*)pINP->p_alloc : NULL;
			if( pS ) {
				pAT = pGSM->pAT( nAT, pS, nINP, " \t:," ); //, " \r\n\t:," );
				if( nAT ) {
					//pGSM->so(pAT, nAT);
					if( *pANSW )
						nAT = 0;
				} else
					gpmDEL(pINP);
			} else
				nAT = 0;
		}

	}

	if( nAT )
		return iCNT;

	if( *pANSW ) {
		pCLR = stdPURPLE;
		pS = pANSW;
	} else {
		pCLR = stdYELLOW;
		pS = pALL+iCNT;
	}
	pSe = pS + gpmVAN( pS, "\r\n", nLEN );
	if( pSe-pS < 1 )
		return iCNT;
	pSe+=2;

	int OiCNT = iCNT;
	while( *pS != 'A' ) {
		switch( *pS ) {
			case 'w':
			case 'W': {
				pSi = pS+gpmVAN(pS," \t\r\n+-0123456789",nLEN);
				gpmZ(aN);
				if( pSi < pSe ) {
					aN[0] = gpfSTR2I8(pSi, &pSi);
					pSi = pSi+gpmVAN(pSi," \t\r\n+-0123456789",nLEN);
					if( pSi < pSe )
						aN[1] = gpfSTR2I8(pSi, &pSi);
				}
				int msDIF = pGSM->iW-(int)pWIN->mSEC.x;
				if( pGSM->iW && (msDIF<(ms/2)) ) {
					pGSM->iW = 0;
					iCNT = aGSMcnt[pGSM->iACTION = aN[1]];
					pS = pALL+iCNT;
					break;
				}

				if( pGSM->iW >= (int)pWIN->mSEC.x ) {
					if(bSTDcout){
						std::cout	<<std::endl
									<< stdRED << "WAIT:" << pGSM->iW-(int)pWIN->mSEC.x << "ms"
									<<std::endl;
					}

					iCNT = aGSMcnt[pGSM->iACTION = aN[0]];
					pS = pALL+iCNT;
					break;
				}

				if( !pGSM->iW ) {
					pGSM->iW = ms + (int)pWIN->mSEC.x;
					iCNT = aGSMcnt[pGSM->iACTION = aN[0]];
					pS = pALL+iCNT;
					break;
				}
				// letelt az idő következő sor
				pS += gpfVnN( pS, "\r\n" );
				pGSM->iW = 0;

			} break;
			default: {
				switch( pGSM->iACTION = gpfSTR2U8(pS) ) {
					case 2:	/// IDLE
						/*if( !pGSM->aNUM[1] )
						if( *pGSM->sSTR ) {
							// letesz
							*pGSM->sSTR = 0;
							pGSM->iACTION = 4;
							iCNT = aGSMcnt[pGSM->iACTION];
							pS = pALL+iCNT;
							break;
						}*/

						if( pGSM->nCLIP ) {
							pGSM->nCLIP = 0;
							pGSM->iACTION = 3;
							iCNT = aGSMcnt[pGSM->iACTION];
							pS = pALL+iCNT;
							break;
						}
					case 6:
						if( pGSM->aNUM[1] ) {
							if( !*pGSM->sSTR )
							{
								sprintf( pGSM->sSTR, "%0.11lld;", pGSM->aNUM[1] );
								pGSM->iACTION = 7;
								iCNT = aGSMcnt[pGSM->iACTION];
								pS = pALL+iCNT;
								break;
							}
						} else if( *pGSM->sSTR ) {
							// letesz
							*pGSM->sSTR = 0;
							pGSM->iACTION = 4;
							iCNT = aGSMcnt[pGSM->iACTION];
							pS = pALL+iCNT;
							break;
						}

						if( pGSM->iCMTI < pGSM->nCMTI ) {
							I8x2* pCMTI = (I8x2*)pGSM->cmti.Ux(pGSM->iCMTI,sizeof(*pCMTI));
							pS = pGSM->answCMGR( pANSW, pCMTI->num );

							pGSM->iCMTI++;
							break;
						}
					default:
						iCNT = aGSMcnt[pGSM->iACTION];
						pS = pALL+iCNT;
						break;
				}
			} break;
		}
	}
	if( OiCNT != iCNT ){
		pSe = pS + gpmVAN( pS, "\r\n", nLEN );
		if( *pSe )
			pSe+=2;
	}

	if( pANSW != pS ) {
		gpmMcpy( pANSW, pS, pSe-pS )[pSe-pS] = 0;
		iCNT = pSe-pALL + gpmNINCS( pSe, "\r\n" );
	}

	pSi = strstr( pANSW, "%s" );
	if( pSi ? pSi < pSe : false ) {
		pS = pANSW + 0x800;
		pSe = pS + sprintf( pS, pANSW, pGSM->sSTR );
	}

	if(bSTDcout) {
		std::cout	<< stdBREAK << "ACT:"	<< pGSM->iACTION
					<< stdCMPLR << "CLIP:"	<< pGSM->iCLIP << "/" << pGSM->nCLIP
					<< stdRUN	<< "CMTI:"	<< pGSM->iCMTI << "/" << pGSM->nCMTI
					<< pCLR << pANSW <<std::endl;
	}

	if( pGSM->iW )
	if(bSTDcout){
			std::cout	<<std::endl
						<< stdRED << "WAIT:" << pGSM->iW-(int)pWIN->mSEC.x << "ms"
						<<std::endl;
	}

	int nW = 0;
	pSi = pS;
	while( pSi < pSe ) {
		nW = write(socket,pSi,pSe-pSi);
		pSi += nW;
	}
	// *pANSW = 0;
	return iCNT;
}
I8 gpcGT::GTgsm3( gpcWIN* pWIN ) {
	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? iCNT : 0;
	gpcGT* pGT = this;
	gpcGSM	*pGSM = pOUT ? (gpcGSM*)pOUT->p_alloc : NULL;
	char	*pANSW = NULL, aGO[0x100];
	U8 nLEN, s;
	int baud = 115200, pin = 2028;
	if( socket == INVALID_SOCKET ) {
		sprintf( aGO, sSER, s_ip );
		if( (socket=(SOCKET)serialOpen( aGO, baud )) < 0 ) {
			if(bSTDcout){std::cout << stdALU "GSM ERR:" << strerror(errno) << std::endl;}
			msGTdie = pWIN->mSEC.x + 3000;
			return iCNT;
		}
		else if( !pOUT ){
			int nCNT = 0;
			pOUT = pOUT->lzyADD( pGSM ? pGSM : &gsmZERO, sizeof(gsmZERO), (s=0), -1 );
			iCNT = pOUT->nLD();
			/// 0 CRESET
			pOUT = pOUT->lzyADD( sGSMreset, gpmSTRLEN(sGSMreset)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 1 CREG
			pOUT = pOUT->lzyADD( sGSMreg, gpmSTRLEN(sGSMreg)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 2 IDLE
			pOUT = pOUT->lzyADD( sGSMidle, gpmSTRLEN(sGSMidle)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 2 WAIT ATA
			pOUT = pOUT->lzyADD( sGSMwait, gpmSTRLEN(sGSMwait)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 4 	VOICE CALL END
			pOUT = pOUT->lzyADD( sGSMhup, gpmSTRLEN(sGSMhup)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 5 	VOICE CALL BEGIN
			pOUT = pOUT->lzyADD( sGSMansw, gpmSTRLEN(sGSMansw)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 6	VOICE CALL chat
			pOUT = pOUT->lzyADD( sGSMchat, gpmSTRLEN(sGSMchat)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// n
			pGSM = (gpcGSM*)pOUT->p_alloc;
			aGSMcnt[nCNT] = pOUT->nLD();
			++nCNT;
			pGSM->nCNT = nCNT;
		}
	}

	pANSW = (char*)pOUT->Ux( aGSMcnt[pGSM->nCNT], 0x1000 );
	if(!pANSW)
		return 0;

	*pANSW = 0;

	char	*pS, *pSi, *pSe, *pSat = NULL, *pCLR, *pALL = (char*)pOUT->p_alloc,
			ms = 20000;
	int	nR = serialDataAvail(socket), nINP = pINP->nLD(),
		nAT = 0,
		aN[0x10], aSUB[2],
		aP[2], aA[0x10], iOK, iCLIP, nCM;
	if( nR > 0 ) {
		nR = read( socket, pANSW, nR );
		if( nR > 0 )
			pINP = pINP->lzyADD( pANSW, nR, s=-1, -1 );
		nINP = pINP->nLD();
	}
	pSat = nINP ? (char*)pINP->p_alloc : NULL;
	I8x2* pAT = pGSM->pAT( nAT, pSat, nINP, " \r\n\t:," );

	aA[3]=aA[2] = nAT;
	gpmZ(aSUB);

	if( pAT ) {
		*pANSW = 0;

		if(bSTDcout){std::cout
								<< asACTname[pGSM->iACTION%gpdACTn]
								<< " pSat:\r\n"
								<< pSat <<std::endl;}
		pGSM->so( pAT, nAT );
		iOK = pAT->aALFfnd( aALFok, nAT, gpmN(aALFok) );
		bool bOK = iOK < nAT;
		for( aA[0] = 0; aA[0] < nAT; aA[0]++ ) {
			pSi = pSat+pAT[aA[0]].num;
			if( bOK )
				bOK = aA[0] < iOK;
			switch( pAT[aA[0]].alf ) {
				case gpeALF_OK:
				case gpeALF_ERROR:
					iOK++;
					aA[0] = iOK;
					iOK += pAT[iOK].aALFfnd(aALFok, nAT-iOK, gpmN(aALFok));
					bOK = iOK < nAT;
				case gpeALF_DONE:
				case gpeALF_READY:
					aSUB[1] = pAT[aA[0]].num;
					break;

				case gpeALF_AT:
					aA[1] = aA[0];
					break;
				case gpeALF_ATA:

					break;
				case gpeALF_RDY:
					pGSM->bGSMrdy = true;

					break;
				case gpeALF_PLUS:
					aP[0] = aA[0];
					aSUB[1] = pAT[aA[0]].num;
					break;
				case gpeALF_MRK:
					aA[0]++;
					aA[0] += pAT[aA[0]].alfFND( gpeALF_AT, nAT-aA[0] );
					break;
				case gpeALF_CPIN:
					aA[0]++;
					aA[0] += pGSM->answ2CPIN( pANSW, pAT+aA[0], nAT-aA[0], pin )-1;
					aSUB[1] = pAT[aA[0]].num;
					break;
				case gpeALF_CREG:
					aA[0] += pGSM->answ2CREG( pANSW, pAT+aA[0], nAT-aA[0], pSat );
					break;
				case gpeALF_CGMI:
				case gpeALF_CGMM:
				case gpeALF_CGSN:
				case gpeALF_CGMR:
					aA[0] = iOK-1;
					break;
				case gpeALF_RING:
					break;
				case gpeALF_PB:
					break;
				case gpeALF_SMS:
					break;
				case gpeALF_FULL:
					// űríteni kéne a slotokat
					sprintf( pANSW, "AT+CMGD=0,2\r\n" );
					aSUB[1] = pAT[aA[0]].num;
					break;
				case gpeALF_CMGD:
					sprintf( pANSW, "AT+CPMS?\r\n" );
					break;
				case gpeALF_CPMS:
					aA[0] = iOK-1;
					break;
				case gpeALF_CGPSINFO:
					aA[0] = iOK-1;
					break;

				case gpeALF_CLIP:
					if( bOK ) {
						if( *pSi == ':' ) {
							gpcCLIP* pCn = (gpcCLIP*)pGSM->clip.Ux( pGSM->nCLIP, sizeof(*pCn) ), *pC0;
							if( pCn ){
								pC0 = pCn-pGSM->nCLIP;
								*pCn = pSi;
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
						}
						aA[3] = 0;
					}
					aA[0] = iOK-1;
					break;
				case gpeALF_CMTI:
					if(iOK<=aA[3]) {
						I8x2* pCMTI = (I8x2*)pGSM->cmti.Ux( pGSM->nCMTI, sizeof(*pCMTI) );
						if( pCMTI ) {
							pCMTI->alf = pAT[aP[0]+1].alf;
							pCMTI->num = gpfSTR2I8( pSat+pAT[aP[0]+1].num+2 );
							pGSM->nCMTI++;
							if( pCMTI->num > -1 )
								pGSM->iREAD = pCMTI->num;
						}
					} break;
				case gpeALF_VOICE:

					break;
				case gpeALF_CALL:

					break;
				case gpeALF_begin:

					break;
				case gpeALF_end:

					break;
				case gpeALF_MISSED:

					break;
				default:

					break;
			}

		}

		if( aSUB[1] ) {
			aSUB[1] += gpmNINCS( pSat+aSUB[1], " \r\n\t" );
			pINP->lzySUB(s=0, aSUB[1] );
			nINP = pINP->nLD();
			pSat = nINP ? (char*)pINP->p_alloc : NULL;
			if( pSat ) {
				pAT = pGSM->pAT( nAT, pSat, nINP, " \r\n\t:," );
				pGSM->so(pAT, nAT);
				if( *pANSW && nAT )
					nAT = 0;
			} else
				nAT = 0;

		}

		if( false ) {
			aA[0] = pAT->alfFND( gpeALF_AT, aA[3] );
			aP[0] = pAT->alfFND( gpeALF_ATA, aA[3] );
			if( aP[0] >= aA[3] )
				aP[0] = pAT->alfFND( gpeALF_PLUS, aA[3])+1;


			if( aP[0] < aA[3] ) {
				aA[1] = pAT[aP[0]].alfFND( gpeALF_AT,	aA[3]-aP[0]) + aP[0];
				aP[1] = pAT[aP[0]].alfFND( gpeALF_PLUS,	aA[3]-aP[0]) + aP[0];
				if( aA[1] > aP[1] ) {
					// PLUS
					aA[3] = aP[1];
					aSUB[1] = pAT[aA[3]].y-1;
				} else {
					// AT
					if( aA[3] > aA[1] ) {
						aA[3] = aA[1];
						aSUB[1] = pAT[aA[3]].y-2;
					} else {
						aSUB[1] = pAT[aA[3]].y;
					}
				}

				iOK = pAT->alfFND(gpeALF_OK, aA[3]);

				char *pSnum = pSat+pAT[aP[0]].num;
				//iCLIP = pAT->alfFND(gpeALF_CLIP, aA[3]);
				switch( pAT[aP[0]].alf ) {
					case gpeALF_ATA: {
							aA[3] = 0;
						} break;
					case gpeALF_CRESET: {
							if( pGSM->bGSMrdy = (pAT->alfFND(gpeALF_RDY,aA[2])<aA[2]) )
								aA[3] = 0;
						} break;
					case gpeALF_CPIN: {
							aA[3] = pGSM->answCPIN( pANSW, aSUB, pSat, pAT, aA[3], aP[0], pin );
						} break;
					case gpeALF_CGPSCOLD: {
							pGSM->bGPS = ( pAT->alfFND( gpeALF_OK, aA[3]) < aA[3] );
							aA[3] = 0;
						} break;
					case gpeALF_CGMI: {
							pGSM->bCGMI = (iOK<=aA[3]);
							if( !pGSM->bCGMI )
								break;

							aA[1] = pAT[iOK].num-pAT[aP[0]].num;
							gpmMcpy( pGSM->sCGMI, pSat+pAT[aP[0]].num, aA[1] )[aA[1]] = 0;
							pGSM->bCGMI = true;
							aA[3] = 0;
						} break;
					case gpeALF_CGMM: {
							pGSM->bCGMM = (iOK<=aA[3]);
							if( !pGSM->bCGMM )
								break;

							aA[1] = pAT[iOK].num-pAT[aP[0]].num;
							gpmMcpy( pGSM->sCGMM, pSat+pAT[aP[0]].num, aA[1] )[aA[1]] = 0;
							pGSM->bCGMM = true;
							aA[3] = 0;
						} break;
					case gpeALF_CGSN: {
							pGSM->bCGSN = (iOK<=aA[3]);
							if( !pGSM->bCGSN )
								break;

							aA[1] = pAT[iOK].num-pAT[aP[0]].num;
							gpmMcpy( pGSM->sCGSN, pSat+pAT[aP[0]].num, aA[1] )[aA[1]] = 0;
							pGSM->bCGSN = true;
							aA[3] = 0;
						} break;
					case gpeALF_CGMR: {
							pGSM->bCGMR = (iOK<=aA[3]);
							if( !pGSM->bCGMR )
								break;

							aA[1] = pAT[iOK].num-pAT[aP[0]].num;
							gpmMcpy( pGSM->sCGMR, pSat+pAT[aP[0]].num, aA[1] )[aA[1]] = 0;
							pGSM->bCGMR = true;
							aA[3] = 0;
						} break;
					case gpeALF_COPS: {
							pGSM->bCOPS = (iOK<=aA[3]);
							if( !pGSM->bCOPS )
								break;

							aA[1] = pAT[iOK].num-pAT[aP[0]].num;
							gpmMcpy( pGSM->sCOPS, pSat+pAT[aP[0]].num, aA[1] )[aA[1]] = 0;
							pGSM->bCOPS = true;
							aA[3] = 0;
						} break;
					case gpeALF_CREG:
						if( aA[0] >= aA[3] ) {
							U4 nCOM = 0;
							for( U4 i = aP[0]; i < aA[3]; i++ )
							if( pAT[i].alf == gpeALF_CM )
								nCOM++;
							pGSM->CREG.n = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
							if( nCOM > 0 )
								pGSM->CREG.stat = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
							if( nCOM > 1 ) {
								pGSM->CREG.lac = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
								if( nCOM > 2 )
									pGSM->CREG.ci = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
								if( pGSM->CREG.stat != 1 ) {
									iCNT = aGSMcnt[pGSM->iACTION=1];
								}
							}
							aA[3] = 0;
						} if(iOK<=aA[3]) {
							//char *pSnum = pSat+pAT[aP[0]].num;

							switch( *pSnum ){
								case '=':
									pSnum += gpmNINCS( pSnum, "=: \t" );
									pGSM->CREG.n = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
									break;
								case '?':
									pSnum += gpmNINCS( pSnum, "?: \t" );
									pGSM->CREG.n = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
									//break;
								default:
									pSnum += gpmNINCS( pSnum, ": \t" );
									//pGSM->CREG.reg.y = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
									pGSM->CREG.stat = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
									pGSM->CREG.lac = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
									pGSM->CREG.ci = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
									if( pGSM->CREG.stat != 1 ) {
										sprintf( pANSW, "AT+CREG=2\r\n" );
										//iCNT = aGSMcnt[pGSM->iACTION=1];
									}
									break;
							}
							aA[3] = 0;
						} break;
					case gpeALF_CNMI:
						if(iOK<=aA[3]) {
							aA[3] = 0;
						} break;
					case gpeALF_CLIP:
						if(iOK<=aA[3]) {
							if( *pSnum == ':' ) {
								gpcCLIP* pCn = (gpcCLIP*)pGSM->clip.Ux( pGSM->nCLIP, sizeof(*pCn) ), *pC0;
								if( pCn ){
									pC0 = pCn-pGSM->nCLIP;
									*pCn = pSnum;
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
							}
							aA[3] = 0;
						} break;
					case gpeALF_CMTI:
						if(iOK<=aA[3]) {
							I8x2* pCMTI = (I8x2*)pGSM->cmti.Ux( pGSM->nCMTI, sizeof(*pCMTI) );
							if( pCMTI ) {
								pCMTI->alf = pAT[aP[0]+1].alf;
								pCMTI->num = gpfSTR2I8( pSat+pAT[aP[0]+1].num+2 );
								pGSM->nCMTI++;
								if( pCMTI->num > -1 )
									pGSM->iREAD = pCMTI->num;
							}
						} break;
					case gpeALF_CSQ:
						if(iOK<=aA[3]) {
							sscanf( pSat+pAT[2].num, "%d,%d", &pGSM->CSQ.x, &pGSM->CSQ.y);
							aA[3] = 0;
						} break;
					case gpeALF_CGPSINFO:
						if(iOK<=aA[3]) {
							pGSM->cgps = pSat+pAT[2].num;
							aA[3] = 0;
						} break;
					case gpeALF_CPSI:
						if(iOK<=aA[3]) {
							//char *pSnum = pSat+pAT[aP[0]].num;
							if( *pSnum != '?' ){
								pSnum += gpmNINCS( pSnum, ": \t" );
							}
							aA[3] = 0;
						} break;
					default:
						aA[3] = 0;
						break;
				}
				if( !aA[3] )
				if( aSUB[1] ) {
					pINP->lzySUB(s=0, aSUB[1] );
					nINP = pINP->nLD();
					pSat = nINP ? (char*)pINP->p_alloc : NULL;
					if( pSat ) {
						pAT = pGSM->pAT( aA[3], pSat, nINP, " \r\n\t:," );
						pGSM->so(pAT, aA[3]);
						aP[0] = pAT->alfFND( gpeALF_PLUS, aA[3])+1;
					}

				}


			}
		}
	}

	if( nAT )
		return iCNT;

	pCLR = stdRED;
	if( *pANSW ) {
		pCLR = stdPURPLE;
		pS = pANSW;
	} else {
		pCLR = stdYELLOW;
		pS = pALL+iCNT;
	}

	pSe = pS + gpmVAN( pS, "\r\n", nLEN );
	if( pSe-pS < 1 )
		return iCNT;
	pSe+=2;
	int OiCNT = iCNT;
	while(
				(*pS != 'A')
			//&& 	!*pANSW
								) {
		switch( *pS ) {
			case 'w':
			case 'W': {
				pSi = pS+gpmVAN(pS," \t\r\n+-0123456789",nLEN);
				gpmZ(aN);
				if( pSi < pSe ) {
					aN[0] = gpfSTR2I8(pSi, &pSi);
					pSi = pSi+gpmVAN(pSi," \t\r\n+-0123456789",nLEN);
					if( pSi < pSe )
						aN[1] = gpfSTR2I8(pSi, &pSi);
				}
				int msDIF = pGSM->iW-(int)pWIN->mSEC.x;
				if( pGSM->iW && (msDIF<(ms/2)) ) {
					pGSM->iW = 0;
					iCNT = aGSMcnt[pGSM->iACTION = aN[1]];
					pS = pALL+iCNT;
					break;
				}

				if( pGSM->iW >= (int)pWIN->mSEC.x ) {
					if(bSTDcout){
						std::cout	<<std::endl
									<< stdRED << "WAIT:" << pGSM->iW-(int)pWIN->mSEC.x << "ms"
									<<std::endl;
					}

					iCNT = aGSMcnt[pGSM->iACTION = aN[0]];
					pS = pALL+iCNT;
					break;
				}

				if( !pGSM->iW ) {
					pGSM->iW = ms + (int)pWIN->mSEC.x;
					iCNT = aGSMcnt[pGSM->iACTION = aN[0]];
					pS = pALL+iCNT;
					break;
				}
				// letelt az idő következő sor
				pS += gpfVnN( pS, "\r\n" );
				pGSM->iW = 0;

			} break;
			default: {
				switch( pGSM->iACTION = gpfSTR2U8(pS) ) {
					case 2:	/// IDLE
						if( pGSM->nCLIP ) {
							pGSM->nCLIP = 0;
							pGSM->iACTION = 3;
							iCNT = aGSMcnt[pGSM->iACTION];
							pS = pALL+iCNT;
							break;
						}
					case 6:
						if( pGSM->iCMTI < pGSM->nCMTI ) {
							I8x2* pCMTI = (I8x2*)pGSM->cmti.Ux(pGSM->iCMTI,sizeof(*pCMTI));
							pS = pGSM->answCMGR( pANSW, pCMTI->num );

							pGSM->iCMTI++;
							break;
						}
					default:
						iCNT = aGSMcnt[pGSM->iACTION];
						pS = pALL+iCNT;
						break;
				}
			} break;
		}
	}
	if( OiCNT != iCNT ){
		pSe = pS + gpmVAN( pS, "\r\n", nLEN );
		if( *pSe )
			pSe+=2;
	}

	if( pANSW != pS ) {

		gpmMcpy( pANSW, pS, pSe-pS )[pSe-pS] = 0;
		iCNT = pSe-pALL + gpmNINCS( pSe, "\r\n" );
	}

	if(bSTDcout) {
		std::cout	<< stdBREAK << "ACT:"	<< pGSM->iACTION
					<< stdCMPLR << "CLIP:"	<< pGSM->iCLIP << "/" << pGSM->nCLIP
					<< stdRUN	<< "CMTI:"	<< pGSM->iCMTI << "/" << pGSM->nCMTI
					<< pCLR << pANSW <<std::endl;
	}

	if( pGSM->iW )
	if(bSTDcout){
			std::cout	<<std::endl
						<< stdRED << "WAIT:" << pGSM->iW-(int)pWIN->mSEC.x << "ms"
						<<std::endl;
	}

	int nW = 0;
	while( pS < pSe ) {
		nW = write(socket,pS,pSe-pS);
		pS += nW;
	}
	return iCNT;
}
I8 gpcGT::GTgsm2( gpcWIN* pWIN ) {
	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? iCNT : 0;
	gpcGT* pGT = this;
	char	sOUT[0x1000], sANSW[0x1000],
			*pS, *pSe,
			*pSat, *pSatI, *pSatE,
			*pSnum;
	int baud = 115200, aI[0x20],
		iAT = -1, nAT=0,// nWT = 0, nWok = 0,
		iQ1 = -1, iOK = -1, iERR = -1, iNO = -1,
		iCMTI = -1, iCLIP = -1, iRING = -1, iMISS = -1,
		iVOICE = -1, iCALL = -1, iBEGIN = -1, iEND = -1,
		iMRK = -1, iCREG = -1,
		pin = 2028; //5779; //2028; //16550;
	U8 s = -1, nLEN;
	I8x2 *pAT;
	gpcGSM	*pGSM = NULL;
	int nR = serialDataAvail(socket), nINP = pINP->nLD();
	if( nR > 0 )
	{
		nR = read( socket, sOUT, nR );
		if( nR > 0 )
			pINP = pINP->lzyADD( sOUT, nR, s=-1, -1 );
		nINP = pINP->nLD();
	}

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

			pOUT = pOUT->lzyADD( sGSMreset, gpmSTRLEN(sGSMreset)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMidle, gpmSTRLEN(sGSMidle)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMwait, gpmSTRLEN(sGSMwait)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMhup, gpmSTRLEN(sGSMhup)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMansw, gpmSTRLEN(sGSMansw)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			pOUT = pOUT->lzyADD( sGSMchat, gpmSTRLEN(sGSMchat)+1, (s=-1), -1 );
			aGSMcnt[nCNT] = s;
			++nCNT;

			gpcGSM	*pGSM = (gpcGSM*)pOUT->p_alloc;
			aGSMcnt[nCNT] = pOUT->nLD();
			++nCNT;
			pGSM->nCNT = nCNT;

			pGSM = (gpcGSM*)pOUT->p_alloc;
			nAT = aGSMcnt[1]-aGSMcnt[0];

			nAT = pGSM->nAT( 	pAT,
								iAT, iQ1, iOK, iNO, iERR,
								iCMTI, iCLIP, iRING, iMISS,
								iVOICE, iCALL, iBEGIN, iEND,
								iMRK, iCREG,
								(char*)(pOUT->p_alloc+aGSMcnt[0]), nAT );
			pGSM->so( pAT, nAT );
			int iC0 = pAT->alfFND( gpeALF_CREG, nAT );
			pGSM->iCREG0 = iC0;
			if( pGSM->iCREG0 < nAT ) {
				pGSM->iCREG0 = (int)pAT[pGSM->iCREG0-1].y -3 +aGSMcnt[0];
				pSatI = (char*)pOUT->p_alloc + pGSM->iCREG0;
			}


		}
	}
	if( pOUT )
		pGSM = (gpcGSM*)pOUT->p_alloc;




	if(bSTDcout){std::cout << stdALU "iCNT:" << iCNT <<std::endl;}
	pSat = pINP ? (char*)pINP->p_alloc : NULL;
	/// RECV -------------------------------------
	*sANSW = 0;
	pGSM->bATA =
	pGSM->bCNT0 =
	pGSM->bCNT1 = false;

	//bool bFREEinp = true;
	if( pSat ) {
		if(bSTDcout){std::cout  << asACTclr[pGSM->iACTION%6]
								<< asACTname[pGSM->iACTION%6] << " pSat\r\n" << pSat <<std::endl;}
		nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
								iCMTI, iCLIP, iRING, iMISS,
								iVOICE, iCALL, iBEGIN, iEND,
								iMRK,  iCREG,
								pSat, nINP );

		pGSM->so(pAT, nAT);

		//-------------------------------------------------------------------------------------
		if( iCREG > 1 ) {
			pSat = (char*)pINP->p_alloc;
			pSatI = pSat+pAT[iCREG].y-6;
			pGSM->CREG = pSat+pAT[iCREG].y;
			pSat = (char*)pINP->p_alloc;
			nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
									iCMTI, iCLIP, iRING, iMISS,
									iVOICE, iCALL, iBEGIN, iEND,
									iMRK, iCREG,
									pSat, nINP );
			pGSM->so(pAT, nAT);
		}

		///-------------------------------------------------------------------\\
		/// CALL RING
		while( iRING > -1 ) {
			pGSM->nRING++;
			U8 a = pAT[iRING].num-7, b = pAT[iRING].num+3, sub = b-a;
			pINP->lzySUB( a, sub );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
									iCMTI, iCLIP, iRING, iMISS,
									iVOICE, iCALL, iBEGIN, iEND,
									iMRK,  iCREG,
									pSat, nINP );
			pGSM->so(pAT, nAT);
		}
		/// CALL CLIP( Calling line identification presentation )
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

			nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
									iCMTI, iCLIP, iRING, iMISS,
									iVOICE, iCALL, iBEGIN, iEND,
									iMRK, iCREG,
									pSat, nINP );
			pGSM->so(pAT, nAT);
			//bFREEinp = false;
		}
		/// VOICE CALL END
		while( iVOICE > -1 ) {

			pSat = (char*)pINP->p_alloc;
			pSatI = pSat+iVOICE-5;




			pSat = (char*)pINP->p_alloc;
			nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
									iCMTI, iCLIP, iRING, iMISS,
									iVOICE, iCALL, iBEGIN, iEND,
									iMRK, iCREG,
									pSat, nINP );
			pGSM->so(pAT, nAT);
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
					if( i == pGSM->nCLIP )
						break;
					pC0[i] = pC0[pGSM->nCLIP];
					i--;
				}

			}

			pINP->lzySUB( s = pNs-pSat, pNe-pNs );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
									iCMTI, iCLIP, iRING, iMISS,
									iVOICE, iCALL, iBEGIN, iEND,
									iMRK, iCREG,
									pSat, nINP );
			pGSM->so(pAT, nAT);
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

			U8 a = pAT[iCMTI].num-8, b = (pAT[gpmMIN(iCMTI+2,nAT)].num-3);
			pINP->lzySUB( a, b-a );
			nINP = pINP->nLD();
			pSat = (char*)pINP->p_alloc;

			//gpmMcpy( pSat+a, pSat+b, nINP-b+1 );
			//nINP -= sub;

			nAT = pGSM->nAT( pAT, 	iAT, iQ1, iOK, iNO, iERR,
									iCMTI, iCLIP, iRING, iMISS,
									iVOICE, iCALL, iBEGIN, iEND,
									iMRK, iCREG,
									pSat, nINP );
			pGSM->so(pAT, nAT);
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
					case gpeALF_CHUP:
						if( pGSM->nCLIP )
							pGSM->nCLIP = 0;
						nAT = 0;
						pGSM->bCNT0 = true;
						break;
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
					case gpeALF_CGMI: {									// SIM7600 - Module Manufacture
						gpmSTRCPY( pGSM->sCGMI, pSat+pAT[iMRK].num );
						pGSM->bCGMI = true;
						nAT = 0;
					} break;
					case gpeALF_CGMM: {									// SIM7600 - Module MODEL
						gpmSTRCPY( pGSM->sCGMM, pSat+pAT[iMRK].num );
						pGSM->bCGMM = true;
						nAT = 0;
					} break;
					case gpeALF_CGMR: {									// SIM7600 - Request revision identification
						gpmSTRCPY( pGSM->sCGMR, pSat+pAT[iMRK].num );
						pGSM->bCGMR = true;
						nAT = 0;
					} break;
					case gpeALF_COPS: {									// SIM7600 - PROVIDER?
						gpmSTRCPY( pGSM->sCOPS, pSat+pAT[iMRK].num );
						pGSM->bCOPS= true;
						nAT = 0;
					} break;
					case gpeALF_CGSN: {									// SIM7600 - Serial Num
						gpmSTRCPY( pGSM->sCGSN, pSat+pAT[iMRK].num );
						pGSM->bCGSN = true;
						nAT = 0;
					} break;
					case gpeALF_CPSI: {									// SIM7600 - Inquiring UE system information
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
						pGSM->CREG.n = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
						pGSM->CREG.stat = gpfSTR2U8( pSnum, &pSnum ); pSnum++;
						if(iQ1>0) {
							if( pGSM->CREG.stat != 1 )
								iCNT = pGSM->iCREG0;
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
	if( iAT > 0 )
		pINP->lzySUB(s=0,pAT[iAT].num-3);
	else
		gpmDEL(pINP);

	const char* pCLR = *sANSW ? stdRUN : stdCMPLR;
	if( *sANSW ) {
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
		U8 ms = 20000, act, gd, oACT = pGSM->iACTION;
		while( (*pS != 'A') && !*sANSW ) {
			switch( *pS ) {
				case 'w':
				case 'W':
					act = gpfSTR2U8(pS+gpmVAN(pS," \t\r\n+-0123456789",nLEN), &pS);
					gd = gpfSTR2U8(pS+gpmVAN(pS," \t\r\n+-0123456789",nLEN), &pS);
					//ms = 20000;
					if( pGSM->iW >= pWIN->mSEC.x ) {
						iCNT = aGSMcnt[pGSM->iACTION = act];

						if( pGSM->iW-pWIN->mSEC.x < (ms/2) )
							iCNT = aGSMcnt[pGSM->iACTION = gd];

						pS = (char*)pOUT->p_alloc + iCNT;
						break;
					} else if( !pGSM->iW ) {
						pGSM->iW = ms + pWIN->mSEC.x;
						iCNT = aGSMcnt[pGSM->iACTION = act];
						pS = (char*)pOUT->p_alloc + iCNT;
						break;
					}
					// következő sor
					pGSM->iW = 0;
					pS += gpmVAN( pS, "\r\n", nLEN );
					pS += gpmNINCS( pS, "\r\n" );
					break;
				default: {
						switch( pGSM->iACTION = gpfSTR2U8(pS) ) {
							case 1:
								if( pGSM->nCLIP ) {
									pGSM->iACTION = 2;
									iCNT = aGSMcnt[pGSM->iACTION];
									pS = (char*)pOUT->p_alloc+iCNT;
									pGSM->nCLIP = 0;
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

	if(bSTDcout){std::cout	<< stdBREAK << "ACT:" << pGSM->iACTION
							<< stdCMPLR << "CLIP:" << pGSM->iCLIP << "/" << pGSM->nCLIP
							<< stdRUN << "CMTI:" << pGSM->iCMTI << "/" << pGSM->nCMTI
							<< pCLR << sANSW <<std::endl;}
	if( pGSM->iW >= pWIN->mSEC.x )
		if(bSTDcout){std::cout	<< stdBREAK << "WAIT:" << pGSM->iW-pWIN->mSEC.x << "ms"
								<<std::endl;}

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
