1   ' Fontos paraméterek beszerzése
2   ' Elso a tengely limitek, hogy ne hazárdirozás legyen mikor nyekk!
3   ' és TB-n lehessen szabályozni
4   PrmRead 0, "MEJAR",CMEJAR$
5   Dim MNMX(7,4)
6   M3=1
7   M4=1
8   M5=Len(CMEJAR$)
9   For M1=1 To 7 Step 1
10     For M2=1 To 2 Step 1
11       C1$=Mid$(CMEJAR$,M3,M5-M3)
12       M4=Strpos(C1$,",")
13       MNMX(M1,M2)=Val(C1$)
14       M3=M4+M3
15     Next M2
16     ' nem tetszett neki hogy elöbb a 4-est számoltam ki
17     ' ha lenne += operátor érteném de így gáz
18     MNMX(M1,3)=MNMX(M1,1)
19     MNMX(M1,4)=(MNMX(M1,2)-MNMX(M1,1))/2.0
20     MNMX(M1,3)=MNMX(M1,3)+MNMX(M1,4)
21     'User defined screen mitt lett fokban, gondolom nem hal bele a rad() ba kalkulációnál
22     'de kiderült belehal néha ott van a hiba listában, helyette a *M_PI/180.0 használom inkább :)
23   Next M1
24     'Mozgás vezérlõ, RV-13FRLM-R, K+F, BMR18905, RC1
25     'M_IP(4) -at olvasom
26     'M_QP(4) -ot írom
27     'Globális változók hívása
28     M_00#=0
29     Dim M_IP(4)
30     Dim M_QP(4)
31     'Dim M_Parancs
32     '
33    Servo Off
34    '
35    'Helyi változók felsorolás-------------------
36    Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
37    Def Io ioHS1=Bit,10160
38    Def Io ioHS2=Bit,10161
39    Def Io ioHS3=Bit,10162
40    Def Io ioCOM=DWord,10192
41    Def Io ioSEC=DWord,10224
42    Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
43    Def Io oMx=DWord,10256
44    Def Io oMy=DWord,10288
45    Def Io oMz=DWord,10320
46    Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
47    Def Io oMa=DWord,10352
48    Def Io oMb=DWord,10384
49    Def Io oMc=DWord,10416
50    Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
51    Def Io oMs12=DWord,10448
52    Def Io oMs32=DWord,10480
53    Def Io oMsR2D=DWord,10512
54    oMs12=0
55    Rem -  -- ---=--==-=== MAJRE BUTTON ===-==--=--- --  -
56    Def Io ioMajre=Bit,11313
57    MSebesseg=100 'Robot sebesség EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 100
58    MutazasiMagassag=740 'Utazási magasság a fejnek EZT NEM SZABAD MÁSHOL VÁLTOZTATNI
59    Mgyors=80 'Robot gyorsulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
60    Mlass=80 'Robot lassulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
61    MSebPakolas=100 'Felvételi, lerakási sebesség!!!
62    MlstV=MSebesseg
63    MGyorsPakolas=100 'Felvételi, lerakási gyorsulás!!!
64    MLassPakolas=100 'Felvételi, lerakási lassulás!!!
65    Mi1=0 'Minden For ciklusnak
66    Mi2=0 'Lefoglalva a Getparam fv-nek
67    Dim MIp(4) 'bemenetek helyi változója
68    Dim MQp(4) 'kimenetek helyi változója
69    '
70    Dim pKamera(4) 'Kamera pozíció a 4 típusra
71    '
72    'Tool (100,100,100,100)
73    Mdarab=0 'Megfogó darab érzékelés tárolása
74    MHibaszam=0 'Hibaszám tárolása
75    MPosS=0 'Pozíció index tárolása
76    MPosX=0 'X pozíció átvétele
77    MPosY=0 'Y pozíció átvétele
78    MPosZ=0 'Z pozíció átvétele
79    MPosA=0 'A pozíció fok átvétele
80    MPosB=0 'B pozíció fok átvétele
81    MPosC=0 'C pozíció fok átvétele
82    MPosXe=0 'X pozíció eltolás átvétele
83    MPosYe=0 'Y pozíció eltolás átvétele
84    MPosZe=0 'Z pozíció eltolás átvétele
85    MPosAe=0 'A pozíció fok eltolás átvétele
86    MPosBe=0 'B pozíció fok eltolás átvétele
87    MPosCe=0 'C pozíció fok eltolás átvétele
88    MPosJ1=0 'J1 tengely fok átvétele
89    MPosJ2=0 'J2 tengely fok átvétele
90    MPosJ3=0 'J3 tengely fok átvétele
91    MPosJ4=0 'J4 tengely fok átvétele
92    MPosJ5=0 'J5 tengely fok átvétele
93    MPosJ6=0 'J6 tengely fok átvétele
94    MPosJ1e=0 'J1 tengely fok eltolás átvétele
95    MPosJ2e=0 'J2 tengely fok eltolás átvétele
96    MPosJ3e=0 'J3 tengely fok eltolás átvétele
97    MPosJ4e=0 'J4 tengely fok eltolás átvétele
98    MPosJ5e=0 'J5 tengely fok eltolás átvétele
99    MPosJ6e=0 'J6 tengely fok eltolás átvétele
100    'MHibaszám jelentései:
101    '900 - Rossz parancs küldése a robotnak
102    '910 - Rossz mozgás pozíció index paraméter (MpozIndex)
103    '920 - Servo OFF
104    '930 - Range problem *S10
105    '931 - Range problem *S11
106    '999 - Alaphelyzetfelvételi hiba
107    '
108    MFel=0 'Ez a változó, ha nulla, akkor bármilyen parancs jöhet
109    MELOZOTERULET=0
110    MTERULET=0
111    'MTERULET jelentései (milyen pozícióba képes menni):
112    '10 - alaphelyzet pozíció jog
113    '20 - szerviz pozício jog
114    '30 - amegfogó ürítése jog
115    '
116    mXtemp=0 'alaphelyzet pozíció kereséshez
117    mYtemp=0 'alaphelyzet pozíció kereséshez
118    mZtemp=0 'alaphelyzet pozíció kereséshez
119    'ptemp0=0 pozíció segéd alaphelyzethez
120    'ptemp1=0 'pozíció segéd folyamathoz
121    'jtemp1=0 'pozíció segéd forgatáshoz
122    MTavol=0 'alaphelyzetbe értünk-e már?
123    '===================================================
124    '===================================================
125    '---------------------------------------------------
126    'Helyi változók felsorolás vége------------------
127    '---------------------------------------------------
128    '
129    'Hiba esetén megszakítás generálódik
130   '
131   While (M_Err=0)
132     Select ioCOM 'Mozgás végrehajtás parancs
133       Case 0 'nope
134         Mms = M_Timer(1)
135         Ms=Mms/1000.0
136         oMs12 = Mms
137         Break
138       ' DANGER!! nincsen megálapodás róla IGNORE küldjön hibát
139       'Case 1 'Alaphelyzet felvétel ES után
140       '  GoSub *S1
141       'Break
142       'Case 2 'Alaphelyzet felvétel CIKLUS
143       '  GoSub *S2
144       'Break
145       'Case 3 'Szerviz pozíció felvétel
146       '  GoSub *S3
147       'Break
148       'Case 4 'Megfogó kiürítése
149       '  GoSub *S4
150       'Break
151       Case 10 'XYZ ABC joint
152         GoSub *S10
153         Break
154       Case 11 'XYZ ABC lineáris
155         GoSub *S110
156         Break
157       ' DANGER!! még nem nem tudtam foglalkozni vele IGNORE küldjön hibát
158       'Case 12 'XYZ ABC lineáris erõre
159       '  GoSub *S12
160       'Break
161       'Case 20 'XYZ ABC eltolás joint
162       '  GoSub *S20
163       'Break
164       'Case 21 'XYZ ABC eltolás lineáris
165       '  GoSub *S21
166       'Break
167       'Case 22 'XYZ ABC eltolás lineáris erõre
168       '  GoSub *S22
169       'Break
170       'Case 30 'J1-J6
171       '  GoSub *S30
172       'Break
173       'Case 40 'J1-J6 eltolás
174       '  GoSub *S40
175       'Break
176       Default
177         'IGNORE küldjön hibát
178         GoSub *HANDSHAKE1
179         MHibaszam=900
180         GoSub *HANDSHAKE2
181         Break
182     End Select
183     Dly 0.001
184   WEnd
185   End
186   '
187   '
188   '=================================================================
189   '---------------------------------
190   'Szubrutinok következnek-----
191   '---------------------------------
192   '=================================================================
193   *S1 '=======================
194   'Alaphelyzet
195    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
196    'Inicializációs rész
197    Fsc Off
198    Servo On 'szervo
199    Ovrd 5 'Sebesség
200    Accel 5,5 'Gyorsulás, lassulás
201    M_Tool=0
202   MHibaszam=0 'Hiba nullázás
203   Mdarab=0
204   '=================================================================
205   '=================================================================
206   'Mozgás: Kamerás ellenõrzõ pozíciótól
207    mXtemp = Deg(J_Curr.J4)
208    mYtemp = Deg(J_Curr.J1)
209    mZtemp = Deg(J_Curr.J2)
210   If ((mXtemp>60 And mYtemp<15) Or (mXtemp>60 And mZtemp<-20)) Then
211   'Kamerás pozícióban áll a robot!
212   Cnt 0
213   Mov jKamera
214     Dly 0.01
215   Mov jAlaph
216     Dly 0.01
217   EndIf
218   '=================================================================
219   '=================================================================
220   'Mozgás: Lerakó területrõl
221     ptemp0 = P_Curr 'Pillanatnyi pozíció
222   If (ptemp0.X > (250)) Then
223   'Lerakó pozícióban áll a fej!
224   Cnt 0
225   Dly 0.1
226     ptemp0.Z=520
227   Mvs ptemp0
228     Dly 0.01
229   Mov jLerakas
230     Dly 0.01
231   Mov jAlaph
232     Dly 0.01
233   EndIf
234   '=================================================================
235   '=================================================================
236   'Mozgás: Felvételi pozíciótól
237     ptemp0 = P_Curr 'Pillanatnyi pozíció
238   If ((ptemp0.X) < (100)) Then
239   'Felvételi pozícióban áll a fej!
240   Cnt 0
241     ptemp0.Z=520
242   Mvs ptemp0
243     Dly 0.01
244   Mov jFelvetel
245     Dly 0.01
246   Mov jAlaph
247     Dly 0.01
248   EndIf
249   '=================================================================
250   '=================================================================
251   ptemp0 = P_Curr 'Pillanatnyi pozíció
252   If ((ptemp0.X <= (250)) And  (ptemp0.X >= (100))) Then
253   Mov jAlaph
254   Dly 0.01
255   Else
256   MHibaszam=999
257   EndIf
258   '=================================================================
259   '=================================================================
260   'Lekérdezés, hogy Alaph-ben vagyunk
261    mXtemp = Deg(J_Curr.J1)
262    mYtemp = Deg(jAlaph.J1)
263    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
264     Dly 0.1
265    WEnd
266    mXtemp = Deg(J_Curr.J2)
267    mYtemp = Deg(jAlaph.J2)
268    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
269     Dly 0.1
270    WEnd
271    mXtemp = Deg(J_Curr.J3)
272    mYtemp = Deg(jAlaph.J3)
273    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
274     Dly 0.1
275    WEnd
276    mXtemp = Deg(J_Curr.J4)
277    mYtemp = Deg(jAlaph.J4)
278    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
279     Dly 0.1
280    WEnd
281    mXtemp = Deg(J_Curr.J5)
282    mYtemp = Deg(jAlaph.J5)
283    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
284     Dly 0.1
285    WEnd
286    mXtemp = Deg(J_Curr.J6)
287    mYtemp = Deg(jAlaph.J6)
288    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
289     Dly 0.1
290    WEnd
291   '
292   'Belsõ változók beállítása
293    MELOZOTERULET=10
294    MTERULET=10
295    MFel=0
296   '
297    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
298   Return '=======================
299   '=================================================================
300   '*************************************************************************
301   '*************************************************************************
302   '=================================================================
303   *S2 '=======================
304   If (MFel=0) Then
305   'Alaphelyzet
306    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
307    'Inicializációs rész
308     Ovrd MSebesseg 'Sebesség
309    Accel Mgyors,Mlass 'Gyorsulás
310   '
311    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
312   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
313    MTERULET=10 'amelyik terulethez kell állítani a tengelyeket
314    GoSub *TERULETVALTAS
315    Mov jAlaph
316   Dly 0.01
317   MHibaszam=0 'Hiba nullázás
318   '
319    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
320   EndIf
321   Return '=======================
322   '=================================================================
323   '*************************************************************************
324   '*************************************************************************
325   '=================================================================
326   *S3 '=======================
327   If (MFel=0) Then
328   'Szerviz pozíció felvétele
329    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
330    'Inicializációs rész
331    Ovrd 10 'Sebesség
332    Accel 10,10 'Gyorsulás
333   '
334    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
335   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
336    MTERULET=20 'amelyik terulethez kell állítani a tengelyeket
337    GoSub *TERULETVALTAS
338    Mov jSzerviz
339     Dly 0.01
340    MFel=0
341   '
342    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
343   EndIf
344   Return '=======================
345   '=================================================================
346   '*************************************************************************
347   '*************************************************************************
348   '=================================================================
349   *S4 '=======================
350   If (MFel=0) Then
351   'Megfogó kiürítése
352    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
353    'Inicializációs rész
354     Ovrd MSebesseg 'Sebesség
355    Accel Mgyors,Mlass 'Gyorsulás
356   '
357    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
358   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
359    MTERULET=30 'amelyik terulethez kell állítani a tengelyeket
360    GoSub *TERULETVALTAS
361    Mov jMurites
362     Dly 0.001
363   Cnt 0
364   Mvs pMurites
365     Dly 0.01
366   '
367    Mdarab=0
368    MFel=0
369   '
370    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
371   EndIf
372   Return '=======================
373   '=================================================================
374   '=================================================================
375   '**************************XYZ ABC joint**************************
376   '=================================================================
377   '=================================================================
378   *S10 '=======================
379     '
380     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
381       'XYZ ABC joint
382       GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
383       ptemp1=P_Curr
384       ptemp1.X=(MPosX/100)
385       ptemp1.Y=(MPosY/100)
386       ptemp1.Z=(MPosZ/100)
387       If Dist(P_Zero,ptemp1) < 300 Then
388         MHibaszam=910 'Zero detect!!!
389       Else 'Zero detect!!!
390         ptemp1.A=Rad(MPosA/100)
391         ptemp1.B=Rad(MPosB/100)
392         ptemp1.C=Rad(MPosC/100)
393         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
394         '
395         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
396           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
397           ' akkor nekem nem kell alahelyzet felvétel
398           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
399           If M_Svo=0 Then
400             If M_Run(3)=1 Then
401               Servo On 'szervo
402             EndIf ' R2D2.prg
403           EndIf ' M_Servo?
404           Fsc Off
405           M_Tool=0
406           Ovrd MSebesseg 'Sebesség
407           Accel Mgyors,Mlass 'Gyorsulás
408           '
409           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
410           Rem úgy gondolom bajos, lutri
411           ' Cnt 1
412           '
413           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
414           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
415           'GoSub *TERULETVALTAS
416           MELOZOTERULET=MTERULET
417           MHibaszam=0
418           '
419           Cnt 0
420           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
421           '
422           M2=Abs(ptemp1.A)
423              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
424              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
425           '
426           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
427           '
428           'If ((MPosX>-100000& And MPosX<100000& And MPosY>-100000& And MPosY<100000& And MPosZ>-30000 And MPosZ<150000&) And (MPosA>-36000& And MPosA<36000& And MPosB>-36000& And MPosB<36000& And MPosC>-36000& And MPosC<36000&)) Then
429           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
430           If M1 < M3 And M2 < (M_PI*2.0) Then
431             M1=PosCq(ptemp1)
432             M2=0
433             While (M1=0) And (M2<100)
434               Select M2
435                 Case 0 'talán rossz az f2
436                   jtemp1 = PtoJ(ptemp1)
437                   ptemp1 = JtoP(jtemp1)
438                   Dly 1.0 'a userdefined screenen lassam itt van
439                 Break
440                 Case 1 'kérjünk egy közbenso állapotot
441                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
442                   Dly 1.0 'a userdefined screenen lassam itt van
443                 Break
444                 Case 2 'talán rossz a min.f2
445                   jtemp1 = PtoJ(ptemp1)
446                   ptemp1 = JtoP(jtemp1)
447                   Dly 1.0 'a userdefined screenen lassam itt van
448                 Break
449                 Case 3 '
450                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
451                   M2=99
452                   Dly 4.0 'a userdefined screenen lassam itt van
453                 Break
454               End Select
455               M1 = PosCq(ptemp1)
456               M2=M2+1
457               Dly 1.0 'a userdefined screenen lassam itt van
458               ' valami nem tetszett
459             WEnd
460             M1 = PosCq(ptemp1)
461             If M1=1 Then
462               Mov ptemp1
463             Else
464               'küldjön akkor is hibát ha netán sikerül mozogni
465               'ezeket fel akarom jegyezni
466               MHibaszam=931
467               Dly 10.0 'a userdefined screenen lassam itt van
468               ' valami nem tetszett
469               ' esetleg le löjem, vagy sem? 10 másodperc van rá
470               Mov ptemp1
471             EndIf
472             '
473             MFel=0 'Bármelyik parancs következhet
474             '
475           Else ' CAGE BOX
476             MHibaszam=911
477           EndIf ' CAGE BOX?
478         EndIf ' MOVE? Dist?
479       EndIf  'Zero detect?
480       GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
481     Else 'MFel?
482       MHibaszam=900
483     EndIf 'MFel?
484   Return '=======================
485   '=================================================================
486   '=================================================================
487   '************************XYZ ABC lineáris**************************
488   '=================================================================
489   '=================================================================
490   *S110 '=======================
491     GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
492   *S110CNT
493     If (MFel<>0) Then 'MFel?
494       MHibaszam=900
495     Else  'MFel?
496       ptemp2=P_Curr
497       ptemp2.X=(MPosX/100)
498       ptemp2.Y=(MPosY/100)
499       ptemp2.Z=(MPosZ/100)
500       If Dist(P_Zero,ptemp2) < 300 Then 'Zero detect!!!
501         MHibaszam=910 'Zero detect!!!
502       Else 'Zero detect!!!
503         ptemp2.A=Rad(MPosA/100)
504         ptemp2.B=Rad(MPosB/100)
505         ptemp2.C=Rad(MPosC/100)
506         'ABC Posture különbség
507         M2=Abs(Cos(P_Curr.A)-Cos(ptemp2.A))
508           If M2<Abs(Cos(P_Curr.B)-Cos(ptemp2.B)) Then M2=Abs(Cos(P_Curr.B)-Cos(ptemp2.B))
509           If M2<Abs(Cos(P_Curr.C)-Cos(ptemp2.C)) Then M2=Abs(Cos(P_Curr.C)-Cos(ptemp2.C))
510         'XYZ Távolság
511         M1=Dist(P_Curr,ptemp2)
512         MHibaszam=0
513         If (M1+M2)=0 Then  ' MOVE? TURN?
514           MHibaszam=0
515         Else ' MOVE? TURN?
516           If M_Svo=0 Then
517             If M_Run(3)=1 Then
518               Servo On 'szervo
519             EndIf ' R2D2.prg
520           EndIf ' M_Servo?
521           Fsc Off
522           M_Tool=0
523           ' Tehát M1 mm-ben meg van a távolság
524           Md = M1
525           MlstV=MSebesseg
526           'Me=(MPosS-MlstE) ' Mikorra kéne ott lenni
527           If MPosS>0 Then
528             ' megvan adva mennyi ido van rá
529             MPosS100=MPosS/100.0
530             MV=(100*M_OPovrd)/100.0 'sebesség 150mm/sec * OP
531             M6=(Md/MV)*1000.0 'd/Mv = sec lenne max sebességben, felszorozzuk menyivel több ebben a sebeségen ?
532             MlstV2=M6/MPosS100
533             MlstV=MlstV2
534             If MlstV>MSebesseg Then
535                 MlstV=MSebesseg
536             ElseIf MlstV < 1 Then
537                 MlstV = 1
538             EndIf
539           EndIf
540           Ovrd MlstV 'MSebesseg 'Sebesség
541           Accel Mgyors,Mlass 'Gyorsulás
542           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
543           MELOZOTERULET=MTERULET
544           Cnt 0
545           ' CAGE check -----------------------------------------------
546           M2=Abs(ptemp2.A)
547             If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
548             If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
549           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
550           If (M1 > M3) Or (M2 > (M_PI*2.0)) Then ' CAGE BOX
551             MHibaszam=911
552           Else ' CAGE BOX
553             jtemp2 = PtoJ(ptemp2)
554             'old If (Abs(jtemp2.J4)<Rad(90)) And Abs(jtemp2.J5)<Rad(75.0)  Then ' Soft J4
555             M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
556             M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
557             'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
558             'egyszeru eltérést számol a középtol
559             If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Soft J4
560               Mvs ptemp2 WthIf M_Ratio >= 75, ioHS3 = 1
561               M_Out(11313)=1
562               M_Out(11314)=0
563               Cnt 0
564             Else ' Soft J4
565               M_Out(11314)=1
566               M_Out(11313)=0
567               ' Hard J4
568               jtemp0=J_Curr
569               ' FLIP? 4 ax
570               If M4>MNMX(4,4) Then
571                 If Deg(jtemp2.J4)>MNMX(4,3) Then
572                   jtemp2.J4=jtemp2.J4-M_PI
573                 Else
574                   jtemp2.J4=jtemp2.J4+M_PI
575                 EndIf
576                 ' FLIP 5 ax
577                 jtemp2.J5=jtemp2.J5*-1.0
578                 M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet így mar jó?
579                 ' FLIP 6 ax
580                 jtemp2.J6=jtemp2.J6*-1.0
581               EndIf
582               If M5>MNMX(5,4) Then
583                 MHibaszam=9115
584                 If Deg(jtemp2.J5)>MNMX(5,3) Then
585                   jtemp2.J5=M_PI*((MNMX(5,2)-1.0)/180.0) 'maximum
586                 Else
587                   jtemp2.J5=M_PI*((MNMX(5,1)+1.0)/180.0) 'minimum
588                 EndIf
589               EndIf
590               ' MIDLE jtemp1
591               jtemp1.J1 = (jtemp2.J1-J_Curr.J1)*0.66 + J_Curr.J1
592               jtemp1.J2 = (jtemp2.J2-J_Curr.J2)*0.66 + J_Curr.J2
593               jtemp1.J3 = (jtemp2.J3-J_Curr.J3)*0.66 + J_Curr.J3
594               jtemp1.J4 = (jtemp2.J4-J_Curr.J4)*0.66 + J_Curr.J4
595               jtemp1.J5 = (jtemp2.J5-J_Curr.J5)*0.66 + J_Curr.J5
596               jtemp1.J6 = (jtemp2.J6-J_Curr.J6)*0.66 + J_Curr.J6
597               Mov jtemp1 WthIf M_Ratio >= 75, ioHS3=1
598             EndIf ' Soft J4
599             If ioHS3=1 Then
600               Cnt 1
601               GoSub *FEEDBACK
602               Wait ioHS3=0
603               GoSub *PXYZABC
604               If ioCOM=11 Then
605                 ioHS3=0
606                 GoTo *S110CNT
607               EndIf
608             EndIf
609             ioHS3=0
610           EndIf ' CAGE BOX?
611         EndIf ' MOVE? TURN?
612       EndIf  'Zero detect?
613     EndIf 'MFel?
614     GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
615   Return '=======================
616   *S11 '=======================
617     '
618     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
619       'XYZ ABC joint
620       GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
621       ptemp1=P_Curr
622       ptemp1.X=(MPosX/100)
623       ptemp1.Y=(MPosY/100)
624       ptemp1.Z=(MPosZ/100)
625       If Dist(P_Zero,ptemp1) < 300 Then
626         MHibaszam=910 'Zero detect!!!
627       Else 'Zero detect!!!
628         ptemp1.A=Rad(MPosA/100)
629         ptemp1.B=Rad(MPosB/100)
630         ptemp1.C=Rad(MPosC/100)
631         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
632         '
633         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
634           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
635           ' akkor nekem nem kell alahelyzet felvétel
636           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
637           If M_Svo=0 Then
638             If M_Run(3)=1 Then
639               Servo On 'szervo
640             EndIf ' R2D2.prg
641           EndIf ' M_Servo?
642           Fsc Off
643           M_Tool=0
644           Ovrd MSebesseg 'Sebesség
645           Accel Mgyors,Mlass 'Gyorsulás
646           '
647           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
648           Rem úgy gondolom bajos, lutri
649           ' Cnt 1
650           '
651           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
652           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
653           'GoSub *TERULETVALTAS
654           MELOZOTERULET=MTERULET
655           MHibaszam=0
656           '
657           Cnt 0
658           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
659           '
660           M2=Abs(ptemp1.A)
661              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
662              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
663           '
664           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
665           '
666           If M1 < M3 And M2 < (M_PI*2.0) Then
667             M1=PosCq(ptemp1)
668             M2=0
669             While (M1=0) And (M2<100)
670               Select M2
671                 Case 0 'talán rossz az f2
672                   jtemp1 = PtoJ(ptemp1)
673                   ptemp1 = JtoP(jtemp1)
674                   Dly 1.0 'a userdefined screenen lassam itt van
675                 Break
676                 Case 1 'kérjünk egy közbenso állapotot
677                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
678                   Dly 1.0 'a userdefined screenen lássam itt van
679                 Break
680                 Case 2 'talán rossz a min.f2
681                   jtemp1 = PtoJ(ptemp1)
682                   ptemp1 = JtoP(jtemp1)
683                   Dly 1.0 'a userdefined screenen lássam itt van
684                 Break
685                 Case 3 '
686                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
687                   M2=99
688                   Dly 1.0 'a userdefined screenen lássam itt van
689                 Break
690               End Select
691               M1=PosCq(ptemp1)
692               M2=M2+1
693               Dly 1.0 'a userdefined screenen lássam itt van
694               ' valami nem tetszett
695             WEnd
696             '
697             jtemp1 = PtoJ(ptemp1)
698             If Abs(jtemp1.J4) > (M_PI*2.0/4.0) Then
699               Mvs ptemp1 Type 1,0
700               MHibaszam=931
701             Else
702               Mvs ptemp1
703             EndIf
704             '
705             MFel=0 'Bármelyik parancs következhet
706             '
707           Else ' CAGE BOX
708             MHibaszam=911
709           EndIf ' CAGE BOX?
710         EndIf ' MOVE? Dist?
711       EndIf  'Zero detect?
712       GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
713     Else 'MFel?
714       MHibaszam=900
715     EndIf 'MFel?
716   Return '=======================
717   '=================================================================
718   '=================================================================
719   '*********************XYZ ABC lineáris erõre***********************
720   '=================================================================
721   '=================================================================
722   *S12 '=======================
723   '
724   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
725    'XYZ ABC lineáris erõre
726    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
727   '
728    Ovrd MSebesseg 'Sebesség
729    Accel Mgyors,Mlass 'Gyorsulás
730   '
731   Cnt 1
732   '
733   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
734    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
735    GoSub *TERULETVALTAS
736   MHibaszam=0
737   '
738   Cnt 0
739   '
740   ptemp1=P_Curr 'Pillanatnyi pozíció
741   '
742   If ((MPosX>100 And MPosX<300 And MPosY>100 And MPosY<300 And MPosZ>100 And MPosZ<300) And (MPosA>100 And MPosA<300 And MPosB>100 And MPosB<300 And MPosC>100 And MPosC<300)) Then
743    ptemp1.X=(MPosX/100)
744    ptemp1.Y=(MPosY/100)
745    ptemp1.Z=(MPosZ/100)
746    ptemp1.A=Rad(MPosA/100)
747    ptemp1.B=Rad(MPosB/100)
748    ptemp1.C=Rad(MPosC/100)
749    Fsc On, 1,1,1
750    FsCTrg 1,10,1
751    Mvs ptemp1
752    Dly 0.01
753    Fsc Off
754   '
755   MFel=0 'Bármelyik parancs következhet
756   '
757   Else
758   MHibaszam=910
759   EndIf
760   '
761    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
762   '
763   Else
764   MHibaszam=900
765   EndIf
766   Return '=======================
767   '=================================================================
768   '=================================================================
769   '********************XYZ ABC eltolás joint************************
770   '=================================================================
771   '=================================================================
772   *S20 '=======================
773   '
774   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
775    'XYZ ABC eltolás joint
776    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
777   '
778    Ovrd MSebesseg 'Sebesség
779    Accel Mgyors,Mlass 'Gyorsulás
780   '
781   Cnt 1
782   '
783   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
784    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
785    GoSub *TERULETVALTAS
786   MHibaszam=0
787   '
788   Cnt 0
789   '
790   ptemp1=P_Curr 'Pillanatnyi pozíció
791   '
792   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
793    ptemp1.X=ptemp1.X+(MPosXe/100)
794    ptemp1.Y=ptemp1.Y+(MPosYe/100)
795    ptemp1.Z=ptemp1.Z+(MPosZe/100)
796    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
797    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
798    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
799    Mov ptemp1
800    Dly 0.01
801   '
802   MFel=0 'Bármelyik parancs következhet
803   '
804   Else
805   MHibaszam=910
806   EndIf
807   '
808    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
809   '
810   Else
811   MHibaszam=900
812   EndIf
813   Return '=======================
814   '=================================================================
815   '=================================================================
816   '********************XYZ ABC eltolás lineáris*********************
817   '=================================================================
818   '=================================================================
819   *S21 '=======================
820   '
821   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
822    'XYZ ABC eltolás lineáris
823    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
824   '
825    Ovrd MSebesseg 'Sebesség
826    Accel Mgyors,Mlass 'Gyorsulás
827   '
828   Cnt 1
829   '
830   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
831    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
832    GoSub *TERULETVALTAS
833   MHibaszam=0
834   '
835   Cnt 0
836   '
837   ptemp1=P_Curr 'Pillanatnyi pozíció
838   '
839   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
840    ptemp1.X=ptemp1.X+(MPosXe/100)
841    ptemp1.Y=ptemp1.Y+(MPosYe/100)
842    ptemp1.Z=ptemp1.Z+(MPosZe/100)
843    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
844    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
845    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
846    Mvs ptemp1
847    Dly 0.01
848   '
849   MFel=0 'Bármelyik parancs következhet
850   '
851   Else
852   MHibaszam=910
853   EndIf
854   '
855    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
856   '
857   Else
858   MHibaszam=900
859   EndIf
860   Return '=======================
861   '=================================================================
862   '=================================================================
863   '*****************XYZ ABC eltolás lineáris erõre******************
864   '=================================================================
865   '=================================================================
866   *S22 '=======================
867   '
868   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
869    'XYZ ABC eltolás lineáris erõre
870    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
871   '
872    Ovrd MSebesseg 'Sebesség
873    Accel Mgyors,Mlass 'Gyorsulás
874   '
875   Cnt 1
876   '
877   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
878    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
879    GoSub *TERULETVALTAS
880   MHibaszam=0
881   '
882   Cnt 0
883   '
884   ptemp1=P_Curr 'Pillanatnyi pozíció
885   '
886   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
887    ptemp1.X=ptemp1.X+(MPosXe/100)
888    ptemp1.Y=ptemp1.Y+(MPosYe/100)
889    ptemp1.Z=ptemp1.Z+(MPosZe/100)
890    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
891    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
892    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
893    Mvs ptemp1
894    Dly 0.01
895   '
896   MFel=0 'Bármelyik parancs következhet
897   '
898   Else
899   MHibaszam=910
900   EndIf
901   '
902    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
903   '
904   Else
905   MHibaszam=900
906   EndIf
907   Return '=======================
908   '=================================================================
909   '=================================================================
910   '*******************************J1-J6*****************************
911   '=================================================================
912   '=================================================================
913   *S30 '=======================
914   '
915   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
916    'J1-J6
917    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
918   '
919    Ovrd MSebesseg 'Sebesség
920    Accel Mgyors,Mlass 'Gyorsulás
921   '
922   Cnt 1
923   '
924   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
925    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
926    GoSub *TERULETVALTAS
927   MHibaszam=0
928   '
929   Cnt 0
930   '
931   jtemp1=J_Curr 'Pillanatnyi pozíció
932   '
933   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
934    jtemp1.J1=Rad(MPosJ1/100)
935    jtemp1.J2=Rad(MPosJ2/100)
936    jtemp1.J3=Rad(MPosJ3/100)
937    jtemp1.J4=Rad(MPosJ4/100)
938    jtemp1.J5=Rad(MPosJ5/100)
939    jtemp1.J6=Rad(MPosJ6/100)
940    Mov jtemp1
941    Dly 0.01
942    '
943    MFel=0 'Bármelyik parancs következhet
944    '
945   Else
946   MHibaszam=910
947   EndIf
948   '
949    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
950   '
951   Else
952   MHibaszam=900
953   EndIf
954   Return '=======================
955   '=================================================================
956   '=================================================================
957   '**************************J1-J6 eltolás**************************
958   '=================================================================
959   '=================================================================
960   *S40 '=======================
961   '
962   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
963    'J1-J6 eltolás
964    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
965   '
966    Ovrd MSebesseg 'Sebesség
967    Accel Mgyors,Mlass 'Gyorsulás
968   '
969   Cnt 1
970   '
971   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
972    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
973    GoSub *TERULETVALTAS
974   MHibaszam=0
975   '
976   Cnt 0
977   '
978   jtemp1=J_Curr 'Pillanatnyi pozíció
979   '
980   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
981    jtemp1.J1=jtemp1.J1+Rad(MPosJ1/100)
982    jtemp1.J2=jtemp1.J2+Rad(MPosJ2/100)
983    jtemp1.J3=jtemp1.J3+Rad(MPosJ3/100)
984    jtemp1.J4=jtemp1.J4+Rad(MPosJ4/100)
985    jtemp1.J5=jtemp1.J5+Rad(MPosJ5/100)
986    jtemp1.J6=jtemp1.J6+Rad(MPosJ6/100)
987    Mov jtemp1
988    Dly 0.01
989   '
990   MFel=0 'Bármelyik parancs következhet
991   '
992   Else
993   MHibaszam=910
994   EndIf
995   '
996    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
997   '
998   Else
999   MHibaszam=900
1000   EndIf
1001   Return '=======================
1002   '=================================================================
1003   '=================================================================
1004   '*****************************************************************
1005   '=================================================================
1006   '=================================================================
1007   '
1008   *FEEDBACK '=======================
1009     Mms = M_Timer(1)
1010     Ms=Mms/1000.0
1011     oMs12 = Mms
1012     oMx=P_Curr.X*100
1013     oMy=P_Curr.Y*100
1014     oMz=P_Curr.Z*100
1015     oMa=Deg(P_Curr.A)*100
1016     oMb=Deg(P_Curr.B)*100
1017     oMc=Deg(P_Curr.C)*100
1018   Return '=======================
1019   '
1020   *PXYZABC '=======================
1021     MPosS=ioSEC 'pozíció index átvétele
1022     MPosX=oMx 'X pozíció átvétele
1023     MPosY=oMy 'Y pozíció átvétele
1024     MPosZ=oMz 'Z pozíció átvétele
1025     MPosA=oMa 'A pozíció fok átvétele
1026     MPosB=oMb 'B pozíció fok átvétele
1027     MPosC=oMc 'C pozíció fok átvétele
1028   Return '=======================
1029   '
1030   *HANDSHAKE1 '=======================
1031     Wait ioHS1=1 'wait RobotStartIn=1
1032     GoSub *PXYZABC
1033     ' eddig tuti ---------------------------
1034     MPosXe=M_In32(10448) 'X pozíció eltolás átvétele
1035     MPosYe=M_In32(10480) 'Y pozíció eltolás átvétele
1036     MPosZe=M_In32(10512) 'Z pozíció eltolás átvétele
1037     MPosAe=M_In32(10544) 'A pozíció fok eltolás átvétele
1038     MPosBe=M_In32(10576) 'B pozíció fok eltolás átvétele
1039     MPosCe=M_In32(10608) 'C pozíció fok eltolás átvétele
1040     MPosJ1=M_In32(10640) 'J1 tengely fok átvétele
1041     MPosJ2=M_In32(10672) 'J2 tengely fok átvétele
1042     MPosJ3=M_In32(10704) 'J3 tengely fok átvétele
1043     MPosJ4=M_In32(10736) 'J4 tengely fok átvétele
1044     MPosJ5=M_In32(10768) 'J5 tengely fok átvétele
1045     MPosJ6=M_In32(10800) 'J6 tengely fok átvétele
1046     MPosJ1e=M_In32(10832) 'J1 tengely fok eltolás átvétele
1047     MPosJ2e=M_In32(10864) 'J2 tengely fok eltolás átvétele
1048     MPosJ3e=M_In32(10896) 'J3 tengely fok eltolás átvétele
1049     MPosJ4e=M_In32(10928) 'J4 tengely fok eltolás átvétele
1050     MPosJ5e=M_In32(10960) 'J5 tengely fok eltolás átvétele
1051     MPosJ6e=M_In32(10992) 'J6 tengely fok eltolás átvétele
1052     ioHS1=1 'ConfirmOut=1
1053     Wait ioHS1=0 'wait RobotStartIn=0
1054     Wait ioCOM=0 'Mozgás parancs nullázására vár
1055     'Wait ioSEC=0 'pozíció index kinullázására vár
1056     'Wait oMx=0 'X pozíció átvétele parancs nullázására vár
1057     'Wait oMy=0 'Y pozíció átvétele parancs nullázására vár
1058     'Wait oMz=0 'Z pozíció átvétele parancs nullázására vár
1059     'Wait oMa=0 'A pozíció fok átvétele parancs nullázására vár
1060     'Wait oMb=0 'B pozíció fok átvétele parancs nullázására vár
1061     'Wait oMc=0 'C pozíció fok átvétele parancs nullázására vár
1062     'Wait M_In32(10448)=0 'X pozíció eltolás átvétele parancs nullázására vár
1063     'Wait M_In32(10480)=0 'Y pozíció eltolás átvétele parancs nullázására vár
1064     'Wait M_In32(10512)=0 'Z pozíció eltolás átvétele parancs nullázására vár
1065     'Wait M_In32(10544)=0 'A pozíció fok eltolás átvétele parancs nullázására vár
1066     'Wait M_In32(10576)=0 'B pozíció fok eltolás átvétele parancs nullázására vár
1067     'Wait M_In32(10608)=0 'C pozíció fok eltolás átvétele parancs nullázására vár
1068     'Wait M_In32(10640)=0 'J1 tengely fok átvétele parancs nullázására vár
1069     'Wait M_In32(10672)=0 'J2 tengely fok átvétele parancs nullázására vár
1070     'Wait M_In32(10704)=0 'J3 tengely fok átvétele parancs nullázására vár
1071     'Wait M_In32(10736)=0 'J4 tengely fok átvétele parancs nullázására vár
1072     'Wait M_In32(10768)=0 'J5 tengely fok átvétele parancs nullázására vár
1073     'Wait M_In32(10800)=0 'J6 tengely fok átvétele parancs nullázására vár
1074     'Wait M_In32(10832)=0 'J1 tengely fok eltolás átvétele parancs nullázására vár
1075     'Wait M_In32(10864)=0 'J2 tengely fok eltolás átvétele parancs nullázására vár
1076     'Wait M_In32(10896)=0 'J3 tengely fok eltolás átvétele parancs nullázására vár
1077     'Wait M_In32(10928)=0 'J4 tengely fok eltolás átvétele parancs nullázására vár
1078     'Wait M_In32(10960)=0 'J5 tengely fok eltolás átvétele parancs nullázására vár
1079     'Wait M_In32(10992)=0 'J6 tengely fok eltolás átvétele parancs nullázására vár
1080     ioHS1=0 'ConfirmOut=0
1081     M_00#=1 ' jelzes R2D2-nak hogy elkezdödött valami
1082   Return '=======================
1083   '
1084   *HANDSHAKE2 '=======================
1085     MPosS = 0
1086     If M_Svo(1)=0 Then
1087       ' Így nem nyírja ki miért lett leállítva
1088       If MHibaszam<1 Then
1089         MHibaszam=920
1090       EndIf
1091     EndIf
1092     ' Így az R2D2-nál is frissebb adatok lesznek mire olvasnám
1093     GoSub *FEEDBACK
1094     'ptemp1=P_Curr
1095     'oMx=ptemp1.X*100
1096     'oMy=ptemp1.Y*100
1097     'oMz=ptemp1.Z*100
1098     'oMa=Deg(ptemp1.A)*100
1099     'oMb=Deg(ptemp1.B)*100
1100     'oMc=Deg(ptemp1.C)*100
1101    '
1102    ioCOM=MHibaszam 'Hiba státusz küldése
1103    ioHS2=1 'ReadyOut=1
1104    Wait ioHS2=1 'wait ConfirmIn=1
1105    ioCOM=0 'Hibaküldés kinullázása
1106    ioHS2=0 'ReadyOut=0
1107    Wait ioHS2=0 'wait ConfirmIn=0
1108    M_00#=0
1109  Return '=======================
1110  '=================================================================
1111  '*************************************************************************
1112  '*************************************************************************
1113  '*************************************************************************
1114  '*************************************************************************
1115  '=================================================================
1116  *TERULETVALTAS '=======================
1117  If (MELOZOTERULET<>MTERULET) Then
1118  '................................
1119   If (MELOZOTERULET=10 And MTERULET=20) Then 'váltás alaphelyzetbõl szervizpozícióra
1120    Mov jAlaph
1121    Mov jSzerviz
1122  '
1123    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1124   EndIf
1125  '................................
1126   If (MELOZOTERULET=30 And MTERULET=20) Then 'váltás megfogó ürítésrõl szervizpozícióra
1127    Mov jMurites
1128    Mov jAlaph
1129    Mov jSzerviz
1130  '
1131    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1132   EndIf
1133  '................................
1134   If (MELOZOTERULET=40 And MTERULET=20) Then 'váltás felvételrõl szervizpozícióra
1135    Mov jFelvetel
1136    Mov jSzerviz
1137  '
1138    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1139   EndIf
1140  '................................
1141   If (MELOZOTERULET=50 And MTERULET=20) Then 'váltás kamerásról szervizpozícióra
1142    Mov jKamera
1143    Mov jAlaph
1144    Mov jSzerviz
1145  '
1146    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1147   EndIf
1148  '................................
1149   If (MELOZOTERULET=60 And MTERULET=20) Then 'váltás lerakásról szervizpozícióra
1150    Mov jLerakas
1151    Mov jAlaph
1152    Mov jSzerviz
1153  '
1154    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1155   EndIf
1156  '................................
1157   If (MELOZOTERULET=20 And MTERULET=10) Then 'váltás szervizpozícióról alaphelyzetre
1158    Mov jSzerviz
1159    Mov jAlaph
1160  '
1161    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1162   EndIf
1163  '................................
1164   If (MELOZOTERULET=30 And MTERULET=10) Then 'váltás megfogó ürítésrõl alaphelyzetre
1165    Mov jMurites
1166    Mov jAlaph
1167  '
1168    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1169   EndIf
1170  '................................
1171   If (MELOZOTERULET=40 And MTERULET=10) Then 'váltás felvételrõl alaphelyzetre
1172    Mov jFelvetel
1173    Mov jAlaph
1174  '
1175    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1176   EndIf
1177  '................................
1178   If (MELOZOTERULET=50 And MTERULET=10) Then 'váltás kamerásról alaphelyzetre
1179    Mov jKamera
1180     Mov jAlaph
1181   '
1182     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1183   EndIf
1184  '................................
1185   If (MELOZOTERULET=60 And MTERULET=10) Then 'váltás lerakásról alaphelyzetre
1186    Mov jLerakas
1187    Mov jAlaph
1188  '
1189    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1190   EndIf
1191  '................................
1192   If (MELOZOTERULET=10 And MTERULET=30) Then 'váltás alaphelyzetbõl megfogó ürítésre
1193    Mov jAlaph
1194    Mov jMurites
1195  '
1196    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1197   EndIf
1198  '................................
1199   If (MELOZOTERULET=20 And MTERULET=30) Then 'váltás szervizpozícióról megfogó ürítésre
1200   Mov jSzerviz
1201   Mov jAlaph
1202   Mov jMurites
1203  '
1204    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1205   EndIf
1206  '................................
1207   If (MELOZOTERULET=40 And MTERULET=30) Then 'váltás felvételrõl megfogó ürítésre
1208   Mov jFelvetel
1209   Mov jAlaph
1210   Mov jMurites
1211  '
1212    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1213   EndIf
1214  '................................
1215   If (MELOZOTERULET=50 And MTERULET=30) Then 'váltás kamerásról megfogó ürítésre
1216    Mov jKamera
1217    Mov jMurites
1218  '
1219    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1220   EndIf
1221  '................................
1222   If (MELOZOTERULET=60 And MTERULET=30) Then 'váltás lerakásról megfogó ürítésre
1223    Mov jLerakas
1224    Mov jMurites
1225  '
1226    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1227   EndIf
1228  '................................
1229   If (MELOZOTERULET=10 And MTERULET=40) Then 'váltás alaphelyzetbõl felvételre
1230    Mov jAlaph
1231    Mov jFelvetel
1232  '
1233    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1234   EndIf
1235  '................................
1236   If (MELOZOTERULET=20 And MTERULET=40) Then 'váltás szervizpozícióról felvételre
1237    Mov jSzerviz
1238    Mov jFelvetel
1239  '
1240    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1241   EndIf
1242  '................................
1243   If (MELOZOTERULET=30 And MTERULET=40) Then 'váltás megfogó ürítésrõl felvételre
1244    Mov jMurites
1245    Mov jAlaph
1246    Mov jFelvetel
1247  '
1248    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1249   EndIf
1250  '................................
1251   If (MELOZOTERULET=50 And MTERULET=40) Then 'váltás kamerásról felvételre
1252    Mov jKamera
1253    Mov jFelvetel
1254  '
1255    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1256   EndIf
1257  '................................
1258   If (MELOZOTERULET=60 And MTERULET=40) Then 'váltás lerakásról felvételre
1259    Mov jLerakas
1260    Mov jAlaph
1261    Mov jFelvetel
1262  '
1263    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1264   EndIf
1265  '................................
1266   If (MELOZOTERULET=10 And MTERULET=50) Then 'váltás alaphelyzetbõl kamerásra
1267    Mov jAlaph
1268    Mov jKamera
1269  '
1270    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1271   EndIf
1272  '................................
1273   If (MELOZOTERULET=20 And MTERULET=50) Then 'váltás szervizpozícióról kamerásra
1274    Mov jSzerviz
1275    Mov jAlaph
1276    Mov jKamera
1277  '
1278    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1279   EndIf
1280  '................................
1281   If (MELOZOTERULET=30 And MTERULET=50) Then 'váltás megfogó ürítésrõl kamerásra
1282    Mov jMurites
1283     Mov jKamera
1284   '
1285     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1286   EndIf
1287  '................................
1288   If (MELOZOTERULET=40 And MTERULET=50) Then 'váltás felvételrõl kamerásra
1289    Mov jFelvetel
1290    Mov jAlaph
1291    Mov jKamera
1292  '
1293    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1294   EndIf
1295  '................................
1296   If (MELOZOTERULET=60 And MTERULET=50) Then 'váltás lerakásról kamerásra
1297    Mov jLerakas
1298    Mov jKamera
1299  '
1300    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1301   EndIf
1302  '................................
1303   If (MELOZOTERULET=10 And MTERULET=60) Then 'váltás alaphelyzetbõl lerakásra
1304    Mov jAlaph
1305    Mov jLerakas
1306  '
1307    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1308   EndIf
1309 '................................
1310   If (MELOZOTERULET=20 And MTERULET=60) Then 'váltás szervizpozícióról lerakásra
1311    Mov jSzerviz
1312    Mov jAlaph
1313    Mov jLerakas
1314  '
1315    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1316   EndIf
1317  '................................
1318   If (MELOZOTERULET=30 And MTERULET=60) Then 'váltás megfogó ürítésrõl lerakásra
1319    Mov jMurites
1320    Mov jLerakas
1321  '
1322    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1323   EndIf
1324  '................................
1325   If (MELOZOTERULET=40 And MTERULET=60) Then 'váltás felvételrõl lerakásra
1326    Mov jFelvetel
1327    Mov jAlaph
1328    Mov jLerakas
1329  '
1330    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1331   EndIf
1332  '................................
1333   If (MELOZOTERULET=50 And MTERULET=60) Then 'váltás kamerásról lerakásra
1334    Mov jKamera
1335    Mov jLerakas
1336  '
1337    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1338   EndIf
1339  '................................
1340  EndIf
1341  Return '=======================
1342 '
1343 '
1344 'Kártya Inputok:
1345 '0 - Gyárilag Foglalt
1346 '1 -
1347 '2 -
1348 '3 -
1349 '4 -
1350 '5 -
1351 '6 -
1352 '7 -
1353 '8 -
1354 '9 -
1355 '10 -
1356 '11 -
1357 '12 -
1358 '13 -
1359 '14 -
1360 '15 -
1361 '
1362 'Kártya Outputok:
1363 '0 -
1364 '1 -
1365 '2 -
1366 '3 -
1367 '4 -
1368 '5 -
1369 '6 -
1370 '7 -
1371 '8 -
1372 '9 -
1373 '10 -
1374 '11 -
1375 '12 -
1376 '13 -
1377 '14 -
1378 '15 -
1379 '
1380 '
1381 'Tool-ok:
1382 '0=semmilyen tool
jAlaph=(2.600,-33.050,136.100,0.000,76.950,92.600)
jFelvetel=(2.600,-33.050,136.100,0.000,76.950,92.600)
jKamera=(2.600,-33.050,136.100,0.000,76.950,92.600)
jLerakas=(2.600,-33.050,136.100,0.000,76.950,92.600)
jMurites=(2.600,-33.050,136.100,0.000,76.950,92.600)
jSzerviz=(2.600,-33.050,136.100,0.000,76.950,92.600)
jtemp0=(86.610,68.990,115.250,-7.390,0.060,161.200)
jtemp1=(86.890,69.160,116.720,3.280,-13.160,76.700)
jtemp2=(84.250,80.430,125.980,4.770,-34.800,73.750,0.000,0.000)
pMurites=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
ptemp0=(471.620,385.480,274.160,-163.850,39.710,-96.520)(7,15728640)
ptemp1=(606.350,463.480,652.290,157.300,2.330,-167.550)(7,0)
ptemp2=(49.890,449.700,-198.860,172.450,4.400,-173.910)(6,0)
pKamera(1)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(2)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(3)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(4)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
