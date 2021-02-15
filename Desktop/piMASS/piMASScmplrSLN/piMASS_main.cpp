//~ MIT License
//
//~ Copyright (c) 2019 AliveMOon
//
//~ Permission is hereby granted, free of charge, to any person obtaining a copy
//~ of this software and associated documentation files (the "Software"), to deal
//~ in the Software without restriction, including without limitation the rights
//~ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//~ copies of the Software, and to permit persons to whom the Software is
//~ furnished to do so, subject to the following conditions:
//
//~ The above copyright notice and this permission notice shall be included in all
//~ copies or substantial portions of the Software.
//
//~ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//~ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//~ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//~ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//~ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//~ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//~ SOFTWARE.

#include "piMASS.h"
#include "gpcSRC.h"
#include "gpccrs.h"

#include <gpsKEYMAP.h>




char gpsEXEpath[gpdMAX_PATH], *gppEXEfile = gpsEXEpath,
	 gpsEXEname[0x100],
	 gpsMASSpath[gpdMAX_PATH], *gppMASSfile = gpsMASSpath,
	 gpsMASSname[0x100];

gpcLZY gpMASS;
U1 gpdONEcell[] = " \a ";

gpcSRC* gpcMASS::SRCnew( gpcSRC& tmp, U1* pS, I4x2 an, I4 ig, U4 nS )
{
	if( !an.x )
		return NULL;

	U4x2 znP1 = an + I4x2(0,1);
	U4x4 mCR;

	U4	*pM = mapCR.MAPalloc( znP1, mCR );

	U4	iZN = (an * U4x2( 1, mCR.z ))-1,
		xFND = pM[iZN];

	gpcSRC* p_fnd = xFND ? srcFND( xFND ) : NULL;
	if( p_fnd )
	{

		if( pS ? nS : false )
		if( !p_fnd->SRCcmp(pS,nS) )
		{
			p_fnd->SRCcpy(pS,pS+nS);
			p_fnd->srcUPDT( ig );
		}
		return p_fnd;
	}

	if( !pS )
		pS = gpdONEcell;

	U1	*pSe = pS + (nS?nS:gpmSTRLEN(pS)), *pSS;
	aSP44[nSP].a4x2[0] = an;

	tmp.reset( pS, pSe, &pSS, aSP44[nSP] );
	aSPix[nSP] = tmp.IX = nLST;
	U4 n;
	if( nSP )
		aSP44[nSP-1].a4x2[1].mx(  aSP44[nSP].a4x2[0] );

	p_fnd = SRCadd( &tmp, xADD, aSPix[nSP], n );
	if( !p_fnd )
		return NULL;

	p_fnd->iGT = ig;
	pM[iZN] = xADD;
	xADD++;
	return p_fnd;
}









U1	gpsRENMbf[0x1000],
	gpsSAVEbf[0x1000],
	gpsSVadr[0x100];
