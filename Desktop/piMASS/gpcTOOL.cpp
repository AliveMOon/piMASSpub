#include "gpcwin.h"
SDL_Surface* gpapP[0x10];




class gpcTRDbug {
public:
	U1x4	*pI, *pQ; //,*pOUT;
	I4x2	*pR;
	I4		*pD;
	gpcBOB	**ppBOB;
	U4		nBOBall, nBOB, frBOB,
			*pM,
			s, m, wh, rght,
			//mx,
			nDONE, nALL, nR,
			w, h, n_run, n_join;
	std::thread trd;

	~gpcTRDbug() {
		for( U4 i = 0; i < nBOB; i++ )
			gpmDEL(ppBOB[i]);
		if( n_join < n_run )
			trd.join();
		gpmDELary(ppBOB);
	}
	gpcTRDbug(){ gpmZn(this,gpmOFF(gpcTRDbug,trd)); } // gpcTRDbug(){ gpmCLR; };

	gpcBOB* pB() {
		if( nBOBall <= nBOB )
		{
			nBOBall = nBOB+0x10;
			gpcBOB** ppKILL = ppBOB;
			ppBOB = new gpcBOB*[nBOBall];
			if( nBOB )
				gpmMcpyOF( ppBOB, ppKILL, nBOB );
			gpmZnOF( ppBOB+nBOB, nBOBall-nBOB );
			gpmDELary(ppKILL);
		}

		return ppBOB[nBOB];
	}
	U4 flsB() {
		while( frBOB < nBOB )
		{
			if(ppBOB[frBOB])
			{
				frBOB++;
				continue;
			}

			nBOB--;
			if(!ppBOB[nBOB])
				continue;

			ppBOB[frBOB] = ppBOB[nBOB];
			frBOB++;
			ppBOB[nBOB] = NULL;
		}
		return frBOB;
	}
	gpcTRDbug& operator -= ( gpcBOB* pB ) {
		if( !pB )
			return *this;

		flsB();

		for( U4 i = 0; i < nBOB; i++ )
		{
			if( ppBOB[i] != pB )
				continue;

			ppBOB[frBOB=i] = NULL;
			while( frBOB < nBOB )
			{
				nBOB--;
				if(!ppBOB[nBOB])
					continue;

				ppBOB[frBOB] = ppBOB[nBOB];
				frBOB++;
				ppBOB[nBOB] = NULL;
			}
			break;
		}

		return *this;
	}
	gpcTRDbug& operator = ( gpcBOB* pB ) {
		if( !pB )
			return *this;

		flsB();

		for( U4 i = 0; i < nBOB; i++ )
		{
			if( ppBOB[i] == pB )
				return *this;
			if( ppBOB[i] ? true : (frBOB<i) )
				continue;
			frBOB = i;
		}

		if( frBOB < nBOB )
		{
			ppBOB[frBOB] = pB;
			return *this;
		}

		if( nBOBall <= nBOB )
		{
			nBOBall = nBOB+0x10;
			gpcBOB** ppKILL = ppBOB;
			ppBOB = new gpcBOB*[nBOBall];
			if( nBOB )
				gpmMcpyOF( ppBOB, ppKILL, nBOB );
			gpmZnOF( ppBOB+nBOB, nBOBall-nBOB );
			gpmDELary(ppKILL);
		}

		ppBOB[frBOB] = pB;
		nBOB++;
		frBOB = nBOB;
		return *this;
	}

