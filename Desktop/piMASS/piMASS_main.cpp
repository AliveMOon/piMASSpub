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

U1 gp_s_key_map_sdl[] =
/*
 0123456789abcdef*/
"0000            "		"0123abcdefghijkl"
"                "		"mnopqrstuvwxzy12"
"       ://// :'\""		"3456789onebt uoo"
"'\"3''    3ffffff"		"uu2ea0,.-9123456"
"ffffff444444/44_"		"789abc6789abdder"
"___     /       "		"lduN/*-+n1234567"
"    '66666666666"		"890.i56789abcdef"
"7777777777777777"		"0123456789abcdef"
"8888888888888888"		"0123456789abcdef"
"9999999999999999"		"0123456789abcdef"
"aaaaaaaaaaaaaaaa"		"0123456789abcdef"
"bbbbbbbbbbbbbbbb"		"0123456789abcdef"
"cccccccccccccccc"		"0123456789abcdef"
"dddddddddddddddd"		"0123456789abcdef"
"xxxxxxxxeeeeeeee"		"0123456789abcdef"
"ffffffffffffffff"		"0123456789abcdef"
// shift
"0000            "		"0123ABCDEFGHIJKL"
"                "		"MNOPQRSTUVWXZY'\""
"       ://// :'\""		"+!%/=()ONEBT UOO" // \n
"'\"3''    3FFFFFF"		"UU2EA0?:_9123456"
"FFFFFF444444/44_"		"789abc6789ABDDER"
"___     /       "		"LDUN/*-+n1234567"
"    '66666666666"		"890,i56789abcdef"
"7777777777777777"		"0123456789ABCDEF"
"8888888888888888"		"0123456789ABCDEF"
"9999999999999999"		"0123456789ABCDEF"
"AAAAAAAAAAAAAAAA"		"0123456789ABCDEF"
"BBBBBBBBBBBBBBBB"		"0123456789ABCDEF"
"CCCCCCCCCCCCCCCC"		"0123456789ABCDEF"
"DDDDDDDDDDDDDDDD"		"0123456789ABCDEF"
"xxxxxxxxEEEEEEEE"		"0123456789ABCDEF"
"FFFFFFFFFFFFFFFF"		"0123456789ABCDEF"
// alt
"0000            "		"0123A{&DE[]HIJKL"
"        =       "		"M}OP\\RSTC@|# >~?"
" 2222222/222 222"		"^1234567N9AB DEF"
"333 33 3 3333333"		"012$45;7*9123456"
"FFFFFF444444/44_"		"789abc6789ABDDER"
"___5555555555555"		"LDU3456789ABCDEF"
"6666 66666666666"		"0123<56789ABCDEF"
"7777777777777777"		"0123456789ABCDEF"
"8888888888888888"		"0123456789ABCDEF"
"9999999999999999"		"0123456789ABCDEF"
"AAAAAAAAAAAAAAAA"		"0123456789ABCDEF"
"BBBBBBBBBBBBBBBB"		"0123456789ABCDEF"
"CCCCCCCCCCCCCCCC"		"0123456789ABCDEF"
"DDDDDDDDDDDDDDDD"		"0123456789ABCDEF"
"xxxxxxxxEEEEEEEE"		"0123456789ABCDEF"
"FFFFFFFFFFFFFFFF"		"0123456789ABCDEF"
// shift+alt
"0000            "		"0123ABCDEFGHIJKL"
"              11"		"MNOPQRSTUVWXZYEF"
"22222222 222 222"		"01234567\n9AB DEF"
"3333333333333333"		"0123456789123456"
"FFFFFF444444/44_"		"789abc6789ABDDER"
"___5555555555555"		"LDU3456789ABCDEF"
"6666666666666666"		"0123456789ABCDEF"
"7777777777777777"		"0123456789ABCDEF"
"8888888888888888"		"0123456789ABCDEF"
"9999999999999999"		"0123456789ABCDEF"
"AAAAAAAAAAAAAAAA"		"0123456789ABCDEF"
"BBBBBBBBBBBBBBBB"		"0123456789ABCDEF"
"CCCCCCCCCCCCCCCC"		"0123456789ABCDEF"
"DDDDDDDDDDDDDDDD"		"0123456789ABCDEF"
"xxxxxxxxEEEEEEEE"		"0123456789ABCDEF"
"FFFFFFFFFFFFFFFF"		"0123456789ABCDEF"
;




