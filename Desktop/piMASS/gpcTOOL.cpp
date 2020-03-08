#include "gpcwin.h"
SDL_Surface* gpapP[0x10];

class gpcTRDbug
{
public:
	U1x4	*pQ; //,*pOUT;
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
			if( pBOB->nAREA <= 16 )
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
			if( l > 6 )
				break;
			LURD += I4x4(1,1,-1,-1);
			mn = (LURD.a4x2[1]-LURD.a4x2[0] ).mn();
			if( mn < 16 )
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

					if( pBOB->nAREA <= 16 )
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

U1x4* gpcPIC::TOOLexplode(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE )
{
	gpapP[0] = surDRW();			// TARGET
	gpapP[1] = ppPIC[0]->surDRW();	// dilet
	if( !gpapP[1] )
		return gpapP[0] ? (U1x4*)gpapP[0]->pixels : NULL;

	if( gpapP[0] )
	if( gpapP[0]->h != gpapP[1]->h*2 || gpapP[0]->pitch != gpapP[1]->pitch )
		gpmSDL_FreeSRF(gpapP[0]);

	if( !gpapP[0] )
	{
		txWH.a4x2[1] = ppPIC[0]->txWH.a4x2[1]&I4x2(1,2);
		gpapP[0] = pSRF = SDL_CreateRGBSurface( 0, txWH.z, txWH.w, 32, 0,0,0,0 );
		if( !pSRF )
			return NULL;
	}
	aiQC[0] = aiQC[1]+1;
	pREF = NULL;

	///--------------------
	//gpapP[2] = ppPIC[1]->surDRW();	// cam1


	I4	w = txWH.a4x2[1].x,		wQ = w>>1,
		h = txWH.a4x2[1].y>>1,	hQ = h>>1,
		wh = w*h, 				whQ = wh>>1,
		wph = w+h, wph2 = wph*2,
		dwn = wh-w,
		aDIR[] = { -w, +1, w, -1 };	// URDL

	U1x4* pI = (U1x4*)gpapP[1]->pixels;

	U1	*pQ = (U1*)gpapP[0]->pixels;
	gpmZnOF( pQ, wh*(1+4) ); // U1-től törlünk

	U4	*pM = (U4*)(pQ+wh);
	I4x2	*pR = (I4x2*)(pM+wh), *pRi = pR,
			vQ, ofQ(wQ, hQ), trfQ(1,w);

	U4 bugID = 1;
	bool bIN, bPREV;
	U1 rule, srt;
	for( I4 i = (w+1)*2, q, ix, e = wh-i; i < e; i++ )
	{
		vQ = I4x2( i%w, i/w );
		if( vQ.x < 3 || vQ.x > (w-3) )
			continue;

		srt = ((U4)pI[i].srt3()*0x100)>>3;
		if( !srt )
			continue;

		q = (
				((vQ&0x100000001)&ofQ)
				+(vQ/2)
			)*trfQ;

		pQ[q] = srt;

		ix = q%w;
		pQ[q-1] = pQ[q+1] =
		pQ[q-w] = pQ[q+w] = srt;
	}

	U4 mom = 0, nBtrd = 0;
	//nBOB = 0;

	gpcTRDbug		aBUG[4];
	U8 nB = 0, nA = 0;
	for( U4 i = 0, e = 4*2, q, t; i < e; i++ )
	{
		vQ = I4x2( i%2, i/2 );
		t = i%4;
        if( i/4 )
        {
			aBUG[t].trd.join();
			aBUG[t].n_join++;
			nB += aBUG[t].nBOB;
			for( U4 j = 0; j < aBUG[t].nBOB; j++ )
			{
				if(!aBUG[t].ppBOB[j])
					continue;
				nA += aBUG[t].ppBOB[j]->nAREA;

				//aBUG[vQ.x].nDONE = 0;

			}

        } else {
			q = (
				((vQ&0x100000001)&ofQ)
				+(vQ/2)
			)*trfQ;
			aBUG[t].pQ = (U1x4*)(pQ+q);
			aBUG[t].pM = pM+q;
			aBUG[t].pD = aDIR;
			aBUG[t].pR = pR + t*wh/16;
			aBUG[t].s = aDIR[2]+1;
			aBUG[t].m = 0;
			aBUG[t].wh = wh/2;
			aBUG[t].rght = aDIR[2]/2;
			aBUG[t].nDONE = 0;
			aBUG[t].n_run = 0;
		}

		if( aBUG[t].nDONE )
			continue;

		aBUG[t].trd = std::thread( TRDexp, aBUG+t );
		aBUG[t].n_run++;
	}

	return (U1x4*)gpapP[0]->pixels;

}
U1x4* gpcPIC::TOOLspace(	gpcLZYall& MANus, gpcPIC** ppPIC,
							char* pNAME, char *pPATH, char *pFILE ) {
	if( pSRF )
	if( pSRF->w*pSRF->h != 512*512 )
		gpmSDL_FreeSRF( pSRF );
	//cout << "TOOL:"<< pNAME << endl;
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
	//cout << "TRG"<< 0 << "-" << (gpapP[0] ? "ok" : "nok") << endl;
	//for( U4 i = 0; i < 3; i++ )
	//	cout << "PIC"<< i << "-" << (ppPIC[i] ? "ok" : "nok") << "-" << (gpapP[i+1] ? "ok" : "nok") << endl;


	if( !gpapP[0] || !gpapP[1] )
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
		ixB2 = boxB.y*trfB.y, ixA, ixB, ixT, ix05 = 0, sum;
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
		n1x4 = n1*sizeof(U1x4), nCMP = 0;
	//cout << "nMASK "<< n1 << endl;
	if( n1 )
	if( gpcLZY *pLZYin = MANus.LZY(ppPIC[1]->TnID) ) {
		U1x4	*pU1x4 = (U1x4*)pLZYin->p_alloc,
				*pDB = NULL;

		char* pDIR = pFILE+sprintf( pFILE, "%s/", pNAME );

		pDIR = strrchr( pFILE, '.' );
		if( pDIR )
			pDIR += sprintf( pDIR, "_dir/" );
		else
			pDIR = pFILE+gpmSTRLEN(pFILE);
		//cout << "DIR:"<< pPATH << endl;

		if( gpfACE(pPATH, 4) < 0 ) {
			cout << "mkdir:"<< pPATH << endl;
			gpfMKDR( (char*)MANus.aPUB, pPATH );
		} /*else {
			cout << " OK"<< endl;
		}*/

		if( !pLZYin->n_load ) {
			U8 s = 0;
			pLZYin->lzyADD( NULL, 0x10 + n1*0x100*sizeof(*pU1x4), s, 1 );
			pU1x4 = (U1x4*)pLZYin->p_alloc;
			for( U4 i = 1; i < 0x100; i++ )
			{

				(ppPIC[2]->TnID+I8x2(0,i))
				.an2str( (U1*)pDIR, (U1*)".png", true, true );
				cout << pPATH;

				if( gpfACE( pPATH, 4) > -1 )
				{
					pDB = pU1x4 + 4 + n1*i;
					gpapP[4] = IMG_Load( pPATH );
					if( gpapP[4] )
					if( gpapP[4]->pitch*gpapP[4]->h == n1x4 )
					{
						pDB->zyxw( gpapP[4]->pixels, n1 );
						pDB->u4 = 0x10000+0x100+0x10;
					}

					gpmSDL_FreeSRF(gpapP[4]);

					cout << " OK" << endl;
				} else
					cout << " GOOD" << endl;

			}
			gpmZnOF( pU1x4, 0x10 );
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
			cout << i << ":" << (int)pDB->x << "/" << (int)pDB->y << ":" << nCMP << "/" << n1-1 << endl;
			if( nCMP < n1-1 )
			{
				pDB->x = 0; // jelzi, hogy változás van
			}
			else if( pDB->x <= pDB->y )
			{

				pDB->x++;
				if( pDB->x >= pDB->y )
				{
					pDB->y = pDB->x+1;
					IMG_SavePNG( gpapP[3], "/mnt/ram/space.png" );

					(ppPIC[2]->TnID + I8x2(0,pU1x4->x))
					.an2str( (U1*)pDIR, (U1*)".png", true, true );
					cout << "save "<< pPATH << endl;


					if( gpfACE( pPATH, 4) > -1 )
					{
						cout << "kill "<< pPATH << endl;
						rename( pPATH, "/mnt/ram/space.kill" );
					}
					cout << "rename /mnt/ram/space.png "<< pPATH << endl;
					if( int err = rename( "/mnt/ram/space.png", pPATH ) )
					{
						gpcLZY load;
						U8 s;
						load.lzyRD( "/mnt/ram/space.png", s = 0, 1 );
						load.lzyWR( pPATH, true );

						cout << " ERR " << err << endl;

					} else
						cout << " OK" << endl;

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
				gpmMcpyOF( pDB, gpapP[3]->pixels, n1 );
			}


			pU1x4->x = ix05;

			pDB = pU1x4 + 4 + n1*(U4)pU1x4->x;
			pDB->x = 0;

			if( pDB->u4&0xf0000 )
				gpmMcpyOF( (U1x4*)gpapP[3]->pixels, pDB, n1 );
			else {
                U1x4* pPRE = NULL, *pNEX = NULL;
				for( U4 s = pU1x4->x; s; --s )
				{
					pPRE = pU1x4 + 4 + n1*s;
					if( pPRE->u4&0xf0000 )
						break;
					pPRE = NULL;
				}
				if( pPRE )
				{
					for( U4 s = pU1x4->x; s; ++s )
					{
						pNEX = pU1x4 + 4 + n1*s;
						if( pNEX->u4&0xf0000 )
							break;
						pNEX = NULL;
					}
					if( pNEX )
					{
						gpmMcpyOF( pDB, pPRE, n1 );
						pDB->AND( pNEX, n1 );
						pDB->u4 = 0x10000;
						pDB->x = 0;
						pDB->y = 1;
					}
				}

			}
			ppPIC[2]->pREF = NULL;
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