	gpcBOB* pBOB, *pMOM;
	void loop() {
		if( !this )
			return;
		w = pD[2];
		h = wh/w;
		U1x4 s3;
		U1 srt, *p_q = (U1*)pQ;
		for( U4 y = (w+1)*2, ye = wh*2-w; y<ye; y += w*2 )
		{
			for( U4 x = 4, xe = w-4; x < xe; x+=2 )
			{
				s3 = pI[x+y].srt3();
				if( !s3.x )
					continue;
				srt = s3.x; //((((U4)s3.x*(U4)(s3.x>>4))>>8) & ~0x7)|s3.w;
                p_q[(x+y)/2] = srt;
			}
		}

		nDONE = 1;
		U1 l = 0x1;
		U8 nAsum = 0, nAVG = 0, nMX = 0;
		I4x4 LURD( 0x10000,0x10000, 0,0 );
		for( ; s < wh; s++ )
		{
			if( s%w >= rght )
			{
				s += w-rght-1;
				continue;
			}
			if(pM[s]!=m)
				continue;

			nR = pQ->bugU1( pR, pM, m,
							s, pD, wh,
							rght, 0 );
			pBOB = pB()->pBOB(
								pM, l, s,
								I4x2(w,h), m,
								pR+1, pR->x, (w+h)*2,
								rght, 0
							);
			if( !pBOB )
				continue;
			if( pBOB->nAREA <= 9 )
			{
				gpmDEL(pBOB);
				continue;
			}
			pBOB->srt3 = ((U1*)pQ)[s];
			*this = pBOB;
			LURD.a4x2[0].mn( pBOB->lurd.a4x2[0] );
			LURD.a4x2[1].mx( pBOB->lurd.a4x2[1] );
			nMX = max( nMX, pBOB->nAREA );
			nAsum += pBOB->nAREA;
		}
		if( !nBOB)
			return;

		U4 iB = 0, nB, nNEW = nBOB, mn;
		I4x4 ij, trfQ(1, w);
		U1 u1, *pQ1 = (U1*)pQ;
		while( nNEW )
		{
			l++;
			if( l > 5 )
				break;
			LURD += I4x4(1,1,-1,-1);
			mn = (LURD.a4x2[1]-LURD.a4x2[0] ).mn();
			if( mn < 3 )
				break;
			nAVG = nAsum/nNEW;
			if( nAVG <= 32 )
				break;
			if( nAVG >= nMX )
				break;

			nMX = nAsum = 0;


			LURD.swpZWXY();
			for( nB = nBOB; iB < nB; iB++ )
			{
				pMOM = ppBOB[iB];
				if( pMOM ? pMOM->nAREA < nAVG : true )
					continue;
				u1 = pQ1[pMOM->strt];
				m = pMOM->id;
				ij = pMOM->lurd+I4x4(1,1,-1,-1);
				for( U4 i = ij.x; ij.y < ij.w; ij.y++ )
				for( ij.x = i; ij.x < ij.z; ij.x++ )
				{
					s = ij*trfQ;
					if( pM[s] != m )
						continue;

					if( (pM[s]>>24) != (l-1) )
						continue;

					if( pQ1[s] == u1 )
						continue;

					nR = pQ->bugU1( pR, pM, m,
									s, pD, wh,
									pMOM->lurd.z, 0 );

					pBOB = pB()->pBOB(
										pM, l, s,
										I4x2(w,h), m,
										pR+1, pR->x, (w+h)*2,
										pMOM->lurd.z, (pMOM->nAREA*7)/8 );

					if( !pBOB )
						continue;

					if( pBOB->nAREA <= 9 )
					{
						gpmDEL(pBOB);
						continue;
					}

					pBOB->srt3 = ((U1*)pQ)[s];
					*this = pBOB->X();
					LURD.a4x2[0].mn( pBOB->lurd.a4x2[0] );
					LURD.a4x2[1].mx( pBOB->lurd.a4x2[1] );
					nMX = max( nMX, pBOB->nAREA );
					nAsum += pBOB->nAREA;

				}
			}
			nNEW = nBOB-iB;
		}


		m = pM[s];
		//nDONE = nALL;
	}
};