bool gpcMASS::HTMLsave( U1* pPATH, U1* pFILE, U1* pNAME, bool bALT ) {
	if( this ? !mapCR.pMAP : true )
		return false;
	U4	*pM = mapCR.pMAP,
		*pC = mapCR.pCOL;
	gpcLZY buff;


	gpcSRC* pSRC;
	U4 z = mapCR.mapZN44.z;

	U1 *pA, *pALF = gpsSVadr, *pNUM, *pNX, *pANo, *pLFT, *pRIG;
	U8 nS = -1, nINS, iB, nADR;
	buff.lzyFRMT( nS = -1,	"<HTML>\r\n"
								"<HEAD>\r\n"
								"<TITLE>HTML %s</TITLE>\r\n"
								"</HEAD><BODY>\r\n"
								"<TABLE BGCOLOR=#e0e0e0 "
								"BORDER=0 "
								"CELLPADDING=0 cellspacing=1 valign=top >\r\n",
								pNAME
					);

	bool bTR = false, bTD = false;
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( !(i%z) )
		{
			buff.lzyFRMT( nS = -1, "%s<TR>", bTR ? "</TD></TR>\r\n":"" );
			bTR = true;
			bTD = false;
		}
		bTD = true;
		if( !pM[i] )
		{
			buff.lzyFRMT( nS = -1, "%s<TD BGCOLOR=%s BORDER=0> ", bTD ? "</TD>" : "", "#ffffff"  );
			continue;
		}
		buff.lzyFRMT( nS = -1, "%s<TD BGCOLOR=%s BORDER=1> ", bTD ? "</TD>" : "", ((i%z)+(i/z))%2 ? "#bfbfbf":"#afafaf"  );

		pSRC = srcFND( pM[i] );
		if( !pSRC )
			continue;



		pNUM = pALF+gpfALF2STR( pALF, (i%z)+1 );
		pNX = pNUM + sprintf( (char*)pNUM, "%d", i/z );

		if( pSRC->pMINI->nLD() )
		{
			sprintf( (char*)gpsSAVEbf, "%s%s/%s.mini", pPATH, pNAME, gpsSVadr  );
			pSRC->pMINI->lzyWR( (char*)gpsSAVEbf );
		}
		if( pSRC->pDBG->nLD() )
		{
			sprintf( (char*)gpsSAVEbf, "%s%s/%s.asm", pPATH, pNAME, gpsSVadr  );
			pSRC->pDBG->lzyWR( (char*)gpsSAVEbf );
		}
		pNX += sprintf( (char*)pNX, "\t" );

		if( bALT )
			buff.lzyFRMT( nS = -1, " %s", gpsSVadr );


		pA = pSRC->pA;
		if( !pA )
		{
			// megszünt a string ideje létrehozni ha van pRES
			if( bALT )
				continue;

			buff.lzyFRMT( nS = -1, "<BR>+--- --  -   <BR>" );
			continue;
		}
		iB = pSRC->iB();
		pLFT = pA;
		pRIG = pA+pSRC->n_ld_add();
        pANo = (U1*)gpmSTRiSTR( (char*)pLFT, (char*)gpsSVadr );

        if( bALT )
        {
			if( !pANo )
			{
				// nem talált további AN címzést azaz nem kell semmit kihagyni
				pANo = pLFT+gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"
				buff.lzyINS( pANo, iB-(pANo-pA), nS = -1, -1 );

				//continue;
			} else {
				nADR = pNX-gpsSVadr;
				while( pANo-pA < iB )
				{
					buff.lzyINS( pLFT, pANo-pLFT, nS = -1, -1 );

					pLFT = pANo + nADR;
					pLFT += gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"

					pANo = (U1*)gpmSTRiSTR( (char*)pLFT, (char*)gpsSVadr );
					if( !pANo )
						break;
				}
				if( pLFT-pA < iB )
					buff.lzyINS( pLFT, iB-(pLFT-pA), nS = -1, -1 );
			}
			buff.lzyFRMT(
							nS = -1,	"<BR>"
										gpdPUB //"+--- --  -   "
										"<BR>"
						);
        }
        pLFT = pA+iB+1;
        bool bBR = true;
        while( pRIG > pLFT )
        {
			if( *pLFT == '\n' )
			{
				buff.lzyFRMT( nS = -1, bBR ? "<BR>" : "\n" );
				pLFT++;
				continue;
			}
            pANo = pLFT + gpdVAN( (char*)pLFT, "\n<>" );
            if( *pANo == '<' )
            {
				pANo++;
				bBR = false;
				buff.lzyINS( pLFT, pANo-pLFT, nS = -1, -1 );
				pLFT = pANo;
				continue;
            }

			if( *pANo == '>' )
            {
				pANo++;
				bBR = true;
				buff.lzyINS( pLFT, pANo-pLFT, nS = -1, -1 );
				pLFT = pANo;
				continue;
            }

			buff.lzyINS( pLFT, pANo-pLFT, nS = -1, -1 );
			pLFT = pANo;
        }
	}
	buff.lzyFRMT( nS = -1, "</TD></TR></TABLE></BODY>" );

	sprintf( (char*)gpsSAVEbf, "%s%s/", pPATH, pNAME );
	char* pPR = strrchr( (char*)gpsSAVEbf, '.' );
	if( !pPR )
		pPR = (char*)(gpsSAVEbf + sprintf( (char*)gpsSAVEbf, "%s", gpsSAVEbf ));

	strcpy( pPR, "/index.html");

	strcpy( (char*)gpsRENMbf, (char*)gpsSAVEbf );
	pPR = (pPR-(char*)gpsSAVEbf) + (char*)gpsRENMbf;

	U8 nUNDO = 0;
	while( gpfACE((char*)gpsRENMbf, 0 ) > -1 )
	{
		nUNDO++;
		sprintf( pPR, "/index0x%0.4llx.html", nUNDO );
	}

	if( nUNDO)
		rename( (char*)gpsSAVEbf, (char*)gpsRENMbf );

	buff.lzyWR( (char*)gpsSAVEbf );
	return false;
}
bool gpcMASS::SRCsave( U1* pPATH, U1* pFILE ) {
	if( this ? !mapCR.pMAP : true )
		return false;
	U4	*pM = mapCR.pMAP,
		*pC = mapCR.pCOL;
	gpcLZY buff;
	gpcSRC* pSRC;
	U4 z = mapCR.mapZN44.z;

	U1 *pA, *pALF = gpsSVadr, *pNUM, *pNX, *pANo, *pLFT, *pRIG;
	U8 nS = -1, nINS, iB, nADR;
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( !pM[i] )
			continue;

		pSRC = srcFND( pM[i] );
		if( !pSRC )
			continue;
		pNUM = pALF+gpfALF2STR( pALF, (i%z)+1 );
		pNX = pNUM + sprintf( (char*)pNUM, "%d\t", i/z );
		buff.lzyFRMT( nS = -1, "\a %s", gpsSVadr );

		pA = pSRC->pA;
		if( !pA )
		{
			// megszünt a string ideje létrehozni ha van pRES
			buff.lzyFRMT( nS = -1, "\a " );
			continue;
		}
		iB = pSRC->iB();
		pLFT = pA;
		pRIG = pA+pSRC->n_ld_add();
        pANo = (U1*)gpmSTRiSTR( (char*)pLFT, (char*)gpsSVadr );

		if( !pANo )
		{
			// nem talált további AN címzést azaz nem kell semmit kihagyni
            pANo = pLFT+gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"
            buff.lzyINS( pANo, pRIG-pANo, nS = -1, -1 );
			continue;
		}
		nADR = pNX-gpsSVadr;
		while( pANo-pA < iB )
        {
			buff.lzyINS( pLFT, pANo-pLFT, nS = -1, -1 );

			pLFT = pANo + nADR;
			pLFT += gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"

			pANo = (U1*)gpmSTRiSTR( (char*)pLFT, (char*)gpsSVadr );
			if( !pANo )
				break;
        }
		buff.lzyINS( pLFT, pRIG-pLFT, nS = -1, -1 );
	}
	sprintf( (char*)gpsSAVEbf, "%s", pPATH );
	int iGD = 0;
	if( gpfACE((char*)gpsSAVEbf, 4) > -1 )
	{
		strcpy( (char*)gpsRENMbf, (char*)gpsSAVEbf );
		char* pPR = strrchr( (char*)gpsRENMbf, '.' );
		if(pPR)
		{
			strcpy( pPR, "_undo/" );
			pPR += gpmSTRLEN(pPR);
		}
		U8 nUNDO = 0;
		char s_buff[gpdMAX_PATH];
		if( gpfMKDR( s_buff, (char*)gpsRENMbf ) )
		{
			sprintf( pPR, "0x%0.4llx", nUNDO );
			while( gpfACE((char*)gpsRENMbf, 4) > -1 )
			{
				nUNDO++;
				sprintf( pPR, "0x%0.4llx", nUNDO );
			}
			iGD = rename( (char*)pPATH, (char*)gpsRENMbf );
		}
	}
	buff.lzyWR( (char*)pPATH );
	return false;
}

