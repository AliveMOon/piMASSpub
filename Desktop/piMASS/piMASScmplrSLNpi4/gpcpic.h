#ifndef GPCPIC_H
#define GPCPIC_H
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

//#include "gpcSRC.h"
//GLOVAL(void)
bool gpfSRFjpgSAVE( U1* pFILE, SDL_Surface* pSRF, I4 q );
gpcLZY* gpfSRF2JPG( gpcLZY* pBUFF, SDL_Surface* pSRF, I4 q );
#ifdef gpdSYSubi

#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <sys/mman.h>

class gpcCAMubi
{
    U1 frm;
    U4x2 wh;
    U4 nGRB;


    /// openCAM ----------------------------------------------------------------
    int fd;                             ///  1. Open the device ---------------
    v4l2_capability     capability;     ///  2. Ask the device if it can capture frames
    v4l2_fmtdesc        fmtdesc;        ///  3. VIDIOC_ENUM_FMT-----------------
    v4l2_frmsizeenum    frmsize;        ///  4. VIDIOC_ENUM_FRAMESIZES ---------
    /// setCaptureSize ---------------------------------------------------------
    v4l2_format         format;         ///  5. VIDIOC_S_FMT -------------------
    v4l2_requestbuffers requestBuffer;  ///  6. VIDIOC_REQBUFS -----------------
    v4l2_buffer         queryBuffer;    ///  7. VIDIOC_QUERYBUF ----------------
    int type;                           ///  8. VIDIOC_STREAMON ----------------
    /// greb -------------------------------------------------------------------
    v4l2_buffer         bufferinfo;     ///  9. VIDIOC_QBUF --------------------
                                        /// 10. VIDIOC_DQBUF -------------------
    /// retrieve ---------------------------------------------------------------
                                        /// 11. VIDIOC_STREAMOFF ---------------

    //v4l2_frmivalenum frmival;


    bool bOPEN;
public:
    char* pBUFF;                        /// mmap << queryBuffer.length
    U4x2 wh0;

    bool bLIVE(){ return this ? fd > -1 : false; }
    void setFormat( RASPICAM_FORMAT f )
    {
        frm = f;
    }
    int setCaptureSize( U4 w, U4 h, U4 picFRM = V4L2_PIX_FMT_RGB24 );


    U4 getImageTypeSize( RASPICAM_FORMAT f );
    bool    openCAM();
    U4      grab();
    U1*     getImageBufferData() { return (U1*)pBUFF; }
    U4 getWidth() { return wh.x; }
    U4 getHeight() { return wh.y; }


    void* retrieve( void *pPIX, RASPICAM_FORMAT f );

    gpcCAMubi(){ gpmCLR; fd = -1; };
    ~gpcCAMubi()
    {
        if( fd > -1 )
            close(fd);
        if( !pBUFF )
            return;
        munmap(pBUFF, queryBuffer.length);
    }
};
#endif

class gpcPICAM {
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
		cam.wh0 = wh = I4x2( gpdRPI_WIDTH,gpdRPI_HEIGHT );
		//cam.setCaptureSize( wh.x, wh.y );

		// Open camera
		//cout<<"Opening Camera..."<<endl;
		if( !cam.openCAM() )
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
class gpcBOB {
public:
	U4 		mom, id, strt, srt3,
			nRDall, nRD, lurdR, nX, aiL[0x10], nL, iQC,
			nAREA,			// a bobban szereplő pixelek száma
			iAXIS, lAXIS,	// iAX pX[0] az egyik pont pX[iAXIS] másik pont
			nRND, 			// kerület
			wR;				// sugar
			//nKIDall, iKID;
	I4x4	lurd;
	I4x2	picWH, *pRD, *pFILL, *pX, *pRDsrt, wCNTR, lurdC;
	//gpcBOB	**ppKID;
	I4x2 inDRW( U4 qc )
	{
		if( !nL )
			return 0;

		qc -= iQC;
		U4 i = nL-1;
		if( qc/nL )
		{
			if( qc%nL )
				i -= qc%nL;
			else
				iQC = qc;
		}

		return I4x2( aiL[i], aiL[i+1]-aiL[i] );
	}

