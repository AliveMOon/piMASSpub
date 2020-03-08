#ifndef GPCPIC_H
#define GPCPIC_H

#include "piMASS.h"

//#include "gpcSRC.h"
//GLOVAL(void)
bool gpfSRFjpgSAVE( U1* pFILE, SDL_Surface* pSRF, I4 q );
gpcLZY* gpfSRF2JPG( gpcLZY* pBUFF, SDL_Surface* pSRF, I4 q );
#ifndef gpdSYSpi


class gpcCAMubi
{
    U1 frm;
    U4x2 wh;
    U4 nGRB;
    bool bOPEN;
public:
    void setFormat( RASPICAM_FORMAT f )
    {
        frm = f;
    }
    void setCaptureSize( U4 w, U4 h )
    {
        wh.x = w;
        wh.y = h;
    }
    bool open()
    {
        if( bOPEN )
            return bOPEN;

        return bOPEN = true;
    }
    U4 getImageTypeSize( RASPICAM_FORMAT f )
    {
        return 0;
    }
    U4 getWidth()
    {
        return wh.x;
    }
    U4 getHeight()
    {
        return wh.y;
    }
    U4 grab()
    {
       return nGRB++;
    }
		// Extract the image
    void* retrieve( void *pPIX, RASPICAM_FORMAT f )
    {
        return pPIX;
    }
    gpcCAMubi(){ gpmCLR; };
};
#endif

class gpcPICAM
{
	bool bCAMU;
public:
	gpdCAMu	cam;
	I4x2 wh;
	gpcPICAM(){
		// Allowable values: RASPICAM_FORMAT_GRAY,RASPICAM_FORMAT_RGB,RASPICAM_FORMAT_BGR,RASPICAM_FORMAT_YUV420

		#ifdef gpdSYSpi
			bCAMU = false;
			cam.setFormat(raspicam::RASPICAM_FORMAT_RGB);
		#else
			bCAMU = true;
			cam.setFormat( RASPICAM_FORMAT_RGB );
		#endif

		// Allowable widths: 320, 640, 1280
		// Allowable heights: 240, 480, 960
		// setCaptureSize(width,height)
		wh = I4x2( gpdRPI_WIDTH,gpdRPI_HEIGHT );
		cam.setCaptureSize( wh.x, wh.y );

		// Open camera
		//cout<<"Opening Camera..."<<endl;
		if( !cam.open() )
		{
			//cerr<<"Error opening camera"<<endl;return -1;
			return;
		}
		usleep(300*1000); // ? ez feltétlen kell
	};
	bool bGD()
	{
		return !bCAMU;
	}

};
class gpcBOB
{
public:
	U4 		mom, 	id, strt,
			nRDall, nRD,
			nAREA,	//iW,
			nKIDall, iKID;
	I4x4	lurd;
	I4x2	*pRD, *pRDsrt, wCNTR, lurdC;
	gpcBOB	**ppKID;
	~gpcBOB(){
		gpmDELary(pRD);

		if( !ppKID )
			return;

		for( U4 i = 0; i < iKID; i++ )
			gpmDEL(ppKID[i]);

		delete[] ppKID;
	};
	gpcBOB(){ gpmCLR; };
	gpcBOB* pBOB(	//U1* pU1,
					U4* pM, U1 l, U4 s,
					I4x2 Mwh, U4 m,
					I4x2* pR, U4 nR, U4 nRx,
					I4 rg, I4 nA ) {
		if( !this )
		{
			gpcBOB* pTHIS = new gpcBOB;
			if( !pTHIS )
				return NULL;

			return pTHIS->pBOB( //pU1,
								pM, l, s,
								Mwh, m,
								pR, nR, nRx,
								rg, nA );
		}
		mom = m;
		strt = s;
		id = 0;

        if( nRDall <= gpmPAD(nR*4,0x10) )
        {
			nRDall = gpmPAD(nR*4,0x10)+0x10;
			gpmDELary(pRD);
			pRD = new I4x2[nRDall];
        }



        gpmMcpyOF( pRD, pR, nR );
        gpmMcpyOF( pRDsrt = pRD+nR, pR, nR );

		pRDsrt->median( nR, pRDsrt+nR,true );
		nAREA = 0;
		U8 wA = 0, wB = 0, nW = 0;
		lurd.a4x2[0] = I4x2(rg,Mwh.y);
		lurd.a4x2[1].null();
		if( !nA )
		{
			nA = rg*Mwh.y;
		}
		for( I4 i = 0,a,b,ab; i < nR; i+= 2 )
		{
			pRDsrt[i+1].y--;
			pRDsrt[i].y++;

			ab = pRDsrt[i+1].y - pRDsrt[i].y;
			if(ab<1)
				continue;
			a = pRDsrt[i].y;
			b = a+ab;

			nW++;
			nAREA += ab; /// AREA
			a %= Mwh.x;
			b /= Mwh.x;
			if(	lurd.x>a )
				lurd.x=a;
			if( lurd.y>b )
				lurd.y=b;

			if( lurd.z<a+ab )
				lurd.z=a+ab;
			if( lurd.w<b )
				lurd.w=b;

			wA += a+ab/2;
			wB += b;

		}

		if( !nW || (nAREA > nA) )
		{
			nAREA = nRD = 0;
			return this;
		}

		wCNTR.x = wA/nW;
		wCNTR.y = wB/nW;
		lurdC = (lurd.a4x2[0]+lurd.a4x2[1])/2;
		if( nR < 9 || nR > nRx )
		{
			nAREA = nRD = 0;
			return this;
		}
		nRD = nR;

		U1x4& n0x100 = *(U1x4*)&id;
		U4 wh = (rg*Mwh.x)>>8;
		n0x100.x = (wCNTR.x<<8)/rg;
		n0x100.y = (wCNTR.y<<8)/Mwh.x;
		n0x100.z = 0xff-((nAREA*nAREA)/(wh*wh));
		n0x100.w = l;

		//id = n0x100.u4;
		for( I4 i = 0,ab; i < nR; i+= 2 )
		{
			ab = pRDsrt[i+1].y-pRDsrt[i].y;
			if(ab<1)
				continue;

			gpmMsetOF( pM+pRDsrt[i].y, ab, &id );
		}

		//if( !pU1 )
		//	return this;
		/// na lehet gyerekeket nemzeni
		// van kép hozzá


        return this;
	}

};
class gpcPIC
{
public:
	I8x2			TnID, alfN;
	U1				sFILE[gpdMAX_PATH], *pFILE;
	gpcLZY			*pPACK;
	U4				id, iSRC, aiQC[2],
					nPIXall, nPIX,
					nBOBall, nBOB,