gpcMASS& gpcMASS::null()
{
	if( gpcSRC** ppS = ppSRC() )
	for( U4 n = pSRCc->n_load/sizeof(U1x4*), i = 0; i < n; i++ )
	{
		gpmDEL( ppS[i] );
	}

	gpmDEL(pSRCc);
	gpmDEL(pLST);

	gpmCLR;
	return *this;
}


gpcMASS& gpcMASS::operator = ( const gpcMASS& b )
{
	return null();
}
gpcMASS::gpcMASS( const U1* pU, U8 nU )
{
	gpmCLR;
	xADD = 1;
	if(!nU)
		return;
	if( pU ? !*pU : true )
		return;

	U1	*pS = (U1*)pU,
		*pSe = pS+nU;
	gpcSRC tmp;

	U4 is, n, id, momLV = 0, mCR, *pMAP;
	nSP = 1;
	U4x4 mpZN;
	while( pS < pSe ? *pS : false )
	{
		tmp.reset( pS, pSe, &pS, aSP44[nSP] );
		if(!tmp.n_ld_add())
			continue;
		tmp.IX = nLST;
		if( tmp.bSUB( *this ) )
		{
			momLV = nSP;
            nSP++;
            aSP44[nSP].null();
		}
		else if( tmp.bENTR( *this, aSP44[nSP] ) )
		{
			if(bSTDcout){std::cout << "[ENTER]";} // <<std::endl;};
		}

		aSP44[momLV].a4x2[1].mx(  aSP44[nSP].a4x2[0] );
        apSP[nSP] = SRCadd( &tmp, xADD, aSPix[nSP], n );

        gpcSRC &spREF = *apSP[nSP];
        if( apSP[momLV] )
        {
			if( !apSP[momLV]->pMAP )
				apSP[momLV]->pMAP = new gpcMAP;
			pMAP = apSP[momLV]->pMAP->MAPalloc( spREF.spcZN.a4x2[0], mpZN ); //, false );
        }
        else
			pMAP = mapCR.MAPalloc( spREF.spcZN.a4x2[0], mpZN ); //, false );

		spREF.bMAIN( *this, true );

		if( pMAP )
		{
			mCR = spREF.spcZN.x + spREF.spcZN.y*mpZN.z;
			pMAP[mCR] = xADD; //aSPix[nSP];
		}

		while( apSP[nSP]->bRET( *this ) )
		{
			//apSP[mom]->space = aSP44[mom];
			apSP[momLV]->retIX = aSPix[nSP];
			momLV--;
			nSP--;
		}


		xADD++;

	}
}


