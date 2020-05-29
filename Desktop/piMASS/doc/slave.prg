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
36    Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
37    Def Io ioNM=DWord,10160
38    Def Io ioMx=DWord,10192
39    Def Io ioMy=DWord,10224
40    Def Io ioMz=DWord,10256
41    Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
42    Def Io ioMa=DWord,10288
43    Def Io ioMb=DWord,10320
44    Def Io ioMc=DWord,10352
45    Rem -  -- ---=--==-=== TIME ===-==--=--- --  -
46    Def Io ioMs=DWord,10384
47    Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
48    Def Io ioHS1=Bit,10416
49    Def Io ioHS2=Bit,10417
50    Def Io ioHS3=Bit,10418
51    Def Io ioMajre=Bit,10419
52    Def Io ioPIX=DWord,10448
53    Def Io ioCOM=DWord,10480
54    Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
55    Def Io ioMsSLV=DWord,10512
56    Def Io ioMsMST=DWord,10544
57    Def Io ioMsR2D=DWord,10576
58    ' init
59    ioHS1=0
60    ioMsSLV=0
61    '
62    Rem - -- ---=--==-=== Megszakítás ===-==--=--- --  -
63    'Def Act 3,ioHS1=1 GoTo *S110IRQ, S
64    'Act 3=0
65    Rem -  -- ---=--==-=== MAJRE BUTTON ===-==--=--- --  -
66    ' EZT NEM SZABAD MÁSHOL VÁLTOZTATNI akkor konstast kell belölle csinálni
67    ' és akkor nem is lehet valahogy így:
68    Const MSebesseg=100 'Robot sebesség EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 100
69    Const MutazasiMagassag=740 'Utazási magasság a fejnek EZT NEM SZABAD MÁSHOL VÁLTOZTATNI
70    Const Mgyors=80 'Robot gyorsulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
71    Const Mlass=80 'Robot lassulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
72    Const Mhs2rt=87 'Ratio konstans
73    Const Max5off=3.0
74    Const Mmid=0.66
75    Const MmxSPD=100.0
76    Const Mzero=300.0
77    '
78    MSebPakolas=100 'Felvételi, lerakási sebesség!!!
79    MlstV=MSebesseg
80    MGyorsPakolas=100 'Felvételi, lerakási gyorsulás!!!
81    MLassPakolas=100 'Felvételi, lerakási lassulás!!!
82    Mi1=0 'Minden For ciklusnak
83    Mi2=0 'Lefoglalva a Getparam fv-nek
84    Dim MIp(4) 'bemenetek helyi változója
85    Dim MQp(4) 'kimenetek helyi változója
86    '
87    Dim pKamera(4) 'Kamera pozíció a 4 típusra
88    '
89    Rem - -- ---=--==-=== Saját funkciók ===-==--=--- --  -
90    Rem - Mabc ---------------------------------------------------------------
91    ' megadja mekkora a maximum cos a megadott szögek között
92    ' ellehet dönteni általa, hogy történi-e forgatás
93    Def FNMabc(Pa,Pb)= Max( Abs(Cos(Pa.A)-Cos(Pb.A)), Abs(Cos(Pa.B)-Cos(Pb.B)), Abs(Cos(Pa.C)-Cos(Pb.C)) )
94    '
95    Rem - Mdms ---------------------------------------------------------------
96    ' Mms menyi milisec alatt tegye meg az utat
97    ' Md a megtenni kívánt út mm-ben
98    ' MsMX max speed amit a kb a robot tud mm/sec
99    ' maximális sebeség a TB szerint ((MsMX*MsOP)/100.0) azaz "törtezünk" ((mm/sec)*70)/100
100    ' minimális ido ami alatt megtenné (Md/((MsMX*MsOP)/100.0)) ez még mm/sec (nem mm/msec) ezért *1000.0
101    '                           4       3   21             1      234
102    Def FNMdOVRD(Mms,Md,MsMX)=((1000.0*(Md/((MsMX*M_OPovrd)/100.0)))/Mms)*100.0
103    '
104    'Tool (100,100,100,100)
105    Mdarab=0 'Megfogó darab érzékelés tárolása
106    MHibaszam=0 'Hibaszám tárolása
107    MPosS=0 'Pozíció index tárolása
108    MPosX=0 'X pozíció átvétele
109    MPosY=0 'Y pozíció átvétele
110    MPosZ=0 'Z pozíció átvétele
111    MPosA=0 'A pozíció fok átvétele
112    MPosB=0 'B pozíció fok átvétele
113    MPosC=0 'C pozíció fok átvétele
114    MPosXe=0 'X pozíció eltolás átvétele
115    MPosYe=0 'Y pozíció eltolás átvétele
116    MPosZe=0 'Z pozíció eltolás átvétele
117    MPosAe=0 'A pozíció fok eltolás átvétele
118    MPosBe=0 'B pozíció fok eltolás átvétele
119    MPosCe=0 'C pozíció fok eltolás átvétele
120    MPosJ1=0 'J1 tengely fok átvétele
121    MPosJ2=0 'J2 tengely fok átvétele
122    MPosJ3=0 'J3 tengely fok átvétele
123    MPosJ4=0 'J4 tengely fok átvétele
124    MPosJ5=0 'J5 tengely fok átvétele
125    MPosJ6=0 'J6 tengely fok átvétele
126    MPosJ1e=0 'J1 tengely fok eltolás átvétele
127    MPosJ2e=0 'J2 tengely fok eltolás átvétele
128    MPosJ3e=0 'J3 tengely fok eltolás átvétele
129    MPosJ4e=0 'J4 tengely fok eltolás átvétele
130    MPosJ5e=0 'J5 tengely fok eltolás átvétele
131    MPosJ6e=0 'J6 tengely fok eltolás átvétele
132    'MHibaszám jelentései:
133    '900 - Rossz parancs küldése a robotnak
134    '910 - Zero detect
135    '911 - HS1 elött és után nem stimmel a pozició
136    '912 - CAGE BOX detect error
137    '920 - Servo OFF
138    '930 - Range problem *S10
139    '931 - Range problem *S11
140    '999 - Alaphelyzetfelvételi hiba
141    '
142    MFel=0 'Ez a változó, ha nulla, akkor bármilyen parancs jöhet
143    MELOZOTERULET=0
144    MTERULET=0
145    'MTERULET jelentései (milyen pozícióba képes menni):
146    '10 - alaphelyzet pozíció jog
147    '20 - szerviz pozício jog
148    '30 - amegfogó ürítése jog
149    '
150    mXtemp=0 'alaphelyzet pozíció kereséshez
151    mYtemp=0 'alaphelyzet pozíció kereséshez
152    mZtemp=0 'alaphelyzet pozíció kereséshez
153    'ptemp0=0 pozíció segéd alaphelyzethez
154    'ptemp1=0 'pozíció segéd folyamathoz
155    'jtemp1=0 'pozíció segéd forgatáshoz
156    MTavol=0 'alaphelyzetbe értünk-e már?
157    '===================================================
158    '===================================================
159    '---------------------------------------------------
160    'Helyi változók felsorolás vége------------------
161    '---------------------------------------------------
162    '
163    'Hiba esetén megszakítás generálódik
164   '
165   While (M_Err=0)
166     Select ioCOM 'Mozgás végrehajtás parancs
167       Case 0 'nope
168         Mms = M_Timer(1)
169         Ms=Mms/1000.0
170         ioMsSLV = Mms
171         Dly 0.001
172         Break
173       ' DANGER!! nincsen megálapodás róla IGNORE küldjön hibát
174       'Case 1 'Alaphelyzet felvétel ES után
175       '  GoSub *S1
176       'Break
177       'Case 2 'Alaphelyzet felvétel CIKLUS
178       '  GoSub *S2
179       'Break
180       'Case 3 'Szerviz pozíció felvétel
181       '  GoSub *S3
182       'Break
183       'Case 4 'Megfogó kiürítése
184       '  GoSub *S4
185       'Break
186       Case 10 'XYZ ABC joint
187         GoSub *S10
188         Break
189       Case 11 'XYZ ABC lineáris
190         GoSub *S111
191         Break
192       ' DANGER!! még nem nem tudtam foglalkozni vele IGNORE küldjön hibát
193       'Case 12 'XYZ ABC lineáris erõre
194       '  GoSub *S12
195       'Break
196       'Case 20 'XYZ ABC eltolás joint
197       '  GoSub *S20
198       'Break
199       'Case 21 'XYZ ABC eltolás lineáris
200       '  GoSub *S21
201       'Break
202       'Case 22 'XYZ ABC eltolás lineáris erõre
203       '  GoSub *S22
204       'Break
205       'Case 30 'J1-J6
206       '  GoSub *S30
207       'Break
208       'Case 40 'J1-J6 eltolás
209       '  GoSub *S40
210       'Break
211       Default
212         'IGNORE küldjön hibát
213         GoSub *HS1
214         MHibaszam=900
215         GoSub *HS2
216         Break
217     End Select
218   WEnd
219   End
220   '
221   '
222   '=================================================================
223   '---------------------------------
224   'Szubrutinok következnek-----
225   '---------------------------------
226   '=================================================================
227   *S1 '=======================
228   'Alaphelyzet
229    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
230    'Inicializációs rész
231    Fsc Off
232    Servo On 'szervo
233    Ovrd 5 'Sebesség
234    Accel 5,5 'Gyorsulás, lassulás
235    M_Tool=0
236   MHibaszam=0 'Hiba nullázás
237   Mdarab=0
238   '=================================================================
239   '=================================================================
240   'Mozgás: Kamerás ellenõrzõ pozíciótól
241    mXtemp = Deg(J_Curr.J4)
242    mYtemp = Deg(J_Curr.J1)
243    mZtemp = Deg(J_Curr.J2)
244   If ((mXtemp>60 And mYtemp<15) Or (mXtemp>60 And mZtemp<-20)) Then
245   'Kamerás pozícióban áll a robot!
246   Cnt 0
247   Mov jKamera
248     Dly 0.01
249   Mov jAlaph
250     Dly 0.01
251   EndIf
252   '=================================================================
253   '=================================================================
254   'Mozgás: Lerakó területrõl
255     ptemp0 = P_Curr 'Pillanatnyi pozíció
256   If (ptemp0.X > (250)) Then
257   'Lerakó pozícióban áll a fej!
258   Cnt 0
259   Dly 0.1
260     ptemp0.Z=520
261   Mvs ptemp0
262     Dly 0.01
263   Mov jLerakas
264     Dly 0.01
265   Mov jAlaph
266     Dly 0.01
267   EndIf
268   '=================================================================
269   '=================================================================
270   'Mozgás: Felvételi pozíciótól
271     ptemp0 = P_Curr 'Pillanatnyi pozíció
272   If ((ptemp0.X) < (100)) Then
273   'Felvételi pozícióban áll a fej!
274   Cnt 0
275     ptemp0.Z=520
276   Mvs ptemp0
277     Dly 0.01
278   Mov jFelvetel
279     Dly 0.01
280   Mov jAlaph
281     Dly 0.01
282   EndIf
283   '=================================================================
284   '=================================================================
285   ptemp0 = P_Curr 'Pillanatnyi pozíció
286   If ((ptemp0.X <= (250)) And  (ptemp0.X >= (100))) Then
287   Mov jAlaph
288   Dly 0.01
289   Else
290   MHibaszam=999
291   EndIf
292   '=================================================================
293   '=================================================================
294   'Lekérdezés, hogy Alaph-ben vagyunk
295    mXtemp = Deg(J_Curr.J1)
296    mYtemp = Deg(jAlaph.J1)
297    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
298     Dly 0.1
299    WEnd
300    mXtemp = Deg(J_Curr.J2)
301    mYtemp = Deg(jAlaph.J2)
302    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
303     Dly 0.1
304    WEnd
305    mXtemp = Deg(J_Curr.J3)
306    mYtemp = Deg(jAlaph.J3)
307    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
308     Dly 0.1
309    WEnd
310    mXtemp = Deg(J_Curr.J4)
311    mYtemp = Deg(jAlaph.J4)
312    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
313     Dly 0.1
314    WEnd
315    mXtemp = Deg(J_Curr.J5)
316    mYtemp = Deg(jAlaph.J5)
317    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
318     Dly 0.1
319    WEnd
320    mXtemp = Deg(J_Curr.J6)
321    mYtemp = Deg(jAlaph.J6)
322    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
323     Dly 0.1
324    WEnd
325   '
326   'Belsõ változók beállítása
327    MELOZOTERULET=10
328    MTERULET=10
329    MFel=0
330   '
331    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
332   Return '=======================
333   '=================================================================
334   '*************************************************************************
335   '*************************************************************************
336   '=================================================================
337   *S2 '=======================
338   If (MFel=0) Then
339   'Alaphelyzet
340    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
341    'Inicializációs rész
342     Ovrd MSebesseg 'Sebesség
343    Accel Mgyors,Mlass 'Gyorsulás
344   '
345    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
346   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
347    MTERULET=10 'amelyik terulethez kell állítani a tengelyeket
348    GoSub *TERULETVALTAS
349    Mov jAlaph
350   Dly 0.01
351   MHibaszam=0 'Hiba nullázás
352   '
353    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
354   EndIf
355   Return '=======================
356   '=================================================================
357   '*************************************************************************
358   '*************************************************************************
359   '=================================================================
360   *S3 '=======================
361   If (MFel=0) Then
362   'Szerviz pozíció felvétele
363    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
364    'Inicializációs rész
365    Ovrd 10 'Sebesség
366    Accel 10,10 'Gyorsulás
367   '
368    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
369   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
370    MTERULET=20 'amelyik terulethez kell állítani a tengelyeket
371    GoSub *TERULETVALTAS
372    Mov jSzerviz
373     Dly 0.01
374    MFel=0
375   '
376    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
377   EndIf
378   Return '=======================
379   '=================================================================
380   '*************************************************************************
381   '*************************************************************************
382   '=================================================================
383   *S4 '=======================
384   If (MFel=0) Then
385   'Megfogó kiürítése
386    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
387    'Inicializációs rész
388     Ovrd MSebesseg 'Sebesség
389    Accel Mgyors,Mlass 'Gyorsulás
390   '
391    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
392   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
393    MTERULET=30 'amelyik terulethez kell állítani a tengelyeket
394    GoSub *TERULETVALTAS
395    Mov jMurites
396     Dly 0.001
397   Cnt 0
398   Mvs pMurites
399     Dly 0.01
400   '
401    Mdarab=0
402    MFel=0
403   '
404    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
405   EndIf
406   Return '=======================
407   '=================================================================
408   '=================================================================
409   '**************************XYZ ABC joint**************************
410   '=================================================================
411   '=================================================================
412   *S10 '=======================
413     '
414     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
415       'XYZ ABC joint
416       GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
417       ptemp1=P_Curr
418       ptemp1.X=(MPosX/100)
419       ptemp1.Y=(MPosY/100)
420       ptemp1.Z=(MPosZ/100)
421       If Dist(P_Zero,ptemp1) < 300 Then
422         MHibaszam=910 'Zero detect!!!
423       Else 'Zero detect!!!
424         ptemp1.A=Rad(MPosA/100)
425         ptemp1.B=Rad(MPosB/100)
426         ptemp1.C=Rad(MPosC/100)
427         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
428         '
429         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
430           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
431           ' akkor nekem nem kell alahelyzet felvétel
432           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
433           If M_Svo=0 Then
434             If M_Run(3)=1 Then
435               Servo On 'szervo
436             EndIf ' R2D2.prg
437           EndIf ' M_Servo?
438           Fsc Off
439           M_Tool=0
440           Ovrd MSebesseg 'Sebesség
441           Accel Mgyors,Mlass 'Gyorsulás
442           '
443           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
444           Rem úgy gondolom bajos, lutri
445           ' Cnt 1
446           '
447           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
448           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
449           'GoSub *TERULETVALTAS
450           MELOZOTERULET=MTERULET
451           MHibaszam=0
452           '
453           Cnt 0
454           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
455           '
456           M2=Abs(ptemp1.A)
457              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
458              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
459           '
460           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
461           '
462           'If ((MPosX>-100000& And MPosX<100000& And MPosY>-100000& And MPosY<100000& And MPosZ>-30000 And MPosZ<150000&) And (MPosA>-36000& And MPosA<36000& And MPosB>-36000& And MPosB<36000& And MPosC>-36000& And MPosC<36000&)) Then
463           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
464           If M1 < M3 And M2 < (M_PI*2.0) Then
465             M1=PosCq(ptemp1)
466             M2=0
467             While (M1=0) And (M2<100)
468               Select M2
469                 Case 0 'talán rossz az f2
470                   jtemp1 = PtoJ(ptemp1)
471                   ptemp1 = JtoP(jtemp1)
472                   Dly 1.0 'a userdefined screenen lassam itt van
473                 Break
474                 Case 1 'kérjünk egy közbenso állapotot
475                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
476                   Dly 1.0 'a userdefined screenen lassam itt van
477                 Break
478                 Case 2 'talán rossz a min.f2
479                   jtemp1 = PtoJ(ptemp1)
480                   ptemp1 = JtoP(jtemp1)
481                   Dly 1.0 'a userdefined screenen lassam itt van
482                 Break
483                 Case 3 '
484                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
485                   M2=99
486                   Dly 4.0 'a userdefined screenen lassam itt van
487                 Break
488               End Select
489               M1 = PosCq(ptemp1)
490               M2=M2+1
491               Dly 1.0 'a userdefined screenen lassam itt van
492               ' valami nem tetszett
493             WEnd
494             M1 = PosCq(ptemp1)
495             If M1=1 Then
496               Mov ptemp1
497             Else
498               'küldjön akkor is hibát ha netán sikerül mozogni
499               'ezeket fel akarom jegyezni
500               MHibaszam=931
501               Dly 10.0 'a userdefined screenen lassam itt van
502               ' valami nem tetszett
503               ' esetleg le löjem, vagy sem? 10 másodperc van rá
504               Mov ptemp1
505             EndIf
506             '
507             MFel=0 'Bármelyik parancs következhet
508             '
509           Else ' CAGE BOX
510             MHibaszam=911
511           EndIf ' CAGE BOX?
512         EndIf ' MOVE? Dist?
513       EndIf  'Zero detect?
514       GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
515     Else 'MFel?
516       MHibaszam=900
517     EndIf 'MFel?
518   Return '=======================
519   '=================================================================
520   '=================================================================
521   '************************XYZ ABC lineáris**************************
522   '=================================================================
523   '=================================================================
524   *S111:
525     MGY = Mgyors/4
526     ML = Mlass/4
527     GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
528     MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
529     MELOZOTERULET=MTERULET
530     If (MFel<>0) Then 'MFel?
531       MHibaszam=900
532     Else  'MFel?
533       Mz = Dist(P_Zero,ptemp2)
534       If Mz < Mzero Then
535         MHibaszam=910 'Zero detect!!!
536       ElseIf Dist(ptemp1,ptemp2) > 0 Then
537         MHibaszam=911 'HS1 detect!!!
538       Else 'Zero detect!!!
539         'ptemp1,ptemp2 azonos
540         Md=Dist(P_Curr,ptemp2)
541         Ma=FNMabc(P_Curr,ptemp2)
542         If (Ma+Md)=0 Then  ' MOVE? TURN?
543           MHibaszam=0
544         Else ' MOVE? TURN?
545           If M_Svo=0 Then
546             If M_Run(3)=1 Then
547               Servo On 'szervo
548             EndIf ' R2D2.prg
549           EndIf ' M_Servo?
550           Fsc Off
551           M_Tool=0
552           ' speed
553           MlstV=100
554           If Md>0 And MPosS > 0 Then
555             MlstV=FNMdOVRD(MPosS,Md,MmxSPD)
556             MlstV=Max(1,MlstV)
557             MlstV=Min(100,MlstV)
558           EndIf
559           Ovrd MlstV 'MSebesseg 'Sebesség
560           Accel MGY,ML 'Gyorsulás
561           Mcage=Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
562           M2=Abs(ptemp2.A)
563             If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
564             If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
565           If (Mz>Mcage) Or (M2>(M_PI*2.0)) Then ' CAGE BOX
566             MHibaszam=912
567           Else ' CAGE BOX
568             ptemp0 = P_Curr 'a kiindulási pozició
569             GoSub *MOVE
570           EndIf ' CAGE BOX
571         EndIf ' MOVE? TURN?
572       EndIf 'Zero detect!!!
573     EndIf 'MFel?
574     GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
575   Return '=======================
576   '
577   '
578   '
579   *S110IRQ:
580     Act 2=0
581     'ioHS3=1
582     Cnt 1
583   *S110: '=======================
584     MGY = Mgyors/4
585     ML = Mlass/4
586   *S110CNT:
587     GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
588     If (MFel<>0) Then 'MFel?
589       MHibaszam=900
590     Else  'MFel?
591       If Dist(P_Zero,ptemp2) < 300 Then
592         MHibaszam=910 'Zero detect!!!
593       ElseIf Dist(ptemp1,ptemp2) > 0 Then
594         MHibaszam=911 'HS1 detect!!!
595       Else 'Zero detect!!!
596         'ABC Posture különbség
597         M2=Abs(Cos(P_Curr.A)-Cos(ptemp2.A))
598           If M2<Abs(Cos(P_Curr.B)-Cos(ptemp2.B)) Then M2=Abs(Cos(P_Curr.B)-Cos(ptemp2.B))
599           If M2<Abs(Cos(P_Curr.C)-Cos(ptemp2.C)) Then M2=Abs(Cos(P_Curr.C)-Cos(ptemp2.C))
600         'XYZ Távolság
601         M1=Dist(P_Curr,ptemp2)
602         MHibaszam=0
603         If (M1+M2)=0 Then  ' MOVE? TURN?
604           MHibaszam=0
605         Else ' MOVE? TURN?
606           If M_Svo=0 Then
607             If M_Run(3)=1 Then
608               Servo On 'szervo
609             EndIf ' R2D2.prg
610           EndIf ' M_Servo?
611           Fsc Off
612           M_Tool=0
613           ' Tehát M1 mm-ben meg van a távolság
614           Md = M1
615           MlstV=MSebesseg
616           'Me=(MPosS-MlstE) ' Mikorra kéne ott lenni
617           If MPosS>0 Then
618             ' megvan adva mennyi ido van rá
619             MPosS100=MPosS/100.0
620             MV=(MmxSPD*M_OPovrd)/100.0 'sebesség 150mm/sec * OP
621             M6=(Md/MV)*1000.0 'd/Mv = sec lenne max sebességben, felszorozzuk menyivel több ebben a sebeségen ?
622             MlstV2=M6/MPosS100
623             MlstV=MlstV2
624             If MlstV>MSebesseg Then
625                 MlstV=MSebesseg
626             ElseIf MlstV < 1 Then
627                 MlstV = 1
628             EndIf
629           EndIf
630           Ovrd MlstV 'MSebesseg 'Sebesség
631           Accel MGY,ML 'Gyorsulás
632           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
633           MELOZOTERULET=MTERULET
634           Cnt 0
635           ' CAGE check -----------------------------------------------
636             M2=Abs(ptemp2.A)
637             If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
638             If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
639           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
640           If (M1 > M3) Or (M2 > (M_PI*2.0)) Then ' CAGE BOX
641             MHibaszam=911
642           Else ' CAGE BOX
643             jtemp2 = PtoJ(ptemp2)
644             'old If (Abs(jtemp2.J4)<Rad(90)) And Abs(jtemp2.J5)<Rad(75.0)  Then ' Soft J4
645             M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
646             M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
647             'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
648             'egyszeru eltérést számol a középtol
649             If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Soft J4
650               Mvs ptemp2 'WthIf M_Ratio >= Mhs2rt, ioHS2 = 1
651               Cnt 0
652             Else ' Hard J4
653               jtemp0=J_Curr
654               ' FLIP? 4 ax
655               If M4>MNMX(4,4) Then
656                 If Deg(jtemp2.J4)>MNMX(4,3) Then
657                   jtemp2.J4=jtemp2.J4-M_PI
658                 Else
659                   jtemp2.J4=jtemp2.J4+M_PI
660                 EndIf
661                 ' FLIP 5 ax
662                 jtemp2.J5=jtemp2.J5*-1.0
663                 M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet így mar jó?
664                 ' FLIP 6 ax
665                 jtemp2.J6=jtemp2.J6*-1.0
666               EndIf
667               If M5>MNMX(5,4) Then
668                 MHibaszam=9115
669                 If Deg(jtemp2.J5)>MNMX(5,3) Then
670                   jtemp2.J5=M_PI*((MNMX(5,2)-Max5off)/180.0) 'maximum
671                 Else
672                   jtemp2.J5=M_PI*((MNMX(5,1)+Max5off)/180.0) 'minimum
673                 EndIf
674               EndIf
675               ' MIDLE jtemp1
676               jtemp1.J1 = (jtemp2.J1-J_Curr.J1)*Mmid + J_Curr.J1
677               jtemp1.J2 = (jtemp2.J2-J_Curr.J2)*Mmid + J_Curr.J2
678               jtemp1.J3 = (jtemp2.J3-J_Curr.J3)*Mmid + J_Curr.J3
679               jtemp1.J4 = (jtemp2.J4-J_Curr.J4)*Mmid + J_Curr.J4
680               jtemp1.J5 = (jtemp2.J5-J_Curr.J5)*Mmid + J_Curr.J5
681               jtemp1.J6 = (jtemp2.J6-J_Curr.J6)*Mmid + J_Curr.J6
682               Mov jtemp1 'WthIf M_Ratio >= Mhs2rt, ioHS2=1
683             EndIf ' Soft J4
684             'If ioHS3=1 Then
685             '  Cnt 1
686             '  GoSub *FEEDBACK
687             '  Wait ioHS3=0
688             '  GoSub *PXYZABCS
689             '  If ioCOM=11 Then
690             '    ioHS3=0
691             '    GoTo *S110CNT
692             '  EndIf
693             'EndIf
694           EndIf ' CAGE BOX?
695         EndIf ' MOVE? TURN?
696       EndIf  'Zero detect?
697     EndIf 'MFel?
698     GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
699 '    If (ioCOM=11) And ioHS1=1 And (M_Svo(1)=1) Then
700 '        MGY = 100
701 '        ML = 100
702 '        Cnt 1
703 '        GoTo *S110 'ha folytatásra érkezett parancs
704 '    EndIf
705    ioHS3=0
706    Cnt 0
707   Return '=======================
708   *S11 '=======================
709     '
710     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
711       'XYZ ABC joint
712       GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
713       ptemp1=P_Curr
714       ptemp1.X=(MPosX/100)
715       ptemp1.Y=(MPosY/100)
716       ptemp1.Z=(MPosZ/100)
717       If Dist(P_Zero,ptemp1) < 300 Then
718         MHibaszam=910 'Zero detect!!!
719       Else 'Zero detect!!!
720         ptemp1.A=Rad(MPosA/100)
721         ptemp1.B=Rad(MPosB/100)
722         ptemp1.C=Rad(MPosC/100)
723         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
724         '
725         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
726           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
727           ' akkor nekem nem kell alahelyzet felvétel
728           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
729           If M_Svo=0 Then
730             If M_Run(3)=1 Then
731               Servo On 'szervo
732             EndIf ' R2D2.prg
733           EndIf ' M_Servo?
734           Fsc Off
735           M_Tool=0
736           Ovrd MSebesseg 'Sebesség
737           Accel Mgyors,Mlass 'Gyorsulás
738           '
739           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
740           Rem úgy gondolom bajos, lutri
741           ' Cnt 1
742           '
743           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
744           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
745           'GoSub *TERULETVALTAS
746           MELOZOTERULET=MTERULET
747           MHibaszam=0
748           '
749           Cnt 0
750           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
751           '
752           M2=Abs(ptemp1.A)
753              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
754              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
755           '
756           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
757           '
758           If M1 < M3 And M2 < (M_PI*2.0) Then
759             M1=PosCq(ptemp1)
760             M2=0
761             While (M1=0) And (M2<100)
762               Select M2
763                 Case 0 'talán rossz az f2
764                   jtemp1 = PtoJ(ptemp1)
765                   ptemp1 = JtoP(jtemp1)
766                   Dly 1.0 'a userdefined screenen lassam itt van
767                 Break
768                 Case 1 'kérjünk egy közbenso állapotot
769                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
770                   Dly 1.0 'a userdefined screenen lássam itt van
771                 Break
772                 Case 2 'talán rossz a min.f2
773                   jtemp1 = PtoJ(ptemp1)
774                   ptemp1 = JtoP(jtemp1)
775                   Dly 1.0 'a userdefined screenen lássam itt van
776                 Break
777                 Case 3 '
778                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
779                   M2=99
780                   Dly 1.0 'a userdefined screenen lássam itt van
781                 Break
782               End Select
783               M1=PosCq(ptemp1)
784               M2=M2+1
785               Dly 1.0 'a userdefined screenen lássam itt van
786               ' valami nem tetszett
787             WEnd
788             '
789             jtemp1 = PtoJ(ptemp1)
790             If Abs(jtemp1.J4) > (M_PI*2.0/4.0) Then
791               Mvs ptemp1 Type 1,0
792               MHibaszam=931
793             Else
794               Mvs ptemp1
795             EndIf
796             '
797             MFel=0 'Bármelyik parancs következhet
798             '
799           Else ' CAGE BOX
800             MHibaszam=911
801           EndIf ' CAGE BOX?
802         EndIf ' MOVE? Dist?
803       EndIf  'Zero detect?
804       GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
805     Else 'MFel?
806       MHibaszam=900
807     EndIf 'MFel?
808   Return '=======================
809   '=================================================================
810   '=================================================================
811   '*********************XYZ ABC lineáris erõre***********************
812   '=================================================================
813   '=================================================================
814   *S12 '=======================
815   '
816   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
817    'XYZ ABC lineáris erõre
818    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
819   '
820    Ovrd MSebesseg 'Sebesség
821    Accel Mgyors,Mlass 'Gyorsulás
822   '
823   Cnt 1
824   '
825   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
826    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
827    GoSub *TERULETVALTAS
828   MHibaszam=0
829   '
830   Cnt 0
831   '
832   ptemp1=P_Curr 'Pillanatnyi pozíció
833   '
834   If ((MPosX>100 And MPosX<300 And MPosY>100 And MPosY<300 And MPosZ>100 And MPosZ<300) And (MPosA>100 And MPosA<300 And MPosB>100 And MPosB<300 And MPosC>100 And MPosC<300)) Then
835    ptemp1.X=(MPosX/100)
836    ptemp1.Y=(MPosY/100)
837    ptemp1.Z=(MPosZ/100)
838    ptemp1.A=Rad(MPosA/100)
839    ptemp1.B=Rad(MPosB/100)
840    ptemp1.C=Rad(MPosC/100)
841    Fsc On, 1,1,1
842    FsCTrg 1,10,1
843    Mvs ptemp1
844    Dly 0.01
845    Fsc Off
846   '
847   MFel=0 'Bármelyik parancs következhet
848   '
849   Else
850   MHibaszam=910
851   EndIf
852   '
853    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
854   '
855   Else
856   MHibaszam=900
857   EndIf
858   Return '=======================
859   '=================================================================
860   '=================================================================
861   '********************XYZ ABC eltolás joint************************
862   '=================================================================
863   '=================================================================
864   *S20 '=======================
865   '
866   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
867    'XYZ ABC eltolás joint
868    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
869   '
870    Ovrd MSebesseg 'Sebesség
871    Accel Mgyors,Mlass 'Gyorsulás
872   '
873   Cnt 1
874   '
875   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
876    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
877    GoSub *TERULETVALTAS
878   MHibaszam=0
879   '
880   Cnt 0
881   '
882   ptemp1=P_Curr 'Pillanatnyi pozíció
883   '
884   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
885    ptemp1.X=ptemp1.X+(MPosXe/100)
886    ptemp1.Y=ptemp1.Y+(MPosYe/100)
887    ptemp1.Z=ptemp1.Z+(MPosZe/100)
888    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
889    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
890    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
891    Mov ptemp1
892    Dly 0.01
893   '
894   MFel=0 'Bármelyik parancs következhet
895   '
896   Else
897   MHibaszam=910
898   EndIf
899   '
900    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
901   '
902   Else
903   MHibaszam=900
904   EndIf
905   Return '=======================
906   '=================================================================
907   '=================================================================
908   '********************XYZ ABC eltolás lineáris*********************
909   '=================================================================
910   '=================================================================
911   *S21 '=======================
912   '
913   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
914    'XYZ ABC eltolás lineáris
915    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
916   '
917    Ovrd MSebesseg 'Sebesség
918    Accel Mgyors,Mlass 'Gyorsulás
919   '
920   Cnt 1
921   '
922   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
923    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
924    GoSub *TERULETVALTAS
925   MHibaszam=0
926   '
927   Cnt 0
928   '
929   ptemp1=P_Curr 'Pillanatnyi pozíció
930   '
931   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
932    ptemp1.X=ptemp1.X+(MPosXe/100)
933    ptemp1.Y=ptemp1.Y+(MPosYe/100)
934    ptemp1.Z=ptemp1.Z+(MPosZe/100)
935    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
936    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
937    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
938    Mvs ptemp1
939    Dly 0.01
940   '
941   MFel=0 'Bármelyik parancs következhet
942   '
943   Else
944   MHibaszam=910
945   EndIf
946   '
947    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
948   '
949   Else
950   MHibaszam=900
951   EndIf
952   Return '=======================
953   '=================================================================
954   '=================================================================
955   '*****************XYZ ABC eltolás lineáris erõre******************
956   '=================================================================
957   '=================================================================
958   *S22 '=======================
959   '
960   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
961    'XYZ ABC eltolás lineáris erõre
962    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
963   '
964    Ovrd MSebesseg 'Sebesség
965    Accel Mgyors,Mlass 'Gyorsulás
966   '
967   Cnt 1
968   '
969   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
970    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
971    GoSub *TERULETVALTAS
972   MHibaszam=0
973   '
974   Cnt 0
975   '
976   ptemp1=P_Curr 'Pillanatnyi pozíció
977   '
978   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
979    ptemp1.X=ptemp1.X+(MPosXe/100)
980    ptemp1.Y=ptemp1.Y+(MPosYe/100)
981    ptemp1.Z=ptemp1.Z+(MPosZe/100)
982    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
983    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
984    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
985    Mvs ptemp1
986    Dly 0.01
987   '
988   MFel=0 'Bármelyik parancs következhet
989   '
990   Else
991   MHibaszam=910
992   EndIf
993   '
994    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
995   '
996   Else
997   MHibaszam=900
998   EndIf
999   Return '=======================
1000   '=================================================================
1001   '=================================================================
1002   '*******************************J1-J6*****************************
1003   '=================================================================
1004   '=================================================================
1005   *S30 '=======================
1006   '
1007   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
1008    'J1-J6
1009    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
1010   '
1011    Ovrd MSebesseg 'Sebesség
1012    Accel Mgyors,Mlass 'Gyorsulás
1013   '
1014   Cnt 1
1015   '
1016   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
1017    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
1018    GoSub *TERULETVALTAS
1019   MHibaszam=0
1020   '
1021   Cnt 0
1022   '
1023   jtemp1=J_Curr 'Pillanatnyi pozíció
1024   '
1025   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
1026    jtemp1.J1=Rad(MPosJ1/100)
1027    jtemp1.J2=Rad(MPosJ2/100)
1028    jtemp1.J3=Rad(MPosJ3/100)
1029    jtemp1.J4=Rad(MPosJ4/100)
1030    jtemp1.J5=Rad(MPosJ5/100)
1031    jtemp1.J6=Rad(MPosJ6/100)
1032    Mov jtemp1
1033    Dly 0.01
1034    '
1035    MFel=0 'Bármelyik parancs következhet
1036    '
1037   Else
1038   MHibaszam=910
1039   EndIf
1040   '
1041    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
1042   '
1043   Else
1044   MHibaszam=900
1045   EndIf
1046   Return '=======================
1047   '=================================================================
1048   '=================================================================
1049   '**************************J1-J6 eltolás**************************
1050   '=================================================================
1051   '=================================================================
1052   *S40 '=======================
1053   '
1054   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
1055    'J1-J6 eltolás
1056    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
1057   '
1058    Ovrd MSebesseg 'Sebesség
1059    Accel Mgyors,Mlass 'Gyorsulás
1060   '
1061   Cnt 1
1062   '
1063   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
1064    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
1065    GoSub *TERULETVALTAS
1066   MHibaszam=0
1067   '
1068   Cnt 0
1069   '
1070   jtemp1=J_Curr 'Pillanatnyi pozíció
1071   '
1072   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
1073    jtemp1.J1=jtemp1.J1+Rad(MPosJ1/100)
1074    jtemp1.J2=jtemp1.J2+Rad(MPosJ2/100)
1075    jtemp1.J3=jtemp1.J3+Rad(MPosJ3/100)
1076    jtemp1.J4=jtemp1.J4+Rad(MPosJ4/100)
1077    jtemp1.J5=jtemp1.J5+Rad(MPosJ5/100)
1078    jtemp1.J6=jtemp1.J6+Rad(MPosJ6/100)
1079    Mov jtemp1
1080    Dly 0.01
1081   '
1082   MFel=0 'Bármelyik parancs következhet
1083   '
1084   Else
1085   MHibaszam=910
1086   EndIf
1087   '
1088    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
1089   '
1090   Else
1091   MHibaszam=900
1092   EndIf
1093   Return '=======================
1094   '=================================================================
1095   '=================================================================
1096   '*****************************************************************
1097   '=================================================================
1098   '=================================================================
1099   '
1100   *FEEDBACK '=======================
1101     Mms = M_Timer(1)
1102     Ms=Mms/1000.0
1103     ioMsSLV = Mms
1104     ioMx=P_Curr.X*100
1105     ioMy=P_Curr.Y*100
1106     ioMz=P_Curr.Z*100
1107     ioMa=Deg(P_Curr.A)*100
1108     ioMb=Deg(P_Curr.B)*100
1109     ioMc=Deg(P_Curr.C)*100
1110   Return '=======================
1111   '
1112   *PXYZABCS '=======================
1113     ptemp2=P_Curr
1114     MPosX=ioMx 'X pozíció átvétele
1115     MPosY=ioMy 'Y pozíció átvétele
1116     MPosZ=ioMz 'Z pozíció átvétele
1117     MPosA=ioMa 'A pozíció fok átvétele
1118     MPosB=ioMb 'B pozíció fok átvétele
1119     MPosC=ioMc 'C pozíció fok átvétele
1120     MPosS=ioMs 'pozíció utazási ido átvétele
1121       ptemp2.X=(MPosX/100)
1122       ptemp2.Y=(MPosY/100)
1123       ptemp2.Z=(MPosZ/100)
1124       ptemp2.A=Rad(MPosA/100)
1125       ptemp2.B=Rad(MPosB/100)
1126       ptemp2.C=Rad(MPosC/100)
1127   Return '=======================
1128   '
1129   *HS1: '=======================
1130     Wait ioHS1=1 'wait RobotStartIn=1
1131     GoSub *PXYZABCS 'ptemp2 kitölti az io-ról
1132     ptemp1 = ptemp2 'csinálunk egy másolatot
1133     GoSub *FEEDBACK
1134     ioHS1=1 'ConfirmOut=1
1135     Wait ioHS1=0 'wait RobotStartIn=0
1136     Wait ioCOM=0 'Mozgás parancs nullázására vár
1137     GoSub *PXYZABCS 'ptemp2 hs1 után kitölti az io-t
1138     'így össze lehet hasonlítani, hogy elrontotta e valaki
1139     'ha nem stimmel hiba!!!
1140     ioHS1=0 'ConfirmOut=0
1141     M_00#=1 ' jelzes R2D2-nak hogy elkezdödött valami
1142   Return '=======================
1143   '
1144   *HS2: '=======================
1145     MPosS = 0
1146     If M_Svo(1)=0 Then
1147       ' Így nem nyírja ki miért lett leállítva
1148       If MHibaszam<1 Then
1149         MHibaszam=920
1150       EndIf
1151     EndIf
1152     ' Így az R2D2-nál is frissebb adatok lesznek mire olvasnám
1153     GoSub *FEEDBACK
1154     '
1155     ioCOM=MHibaszam 'Hiba státusz küldése
1156     ioHS2=1 'ReadyOut=1
1157     Wait ioHS2=1 'wait ConfirmIn=1
1158     ioCOM=0 'Hibaküldés kinullázása
1159     ioHS2=0 'ReadyOut=0
1160     Wait ioHS2=0 'wait ConfirmIn=0
1161     M_00#=0
1162   Return '=======================
1163   '
1164   *MOVE: ' bemenet a ( ptemp2
1165     jtemp2 = PtoJ(ptemp2)
1166     'old If (Abs(jtemp2.J4)<Rad(90)) And Abs(jtemp2.J5)<Rad(75.0)  Then ' Soft J4
1167     M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
1168     M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
1169     'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
1170     'egyszeru eltérést számol a középtol
1171     If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Hard J4
1172       ' nem ez Soft J4
1173       Mvs ptemp2
1174     Else ' Hard J4
1175       jtemp0=J_Curr
1176       ' FLIP? 4 ax
1177       If M4>MNMX(4,4) Then
1178         If Deg(jtemp2.J4)>MNMX(4,3) Then
1179           jtemp2.J4=jtemp2.J4-M_PI
1180         Else
1181           jtemp2.J4=jtemp2.J4+M_PI
1182         EndIf
1183         ' FLIP 5 ax
1184         jtemp2.J5=jtemp2.J5*-1.0
1185         M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet így mar jó?
1186         ' FLIP 6 ax
1187         jtemp2.J6=jtemp2.J6*-1.0
1188       EndIf
1189       If M5>MNMX(5,4) Then
1190         MHibaszam=9115
1191         If Deg(jtemp2.J5)>MNMX(5,3) Then
1192           jtemp2.J5=M_PI*((MNMX(5,2)-Max5off)/180.0) 'maximum
1193         Else
1194           jtemp2.J5=M_PI*((MNMX(5,1)+Max5off)/180.0) 'minimum
1195         EndIf
1196       EndIf
1197       ' MIDLE jtemp1
1198       jtemp1.J1 = (jtemp2.J1-J_Curr.J1)*Mmid + J_Curr.J1
1199       jtemp1.J2 = (jtemp2.J2-J_Curr.J2)*Mmid + J_Curr.J2
1200       jtemp1.J3 = (jtemp2.J3-J_Curr.J3)*Mmid + J_Curr.J3
1201       jtemp1.J4 = (jtemp2.J4-J_Curr.J4)*Mmid + J_Curr.J4
1202       jtemp1.J5 = (jtemp2.J5-J_Curr.J5)*Mmid + J_Curr.J5
1203       jtemp1.J6 = (jtemp2.J6-J_Curr.J6)*Mmid + J_Curr.J6
1204       Mov jtemp1 'WthIf M_Ratio >= Mhs2rt, ioHS2=1
1205     EndIf ' Hard J4
1206   Return '=======================
1207   '
1208  '=================================================================
1209  '*************************************************************************
1210  '*************************************************************************
1211  '*************************************************************************
1212  '*************************************************************************
1213  '=================================================================
1214  *TERULETVALTAS '=======================
1215  If (MELOZOTERULET<>MTERULET) Then
1216  '................................
1217   If (MELOZOTERULET=10 And MTERULET=20) Then 'váltás alaphelyzetbõl szervizpozícióra
1218    Mov jAlaph
1219    Mov jSzerviz
1220  '
1221    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1222   EndIf
1223  '................................
1224   If (MELOZOTERULET=30 And MTERULET=20) Then 'váltás megfogó ürítésrõl szervizpozícióra
1225    Mov jMurites
1226    Mov jAlaph
1227    Mov jSzerviz
1228  '
1229    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1230   EndIf
1231  '................................
1232   If (MELOZOTERULET=40 And MTERULET=20) Then 'váltás felvételrõl szervizpozícióra
1233    Mov jFelvetel
1234    Mov jSzerviz
1235  '
1236    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1237   EndIf
1238  '................................
1239   If (MELOZOTERULET=50 And MTERULET=20) Then 'váltás kamerásról szervizpozícióra
1240    Mov jKamera
1241    Mov jAlaph
1242    Mov jSzerviz
1243  '
1244    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1245   EndIf
1246  '................................
1247   If (MELOZOTERULET=60 And MTERULET=20) Then 'váltás lerakásról szervizpozícióra
1248    Mov jLerakas
1249    Mov jAlaph
1250    Mov jSzerviz
1251  '
1252    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1253   EndIf
1254  '................................
1255   If (MELOZOTERULET=20 And MTERULET=10) Then 'váltás szervizpozícióról alaphelyzetre
1256    Mov jSzerviz
1257    Mov jAlaph
1258  '
1259    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1260   EndIf
1261  '................................
1262   If (MELOZOTERULET=30 And MTERULET=10) Then 'váltás megfogó ürítésrõl alaphelyzetre
1263    Mov jMurites
1264    Mov jAlaph
1265  '
1266    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1267   EndIf
1268  '................................
1269   If (MELOZOTERULET=40 And MTERULET=10) Then 'váltás felvételrõl alaphelyzetre
1270    Mov jFelvetel
1271    Mov jAlaph
1272  '
1273    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1274   EndIf
1275  '................................
1276   If (MELOZOTERULET=50 And MTERULET=10) Then 'váltás kamerásról alaphelyzetre
1277    Mov jKamera
1278     Mov jAlaph
1279   '
1280     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1281   EndIf
1282  '................................
1283   If (MELOZOTERULET=60 And MTERULET=10) Then 'váltás lerakásról alaphelyzetre
1284    Mov jLerakas
1285    Mov jAlaph
1286  '
1287    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1288   EndIf
1289  '................................
1290   If (MELOZOTERULET=10 And MTERULET=30) Then 'váltás alaphelyzetbõl megfogó ürítésre
1291    Mov jAlaph
1292    Mov jMurites
1293  '
1294    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1295   EndIf
1296  '................................
1297   If (MELOZOTERULET=20 And MTERULET=30) Then 'váltás szervizpozícióról megfogó ürítésre
1298   Mov jSzerviz
1299   Mov jAlaph
1300   Mov jMurites
1301  '
1302    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1303   EndIf
1304  '................................
1305   If (MELOZOTERULET=40 And MTERULET=30) Then 'váltás felvételrõl megfogó ürítésre
1306   Mov jFelvetel
1307   Mov jAlaph
1308   Mov jMurites
1309  '
1310    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1311   EndIf
1312  '................................
1313   If (MELOZOTERULET=50 And MTERULET=30) Then 'váltás kamerásról megfogó ürítésre
1314    Mov jKamera
1315    Mov jMurites
1316  '
1317    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1318   EndIf
1319  '................................
1320   If (MELOZOTERULET=60 And MTERULET=30) Then 'váltás lerakásról megfogó ürítésre
1321    Mov jLerakas
1322    Mov jMurites
1323  '
1324    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1325   EndIf
1326  '................................
1327   If (MELOZOTERULET=10 And MTERULET=40) Then 'váltás alaphelyzetbõl felvételre
1328    Mov jAlaph
1329    Mov jFelvetel
1330  '
1331    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1332   EndIf
1333  '................................
1334   If (MELOZOTERULET=20 And MTERULET=40) Then 'váltás szervizpozícióról felvételre
1335    Mov jSzerviz
1336    Mov jFelvetel
1337  '
1338    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1339   EndIf
1340  '................................
1341   If (MELOZOTERULET=30 And MTERULET=40) Then 'váltás megfogó ürítésrõl felvételre
1342    Mov jMurites
1343    Mov jAlaph
1344    Mov jFelvetel
1345  '
1346    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1347   EndIf
1348  '................................
1349   If (MELOZOTERULET=50 And MTERULET=40) Then 'váltás kamerásról felvételre
1350    Mov jKamera
1351    Mov jFelvetel
1352  '
1353    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1354   EndIf
1355  '................................
1356   If (MELOZOTERULET=60 And MTERULET=40) Then 'váltás lerakásról felvételre
1357    Mov jLerakas
1358    Mov jAlaph
1359    Mov jFelvetel
1360  '
1361    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1362   EndIf
1363  '................................
1364   If (MELOZOTERULET=10 And MTERULET=50) Then 'váltás alaphelyzetbõl kamerásra
1365    Mov jAlaph
1366    Mov jKamera
1367  '
1368    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1369   EndIf
1370  '................................
1371   If (MELOZOTERULET=20 And MTERULET=50) Then 'váltás szervizpozícióról kamerásra
1372    Mov jSzerviz
1373    Mov jAlaph
1374    Mov jKamera
1375  '
1376    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1377   EndIf
1378  '................................
1379   If (MELOZOTERULET=30 And MTERULET=50) Then 'váltás megfogó ürítésrõl kamerásra
1380    Mov jMurites
1381     Mov jKamera
1382   '
1383     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1384   EndIf
1385  '................................
1386   If (MELOZOTERULET=40 And MTERULET=50) Then 'váltás felvételrõl kamerásra
1387    Mov jFelvetel
1388    Mov jAlaph
1389    Mov jKamera
1390  '
1391    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1392   EndIf
1393  '................................
1394   If (MELOZOTERULET=60 And MTERULET=50) Then 'váltás lerakásról kamerásra
1395    Mov jLerakas
1396    Mov jKamera
1397  '
1398    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1399   EndIf
1400  '................................
1401   If (MELOZOTERULET=10 And MTERULET=60) Then 'váltás alaphelyzetbõl lerakásra
1402    Mov jAlaph
1403    Mov jLerakas
1404  '
1405    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1406   EndIf
1407 '................................
1408   If (MELOZOTERULET=20 And MTERULET=60) Then 'váltás szervizpozícióról lerakásra
1409    Mov jSzerviz
1410    Mov jAlaph
1411    Mov jLerakas
1412  '
1413    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1414   EndIf
1415  '................................
1416   If (MELOZOTERULET=30 And MTERULET=60) Then 'váltás megfogó ürítésrõl lerakásra
1417    Mov jMurites
1418    Mov jLerakas
1419  '
1420    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1421   EndIf
1422  '................................
1423   If (MELOZOTERULET=40 And MTERULET=60) Then 'váltás felvételrõl lerakásra
1424    Mov jFelvetel
1425    Mov jAlaph
1426    Mov jLerakas
1427  '
1428    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1429   EndIf
1430  '................................
1431   If (MELOZOTERULET=50 And MTERULET=60) Then 'váltás kamerásról lerakásra
1432    Mov jKamera
1433    Mov jLerakas
1434  '
1435    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1436   EndIf
1437  '................................
1438  EndIf
1439  Return '=======================
1440 '
1441 '
1442 'Kártya Inputok:
1443 '0 - Gyárilag Foglalt
1444 '1 -
1445 '2 -
1446 '3 -
1447 '4 -
1448 '5 -
1449 '6 -
1450 '7 -
1451 '8 -
1452 '9 -
1453 '10 -
1454 '11 -
1455 '12 -
1456 '13 -
1457 '14 -
1458 '15 -
1459 '
1460 'Kártya Outputok:
1461 '0 -
1462 '1 -
1463 '2 -
1464 '3 -
1465 '4 -
1466 '5 -
1467 '6 -
1468 '7 -
1469 '8 -
1470 '9 -
1471 '10 -
1472 '11 -
1473 '12 -
1474 '13 -
1475 '14 -
1476 '15 -
1477 '
1478 '
1479 'Tool-ok:
1480 '0=semmilyen tool
jAlaph=(2.600,-33.050,136.100,0.000,76.950,92.600)
jFelvetel=(2.600,-33.050,136.100,0.000,76.950,92.600)
jKamera=(2.600,-33.050,136.100,0.000,76.950,92.600)
jLerakas=(2.600,-33.050,136.100,0.000,76.950,92.600)
jMurites=(2.600,-33.050,136.100,0.000,76.950,92.600)
jSzerviz=(2.600,-33.050,136.100,0.000,76.950,92.600)
jtemp0=(59.560,30.300,135.010,67.300,-36.230,-42.660)
jtemp1=(54.820,28.930,130.380,-23.070,0.570,45.870)
jtemp2=(38.660,88.250,39.420,0.000,52.330,-179.930,0.000,0.000)
pMurites=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
ptemp0=(464.090,692.810,600.000,-180.000,0.000,38.590)(7,0)
ptemp1=(1000.000,800.000,0.000,-180.000,0.000,38.590)(7,0)
ptemp2=(1000.000,800.000,0.000,-180.000,0.000,38.590)(7,0)
pKamera(1)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(2)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(3)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(4)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
