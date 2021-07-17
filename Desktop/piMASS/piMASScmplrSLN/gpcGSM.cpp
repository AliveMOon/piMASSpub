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

	//"AT+CSDVC=3" 						// Switch voice channel device	1hndst 3spkr
	"AT+CMGF=1\r\n"						//	msg format
	"AT+CSCS=\"UCS2\"\r\n"	//	msg Select TE character set
	"AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n"	//
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
char* asACTstat[] = {
				"INIT",
				"REG",
				"IDLE",
				"WAIT",
				"END",
				"BEGI",
				"CHAT",
				"CALL",

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
static const gpeALF aALFokER[] = {
	gpeALF_OK,
	gpeALF_ERROR,
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
			sOUT[0x400],
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
	gpcLZY	AT, clip, cmti, lzyPUB;

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
		pANSW = pANSW->lzyFRMT( s=-1,	"\r\n ACT:%s"
										"\r\n",
										asACTstat[iACTION%gpdACTn] );
		if( aNUM[1] ) {
			pANSW = pANSW->lzyFRMT( s=-1, " call: +%0.11lld last: +%0.11lld\r\n", aNUM[1], aNUM[2] );
		} else if( aNUM[2] ) {
			pANSW = pANSW->lzyFRMT( s=-1, " last: +%d\r\n", aNUM[2] );
		}
		int utc = cgps.utc;



		pANSW = pANSW->lzyFRMT( s=-1, 	" dat: %d %0.2d %0.2d\ttim: %d:%d %d\r\n",
										(cgps.dat%100) + 2000, (cgps.dat/100)%100, (cgps.dat/10000)%100,
										utc/10000, (utc%10000)/100, utc%100 );
		pANSW = pANSW->lzyFRMT( s=-1, 	" sig: %0.2d %0.2d reg: %s\r\n",
										CSQ.x, CSQ.y, ((CREG.stat == 1) ? "\"ok\"" : "\"searching\"") );
		pANSW = pANSW->lzyFRMT( s=-1, 	" lat: %c %6.4f lng: %c %6.4f\r\n",
										(cgps.ns ? cgps.ns : '?'), cgps.lat, (cgps.ew ? cgps.ew: '?'), cgps.log );
		pANSW = pANSW->lzyFRMT( s=-1, 	" alt:   %6.4f spd:   %6.4f\r\n",
										cgps.alt, cgps.spd );

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
		pAT->alfCON(sOUT,nAT);
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

	int answ2pin( char* pANSW, I8x2 *pAT, int nAT, int pin ) {
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
	int answ2csq( char* pANSW, I8x2 *pAT, int nAT, char* pS, int n ) {
		*pANSW = 0;

		int	iOK = pAT->aALFvan( aALFmost, nAT, gpmN(aALFmost));
		pS+=pAT[0].num+1;
		CSQ.x = gpfSTR2I8( pS, &pS );
		pS++;
		CSQ.y = gpfSTR2I8( pS, &pS );

		return iOK;
	}
	int answ2reg( char* pANSW, I8x2 *pAT, int nAT, char* pS, int n ) {
		*pANSW = 0;

		int	iOK = pAT->aALFvan( aALFmost, nAT, gpmN(aALFmost)),
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
			iOK = iCR + pAT[iCR].aALFvan( aALFmost, nAT-iCR, gpmN(aALFmost));
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
		int iOK = pAT->alfFND( gpeALF_OK, nAT ); //pAT->aALFvan( aALFok, nAT, gpmN(aALFok) );
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
	int answCMGRD( char* pANSW, I8x2 *pAT, int nAT, char* pS, const gpeALF* pOKer, int nOKer, gpcWIN* pWIN  ) {
		*pANSW = 0;
		int iOK = pAT->aALFvan( pOKer, nAT, nOKer );
		if( iOK >= nAT )
			return nAT;
		int aiPN[6];
		aiPN[0] = pAT->alfRIG( gpeALF_MRK, nAT, 3 ); aiPN[1] = aiPN[0]+1;
		aiPN[1] += pAT[aiPN[1]].alfRIG( gpeALF_MRK, nAT-aiPN[1], 1 ); aiPN[2] = aiPN[1]+1;

		aiPN[2] += pAT[aiPN[2]].alfRIG( gpeALF_MRK, nAT-aiPN[2], 3 ); aiPN[3] = aiPN[2]+1;
		aiPN[3] += pAT[aiPN[3]].alfRIG( gpeALF_MRK, nAT-aiPN[3], 1 );

		aiPN[4] = aiPN[3];
		aiPN[5] = iOK;
		int ucs2;
		char sUCS2[] = "0x0000", sPATH[0x100];
		U8 s, s0;
		lzyPUB.lzyRST();
		lzyPUB.lzyFRMT( s = 0, "\a enter \a\r\n" );
		s0 = lzyPUB.nLD();
		for( int i = 0, n = gpmN(aiPN); i<n; i+=2 ) {
			if( i == 2 )
				lzyPUB.lzyADD( pS+pAT[aiPN[i]].y, pAT[aiPN[i+1]].y-pAT[aiPN[i]].y-1, s= -1 );
			else for( char *pSi = pS+pAT[aiPN[i]].y+gpmVAN(pS+pAT[aiPN[i]].y,"0123456789aAbBcCdDeEfF",s), *pSe = pS+pAT[aiPN[i+1]].y-1; pSi < pSe; pSi+=4 )
				lzyPUB.utf8(gpfSTR2I8( gpmMcpy(sUCS2+2,pSi,4)-2 ));

			if( pWIN )
			if( !i ) {
				sprintf( pWIN->gppMASSfile, "%s/sms.mass", lzyPUB.p_alloc+s0+1 );
				lzyPUB.n_load = s0;
			}
			lzyPUB.utf8('\r');
			lzyPUB.utf8('\n');
		}

		lzyPUB.lzyWR( pWIN->gpsMASSpath, false, true );


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
gpcLZY* gpcGT::GTgsmOS( gpcLZY* pANS, U1* pSTR, gpcMASS* pMASS, SOCKET sockUSR, U4 mSEC ) {
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
#define gpmGSManswNp2 0x1000
gpcGSM* gpcGT::GTgsm( gpcWIN* pWIN ) {
	gpcGSM	*pGSM = pOUT ? (gpcGSM*)pOUT->p_alloc : NULL;

	if( this ? msGTdie > pWIN->mSEC.x : true )
		return this ? pGSM : 0;

	U8 nLEN, s;
	int baud = 115200, pin = 2028;
	char	*pANSW = NULL, sGO[0x100];

	if( socket == INVALID_SOCKET ) {
		sprintf( sGO, sSER, s_ip );
		if( (socket=(SOCKET)serialOpen( sGO, baud )) < 0 ) {
			if(bSTDcout){gpdCOUT << stdALU "GSM ERR:" << strerror(errno) << gpdENDL;}
			msGTdie = pWIN->mSEC.x + 3000;
			return pGSM;
		}
		else if( !pOUT ){
			int nCNT = 0;
			pOUT = pOUT->lzyADD( pGSM ? pGSM : &gsmZERO, sizeof(gsmZERO), (s=0) );
			iCNT = pOUT->nLD();
			/// 0 CRESET
			pOUT = pOUT->lzyADD( sGSMreset, gpmSTRLEN(sGSMreset)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 1 CREG
			pOUT = pOUT->lzyADD( sGSMreg, gpmSTRLEN(sGSMreg)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 2 IDLE
			pOUT = pOUT->lzyADD( sGSMidle, gpmSTRLEN(sGSMidle)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 2 WAIT ATA
			pOUT = pOUT->lzyADD( sGSMwait, gpmSTRLEN(sGSMwait)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 4 	VOICE CALL END
			pOUT = pOUT->lzyADD( sGSMhup, gpmSTRLEN(sGSMhup)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 5 	VOICE CALL BEGIN
			pOUT = pOUT->lzyADD( sGSMansw, gpmSTRLEN(sGSMansw)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 6	VOICE CALL chat
			pOUT = pOUT->lzyADD( sGSMchat, gpmSTRLEN(sGSMchat)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// 7	VOICE CALL
			pOUT = pOUT->lzyADD( sGSMcall, gpmSTRLEN(sGSMcall)+1, (s=-1) );
			aGSMcnt[nCNT] = s;
			++nCNT;

			/// n
			pGSM = (gpcGSM*)pOUT->p_alloc;
			aGSMcnt[nCNT] = pOUT->nLD();
			++nCNT;
			pGSM->nCNT = nCNT;
		}
	}

	pANSW = (char*)pOUT->Ux( aGSMcnt[pGSM->nCNT], gpmGSManswNp2*2 );
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
			pINP = pINP->lzyADD( pANSW, nR, s=-1 );
		nINP = pINP->nLD();
	}

	*pANSW = 0;
	pS = nINP ? (char*)pINP->p_alloc : NULL;
	I8x2* pAT = pGSM->pAT( nAT, pS, nINP, " \t:" ); //, " \r\n\t:," );
	while( nAT ) {
		gpmZ(aSUB);
		if(bSTDcout){ gpdCOUT
								<< asACTname[pGSM->iACTION%gpdACTn]
								<< " pS:\r\n"
								<< pS <<gpdENDL; }
		pGSM->so( pAT, nAT );

		iA = pAT->aALFvan(aALFat, nAT, gpmN(aALFat));
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
						iA2 = iA2 + pAT[iA2].aALFvan(  aALFat, nAT-iA2, gpmN(aALFat));	///
						if( pAT[iA2].alf == gpeALF_PLUS ){
							iA2++;
							iA2 = iA2 + pAT[iA2].aALFvan(  aALFat, nAT-iA2, gpmN(aALFat));

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
									return pGSM;
								}
						}
						if( !pGSM->bGSMrdy )
							return pGSM;

						if( aSUB[1] > aSUB[0] )
							break;
					case gpeALF_CPIN: 													///
						iA2++;															///
						iA2 += pGSM->answ2pin( pANSW, pAT+iA2, nAT-iA2, pin );			///
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
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CMTI: {
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CGPSINFO: {
						//iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						iA2 += pGSM->answ2gps( pANSW, pAT+iA2, nAT-iA2, pS ); // cgps = pS+pAT[2].num;
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CSQ: {
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CGPS: {
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CGMR: {
						iA2+=2;
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CMGR: {
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CMGRD: {
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					case gpeALF_CPMS: {
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
						aSUB[1] = pAT[iA2].y;
					} break;
					default:
						iA2 += pAT[iA2].aALFvan( aALFok, nAT-iA2, gpmN(aALFok) );
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
				iA += pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;

			} break;
			case gpeALF_ATD:{
				aSUB[1] = aSUB[0] = pAT[iA].y-3;	// ATA
				pGSM->iW = 0;
				iA++;
				iA += pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
				if( iA >= nAT )
					break;
				aSUB[1] = pAT[iA].y;

			} break;
			case gpeALF_CSDVC: {
				aSUB[1] = aSUB[0] = pAT[iA].y-5;	// CSDVC
				iA++;
				iA += pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
				if( iA >= nAT )
					break;
				aSUB[1] = pAT[iA].y;
				break;
			}
			case gpeALF_CPIN:{
				aSUB[0] = pAT[iA].y-5;	// CPIN
				iA++;
				iA += pGSM->answ2pin( pANSW, pAT+iA, nAT-iA, pin );
				aSUB[1] = pAT[iA].y;
			} break;
			case gpeALF_CSQ: {
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);
				iA2 = iA + pGSM->answ2csq( pANSW, pAT+iA, nAT-iA, pS, 2 );
				aSUB[1] = pAT[iA2].y;
			} break;
			case gpeALF_CREG:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//+CREG
				iA2 = iA + pGSM->answ2reg( pANSW, pAT+iA, nAT-iA, pS, 2 );
				aSUB[1] = pAT[iA2].y;
			} break;
			case gpeALF_CLIP:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//+CLIP
				iA += pGSM->answCLIP( pANSW, pAT+iA, nAT-iA, pS );
						//pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
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
				iA2 += pAT[iA2].aALFvan(aALFvoice, nAT-iA2, gpmN(aALFvoice));
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
				iA2 += pAT[iA2].aALFvan(aALFampm, nAT-iA2, gpmN(aALFampm));
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
/// SMS BEGIN ------------------------------------------------------
			case gpeALF_SMS:
				aSUB[1] = aSUB[0] = pAT[iA].y-3;	//SMS
				iA++;
				switch( pAT[iA].alf ){
					case gpeALF_DONE:
						pGSM->bSMS = true;
						break;
					case gpeALF_FULL:
						pGSM->bSMSfull = true;
						sprintf( pANSW, "AT+CMGRD=1\r\n" );
						break;
					default: break;
				}
				aSUB[1] = pAT[iA].y;
				break;
			case gpeALF_CMTI:{
				aSUB[1] = aSUB[0] = gpmMAX(0,pAT[iA].y-5);	//+CMTI
				int iMRK = iA+pAT[iA].alfFND( gpeALF_MRK, nAT-iA ),
					iCM = iMRK+pAT[iMRK].alfFND( gpeALF_MRK, nAT-iMRK );
				if( iCM >= nAT )
					break;
				iA = iCM+1;

				I8x2* pCMTI = (I8x2*)pGSM->cmti.Ux( pGSM->nCMTI, sizeof(*pCMTI) );
				if( pCMTI ) {
					pCMTI->alf = pAT[iMRK+1].alf;
					pCMTI->num = gpfSTR2I8( pS+pAT[iCM].num+2 );
					pGSM->nCMTI++;
					if( pCMTI->num > -1 )
						pGSM->iREAD = pCMTI->num;
				}

				iA += pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
			} break;
			case gpeALF_CMGRD: {
				aSUB[1] = aSUB[0] = gpmMAX( 0, pAT[iA].y-6 );	// +CMGRD

				iA2 = pGSM->answCMGRD( pANSW, pAT+iA, nAT-iA, pS, aALFokER, gpmN(aALFokER), pWIN );
				iA += iA2; //pAT[iA].aALFvan( aALFokER, nAT-iA, gpmN(aALFokER) );
				if( iA >= nAT )
					break;
				/*if( (pAT[iA].alf!=gpeALF_OK) && (pAT[iA].alf!=gpeALF_ERROR) )
					break;*/
				aSUB[1] = pAT[iA].y;
			} break;
			case gpeALF_CPMS: {
				int iOK = pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) ),
					nUSED = gpfSTR2I8(pS+pAT[iA].y+1);
				if( nUSED  )
					sprintf( pANSW, "AT+CMGRD=%d\r\n", nUSED-1 );
				iA += iOK;
				aSUB[1] = pAT[iA].y;
			} break;
/// SMS END ------------------------------------------------------
			case gpeALF_PB:
				aSUB[1] = aSUB[0] = pAT[iA].y-2;	//PB
				iA++;
				pGSM->bPB = (pAT[iA].alf==gpeALF_DONE);
				aSUB[1] = pAT[iA].y;
				break;
			case gpeALF_COPS:
				aSUB[1] = aSUB[0] = pAT[iA].y-4;	//COPS
				iA++;
				iA += pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
				break;
			default:
				if(iA >= nAT)
					iA = 0;
				aSUB[1] = aSUB[0] = gpmMAX( 0, pAT[iA].y-(alfLEN(pAT[iA].alf)+1));	//+CMTI
				iA += pAT[iA].aALFvan( aALFok, nAT-iA, gpmN(aALFok) );
				aSUB[1] = pAT[iA].y;
				break;

		}

		if( aSUB[1] > aSUB[0] ) {
			aSUB[1] += gpmNINCS( pS+aSUB[1], " \r\n\t" );
			nR = serialDataAvail(socket);
			if( nR > 0 ) {
				nR = read( socket, pANSW, nR );
				if( nR > 0 )
					pINP = pINP->lzyADD( pANSW, nR, s=-1 );
				//nINP = pINP->nLD();
			}
			s = aSUB[0];
			pINP->lzySUB(s, aSUB[1]-aSUB[0]);
			nINP = pINP->nLD();
			pS = nINP ? (char*)pINP->p_alloc : NULL;
			if( pS ) {
				pAT = pGSM->pAT( nAT, pS, nINP, " \t:" ); //, " \r\n\t:," );
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
		return pGSM;

	if( *pANSW ) {
		pCLR = stdPURPLE;
		pS = pANSW;
	} else {
		pCLR = stdYELLOW;
		pS = pALL+iCNT;
	}
	pSe = pS + gpmVAN( pS, "\r\n", nLEN );
	if( pSe-pS < 1 )
		return pGSM;
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
						gpdCOUT	<<gpdENDL
									<< stdRED << "WAIT:" << pGSM->iW-(int)pWIN->mSEC.x << "ms"
									<<gpdENDL;
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

							pSe = pANSW + sprintf( pS = pANSW, "AT+CMGRD=%d\r\n", pCMTI->num );
							pGSM->iCMTI++;
							iCNT = aGSMcnt[pGSM->iACTION];
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
		pS = pANSW + (gpmGSManswNp2);
		pSe = pS + sprintf( pS, pANSW, pGSM->sSTR );
	}

	if(bSTDcout) {
		gpdCOUT	<< stdBREAK << "ACT:"	<< pGSM->iACTION
					<< stdCMPLR << "CLIP:"	<< pGSM->iCLIP << "/" << pGSM->nCLIP
					<< stdRUN	<< "CMTI:"	<< pGSM->iCMTI << "/" << pGSM->nCMTI
					<< pCLR << pANSW <<gpdENDL;
	}

	if( pGSM->iW )
	if(bSTDcout){
			gpdCOUT	<<gpdENDL
						<< stdRED << "WAIT:" << pGSM->iW-(int)pWIN->mSEC.x << "ms"
						<<gpdENDL;
	}

	int nW = 0;
	pSi = pS;
	while( pSi < pSe ) {
		nW = write(socket,pSi,pSe-pSi);
		pSi += nW;
	}
	// *pANSW = 0;
	return pGSM;
}

I4 gpMEM::instDOitGSM( gpcGT* pGT ) {
	if( this ? !pGT : true )
		return -1;

	I4 cnt = pGT->iCNT;
	gpcGSM	*pGSM = pGT->GTgsm( pWIN );
	if( cnt == pGT->iCNT )
		return cnt;

#ifdef stdON
	if(bSTDcout){gpdCOUT << stdALU "GSM" << pGT->iCNT;}
#endif
	//gpcGSM	*pGSM = pGT->pOUT ? (gpcGSM*)pGT->pOUT->p_alloc : NULL;
	if( !pGSM )
		return pGT->iCNT;

	gpOBJ	*pOi = pOBJ(gpeALF_GINP),
			*pOo = pOBJ(gpeALF_GOUT);
	U1* pU1 = pOo ? pOo->pU1() : NULL;
	if( !pU1 )
		return pGT->iCNT;

	I8x2 an;
	an.x = *(U4*)pU1;
	an.y = an.x&0xff;
	an.x >>= 0x10;
	int xfnd = pMASS->getXFNDan( an );
	gpcSRC* pS2 = xfnd ? pMASS->srcFND( xfnd ) : NULL;
	if( !pS2 )
		return pGT->iCNT;
	pS2->pMINI = pGSM->answSTAT( pS2->pMINI->lzyRST() );

	return pGT->iCNT;
}