U1	gpsKEYbuff[0x100], *gppKEYbuff = gpsKEYbuff, *gppMOUSEbuff = gpsKEYbuff;
char gpsMNpub[0x1000], gpsTITLEpub[0x100];
I8 gpnEVENT = 0, gpnTITLE = 0;
class U4STR
{
public:
	U4		sz;
	char	*pSTR;
	U4STR( U4 u4, const char* pS )
	{
		sz = u4;
		pSTR = (char*)pS;
	};
};
/*
26976	gpcMASS	1686	0
7528	gpcWIN	470	8
208	gpcCRS	13	0
144	gpcSRC	9	0
144	gpcRES	9	0
848	gpcSTK	53	0
40	gpcREG	2	8
20	gpcGTall	1	4
85528	gpcGT	5345	8
4208	gpcPIC	263	0
72	gpcALU	4	8
32	gpcADR	2	0
24	gpcISA	1	8
32	I8x4	2	0
16	I8x2	1	0
16	U4x4	1	0
8	U4x2	0	8

*/
U4STR aSIZEOF[] = {
	{ sizeof(gpcMASS),	"gpcMASS"	},
	{ sizeof(gpcWIN),	"gpcWIN" 	},
	{ sizeof(gpcCRS),	"gpcCRS" 	},
	{ sizeof(gpcSRC),	"gpcSRC" 	},
	{ sizeof(gpcRES), 	"gpcRES" 	},
	{ sizeof(gpcSTK), 	"gpcSTK" 	},
	{ sizeof(gpcREG), 	"gpcREG" 	},
	{ sizeof(gpcGTall),	"gpcGTall"	},
	{ sizeof(gpcGT),	"gpcGT"		},
	{ sizeof(gpcPIC),	"gpcPIC"	},
	{ sizeof(gpcALU),	"gpcALU" },
	{ sizeof(gpcADR),	"gpcADR" },
	{ sizeof(gpcISA),	"gpcISA" },
	{ sizeof(I8x4),		"I8x4" },
	{ sizeof(I8x2),		"I8x2" },
	{ sizeof(U4x4),		"U4x4" },
	{ sizeof(U4x2),		"U4x2" },
	{ sizeof(I4x4),		"I4x4" },
	{ sizeof(I4x2),		"I4x2" },
	{ sizeof(F4),		"F4" },
	{ sizeof(F2),		"F2" },
	{ sizeof(gpcSLMP),  "gpcSLMP" },
	{ sizeof(gpcDrc),  "gpcDrc" },
	{ sizeof(gpcZS),  "gpcZS" },
	{ sizeof(gpcROB),  "gpcROB" },
	{ sizeof(gpcZSnD),  "gpcZSnD" },
	{ sizeof(gpcLZY),  "gpcLZY" },
};