					bppS, nPKavg;

	SDL_Surface		*pSRF, *pSHR, *pREF;

	SDL_Texture		*pTX, *pRTX;
	U1x4			*pLOCK;

	I4x4			xyOUT, xySRC, txWH;
	gpcPIC			*pSRC;
	gpcBOB			**ppBOB;

	bool			bTHRD;
	std::thread		T;
	//U1		*pPIX;

	~gpcPIC()
	{
		unLOCK();
		for( U4 i = 0; i < nBOB; i++ )
			gpmDEL(ppBOB[i]);

		gpmDELary(ppBOB);
		gpmSDL_FreeTX(pTX);
		gpmSDL_FreeTX(pRTX);
		gpmSDL_FreeSRF(pSRF);
		gpmSDL_FreeSRF(pSHR);

	}
	gpcPIC(){ gpmCLR; pFILE = sFILE; };
	gpcPIC( I8x2 an, U4 i )
	{
		gpmCLR;
		id = i;
		TnID = an;
	}

	U1* getPIX()
	{
		if( !this )
			return NULL;

		return pSRF ? (U1*)pSRF->pixels : NULL;
	}
	U1* getPIX( gpcPICAM* pC, U4 qc );

	SDL_Surface* surDRW() {
		if( !pSHR )
			return pSRF;

		if( pSRF ? aiQC[0] <= aiQC[1] : true )
			return pSHR;

		return pSRF;
	}
	bool unLOCK( void ) {
		if( this ? (!pLOCK||!pTX) : true )
			return false;


		SDL_UnlockTexture(pTX);
		pLOCK = NULL;
		return true;
	}
	U1x4* u1x4LOCK( SDL_Renderer* pRNDR, U4x2 wh, int& ptch ) {
		if( !this )
			return NULL;
		if( pLOCK )
			return pLOCK;

		ptch = 0;

		if( pTX )
		{
			int w = 0, h = 0, acc = 0;
			U4 frm;
			SDL_QueryTexture( pTX, &frm, &acc, &w, &h );
			if(    SDL_PIXELFORMAT_RGBA8888  != frm
				|| wh.x != w || wh.y != h
				|| acc != SDL_TEXTUREACCESS_STREAMING )
				gpmSDL_FreeTX(pTX);
		}
		if( !pTX )
		{
			pTX = SDL_CreateTexture( pRNDR, SDL_PIXELFORMAT_RGBA8888,
											SDL_TEXTUREACCESS_STREAMING,
											 wh.x, wh.y );
			txWH.a4x2[1] = wh;
		}


		if( pTX ? SDL_LockTexture( pTX, NULL, (void**)&pLOCK, &ptch ) : true )
		{
			pLOCK = NULL;
			return NULL;
		}
		ptch /= sizeof(*pLOCK);
		return pLOCK;
	}