void TRDexp( gpcTRDbug* pT ) {
	return pT->loop();
}
#define gpdEXPdbgCOUT if(false)
U1x4* gpcPIC::TOOLexplode(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE ) {
	gpapP[0] = surDRW();			// TARGET
	gpapP[1] = ppPIC[0]->surDRW();	// dilet
	U1x4* pPIX = gpapP[0] ? (U1x4*)gpapP[0]->pixels : NULL;

	if( !gpapP[1] )
		return pPIX;

	if( gpapP[0] )
	if( gpapP[0]->h != gpapP[1]->h*2 || gpapP[0]->pitch != gpapP[1]->pitch )
		gpapP[0] = surFREE(gpapP[0]);

	if( !gpapP[0] )
	{
		txWH.a4x2[1] = ppPIC[0]->txWH.a4x2[1]&I4x2(1,2);
		gpapP[0] =
		pSRF = SDL_CreateRGBSurface( 0, txWH.z, txWH.w, 32, 0,0,0,0 );
		if( !pSRF )
			return NULL;
	}

	if( aiQC[0]%3 )
	{
		aiQC[0]++;
		return pPIX;
	}
	aiQC[0] = aiQC[1]+1;

	pREF = NULL;
	///--------------------

	I4	w = txWH.a4x2[1].x,		wQ = w>>1,
		h = txWH.a4x2[1].y>>1,	hQ = h>>1,
		wh = w*h, 				whQ = wh>>1,
		wph = w+h, wph2 = wph*2,
		dwn = wh-w,
		aDIR[] = { -w, +1, w, -1 }, any = 0;	// URDL

	U1x4	*pI = (U1x4*)gpapP[1]->pixels, s3;
	U1		*pQ = (U1*)gpapP[0]->pixels, srt;


	gpmZnOF( pQ, wh*(1+4) ); // U1-től törlünk

	U4	*pM = (U4*)(pQ+wh);
	I4x2	*pR = (I4x2*)(pM+wh), *pRi = pR,
			vQ, ofQ(wQ, hQ), trfQ(1,w);


	gpdEXPdbgCOUT
	std::cout << "Explode: " << any <<std::endl;


	U4 mom = 0, nBtrd = 0;

	gpcTRDbug aBUG[4];
	U8 nB = 0, nA = 0, trd=0, nF = 0, nF2, k;
	for( U4 i = 0, e = gpmN(aBUG), q; i < e; i++ )
	{
		vQ = I4x2( i%2, i/2 );
		q = (
			((vQ&0x100000001)&ofQ)
			+(vQ/2)
		)*trfQ;
		aBUG[i].pI = pI+(vQ*trfQ);
		aBUG[i].pQ = (U1x4*)(pQ+q);
		aBUG[i].pM = pM+q;
		aBUG[i].pD = aDIR;
		aBUG[i].pR = pR + i*wh/16;
		aBUG[i].s = aDIR[2]+1;
		aBUG[i].m = 0;
		aBUG[i].wh = wh/2;
		aBUG[i].rght = aDIR[2]/2;
		aBUG[i].nDONE = 0;
		aBUG[i].n_run = 0;

		if( i >= 3 )
		{
			trd = i;
			break;
		}
		gpdEXPdbgCOUT std::cout << "Explode: nTRD " << i <<std::endl;
		aBUG[i].trd = std::thread( TRDexp, aBUG+i );
		aBUG[i].n_run++;
		trd++;
	}

	gpdEXPdbgCOUT
	std::cout << "Explode: nTRD " << trd <<std::endl;

	aBUG[trd].loop();

	gpdEXPdbgCOUT std::cout << "Explode: mainLOOP " << trd <<std::endl;
	for( U4 i = 0; i <= trd; i++ ) {
		if( aBUG[i].n_join < aBUG[i].n_run )
		{
			aBUG[i].trd.join();
			aBUG[i].n_join = aBUG[i].n_run;
		}
		nB += aBUG[i].nBOB;
		for( U4 j = 0; j < aBUG[i].nBOB; j++ )
		{
			if(!aBUG[i].ppBOB[j])
				continue;
			// iAXIS lAXIX
			/// wCNTR wR
			/// nAREA

			nA += aBUG[i].ppBOB[j]->nAREA;
		}
	}
	if( nB ) {
		gpcBOB *pBA,*pBB;
		U4 nFall = nB*3;
		I4x2* pF = new I4x2[nFall], *pK;
		nF = 0;
		nF2 ;
		for( U4 i = 0, j; i <= trd; i++ )
		for( j = 0; j < aBUG[i].nBOB; j++ )
		{
			pBB = aBUG[i].ppBOB[j];
			if( pBB ? !pBB->nRND : true )
				continue;
			pF[nF].x = (i<<24) + j;
			pF[nF].y = pBB->nRND;
			nF++;
		}

		pF->median( nF, pF+nF, true );
		nF /= 2;

		nF2 = nF*nF;
		if( nF2+nF > nFall )
		{
			nFall = nF2+nF;
			pK = pF;
			pF = new I4x2[nFall];
            gpmMcpyOF( pF, pK, nF );
            delete[] pK;
		}
		pK = pF+nF;
		k = 0;
		for( U4 i = 0,t,b,j; i < nF; i++ )
		{
			b = pF[i].x;
			pBA = aBUG[b>>24].ppBOB[b&0xffffff];
			for( j = i+1; j < nF; j++ )
			{
				b = pF[j].x;
				pBB = aBUG[b>>24].ppBOB[b&0xffffff];
				pK[k].x = (i<<16)|j;
				pK[k].y = sqrt((pBA->wCNTR-pBB->wCNTR).qlen()) + abs(pBA->nRND-pBB->nRND) ;
				k++;
			}
		}
		int m = pK->median( k, pK+k, true ); //, true );
		for( U4 i = 0; i < k; i++, k = i )
		if( pK[k].y > m )
			break;

		if( k )
		{
			if( k > 0x8 )
				k = 0x8;
			U4 n = 0, k2 = k*2;
			if( nBOBall < 0x20 )
			{
				gpcBOB** ppKILL = ppBOB;
				nBOBall = 0x20;
				ppBOB = new gpcBOB*[nBOBall];
				if( ppKILL )
				{
					gpmMcpyOF( ppBOB, ppKILL, nBOB );
				}
				gpmZnOF( ppBOB+nBOB, nBOBall-nBOB);
			}
            for( U4 i = min(nBOB+k2,nBOBall)-1; k2 <= i; i-- )
            {
				// copyback
				gpmDEL(ppBOB[i]);
				ppBOB[i] = ppBOB[i-k2];
				ppBOB[i-k2] = NULL;
            }

            for( U4 i = 0,b,t,j; i < k; i++ )
            {
				b = pK[i].x;
				for( j = 0; j < 2; j++, b >>= 0x10 )
				{
					t = pF[b&0xffff].x;
					pBB = aBUG[t>>24].ppBOB[t&0xffffff];
					if( !pBB )
						continue;

					gpmDEL( ppBOB[n] );
					ppBOB[n] = pBB;
					n++;
					aBUG[t>>24].ppBOB[t&0xffffff] = NULL;
				}
            }
            nBOB += k2;
            if( nBOB > nBOBall )
				nBOB = nBOBall;


			gpdEXPdbgCOUT
			if( nBOB )
			{
				std::cout << "Explode: " << nB << "/" << nA <<std::endl;
				for( U4 i = 0; i < k2; i++)
				if( pBB = ppBOB[i] )
					std::cout << i << "BOB:"
								<< pBB->wCNTR.x
								<< "x"
								<< pBB->wCNTR.y
								<< "r"
								<< pBB->wR
								<< "rnd"
								<< pBB->nRND
								<< "A"
								<< pBB->nAREA <<std::endl;
			}
			delete[] pF;
		} else
			delete[] pF;
	}
	//gpdEXPdbgCOUT std::cout << "Explode: " << nB << "/" << nA <<std::endl;
	return (U1x4*)gpapP[0]->pixels;

}
U1x4* gpcPIC::food( U1x4* pPET, U4 i, U4 n,
				char* pPATH, char* pDIR, char* pEXP ) {
	if( this ? !pPET : true )
		return NULL;

	pPET += 4 + n*i;
	if( pPET->z&0xf )	// alive!
		return pPET;

	if( pPET->w != 1 )	// on HDD?
		return pPET;

	(TnID+I8x2(0,i)).
	an2str( (U1*)pDIR, (U1*)pEXP, true, true );
	std::cout << "FOOD:" << pPATH <<std::endl;

	SDL_Surface* pTMP = NULL;
	if( gpfACE( pPATH, 4) > -1 )
	{
		pTMP = IMG_Load( pPATH );
		if( !pTMP )
		{
			pPET->w = 0;
			gpmSDL_FreeSRF(pTMP);
			std::cout << " ERR! NoGood FILE" <<std::endl;
			return pPET;
		}

		if( pTMP->pitch*pTMP->h == n*sizeof(*pPET) )
		{
			pPET->zyxw( pTMP->pixels, n );
			pPET->x = 0x10; // x nCMP
			pPET->y =		// nCMP == y save
			pPET->z = 0x1;	// z alive!
			pPET->w = 2;	// w load.
			gpmSDL_FreeSRF(pTMP);
			std::cout << " OK" <<std::endl;
			return pPET;
		}

		pPET->w = 0;
		gpmSDL_FreeSRF(pTMP);
		std::cout << " ERR! NoGood SIZE" <<std::endl;
		return pPET;

	}

	std::cout << " NoFnd!" <<std::endl;
	pPET->w = 0;
	return pPET;
}

