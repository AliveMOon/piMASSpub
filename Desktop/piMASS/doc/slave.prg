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
58    ioMsSLV=0
59    Rem -  -- ---=--==-=== MAJRE BUTTON ===-==--=--- --  -
60    ' EZT NEM SZABAD MÁSHOL VÁLTOZTATNI akkor konstast kell belölle csinálni
61    ' és akkor nem is lehet valahogy így:
62    Const MSebesseg=100 'Robot sebesség EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 100
63    Const MutazasiMagassag=740 'Utazási magasság a fejnek EZT NEM SZABAD MÁSHOL VÁLTOZTATNI
64    Const Mgyors=80 'Robot gyorsulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
65    Const Mlass=80 'Robot lassulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
66    Const Mhs2rt=87
67    Const Max5off=3.0
68    Const Mmid=0.66
69    '
70    MSebPakolas=100 'Felvételi, lerakási sebesség!!!
71    MlstV=MSebesseg
72    MGyorsPakolas=100 'Felvételi, lerakási gyorsulás!!!
73    MLassPakolas=100 'Felvételi, lerakási lassulás!!!
74    Mi1=0 'Minden For ciklusnak
75    Mi2=0 'Lefoglalva a Getparam fv-nek
76    Dim MIp(4) 'bemenetek helyi változója
77    Dim MQp(4) 'kimenetek helyi változója
78    '
79    Dim pKamera(4) 'Kamera pozíció a 4 típusra
80    '
81    'Tool (100,100,100,100)
82    Mdarab=0 'Megfogó darab érzékelés tárolása
83    MHibaszam=0 'Hibaszám tárolása
84    MPosS=0 'Pozíció index tárolása
85    MPosX=0 'X pozíció átvétele
86    MPosY=0 'Y pozíció átvétele
87    MPosZ=0 'Z pozíció átvétele
88    MPosA=0 'A pozíció fok átvétele
89    MPosB=0 'B pozíció fok átvétele
90    MPosC=0 'C pozíció fok átvétele
91    MPosXe=0 'X pozíció eltolás átvétele
92    MPosYe=0 'Y pozíció eltolás átvétele
93    MPosZe=0 'Z pozíció eltolás átvétele
94    MPosAe=0 'A pozíció fok eltolás átvétele
95    MPosBe=0 'B pozíció fok eltolás átvétele
96    MPosCe=0 'C pozíció fok eltolás átvétele
97    MPosJ1=0 'J1 tengely fok átvétele
98    MPosJ2=0 'J2 tengely fok átvétele
99    MPosJ3=0 'J3 tengely fok átvétele
100    MPosJ4=0 'J4 tengely fok átvétele
101    MPosJ5=0 'J5 tengely fok átvétele
102    MPosJ6=0 'J6 tengely fok átvétele
103    MPosJ1e=0 'J1 tengely fok eltolás átvétele
104    MPosJ2e=0 'J2 tengely fok eltolás átvétele
105    MPosJ3e=0 'J3 tengely fok eltolás átvétele
106    MPosJ4e=0 'J4 tengely fok eltolás átvétele
107    MPosJ5e=0 'J5 tengely fok eltolás átvétele
108    MPosJ6e=0 'J6 tengely fok eltolás átvétele
109    'MHibaszám jelentései:
110    '900 - Rossz parancs küldése a robotnak
111    '910 - Zero detect
112    '911 - HS1 elött és után nem stimmel a pozició
113    '920 - Servo OFF
114    '930 - Range problem *S10
115    '931 - Range problem *S11
116    '999 - Alaphelyzetfelvételi hiba
117    '
118    MFel=0 'Ez a változó, ha nulla, akkor bármilyen parancs jöhet
119    MELOZOTERULET=0
120    MTERULET=0
121    'MTERULET jelentései (milyen pozícióba képes menni):
122    '10 - alaphelyzet pozíció jog
123    '20 - szerviz pozício jog
124    '30 - amegfogó ürítése jog
125    '
126    mXtemp=0 'alaphelyzet pozíció kereséshez
127    mYtemp=0 'alaphelyzet pozíció kereséshez
128    mZtemp=0 'alaphelyzet pozíció kereséshez
129    'ptemp0=0 pozíció segéd alaphelyzethez
130    'ptemp1=0 'pozíció segéd folyamathoz
131    'jtemp1=0 'pozíció segéd forgatáshoz
132    MTavol=0 'alaphelyzetbe értünk-e már?
133    '===================================================
134    '===================================================
135    '---------------------------------------------------
136    'Helyi változók felsorolás vége------------------
137    '---------------------------------------------------
138    '
139    'Hiba esetén megszakítás generálódik
140   '
141   While (M_Err=0)
142     Select ioCOM 'Mozgás végrehajtás parancs
143       Case 0 'nope
144         Mms = M_Timer(1)
145         Ms=Mms/1000.0
146         ioMsSLV = Mms
147         Break
148       ' DANGER!! nincsen megálapodás róla IGNORE küldjön hibát
149       'Case 1 'Alaphelyzet felvétel ES után
150       '  GoSub *S1
151       'Break
152       'Case 2 'Alaphelyzet felvétel CIKLUS
153       '  GoSub *S2
154       'Break
155       'Case 3 'Szerviz pozíció felvétel
156       '  GoSub *S3
157       'Break
158       'Case 4 'Megfogó kiürítése
159       '  GoSub *S4
160       'Break
161       Case 10 'XYZ ABC joint
162         GoSub *S10
163         Break
164       Case 11 'XYZ ABC lineáris
165         GoSub *S110
166         Break
167       ' DANGER!! még nem nem tudtam foglalkozni vele IGNORE küldjön hibát
168       'Case 12 'XYZ ABC lineáris erõre
169       '  GoSub *S12
170       'Break
171       'Case 20 'XYZ ABC eltolás joint
172       '  GoSub *S20
173       'Break
174       'Case 21 'XYZ ABC eltolás lineáris
175       '  GoSub *S21
176       'Break
177       'Case 22 'XYZ ABC eltolás lineáris erõre
178       '  GoSub *S22
179       'Break
180       'Case 30 'J1-J6
181       '  GoSub *S30
182       'Break
183       'Case 40 'J1-J6 eltolás
184       '  GoSub *S40
185       'Break
186       Default
187         'IGNORE küldjön hibát
188         GoSub *HANDSHAKE1
189         MHibaszam=900
190         GoSub *HANDSHAKE2
191         Break
192     End Select
193     Dly 0.001
194   WEnd
195   End
196   '
197   '
198   '=================================================================
199   '---------------------------------
200   'Szubrutinok következnek-----
201   '---------------------------------
202   '=================================================================
203   *S1 '=======================
204   'Alaphelyzet
205    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
206    'Inicializációs rész
207    Fsc Off
208    Servo On 'szervo
209    Ovrd 5 'Sebesség
210    Accel 5,5 'Gyorsulás, lassulás
211    M_Tool=0
212   MHibaszam=0 'Hiba nullázás
213   Mdarab=0
214   '=================================================================
215   '=================================================================
216   'Mozgás: Kamerás ellenõrzõ pozíciótól
217    mXtemp = Deg(J_Curr.J4)
218    mYtemp = Deg(J_Curr.J1)
219    mZtemp = Deg(J_Curr.J2)
220   If ((mXtemp>60 And mYtemp<15) Or (mXtemp>60 And mZtemp<-20)) Then
221   'Kamerás pozícióban áll a robot!
222   Cnt 0
223   Mov jKamera
224     Dly 0.01
225   Mov jAlaph
226     Dly 0.01
227   EndIf
228   '=================================================================
229   '=================================================================
230   'Mozgás: Lerakó területrõl
231     ptemp0 = P_Curr 'Pillanatnyi pozíció
232   If (ptemp0.X > (250)) Then
233   'Lerakó pozícióban áll a fej!
234   Cnt 0
235   Dly 0.1
236     ptemp0.Z=520
237   Mvs ptemp0
238     Dly 0.01
239   Mov jLerakas
240     Dly 0.01
241   Mov jAlaph
242     Dly 0.01
243   EndIf
244   '=================================================================
245   '=================================================================
246   'Mozgás: Felvételi pozíciótól
247     ptemp0 = P_Curr 'Pillanatnyi pozíció
248   If ((ptemp0.X) < (100)) Then
249   'Felvételi pozícióban áll a fej!
250   Cnt 0
251     ptemp0.Z=520
252   Mvs ptemp0
253     Dly 0.01
254   Mov jFelvetel
255     Dly 0.01
256   Mov jAlaph
257     Dly 0.01
258   EndIf
259   '=================================================================
260   '=================================================================
261   ptemp0 = P_Curr 'Pillanatnyi pozíció
262   If ((ptemp0.X <= (250)) And  (ptemp0.X >= (100))) Then
263   Mov jAlaph
264   Dly 0.01
265   Else
266   MHibaszam=999
267   EndIf
268   '=================================================================
269   '=================================================================
270   'Lekérdezés, hogy Alaph-ben vagyunk
271    mXtemp = Deg(J_Curr.J1)
272    mYtemp = Deg(jAlaph.J1)
273    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
274     Dly 0.1
275    WEnd
276    mXtemp = Deg(J_Curr.J2)
277    mYtemp = Deg(jAlaph.J2)
278    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
279     Dly 0.1
280    WEnd
281    mXtemp = Deg(J_Curr.J3)
282    mYtemp = Deg(jAlaph.J3)
283    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
284     Dly 0.1
285    WEnd
286    mXtemp = Deg(J_Curr.J4)
287    mYtemp = Deg(jAlaph.J4)
288    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
289     Dly 0.1
290    WEnd
291    mXtemp = Deg(J_Curr.J5)
292    mYtemp = Deg(jAlaph.J5)
293    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
294     Dly 0.1
295    WEnd
296    mXtemp = Deg(J_Curr.J6)
297    mYtemp = Deg(jAlaph.J6)
298    While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
299     Dly 0.1
300    WEnd
301   '
302   'Belsõ változók beállítása
303    MELOZOTERULET=10
304    MTERULET=10
305    MFel=0
306   '
307    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
308   Return '=======================
309   '=================================================================
310   '*************************************************************************
311   '*************************************************************************
312   '=================================================================
313   *S2 '=======================
314   If (MFel=0) Then
315   'Alaphelyzet
316    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
317    'Inicializációs rész
318     Ovrd MSebesseg 'Sebesség
319    Accel Mgyors,Mlass 'Gyorsulás
320   '
321    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
322   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
323    MTERULET=10 'amelyik terulethez kell állítani a tengelyeket
324    GoSub *TERULETVALTAS
325    Mov jAlaph
326   Dly 0.01
327   MHibaszam=0 'Hiba nullázás
328   '
329    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
330   EndIf
331   Return '=======================
332   '=================================================================
333   '*************************************************************************
334   '*************************************************************************
335   '=================================================================
336   *S3 '=======================
337   If (MFel=0) Then
338   'Szerviz pozíció felvétele
339    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
340    'Inicializációs rész
341    Ovrd 10 'Sebesség
342    Accel 10,10 'Gyorsulás
343   '
344    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
345   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
346    MTERULET=20 'amelyik terulethez kell állítani a tengelyeket
347    GoSub *TERULETVALTAS
348    Mov jSzerviz
349     Dly 0.01
350    MFel=0
351   '
352    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
353   EndIf
354   Return '=======================
355   '=================================================================
356   '*************************************************************************
357   '*************************************************************************
358   '=================================================================
359   *S4 '=======================
360   If (MFel=0) Then
361   'Megfogó kiürítése
362    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
363    'Inicializációs rész
364     Ovrd MSebesseg 'Sebesség
365    Accel Mgyors,Mlass 'Gyorsulás
366   '
367    M_Tool=0 'Nincs még semmilyen tool használva, a pozíciók beállításához használok csak tool-t, elmenteni már tool0-val mentem
368   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
369    MTERULET=30 'amelyik terulethez kell állítani a tengelyeket
370    GoSub *TERULETVALTAS
371    Mov jMurites
372     Dly 0.001
373   Cnt 0
374   Mvs pMurites
375     Dly 0.01
376   '
377    Mdarab=0
378    MFel=0
379   '
380    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
381   EndIf
382   Return '=======================
383   '=================================================================
384   '=================================================================
385   '**************************XYZ ABC joint**************************
386   '=================================================================
387   '=================================================================
388   *S10 '=======================
389     '
390     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
391       'XYZ ABC joint
392       GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
393       ptemp1=P_Curr
394       ptemp1.X=(MPosX/100)
395       ptemp1.Y=(MPosY/100)
396       ptemp1.Z=(MPosZ/100)
397       If Dist(P_Zero,ptemp1) < 300 Then
398         MHibaszam=910 'Zero detect!!!
399       Else 'Zero detect!!!
400         ptemp1.A=Rad(MPosA/100)
401         ptemp1.B=Rad(MPosB/100)
402         ptemp1.C=Rad(MPosC/100)
403         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
404         '
405         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
406           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
407           ' akkor nekem nem kell alahelyzet felvétel
408           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
409           If M_Svo=0 Then
410             If M_Run(3)=1 Then
411               Servo On 'szervo
412             EndIf ' R2D2.prg
413           EndIf ' M_Servo?
414           Fsc Off
415           M_Tool=0
416           Ovrd MSebesseg 'Sebesség
417           Accel Mgyors,Mlass 'Gyorsulás
418           '
419           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
420           Rem úgy gondolom bajos, lutri
421           ' Cnt 1
422           '
423           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
424           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
425           'GoSub *TERULETVALTAS
426           MELOZOTERULET=MTERULET
427           MHibaszam=0
428           '
429           Cnt 0
430           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
431           '
432           M2=Abs(ptemp1.A)
433              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
434              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
435           '
436           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
437           '
438           'If ((MPosX>-100000& And MPosX<100000& And MPosY>-100000& And MPosY<100000& And MPosZ>-30000 And MPosZ<150000&) And (MPosA>-36000& And MPosA<36000& And MPosB>-36000& And MPosB<36000& And MPosC>-36000& And MPosC<36000&)) Then
439           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
440           If M1 < M3 And M2 < (M_PI*2.0) Then
441             M1=PosCq(ptemp1)
442             M2=0
443             While (M1=0) And (M2<100)
444               Select M2
445                 Case 0 'talán rossz az f2
446                   jtemp1 = PtoJ(ptemp1)
447                   ptemp1 = JtoP(jtemp1)
448                   Dly 1.0 'a userdefined screenen lassam itt van
449                 Break
450                 Case 1 'kérjünk egy közbenso állapotot
451                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
452                   Dly 1.0 'a userdefined screenen lassam itt van
453                 Break
454                 Case 2 'talán rossz a min.f2
455                   jtemp1 = PtoJ(ptemp1)
456                   ptemp1 = JtoP(jtemp1)
457                   Dly 1.0 'a userdefined screenen lassam itt van
458                 Break
459                 Case 3 '
460                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
461                   M2=99
462                   Dly 4.0 'a userdefined screenen lassam itt van
463                 Break
464               End Select
465               M1 = PosCq(ptemp1)
466               M2=M2+1
467               Dly 1.0 'a userdefined screenen lassam itt van
468               ' valami nem tetszett
469             WEnd
470             M1 = PosCq(ptemp1)
471             If M1=1 Then
472               Mov ptemp1
473             Else
474               'küldjön akkor is hibát ha netán sikerül mozogni
475               'ezeket fel akarom jegyezni
476               MHibaszam=931
477               Dly 10.0 'a userdefined screenen lassam itt van
478               ' valami nem tetszett
479               ' esetleg le löjem, vagy sem? 10 másodperc van rá
480               Mov ptemp1
481             EndIf
482             '
483             MFel=0 'Bármelyik parancs következhet
484             '
485           Else ' CAGE BOX
486             MHibaszam=911
487           EndIf ' CAGE BOX?
488         EndIf ' MOVE? Dist?
489       EndIf  'Zero detect?
490       GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
491     Else 'MFel?
492       MHibaszam=900
493     EndIf 'MFel?
494   Return '=======================
495   '=================================================================
496   '=================================================================
497   '************************XYZ ABC lineáris**************************
498   '=================================================================
499   '=================================================================
500   *S110 '=======================
501     MGY = Mgyors
502     ML = Mlass
503   *S110CNT
504     GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
505     If (MFel<>0) Then 'MFel?
506       MHibaszam=900
507     Else  'MFel?
508       If Dist(P_Zero,ptemp2) < 300 Then
509         MHibaszam=910 'Zero detect!!!
510       ElseIf Dist(ptemp1,ptemp2) > 0 Then
511         MHibaszam=911 'HS1 detect!!!
512       Else 'Zero detect!!!
513         ptemp2.A=Rad(MPosA/100)
514         ptemp2.B=Rad(MPosB/100)
515         ptemp2.C=Rad(MPosC/100)
516         'ABC Posture különbség
517         M2=Abs(Cos(P_Curr.A)-Cos(ptemp2.A))
518           If M2<Abs(Cos(P_Curr.B)-Cos(ptemp2.B)) Then M2=Abs(Cos(P_Curr.B)-Cos(ptemp2.B))
519           If M2<Abs(Cos(P_Curr.C)-Cos(ptemp2.C)) Then M2=Abs(Cos(P_Curr.C)-Cos(ptemp2.C))
520         'XYZ Távolság
521         M1=Dist(P_Curr,ptemp2)
522         MHibaszam=0
523         If (M1+M2)=0 Then  ' MOVE? TURN?
524           MHibaszam=0
525         Else ' MOVE? TURN?
526           If M_Svo=0 Then
527             If M_Run(3)=1 Then
528               Servo On 'szervo
529             EndIf ' R2D2.prg
530           EndIf ' M_Servo?
531           Fsc Off
532           M_Tool=0
533           ' Tehát M1 mm-ben meg van a távolság
534           Md = M1
535           MlstV=MSebesseg
536           'Me=(MPosS-MlstE) ' Mikorra kéne ott lenni
537           If MPosS>0 Then
538             ' megvan adva mennyi ido van rá
539             MPosS100=MPosS/100.0
540             MV=(100*M_OPovrd)/100.0 'sebesség 150mm/sec * OP
541             M6=(Md/MV)*1000.0 'd/Mv = sec lenne max sebességben, felszorozzuk menyivel több ebben a sebeségen ?
542             MlstV2=M6/MPosS100
543             MlstV=MlstV2
544             If MlstV>MSebesseg Then
545                 MlstV=MSebesseg
546             ElseIf MlstV < 1 Then
547                 MlstV = 1
548             EndIf
549           EndIf
550           Ovrd MlstV 'MSebesseg 'Sebesség
551           Accel MGY,ML 'Gyorsulás
552           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
553           MELOZOTERULET=MTERULET
554           Cnt 0
555           ' CAGE check -----------------------------------------------
556           M2=Abs(ptemp2.A)
557             If M2<Abs(ptemp2.B) Then M2=Abs(ptemp2.B)
558             If M2<Abs(ptemp2.C) Then M2=Abs(ptemp2.C)
559           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
560           If (M1 > M3) Or (M2 > (M_PI*2.0)) Then ' CAGE BOX
561             MHibaszam=911
562           Else ' CAGE BOX
563             jtemp2 = PtoJ(ptemp2)
564             'old If (Abs(jtemp2.J4)<Rad(90)) And Abs(jtemp2.J5)<Rad(75.0)  Then ' Soft J4
565             M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
566             M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
567             'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
568             'egyszeru eltérést számol a középtol
569             If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Soft J4
570               Mvs ptemp2 WthIf M_Ratio >= Mhs2rt, ioHS2 = 1
571               M_Out(10387)=1
572               M_Out(11314)=0
573               Cnt 0
574             Else ' Soft J4
575               M_Out(11314)=1
576               M_Out(10387)=0
577               ' Hard J4
578               jtemp0=J_Curr
579               ' FLIP? 4 ax
580               If M4>MNMX(4,4) Then
581                 If Deg(jtemp2.J4)>MNMX(4,3) Then
582                   jtemp2.J4=jtemp2.J4-M_PI
583                 Else
584                   jtemp2.J4=jtemp2.J4+M_PI
585                 EndIf
586                 ' FLIP 5 ax
587                 jtemp2.J5=jtemp2.J5*-1.0
588                 M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet így mar jó?
589                 ' FLIP 6 ax
590                 jtemp2.J6=jtemp2.J6*-1.0
591               EndIf
592               If M5>MNMX(5,4) Then
593                 MHibaszam=9115
594                 If Deg(jtemp2.J5)>MNMX(5,3) Then
595                   jtemp2.J5=M_PI*((MNMX(5,2)-Max5off)/180.0) 'maximum
596                 Else
597                   jtemp2.J5=M_PI*((MNMX(5,1)+Max5off)/180.0) 'minimum
598                 EndIf
599               EndIf
600               ' MIDLE jtemp1
601               jtemp1.J1 = (jtemp2.J1-J_Curr.J1)*Mmid + J_Curr.J1
602               jtemp1.J2 = (jtemp2.J2-J_Curr.J2)*Mmid + J_Curr.J2
603               jtemp1.J3 = (jtemp2.J3-J_Curr.J3)*Mmid + J_Curr.J3
604               jtemp1.J4 = (jtemp2.J4-J_Curr.J4)*Mmid + J_Curr.J4
605               jtemp1.J5 = (jtemp2.J5-J_Curr.J5)*Mmid + J_Curr.J5
606               jtemp1.J6 = (jtemp2.J6-J_Curr.J6)*Mmid + J_Curr.J6
607               Mov jtemp1 WthIf M_Ratio >= Mhs2rt, ioHS2=1
608             EndIf ' Soft J4
609             'If ioHS3=1 Then
610             '  Cnt 1
611             '  GoSub *FEEDBACK
612             '  Wait ioHS3=0
613             '  GoSub *PXYZABCS
614             '  If ioCOM=11 Then
615             '    ioHS3=0
616             '    GoTo *S110CNT
617             '  EndIf
618             'EndIf
619             ioHS3=0
620           EndIf ' CAGE BOX?
621         EndIf ' MOVE? TURN?
622       EndIf  'Zero detect?
623     EndIf 'MFel?
624     GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
625     If (ioCOM=11) And ioHS1=1 And (M_Svo(1)=1) Then
626         MGY = 100
627         ML = 100
628         Cnt 1
629         GoTo *S110 'ha folytatásra érkezett parancs
630     EndIf
631     Cnt 0
632   Return '=======================
633   *S11 '=======================
634     '
635     If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
636       'XYZ ABC joint
637       GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
638       ptemp1=P_Curr
639       ptemp1.X=(MPosX/100)
640       ptemp1.Y=(MPosY/100)
641       ptemp1.Z=(MPosZ/100)
642       If Dist(P_Zero,ptemp1) < 300 Then
643         MHibaszam=910 'Zero detect!!!
644       Else 'Zero detect!!!
645         ptemp1.A=Rad(MPosA/100)
646         ptemp1.B=Rad(MPosB/100)
647         ptemp1.C=Rad(MPosC/100)
648         M1 = Abs(Cos(P_Curr.A)-Cos(ptemp1.A)) + Abs(Cos(P_Curr.B)-Cos(ptemp1.B)) + Abs(Cos(P_Curr.C)-Cos(ptemp1.C))
649         '
650         If Dist(P_Curr,ptemp1) > 0 Or M1 > 0 Then
651           ' ha fut a R2D2.prg bekapcsolhatja tudtam hol a robot
652           ' akkor nekem nem kell alahelyzet felvétel
653           ' ha pedig S.A szolgáltatja az adatokatt akkor meg az ellenörzés sem kell
654           If M_Svo=0 Then
655             If M_Run(3)=1 Then
656               Servo On 'szervo
657             EndIf ' R2D2.prg
658           EndIf ' M_Servo?
659           Fsc Off
660           M_Tool=0
661           Ovrd MSebesseg 'Sebesség
662           Accel Mgyors,Mlass 'Gyorsulás
663           '
664           Rem ezt kiszedtem mert amíg nincsenek felvéve területek
665           Rem úgy gondolom bajos, lutri
666           ' Cnt 1
667           '
668           'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
669           MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
670           'GoSub *TERULETVALTAS
671           MELOZOTERULET=MTERULET
672           MHibaszam=0
673           '
674           Cnt 0
675           M1 = Dist(P_Zero,ptemp1) 'ha jol látom most jo ez is de egyébként lehet egy "doboz" közepe zero helyett
676           '
677           M2=Abs(ptemp1.A)
678              If M2<Abs(ptemp1.B) Then M2=Abs(ptemp1.B)
679              If M2<Abs(ptemp1.C) Then M2=Abs(ptemp1.C)
680           '
681           M3 = Sqr(3.0*(800.0*800.0)) 'kb ~1385mm
682           '
683           If M1 < M3 And M2 < (M_PI*2.0) Then
684             M1=PosCq(ptemp1)
685             M2=0
686             While (M1=0) And (M2<100)
687               Select M2
688                 Case 0 'talán rossz az f2
689                   jtemp1 = PtoJ(ptemp1)
690                   ptemp1 = JtoP(jtemp1)
691                   Dly 1.0 'a userdefined screenen lassam itt van
692                 Break
693                 Case 1 'kérjünk egy közbenso állapotot
694                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
695                   Dly 1.0 'a userdefined screenen lássam itt van
696                 Break
697                 Case 2 'talán rossz a min.f2
698                   jtemp1 = PtoJ(ptemp1)
699                   ptemp1 = JtoP(jtemp1)
700                   Dly 1.0 'a userdefined screenen lássam itt van
701                 Break
702                 Case 3 '
703                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
704                   M2=99
705                   Dly 1.0 'a userdefined screenen lássam itt van
706                 Break
707               End Select
708               M1=PosCq(ptemp1)
709               M2=M2+1
710               Dly 1.0 'a userdefined screenen lássam itt van
711               ' valami nem tetszett
712             WEnd
713             '
714             jtemp1 = PtoJ(ptemp1)
715             If Abs(jtemp1.J4) > (M_PI*2.0/4.0) Then
716               Mvs ptemp1 Type 1,0
717               MHibaszam=931
718             Else
719               Mvs ptemp1
720             EndIf
721             '
722             MFel=0 'Bármelyik parancs következhet
723             '
724           Else ' CAGE BOX
725             MHibaszam=911
726           EndIf ' CAGE BOX?
727         EndIf ' MOVE? Dist?
728       EndIf  'Zero detect?
729       GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
730     Else 'MFel?
731       MHibaszam=900
732     EndIf 'MFel?
733   Return '=======================
734   '=================================================================
735   '=================================================================
736   '*********************XYZ ABC lineáris erõre***********************
737   '=================================================================
738   '=================================================================
739   *S12 '=======================
740   '
741   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
742    'XYZ ABC lineáris erõre
743    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
744   '
745    Ovrd MSebesseg 'Sebesség
746    Accel Mgyors,Mlass 'Gyorsulás
747   '
748   Cnt 1
749   '
750   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
751    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
752    GoSub *TERULETVALTAS
753   MHibaszam=0
754   '
755   Cnt 0
756   '
757   ptemp1=P_Curr 'Pillanatnyi pozíció
758   '
759   If ((MPosX>100 And MPosX<300 And MPosY>100 And MPosY<300 And MPosZ>100 And MPosZ<300) And (MPosA>100 And MPosA<300 And MPosB>100 And MPosB<300 And MPosC>100 And MPosC<300)) Then
760    ptemp1.X=(MPosX/100)
761    ptemp1.Y=(MPosY/100)
762    ptemp1.Z=(MPosZ/100)
763    ptemp1.A=Rad(MPosA/100)
764    ptemp1.B=Rad(MPosB/100)
765    ptemp1.C=Rad(MPosC/100)
766    Fsc On, 1,1,1
767    FsCTrg 1,10,1
768    Mvs ptemp1
769    Dly 0.01
770    Fsc Off
771   '
772   MFel=0 'Bármelyik parancs következhet
773   '
774   Else
775   MHibaszam=910
776   EndIf
777   '
778    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
779   '
780   Else
781   MHibaszam=900
782   EndIf
783   Return '=======================
784   '=================================================================
785   '=================================================================
786   '********************XYZ ABC eltolás joint************************
787   '=================================================================
788   '=================================================================
789   *S20 '=======================
790   '
791   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
792    'XYZ ABC eltolás joint
793    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
794   '
795    Ovrd MSebesseg 'Sebesség
796    Accel Mgyors,Mlass 'Gyorsulás
797   '
798   Cnt 1
799   '
800   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
801    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
802    GoSub *TERULETVALTAS
803   MHibaszam=0
804   '
805   Cnt 0
806   '
807   ptemp1=P_Curr 'Pillanatnyi pozíció
808   '
809   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
810    ptemp1.X=ptemp1.X+(MPosXe/100)
811    ptemp1.Y=ptemp1.Y+(MPosYe/100)
812    ptemp1.Z=ptemp1.Z+(MPosZe/100)
813    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
814    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
815    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
816    Mov ptemp1
817    Dly 0.01
818   '
819   MFel=0 'Bármelyik parancs következhet
820   '
821   Else
822   MHibaszam=910
823   EndIf
824   '
825    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
826   '
827   Else
828   MHibaszam=900
829   EndIf
830   Return '=======================
831   '=================================================================
832   '=================================================================
833   '********************XYZ ABC eltolás lineáris*********************
834   '=================================================================
835   '=================================================================
836   *S21 '=======================
837   '
838   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
839    'XYZ ABC eltolás lineáris
840    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
841   '
842    Ovrd MSebesseg 'Sebesség
843    Accel Mgyors,Mlass 'Gyorsulás
844   '
845   Cnt 1
846   '
847   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
848    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
849    GoSub *TERULETVALTAS
850   MHibaszam=0
851   '
852   Cnt 0
853   '
854   ptemp1=P_Curr 'Pillanatnyi pozíció
855   '
856   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
857    ptemp1.X=ptemp1.X+(MPosXe/100)
858    ptemp1.Y=ptemp1.Y+(MPosYe/100)
859    ptemp1.Z=ptemp1.Z+(MPosZe/100)
860    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
861    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
862    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
863    Mvs ptemp1
864    Dly 0.01
865   '
866   MFel=0 'Bármelyik parancs következhet
867   '
868   Else
869   MHibaszam=910
870   EndIf
871   '
872    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
873   '
874   Else
875   MHibaszam=900
876   EndIf
877   Return '=======================
878   '=================================================================
879   '=================================================================
880   '*****************XYZ ABC eltolás lineáris erõre******************
881   '=================================================================
882   '=================================================================
883   *S22 '=======================
884   '
885   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
886    'XYZ ABC eltolás lineáris erõre
887    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
888   '
889    Ovrd MSebesseg 'Sebesség
890    Accel Mgyors,Mlass 'Gyorsulás
891   '
892   Cnt 1
893   '
894   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
895    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
896    GoSub *TERULETVALTAS
897   MHibaszam=0
898   '
899   Cnt 0
900   '
901   ptemp1=P_Curr 'Pillanatnyi pozíció
902   '
903   If ((MPosXe>-1000 And MPosXe<1000 And MPosYe>100 And MPosYe<300 And MPosZe>100 And MPosZe<300) And (MPosAe>100 And MPosAe<300 And MPosBe>100 And MPosBe<300 And MPosCe>100 And MPosCe<300)) Then
904    ptemp1.X=ptemp1.X+(MPosXe/100)
905    ptemp1.Y=ptemp1.Y+(MPosYe/100)
906    ptemp1.Z=ptemp1.Z+(MPosZe/100)
907    ptemp1.A=ptemp1.A+Rad(MPosAe/100)
908    ptemp1.B=ptemp1.B+Rad(MPosBe/100)
909    ptemp1.C=ptemp1.C+Rad(MPosCe/100)
910    Mvs ptemp1
911    Dly 0.01
912   '
913   MFel=0 'Bármelyik parancs következhet
914   '
915   Else
916   MHibaszam=910
917   EndIf
918   '
919    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
920   '
921   Else
922   MHibaszam=900
923   EndIf
924   Return '=======================
925   '=================================================================
926   '=================================================================
927   '*******************************J1-J6*****************************
928   '=================================================================
929   '=================================================================
930   *S30 '=======================
931   '
932   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
933    'J1-J6
934    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
935   '
936    Ovrd MSebesseg 'Sebesség
937    Accel Mgyors,Mlass 'Gyorsulás
938   '
939   Cnt 1
940   '
941   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
942    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
943    GoSub *TERULETVALTAS
944   MHibaszam=0
945   '
946   Cnt 0
947   '
948   jtemp1=J_Curr 'Pillanatnyi pozíció
949   '
950   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
951    jtemp1.J1=Rad(MPosJ1/100)
952    jtemp1.J2=Rad(MPosJ2/100)
953    jtemp1.J3=Rad(MPosJ3/100)
954    jtemp1.J4=Rad(MPosJ4/100)
955    jtemp1.J5=Rad(MPosJ5/100)
956    jtemp1.J6=Rad(MPosJ6/100)
957    Mov jtemp1
958    Dly 0.01
959    '
960    MFel=0 'Bármelyik parancs következhet
961    '
962   Else
963   MHibaszam=910
964   EndIf
965   '
966    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
967   '
968   Else
969   MHibaszam=900
970   EndIf
971   Return '=======================
972   '=================================================================
973   '=================================================================
974   '**************************J1-J6 eltolás**************************
975   '=================================================================
976   '=================================================================
977   *S40 '=======================
978   '
979   If (MFel=0) Then 'Bármelyik mozgás után elvégezhetõ a parancs
980    'J1-J6 eltolás
981    GoSub *HANDSHAKE1 'üzenet megérkezett, parancs elkezdve nyugta
982   '
983    Ovrd MSebesseg 'Sebesség
984    Accel Mgyors,Mlass 'Gyorsulás
985   '
986   Cnt 1
987   '
988   'Robot elhelyezkedésének ellenõrzése, hogy a tengelyek megfelelõnek álljanak
989    MTERULET=40 'amelyik terulethez kell állítani a tengelyeket
990    GoSub *TERULETVALTAS
991   MHibaszam=0
992   '
993   Cnt 0
994   '
995   jtemp1=J_Curr 'Pillanatnyi pozíció
996   '
997   If (MPosJ1>-36000& And MPosJ1<36000& And MPosJ2>-36000& And MPosJ2<36000& And MPosJ3>-36000& And MPosJ3<36000& And MPosJ4>-36000& And MPosJ4<36000& And MPosJ5>-36000& And MPosJ5<36000& And MPosJ6>-36000& And MPosJ6<36000&) Then
998    jtemp1.J1=jtemp1.J1+Rad(MPosJ1/100)
999    jtemp1.J2=jtemp1.J2+Rad(MPosJ2/100)
1000    jtemp1.J3=jtemp1.J3+Rad(MPosJ3/100)
1001    jtemp1.J4=jtemp1.J4+Rad(MPosJ4/100)
1002    jtemp1.J5=jtemp1.J5+Rad(MPosJ5/100)
1003    jtemp1.J6=jtemp1.J6+Rad(MPosJ6/100)
1004    Mov jtemp1
1005    Dly 0.01
1006   '
1007   MFel=0 'Bármelyik parancs következhet
1008   '
1009   Else
1010   MHibaszam=910
1011   EndIf
1012   '
1013    GoSub *HANDSHAKE2 'vége a mûveletnek, várakozás a nyugtázásra
1014   '
1015   Else
1016   MHibaszam=900
1017   EndIf
1018   Return '=======================
1019   '=================================================================
1020   '=================================================================
1021   '*****************************************************************
1022   '=================================================================
1023   '=================================================================
1024   '
1025   *FEEDBACK '=======================
1026     Mms = M_Timer(1)
1027     Ms=Mms/1000.0
1028     ioMsSLV = Mms
1029     ioMx=P_Curr.X*100
1030     ioMy=P_Curr.Y*100
1031     ioMz=P_Curr.Z*100
1032     ioMa=Deg(P_Curr.A)*100
1033     ioMb=Deg(P_Curr.B)*100
1034     ioMc=Deg(P_Curr.C)*100
1035   Return '=======================
1036   '
1037   *PXYZABCS '=======================
1038     ptemp2=P_Curr
1039     MPosX=ioMx 'X pozíció átvétele
1040     MPosY=ioMy 'Y pozíció átvétele
1041     MPosZ=ioMz 'Z pozíció átvétele
1042     MPosA=ioMa 'A pozíció fok átvétele
1043     MPosB=ioMb 'B pozíció fok átvétele
1044     MPosC=ioMc 'C pozíció fok átvétele
1045     MPosS=ioMs 'pozíció utazási ido átvétele
1046       ptemp2.X=(MPosX/100)
1047       ptemp2.Y=(MPosY/100)
1048       ptemp2.Z=(MPosZ/100)
1049       ptemp2.A=Rad(MPosA/100)
1050       ptemp2.B=Rad(MPosB/100)
1051       ptemp2.C=Rad(MPosC/100)
1052   Return '=======================
1053   '
1054   *HANDSHAKE1 '=======================
1055     Wait ioHS1=1 'wait RobotStartIn=1
1056     GoSub *PXYZABCS 'ptemp2 kitölti az io-ról
1057     ptemp1 = ptemp2 'csinálunk egy másolatot
1058     ioHS1=1 'ConfirmOut=1
1059     Wait ioHS1=0 'wait RobotStartIn=0
1060     Wait ioCOM=0 'Mozgás parancs nullázására vár
1061     GoSub *PXYZABCS 'ptemp2 hs1 után kitölti az io-t
1062     'így össze lehet hasonlítani, hogy elrontotta e valaki
1063     'ha nem stimmel hiba!!!
1064     ioHS1=0 'ConfirmOut=0
1065     M_00#=1 ' jelzes R2D2-nak hogy elkezdödött valami
1066   Return '=======================
1067   '
1068   *HANDSHAKE2 '=======================
1069     MPosS = 0
1070     If M_Svo(1)=0 Then
1071       ' Így nem nyírja ki miért lett leállítva
1072       If MHibaszam<1 Then
1073         MHibaszam=920
1074       EndIf
1075     EndIf
1076     ' Így az R2D2-nál is frissebb adatok lesznek mire olvasnám
1077     GoSub *FEEDBACK
1078    '
1079    ioCOM=MHibaszam 'Hiba státusz küldése
1080    ioHS2=1 'ReadyOut=1
1081    Wait ioHS2=1 'wait ConfirmIn=1
1082    ioCOM=0 'Hibaküldés kinullázása
1083    ioHS2=0 'ReadyOut=0
1084    Wait ioHS2=0 'wait ConfirmIn=0
1085    M_00#=0
1086  Return '=======================
1087  '=================================================================
1088  '*************************************************************************
1089  '*************************************************************************
1090  '*************************************************************************
1091  '*************************************************************************
1092  '=================================================================
1093  *TERULETVALTAS '=======================
1094  If (MELOZOTERULET<>MTERULET) Then
1095  '................................
1096   If (MELOZOTERULET=10 And MTERULET=20) Then 'váltás alaphelyzetbõl szervizpozícióra
1097    Mov jAlaph
1098    Mov jSzerviz
1099  '
1100    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1101   EndIf
1102  '................................
1103   If (MELOZOTERULET=30 And MTERULET=20) Then 'váltás megfogó ürítésrõl szervizpozícióra
1104    Mov jMurites
1105    Mov jAlaph
1106    Mov jSzerviz
1107  '
1108    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1109   EndIf
1110  '................................
1111   If (MELOZOTERULET=40 And MTERULET=20) Then 'váltás felvételrõl szervizpozícióra
1112    Mov jFelvetel
1113    Mov jSzerviz
1114  '
1115    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1116   EndIf
1117  '................................
1118   If (MELOZOTERULET=50 And MTERULET=20) Then 'váltás kamerásról szervizpozícióra
1119    Mov jKamera
1120    Mov jAlaph
1121    Mov jSzerviz
1122  '
1123    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1124   EndIf
1125  '................................
1126   If (MELOZOTERULET=60 And MTERULET=20) Then 'váltás lerakásról szervizpozícióra
1127    Mov jLerakas
1128    Mov jAlaph
1129    Mov jSzerviz
1130  '
1131    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1132   EndIf
1133  '................................
1134   If (MELOZOTERULET=20 And MTERULET=10) Then 'váltás szervizpozícióról alaphelyzetre
1135    Mov jSzerviz
1136    Mov jAlaph
1137  '
1138    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1139   EndIf
1140  '................................
1141   If (MELOZOTERULET=30 And MTERULET=10) Then 'váltás megfogó ürítésrõl alaphelyzetre
1142    Mov jMurites
1143    Mov jAlaph
1144  '
1145    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1146   EndIf
1147  '................................
1148   If (MELOZOTERULET=40 And MTERULET=10) Then 'váltás felvételrõl alaphelyzetre
1149    Mov jFelvetel
1150    Mov jAlaph
1151  '
1152    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1153   EndIf
1154  '................................
1155   If (MELOZOTERULET=50 And MTERULET=10) Then 'váltás kamerásról alaphelyzetre
1156    Mov jKamera
1157     Mov jAlaph
1158   '
1159     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1160   EndIf
1161  '................................
1162   If (MELOZOTERULET=60 And MTERULET=10) Then 'váltás lerakásról alaphelyzetre
1163    Mov jLerakas
1164    Mov jAlaph
1165  '
1166    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1167   EndIf
1168  '................................
1169   If (MELOZOTERULET=10 And MTERULET=30) Then 'váltás alaphelyzetbõl megfogó ürítésre
1170    Mov jAlaph
1171    Mov jMurites
1172  '
1173    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1174   EndIf
1175  '................................
1176   If (MELOZOTERULET=20 And MTERULET=30) Then 'váltás szervizpozícióról megfogó ürítésre
1177   Mov jSzerviz
1178   Mov jAlaph
1179   Mov jMurites
1180  '
1181    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1182   EndIf
1183  '................................
1184   If (MELOZOTERULET=40 And MTERULET=30) Then 'váltás felvételrõl megfogó ürítésre
1185   Mov jFelvetel
1186   Mov jAlaph
1187   Mov jMurites
1188  '
1189    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1190   EndIf
1191  '................................
1192   If (MELOZOTERULET=50 And MTERULET=30) Then 'váltás kamerásról megfogó ürítésre
1193    Mov jKamera
1194    Mov jMurites
1195  '
1196    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1197   EndIf
1198  '................................
1199   If (MELOZOTERULET=60 And MTERULET=30) Then 'váltás lerakásról megfogó ürítésre
1200    Mov jLerakas
1201    Mov jMurites
1202  '
1203    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1204   EndIf
1205  '................................
1206   If (MELOZOTERULET=10 And MTERULET=40) Then 'váltás alaphelyzetbõl felvételre
1207    Mov jAlaph
1208    Mov jFelvetel
1209  '
1210    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1211   EndIf
1212  '................................
1213   If (MELOZOTERULET=20 And MTERULET=40) Then 'váltás szervizpozícióról felvételre
1214    Mov jSzerviz
1215    Mov jFelvetel
1216  '
1217    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1218   EndIf
1219  '................................
1220   If (MELOZOTERULET=30 And MTERULET=40) Then 'váltás megfogó ürítésrõl felvételre
1221    Mov jMurites
1222    Mov jAlaph
1223    Mov jFelvetel
1224  '
1225    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1226   EndIf
1227  '................................
1228   If (MELOZOTERULET=50 And MTERULET=40) Then 'váltás kamerásról felvételre
1229    Mov jKamera
1230    Mov jFelvetel
1231  '
1232    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1233   EndIf
1234  '................................
1235   If (MELOZOTERULET=60 And MTERULET=40) Then 'váltás lerakásról felvételre
1236    Mov jLerakas
1237    Mov jAlaph
1238    Mov jFelvetel
1239  '
1240    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1241   EndIf
1242  '................................
1243   If (MELOZOTERULET=10 And MTERULET=50) Then 'váltás alaphelyzetbõl kamerásra
1244    Mov jAlaph
1245    Mov jKamera
1246  '
1247    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1248   EndIf
1249  '................................
1250   If (MELOZOTERULET=20 And MTERULET=50) Then 'váltás szervizpozícióról kamerásra
1251    Mov jSzerviz
1252    Mov jAlaph
1253    Mov jKamera
1254  '
1255    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1256   EndIf
1257  '................................
1258   If (MELOZOTERULET=30 And MTERULET=50) Then 'váltás megfogó ürítésrõl kamerásra
1259    Mov jMurites
1260     Mov jKamera
1261   '
1262     MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1263   EndIf
1264  '................................
1265   If (MELOZOTERULET=40 And MTERULET=50) Then 'váltás felvételrõl kamerásra
1266    Mov jFelvetel
1267    Mov jAlaph
1268    Mov jKamera
1269  '
1270    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1271   EndIf
1272  '................................
1273   If (MELOZOTERULET=60 And MTERULET=50) Then 'váltás lerakásról kamerásra
1274    Mov jLerakas
1275    Mov jKamera
1276  '
1277    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1278   EndIf
1279  '................................
1280   If (MELOZOTERULET=10 And MTERULET=60) Then 'váltás alaphelyzetbõl lerakásra
1281    Mov jAlaph
1282    Mov jLerakas
1283  '
1284    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1285   EndIf
1286 '................................
1287   If (MELOZOTERULET=20 And MTERULET=60) Then 'váltás szervizpozícióról lerakásra
1288    Mov jSzerviz
1289    Mov jAlaph
1290    Mov jLerakas
1291  '
1292    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1293   EndIf
1294  '................................
1295   If (MELOZOTERULET=30 And MTERULET=60) Then 'váltás megfogó ürítésrõl lerakásra
1296    Mov jMurites
1297    Mov jLerakas
1298  '
1299    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1300   EndIf
1301  '................................
1302   If (MELOZOTERULET=40 And MTERULET=60) Then 'váltás felvételrõl lerakásra
1303    Mov jFelvetel
1304    Mov jAlaph
1305    Mov jLerakas
1306  '
1307    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1308   EndIf
1309  '................................
1310   If (MELOZOTERULET=50 And MTERULET=60) Then 'váltás kamerásról lerakásra
1311    Mov jKamera
1312    Mov jLerakas
1313  '
1314    MELOZOTERULET=MTERULET 'új tengely pozíciók felvétele megtörtént
1315   EndIf
1316  '................................
1317  EndIf
1318  Return '=======================
1319 '
1320 '
1321 'Kártya Inputok:
1322 '0 - Gyárilag Foglalt
1323 '1 -
1324 '2 -
1325 '3 -
1326 '4 -
1327 '5 -
1328 '6 -
1329 '7 -
1330 '8 -
1331 '9 -
1332 '10 -
1333 '11 -
1334 '12 -
1335 '13 -
1336 '14 -
1337 '15 -
1338 '
1339 'Kártya Outputok:
1340 '0 -
1341 '1 -
1342 '2 -
1343 '3 -
1344 '4 -
1345 '5 -
1346 '6 -
1347 '7 -
1348 '8 -
1349 '9 -
1350 '10 -
1351 '11 -
1352 '12 -
1353 '13 -
1354 '14 -
1355 '15 -
1356 '
1357 '
1358 'Tool-ok:
1359 '0=semmilyen tool
jAlaph=(2.600,-33.050,136.100,0.000,76.950,92.600)
jFelvetel=(2.600,-33.050,136.100,0.000,76.950,92.600)
jKamera=(2.600,-33.050,136.100,0.000,76.950,92.600)
jLerakas=(2.600,-33.050,136.100,0.000,76.950,92.600)
jMurites=(2.600,-33.050,136.100,0.000,76.950,92.600)
jSzerviz=(2.600,-33.050,136.100,0.000,76.950,92.600)
jtemp0=(44.990,48.790,36.230,0.010,90.000,20.460)
jtemp1=(44.990,48.930,35.990,0.020,90.000,20.430)
jtemp2=(38.660,99.730,26.700,0.000,53.570,11.830,0.000,0.000)
pMurites=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
ptemp0=(471.620,385.480,274.160,-163.850,39.710,-96.520)(7,15728640)
ptemp1=(999.990,799.990,-99.980,-180.000,0.000,-153.170)(7,0)
ptemp2=(999.990,799.990,-99.980,-180.000,0.000,-153.170)(7,0)
pKamera(1)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(2)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(3)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
pKamera(4)=(508.180,23.120,734.150,180.000,0.000,90.000)(7,0)