char gpsEXEpath[gpdMAX_PATH], *gppEXEfile = gpsEXEpath,
	 gpsEXEname[0x100],
	 gpsMASSpath[gpdMAX_PATH], *gppMASSfile = gpsMASSpath,
	 gpsMASSname[0x100];

gpcLAZY gpMASS;
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

	gpcSRC* p_fnd = xFND ? SRCfnd( xFND ) : NULL;
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
	gpcLAZY buff;


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

		pSRC = SRCfnd( pM[i] );
		if( !pSRC )
			continue;

		pNUM = pALF+gpfALF2STR( pALF, (i%z)+1 );
		pNX = pNUM + sprintf( (char*)pNUM, "%d\t", i/z );
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
		pRIG = pA+pSRC->nL;
        pANo = (U1*)strcasestr( (char*)pLFT, (char*)gpsSVadr );

        if( bALT )
        {
			if( !pANo )
			{
				// nem talált további AN címzést azaz nem kell semmit kihagyni
				pANo = pLFT+gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"
				buff.lzy_ins( pANo, iB-(pANo-pA), nS = -1, -1 );

				//continue;
			} else {
				nADR = pNX-gpsSVadr;
				while( pANo-pA < iB )
				{
					buff.lzy_ins( pLFT, pANo-pLFT, nS = -1, -1 );

					pLFT = pANo + nADR;
					pLFT += gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"

					pANo = (U1*)strcasestr( (char*)pLFT, (char*)gpsSVadr );
					if( !pANo )
						break;
				}
				if( pLFT-pA < iB )
					buff.lzy_ins( pLFT, iB-(pLFT-pA), nS = -1, -1 );
			}
			buff.lzyFRMT( nS = -1, "<BR>+--- --  -   <BR>" );
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
				buff.lzy_ins( pLFT, pANo-pLFT, nS = -1, -1 );
				pLFT = pANo;
				continue;
            }

			if( *pANo == '>' )
            {
				pANo++;
				bBR = true;
				buff.lzy_ins( pLFT, pANo-pLFT, nS = -1, -1 );
				pLFT = pANo;
				continue;
            }

			buff.lzy_ins( pLFT, pANo-pLFT, nS = -1, -1 );
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

	buff.lzy_write( (char*)gpsSAVEbf );
	return false;
}
bool gpcMASS::SRCsave( U1* pPATH, U1* pFILE ) {
	if( this ? !mapCR.pMAP : true )
		return false;
	U4	*pM = mapCR.pMAP,
		*pC = mapCR.pCOL;
	gpcLAZY buff;
	gpcSRC* pSRC;
	U4 z = mapCR.mapZN44.z;

	U1 *pA, *pALF = gpsSVadr, *pNUM, *pNX, *pANo, *pLFT, *pRIG;
	U8 nS = -1, nINS, iB, nADR;
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( !pM[i] )
			continue;

		pSRC = SRCfnd( pM[i] );
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
		pRIG = pA+pSRC->nL;
        pANo = (U1*)strcasestr( (char*)pLFT, (char*)gpsSVadr );

		if( !pANo )
		{
			// nem talált további AN címzést azaz nem kell semmit kihagyni
            pANo = pLFT+gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"
            buff.lzy_ins( pANo, pRIG-pANo, nS = -1, -1 );
			continue;
		}
		nADR = pNX-gpsSVadr;
		while( pANo-pA < iB )
        {
			buff.lzy_ins( pLFT, pANo-pLFT, nS = -1, -1 );

			pLFT = pANo + nADR;
			pLFT += gpmNINCS( pLFT, " \t" ); // ne gyarapodjon a cím utáni " \t"

			pANo = (U1*)strcasestr( (char*)pLFT, (char*)gpsSVadr );
			if( !pANo )
				break;
        }
		buff.lzy_ins( pLFT, pRIG-pLFT, nS = -1, -1 );
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
	buff.lzy_write( (char*)pPATH );
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
	null();


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
		if(!tmp.nL)
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
			cout << "[ENTER]"; // << endl;
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
char gpsMAINpub[0x100], gpsTITLEpub[0x100];
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
};

