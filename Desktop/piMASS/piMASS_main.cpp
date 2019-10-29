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
 0123456789abcdef
*/
"0000            "		"0123abcdefghijkl"
"                "		"mnopqrstuvwxzy12"
"       ://// :'\""		"3456789onebt uoo"
"'\"3''    3ffffff"		"uu2ea0,.-9123456"
"ffffff444444/44_"		"789abc6789abdder"
"___5555555555555"		"ldu3456789abcdef"
"6666'66666666666"		"0123i56789abcdef"
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
"___5555555555555"		"LDU3456789ABCDEF"
"6666'66666666666"		"0123I56789ABCDEF"
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
/*
U1 gp_s_key_map[] =
// simple
"00123456789-=000"
"qwertzuiopőú11as"
"dfghjkléá022yxcv"
"bnm,.-3*3 3fffff"
"fffff44789-456+1"
"230.555ff5555555"
"6666fff666666666"
"7777777777777777"
"8888888888888888"
"9999999999999999"
"aaaaaaaaaaaaaaaa"
"bbbbbbbbbbbbbbbb"
"cccccccccccccccc"
"dddddddddddddddd"
"eeeeeeeeeeeeeeee"
"ffffffffffffffff"
// shift
"00\'\"+!%/=()0-=00"
"QWERTZUIOP1111AS"
"DFGHJKL22222YXCV"
"BNM?:_3*3 3fffff"
"fffff44789-456+1"
"230.555ff5555555"
"6666fff666666666"
"7777777777777777"
"8888888888888888"
"9999999999999999"
"aaaaaaaaaaaaaaaa"
"bbbbbbbbbbbbbbbb"
"cccccccccccccccc"
"dddddddddddddddd"
"eeeeeeeeeeeeeeee"
"ffffffffffffffff"
// alt
"001234567890-=00"
"\\|ERTYUIOP[]11AS"
"D[]HJKL$2'22>#&@"
"{}M;.*3*3 3fffff"
"fffff44789-456+1"
"230.55<ff5555555"
"6666fff666666666"
"7777777777777777"
"8888888888888888"
"9999999999999999"
"aaaaaaaaaaaaaaaa"
"bbbbbbbbbbbbbbbb"
"cccccccccccccccc"
"dddddddddddddddd"
"eeeeeeeeeeeeeeee"
"ffffffffffffffff";






class gpcWINo
{
	SDL_Rect	txt, chr;
	U1x4		*pTXT;
	U4			nTXT, nX,dX;
	I4			winID;
	SDL_Surface		*pSRFload,
					*pSRFchar,
					*pSRFwin;
    SDL_Renderer	*pSDLrndr;
public:
    SDL_Window		*pSDLwin;
	U1x4			*pCRS;
    gpcWINo( U4 flags = 0, char* pPATH = NULL, char*pFILE = NULL );
    virtual ~gpcWINo();
    void draw();
    void TXT_draw();
    void ins( U1* pC, U1* pM, U1* pB  );
};*/



char gpsEXEpath[gpeMXPATH], *gppEXEfile = gpsEXEpath,
	 gpsEXEname[0x100],
	 gpsMASSpath[gpeMXPATH], *gppMASSfile = gpsMASSpath,
	 gpsMASSname[0x100];

gpcLAZY gpMASS;
U1 gpdONEcell[] = " \a ";

gpcSRC* gpcMASS::SRCnew( gpcSRC& tmp, U1* pS, I4x2 an )
{
	if( !an.x )
		return NULL;

	U4	i = (an * I4x2( 1, mapCR.mapZN44.z ))-1,
		x_fnd = mapCR.pMAP[i];

	gpcSRC* p_fnd = x_fnd ? SRCfnd( x_fnd ) : NULL;
	if( p_fnd )
		return p_fnd;

	if( !pS )
		pS = gpdONEcell;
	U1	*pSe = pS+gpmSTRLEN( pS ), *pSS;
	aSP44[nSP].a4x2[0] = an;

	tmp.reset( pS, pSe, &pSS, aSP44[nSP] );
	aSPix[nSP] = tmp.IX = nLST;
	U4 n;
	if( nSP )
		aSP44[nSP-1].a4x2[1].mx(  aSP44[nSP].a4x2[0] );

	p_fnd = SRCadd( &tmp, xADD, aSPix[nSP], n );
	if( !p_fnd )
		return NULL;
	I4x2 zn = an + I4x2(0,1);
	mapCR.mapZN44.a4x2[0].mx( zn );

	mapCR.pMAP[i] = xADD;
	xADD++;
	return p_fnd;
}



