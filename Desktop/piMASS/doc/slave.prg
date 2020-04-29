	 Servo Off
  ' Fontos paraméterek beszerzése
  ' Első a tengely limitek, hogy ne hazárdirozás legyen mikor nyekk!
  ' és TB-n lehessen szabályozni
  PrmRead 0, "MEJAR",CMEJAR$
  Dim MNMX(7,4)
  M3=1
  M4=1
  M5=Len(CMEJAR$)
  For M1=1 To 7 Step 1
    For M2=1 To 2 Step 1
      C1$=Mid$(CMEJAR$,M3,M5-M3)
      M4=Strpos(C1$,",")
      MNMX(M1,M2)=Val(C1$)
      M3=M4+M3
    Next M2
    ' nem tetszett neki hogy elöbb a 4-est számoltam ki
    ' ha lenne += operátor érteném de így gáz
    MNMX(M1,3)=MNMX(M1,1)
    MNMX(M1,4)=(MNMX(M1,2)-MNMX(M1,1))/2.0
    MNMX(M1,3)=MNMX(M1,3)+MNMX(M1,4)
    'User defined screen mitt lett fokban, gondolom nem hal bele a rad() ba kalkulációnál
  Next M1
    
    'Globális változók hívása
    M_00#=0
   'Helyi változók felsorolás-------------------
   Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
   Def Io ioHS1=Bit,10160
   Def Io ioHS2=Bit,10161
   Def Io ioCOM=DWord,10192
   Def Io ioPIX=DWord,10224
   Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
   Def Io oMx=DWord,10256
   Def Io oMy=DWord,10288
   Def Io oMz=DWord,10320
   Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
   Def Io oMa=DWord,10352
   Def Io oMb=DWord,10384
   Def Io oMc=DWord,10416
   Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
   Def Io oMs12=DWord,10448
   Def Io oMs32=DWord,10480
   Def Io oMsR2D=DWord,10512
   oMs12=0
   Rem -  -- ---=--==-=== MAJRE BUTTON ===-==--=--- --  -
   Def Io ioMajre=Bit,11313
   MSebesseg=100 'Robot sebesség EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 100
   MutazasiMagassag=740 'Utazási magasság a fejnek EZT NEM SZABAD MÁSHOL VÁLTOZTATNI
   Mgyors=80 'Robot gyorsulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
   Mlass=80 'Robot lassulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
   MSebPakolas=100 'Felvételi, lerakási sebesség!!!
   MGyorsPakolas=100 'Felvételi, lerakási gyorsulás!!!
   MLassPakolas=100 'Felvételi, lerakási lassulás!!!
   Mi1=0 'Minden For ciklusnak
   Mi2=0 'Lefoglalva a Getparam fv-nek
   Dim MIp(4) 'bemenetek helyi változója
   Dim MQp(4) 'kimenetek helyi változója
   '
   Dim pKamera(4) 'Kamera pozíció a 4 típusra
   '
   'Tool (100,100,100,100)
   Mdarab=0 'Megfogó darab érzékelés tárolása
   MHibaszam=0 'Hibaszám tárolása
   MpozIndex=0 'Pozíció index tárolása
   MPosX=0 'X pozíció átvétele
   MPosY=0 'Y pozíció átvétele
   MPosZ=0 'Z pozíció átvétele
   MPosA=0 'A pozíció fok átvétele
   MPosB=0 'B pozíció fok átvétele
   MPosC=0 'C pozíció fok átvétele
   MPosXe=0 'X pozíció eltolás átvétele
   MPosYe=0 'Y pozíció eltolás átvétele
   MPosZe=0 'Z pozíció eltolás átvétele
   MPosAe=0 'A pozíció fok eltolás átvétele
   MPosBe=0 'B pozíció fok eltolás átvétele
   MPosCe=0 'C pozíció fok eltolás átvétele
   MPosJ1=0 'J1 tengely fok átvétele
   MPosJ2=0 'J2 tengely fok átvétele
   MPosJ3=0 'J3 tengely fok átvétele
   MPosJ4=0 'J4 tengely fok átvétele
   MPosJ5=0 'J5 tengely fok átvétele
   MPosJ6=0 'J6 tengely fok átvétele
   MPosJ1e=0 'J1 tengely fok eltolás átvétele
   MPosJ2e=0 'J2 tengely fok eltolás átvétele
   MPosJ3e=0 'J3 tengely fok eltolás átvétele
   MPosJ4e=0 'J4 tengely fok eltolás átvétele
   MPosJ5e=0 'J5 tengely fok eltolás átvétele
   MPosJ6e=0 'J6 tengely fok eltolás átvétele
   'MHibaszám jelentései:
   '900 - Rossz parancs küldése a robotnak
   '910 - Rossz mozgás pozíció index paraméter (MpozIndex)
   '920 - Servo OFF
   '930 - Range problem *S10
   '931 - Range problem *S11
   '999 - Alaphelyzetfelvételi hiba
   '
   MFel=0 'Ez a változó, ha nulla, akkor bármilyen parancs jöhet
   MELOZOTERULET=0
   MTERULET=0
   'MTERULET jelentései (milyen pozícióba képes menni):
   '10 - alaphelyzet pozíció jog
   '20 - szerviz pozício jog
   '30 - amegfogó ürítése jog
   '
   mXtemp=0 'alaphelyzet pozíció kereséshez
   mYtemp=0 'alaphelyzet pozíció kereséshez
   mZtemp=0 'alaphelyzet pozíció kereséshez
   'ptemp0=0 pozíció segéd alaphelyzethez
   'ptemp1=0 'pozíció segéd folyamathoz
   'jtemp1=0 'pozíció segéd forgatáshoz
   MTavol=0 'alaphelyzetbe értünk-e már?
   '===================================================
   '===================================================
   '---------------------------------------------------
   'Helyi változók felsorolás vége------------------
   '---------------------------------------------------
   '
   'Hiba esetén megszakítás generálódik
  '
  While (M_Err=0)
    Select ioCOM 'Mozgás végrehajtás parancs
      Case 0 'nope
        Mms = M_Timer(1)
        Ms=Mms/1000.0
        oMs12 = Mms
        Break
     Case 110 'XYZ ABC töbnyire :) lineáris
        GoSub *S110
        Break
    End Select
    Dly 0.001
  WEnd
  End
  '
  '
  '=================================================================
  '---------------------------------
  'Szubrutinok következnek-----
  '---------------------------------
  '=================================================================
  *S110 '=======================
    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
    If (MFel<>0) Then 'MFel?
      MHibaszam=900
    Else  'MFel?
      ptemp2=P_Curr
      ptemp2.X=(MPosX/100)
      ptemp2.Y=(MPosY/100)
      ptemp2.Z=(MPosZ/100)
      If Dist(P_Zero,ptemp2) < 300 Then 'Zero detect!!!
        MHibaszam=910 'Zero detect!!!
      Else 'Zero detect!!!
        ptemp2.A=Rad(MPosA/100)
        ptemp2.B=Rad(MPosB/100)
        ptemp2.C=Rad(MPosC/100)
        'ABC Posture különbség
        M2=Abs(Cos(P_Curr.A)-Cos(ptemp2.A))
          If M2<Abs(Cos(P_Curr.B)-Cos(ptemp2.B)) Then M2=Abs(Cos(P_Curr.B)-Cos(ptemp2.B))
          If M2<Abs(Cos(P_Curr.C)-Cos(ptemp2.C)) Then M2=Abs(Cos(P_Curr.C)-Cos(ptemp2.C))
        'XYZ Távolság
        M1 = Dist(P_Curr,ptemp2)
        MHibaszam=0
        If (M1+M2)=0 Then  ' MOVE? TURN?
          MHibaszam=0
        Else ' MOVE? TURN?
          If M_Svo=0 Then
            If M_Run(3)=1 Then
              Servo On 'szervo
            EndIf ' R2D2.prg
          EndIf ' M_Servo?
          Fsc Off
          M_Tool=0
          Ovrd MSebesseg 'Sebesség
          Accel Mgyors,Mlass 'Gyorsulás
          MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
          MELOZOTERULET=MTERULET
          Cnt 0
          ' CAGE check -----------------------------------------------
          M2=Abs(ptemp2.A)
            If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
            If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
          M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
          If (M1 > M3) Or (M2 > (M_PI*2.0)) Then ' CAGE BOX
            MHibaszam=911
          Else ' CAGE BOX
            jtemp2 = PtoJ(ptemp2)
            'old If (Abs(jtemp2.J4)<Rad(90)) And Abs(jtemp2.J5)<Rad(75.0)  Then ' Soft J4
            M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
            M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
            'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
            'egyszerű eltérést számol a középtől 
            If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Soft J4
              Mvs ptemp2
              M_Out(11313)=1
              M_Out(11314)=0
              Cnt 0
            Else ' Soft J4
              M_Out(11314)=1
              M_Out(11313)=0
              ' Hard J4
              jtemp0=J_Curr
              ' FLIP? 4 ax
              If M4>MNMX(4,4) Then
                If Deg(jtemp2.J4)>MNMX(4,3) Then
                  jtemp2.J4=jtemp2.J4-M_PI
                Else
                  jtemp2.J4=jtemp2.J4+M_PI
                EndIf
                ' FLIP 5 ax
                jtemp2.J5=jtemp2.J5*-1.0
                M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet így mar jó?
                ' FLIP 6 ax
                jtemp2.J6=jtemp2.J6*-1.0
              EndIf
              If M5>MNMX(5,4) Then
                MHibaszam=9115
                If Deg(jtemp2.J5)>MNMX(5,3) Then
                  jtemp2.J5=M_PI*((MNMX(5,2)-1.0)/180.0) 'maximum
                Else
                  jtemp2.J5=M_PI*((MNMX(5,1)+1.0)/180.0) 'minimum
                EndIf
              EndIf
              ' MIDLE jtemp1
              jtemp1.J1 = (jtemp2.J1-J_Curr.J1)*0.66 + J_Curr.J1
              jtemp1.J2 = (jtemp2.J2-J_Curr.J2)*0.66 + J_Curr.J2
              jtemp1.J3 = (jtemp2.J3-J_Curr.J3)*0.66 + J_Curr.J3
              jtemp1.J4 = (jtemp2.J4-J_Curr.J4)*0.66 + J_Curr.J4
              jtemp1.J5 = (jtemp2.J5-J_Curr.J5)*0.66 + J_Curr.J5
              jtemp1.J6 = (jtemp2.J6-J_Curr.J6)*0.66 + J_Curr.J6
              Mov jtemp1
              'GoSub *FEEDBACK
              Cnt 1
              'Mov jtemp2
              '
              '
            EndIf ' Soft J4
          EndIf ' CAGE BOX?
        EndIf ' MOVE? TURN?
      EndIf  'Zero detect?
    EndIf 'MFel?
    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
  Return '=======================
  '=================================================================
  '=================================================================
  '*****************************************************************
  '=================================================================
  '=================================================================
  '
  *FEEDBACK '=======================
    Mms = M_Timer(1)
    Ms=Mms/1000.0
    oMs12 = Mms
    oMx=P_Curr.X*100
    oMy=P_Curr.Y*100
    oMz=P_Curr.Z*100
    oMa=Deg(P_Curr.A)*100
    oMb=Deg(P_Curr.B)*100
    oMc=Deg(P_Curr.C)*100
  Return '=======================
  '
  *HANDSHAKE1 '=======================
    Wait ioHS1=1 'wait RobotStartIn=1
    MpozIndex=ioPIX 'pozíció index átvétele
    MPosX=oMx 'X pozíció átvétele
    MPosY=oMy 'Y pozíció átvétele
    MPosZ=oMz 'Z pozíció átvétele
    MPosA=oMa 'A pozíció fok átvétele
    MPosB=oMb 'B pozíció fok átvétele
    MPosC=oMc 'C pozíció fok átvétele
    ' eddig tuti ---------------------------
    MPosXe=M_In32(10448) 'X pozíció eltolás átvétele
    MPosYe=M_In32(10480) 'Y pozíció eltolás átvétele
    MPosZe=M_In32(10512) 'Z pozíció eltolás átvétele
    MPosAe=M_In32(10544) 'A pozíció fok eltolás átvétele
    MPosBe=M_In32(10576) 'B pozíció fok eltolás átvétele
    MPosCe=M_In32(10608) 'C pozíció fok eltolás átvétele
    MPosJ1=M_In32(10640) 'J1 tengely fok átvétele
    MPosJ2=M_In32(10672) 'J2 tengely fok átvétele
    MPosJ3=M_In32(10704) 'J3 tengely fok átvétele
    MPosJ4=M_In32(10736) 'J4 tengely fok átvétele
    MPosJ5=M_In32(10768) 'J5 tengely fok átvétele
    MPosJ6=M_In32(10800) 'J6 tengely fok átvétele
    MPosJ1e=M_In32(10832) 'J1 tengely fok eltolás átvétele
    MPosJ2e=M_In32(10864) 'J2 tengely fok eltolás átvétele
    MPosJ3e=M_In32(10896) 'J3 tengely fok eltolás átvétele
    MPosJ4e=M_In32(10928) 'J4 tengely fok eltolás átvétele
    MPosJ5e=M_In32(10960) 'J5 tengely fok eltolás átvétele
    MPosJ6e=M_In32(10992) 'J6 tengely fok eltolás átvétele
    ioHS1=1 'ConfirmOut=1
    Wait ioHS1=0 'wait RobotStartIn=0
    Wait ioCOM=0 'Mozgás parancs nullázására vár
    Wait ioPIX=0 'pozíció index kinullázására vár
    ioHS1=0 'ConfirmOut=0
    M_00#=1 ' jelzes R2D2-nak hogy elkezdödött valami
  Return '=======================
  '
  *HANDSHAKE2 '=======================
    If M_Svo(1)=0 Then
      ' Így nem nyírja ki miért lett leállítva
      If MHibaszam<1 Then
        MHibaszam=920
      EndIf
    EndIf
    ' Így az R2D2-nál is frissebb adatok lesznek mire olvasnám
    GoSub *FEEDBACK
    'ptemp1=P_Curr
    'oMx=ptemp1.X*100
    'oMy=ptemp1.Y*100
    'oMz=ptemp1.Z*100
    'oMa=Deg(ptemp1.A)*100
    'oMb=Deg(ptemp1.B)*100
    'oMc=Deg(ptemp1.C)*100
   '
   ioCOM=MHibaszam 'Hiba státusz küldése
   ioHS2=1 'ReadyOut=1
   Wait ioHS2=1 'wait ConfirmIn=1
   ioCOM=0 'Hibaküldés kinullázása
   ioHS2=0 'ReadyOut=0
   Wait ioHS2=0 'wait ConfirmIn=0
   M_00#=0
 Return '=======================
 