I4x4 gpaCAGEtst[] = {
	{ 600, 600, 600, gpeZS_BILL }, { 600, 1600, 600, gpeZS_BILL },
	{ 600, 600, 600, gpeZS_BILL }, { 1600, 0, 600, gpeZS_BILL },
	{ 600, 600, 600, gpeZS_BILL }, { -1600, 0, 600, gpeZS_BILL },
	{ 600, 600, 600, gpeZS_BILL }, { -1600, 1600, 600, gpeZS_BILL },
};
I4x4 gpaABC[]
{
		{ -160, 0, 0 },
		{ -160, 33, 0 },
		{ -160, 33, 14 },

};
extern I4x4 gpaCAGEbillBALL[],
			gpaCAGEbillBOX[];
extern U4	gpnCAGEbillBALL,
			gpnCAGEbillBOX;
//F4x4 tMX,iMX, A,B,C;
//F4	iABC, tABC;
gpcDrc gpCHK( gpsMNpub, I4x4( 600, 500, 500 )*mmX(1), I4x4( 600, 500, 400 )*mmX(1), I4x4( 600, 500, 0 )*mmX(1) );
void gpfMAINchk()
{
	if(bSTDcout){std::cout <<std::endl;}
	for( U4 i = 0, e = gpmN(aSIZEOF); i < e; i++  )
	{
		if(bSTDcout){std::cout << aSIZEOF[i].sz << "\t" << aSIZEOF[i].pSTR << "\t" << aSIZEOF[i].sz/0x10 << "\t" << aSIZEOF[i].sz%0x10 <<std::endl;};
	}
    if(bSTDcout){std::cout << "gpeU4x2nSTR" << "\t" << gpeU4x2nSTR <<std::endl;};
	if(bSTDcout){std::cout << 	stdRED	"\033[1;31m Megvan már?"
					stdBLUE "\r\n1. A mini ha szerkesztve van _# ket is rajzolja"
					stdBLUE "\r\nés ne használja az extra karaktereket!"
					stdBLUE "\r\n3. rosszul azonosítja hol van a pointer"
					stdRED	"\r\n2. F5 CPY F6 MOVE stb???"
					stdRESET << std::endl;}
	I4x2 snail;
	for( U4 i = 0; i < 100; i++ )
	{
		snail.snail(i);
		if(bSTDcout){std::cout << i << "->" << snail.pSTR(gpsMNpub) <<std::endl;}
	}
	I4x4 j0 = I4x4( 0, 600, 1500 )*mmX(1), j1 = I4x4( 0, 0, 0 )*mmX(1), t0 = I4x4( 0, 600, 0 )*mmX(1),
		 up0 = j0-t0, xyz;
	for( I8 ti = 0, tn = 540*ms2sec; ti < tn; ti+=10*ms2sec )
	{
		xyz = j0.drop( j1, up0, mmX(100), ti, tn );
		if(bSTDcout){std::cout << ti << "\t" << xyz.pSTR( gpsMNpub ) <<std::endl;}
	}
	for( I8 ti = 0, tn = 540*ms2sec; ti < tn; ti+=10*ms2sec )
	{
		xyz = j1.drop( j0, up0, mmX(100), ti, tn );
		if(bSTDcout){std::cout << ti << "\t" << xyz.pSTR( gpsMNpub ) <<std::endl;}
	}


//	char* pSTR = NULL;
//	for( U4 i = 0; i < gpmN(gpaABC); i++ )
//	{
//		pSTR = gpsMNpub;
//		pSTR += gpaABC[i].str_t( pSTR, ",", "\r\n" );
//		iMX.ABC( gpaABC[i], 180.0/PI);
//		//B.b( gpaABC[i].y, 180.0/PI);
//		//C.c( gpaABC[i].z, 180.0/PI);
//		//iMX = A*B*C; //abc( gpaABC[i], 180.0/PI );
//		tABC = iMX.eABC()*(180.0/PI);
//		tMX.ABC( tABC, 180.0/PI);
//
//		/*A.a( tABC.A, 180.0/PI);
//		B.b( tABC.B, 180.0/PI);
//		C.c( tABC.C, 180.0/PI);
//		tMX = A*B*C;*/
//		//tMX.abc(tABC, 180.0/PI );
//		pSTR += iMX.str( pSTR, ",", "\r\n" );
//		pSTR += tABC.str( pSTR, ",", "\r\n" );
//		pSTR += tMX.str( pSTR, ",", "\r\n" );
//		if(bSTDcout){std::cout << gpsMNpub <<std::endl;}
//	}
//	*gpsMNpub = 0;

}
#ifdef _WIN64
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR apA, INT nA)
{
	if (apA ? nA > 0 : false)
	{
		gppEXEfile = gpfP2F(gpsEXEpath, gpsEXEname, (char*)apA, '\\' );

		if(bSTDcout){std::cout << "Start in:" << gpsEXEpath << std::endl;};
		if(bSTDcout){std::cout << "Exe is:" << gpsEXEname << std::endl;};
		*gppMASSfile = 0;
		nA = 1;
	}
#else
int main( int nA, char *apA[] )

{

	if(	apA[0] ? nA > 0 : false )
	{
		gppEXEfile = gpfP2F( gpsEXEpath, gpsEXEname, (char*)(apA[0]) );

		if(bSTDcout){std::cout << "Start in:" << gpsEXEpath <<std::endl;};
		if(bSTDcout){std::cout << "Exe is:" << gpsEXEname <<std::endl;};
		*gppMASSfile = 0;
	}
#endif

	gpf_aALF_init();
	gpfMAINchk();

	gpcDrc tstCAGE;
	I4x4 tmp;
	for( U4 i = 0, e = gpmN(gpaCAGEtst); i < e; i += 2 )
	{
		tstCAGE.iXYZ.xyz_( gpaCAGEtst[i]*mmX(1) );
		tstCAGE.tXYZ.xyz_( gpaCAGEtst[i+1]*mmX(1) );
		if(bSTDcout){std::cout << "tstCAGE.tXYZ0000:" << (tstCAGE.tXYZ/mmX(1)).pSTR( gpsMNpub ) <<std::endl;};
		tmp = tstCAGE.tXYZ.xyz0();
		tmp = tstCAGE.cageBALL( tmp, gpaCAGEbillBALL, gpnCAGEbillBALL );
		if(bSTDcout){std::cout << "tstCAGE.tXYZball:" << (tmp/mmX(1)).pSTR( gpsMNpub ) <<std::endl;};
		tmp = tstCAGE.cageBOX( tmp, gpaCAGEbillBOX, gpnCAGEbillBOX );
		if(bSTDcout){std::cout << "tstCAGE.tXYZboxx:" << (tmp/mmX(1)).pSTR( gpsMNpub ) <<std::endl;};
	}
	gpeALF alfFFFFffff = (gpeALF)0xFFFFffff;
	gpfALF2STR( gpsKEYbuff, 0xFFFFffff );
	*gpsKEYbuff = 0;
    try
    {
		IMG_Init( IMG_INIT_JPG|IMG_INIT_PNG );
		if( SDL_Init( SDL_INIT_EVERYTHING | SDL_VIDEO_OPENGL ) != 0 )
			throw InitError();

		for( int i = 1; i < nA; i++ )
		{
			if (!(apA[i]))
				continue;

			if(bSTDcout){std::cout << (apA[i]) <<std::endl;};
			if( gpmSTRiSTR( (char*)(apA[i]), ".mass" ) )
			{
				gppMASSfile = gpfP2F( gpsMASSpath, gpsMASSname, (char*)(apA[i]) );

				if(bSTDcout){std::cout << "MASS is:" << gpsMASSname <<std::endl;};
				continue;
			}
		}
		if (gppMASSfile == gpsMASSpath)
		{
			#ifdef _WIN64
				if(bSTDcout){std::cout << "\033[1;31m Meg kell szerezni a user HOME könjyvtárát?" << std::endl;}
			#else

						struct passwd *pw = getpwuid(getuid());
						const char *pHOME = pw->pw_dir;
						gppMASSfile = gpsMASSpath + sprintf( gpsMASSpath, "./" ); //, pHOME );

			#endif
		}
		if( !*gpsMASSname )
			strcpy( gpsMASSname, "pi.mass" );



		strcpy( gppMASSfile, gpsMASSname );
		U8 s;
		if(bSTDcout){std::cout << "Load:"<< gpsMASSpath <<std::endl;};
		if( gpfACE(gpsMASSpath, 4) > -1 )
			gpMASS.lzyRD( gpsMASSpath, s = -1, -1 );

		gpcMASS* piMASS = new gpcMASS( gpMASS.p_alloc, gpMASS.n_load );
		if( piMASS )
		if( char* pUND = gpmSTRiSTR( gpsMASSname, ".undo0x" ) )
		{
			*pUND = 0;
		}

		strcpy( gppMASSfile, gpsMINI_ISO ); //gpsMINI_CHAR ); // "mini_char_12x16.png" ); //bmp" );0
		gpcWIN win( gpsMASSpath, gppMASSfile, gpsMASSname, piMASS );
		gpcCRS main_crs( win, 0 );
		win.winRUN( 	"                                        "
						"     **** GRANDPACE piMASS V0.1 ****    "
						"                                        "
						" 1GB RAM SYSTEM  38911 piMASS BYTE FREE "
						"                                        "
						"READY                                   " );


        return 0;
    }
    catch ( const InitError & err )
    {
        if(bSTDcout){std::cout << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;}
    }

    return 1;
}