#ifdef _WIN64
//int WINAPI WinMain( int nA, char *apA[] )
//int Main(int nA, char **apA )
int main(int nA, char** apA )
#else
int main( int nA, char *apA[] )
#endif
{
	cout << endl;
	for( U4 i = 0, e = gpmN(aSIZEOF); i < e; i++  )
	{
		cout << aSIZEOF[i].sz << "\t" << aSIZEOF[i].pSTR << "\t" << aSIZEOF[i].sz/0x10 << "\t" << aSIZEOF[i].sz%0x10 << endl;
	}
    cout << "gpeU4x2nSTR" << "\t" << gpeU4x2nSTR << endl;
	gpf_aALF_init();

	if( nA > 0 )
	{
		gppEXEfile = gpfP2F( gpsEXEpath, gpsEXEname, apA[0] );

		cout << "Start in:" << gpsEXEpath << endl;
		cout << "Exe is:" << gpsEXEname << endl;
		*gppMASSfile = 0;
	}

	gpeALF alfFFFFffff = (gpeALF)0xFFFFffff;
	gpfALF2STR( gpsKEYbuff, 0xFFFFffff );
	*gpsKEYbuff = 0;
    try
    {
		IMG_Init( IMG_INIT_JPG|IMG_INIT_PNG );
		if( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
			throw InitError();

		for( int i = 1; i < nA; i++ )
		{
			cout << apA[i] << endl;
			if( strcasestr( apA[i], ".mass" ) )
			{
				gppMASSfile = gpfP2F( gpsMASSpath, gpsMASSname, apA[i] );

				cout << "MASS is:" << gpsMASSname << endl;
				continue;
			}
		}
		if( gppMASSfile == gpsMASSpath )
		{
			struct passwd *pw = getpwuid(getuid());
			const char *pHOME = pw->pw_dir;
			gppMASSfile = gpsMASSpath + sprintf( gpsMASSpath, "./" ); //, pHOME );

		}
		if( !*gpsMASSname )
			strcpy( gpsMASSname, "pi.mass" );



		strcpy( gppMASSfile, gpsMASSname );
		U8 s;
		cout << "Load:"<< gpsMASSpath << endl;
		if( gpfACE(gpsMASSpath, 4) > -1 )
			gpMASS.lzyRD( gpsMASSpath, s = -1, -1 );

		gpcMASS* piMASS = new gpcMASS( gpMASS.p_alloc, gpMASS.n_load );
		if( piMASS )
		if( char* pUND = strcasestr( gpsMASSname, ".undo0x" ) )
		{
			*pUND = 0;
		}
		strcpy( gppMASSfile, "mini_char.png" ); //bmp" );0


		//I4x4 mouseXY(0,0), mouseW(0), winSIZ(800,600,800,600), SRCxycr(0), SRCin(0);
		/*SDL_DisplayMode sdlDM;
		SDL_GetCurrentDisplayMode(0, &sdlDM);
		winSIZ.z = (sdlDM.w*7)/8;
		winSIZ.w = sdlDM.h;
		winSIZ.a4x2[0] = winSIZ.a4x2[1];*/

		gpcWIN win( gpsMASSpath, gppMASSfile, gpsMASSname, piMASS ); //, winSIZ ); //SDL_INIT_VIDEO | SDL_INIT_TIMER );
		//win.pMASS = piMASS;
        gpcCRS main_crs( win, 0 ); //, *apCRS[4];


		// gpmZ(apCRS);
        //apCRS[srcDIV] = &main_crs;
        //sdl.draw();

		win.WINrun( 	"                                        "
						"     **** GRANDPACE piMASS V0.1 ****    "
						"                                        "
						" 1GB RAM SYSTEM  38911 piMASS BYTE FREE "
						"                                        "
						"READY                                   " );

/*
        //SDL_Event ev;
        U1 c = 0;
        U1* pKEY; // = (U1*)SDL_GetKeyboardState(NULL);
        U4 aKT[0x200], scan, bug = 0, nBUG;
        gpmZ(aKT);
        U1 aXY[] = "00";

        I4 nMOV, nMAG = 0, nMB = 0, nMBB = 0, nF = 0;
        gppKEYbuff = (
						gppMOUSEbuff +  sprintf( (char*)gppMOUSEbuff,
																	"                                        "
																	"     **** GRANDPACE piMASS V0.1 ****    "
																	"                                        "
																	" 1GB RAM SYSTEM  38911 piMASS BYTE FREE "
																	"                                        "
																	"READY                                   "
																	//"_"
												)
					);
		//----------------------------------------------------
        //
        //						MAIN WHILE
        //
        //		ha gppKEYbuff == NULL akkor zártuk be a programot
        //
        //----------------------------------------------------
        bool bSHIFT, bCTRL, bALT, abALT[2];
        U4x4 mSEC = 0;
        */


		/// END
        return 0;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return 1;
}