gpcREStrs& gpcREStrs::null()
{
	if( !a )
	{
		gpmCLR;
		return *this;
	}

	switch( typ )
	{
		case gpeNET4_MAS:
			if( an < 2 )
				delete (gpcMASS*)pDAT;
			else
				delete[] (gpcMASS*)pDAT;
			break;
		case gpeNET4_RES:
			if( an < 2 )
				delete (gpcREStrs*)pDAT;
			else
				delete[] (gpcREStrs*)pDAT;
			break;
		default:
			delete[] pDAT;
	}

	gpmCLR;
	return *this;
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
	buff.lzy_format( nS = -1,	"<HTML>\r\n"
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
			buff.lzy_format( nS = -1, "%s<TR>", bTR ? "</TD></TR>\r\n":"" );
			bTR = true;
			bTD = false;
		}
		bTD = true;
		if( !pM[i] )
		{
			buff.lzy_format( nS = -1, "%s<TD BGCOLOR=%s BORDER=0> ", bTD ? "</TD>" : "", "#ffffff"  );
			continue;
		}
		buff.lzy_format( nS = -1, "%s<TD BGCOLOR=%s BORDER=1> ", bTD ? "</TD>" : "", ((i%z)+(i/z))%2 ? "#bfbfbf":"#afafaf"  );

		pSRC = SRCfnd( pM[i] );
		if( !pSRC )
			continue;

		pNUM = pALF+gpfALF2STR( pALF, (i%z)+1 );
		pNX = pNUM + sprintf( (char*)pNUM, "%d\t", i/z );
		if( bALT )
		buff.lzy_format( nS = -1, " %s", gpsSVadr );

		pA = pSRC->pA;
		if( !pA )
		{
			// megszünt a string ideje létrehozni ha van pRES
			if( bALT )
				continue;

			buff.lzy_format( nS = -1, "<BR>+--- --  -   <BR>" );
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
			buff.lzy_format( nS = -1, "<BR>+--- --  -   <BR>" );
        }
        pLFT = pA+iB+1;
        bool bBR = true;
        while( pRIG > pLFT )
        {
			if( *pLFT == '\n' )
			{
				buff.lzy_format( nS = -1, bBR ? "<BR>" : "\n" );
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
	buff.lzy_format( nS = -1, "</TD></TR></TABLE></BODY>" );

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
		buff.lzy_format( nS = -1, "\a %s", gpsSVadr );

		pA = pSRC->pA;
		if( !pA )
		{
			// megszünt a string ideje létrehozni ha van pRES
			buff.lzy_format( nS = -1, "\a " );
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
	U8 nUNDO = 0;
	while( gpfACE((char*)gpsSAVEbf, 4) > -1 )
	{
		nUNDO++;
		sprintf( (char*)gpsSAVEbf, "%s.undo0x%0.4llx", pPATH, nUNDO );
	}
	if( nUNDO)
		rename( (char*)pPATH, (char*)gpsSAVEbf );
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
/*
U1* gpcMASS::justDOit( U1* sKEYbuff, I4x4& mouseXY, U4* pKT, I4x4& SRCxycr, I4x4& SRCin )
{
	U1* pKEYbuff = sKEYbuff;
	gpcSRC	tmp, *pSRC;
	U4 xFND;
	if( U4 *pM = mapCR.pMAP )
	if( U4 *pC = mapCR.pCOL )
	if( U4 *pR = mapCR.pROW )
	if( pM < pC )
	for( U4 i = 0, ie = pC-pM; i < ie; i++ )
	{
		if( !pM[i] )
			continue;

		xFND = pM[i];
		pSRC = SRCfnd( xFND );
		if( !pSRC )
			continue;

		if( pSRC->qBLD() )
		{
			if( !pSRC->apRES[3] )
			{
				pSRC->apRES[3] = pSRC->apRES[3]->compiEASY( pSRC->pSRCstart( true ), NULL, NULL, NULL );
				//gpmDEL( pSRC->apRES[2] ); // OFF? 2
				if( pSRC->apRES[3] )
				{
					//gpmDEL( pSRC->apRES[2] ); // OFF? 1
					if( pSRC->apRES[2] )
					{
						gpmDEL( pSRC->apRES[1] ); // OFF 0
						if( pSRC->apRES[1] )
						{
							gpmDEL( pSRC->apRES[0] );
							pSRC->apRES[0] = pSRC->apRES[1];
						}


						pSRC->apRES[1] = pSRC->apRES[2];
					}
					pSRC->apRES[2] = pSRC->apRES[3];
				}
				pSRC->apRES[3] = NULL;
			}
		}

		if( !pSRC->apRES[2] )
			continue;	// ha nincsen 2 nem sikerült az építés

		// egyébként meg kell probálni futatni
		pSRC->pMINI->lzy_reset();
		pSRC->apRES[2]->run( this, pSRC, NULL );

	}

	return pKEYbuff;
}
*/

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
			pMAP = apSP[momLV]->pMAP->MAPalloc( spREF.spcZN, mpZN, false );
        }
        else
			pMAP = mapCR.MAPalloc( spREF.spcZN, mpZN, false );

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
I8 gpnEVENT = 0;
#ifdef _WIN64
//int WINAPI WinMain( int nA, char *apA[] )
//int Main(int nA, char **apA )
int main(int nA, char** apA )
#else
int main( int nA, char *apA[] )
#endif
{
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
			gpMASS.lzy_read( gpsMASSpath, s = -1, -1 );

		gpcMASS* piMASS = new gpcMASS( gpMASS.p_alloc, gpMASS.n_load );
		if( piMASS )
		if( char* pUND = strcasestr( gpsMASSname, ".undo0x" ) )
		{
			*pUND = 0;
		}
		strcpy( gppMASSfile, "mini_char.png" ); //bmp" );0

		I4x4 mouseXY(0,0), mouseW(0), winSIZ(800,600,800,600), SRCxycr(0), SRCin(0);
		gpcWIN win( gpsMASSpath, gppMASSfile, winSIZ ); //SDL_INIT_VIDEO | SDL_INIT_TIMER );
        gpcCRS main_crs( win, 0 ), *apCRS[4];
        U4 iDIV = 0, nDIV = 1, mDIV = iDIV, selDIV = iDIV;

        gpmZ(apCRS);
        apCRS[iDIV] = &main_crs;
        //sdl.draw();
        SDL_Event ev;
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
        while( gppKEYbuff )
        {

			gpcCRS& crs = apCRS[iDIV] ? *apCRS[iDIV] : main_crs;

			if( (gppKEYbuff != gpsKEYbuff) || nMAG )
			{
				*gppKEYbuff = 0;
				if( piMASS )
				{

					U1	*pS = gppMOUSEbuff,
						*pE = pS;

					if( gppKEYbuff == pS ) //nMAG )
					{
						// nincsen begépelve semmi
						// mondjuk ZOOM, stb..?
						crs.miniRDY(  win, iDIV, *piMASS, gppKEYbuff, pS );
						pS = gppKEYbuff;
					} else {

						while( pE < gppKEYbuff )
						{
							switch( *pE )
							{
								case '\v': {
										//*pE = 0;
										crs.miniRDY( win, iDIV, *piMASS, pE, pS );
										pS = pE+1;
										// tehát ha bent van ki kell lépni a szerkeszttett cellából
										crs.CRSbEDswitch();
									} break;
								case '\t': {
										if( crs.CRSbEDget() )
											break;

										crs.miniRDY( win, iDIV, *piMASS, pE, pS );
										if( *pE == '\r' )
										if( pE[1] == '\n' )
											pE++;

										pS = pE+1;
										crs.CRSstpCL(
														win, *piMASS,
														3, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
													);
									} break;
								case '\r':
								case '\n': {
										if( crs.CRSbEDget() )
											break;


										crs.miniRDY( win, iDIV, *piMASS, pE, pS );
										if( *pE == '\r' )
										if( pE[1] == '\n' )
											pE++;

										pS = pE+1;
										crs.CRSstpCL(
														win, *piMASS,
														5, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
													);
									} break;


								case 2:			// left
								case 3:			// right
								case 4:			// up
								case 5:	{ 		// down
										crs.miniRDY( win, iDIV, *piMASS, pE, pS );
										pS = pE+1;
										if( !crs.CRSbEDget() )
										{
											crs.CRSstpCL(
															win, *piMASS,
															*pE, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
														);

											break;
										}
										//crs.miniRDY( win, iDIV, *piMASS, pE, pS );
										//pS = pE+1;

										//------------------------------------
										//
										//			CRS MOVE
										//
										//------------------------------------
										crs.CRSstpED(
														win, *piMASS,
														*pE, (1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
													);
									} break;
							}
							pE++;
						}

					}
					//if( pS < gppKEYbuff )
					crs.miniRDY(win, iDIV, *piMASS, gppKEYbuff, pS );
					gppKEYbuff = gppMOUSEbuff;
					*gppKEYbuff = 0;
				} else {
					crs.miniINS( gppKEYbuff, gppMOUSEbuff, gpsKEYbuff );
				}
				for( U1 i = 0; i < 4; i++ )
				{
					if( iDIV != i )
					{
						if( !apCRS[i] )
							continue;

						apCRS[i]->miniRDY( win, iDIV, *piMASS, gppKEYbuff, gppKEYbuff );
					}
					apCRS[i]->miniDRW( win, i ); //DIV );
				}
				SDL_UpdateWindowSurface( win.pSDLwin );
			}


			nMB = SDL_GetMouseState( &mouseXY.x, &mouseXY.y );

			gppMOUSEbuff = gppKEYbuff = piMASS->justDOit( gpsKEYbuff, mouseXY, aKT, SRCxycr, SRCin );


			if(
				(
					nMOV =	abs( mouseXY.z-mouseXY.x)+abs( mouseXY.w-mouseXY.y)	// pntr pos
							+abs(nMBB-nMB)										// mBUTTON
							+abs( mouseW.z-mouseW.x)+abs( mouseW.w-mouseW.y)	// mWheel
							+nF
							+nMAG
				) > 0
			)
			{
				gppKEYbuff += sprintf( (char*)gppKEYbuff, "move" );
				gppMOUSEbuff = gppKEYbuff;

				*gpsMAINpub = 0;
				mDIV = win.mDIV( mouseXY.a4x2[0] );
				if( apCRS[mDIV] )
				{
					SRCxycr = apCRS[mDIV]->scnZNCR( win, mDIV, *piMASS, mouseXY.a4x2[0] );

					char *pE = gpsMAINpub + gpfALF2STR( (U1*)gpsMAINpub, apCRS[mDIV]->scnZN.x );
					pE += sprintf( pE, "%d", apCRS[mDIV]->scnZN.y );
					SRCin = apCRS[mDIV]->scnIN;

					if( (nMBB&1) )
					if( !(nMB&1) )
					{
						// SELECT
						if( 1&(aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]) )
						{
							//------------------------------
							//
							// 		AN INSERT
							//
							//---------------------
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s%s",
														(1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT])) ? "#":"",
														gpsMAINpub );
						} else {
							if( iDIV != mDIV )
							if(!(1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT])))
								iDIV = mDIV;
							apCRS[iDIV]->CRSsel(
													win, *apCRS[mDIV], *piMASS,
													(1&(aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]))
												);
						}
					}
				}
				*gppKEYbuff = 0;

				//gppKEYbuff +=
								sprintf(
											gpsTITLEpub,
											"-= piMASS::%s"
											" x:%d y:%d, mDIV: %d"
											" xycr:%s AN:%s"
											" IN %s %0.2f %0.2f"
											" wx:%d wy:%d"
											" %d F%d =-"
											" %d, %d",
											gpsMASSname,
											mouseXY.x, mouseXY.y, mDIV,
											SRCxycr.str(gpsMAINpub+0x40), gpsMAINpub,
											SRCin.str(gpsMAINpub+0x80),
											(float)SRCin.x/SRCin.z, (float)SRCin.y/SRCin.w,
											mouseW.x, mouseW.y,
											nMB, nF,
											bug, nBUG
										);
				mouseXY.z=mouseXY.x;
				mouseXY.w=mouseXY.y;
				mouseW.z=mouseW.x;
				mouseW.w=mouseW.y;
				nMBB = nMB;

				if( nF )
				{
					nF = 0;
				}
				nMAG = 0;

				/*if( gppMASSfile == gpsMASSpath )
				{
					strcpy( gppMASSfile, gpsMASSname );
				}*/
				SDL_SetWindowTitle( win.pSDLwin, gpsTITLEpub );

			}



			while( SDL_PollEvent( &ev ) )
			{
				gpnEVENT++;
				switch( ev.type )
				{
					case SDL_MOUSEWHEEL: {
							if( 1 & (aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]) )
							{
								if( ev.wheel.y )
								{
									//------------------------------
									//
									// 		WHEEL ZOOM
									//
									//---------------------
									I4 mag = -ev.wheel.y;
									SDL_Rect div = win.wDIV(mDIV);
									if( mag < 0 )
									{
										if( apCRS[mDIV]->gtFRMwh(win,mDIV).x == 4 )
											break;

										if( apCRS[mDIV]->gtFRMwh(win,mDIV).x < 4 )
										{
											apCRS[mDIV]->stFRMwh(
																	win,mDIV,
																	4,
																	(4*div.h*2) / (div.w*3)
																);
											nMAG = 1;
											break;
										}
									} else {
										if( apCRS[mDIV]->gtFRMwh(win,mDIV).x == div.w/8 )
											break;

										if( apCRS[mDIV]->gtFRMwh(win,mDIV).x > div.w/8 )
										{
											apCRS[mDIV]->stFRMwh( 	win,mDIV,
																	div.w/8,
																	((div.w/8)*div.h*2) / (div.w*3)
																);

											/*crs.CRSfrm.a4x2[1].x = div.w/8;
											crs.CRSfrm.a4x2[1].y = max( 1, (crs.CRSfrm.a4x2[1].x*div.h*2) / (div.w*3) );*/
											nMAG = 1;
											break;
										}
									}



									apCRS[mDIV]->stFRMwh(
															win,mDIV,
															apCRS[mDIV]->gtFRMwh(win,mDIV).x+mag, 0,
															mag
														);


									nMAG = 1;
									break;
								}
								break;
							}
							//------------------------------
							//
							// 		WHEEL SCROLL
							//
							//---------------------
							if( 1 & (aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]) )
								apCRS[mDIV]->addFRMxy( ev.wheel.y );
							else
								apCRS[mDIV]->addFRMxy( 0, ev.wheel.y );
							nMAG = 1;

							mouseW.x += ev.wheel.x;
							mouseW.y += ev.wheel.y;
						} break;
					case SDL_QUIT:
						gppKEYbuff = NULL;
						continue;
					case SDL_KEYDOWN:
						aKT[ev.key.keysym.scancode] = ev.key.timestamp|1;
						break;
					case SDL_KEYUP: {
							aKT[ev.key.keysym.scancode] = ev.key.timestamp;
							aKT[ev.key.keysym.scancode] &= ~1;

							scan = ev.key.keysym.scancode&0xff;
							// az SDL scan codjábol csinál egy 0x20*y öszeget
							// ami a táblázatban a kívánt karakterre fog mutatni
							scan = (scan%0x10) + (scan/0x10)*0x20;

							// SHIFT & ALT modosíthatja
							// tehát 0x800-as táblázatban tud válogatni
							if( 1 & (aKT[SDL_SCANCODE_LSHIFT]|aKT[SDL_SCANCODE_RSHIFT]) )
								scan |= 0x200;
							if( 1 & (aKT[SDL_SCANCODE_LALT]|aKT[SDL_SCANCODE_RALT]) )
								scan |= 0x400;

							// a táblázat első 0x10 / 16 a vezérlő kód
							// ' ' sima ASCII
							// - :"' - ékezetes betűk
							// fF azok a felső funkció gombok f1f2f3f4 etc...
							// _ kurzor nyilak
							// / azok a szeparátor azaz enter tab cell etc...

							aXY[0] = c = gp_s_key_map_sdl[scan];
							aXY[1] = gp_s_key_map_sdl[scan+0x10];
						} break;
					case SDL_WINDOWEVENT:
						if( ev.window.event != SDL_WINDOWEVENT_RESIZED )
							break;
						SDL_GetWindowSize( win.pSDLwin, &winSIZ.x, &winSIZ.y );
						if( !(abs( winSIZ.z-winSIZ.x)+abs( winSIZ.w-winSIZ.y))	)
							break;

						win.gpeWINresize();

						winSIZ.a4x2[1] = winSIZ.a4x2[0];

				}
				if( c == 'x' )
					continue;
				if( c )
				{
					char *pUTF8 = NULL;
					switch( c )
					{
						case ' ': {
								if( 1 & (aKT[SDL_SCANCODE_LCTRL]|aKT[SDL_SCANCODE_RCTRL]) )
								{
									switch( aXY[1] )
									{
										case 'v':
										case 'V':
											// paste
											gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", SDL_GetClipboardText());
											aXY[1] = 0;
											break;
										case 'c':
										case 'C':
											if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
												SDL_SetClipboardText( (char*)gppKEYbuff );

											*gppKEYbuff = aXY[1] = 0;
											break;
										case 'x':
										case 'X':
											if( crs.gtUTF8( gppKEYbuff ) > gppKEYbuff )
												SDL_SetClipboardText( (char*)gppKEYbuff );

											*gppKEYbuff = 0;
											aXY[1] = 0x7e;
											break;
										case 's':
											strcpy( gppMASSfile, gpsMASSname );
											piMASS->SRCsave( (U1*)gpsMASSpath, (U1*)gppMASSfile );
											*gppKEYbuff = aXY[1] = 0;
											break;
										case 'S':
											// na mencsük ki ami van
											{
												char* pFILE = gppMASSfile+sprintf( gppMASSfile, "HTML/" );
												bool bALT = ( 1 & (aKT[SDL_SCANCODE_LALT]|aKT[SDL_SCANCODE_RALT]) );
												//strcpy( gppMASSfile, gpsMASSname );
												piMASS->HTMLsave( (U1*)gpsMASSpath, (U1*)pFILE, (U1*)gpsMASSname, bALT );
											}
											*gppKEYbuff = aXY[1] = 0;
											break;
									}

								}
								if( !aXY[1] )
									break;
								*gppKEYbuff = aXY[1];
								gppKEYbuff++;
							} break;
						// enter tab izé bizé
						case '/': {
								switch( aXY[1] )
								{
									case 'e':
									case 'E':
										*gppKEYbuff = '\v';
										gppKEYbuff++;
										break;
									case 't':
									case 'T':
										*gppKEYbuff = '\t';
										gppKEYbuff++;
										break;
									case 'b':
									case 'B':
										*gppKEYbuff = '\b';
										gppKEYbuff++;
										break;
									case 'd':
									case 'D':
										*gppKEYbuff = 0x7f;
										gppKEYbuff++;
										break;
									case 'n':
									case 'N':
										gppKEYbuff += sprintf( (char*)gppKEYbuff, "\r\n" );
										break;
								}
							} break;
						// cursor nyilak
						case '_': {
								switch( aXY[1] )
								{
									case 'l':
									case 'L':
										*gppKEYbuff = 2;
										gppKEYbuff++;
										break;
									case 'r':
									case 'R':
										*gppKEYbuff = 3;
										gppKEYbuff++;
										break;

									case 'u':
									case 'U':
										*gppKEYbuff = 4;
										gppKEYbuff++;
										break;
									case 'd':
									case 'D':
										*gppKEYbuff = 5;
										gppKEYbuff++;
										break;

								}
							} break;
						// ékezetek ----------------
						case '\'': {
								switch( aXY[1] )
								{
									case 'A':
										pUTF8 = "\xc3\x81";
										break;
									case 'a':
										pUTF8 = "\xc3\xa1";
										break;
									case 'E':
										pUTF8 = "\xc3\x89";
										break;
									case 'e':
										pUTF8 = "\xc3\xa9";
										break;
									case 'I':
										pUTF8 = "\xc3\x8d";
										break;
									case 'i':
										pUTF8 = "\xc3\xad";
										break;
									case 'O':
										pUTF8 = "\xc3\x93";
										break;
									case 'o':
										pUTF8 = "\xc3\xb3";
										break;
									case 'U':
										pUTF8 = "\xc3\x9a";
										break;
									case 'u':
										pUTF8 = "\xc3\xba";
										break;
									default:
										pUTF8 = "?";
								}
								gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
							} break;
						case '\"': {
								switch( aXY[1] )
								{
									case 'O':
										pUTF8 = "\xc5\x90";
										break;
									case 'o':
										pUTF8 = "\xc5\x91";
										break;
									case 'U':
										pUTF8 = "\xc5\xb0";
										break;
									case 'u':
										pUTF8 = "\xc5\xb1";
										break;
									default:
										pUTF8 = "?";
								}
								gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
							}break;
						case ':': {
								switch( aXY[1] )
								{
									case 'O':
										pUTF8 = "\xc3\x96";
										break;
									case 'o':
										pUTF8 = "\xc3\xb6";
										break;
									case 'U':
										pUTF8 = "\xc3\x90";
										break;
									case 'u':
										pUTF8 = "\xc3\xbc";
										break;
									default:
										pUTF8 = "?";
								}
								gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
							} break;
						case '=': {
								// € EURO izé
								switch( aXY[1] )
								{
									case 'C':
									case 'c':
											pUTF8 = "\u20AC";
											break;
									default:
										pUTF8 = "?";
								}
								gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", pUTF8 );
							} break;
						case 'f':
						case 'F': {
								//------------------------------
								//
								// 		PANEL SWITCH
								//
								//---------------------
								nF = aXY[1] >= 'a' ?	((aXY[1]-'a')+10) :
														aXY[1]-'0';
								if( nF < 2 )
								{
									win.bSW = 1;
									mDIV = 0;
 								}
								else if( nF < 5 )
								{
									mDIV = nF-1;

									U1 msk = (0x1<<mDIV);
									if( win.bSW&msk )
										win.bSW = (win.bSW&(~msk));
									else {
										win.bSW |= msk;
									}
								}
								win.bSW |= 1;
								//if( iDIV != mDIV )
								{
									//iDIV = mDIV;
									if( !apCRS[mDIV] )
											apCRS[mDIV] = new gpcCRS( win, mDIV );
									for( U1 i = 0, sw = win.bSW; i < 4; i++, sw >>= 1 )
									{
										if( !(sw&1) )
											continue;

										if( !apCRS[i] )
											continue;

										apCRS[i]->stFRMwh( win, i, apCRS[i]->gtFRMwh(win, i).x, 0 );


									}
								}
							} break;
						default:
							gppKEYbuff += sprintf( (char*)gppKEYbuff, "%s", aXY );
					}

					c = 0;
				}
			}

        }

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