	~gpcBOB(){
		gpmDELary(pRD);
	};
	gpcBOB(){ gpmCLR; };
	gpcBOB* pBOB(
					U4* pM, U1 l, U4 s,
					I4x2 Mwh, U4 m,
					I4x2* pR, U4 nR, U4 nRx,
					I4 rg, I4 nA ) {
		if( !this )
		{
			gpcBOB* pTHIS = new gpcBOB;
			if( !pTHIS )
				return NULL;

			return pTHIS->pBOB(
								pM, l, s,
								Mwh, m,
								pR, nR, nRx,
								rg, nA
							);
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


		gpmMcpyOF( pFILL=pRD+nR, gpmMcpyOF(pRD,pR,nR), nR );

		pFILL->median( nR, pRDsrt=pFILL+nR, true );
		nAREA = 0;
		U8 wA = 0, wB = 0, nW = 0;
		lurd.a4x2[0] = picWH = I4x2(rg,Mwh.y);
		lurd.a4x2[1].null();
		if( !nA ) {
			nA = rg*Mwh.y;
		}
		I4 i = 0,a,b,ab;

		for( nRD = 0; nRD < nR; nRD++ ) {
			a = pFILL[nRD].x;

			nRD++;
			b = pFILL[nRD].x;
			/// ennek az a lényege
			/// az ALU egyébként is kiszámolja a hányadost
			/// és a maradékot egy lépésben
			pRD[a].YdivRQ( Mwh.x ).x;	//++; //pFILL[nRD].y++;
			pRD[b].YdivRQ( Mwh.x ).x;	//--;
			ab = pRD[b].x-pRD[a].x;
			if( ab < 2 )
			{
				pFILL[nRD].x = 0;
				continue;
			}
			ab--;
			nW++;
			nAREA += ab; /// AREA
			pFILL[nRD].x = ab;
			wA += pRD[a].x+ab/2;
			wB += pRD[a].y;

			if(	lurd.x>pRD[a].x )
				lurd.x=pRD[a].x;
			if( lurd.z<pRD[b].x )
				lurd.z=pRD[b].x;
		}


		if( nW > 3 ? (nAREA > nA) : true )
		{
			nAREA = nRD = 0;
			return this;
		}

		if( lurd.y>pRD->y )
			lurd.y=pRD->y;
		if( lurd.w<pRD[pFILL[nRD-2].x].y )
			lurd.w=pRD[pFILL[nRD-2].x].y;

		if( (lurd.w-lurd.y) < 3 )
		{
			nAREA = nRD = 0;
			return this;
		}
		else if( (lurd.w-lurd.y) > Mwh.y )
		{
			nAREA = nRD = 0;
			return this;
		}


		if( (lurd.z-lurd.x) < 3 )
		{
			nAREA = nRD = 0;
			return this;
		}

		wCNTR.x = wA/nW;
		wCNTR.y = wB/nW;
		lurdC = (lurd.a4x2[0]+lurd.a4x2[1])/2;
		if( nRD < 4 || nRD > nRx )
		{
			nAREA = nRD = 0;
			return this;
		}


		U1x4& n0x100 = *(U1x4*)&id;
		I4 h = Mwh.y, wh = (rg*h)>>8;
		n0x100.x = (wCNTR.x<<8)/rg;
		n0x100.y = (wCNTR.y<<8)/Mwh.x;
		n0x100.z = 0xff-((nAREA*nAREA)/(wh*wh));
		n0x100.w = l;
		U4* pM1 = pM+1;
		//I4x2 trfQ(1,Mwh.x);
		//id = n0x100.u4;
		for( i = 1; i < nR; i+= 2 )
		{
			if( !pFILL[i].x )
				continue;
			//xx = pFILL[i].x;
			if( pFILL[i].x == 1)
				pM1[pFILL[i-1].y] = id;
			else
				// szar mert nem is inttel tölti fel, memset( pM1+pFILL[i-1].y+1, id, pFILL[i].x*sizeof(id) );
				gpmMsetOF( pM1+pFILL[i-1].y+1, pFILL[i].x, &id );


		}

		//if( !pU1 )
		//	return this;
		/// na lehet gyerekeket nemzeni
		// van kép hozzá


        return this;
	}
	I4x2* pSRTx( U4 n )
	{
		U4 nADD = (nX ? nRD : nX)*n;
		if( pRDsrt > pRD + nRD*3+nADD )
			return pRDsrt;


		I4x2* pKILL = pRD;
		nRDall = gpmPAD( nRD*3+nADD, 0x10)+0x10;

		pRD		= new I4x2[nRDall];
		pFILL	= pRD + nRD;
		pX		= pFILL + nRD;
		pRDsrt	= pX + nADD;

		if( !pKILL )
			return pRDsrt;


		gpmMcpyOF( pRD, pKILL, nRD*2 + nX );
		delete[]pKILL;

		return pRDsrt;
	}
	gpcBOB* X()
	{
		if( this ? !!pX : true )
			return this;

		I4x2 dif = (lurd.a4x2[1]-lurd.a4x2[0]).abs();
		lurdR = (dif.x+dif.y)/2;

		pRDsrt = pSRTx(4);

		nL = nX = 0;
		aiL[nL] = 0;
		pX[0] = 0;

		I8 bb, b = 0, a = 0, aa;
		U4	ib = 0, ja = 0,
			i, j,
			x, xx,
			e;

		*pRD -= wCNTR;
		I4x2	A, B, L,
				R = *pRD;
		for( i = 1; i < nRD; i++ )
		{
			pRD[i] -= wCNTR;
			B = pRD[i]-R;
			bb = B.qlen();

            if( b >= bb )
            	continue;

			ib = i;
			bb = sqrt(b=bb);
			L = B.SCRlft();

			for( j = ja; j < i; j++ )
			{
				if(a)
					a = (L*(pRD[ja]-R))/bb;

				aa = (L*(pRD[j]-R))/bb;
				if( a >= aa )
					continue;

				a = aa;
				ja = j;
			}
		}
		pX[0].y = ja;
		pX[1].x = ib;


		iAXIS = ja = pX[1].y = pX[1].x;
		R = pRD[iAXIS];
		B = *pRD - R;
		lAXIS = bb = sqrt(b);

		L = B.SCRlft();
		a = 0;
		for( j = ja; j < nRD; j++ )
		{
			aa = (L*(pRD[j]-R))/bb;
			if( a >= aa )
				continue;

			a = aa;
			ja = j;
		}
		pX[1].y = ja;
		nX = 2;


		e = gpmN(aiL);
		e = 4;
		for( nL = 1; nL < e; nL++ )
		{
			if( nL > 1 )
			if( aiL[nL-1]-aiL[nL-2] >= nX-aiL[nL-1] )
			{
				nL--;
				nX=aiL[nL];
				break;
			}

			aiL[nL] = nX;
			pRDsrt = pSRTx(4);

			for( x = aiL[nL-1]; x < aiL[nL]; nX++ )
			{
				pX[nX] = pX[x];
				x++;
				xx = ( x < aiL[nL] ) ? pX[x].x : 0;

				ja = pX[nX].x;
				R = pRD[ja];
				L = (pRD[xx]-R).SCRlft();

				ib = pX[nX].y;
				B = pRD[ib]-R;

				aa = (L*B)/sqrt(L.qlen());
				if( aa*aa < 4 )
					continue;


				L = B.SCRlft();
				bb = sqrt(L.qlen());
				a = 0;
				ja++;
				if( nL%2 )
				{
					for( U4 j = ja; j < ib; j++ )
					{
						aa = (L*(pRD[j]-R))/bb;
						if( a >= aa*aa )
							continue;
						a = aa*aa;
						ja = j;
					}
				}
				else for( U4 j = ja; j < ib; j++ )
				{
					aa = (L*(pRD[j]-R))/bb;
					if( a >= aa )
						continue;
					a = aa;
					ja = j;
				}
				pX[nX].y = ja;
				nX++;


				pX[nX].x = ja = ib;
				pX[nX].y = ib = (xx ? xx : nRD);

				R = pRD[ja];
				L = (pRD[xx]-R).SCRlft();
				bb = sqrt(L.qlen());
				a = 0;
				ja++;
				if(bb)
				if( nL%2 )
				{
					for( U4 j = ja; j < ib; j++ )
					{
						aa = (L*(pRD[j]-R))/bb;
						if( a >= aa*aa )
							continue;
						a = aa*aa;
						ja = j;
					}
				}
				else for( U4 j = ja; j < ib; j++ )
				{
					aa = (L*(pRD[j]-R))/bb;
					if( a >= aa )
						continue;
					a = aa;
					ja = j;
				}
				pX[nX].y = ja;

			}
		}



		if(aiL[nL]<nX)
			aiL[nL]=nX;


		R = pRD[pX[aiL[nL-1]].y];
		nRND = sqrt((R-*pRD).qlen());
		for( i = aiL[nL-1]+1; i < nX; i++ )
		{
			nRND += sqrt((pRD[pX[i].x]-R).qlen());
			R = pRD[pX[i].y];
			nRND += sqrt((R-pRD[pX[i].x]).qlen());
		}
		nRND += sqrt((*pRD-R).qlen());

		return this;

	}
};
class gpcPIC {
public:
	I8x2			TnID, alfN;
	U1				sFILE[gpdMAX_PATH], *pFILE;
	gpcLZY			*pPACK;
	U4				id, iSRC, aiQC[2],
					nPIXall, nPIX,
					nBOBall, nBOB,

