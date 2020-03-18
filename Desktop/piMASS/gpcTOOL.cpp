#include "gpcwin.h"
SDL_Surface* gpapP[0x10];

class gpcTRDbug
{
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
	~gpcTRDbug() {
		for( U4 i = 0; i < nBOB; i++ )
			gpmDEL(ppBOB[i]);
		if( n_join < n_run )
			trd.join();
		gpmDELary(ppBOB);
	}
	gpcTRDbug(){ gpmCLR; };
	gpcBOB* pBOB, *pMOM;
	void loop()
	{
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
				srt = ((((U4)s3.x*(U4)(s3.x>>4))>>8) & ~0x7)|s3.w;
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

void TRDexp( gpcTRDbug* pT )
{
	return pT->loop();
}
#define gpdEXPdbgCOUT if(false)
U1x4* gpcPIC::TOOLexplode(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE )
{
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


	gpdEXPdbgCOUT std::cout << "Explode: " << any <<std::endl;


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
	gpdEXPdbgCOUT std::cout << "Explode: nTRD " << trd <<std::endl;
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


			//gpdEXPdbgCOUT
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
				char* pPATH, char* pDIR, char* pEXP )
{
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
U1x4* gpcPIC::TOOLspace(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE ) {
	if( pSRF )
	if( pSRF->w*pSRF->h != 512*512 )
		gpmSDL_FreeSRF( pSRF );
	//std::cout << "TOOL:"<< pNAME <<std::endl;
	if( !pSRF ) {
		txWH.z = txWH.w = 512;
		pSRF = SDL_CreateRGBSurface( 0, txWH.z, txWH.w, 32, 0,0,0,0 );
	}
	gpmZnOF( (U1x4*)pSRF->pixels, txWH.a4x2[1].area() );
	aiQC[0] = aiQC[1]+1;
	pREF = NULL;
	gpmZ(gpapP);
	gpapP[0] = pSRF;				// TARGET
	gpapP[1] = ppPIC[0]->surDRW();	// CAM
	gpapP[2] = ppPIC[1]->surDRW();	// MASK
	gpapP[3] = ppPIC[2]->surDRW();	// MASKi // ezt cserélgetjük

	if( !gpapP[0] || !gpapP[1] || !ppPIC[2] )
		return (U1x4*)pSRF->pixels;


	I4x2	trfT( 1, gpapP[0]->w ),
			trfA( 1, gpapP[1]->w ),
			trfB( 1, gpapP[2]->w ),
			boxA( gpapP[1]->w, gpapP[1]->h ),
			boxB((trfB.y*14)/30,(gpapP[2]->h*7)/31),
			xyOFF,
			xyB((trfB.y*16)/30,(gpapP[2]->h*15)/31), RG,BG,BR;
	U1x4 	aT[4], *pT = (U1x4*)gpapP[0]->pixels,
			a,
			b, *pB = (U1x4*)gpapP[2]->pixels;
	U1		*pA = (U1*)gpapP[1]->pixels;

	U4x4 aHISTI[0x100], aqu;
	gpmZ(aHISTI);
	U4	i, e = boxB.area(),
		ixB2 = boxB.y*trfB.y, ixA, ixB, ixT,
		sum;
	int	ix05 = 0;	//ix0.5
	for( i = 0; i < e; i++ ) {
		xyOFF = I4x2(i%boxB.x,i/boxB.x);
		ixA = ((xyOFF&trfA)/trfB)*trfA;
		a.u4 = *(U4*)(pA+ixA*3);
		aHISTI[a.x].x++;
		aHISTI[a.y].y++;
		aHISTI[a.z].z++;
	}
	for( i = 0, aqu = 0; i < 0x100; i++ ) {
		aqu.x += aHISTI[i].x;
		aqu.y += aHISTI[i].y;
		aqu.z += aHISTI[i].z;

		sum = (aqu.x+aqu.y+aqu.z);
		if( ix05 )
			continue;

		if( (sum*2)/3 < e )
			continue;

		ix05 = i;
		break;
	}

	U4	n1 = gpapP[2] ? ppPIC[1]->txWH.a4x2[1].area() : 0,
		nCMP = 0;


	if( n1 )
	if( gpcLZY *pLZYin = MANus.LZY(ppPIC[1]->TnID) ) {
		U1x4	*pU1x4 = (U1x4*)pLZYin->p_alloc,
				*pDB = NULL;
		gpdSPCdbgCOUT std::cout << "Space: " << ix05 <<std::endl;
		char* pDIR = pFILE+sprintf( pFILE, "%s/", pNAME );

		pDIR = strrchr( pFILE, '.' );
		if( pDIR )
			pDIR += sprintf( pDIR, "_dir/" );
		else
			pDIR = pFILE+gpmSTRLEN(pFILE);
		gpdSPCdbgCOUT std::cout << "DIR:"<< pPATH <<std::endl;

		if( gpfACE(pPATH, 4) < 0 ) {
			std::cout << "mkdir:"<< pPATH <<std::endl;
			gpfMKDR( (char*)MANus.aPUB, pPATH );
		}

		if( !pLZYin->n_load ) {
			gpdSPCdbgCOUT std::cout << "Space - RESET pU1x4 " <<std::endl;
			U8 s = 0;
			pLZYin->lzyADD( NULL, 0x10 + n1*sizeof(*pU1x4) * 0x100, s, 1 );
			gpdSPCdbgCOUT std::cout << "Space - gpmZnOF:" << pLZYin->n_load << "n1*sof:" << n1*sizeof(*pU1x4) << "n1:" << n1 <<std::endl;
			gpmZnOF( pLZYin->p_alloc, 0x10 );
			pU1x4 = (U1x4*)pLZYin->p_alloc;
			U4 j = 0;
			gpdSPCdbgCOUT std::cout << "SPACE_DIR: " << pPATH <<std::endl;
			for( U4 i = 1; i < 0x100; i++ )
			{

				(ppPIC[2]->TnID+I8x2(0,i))
				.an2str( (U1*)pDIR, (U1*)".png", true, true );
				std::cout << "DIR: " << pPATH; // <<std::endl;

				if( gpfACE( pPATH, 4) > -1 )
				{
					pDB = pU1x4 + 4 + n1*i;
					//gpmZnOF( pDB, n1 );
					pDB->u4 = 0x1000000;
					j++;
					std::cout << " FND" <<std::endl;
				} else
					std::cout << " Nothing!" <<std::endl;

			}
			std::cout << " nFND: " << j <<std::endl;

		} else {
			gpdSPCdbgCOUT std::cout << "Space - new TURN " <<std::endl;
		}

		pDB = pU1x4 + 4 + n1*(U4)pU1x4->x;
		if( !gpapP[3] )
		{
			ppPIC[2]->txWH.a4x2[1] = ppPIC[1]->txWH.a4x2[1];
			ppPIC[2]->pSRF = gpapP[3] = SDL_CreateRGBSurface( 0, gpapP[2]->w, gpapP[2]->h, 32, 0,0,0,0 );
			gpmMcpyOF( (U1x4*)gpapP[3]->pixels, pDB, n1 );
			ppPIC[2]->pREF = NULL;
			return (U1x4*)pSRF->pixels;
		}

		if(pU1x4->x==ix05) {
			nCMP = gpmMcmpOF( (pDB+1), (((U1x4*)(gpapP[3]->pixels))+1), n1-1 );
			std::cout << i << ":" << (int)pDB->x << "/" << (int)pDB->y << ":" << (int)nCMP << "/" << (int)n1-1 <<std::endl;
			if( nCMP < n1-1 ) {
				pDB->x = 0; // jelzi, hogy változás van
			}
			else if( pDB->x <= pDB->y ) {
				pDB->x++;
				if( pDB->x >= pDB->y )
				{
					/// mentés -----------------------------
					pDB->y = pDB->x+1;
					IMG_SavePNG( gpapP[3], "/mnt/ram/space.png" );

					(ppPIC[2]->TnID+I8x2(0,pU1x4->x))
					.an2str( (U1*)pDIR, (U1*)".png", true, true );
					std::cout << "save "<< pPATH <<std::endl;


					if( gpfACE( pPATH, 4) > -1 )
					{
						std::cout << "kill "<< pPATH <<std::endl;
						rename( pPATH, "/mnt/ram/space.kill" );
					}
					std::cout << "rename /mnt/ram/space.png "<< pPATH <<std::endl;
					if( int err = rename( "/mnt/ram/space.png", pPATH ) )
					{
						gpcLZY load;
						U8 s;
						load.lzyRD( "/mnt/ram/space.png", s = 0, 1 );
						load.lzyWR( pPATH, true );

						std::cout << " ERR " << err <<std::endl;

					} else
						std::cout << " OK" <<std::endl;

					if( gpfACE("/mnt/ram/space.kill", 4) > -1 )
					{
						remove( "/mnt/ram/space.kill" );
					}

				}
		   }
		}
		else {
			// nincsen még ilyenünk
			if( pU1x4->x ) {
				// elrakjuk ami korábban használtunk
				/// ELÖZŐ
				U4 db = pDB->u4;
				gpmMcpyOF( pDB, gpapP[3]->pixels, n1 );
				pDB->u4 = db;
				pU1x4->x = ix05;

				/// betöltés -----------------------------
				pDB = ppPIC[2]->food( pU1x4, ix05, n1, pPATH, pDIR );
			} else {
				pU1x4->x = ix05;

				pDB = pU1x4 + 4 + n1*ix05;
			}
			pDB->x = 0;	// !nCMP

			if( pDB->z&0xf ) {// z alive?
				gpmMcpyOF( (U1x4*)gpapP[3]->pixels, pDB, n1 );
				ppPIC[2]->pREF = NULL;
			} else {
                U1x4* pPRE = NULL, *pNEX = NULL;
				for( U4 s = ix05; s; --s )
				{
					pPRE = ppPIC[2]->food( pU1x4, s, n1, pPATH, pDIR );
					if( pPRE ? !(pPRE->z&0xf) : true )
						continue;

					break;
				}
				if( pPRE )
				for( U4 s = ix05; s; ++s )
				{
					pNEX = ppPIC[2]->food( pU1x4, s, n1, pPATH, pDIR );
					if( pNEX ? !(pNEX->z&0xf) : true )
						continue;

					gpmMcpyOF( pDB, pPRE, n1 );
					pDB->AND( pNEX, n1 );
					break;
				}
				pDB->x = 0;
				pDB->y = 1;
				pDB->z = 1;

				gpmMcpyOF( (U1x4*)gpapP[3]->pixels, pDB, n1 );
				ppPIC[2]->pREF = NULL;
			}

		}
	}

	ixT = ix05*gpapP[0]->w + 0x100;
	for( i = 0, aqu = 0; i < 0x100; i++ ) {
		aqu.x += aHISTI[i].x;
		aqu.y += aHISTI[i].y;
		aqu.z += aHISTI[i].z;

		pT[i+ixT].z = (aqu.x*255/e);
		pT[i+ixT].y = (aqu.y*255/e);
		pT[i+ixT].x = (aqu.z*255/e);
	}
	for( i = 0; i < e; i++ ) {
		xyOFF = I4x2(i%boxB.x,i/boxB.x);
		ixB = (xyB+xyOFF)*trfB;
		b = pB[ixB];
		if( !b.mx_xyz() )
			continue;

		a = pB[ixB+ixB2];
		RG = I4x2(  a.z, a.y )+255; // RG
		BG = I4x2( -a.x, a.y )+255; // BG
		BR = I4x2( -a.x,-a.z )+255; // BR
		ixT = RG*trfT;
		aT[0] = pT[ixT];
		pT[ixT].u4 |= b.u4;

		ixT = BG*trfT;
		aT[1] = pT[ixT];
		pT[ixT].u4 |= b.u4;

		ixT = BR*trfT;
		aT[2] = pT[ixT];
		pT[ixT].u4 |= b.u4;
	}

	return (U1x4*)pSRF->pixels;
}
