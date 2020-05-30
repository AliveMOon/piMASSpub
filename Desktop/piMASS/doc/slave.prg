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
101    '                          3   21             1      23
102    Def FNMdMS(Md,MsMX)=1000.0*(Md/((MsMX*M_OPovrd)/100.0))
103    Def FNMdOVRD(Mms,Md,MsMX)=100.0*FNMdMS(Md,MsMX)/Mms
104    '                          54       3   21             1      234    5
105    'Def FNMdOVRD(Mms,Md,MsMX)=((1000.0*(Md/((MsMX*M_OPovrd)/100.0)))/Mms)*100.0
106    '
107    'Tool (100,100,100,100)
108    Mdarab=0 'Megfogó darab érzékelés tárolása
109    MHibaszam=0 'Hibaszám tárolása
110    MPosS=0 'Pozíció index tárolása
111    MPosX=0 'X pozíció átvétele
112    MPosY=0 'Y pozíció átvétele
113    MPosZ=0 'Z pozíció átvétele
114    MPosA=0 'A pozíció fok átvétele
115    MPosB=0 'B pozíció fok átvétele
116    MPosC=0 'C pozíció fok átvétele
117    MPosXe=0 'X pozíció eltolás átvétele
118    MPosYe=0 'Y pozíció eltolás átvétele
119    MPosZe=0 'Z pozíció eltolás átvétele
120    MPosAe=0 'A pozíció fok eltolás átvétele
121    MPosBe=0 'B pozíció fok eltolás átvétele
122    MPosCe=0 'C pozíció fok eltolás átvétele
123    MPosJ1=0 'J1 tengely fok átvétele
124    MPosJ2=0 'J2 tengely fok átvétele
125    MPosJ3=0 'J3 tengely fok átvétele
126    MPosJ4=0 'J4 tengely fok átvétele
127    MPosJ5=0 'J5 tengely fok átvétele
128    MPosJ6=0 'J6 tengely fok átvétele
129    MPosJ1e=0 'J1 tengely fok eltolás átvétele
130    MPosJ2e=0 'J2 tengely fok eltolás átvétele
131    MPosJ3e=0 'J3 tengely fok eltolás átvétele
132    MPosJ4e=0 'J4 tengely fok eltolás átvétele
133    MPosJ5e=0 'J5 tengely fok eltolás átvétele
134    MPosJ6e=0 'J6 tengely fok eltolás átvétele
135    'MHibaszám jelentései:
136    '900 - Rossz parancs küldése a robotnak
137    '910 - Zero detect
138    '911 - HS1 elött és után nem stimmel a pozició
139    '912 - CAGE BOX detect error
140    '920 - Servo OFF
141    '930 - Range problem *S10
142    '931 - Range problem *S11
143    '999 - Alaphelyzetfelvételi hiba
144    '
145    MFel=0 'Ez a változó, ha nulla, akkor bármilyen parancs jöhet
146    MELOZOTERULET=0
147    MTERULET=0
148    'MTERULET jelentései (milyen pozícióba képes menni):
149    '10 - alaphelyzet pozíció jog
150    '20 - szerviz pozício jog
151    '30 - amegfogó ürítése jog
152    '
153    mXtemp=0 'alaphelyzet pozíció kereséshez
154    mYtemp=0 'alaphelyzet pozíció kereséshez
155    mZtemp=0 'alaphelyzet pozíció kereséshez
156    'ptemp0=0 pozíció segéd alaphelyzethez
157    'ptemp1=0 'pozíció segéd folyamathoz
158    'jtemp1=0 'pozíció segéd forgatáshoz
159    MTavol=0 'alaphelyzetbe értünk-e már?
160    '===================================================
161    '===================================================
162    '---------------------------------------------------
163    'Helyi változók felsorolás vége------------------
164    '---------------------------------------------------
165    '
166    'Hiba esetén megszakítás generálódik
167   '
168   While (M_Err=0)
169     Select ioCOM 'Mozgás végrehajtás parancs
170       Case 0 'nope
171         Mms = M_Timer(1)
172         Ms=Mms/1000.0
173         ioMsSLV = Mms
174         Dly 0.001
175         Break
176       ' DANGER!! nincsen megálapodás róla IGNORE küldjön hibát
177       'Case 1 'Alaphelyzet felvétel ES után
178       '  GoSub *S1
179       'Break
180       'Case 2 'Alaphelyzet felvétel CIKLUS
181       '  GoSub *S2
182       'Break
183       'Case 3 'Szerviz pozíció felvétel
184       '  GoSub *S3
185       'Break
186       'Case 4 'Megfogó kiürítése
187       '  GoSub *S4
188       'Break
189       Case 10 'XYZ ABC joint
190         GoSub *S10
191         Break
192       Case 11 'XYZ ABC lineáris
193         GoSub *S111
194         Break
195       ' DANGER!! még nem nem tudtam foglalkozni vele IGNORE küldjön hibát
196       'Case 12 'XYZ ABC lineáris erõre
197       '  GoSub *S12
198       'Break
199       'Case 20 'XYZ ABC eltolás joint
200       '  GoSub *S20
201       'Break
202       'Case 21 'XYZ ABC eltolás lineáris
203       '  GoSub *S21
204       'Break
205       'Case 22 'XYZ ABC eltolás lineáris erõre
206       '  GoSub *S22
207       'Break
208       'Case 30 'J1-J6
209       '  GoSub *S30
210       'Break
211       'Case 40 'J1-J6 eltolás
212       '  GoSub *S40
213       'Break
214       Default
215         'IGNORE küldjön hibát
216         GoSub *HS1
217         MHibaszam=900
218         GoSub *HS2
219         Break
220     End Select
221   WEnd
222   End
223   '
224   '
225   '=================================================================
226   '---------------------------------
227   'Szubrutinok következnek-----
228   '---------------------------------
229   '=================================================================
230   *S1 '=======================
231   'Alaphelyzet
232    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
233    'Inicializációs rész
234    Fsc Off
235    Servo On 'szervo
236    Ovrd 5 'Sebesség
237    Accel 5,5 'Gyorsulás, lassulás
238    M_Tool=0
239   MHibaszam=0 'Hiba nullázás
240   Mdarab=0
241   '=================================================================
242   '=================================================================
243   'Mozgás: Kamerás ellenõrzõ pozíciótól
244    mXtemp = Deg(J_Curr.J4)
245    mYtemp = Deg(J_Curr.J1)
246    mZtemp = Deg(J_Curr.J2)
247   If ((mXtemp>60 And mYtemp<15) Or (mXtemp>60 And mZtemp<-20)) Then
248   'Kamerás pozícióban áll a robot!
249   Cnt 0
250   Mov jKamera
251     Dly 0.01
252   Mov jAlaph
253     Dly 0.01
254   EndIf
255   '=================================================================
256   '=================================================================
257   'Mozgás: Lerakó területrõl
258     ptemp0 = P_Curr 'Pillanatnyi pozíció
259   If (ptemp0.X > (250)) Then
260   'Lerakó pozícióban áll a fej!
261   Cnt 0
262   Dly 0.1
263     ptemp0.Z=520
264   Mvs ptemp0
265     Dly 0.01
266   Mov jLerakas
267     Dly 0.01
268   Mov jAlaph
269     Dly 0.01
270   EndIf
271   '=================================================================
272   '=================================================================
273   'Mozgás: Felvételi pozíciótól
274     ptemp0 = P_Curr 'Pillanatnyi pozíció
275   If ((ptemp0.X) < (100)) Then
276   'Felvételi pozícióban áll a fej!
277   Cnt 0
278     ptemp0.Z=520
279   Mvs ptemp0
280     Dly 0.01
281   Mov jFelvetel
282     Dly 0.01
283   Mov jAlaph
284     Dly 0.01
285   EndIf
286   '=================================================================
287   '=================================================================
288   ptemp0 = P_Curr 'Pillanatnyi pozíció
289   If ((ptemp0.X <= (250)) And  (ptemp0.X >= (100))) Then
290   Mov jAlaph
291   Dly 0.01
292   Else
293   MHibaszam=999
294   EndIf
295   '=================================================================
296   '=================================================================
297   'Lekérdezés, hogy Alaph-ben vagyunk
298    mXtemp = Deg(J_Curr.J1)
299    mYtemp = Deg(jAlaph.J1)
300    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
301     Dly 0.1
302    WEnd
303    mXtemp = Deg(J_Curr.J2)
304    mYtemp = Deg(jAlaph.J2)
305    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
306     Dly 0.1
307    WEnd
308    mXtemp = Deg(J_Curr.J3)
309    mYtemp = Deg(jAlaph.J3)
310    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
311     Dly 0.1
312    WEnd
313    mXtemp = Deg(J_Curr.J4)
314    mYtemp = Deg(jAlaph.J4)
315    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
316     Dly 0.1
317    WEnd
318    mXtemp = Deg(J_Curr.J5)
319    mYtemp = Deg(jAlaph.J5)
320    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
321     Dly 0.1
322    WEnd
323    mXtemp = Deg(J_Curr.J6)
324    mYtemp = Deg(jAlaph.J6)
325    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
326     Dly 0.1
327    WEnd
328   '
329   'Belsõ változók beállítása
330    MELOZOTERULET=10
331    MTERULET=10
332    MFel=0
333   '
334    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
335   Return '=======================
336   '=================================================================
337   '*************************************************************************
338   '*************************************************************************
339   '=================================================================
340   *S2 '=======================
341   If (MFel=0) Then
342   'Alaphelyzet
343    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
344    'Inicializációs rész
345     Ovrd MSebesseg 'Sebesség
346    Accel Mgyors,Mlass 'Gyorsulás
347   '
348    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
349   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
350    MTERULET=10 'amelyik terulethez kell állítani a tengelyeket
351    GoSub *TERULETVALTAS
352    Mov jAlaph
353   Dly 0.01
354   MHibaszam=0 'Hiba nullázás
355   '
356    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
357   EndIf
358   Return '=======================
359   '=================================================================
360   '*************************************************************************
361   '*************************************************************************
362   '=================================================================
363   *S3 '=======================
364   If (MFel=0) Then
365   'Szerviz pozíció felvétele
366    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
367    'Inicializációs rész
368    Ovrd 10 'Sebesség
369    Accel 10,10 'Gyorsulás
370   '
371    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
372   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
373    MTERULET=20 'amelyik terulethez kell állítani a tengelyeket
374    GoSub *TERULETVALTAS
375    Mov jSzerviz
376     Dly 0.01
377    MFel=0
378   '
379    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
380   EndIf
381   Return '=======================
382   '=================================================================
383   '*************************************************************************
384   '*************************************************************************
385   '=================================================================
386   *S4 '=======================
387   If (MFel=0) Then
388   'Megfogó kiürítése
389    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
390    'Inicializációs rész
391     Ovrd MSebesseg 'Sebesség
392    Accel Mgyors,Mlass 'Gyorsulás
393   '
394    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
395   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
396    MTERULET=30 'amelyik terulethez kell állítani a tengelyeket
397    GoSub *TERULETVALTAS
398    Mov jMurites
399     Dly 0.001
400   Cnt 0
401   Mvs pMurites
402     Dly 0.01
403   '
404    Mdarab=0
405    MFel=0
406   '
407    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
408   EndIf
409   Return '=======================
410   '=================================================================
411   '=================================================================
412   '**************************XYZ ABC joint**************************
413   '=================================================================
414   '=================================================================
415   *S10 '=======================
416     '
417     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
418       'XYZ ABC joint
419       GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
420       ptemp1=P_Curr
421       ptemp1.X=(MPosX/100)
422       ptemp1.Y=(MPosY/100)
423       ptemp1.Z=(MPosZ/100)
424       If Dist(P_Zero,ptemp1) < 300 Then
425         MHibaszam=910 'Zero detect!!!
426       Else 'Zero detect!!!
427         ptemp1.A=Rad(MPosA/100)
428         ptemp1.B=Rad(MPosB/100)
429         ptemp1.C=Rad(MPosC/100)
430         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
431         '
432         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
433           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
434           ' akkor nekem nem kell alahelyzet felvétel
435           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
436           If M_Svo=0 Then
437             If M_Run(3)=1 Then
438               Servo On 'szervo
439             EndIf ' R2D2.prg
440           EndIf ' M_Servo?
441           Fsc Off
442           M_Tool=0
443           Ovrd MSebesseg 'Sebesség
444           Accel Mgyors,Mlass 'Gyorsulás
445           '
446           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
447           Rem úgy gondolom bajos, lutri
448           ' Cnt 1
449           '
450           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
451           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
452           'GoSub *TERULETVALTAS
453           MELOZOTERULET=MTERULET
454           MHibaszam=0
455           '
456           Cnt 0
457           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
458           '
459           M2=Abs(ptemp1.A)
460              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
461              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
462           '
463           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
464           '
465           'If ((MPosX>-100000& And MPosX<100000& And MPosY>-100000& And MPosY<100000& And MPosZ>-30000 And MPosZ<150000&) And (MPosA>-36000& And MPosA<36000& And MPosB>-36000& And MPosB<36000& And MPosC>-36000& And MPosC<36000&)) Then
466           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
467           If M1 < M3 And M2 < (M_PI*2.0) Then
468             M1=PosCq(ptemp1)
469             M2=0
470             While (M1=0) And (M2<100)
471               Select M2
472                 Case 0 'talán rossz az f2
473                   jtemp1 = PtoJ(ptemp1)
474                   ptemp1 = JtoP(jtemp1)
475                   Dly 1.0 'a userdefined screenen lassam itt van
476                 Break
477                 Case 1 'kérjünk egy közbenso állapotot
478                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
479                   Dly 1.0 'a userdefined screenen lassam itt van
480                 Break
481                 Case 2 'talán rossz a min.f2
482                   jtemp1 = PtoJ(ptemp1)
483                   ptemp1 = JtoP(jtemp1)
484                   Dly 1.0 'a userdefined screenen lassam itt van
485                 Break
486                 Case 3 '
487                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
488                   M2=99
489                   Dly 4.0 'a userdefined screenen lassam itt van
490                 Break
491               End Select
492               M1 = PosCq(ptemp1)
493               M2=M2+1
494               Dly 1.0 'a userdefined screenen lassam itt van
495               ' valami nem tetszett
496             WEnd
497             M1 = PosCq(ptemp1)
498             If M1=1 Then
499               Mov ptemp1
500             Else
501               'küldjön akkor is hibát ha netán sikerül mozogni
502               'ezeket fel akarom jegyezni
503               MHibaszam=931
504               Dly 10.0 'a userdefined screenen lassam itt van
505               ' valami nem tetszett
506               ' esetleg le löjem, vagy sem? 10 másodperc van rá
507               Mov ptemp1
508             EndIf
509             '
510             MFel=0 'Bármelyik parancs következhet
511             '
512           Else ' CAGE BOX
513             MHibaszam=911
514           EndIf ' CAGE BOX?
515         EndIf ' MOVE? Dist?
516       EndIf  'Zero detect?
517       GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
518     Else 'MFel?
519       MHibaszam=900
520     EndIf 'MFel?
521   Return '=======================
522   '=================================================================
523   '=================================================================
524   '************************XYZ ABC lineáris**************************
525   '=================================================================
526   '=================================================================
527   *S111:
528     MGY = Mgyors/4
529     ML = Mlass/4
530     GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
531     MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
532     MELOZOTERULET=MTERULET
533     If (MFel<>0) Then 'MFel?
534       MHibaszam=900
535     Else  'MFel?
536       Mz = Dist(P_Zero,ptemp2)
537       If Mz < Mzero Then
538         MHibaszam=910 'Zero detect!!!
539       ElseIf Dist(ptemp1,ptemp2) > 0 Then
540         MHibaszam=911 'HS1 detect!!!
541       Else 'Zero detect!!!
542         'ptemp1,ptemp2 azonos
543         Md=Dist(P_Curr,ptemp2)
544         Ma=FNMabc(P_Curr,ptemp2)
545         If (Ma+Md)=0 Then  ' MOVE? TURN?
546           MHibaszam=0
547         Else ' MOVE? TURN?
548           If M_Svo=0 Then
549             If M_Run(3)=1 Then
550               Servo On 'szervo
551             EndIf ' R2D2.prg
552           EndIf ' M_Servo?
553           Fsc Off
554           M_Tool=0
555           ' speed
556           MlstV=100
557           If Md>0 Then
558             If MPosS > 0 Then
559               MlstV=FNMdOVRD(MPosS,Md,MmxSPD)
560               MlstV=Max(1,MlstV)
561               MlstV=Min(100,MlstV)
562             Else
563               MPosS=FNMdMS(Md,MmxSPD)
564             EndIf
565           Else
566             MPosS=FNMdMS(100.0,MmxSPD)
567           EndIf
568           Ovrd MlstV 'MSebesseg 'Sebesség
569           Accel MGY,ML 'Gyorsulás
570           Mcage=Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
571           M2=Abs(ptemp2.A)
572             If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
573             If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
574           If (Mz>Mcage) Or (M2>(M_PI*2.0)) Then ' CAGE BOX
575             MHibaszam=912
576           Else ' CAGE BOX
577             MHibaszam=0
578             ptemp0 = P_Curr 'a kiindulási pozició
579             Mti = 0.0
580             Mts = MPosS/4.0
581             If Mts > 250.0 Then ' Mts
582                 While (Mti<MPosS) And (ioHS1=0)
583                     Cnt 1
584                     Mti=Mti+Mts
585                     jtemp0 = PtoJ(ptemp0)
586                     jtemp1 = PtoJ(ptemp1)
587                     jtemp2.J1 = (((jtemp1.J1-jtemp0.J1)*Mti)/MPosS)+jtemp0.J1
588                     jtemp2.J2 = (((jtemp1.J2-jtemp0.J2)*Mti)/MPosS)+jtemp0.J2
589                     jtemp2.J3 = (((jtemp1.J3-jtemp0.J3)*Mti)/MPosS)+jtemp0.J3
590                     jtemp2.J4 = (((jtemp1.J4-jtemp0.J4)*Mti)/MPosS)+jtemp0.J4
591                     jtemp2.J5 = (((jtemp1.J5-jtemp0.J5)*Mti)/MPosS)+jtemp0.J5
592                     jtemp2.J6 = (((jtemp1.J6-jtemp0.J6)*Mti)/MPosS)+jtemp0.J6
593                     ptemp2=JtoP(jtemp2)
594                     GoSub *MOVEt2
595                     GoSub *FEEDBACK
596                     If ioHS1=1 Then
597                       GoTo *S111
598                     EndIf
599                 WEnd
600                 ptemp2 = ptemp1
601             EndIf ' Mts > 250.0
602             GoSub *MOVEt2
603           EndIf ' CAGE BOX
604         EndIf ' MOVE? TURN?
605       EndIf 'Zero detect!!!
606     EndIf 'MFel?
607     Cnt 0
608     GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
609   Return '=======================
610   *S11:
611     MGY = Mgyors/4
612     ML = Mlass/4
613     GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
614     MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
615     MELOZOTERULET=MTERULET
616     If (MFel<>0) Then 'MFel?
617       MHibaszam=900
618     Else  'MFel?
619       Mz = Dist(P_Zero,ptemp2)
620       If Mz < Mzero Then
621         MHibaszam=910 'Zero detect!!!
622       ElseIf Dist(ptemp1,ptemp2) > 0 Then
623         MHibaszam=911 'HS1 detect!!!
624       Else 'Zero detect!!!
625         'ptemp1,ptemp2 azonos
626         Md=Dist(P_Curr,ptemp2)
627         Ma=FNMabc(P_Curr,ptemp2)
628         If (Ma+Md)=0 Then  ' MOVE? TURN?
629           MHibaszam=0
630         Else ' MOVE? TURN?
631           If M_Svo=0 Then
632             If M_Run(3)=1 Then
633               Servo On 'szervo
634             EndIf ' R2D2.prg
635           EndIf ' M_Servo?
636           Fsc Off
637           M_Tool=0
638           ' speed
639           MlstV=100
640           If Md>0 And MPosS > 0 Then
641             MlstV=FNMdOVRD(MPosS,Md,MmxSPD)
642             MlstV=Max(1,MlstV)
643             MlstV=Min(100,MlstV)
644           EndIf
645           Ovrd MlstV 'MSebesseg 'Sebesség
646           Accel MGY,ML 'Gyorsulás
647           Mcage=Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
648           M2=Abs(ptemp2.A)
649             If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
650             If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
651           If (Mz>Mcage) Or (M2>(M_PI*2.0)) Then ' CAGE BOX
652             MHibaszam=912
653           Else ' CAGE BOX
654             ptemp0 = P_Curr 'a kiindulási pozició
655             GoSub *MOVEt2
656           EndIf ' CAGE BOX
657         EndIf ' MOVE? TURN?
658       EndIf 'Zero detect!!!
659     EndIf 'MFel?
660     GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
661   Return '=======================
662   '=================================================================
663   '=================================================================
664   '*********************XYZ ABC lineáris erõre***********************
665   '=================================================================
666   '=================================================================
667   *S12 '=======================
668   '
669   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
670    'XYZ ABC lineáris erõre
671    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
672   '
673    Ovrd MSebesseg 'Sebesség
674    Accel Mgyors,Mlass 'Gyorsulás
675   '
676   Cnt 1
677   '
678   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
679    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
680    GoSub *TERULETVALTAS
681   MHibaszam=0
682   '
683   Cnt 0
684   '
685   ptemp1=P_Curr 'Pillanatnyi pozíció
686   '
687   If ((MPosX>100 And MPosX<300 And MPosY>100 And MPosY<300 And MPosZ>100 And MPosZ<300) And (MPosA>100 And MPosA<300 And MPosB>100 And MPosB<300 And MPosC>100 And MPosC<300)) Then
688    ptemp1.X=(MPosX/100)
689    ptemp1.Y=(MPosY/100)
690    ptemp1.Z=(MPosZ/100)
691    ptemp1.A=Rad(MPosA/100)
692    ptemp1.B=Rad(MPosB/100)
693    ptemp1.C=Rad(MPosC/100)
694    Fsc On, 1,1,1
695    FsCTrg 1,10,1
696    Mvs ptemp1
697    Dly 0.01
698    Fsc Off
699   '
700   MFel=0 'Bármelyik parancs következhet
701   '
702   Else
703   MHibaszam=910
704   EndIf
705   '
706    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
707   '
708   Else
709   MHibaszam=900
710   EndIf
711   Return '=======================
712   '=================================================================
713   '=================================================================
714   '********************XYZ ABC eltolás joint************************
715   '=================================================================
716   '=================================================================
717   *S20 '=======================
718   '
719   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
720    'XYZ ABC eltolás joint
721    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
722   '
723    Ovrd MSebesseg 'Sebesség
724    Accel Mgyors,Mlass 'Gyorsulás
725   '
726   Cnt 1
727   '
728   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
729    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
730    GoSub *TERULETVALTAS
731   MHibaszam=0
732   '
733   Cnt 0
734   '
735   ptemp1=P_Curr 'Pillanatnyi pozíció
736   '
737   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
738    ptemp1.X=ptemp1.X+(MPosXe/100)
739    ptemp1.Y=ptemp1.Y+(MPosYe/100)
740    ptemp1.Z=ptemp1.Z+(MPosZe/100)
741    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
742    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
743    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
744    Mov ptemp1
745    Dly 0.01
746   '
747   MFel=0 'Bármelyik parancs következhet
748   '
749   Else
750   MHibaszam=910
751   EndIf
752   '
753    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
754   '
755   Else
756   MHibaszam=900
757   EndIf
758   Return '=======================
759   '=================================================================
760   '=================================================================
761   '********************XYZ ABC eltolás lineáris*********************
762   '=================================================================
763   '=================================================================
764   *S21 '=======================
765   '
766   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
767    'XYZ ABC eltolás lineáris
768    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
769   '
770    Ovrd MSebesseg 'Sebesség
771    Accel Mgyors,Mlass 'Gyorsulás
772   '
773   Cnt 1
774   '
775   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
776    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
777    GoSub *TERULETVALTAS
778   MHibaszam=0
779   '
780   Cnt 0
781   '
782   ptemp1=P_Curr 'Pillanatnyi pozíció
783   '
784   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
785    ptemp1.X=ptemp1.X+(MPosXe/100)
786    ptemp1.Y=ptemp1.Y+(MPosYe/100)
787    ptemp1.Z=ptemp1.Z+(MPosZe/100)
788    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
789    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
790    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
791    Mvs ptemp1
792    Dly 0.01
793   '
794   MFel=0 'Bármelyik parancs következhet
795   '
796   Else
797   MHibaszam=910
798   EndIf
799   '
800    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
801   '
802   Else
803   MHibaszam=900
804   EndIf
805   Return '=======================
806   '=================================================================
807   '=================================================================
808   '*****************XYZ ABC eltolás lineáris erõre******************
809   '=================================================================
810   '=================================================================
811   *S22 '=======================
812   '
813   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
814    'XYZ ABC eltolás lineáris erõre
815    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
816   '
817    Ovrd MSebesseg 'Sebesség
818    Accel Mgyors,Mlass 'Gyorsulás
819   '
820   Cnt 1
821   '
822   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
823    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
824    GoSub *TERULETVALTAS
825   MHibaszam=0
826   '
827   Cnt 0
828   '
829   ptemp1=P_Curr 'Pillanatnyi pozíció
830   '
831   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
832    ptemp1.X=ptemp1.X+(MPosXe/100)
833    ptemp1.Y=ptemp1.Y+(MPosYe/100)
834    ptemp1.Z=ptemp1.Z+(MPosZe/100)
835    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
836    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
837    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
838    Mvs ptemp1
839    Dly 0.01
840   '
841   MFel=0 'Bármelyik parancs következhet
842   '
843   Else
844   MHibaszam=910
845   EndIf
846   '
847    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
848   '
849   Else
850   MHibaszam=900
851   EndIf
852   Return '=======================
853   '=================================================================
854   '=================================================================
855   '*******************************J1-J6*****************************
856   '=================================================================
857   '=================================================================
858   *S30 '=======================
859   '
860   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
861    'J1-J6
862    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
863   '
864    Ovrd MSebesseg 'Sebesség
865    Accel Mgyors,Mlass 'Gyorsulás
866   '
867   Cnt 1
868   '
869   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
870    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
871    GoSub *TERULETVALTAS
872   MHibaszam=0
873   '
874   Cnt 0
875   '
876   jtemp1=J_Curr 'Pillanatnyi pozíció
877   '
878   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
879    jtemp1.J1=Rad(MPosJ1/100)
880    jtemp1.J2=Rad(MPosJ2/100)
881    jtemp1.J3=Rad(MPosJ3/100)
882    jtemp1.J4=Rad(MPosJ4/100)
883    jtemp1.J5=Rad(MPosJ5/100)
884    jtemp1.J6=Rad(MPosJ6/100)
885    Mov jtemp1
886    Dly 0.01
887    '
888    MFel=0 'Bármelyik parancs következhet
889    '
890   Else
891   MHibaszam=910
892   EndIf
893   '
894    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
895   '
896   Else
897   MHibaszam=900
898   EndIf
899   Return '=======================
900   '=================================================================
901   '=================================================================
902   '**************************J1-J6 eltolás**************************
903   '=================================================================
904   '=================================================================
905   *S40 '=======================
906   '
907   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
908    'J1-J6 eltolás
909    GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
910   '
911    Ovrd MSebesseg 'Sebesség
912    Accel Mgyors,Mlass 'Gyorsulás
913   '
914   Cnt 1
915   '
916   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
917    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
918    GoSub *TERULETVALTAS
919   MHibaszam=0
920   '
921   Cnt 0
922   '
923   jtemp1=J_Curr 'Pillanatnyi pozíció
924   '
925   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
926    jtemp1.J1=jtemp1.J1+Rad(MPosJ1/100)
927    jtemp1.J2=jtemp1.J2+Rad(MPosJ2/100)
928    jtemp1.J3=jtemp1.J3+Rad(MPosJ3/100)
929    jtemp1.J4=jtemp1.J4+Rad(MPosJ4/100)
930    jtemp1.J5=jtemp1.J5+Rad(MPosJ5/100)
931    jtemp1.J6=jtemp1.J6+Rad(MPosJ6/100)
932    Mov jtemp1
933    Dly 0.01
934   '
935   MFel=0 'Bármelyik parancs következhet
936   '
937   Else
938   MHibaszam=910
939   EndIf
940   '
941    GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
942   '
943   Else
944   MHibaszam=900
945   EndIf
946   Return '=======================
947   '=================================================================
948   '=================================================================
949   '*****************************************************************
950   '=================================================================
951   '=================================================================
952   '
953   *FEEDBACK '=======================
954     Mms = M_Timer(1)
955     Ms=Mms/1000.0
956     ioMsSLV = Mms
957     ioMx=P_Curr.X*100
958     ioMy=P_Curr.Y*100
959     ioMz=P_Curr.Z*100
960     ioMa=Deg(P_Curr.A)*100
961     ioMb=Deg(P_Curr.B)*100
962     ioMc=Deg(P_Curr.C)*100
963   Return '=======================
964   '
965   *PXYZABCS '=======================
966     ptemp2=P_Curr
967     MPosX=ioMx 'X pozíció átvétele
968     MPosY=ioMy 'Y pozíció átvétele
969     MPosZ=ioMz 'Z pozíció átvétele
970     MPosA=ioMa 'A pozíció fok átvétele
971     MPosB=ioMb 'B pozíció fok átvétele
972     MPosC=ioMc 'C pozíció fok átvétele
973     MPosS=ioMs 'pozíció utazási ido átvétele
974       ptemp2.X=(MPosX/100)
975       ptemp2.Y=(MPosY/100)
976       ptemp2.Z=(MPosZ/100)
977       ptemp2.A=Rad(MPosA/100)
978       ptemp2.B=Rad(MPosB/100)
979       ptemp2.C=Rad(MPosC/100)
980   Return '=======================
981   '
982   *HS1: '=======================
983     Wait ioHS1=1 'wait RobotStartIn=1
984     GoSub *PXYZABCS 'ptemp2 kitölti az io-ról
985     ptemp1 = ptemp2 'csinálunk egy másolatot
986     GoSub *FEEDBACK
987     ioHS1=1 'ConfirmOut=1
988     Wait ioHS1=0 'wait RobotStartIn=0
989     Wait ioCOM=0 'Mozgás parancs nullázására vár
990     GoSub *PXYZABCS 'ptemp2 hs1 után kitölti az io-t
991     'így össze lehet hasonlítani, hogy elrontotta e valaki
992     'ha nem stimmel hiba!!!
993     ioHS1=0 'ConfirmOut=0
994     M_00#=1 ' jelzes R2D2-nak hogy elkezdödött valami
995   Return '=======================
996   '
997   *HS2: '=======================
998     MPosS = 0
999     If M_Svo(1)=0 Then
1000       ' Így nem nyírja ki miért lett leállítva
1001       If MHibaszam<1 Then
1002         MHibaszam=920
1003       EndIf
1004     EndIf
1005     ' Így az R2D2-nál is frissebb adatok lesznek mire olvasnám
1006     GoSub *FEEDBACK
1007     '
1008     ioCOM=MHibaszam 'Hiba státusz küldése
1009     ioHS2=1 'ReadyOut=1
1010     Wait ioHS2=1 'wait ConfirmIn=1
1011     ioCOM=0 'Hibaküldés kinullázása
1012     ioHS2=0 'ReadyOut=0
1013     Wait ioHS2=0 'wait ConfirmIn=0
1014     M_00#=0
1015   Return '=======================
1016   '
1017   *MOVEt2: ' bemenet a ( ptemp2
1018     jtemp2 = PtoJ(ptemp2)
1019     'old If (Abs(jtemp2.J4)<Rad(90)) And Abs(jtemp2.J5)<Rad(75.0)  Then ' Soft J4
1020     M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
1021     M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
1022     'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
1023     'egyszeru eltérést számol a középtol
1024     If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Hard J4
1025       ' nem ez Soft J4
1026       Mvs ptemp2
1027     Else ' Hard J4
1028       GoSub *MOVEj2
1029     EndIf ' Hard J4
1030   Return '=======================
1031   '
1032   *MOVEj2: ' bemenet a ( jtemp2
1033     jtemp0=J_Curr
1034     ' FLIP? 4 ax
1035     If M4>MNMX(4,4) Then
1036       If Deg(jtemp2.J4)>MNMX(4,3) Then
1037         jtemp2.J4=jtemp2.J4-M_PI
1038       Else
1039         jtemp2.J4=jtemp2.J4+M_PI
1040       EndIf
1041       ' FLIP 5 ax
1042       jtemp2.J5=jtemp2.J5*-1.0
1043       M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet így mar jó?
1044       ' FLIP 6 ax
1045       jtemp2.J6=jtemp2.J6*-1.0
1046     EndIf
1047     If M5>MNMX(5,4) Then
1048       MHibaszam=9115
1049       If Deg(jtemp2.J5)>MNMX(5,3) Then
1050         jtemp2.J5=M_PI*((MNMX(5,2)-Max5off)/180.0) 'maximum
1051       Else
1052         jtemp2.J5=M_PI*((MNMX(5,1)+Max5off)/180.0) 'minimum
1053       EndIf
1054     EndIf
1055     ' MIDLE jtemp1
1056     jtemp1.J1 = (jtemp2.J1-J_Curr.J1)*Mmid + J_Curr.J1
1057     jtemp1.J2 = (jtemp2.J2-J_Curr.J2)*Mmid + J_Curr.J2
1058     jtemp1.J3 = (jtemp2.J3-J_Curr.J3)*Mmid + J_Curr.J3
1059     jtemp1.J4 = (jtemp2.J4-J_Curr.J4)*Mmid + J_Curr.J4
1060     jtemp1.J5 = (jtemp2.J5-J_Curr.J5)*Mmid + J_Curr.J5
1061     jtemp1.J6 = (jtemp2.J6-J_Curr.J6)*Mmid + J_Curr.J6
1062     Mov jtemp1 'WthIf M_Ratio >= Mhs2rt, ioHS2=1
1063   Return '=======================
1064   '
1065  '=================================================================
1066  '*************************************************************************
1067  '*************************************************************************
1068  '*************************************************************************
1069  '*************************************************************************
1070  '=================================================================
1071  *TERULETVALTAS '=======================
1072  If (MELOZOTERULET<>MTERULET) Then
1073  '................................
1074   If (MELOZOTERULET=10 And MTERULET=20) Then 'váltás alaphelyzetbõl szervizpozícióra
1075    Mov jAlaph
1076    Mov jSzerviz
1077  '
1078    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1079   EndIf
1080  '................................
1081   If (MELOZOTERULET=30 And MTERULET=20) Then 'váltás megfogó ürítésrõl szervizpozícióra
1082    Mov jMurites
1083    Mov jAlaph
1084    Mov jSzerviz
1085  '
1086    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1087   EndIf
1088  '................................
1089   If (MELOZOTERULET=40 And MTERULET=20) Then 'váltás felvételrõl szervizpozícióra
1090    Mov jFelvetel
1091    Mov jSzerviz
1092  '
1093    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1094   EndIf
1095  '................................
1096   If (MELOZOTERULET=50 And MTERULET=20) Then 'váltás kamerásról szervizpozícióra
1097    Mov jKamera
1098    Mov jAlaph
1099    Mov jSzerviz
1100  '
1101    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1102   EndIf
1103  '................................
1104   If (MELOZOTERULET=60 And MTERULET=20) Then 'váltás lerakásról szervizpozícióra
1105    Mov jLerakas
1106    Mov jAlaph
1107    Mov jSzerviz
1108  '
1109    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1110   EndIf
1111  '................................
1112   If (MELOZOTERULET=20 And MTERULET=10) Then 'váltás szervizpozícióról alaphelyzetre
1113    Mov jSzerviz
1114    Mov jAlaph
1115  '
1116    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1117   EndIf
1118  '................................
1119   If (MELOZOTERULET=30 And MTERULET=10) Then 'váltás megfogó ürítésrõl alaphelyzetre
1120    Mov jMurites
1121    Mov jAlaph
1122  '
1123    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1124   EndIf
1125  '................................
1126   If (MELOZOTERULET=40 And MTERULET=10) Then 'váltás felvételrõl alaphelyzetre
1127    Mov jFelvetel
1128    Mov jAlaph
1129  '
1130    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1131   EndIf
1132  '................................
1133   If (MELOZOTERULET=50 And MTERULET=10) Then 'váltás kamerásról alaphelyzetre
1134    Mov jKamera
1135     Mov jAlaph
1136   '
1137     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1138   EndIf
1139  '................................
1140   If (MELOZOTERULET=60 And MTERULET=10) Then 'váltás lerakásról alaphelyzetre
1141    Mov jLerakas
1142    Mov jAlaph
1143  '
1144    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1145   EndIf
1146  '................................
1147   If (MELOZOTERULET=10 And MTERULET=30) Then 'váltás alaphelyzetbõl megfogó ürítésre
1148    Mov jAlaph
1149    Mov jMurites
1150  '
1151    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1152   EndIf
1153  '................................
1154   If (MELOZOTERULET=20 And MTERULET=30) Then 'váltás szervizpozícióról megfogó ürítésre
1155   Mov jSzerviz
1156   Mov jAlaph
1157   Mov jMurites
1158  '
1159    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1160   EndIf
1161  '................................
1162   If (MELOZOTERULET=40 And MTERULET=30) Then 'váltás felvételrõl megfogó ürítésre
1163   Mov jFelvetel
1164   Mov jAlaph
1165   Mov jMurites
1166  '
1167    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1168   EndIf
1169  '................................
1170   If (MELOZOTERULET=50 And MTERULET=30) Then 'váltás kamerásról megfogó ürítésre
1171    Mov jKamera
1172    Mov jMurites
1173  '
1174    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1175   EndIf
1176  '................................
1177   If (MELOZOTERULET=60 And MTERULET=30) Then 'váltás lerakásról megfogó ürítésre
1178    Mov jLerakas
1179    Mov jMurites
1180  '
1181    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1182   EndIf
1183  '................................
1184   If (MELOZOTERULET=10 And MTERULET=40) Then 'váltás alaphelyzetbõl felvételre
1185    Mov jAlaph
1186    Mov jFelvetel
1187  '
1188    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1189   EndIf
1190  '................................
1191   If (MELOZOTERULET=20 And MTERULET=40) Then 'váltás szervizpozícióról felvételre
1192    Mov jSzerviz
1193    Mov jFelvetel
1194  '
1195    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1196   EndIf
1197  '................................
1198   If (MELOZOTERULET=30 And MTERULET=40) Then 'váltás megfogó ürítésrõl felvételre
1199    Mov jMurites
1200    Mov jAlaph
1201    Mov jFelvetel
1202  '
1203    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1204   EndIf
1205  '................................
1206   If (MELOZOTERULET=50 And MTERULET=40) Then 'váltás kamerásról felvételre
1207    Mov jKamera
1208    Mov jFelvetel
1209  '
1210    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1211   EndIf
1212  '................................
1213   If (MELOZOTERULET=60 And MTERULET=40) Then 'váltás lerakásról felvételre
1214    Mov jLerakas
1215    Mov jAlaph
1216    Mov jFelvetel
1217  '
1218    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1219   EndIf
1220  '................................
1221   If (MELOZOTERULET=10 And MTERULET=50) Then 'váltás alaphelyzetbõl kamerásra
1222    Mov jAlaph
1223    Mov jKamera
1224  '
1225    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1226   EndIf
1227  '................................
1228   If (MELOZOTERULET=20 And MTERULET=50) Then 'váltás szervizpozícióról kamerásra
1229    Mov jSzerviz
1230    Mov jAlaph
1231    Mov jKamera
1232  '
1233    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1234   EndIf
1235  '................................
1236   If (MELOZOTERULET=30 And MTERULET=50) Then 'váltás megfogó ürítésrõl kamerásra
1237    Mov jMurites
1238     Mov jKamera
1239   '
1240     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1241   EndIf
1242  '................................
1243   If (MELOZOTERULET=40 And MTERULET=50) Then 'váltás felvételrõl kamerásra
1244    Mov jFelvetel
1245    Mov jAlaph
1246    Mov jKamera
1247  '
1248    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1249   EndIf
1250  '................................
1251   If (MELOZOTERULET=60 And MTERULET=50) Then 'váltás lerakásról kamerásra
1252    Mov jLerakas
1253    Mov jKamera
1254  '
1255    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1256   EndIf
1257  '................................
1258   If (MELOZOTERULET=10 And MTERULET=60) Then 'váltás alaphelyzetbõl lerakásra
1259    Mov jAlaph
1260    Mov jLerakas
1261  '
1262    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1263   EndIf
1264 '................................
1265   If (MELOZOTERULET=20 And MTERULET=60) Then 'váltás szervizpozícióról lerakásra
1266    Mov jSzerviz
1267    Mov jAlaph
1268    Mov jLerakas
1269  '
1270    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1271   EndIf
1272  '................................
1273   If (MELOZOTERULET=30 And MTERULET=60) Then 'váltás megfogó ürítésrõl lerakásra
1274    Mov jMurites
1275    Mov jLerakas
1276  '
1277    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1278   EndIf
1279  '................................
1280   If (MELOZOTERULET=40 And MTERULET=60) Then 'váltás felvételrõl lerakásra
1281    Mov jFelvetel
1282    Mov jAlaph
1283    Mov jLerakas
1284  '
1285    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1286   EndIf
1287  '................................
1288   If (MELOZOTERULET=50 And MTERULET=60) Then 'váltás kamerásról lerakásra
1289    Mov jKamera
1290    Mov jLerakas
1291  '
1292    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1293   EndIf
1294  '................................
1295  EndIf
1296  Return '=======================
1297 '
1298 '
1299 'Kártya Inputok:
1300 '0 - Gyárilag Foglalt
1301 '1 -
1302 '2 -
1303 '3 -
1304 '4 -
1305 '5 -
1306 '6 -
1307 '7 -
1308 '8 -
1309 '9 -
1310 '10 -
1311 '11 -
1312 '12 -
1313 '13 -
1314 '14 -
1315 '15 -
1316 '
1317 'Kártya Outputok:
1318 '0 -
1319 '1 -
1320 '2 -
1321 '3 -
1322 '4 -
1323 '5 -
1324 '6 -
1325 '7 -
1326 '8 -
1327 '9 -
1328 '10 -
1329 '11 -
1330 '12 -
1331 '13 -
1332 '14 -
1333 '15 -
1334 '
1335 '
1336 'Tool-ok:
1337 '0=semmilyen tool
jAlaph=(2.600,-33.050,136.100,0.000,76.950,92.600)
jFelvetel=(2.600,-33.050,136.100,0.000,76.950,92.600)
jKamera=(2.600,-33.050,136.100,0.000,76.950,92.600)
jLerakas=(2.600,-33.050,136.100,0.000,76.950,92.600)
jMurites=(2.600,-33.050,136.100,0.000,76.950,92.600)
jSzerviz=(2.600,-33.050,136.100,0.000,76.950,92.600)
jtemp0=(56.180,8.640,113.460,0.000,57.900,146.190)
jtemp1=(90.000,13.160,142.710,0.000,24.130,-179.990)
jtemp2=(90.000,13.160,142.710,0.000,24.130,-179.990,0.000,0.000)
pMurites=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
ptemp0=(464.100,692.820,600.000,-180.000,0.000,89.990)(7,0)
ptemp1=(0.000,600.000,300.000,-180.000,0.000,89.990)(7,0)
ptemp2=(0.000,600.000,300.000,-180.000,0.000,89.990)(7,0)
pKamera(1)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(2)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(3)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(4)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
