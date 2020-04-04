#include "gpcgt.h"
#include "gpcwin.h"
extern U1 gpaALFadd[];
extern char gpsTAB[], *gppTAB;

I4x4 gpaCAGEbillBALL[] = {
	{ mm100(1500), 0, mm100(320), mm100(800) },
	{ 0, 0, mm100(320), mm100(420) }, { 0, 0, mm100(-300), mm100(550) },
	{ mm100(685), mm100(-469), mm100(366),  mm100(300) },
};
I4x4 gpaCAGEbillBOX[] = {
	{ mm100(1500/2), mm100(4900), mm100(0), mm100(4000) }, // MIMI2bill
	{ mm100(-1000-2000), mm100(0), mm100(0), mm100(2000) }, // fal_bill
	{ mm100(600), mm100(600), mm100(-200), mm100(330) }, // asztal_bill
};
U4	gpnCAGEbillBALL = gpmN(gpaCAGEbillBALL),
	gpnCAGEbillBOX = gpmN(gpaCAGEbillBOX);


I4x4 gpaCAGEjohnBALL[] = {
	{ 0, 0, mm100(320), mm100(420) }, { 0, 0, mm100(-300), mm100(550) },
	{ mm100(1500), 0, mm100(320), mm100(800) },
	{ mm100(685), mm100(-469), mm100(366),  mm100(300) },
};
I4x4 gpaCAGEjohnBOX[] = {
	{ mm100(1500-600), mm100(-600), mm100(-200), mm100(330) }, // asztal_john
	{ mm100(-500-2000), mm100(0), mm100(0), mm100(2000) }, // MIMI_john
	{ mm100(1500/2), mm100(-4900), mm100(0), mm100(4000) }, // MIMI2john
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
gpcDrc* gpcDrc::chk( I4 lim, I4x4* pBOX, U4 nBOX, I4x4* pBALL, U4 nBALL )
{
	if( !this )
		return NULL;
	if( okXYZ.qlen_xyz() > mm100(400) )
	{
		iXYZ.xyz_( okXYZ );
	}

	I4x4 tmp;
	tmp = cageBALL( tXYZ.xyz0(), pBALL, nBALL );
	tmp = cageBOX( tmp, pBOX, nBOX );
	if( lim )
		oXYZ.xyz_( iXYZ.lim_xyz(tmp,lim) );
	else
		oXYZ.xyz_( tmp );


	return this;
}
gpcDrc* gpcDrc::chk( I4 lim, U4 id )
{
	if( id > gpmN(gpanBALL) )
	{
		if( okXYZ.qlen_xyz() > mm100(400) )
		{
			iXYZ.xyz_( okXYZ );
		}
		oXYZ.xyz_( iXYZ );
		return this;
	}
	return chk( lim, gpapBOX[id], gpanBOX[id], gpapBALL[id], gpanBALL[id] );
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
		if( an.alf )
		{
			comA = *(U4*)pCOM;
			iNUM = 0;
			switch( an.alf )
			{
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
				case gpeALF_GRIP:{
						iNUM = 24;
						nNUM = 3;
					} break;

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
				default:
					iNUM = 23;
					break;
			}

			if( oD != iD )
				pANS = pD->chk( 0, iD
								/*iD? gpaCAGEbillBOX : gpaCAGEjohnBOX,
								iD? gpnCAGEbillBOX : gpnCAGEjohnBOX,
								iD? gpaCAGEbillBALL : gpaCAGEjohnBALL,
								iD? gpnCAGEbillBALL : gpnCAGEjohnBALL*/
							)->answSTAT( pANS );
			pD = NULL;
			oD = iD;
			continue;
		} else if( iNUM > 23 )
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
				pD->tXYZ.aXYZW[(iNUM-0)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			// pos
			case 12:
			case 13:
			case 14:
				pD->txyz.aXYZW[(iNUM-12)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
				// GRIP
			case 24:
			case 25:
			case 26:
				pD->tGRP.aXYZW[(iNUM-24)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;


				// DIR
			case 3:
			case 4:
			case 5:
				pD->tABC.aXYZW[(iNUM-3)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				// AXIS
				pD->aoAX1to6[((iNUM-6)%6)/3].aXYZW[(iNUM-6)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;

			// OFFSET - eltolás

				// DIR
			case 15:
			case 16:
			case 17:
				pD->oabc.aXYZW[(iNUM-15)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				// AXIS
				pD->aoax1to6[((iNUM-18)%6)/3].aXYZW[(iNUM-18)%nNUM] = (d8 == 0.0) ? (I4)an.num*100 : (I4)(d8*100.0);
				break;


			default:
				break;
        }
		iNUM++;
	}
	if( pD )
		return pD->chk( 0, iD
						/*iD? gpaCAGEbillBOX : gpaCAGEjohnBOX,
						iD? gpnCAGEbillBOX : gpnCAGEjohnBOX,
						iD? gpaCAGEbillBALL : gpaCAGEjohnBALL,
						iD? gpnCAGEbillBALL : gpnCAGEjohnBALL*/
					)->answSTAT( pANS );

	return pANS->lzyFRMT( s = -1, "nonsens" );
}
