#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

I4x4 gpaCAGEbillBALL[] = {
	{ mmX(1500), 0, mmX(320), mmX(800) },
	{ 0, 0, mmX(320), mmX(420) }, { 0, 0, mmX(-300), mmX(550) },
	{ mmX(685), mmX(-469), mmX(366),  mmX(300) },
};
I4x4 gpaCAGEbillBOX[] = {
	{ mmX(600), mmX(500), mmX(-250), mmX(330) }, // asztal_bill
	{ mmX(1500/2), mmX(5100), mmX(0), mmX(4000) }, // MIMI2bill
	{ mmX(-1000-2000), mmX(0), mmX(0), mmX(2000) }, // fal_bill
};
U4	gpnCAGEbillBALL = gpmN(gpaCAGEbillBALL),
	gpnCAGEbillBOX = gpmN(gpaCAGEbillBOX);


I4x4 gpaCAGEjohnBALL[] = {
	{ 0, 0, mmX(320), mmX(420) }, { 0, 0, mmX(-300), mmX(550) },
	{ mmX(1500), 0, mmX(320), mmX(800) },
	{ mmX(685), mmX(-469), mmX(366),  mmX(300) },
};
I4x4 gpaCAGEjohnBOX[] = {
	{ mmX(1500-600), mmX(-500), mmX(-250), mmX(330) }, // asztal_john
	{ mmX(-500-2000), mmX(0), mmX(0), mmX(2000) }, // MIMI_john
	{ mmX(1500/2), mmX(-5100), mmX(0), mmX(4000) }, // MIMI2john
};
U4	gpnCAGEjohnBALL = gpmN(gpaCAGEjohnBALL),
	gpnCAGEjohnBOX = gpmN(gpaCAGEjohnBOX);

I4x4* gpapBOX[] = {
	gpaCAGEbillBOX,
	gpaCAGEjohnBOX,
};
I4x4* gpapBALL[] = {
	gpaCAGEbillBALL,
	gpaCAGEjohnBALL,
};
U4 gpanBOX[] = {
	gpnCAGEbillBOX,
	gpnCAGEjohnBOX
};
U4 gpanBALL[] = {
	gpnCAGEbillBALL,
	gpnCAGEjohnBALL
};
I4x4 gpcDrc::chkXYZ( I4x4 trg, I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL )
{
	if( !this )
		return I4x2(mmX(500)).xxxy();

	if( iXYZ.qlen_xyz() < mmX(250) )
	if( okXYZ.qlen_xyz() > mmX(250) )
	{
		iXYZ.xyz_( okXYZ );
	}

	I4x4 tmp = cageBALL( trg.xyz0(), pBALL, nBALL );
	tmp = cageBOX( tmp, pBOX, nBOX );
	if( lim )
		return iXYZ.lim_xyz(tmp,lim);

	return tmp;
}
I4x4 gpcDrc::chkXYZ( I4 lim, U4 id )
{
	if( id > gpmN(gpanBALL) )
	{
		if( iXYZ.qlen_xyz() < mmX(250) )
		if( okXYZ.qlen_xyz() > mmX(250) )
		{
			iXYZ.xyz_( okXYZ );
		}
		//oXYZ.xyz_( iXYZ );
		return iXYZ;
	}

	return chkXYZ( tXYZ, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id] );
}
I4x4 gpcDrc::chkXYZ( I4x4 trg, I4 lim, U4 id )
{
	if( id > gpmN(gpanBALL) )
	{
		if( iXYZ.qlen_xyz() < mmX(250) )
		if( okXYZ.qlen_xyz() > mmX(250) )
		{
			iXYZ.xyz_( okXYZ );
		}
		return iXYZ;
	}

	return chkXYZ( trg, lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id] );
}

bool gpcDrc::jdPRGstp()
{
	// ha létre jött mozgá hagyja végre hajtani
	if( oCTRL.z )
		return false;

	// ha megált  megnézi
	// program végrehajtásával foglalkozik
	if( jdPRG.x )
	{
		// igen
		if( jdPRG.y >= jdPRG.z )
		{
			/// END --------------------------
			// de pont befejezte
			jdPRG.null();
			tXYZ.xyz_(jd0XYZ);
			return true;
		}
	} else
		return true;	// nem akor pihi

	// na nézzük a programot
	if( !jdPRG.y )
	{
		/// START ----------------------------
		jdPRG.z = (jdPRG.w = jd0PRG.x) * jd0PRG.y;
		if( jdPRG.z ? !okXYZ.abs0().mx().x : true )
		{
			jdPRG.null();
			return true;
		}
		jd0XYZ.xyz_(okXYZ);
		jd0ABC.xyz_(okABC);
		jd0xyz.xyz_(okxyz);
		jd0mx.ABC(jd0ABC,degX(180.0/PI));
	}

	I4 zl = sqrt((jd0XYZ-jd0xyz).qlen_xyz());

	I4x2 xy = jdPRG.y;
	xy.XdivRQ(jdPRG.w) += jdPRG.w;
	xy -= jd0PRG.a4x2[1];
	xy %= jdPRG.w;

	F4 cr;
	cr.gr2cr( xy, jdPRG.w );
	float d = cr.w/zl;
	I4x4 vec = ((jd0mx.x*(cr.x/d)) + (jd0mx.y*(cr.y/d)) + (jd0mx.z*(cr.z/d)));
	tXYZ.xyz_( jd0xyz - vec );

	jdPRG.y++;
	return true;
}