	SDL_Texture* surDRWtx( SDL_Renderer* pRNDR ) {
		SDL_Surface* pS = surDRW();

		if( pREF == pS )
			return pTX;

		int acc = 0;
		U4 frm;
		pREF = pS;

		if( pREF ? pTX : NULL )
		{
			SDL_QueryTexture( pTX, &frm, &acc, &txWH.z, &txWH.w );
			if( (pREF->format ? pREF->format->format : 0)  != frm
				|| pREF->w != txWH.z || pREF->h != txWH.w
				|| acc != SDL_TEXTUREACCESS_STREAMING )
			{
				gpmSDL_FreeTX(pTX);
				pTX = SDL_CreateTexture( pRNDR, pREF->format->format,
												SDL_TEXTUREACCESS_STREAMING,
												pREF->w, pREF->h );
				txWH.z = pREF->w;
				txWH.w = pREF->h;
			}

			if( pTX )
			{
				void* pLOCK;
				int pitch;
				if( SDL_LockTexture( pTX, NULL, &pLOCK, &pitch ) )
					return pTX;

				memcpy( pLOCK, pREF->pixels, pitch*pREF->h );
				SDL_UnlockTexture(pTX);
			}
			return pTX;
		}

		return pTX = SDL_CreateTextureFromSurface( pRNDR, pREF );
	}
	U1x4* TOOLspace(	gpcLZYall& MANus, gpcPIC** ppPIC,
						char* pNAME, char *pPATH, char *pFILE );
	U1x4* TOOLexplode(	gpcLZYall& MANus, gpcPIC** ppPIC,
						char* pNAME, char *pPATH, char *pFILE );
};

class gpcPICall
{
	gpcPIC	**ppPIC, *pPIC;
	U4		nPICall, iPICfr, nPICld;
public:
	U4		alfFND( U1* pS );
	gpcPIC*	aluFND( gpcALU& alu );
//	{
//		U4 i =	alu.bSTR() ?
//				PIC.alfFND( (U1*)alu.pDAT ) 	// ez a kép neve
//				: alu.u8();
//
//	}
	gpcPIC*	PIC( U4 i )
	{
		if( pPIC ? pPIC->id == i : false )
			return pPIC;

		if( i < nPICld )
		{
			if( ppPIC[i] )
				return pPIC = ppPIC[i];
		}
		return NULL;
	}
	gpcPIC*	PIC( I8x2 an )
	{
		if( pPIC )
		if( pPIC->TnID == an )
			return pPIC;

		pPIC = NULL;
		if( iPICfr > nPICld )
			iPICfr = nPICld;
		else if( !ppPIC )
			iPICfr = nPICld = nPICall = 0;
		else if( ppPIC[iPICfr] )
			iPICfr = nPICld;

		for( U4 p = 0; p < nPICld; p++ )
		{
			if( !ppPIC[p] )
			{
				if( iPICfr > p )
					iPICfr = p;
				continue;
			}

			if( ppPIC[p]->TnID != an )
				continue;

			pPIC = ppPIC[p];
			return pPIC;
		}
		if( iPICfr >= nPICld )
		{
			nPICld++;
			if( nPICld >= nPICall )
			{
				nPICall += 0x10;
				gpcPIC	**ppKILL = ppPIC;
				ppPIC = new gpcPIC*[nPICall];
				gpmMcpyOF( ppPIC, ppKILL, iPICfr ); // mert nGTld == iGTfr+1
				gpmDELary(ppKILL);
			}
		}
		return ppPIC[iPICfr] = pPIC = new gpcPIC( an, iPICfr );
	}
};



#endif // GPCPIC_H