#define gpdSPCdbgCOUT if(false)
#define gpdLZYdbSPClim 0x20
class gpcTRDspc
{
public:
	gpcLZY*	pDB;
	U1		sRAM[0x100], *pRAM,
			sPATH[0x100], *pDIR;
	U1x4	*pSRC,						// pl. CAM
			*pScpy, *pFcpy, *pBcpy,		// COPY
			*pSpen, *pFpen, *pBpen,		// RNDR
			*pSspc, *pFspc, *pBspc,		// RNDR
			*pSPC,
			x05;
	U4x4	aHISTI[0x40];
	I4x2	srcWH, spcWH;
	I8x2	ALFid;
	U4x4*	pMAP;
	U4		n_join,n_run, id;

	std::thread trd;

	gpcTRDspc(){ gpmZn(this,gpmOFF(gpcTRDspc,trd)); } // gpmCLR; };
	~gpcTRDspc() {
		if( n_join < n_run )
			trd.join();
	}

	SDL_Surface* food( U4 x = 0 ) {
		/// új FOOD
		if( !pMAP->z )
			pMAP->z = 10;
		pMAP->y = 0;
		pMAP->x = x ? x : x05.w;
		U4 ptch = 0x80;
		U4x4* pM = pDB->xFND( pMAP->x, ptch*0x80, gpdLZYdbSPClim );
		U1x4* p_u = NULL, *p_s = NULL;
		if( pM )
		{
			p_u = pDB->pU1x4( *pM, ptch*0x80 );
			if( pM->y )
			{
				pSspc[0]
				.cpyX( 	p_u,						0x40*4,	spcWH.x*0x100, 	4,	spcWH.x*4,	1,	ptch );
				pSspc[spcWH.x*0x100]
				.cpyX( 	p_u + ptch*0x40,			0x80*4,	spcWH.x*0x100, 	4,	spcWH.x*4,	1,	ptch );

				// rndTRG_CPY-ből a FRONT
				pFspc->cpyX( p_u+0x40, 				0x40*2,	spcWH.x*0x40,	2,	spcWH.x*2, 	1,	ptch );
				// rndTRG_CPY-ből a BACK
				pBspc->cpyX( p_u+0x40+ptch*0x20,	0x40*2,	spcWH.x*0x40,	2,	spcWH.x*2, 	1,	ptch );
				return NULL;
			}
		}

		(ALFid+I8x2(0,pMAP->x))
		.an2str( pDIR, (U1*)"spc.png", true, true );
		/// LOAD
		/*if( id < 4 )
			return NULL;*/
		SDL_Surface* pSRF = IMG_Load( (char*)sPATH );
		if( !pSRF )
			return NULL;

		// helyére pakolász
		p_s = (U1x4*)pSRF->pixels;
		if( ptch != pSRF->pitch/sizeof(*p_s) )
		{
			U4 p = pSRF->pitch/sizeof(*p_s);
			if( p_u )
			{
				std::cout	<< " food.w:"	<< ptch
							<< " pitch:"	<< p <<std::endl;

				p_u[0].cpyX( p_s, min(ptch,p), ptch*min(0x80,pSRF->h), 1, min(ptch,p) );
				p_s = p_u->zyxw( p_s, ptch*0x80 );
				p_u = NULL;
			}
		}

		if(p_u)
		{
			pM->y = ptch*0x80;
			p_s = p_u->zyxw( p_s, ptch*0x80 );
		} else
			p_s->zyxw( NULL, ptch*0x80 );

		//								//	dw		dwh = dsy*y		dsx	dsy			ssx	ssy
		// rndTRG_CPY-ből a SPaCe		//
		pSspc[0]
		.cpyX( 	p_s,						0x40*4,	spcWH.x*0x100, 	4,	spcWH.x*4,	1,	ptch );
		pSspc[spcWH.x*0x100]
		.cpyX( 	p_s + ptch*0x40,			0x80*4,	spcWH.x*0x100, 	4,	spcWH.x*4,	1,	ptch );

		// rndTRG_CPY-ből a FRONT
		pFspc->cpyX( p_s+0x40, 				0x40*2,	spcWH.x*0x40,	2,	spcWH.x*2, 	1,	ptch );
		// rndTRG_CPY-ből a BACK
		pBspc->cpyX( p_s+0x40+ptch*0x20,	0x40*2,	spcWH.x*0x40,	2,	spcWH.x*2, 	1,	ptch );
		return pSRF;

	}
	SDL_Surface* save( U4 x = 0, bool bNO = false ) {
		if(x)
			pMAP->x = x;

		SDL_Surface* pSRF = SDL_CreateRGBSurface( 0, 0x80, 0x80, 32, 0,0,0,0 );
		U1x4 *p_d = (U1x4*)pSRF->pixels;
		U4 ptch = pSRF->w, ssy = spcWH.x;
		if( ptch != pSRF->pitch/sizeof(*p_d) )
		{
			ptch = pSRF->pitch/sizeof(*p_d);
			std::cout	<< " save.w:"	<< pSRF->w
						<< " pitch:"	<< ptch <<std::endl;
		}
		// SPaCe
		p_d[0x00]
					//	dw		dwh = dsy*y		dsx	dsy		ssx	ssy
		.cpyX( pSspc, 	0x80, 	ptch*0x80, 		1, 	ptch, 	4, 	ssy*4 );

		// FRONT
		p_d[0x40]
					//	dw		dwh = dsy*y		dsx	dsy		ssx	ssy
		.cpyX( pFspc, 	0x40,	ptch*0x20,		1,	ptch,	2,	ssy*2 );
		// BACK
		p_d[0x40+ptch*0x20]
					//	dw		dwh = dsy*y		dsx	dsy		ssx	ssy
		.cpyX( pBspc,	0x40,	ptch*0x20,		1,	ptch,	2,	ssy*2 );

		if( U4x4* pM = pDB->xFND(pMAP->x,ptch*0x80,gpdLZYdbSPClim) )
		if( U1x4* p_u = pDB->pU1x4(*pM,ptch*0x80) )
		{
			pM->y = ptch*0x80;
			gpmMcpyOF( p_u, p_d, ptch*0x80 );
		}

		if( bNO )
			return pSRF;

		(ALFid+I8x2(0,pMAP->x))
		.an2str( pDIR, (U1*)"spc.png", true, true );
		pRAM = sRAM+sprintf( (char*)sRAM, "/mnt/ram/" );
		gpmSTRCPY(pRAM,pDIR);
		if( gpmACE(sRAM, 4) > -1 )
			remove((char*)sRAM);

		IMG_SavePNG( pSRF, (char*)sRAM );
		if( gpmACE( sPATH, 4) > -1 )
		{
			(ALFid+I8x2(0,pMAP->x))
			.an2str( pRAM, (U1*)".kill", true, true );
			rename( (char*)sPATH, (char*)sRAM );
		}

		gpmSTRCPY(pRAM,pDIR);
		if( int err = rename( (char*)sRAM, (char*)sPATH ) )
		{
			std::cout << id << ":" << (char*)sRAM << "\t" << (char*)sPATH <<std::endl;
			gpcLZY load; U8 s = 0;
			load.lzyRD( (char*)sRAM, s, 1 );
			load.lzyWR( (char*)sPATH, true );
			std::cout << " LZYcpy: " << sRAM << " to " << sPATH <<std::endl;
			remove((char*)sRAM);
			std::cout << " KILL:" << sRAM <<std::endl;
		}

		(ALFid+I8x2(0,pMAP->x))
		.an2str( pRAM, (U1*)".kill", true, true );
		if( gpmACE(sRAM, 4) > -1 )
		{
			remove((char*)sRAM);
			std::cout << " KILL:" << sRAM <<std::endl;
		}

		return pSRF;
	}
	void DO() {
		if(!pSRC)
			return;
		U1x4 c;
		// a space most 0x200 X 0x200 lesz
		// 4 X 4 azaz 16 terület
		// 0x100 / 4 -> 0x40 dec 64 || csat 6 felső bit zaj kinyír
		// a FOOD 0x80 X 0x80 azaz 128x128
		/// 64*32
		/// 0x40 X 0x20 -x4-> 0x100 X 0x80
		gpmZ(aHISTI);
		U1* p_s = (U1*)pSRC;
		U4	of = (id&1)+((id>>2)&1),
			of4 = of<<2,
			shf8 = (id%3)*8,
			nXY = 0;
		for( U4 y = 0, ys = srcWH.x*4; y < 0x80; y+=4 )
		for( U4 x = 0, //(of4+y)&4,
				ixy = x+y*ys; x < 0x100; x+=8, ixy+=8 )
		{
			//pSPC[(x+y*spcWH.x)] = (U4)255<<shf8;
			nXY++;
			c = (U1x4)(p_s[ixy*3])>>2;
			aHISTI[c.x].x++;
			aHISTI[c.y].y++;
			aHISTI[c.z].z++;
		}
		for( U4 y = 0x22, ys = srcWH.x*4; y < 0x60; y+=4 )
		for( U4 x = 0x42, //(of4+y)&4,
				ixy = x+y*ys; x < 0xc0; x+=8, ixy+=8 )
		{
			//pSPC[(x+y*spcWH.x)] = (U4)255<<shf8;
			nXY++;
			c = (U1x4)(p_s[ixy*3])>>2;
			aHISTI[c.x].x++;
			aHISTI[c.y].y++;
			aHISTI[c.z].z++;
		}
		nXY /= 2;
		for( x05.w = 1; x05.w < 0x40; x05.w++ )
		{
			// 0x40*0x10 a 0x40*0x20 szelet felét kell elérni
			// x05 azaz x0.5
			aHISTI[x05.w] += aHISTI[x05.w-1];
			if( aHISTI[x05.w].x < nXY )
				x05.x = x05.w;
			if( aHISTI[x05.w].y < nXY )
				x05.y = x05.w;
			if( aHISTI[x05.w].z < nXY )
				x05.z = x05.w;
		}

		x05.w = x05.srt3().x;
		SDL_Surface* pTMP = NULL;

		if( pMAP->x == x05.w ) {
			// marad a régiben
			if( pMAP->x ) // x 1 és 0x3f3f3f között ~0x400000 // 0 tök sötét le van szarva
			if( pMAP->y )
			if( pMAP->y == pMAP->z ) {
				// azért időnként elmentjük
				pTMP = save();

				pMAP->z+=10;
				pMAP->y = pMAP->z+1;
			} else
				pMAP->y++;

		} else {
			gpdSPCdbgCOUT std::cout << id << ":" << (int)x05.w << "/" << (int)pMAP->x <<std::endl;

            // csere nem stimmel
            /// az a lényeg, ha le van nyomva az egér gomb a RNDR képet átmásolja a maskii-be
            /// és ha most egy újabb körben is azonos akkor azt akarjuk eltárolni
			if( pMAP->x ) // x 1 és 0x3f3f3f között ~0x400000 // 0 tök sötét le van szarva
			if( pMAP->y )
			if( pMAP->y < pMAP->z )
			{
				// természetesen ha frissítve lett mentjük
				pTMP = save();
				gpmSDL_FreeSRF(pTMP);
			}

            /// új FOOD
			if( !pMAP->z )
				pMAP->z = 10;
			pMAP->y = 0;
			pMAP->x = x05.w;
			/// LOAD
			pTMP = food();
		}
		gpmSDL_FreeSRF(pTMP);

		if( !pFcpy )
			return;

		U1x4	fC, GD,// fP,
				v;
		U4 bC, bP, n = 0, xyP;
		I4x2 trfT(1,spcWH.x);
		for( U4 i = 0, iy = spcWH.x*2, ie = iy*0x20; i < ie; i += iy )
		for( U4 x = i, xe = x+0x80; x < xe; x+=2 )
		{
			fC.u4 = pFcpy[x].u4&0xffffff;
			if( !fC.u4 )
				continue;
			if( (pFpen[x].u4&0xffffff) != fC.u4 )
				continue;

			bC = pBcpy[x].u4 & ~0x030303;
			bP = pBpen[x].u4 & ~0x030303;
			if( bC != bP )
				continue;


			pBspc[x].u4 = v.u4 = bP;

			GD = pBcpy[x].u4 = pFcpy[x].u4 = 0;

			/// ! zyx	///
			///   RGB ! ///
			xyP = I4x2( 0x100+v.z, 0x100+v.y )*trfT;// RG
			GD.u4 |= (pSspc[xyP]>>=2).u4;
			pSspc[xyP].u4 |= fC.u4;

			xyP = I4x2( 0x100-v.x, 0x100+v.y )*trfT;// BG
			GD.u4 |= (pSspc[xyP]>>=2).u4;
			pSspc[xyP].u4 |= fC.u4;

			xyP = I4x2( 0x100-v.x, 0x100-v.z )*trfT;// BR
			GD.u4 |= (pSspc[xyP]>>=2).u4;
			pSspc[xyP].u4 |= fC.u4;

			pFspc[x] = GD.u4|fC.u4;
			n++;
		}

		if( !n )
			return;

		// talált azonos pixeleket
		// el kéne tárolni
		pMAP->y = 1;
		pMAP->z = 10;
	}
};
void TRDspc( gpcTRDspc* pT ) {
	return pT->DO();
}
U1x4* gpcPIC::TOOLspaceTRD(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE ) {

	if( pSRF )
	if( pSRF->w*pSRF->h != 640*896 )
		gpmSDL_FreeSRF( pSRF );
	if( !pSRF ) {
		txWH.a4x2[1] = I4x2(640,896);
		pSRF = SDL_CreateRGBSurface( 0, txWH.z, txWH.w, 32, 0,0,0,0 );
	}
	aiQC[0] = aiQC[1]+1;
	pREF = NULL;
	gpmZ(gpapP);
	gpapP[0] = pSRF;				// space 	TRG
	gpapP[1] = ppPIC[0]->surDRW();	// cam		pico
	gpapP[2] = ppPIC[1]->surDRW();	// penna	pici
	gpapP[3] = ppPIC[2]->surDRW();	// cpy		picii
	if( !gpapP[0] || !gpapP[1] || !ppPIC[2] )
		return gpapP[0] ? (U1x4*)pSRF->pixels : NULL;

	char* pDIR = pFILE+sprintf( pFILE, "%s/", pNAME );
	pDIR = strrchr( pFILE, '.' );
	if( pDIR )
		pDIR += sprintf( pDIR, "_dir/" );
	else
		pDIR = pFILE+gpmSTRLEN(pFILE);
	if( gpfACE(pPATH, 4) < 0 ) {
		std::cout << "mkdir:"<< pPATH <<std::endl;
		gpfMKDR( (char*)MANus.aPUB, pPATH );
	}

	gpcTRDspc aSPC[0x10];
	U4 e = gpmN(aSPC), trd = 0, nDIR = pDIR-pPATH;

	// cam pico
	aSPC[0].pSRC	= (U1x4*)gpapP[1]->pixels; // cam pico
	aSPC[0].srcWH	= *gpapP[1];

	aSPC[0].pSPC 	= (U1x4*)gpapP[0]->pixels;
	aSPC[0].spcWH = *gpapP[0];
	if( aSPC[0].spcWH.x != gpapP[0]->pitch/sizeof(*aSPC[0].pSPC) )
	{
		std::cout	<< " spcWH.x:"	<< aSPC[0].spcWH.x
					<< " pitch:"	<< (gpapP[0]->pitch/sizeof(*aSPC[0].pSPC)) <<std::endl;
		aSPC[0].spcWH.x = gpapP[0]->pitch/sizeof(*aSPC[0].pSPC);
	}

	I4x2	of4x4 = 0, of2x2,
			trfSPC(1, aSPC[0].spcWH.x ),
			trfSRC(1, aSPC[0].srcWH.x );

	aSPC[0].ALFid 	= TnID;
	aSPC[0].ALFid.num = 0;

	aSPC[0].pSspc 	=
	aSPC[0].pFspc 	=
	aSPC[0].pSPC 	= (U1x4*)gpapP[0]->pixels;
	aSPC[0].pMAP	= ((U4x4*)(aSPC[0].pSspc + 320*aSPC[0].spcWH.x));

	aSPC[0].pSspc 	+= I4x2(32,320+32)*trfSPC;
	aSPC[0].pFspc 	+= I4x2(320,320)*trfSPC;
	aSPC[0].pBspc 	= aSPC[0].pFspc + I4x2(0,160)*trfSPC;

	I4x4 ofSFB( aSPC[0].pSspc-aSPC[0].pSPC,
				aSPC[0].pFspc-aSPC[0].pSPC,
				aSPC[0].pBspc-aSPC[0].pSPC );

	// penna pici
	aSPC[0].pBpen	=
	aSPC[0].pSpen 	=
	aSPC[0].pFpen 	= (U1x4*)gpapP[2]->pixels;

	aSPC[0].pSpen 	+= ofSFB.x;
	aSPC[0].pFpen 	+= ofSFB.y;
	aSPC[0].pBpen 	+= ofSFB.z;


	// cpy picii
	aSPC[0].pBcpy	=
	aSPC[0].pScpy	=
	aSPC[0].pFcpy	= gpapP[3] ? (U1x4*)gpapP[3]->pixels : NULL;

	aSPC[0].pScpy 	+= ofSFB.x;
	aSPC[0].pFcpy 	+= ofSFB.y;
	aSPC[0].pBcpy 	+= ofSFB.z;

	gpmMcpy( aSPC[0].sPATH, pPATH, nDIR )[nDIR] = 0;
	aSPC[0].pDIR = aSPC[0].sPATH + nDIR;
	aSPC[0].id = trd = 0;

	for( U4 i = 0; i <= e;  i++ )
	{
		if(i)
		{
			memcpy( aSPC+trd, aSPC, gpmOFF(gpcTRDspc,trd) );

			aSPC[trd].id = trd;
			aSPC[trd].ALFid.num += aSPC[trd].id<<16;
			aSPC[trd].pDB = MANus.LZY( aSPC[trd].ALFid );
			aSPC[trd].pDIR = aSPC[trd].sPATH + nDIR;

			ofSFB.x = (of4x4&I4x2(0x40,0x20))*trfSRC;
			ofSFB.y = of4x4*trfSPC;
			ofSFB.z = of2x2*trfSPC + ofSFB.x;
			aSPC[trd].pSRC	+= ofSFB.x;

			aSPC[trd].pSspc += ofSFB.y;
			aSPC[trd].pFspc += ofSFB.z;
			aSPC[trd].pBspc += ofSFB.z;

			aSPC[trd].pSpen += ofSFB.y;
			aSPC[trd].pFpen += ofSFB.z;
			aSPC[trd].pBpen += ofSFB.z;

			aSPC[trd].pScpy += ofSFB.y;
			aSPC[trd].pFcpy += ofSFB.z;
			aSPC[trd].pBcpy += ofSFB.z;

			aSPC[trd].pSPC	+= ofSFB.z*2 - ofSFB.x;
			aSPC[trd].pMAP	+= i;

			aSPC[trd].n_run++;
			aSPC[trd].trd = std::thread( TRDspc, aSPC+trd );
			if( i>=e )
				break;
		}

		of4x4 = I4x2(i%4,i/4);
		of2x2 = of4x4%2;
		aSPC[i].id = trd = i;

	}
	// egyet a föszálban is csinálunk, hogy ne join-nel teljen az idő
	// legjobb ha mind végez míg ez
	//trd = 2;
	aSPC[trd].DO();

	/*aSPC[6].loop();
	aSPC[9].loop();
	aSPC[10].loop();*/

	if( trd >= e-1 )
	for( U4 i = 0; i < trd; i++ )
	if( aSPC[i].n_join < aSPC[i].n_run )
	{
		aSPC[i].trd.join();
		aSPC[i].n_join = aSPC[i].n_run;
	}

	///aSPC[trd].loop();
}
U1x4* gpcPIC::TOOLspace(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE ) {

	if( pSRF )
	if( pSRF->w*pSRF->h != 640*896 )
		gpmSDL_FreeSRF( pSRF );
	if( !pSRF ) {
		txWH.a4x2[1] = I4x2(640,896);
		pSRF = SDL_CreateRGBSurface( 0, txWH.z, txWH.w, 32, 0,0,0,0 );
	}
	pREF = NULL;
	gpmZ(gpapP);
	gpapP[0] = pSRF;				// space 	TRG
	gpapP[1] = ppPIC[0]->surDRW();	// cam		pico
	gpapP[2] = ppPIC[1]->surDRW();	// penna	pici
	gpapP[3] = ppPIC[2]->surDRW();	// cpy		picii
	if( !gpapP[0] || !gpapP[1] || !ppPIC[2] )
	{
		aiQC[0] = aiQC[1]+1;
		return gpapP[0] ? (U1x4*)pSRF->pixels : NULL;
	}

	char* pDIR = pFILE+sprintf( pFILE, "%s/", pNAME );
	pDIR = strrchr( pFILE, '.' );
	if( pDIR )
		pDIR += sprintf( pDIR, "_dir/" );
	else
		pDIR = pFILE+gpmSTRLEN(pFILE);
	if( gpfACE(pPATH, 4) < 0 ) {
		std::cout << "mkdir:"<< pPATH <<std::endl;
		gpfMKDR( (char*)MANus.aPUB, pPATH );
	}

	gpcTRDspc SPC;
	U4 nDIR = pDIR-pPATH;

	// cam pico
	SPC.pSRC	= (U1x4*)gpapP[1]->pixels; // cam pico
	SPC.srcWH	= *gpapP[1];

	SPC.pSPC 	= (U1x4*)gpapP[0]->pixels;
	SPC.spcWH = *gpapP[0];
	if( SPC.spcWH.x != gpapP[0]->pitch/sizeof(*SPC.pSPC) )
	{
		std::cout	<< " spcWH.x:"	<< SPC.spcWH.x
					<< " pitch:"	<< (gpapP[0]->pitch/sizeof(*SPC.pSPC)) <<std::endl;
		SPC.spcWH.x = gpapP[0]->pitch/sizeof(*SPC.pSPC);
	}

	I4x2	of4x4 = 0, of2x2,
			trfSPC(1, SPC.spcWH.x ),
			trfSRC(1, SPC.srcWH.x );

	SPC.ALFid 	= TnID;
	SPC.ALFid.num = 0;

	SPC.pSspc 	=
	SPC.pFspc 	=
	SPC.pSPC 	= (U1x4*)gpapP[0]->pixels;
	SPC.pMAP	= ((U4x4*)(SPC.pSspc + 320*SPC.spcWH.x));

	SPC.pSspc 	+= I4x2(64,320+64)*trfSPC;
	SPC.pFspc 	+= I4x2(320,320)*trfSPC;
	SPC.pBspc 	= SPC.pFspc + I4x2(0,160)*trfSPC;

	I4x4 ofSFB( SPC.pSspc-SPC.pSPC,
				SPC.pFspc-SPC.pSPC,
				SPC.pBspc-SPC.pSPC );

	// penna pici
	SPC.pBpen	=
	SPC.pSpen 	=
	SPC.pFpen 	= (U1x4*)gpapP[2]->pixels;

	SPC.pSpen 	+= ofSFB.x;
	SPC.pFpen 	+= ofSFB.y;
	SPC.pBpen 	+= ofSFB.z;


	// cpy picii
	SPC.pBcpy	=
	SPC.pScpy	=
	SPC.pFcpy	= gpapP[3] ? (U1x4*)gpapP[3]->pixels : NULL;

	SPC.pScpy 	+= ofSFB.x;
	SPC.pFcpy 	+= ofSFB.y;
	SPC.pBcpy 	+= ofSFB.z;

	gpmMcpy( SPC.sPATH, pPATH, nDIR )[nDIR] = 0;
	SPC.pDIR = SPC.sPATH + nDIR;
	SPC.id = aiQC[0]%0x10; //trd = 0;

	of4x4 = I4x2(SPC.id%4,SPC.id/4);
	of2x2 = of4x4%2;

	SPC.ALFid.num += SPC.id<<16;
	SPC.pDB = MANus.LZY( SPC.ALFid );

	ofSFB.x = (of4x4&I4x2(0x40,0x20))*trfSRC;
	ofSFB.y = of4x4*trfSPC;
	ofSFB.z = of2x2*trfSPC + ofSFB.x;
	SPC.pSRC	+= ofSFB.x;

	SPC.pSspc += ofSFB.y;
	SPC.pFspc += ofSFB.z;
	SPC.pBspc += ofSFB.z;

	SPC.pSpen += ofSFB.y;
	SPC.pFpen += ofSFB.z;
	SPC.pBpen += ofSFB.z;

	SPC.pScpy += ofSFB.y;
	SPC.pFcpy += ofSFB.z;
	SPC.pBcpy += ofSFB.z;

	SPC.pSPC	+= ofSFB.z*2 - ofSFB.x;
	SPC.pMAP	+= SPC.id;

	SPC.DO();

	aiQC[0]++;
}