					bppS, nPKavg;

	SDL_Surface		*pSRF, *pSHR, *pREF;

	SDL_Texture		*pTX,*pTXlock,
                                *pRTX;
	U1x4			*pLOCK;

	I4x4			xyOUT, xySRC, txWH;
	U4x4			nJDOIT;
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
	SDL_Surface* surFREE( SDL_Surface* pF )
	{
		if( !pF )
			return pF;
		bool bFREE = pSRF == pF;
		if( bFREE )
		{
			gpmSDL_FreeSRF(pSRF);
			if( pSHR == pF )
				pSHR = NULL;
		}

		if( pSHR == pF )
		{
			gpmSDL_FreeSRF(pSHR);
			bFREE = true;
		}

		if( pREF == pF )
			pREF = NULL;

		if( bFREE )
			return NULL;

		return pF;
	}
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
		if( !pLOCK )
			pTXlock = NULL;

		pLOCK = NULL;

        if( pTXlock != pTX )
            return true;

        SDL_UnlockTexture(pTXlock);
		pTXlock = NULL;
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
		if( pLOCK )
            pTXlock = pTX;
		ptch /= sizeof(*pLOCK);
		return pLOCK;
	}
	bool unLOCK_o( void ) {
		if( this ? (!pLOCK||!pTX) : true )
			return false;

		SDL_UnlockTexture(pTX);
		pLOCK = NULL;
		return true;
	}
	U1x4* u1x4LOCK_o( SDL_Renderer* pRNDR, U4x2 wh, int& ptch ) {
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
        if( !pREF )
            return pTX;
        if( !pTX )
             return pTX = SDL_CreateTextureFromSurface( pRNDR, pREF );

        SDL_QueryTexture( pTX, &frm, &acc, &txWH.z, &txWH.w );
        if( (pREF->format ? pREF->format->format : 0)  != frm
            || pREF->w != txWH.z || pREF->h != txWH.w
            || acc != SDL_TEXTUREACCESS_STREAMING )
        {
            const char* pPixFRM = SDL_GetPixelFormatName(frm);
            gpmSDL_FreeTX(pTX);
            pTX = SDL_CreateTexture( pRNDR, pREF->format->format,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            pREF->w, pREF->h );
            txWH.z = pREF->w;
            txWH.w = pREF->h;
            if( !pTX )
                return NULL;
        }



        //void* pLCK;
        int pitch;
        if( SDL_LockTexture( pTX, NULL, (void**)&pLOCK, &pitch ) )
            return pTX;
        if( pitch == pREF->pitch )
            memcpy( pLOCK, pREF->pixels, pitch*pREF->h );
        else {
            U4 p = gpmMIN(pitch,pREF->pitch );
            for( U4 i = 0, ip = pREF->pitch, ie = pREF->h*ip,
                    j = 0, jp = pitch;

                    i < ie;

                    i+=ip, j+= pitch )
            {
                gpmMcpy( ((char*)pLOCK)+j, ((char*)pREF->pixels)+i, p );
            }
        }
        SDL_UnlockTexture(pTX);
        pLOCK = NULL;
        return pTX;



	}
	SDL_Texture* surDRWtx_o( SDL_Renderer* pRNDR ) {
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
	U1x4* TOOLspaceTRD(	gpcLZYall& MANus, gpcPIC** ppPIC,
						char* pNAME, char *pPATH, char *pFILE );
	U1x4* TOOLspace(	gpcLZYall& MANus, gpcPIC** ppPIC,
						char* pNAME, char *pPATH, char *pFILE );

	U1x4* TOOLexplode(	gpcLZYall& MANus, gpcPIC** ppPIC,
						char* pNAME, char *pPATH, char *pFILE );

	U1x4* food( U1x4* pPET, U4 i, U4 n,
				char* pPATH, char* pDIR, const char* pEXP = ".png" );

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