gpcLZY* gpcGT::GTdrcOS( gpcLZY* pANS, U1* pSTR, gpcMASS& mass, SOCKET sockUSR )
{
	U8 s = -1, nLEN;
	U4 n = gpmSTRLEN( pSTR );
	if( this ? !n : true )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcZSnD	*pZSnD = gpmLZYvali( gpcZSnD, mass.GTlzyALL.LZY( gpdGTlzyIDinp(TnID) ) );
	gpcLZY	*pLZYout = NULL,
			*pLZYusr = mass.GTlzyALL.LZY( gpdGTlzyIDusr(TnID) );
	///-----------------------------
	/// UJ felhasználó?
	///-----------------------------
		U4 iSOCK = 0, nSOCK = 0;
		if( SOCKET* pSOCK = gpmLZYvali( SOCKET, pLZYusr) )
		{
			nSOCK = gpmLZYload(pLZYusr,sockUSR);
			for( iSOCK = 0; iSOCK < nSOCK; iSOCK++ )
			{
				if( pSOCK[iSOCK] != sockUSR )
					continue;
				// nem új bent van a listában
				break;
			}
		}
		if( iSOCK >= nSOCK )
		{
			// új felhasználó!
			pLZYusr->lzyADD( &sockUSR, sizeof(sockUSR), s = -1 );
			iSOCK = nSOCK;
			nSOCK = gpmLZYload(pLZYusr,sockUSR);
		}
	///-----------------------------



	//pLZYout = mass.GTlzyALL.LZY(gpdGTlzyIDref(TnID));
	//if( !pLZYout )
	//	return pANS->lzyFRMT( s = -1, "nonsens" );


	if( !pZSnD )
		return pANS->lzyFRMT( s = -1, "nonsens" );

	gpcZSnD& ZSnD =	*pZSnD;
	U4	nD = 2,
		iD = nD, oD = 2;
	gpcDrc* pD = NULL;
	/*gpcDrc	*pDo = (gpcDrc*)gpmLZYvaliPD( U1, pLZYout, gpdZSio128 ),
			*pDi = (gpcDrc*)gpmLZYvaliPD( U1, pLZYinp, gpdZSio128 );*/
	U1 	sCOM[] = "ABCD",
		*pCOM, *pEND = pSTR+n, *pNUM;
	U4& comA = *(U4*)sCOM, iNUM, nNUM, iE = 2;
	I8x2 an;
	double d8;

	for( pSTR += gpmNINCS( pSTR, " \t\a\r\n;," ); *pSTR; pSTR += gpmNINCS( pSTR, " \t\a\r\n;," ) )
	{
		an.num = pEND-(pCOM = pSTR);
		an = pCOM;
		pSTR += an.num;
		if( an.alf ) {
			comA = *(U4*)pCOM;
			iNUM = 0;
			switch( an.alf )
			{
				case gpeALF_BILL:
						iD = 0;

						if( ZSnD.aDrc[iD].NMnDIF.au4x2[0].x == gpeZS_BILL )
							break;
						ZSnD.aDrc[iD].format( gpeZS_BILL );
						break;
				case gpeALF_JOHN:
						iD = 1;
						if( ZSnD.aDrc[iD].NMnDIF.au4x2[0].x == gpeZS_JOHN )
							break;
						ZSnD.aDrc[iD].format( gpeZS_JOHN );
					 break;


				case gpeALF_POS:
				case gpeALF_XYZ:{
						switch( comA&0xffffff )
						{
							case gpeZS_XYZ0:
							case gpeZS_POS0:
								iNUM = 0;
								break;
							default:
								iNUM = 12;
								break;
						}
						nNUM = 3;
					} break;
				case gpeALF_DIR:
				case gpeALF_ABC:{
						switch( comA&0xffffff )
						{
							case gpeZS_ABC0:
							case gpeZS_DIR0:
								iNUM = 3;
								break;
							default:
								iNUM = 15;
								break;
						}
						nNUM = 3;
					} break;

				case gpeALF_GRIP:{
						iNUM = 24;
						nNUM = 3;
					} break;

				case gpeALF_PRG: {
						iNUM = 24+3;
						nNUM = 4;
					} break;



				case gpeALF_FORMAT:
					ZSnD.format();
					continue;

				case gpeALF_HELO:
				case gpeALF_HELLO:
					return pANS->lzyFRMT( s = -1, "Hello! %d", iD );
				case gpeALF_HELP:
					return pANS->lzyFRMT( s = -1, "ReadMe.txt %d", iD );
				case gpeALF_LINE:
					return pANS->lzyFRMT( s = -1, "Line.txt %d", iD );
				case gpeALF_JOIN:
					return pANS->lzyFRMT( s = -1, "Join.txt %d", iD );

				/*case gpeALF_KOSZON:{
						U4	nR = sizeof(gpsSLMPabc),
							nC = nR/(4*8);
						pANS = pANS->lzyFRMT( s = -1, "CSOKOLOM %s BACSI!", pSTR );

					} return pANS;*/


				case gpeALF_LINK:
				case gpeALF_AXIS:{
						switch( comA )
						{
							case gpeZS_AXIS:
							case gpeZS_LINK:
								iNUM = 6;
								break;
							default:
								iNUM = 18;
								break;
						}
						nNUM = 6;
					} break;


				default:
					iNUM = 31;
					break;
			}

			if( oD != iD )
			if( pD )
			{
				//pD->chkXYZ( mmX(gpdROBlim), iD );
				pANS = pD->answSTAT( pANS, iD );
			}
			pD = NULL;
			oD = iD;
			continue;
		} else if( iNUM > 31 )
			return pANS->lzyFRMT( s = -1, "nonsens" );

		if( iD >= 2 )
		{
			pANS = pANS->lzyFRMT( s = -1, "Who?" );
			for( iD = 0; iD < 2; iD++ )
			{
				if( !ZSnD.aDrc[iD].NMnDIF.au4x2[0].x )
					continue;
				comA = ZSnD.aDrc[iD].NMnDIF.au4x2[0].x;
				pANS = pANS->lzyFRMT( s = -1, "\r\n%s", sCOM );
			}
			return pANS;
		}



		pNUM = pSTR;
		an.num = gpfSTR2I8( pNUM, &pSTR );
		if( *pSTR != '.' )
			d8 = 0.0;
		else {
			d8 = an.num;
			if( an.num < 0 )
				d8 -= strtod( (char*)pSTR, (char**)&pSTR );
			else
				d8 += strtod( (char*)pSTR, (char**)&pSTR );
		}

		pD = ZSnD.aDrc+iD;
		switch(iNUM)
        {

				// POS
			case 0:
			case 1:
			case 2:
				pD->tXYZ.aXYZW[(iNUM-0)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
				break;
				// pos
			case 12:
			case 13:
			case 14:
				pD->txyz.aXYZW[(iNUM-12)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
				break;


				// DIR
			case 3:
			case 4:
			case 5:
				pD->txyz.xyz_(0);
				pD->tABC.aXYZW[(iNUM-3)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;

				// GRIP
			case 24:
			case 25:
			case 26:
				pD->tGRP.aXYZW[(iNUM-24)%nNUM] = (d8 == 0.0) ? (I4)an.num*mmX(1) : (I4)(d8*mmX(1));
				break;
				// PRG
			case 27:
			case 28:
			case 29:
			case 30:
				pD->jd0PRG.aXYZW[(iNUM-27)%nNUM] = (d8 == 0.0) ? (I4)an.num : (I4)d8;
				if(iNUM!=30)
					break;

				pD->jdPRG = I4x4( 1, 0, 1 );
				pD->jdPRGstp();
				break;
			/// -----------------------------------------------


			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				// AXIS
				pD->aoAX1to6[((iNUM-6)%6)/3].aXYZW[(iNUM-6)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;

				// OFFSET - eltolás

				// DIR
			case 15:
			case 16:
			case 17:
				pD->oabc.aXYZW[(iNUM-15)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				// AXIS
				pD->aoax1to6[((iNUM-18)%6)/3].aXYZW[(iNUM-18)%nNUM] = (d8 == 0.0) ? (I4)an.num*degX(1) : (I4)(d8*degX(1));
				break;


			default:
				break;
        }
		iNUM++;
	}
	if( pD )
		return pD->answSTAT( pANS, iD );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}
