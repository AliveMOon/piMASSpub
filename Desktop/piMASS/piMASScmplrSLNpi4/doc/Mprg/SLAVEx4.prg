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
18     MNMX(M1,3) = MNMX(M1,1)
19     MNMX(M1,4) =(MNMX(M1,2)-MNMX(M1,1))/2.0
20     MNMX(M1,3) = MNMX(M1,3)+MNMX(M1,4)
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
35    Const MioIXx = 10192
36    Const MioIXy = 10208
37    Const MioIXz = 10224
38    Const MioIXa = 10240
39    Const MioIXb = 10256
40    Const MioIXc = 10272
41    ' ------------------ DEFINITION IOs --------------------
42    Rem -  -- ---=--==-=== NAME ===-==--=--- --  -
43    Def Io ioNM=DWord,10160
44    Rem -  -- ---=--==-=== XYZ0 ===-==--=--- --  -
45    Def Io ioMx00=Word,MioIXx
46    Def Io ioMy00=Word,MioIXy
47    Def Io ioMz00=Word,MioIXz
48    Rem -  -- ---=--==-=== ABC0 ===-==--=--- --  -
49    Def Io ioMa00=Word,MioIXa
50    Def Io ioMb00=Word,MioIXb
51    Def Io ioMc00=Word,MioIXc
52    Rem -  -- ---=--==-=== XYZ1 ===-==--=--- --  -
53    Def Io ioMx01=Word,10288
54    Def Io ioMy01=Word,10304
55    Def Io ioMz01=Word,10320
56    Rem -  -- ---=--==-=== ABC1 ===-==--=--- --  -
57    Def Io ioMa01=Word,10336
58    Def Io ioMb01=Word,10352
59    Def Io ioMc01=Word,10368
60    Rem -  -- ---=--==-=== XYZ2 ===-==--=--- --  -
61    Def Io ioMx10=Word,10384
62    Def Io ioMy10=Word,10400
63    Def Io ioMz10=Word,10416
64    Rem -  -- ---=--==-=== ABC2 ===-==--=--- --  -
65    Def Io ioMa10=Word,10432
66    Def Io ioMb10=Word,10448
67    Def Io ioMc10=Word,10464
68    Rem -  -- ---=--==-=== XYZ3 ===-==--=--- --  -
69    Def Io ioMx11=Word,10480
70    Def Io ioMy11=Word,10496
71    Def Io ioMz11=Word,10512
72    Rem -  -- ---=--==-=== ABC3 ===-==--=--- --  -
73    Def Io ioMa11=Word,10528
74    Def Io ioMb11=Word,10544
75    Def Io ioMc11=Word,10560
76    '
77    Rem -  -- ---=--==-=== TIME ===-==--=--- --  -
78    Def Io ioMs=DWord,10576
79    Rem -  -- ---=--==-=== ARY ===-==--=--- --  -
80    Def Io ioARYi=Word,10608
81    Def Io ioARYn=Word,10624
82    Rem -  -- ---=--==-=== CMD ===-==--=--- --  -
83    Def Io ioCOM=Word,10640
84    Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
85    Def Io ioHS1=Bit,10656
86    Def Io ioHS2=Bit,10657
87    Def Io ioMajre=Bit,10671
88    Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
89    Def Io ioMsSLV=DWord,10672
90    Def Io ioMsMST=DWord,10704
91    Def Io ioMsR2D=DWord,10736
92    Rem -  -- ---=--==-=== 10752 ===-==--=--- --  -
93    Rem -  -- ---=--==-=== POSn ===-==--=--- --  -
94    ' init
95    ioARYi = ioARYi
96    ioARYn = ioARYn
97    ioCOM = ioCOM
98    ioHS1 = ioHS1
99    ioHS2 = ioHS2
100    ioMajre = ioMajre
101    ioMsSLV=0
102    '
103    Rem - -- ---=--==-=== Megszakítás ===-==--=--- --  -
104    'Def Act 3,ioHS1=1 GoTo *S110IRQ, S
105    'Act 3=0
106    Rem -  -- ---=--==-=== MAJRE BUTTON ===-==--=--- --  -
107    ' EZT NEM SZABAD MÁSHOL VÁLTOZTATNI akkor konstast kell belölle csinálni
108    ' és akkor nem is lehet valahogy így:
109    Const MSebesseg=100 'Robot sebesség EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 100
110    Const Mgyors=80 'Robot gyorsulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
111    Const Mlass=80 'Robot lassulás EZT NEM SZABAD MÁSHOL VÁLTOZTATNI alap: 80
112    Const Mhs2rt=87 'Ratio konstans
113    Const Max5off=3.0
114    Const Mmid=0.66
115    Const MmxSPD=100.0
116    Const Mzero=300.0
117    '
118    Const MLp405 = Rad(360.0)
119    Const MLp315 = Rad(315.0)
120    Const MLp270 = Rad(270.0)
121    Const MLp225 = Rad(225.0)
122    Const MLp135 = Rad(135.0)
123    Const MLp90  = Rad( 90.0)
124    Const MLp45  = Rad( 45.0)
125    Const MLp1   = Rad(  1.0)
126    Const MLm1   = -MLp1
127    Const MLm45  = -MLm45
128    Const MLm90  = -MLp90
129    Const MLm225 = -MLp225
130    Const MLm270 = -MLp270
131    Const MLm315 = -MLp315
132    Const MPI2 = M_PI*2.0
133    Const MLm405 = -MLp405
134    Const MnMAX = 2048
135    '
136    MlstV=MSebesseg
137    Dim pARY(2052) ' MnMAX+4
138    Dim pKamera(4)
139    '
140    Rem - -- ---=--==-=== Saját funkciók ===-==--=--- --  -
141    Rem - Mabc ---------------------------------------------------------------
142    ' megadja mekkora a maximum cos a megadott szögek között
143    ' ellehet dönteni általa, hogy történi-e forgatás
144    Def FNMabcCOS(Pa,Pb)= Max( Abs(Cos(Pa.A)-Cos(Pb.A)), Abs(Cos(Pa.B)-Cos(Pb.B)), Abs(Cos(Pa.C)-Cos(Pb.C)) )
145    Def FNMabcSIN(Pa,Pb)= Max( Abs(Sin(Pa.A)-Sin(Pb.A)), Abs(Sin(Pa.B)-Sin(Pb.B)), Abs(Sin(Pa.C)-Sin(Pb.C)) )
146    '
147    Rem - Mdms ---------------------------------------------------------------
148    ' Mms menyi milisec alatt tegye meg az utat
149    ' Md a megtenni kívánt út mm-ben
150    ' MsMX max speed amit a kb a robot tud mm/sec
151    ' maximális sebeség a TB szerint ((MsMX*MsOP)/100.0) azaz "törtezünk" ((mm/sec)*70)/100
152    ' minimális ido ami alatt megtenné (Md/((MsMX*MsOP)/100.0)) ez még mm/sec (nem mm/msec) ezért *1000.0
153    '                          3   21             1      23
154    Def FNMdMS(Md,MsMX)=1000.0*(Md/((MsMX*M_OPovrd)/100.0))
155    Def FNMdOVRD(Mms,Md,MsMX)=100.0*FNMdMS(Md,MsMX)/Mms
156    '                         54       3   21             1      234    5
157    'Def FNMdOVRD(Mms,Md,MsMX)=((1000.0*(Md/((MsMX*M_OPovrd)/100.0)))/Mms)*100.0
158    '
159    'Tool (100,100,100,100)
160    Mdarab=0 'Megfogó darab érzékelés tárolása
161    MHibaszam=0 'Hibaszám tárolása
162    MPosS=0 'Pozíció index tárolása
163    MPosX=0 'X pozíció átvétele
164    MPosY=0 'Y pozíció átvétele
165    MPosZ=0 'Z pozíció átvétele
166    MPosA=0 'A pozíció fok átvétele
167    MPosB=0 'B pozíció fok átvétele
168    MPosC=0 'C pozíció fok átvétele
169    MPosXe=0 'X pozíció eltolás átvétele
170    MPosYe=0 'Y pozíció eltolás átvétele
171    MPosZe=0 'Z pozíció eltolás átvétele
172    MPosAe=0 'A pozíció fok eltolás átvétele
173    MPosBe=0 'B pozíció fok eltolás átvétele
174    MPosCe=0 'C pozíció fok eltolás átvétele
175    MPosJ1=0 'J1 tengely fok átvétele
176    MPosJ2=0 'J2 tengely fok átvétele
177    MPosJ3=0 'J3 tengely fok átvétele
178    MPosJ4=0 'J4 tengely fok átvétele
179    MPosJ5=0 'J5 tengely fok átvétele
180    MPosJ6=0 'J6 tengely fok átvétele
181    MPosJ1e=0 'J1 tengely fok eltolás átvétele
182    MPosJ2e=0 'J2 tengely fok eltolás átvétele
183    MPosJ3e=0 'J3 tengely fok eltolás átvétele
184    MPosJ4e=0 'J4 tengely fok eltolás átvétele
185    MPosJ5e=0 'J5 tengely fok eltolás átvétele
186    MPosJ6e=0 'J6 tengely fok eltolás átvétele
187    'MHibaszám jelentései:
188    '900 - Rossz parancs küldése a robotnak
189    '910 - Zero detect
190    '911 - HS1 elött és után nem stimmel a pozició
191    '912 - CAGE BOX detect error
192    '920 - Servo OFF
193    '930 - Range problem *S10
194    '931 - Range problem *S11
195    '999 - Alaphelyzetfelvételi hiba
196    '
197    MTavol=0 'alaphelyzetbe értünk-e már?
198    '===================================================
199    '===================================================
200    '---------------------------------------------------
201    'Helyi változók felsorolás vége------------------
202    '---------------------------------------------------
203    '
204    'Hiba esetén megszakítás generálódik
205   '
206   While (M_Err=0)
207     ioSW = 0
208     If ioHS1=1 Then
209       ioSW = ioCOM
210     EndIf
211     Select ioSW 'Mozgás végrehajtás parancs
212       Case 0 'nope
213         Mms = M_Timer(1)
214         Ms=Mms/1000.0
215         ioMsSLV = Mms
216         ioHS1=0
217         Dly 0.001
218         Break
219       Case 11 'XYZ ABC lineáris
220         GoSub *S11
221         Break
222       Default
223         'IGNORE küldjön hibát
224         GoSub *HS1
225         MHibaszam=900
226         GoSub *HS2
227         Break
228     End Select
229   WEnd
230   End
231   '
232   '
233   '=================================================================
234   '---------------------------------
235   'Szubrutinok következnek-----
236   '---------------------------------
237   '=================================================================
238   '
239   *PXYZABCS '=======================
240     P2=P_Curr
241     Mi = ioARYi
242     For M4=1 To 4
243       M4x96=(M4-1)*96
244       MPosX=M_Inw(MioIXx+M4x96)    'X pozíció átvétele
245       MPosY=M_Inw(MioIXy+M4x96) 'Y pozíció átvétele
246       MPosZ=M_Inw(MioIXz+M4x96) 'Z pozíció átvétele
247       MPosA=M_Inw(MioIXa+M4x96) 'A pozíció fok átvétele
248       MPosB=M_Inw(MioIXb+M4x96) 'B pozíció fok átvétele
249       MPosC=M_Inw(MioIXc+M4x96) 'C pozíció fok átvétele
250       P2.X=(MPosX/16)
251       P2.Y=(MPosY/16)
252       P2.Z=(MPosZ/16)
253       P2.A=Rad(MPosA/128)
254       P2.B=Rad(MPosB/128)
255       P2.C=Rad(MPosC/128)
256       If (Mi+M4)<=MnMAX Then
257         pARY(Mi+M4) = P2
258       EndIf
259     Next M4
260     MPosS=ioMs 'pozíció utazási ido átvétele
261   Return '=======================
262   '
263   *FEEDBACK '=======================
264     Mms = M_Timer(1)
265     ioMsSLV = Mms
266     P3 = P_Curr(1)
267     P4 = P3 'P_Curr(2)
268     ioMx00=P3.X*16
269     ioMy00=P3.Y*16
270     ioMz00=P3.Z*16
271     ioMa00=Deg(P3.A)*128
272     ioMb00=Deg(P3.B)*128
273     ioMc00=Deg(P3.C)*128
274     ioMx00=P4.X*16
275     ioMy00=P4.Y*16
276     ioMz00=P4.Z*16
277     ioMa00=Deg(P4.A)*128
278     ioMb00=Deg(P4.B)*128
279     ioMc00=Deg(P4.C)*128
280     ioARYi = Mi
281     ioARYn = Mn
282     Ms=Mms/1000.0
283   Return '=======================
284   '
285   *WIREptemp12:
286     J2 = PtoJ(P2)
287     Mj0 = J_Curr.J6-(MPI2*Int(J_Curr.J6/MPI2))
288     Mj1 = J2.J6-(MPI2*Int(J2.J6/MPI2))
289     '------------------------
290     '
291     '         FLIP?
292     '
293     '------------------------
294     ' RAD~  6.2  5.5  4.7  3.9  3.1  2.3  1.5  0.7  0
295     ' DEG~  360  315  270  225  180  135   90   45  0
296     Mj2 = Mj1
297     If Mj1<MLm315 Then
298         Mj1 = MLp45
299     ElseIf (Mj1<MLp45) And (Mj0>=MLp45) Then
300         Mj1 = Mj1+MPI2
301     ElseIf (Mj1>MLp405) Then
302         Mj1 = MLp45
303     ElseIf (Mj1>MLp45) And (Mj0<=MLp45) Then
304         Mj1 = Mj1-MPI2
305     EndIf
306     If Mj2 <> Mj1 Then
307       J2.J6 = Mj1
308       P2 = JtoP(J2)
309       ptemp1 = P2
310     EndIf
311   Return
312   '
313   *HS1: '=======================
314     Mwait = 0           ' Mwait         1>0
315     While ioHS1=Mwait   ' WAITforHS1     1
316       GoSub *FEEDBACK
317     WEnd
318     Mi = ioARYi
319     Mn = ioARYn
320     GoSub *PXYZABCS 'P2 kitölti az io-ról
321     Mwait = ioHS1       ' Mwait         0>1
322     ioHS1 = Mwait       ' OUT HS1       1
323     While Mi<Mn
324     '                     Mwait      1   0   1   0   1
325       While ioHS1=Mwait ' WAITforHS1  0   1   0   1   0
326         GoSub *FEEDBACK
327       WEnd
328       'P2-t kitölti az INPról
329       Mi = ioARYi
330       Mn = ioARYn
331       GoSub *PXYZABCS
332       Mwait = ioHS1 ' Mwait     1>0 0>1 1>0 0>1 1>0 0>1
333       ioHS1 = Mwait ' OUT HS1     0   1   0   1   0   1
334     WEnd
335     Mi = 0
336     GoSub *FEEDBACK
337     M_00#=1 ' jelzes R2D2-nak hogy elkezdödött valami
338   Return '=======================
339   '
340   *HS2: '=======================
341     MPosS = 0
342     If M_Svo(1)=0 Then
343       ' Így nem nyírja ki miért lett leállítva
344       If MHibaszam<1 Then
345         MHibaszam=920
346       EndIf
347     EndIf
348     ' Így az R2D2-nál is frissebb adatok lesznek mire olvasnám
349     GoSub *FEEDBACK
350     '
351     ioCOM=MHibaszam 'Hiba státusz küldése
352     ioHS2=1 'ReadyOut=1
353     Wait ioHS2=1 'wait ConfirmIn=1
354     ioCOM=0 'Hibaküldés kinullázása
355     ioHS2=0 'ReadyOut=0
356     ioHS1=0
357     Wait ioHS2=0 'wait ConfirmIn=0
358     M_00#=0
359   Return '=======================
360   '=======================
361   '=================================================================
362   '=================================================================
363   '************************XYZ ABC lineáris**************************
364   '=================================================================
365   '=================================================================
366   *S11:
367     MGY = Mgyors/4
368     ML = Mlass/4
369     '
370     GoSub *HS1 'üzenet megérkezett, parancs elkezdve nyugta
371     '
372     While Mn>=Mi  'pARY
373       Cnt 1
374       P2 = pARY(Mi+1)
375       GoSub *WIREptemp12
376       Mz = Dist(P_Zero,P2)
377       If Mz < Mzero Then
378         MHibaszam=910 'Zero detect!!!
379       Else 'Zero detect!!!
380         'P1,P2 azonos? P1
381         Md=Dist(P_Curr,P2)
382         Ma=FNMabcCOS(P_Curr,P2)
383         If Ma = 0 Then
384           Ma=FNMabcSIN(P_Curr,P2)
385         EndIf
386         '-------------------------
387         If Md < 33 And Md = 0 Then
388           Mvs P2 Type 0,0
389         ElseIf (Ma+Md)=0 Then  ' MOVE? TURN?
390           MHibaszam=0
391         Else ' MOVE? TURN?
392           P1 = P2
393           If M_Svo=0 Then
394             If M_Run(3)=1 Then
395               Servo On 'szervo
396             EndIf ' R2D2.prg
397           EndIf ' M_Servo?
398           Fsc Off
399           M_Tool=0
400           ' speed
401           MlstV=100
402           If Md>0 Then
403             If MPosS > 0 Then
404               MlstV=FNMdOVRD(MPosS,Md,MmxSPD)
405               MlstV=Max(1,MlstV)
406               MlstV=Min(100,MlstV)
407             Else
408               MPosS=FNMdMS(Md,MmxSPD)
409             EndIf
410           Else
411             MPosS=FNMdMS(100.0,MmxSPD)
412           EndIf
413           Ovrd MlstV 'MSebesseg 'Sebesség
414           Accel MGY,ML 'Gyorsulás
415           Mcage=Sqr(3.0*(1090.0*1090.0)) 'kb ~1385mm
416           M2=Abs(P2.A)
417             If M2<Abs(P2.B) Then M2=Abs(P2.B)
418             If M2<Abs(P2.C) Then M2=Abs(P2.C)
419           '
420           If (Mz>Mcage) Or (M2>(M_PI*2.0)) Then ' CAGE BOX
421             MHibaszam=912
422           Else ' CAGE BOX
423             MHibaszam=0
424             P0 = P_Curr 'a kiindulási pozició
425             P3 = P2
426             Mti = 0.0
427             Mts = MPosS/4.0
428             swLIN = 1
429             If Mts > 250.0 Then ' Mts
430               J0 = PtoJ(P0)
431               J1 = PtoJ(P1)
432               While (Mti<MPosS) And (ioHS1=0)
433                 'Cnt 1
434                 Mti=Mti+Mts
435                 r1 = Mti/MPosS
436                 J2.J1 = (J1.J1-J0.J1)*r1+J0.J1
437                 J2.J2 = (J1.J2-J0.J2)*r1+J0.J2
438                 J2.J3 = (J1.J3-J0.J3)*r1+J0.J3
439                 J2.J4 = (J1.J4-J0.J4)*r1+J0.J4
440                 J2.J5 = (J1.J5-J0.J5)*r1+J0.J5
441                 J2.J6 = (J1.J6-J0.J6)*r1+J0.J6
442                 P2=JtoP(J2)
443                 If swLIN=1 Then
444                   P3.X = (P1.X-P0.X)*r1+P0.X
445                   P3.Y = (P1.Y-P0.Y)*r1+P0.Y
446                   P3.Z = (P1.Z-P0.Z)*r1+P0.Z
447                   P3.A = P2.A
448                   P3.B = P2.B
449                   P3.C = P2.C
450                   J2 = PtoJ(P3)
451                   J2.J6 = (J1.J6-J0.J6)*r1+J0.J6
452                   P2=JtoP(J2)
453                 EndIf
454                 '
455                 GoSub *MOVEt2
456                 GoSub *FEEDBACK
457               WEnd
458               P2 = P1
459             EndIf ' Mts > 250.0
460             GoSub *MOVEt2
461           EndIf ' CAGE BOX
462         EndIf ' MOVE? TURN?
463       EndIf 'Zero detect!!!
464       If MHibaszam = 0 Then
465         Mi = Mi+1
466       Else
467         Mn=Mi
468       EndIf
469       GoSub *FEEDBACK
470     WEnd 'pARY
471     Cnt 0
472     GoSub *HS2 'vége a mûveletnek, várakozás a nyugtázásra
473   Return '=======================
474   '=================================================================
475   '=================================================================
476   '*****************************************************************
477   '=================================================================
478   '=================================================================
479   '
480   *J6xMr1P2:
481     J6.J1 = (J2.J1-J_Curr.J1)*Mr1P2+J_Curr.J1
482     J6.J2 = (J2.J2-J_Curr.J2)*Mr1P2+J_Curr.J2
483     J6.J3 = (J2.J3-J_Curr.J3)*Mr1P2+J_Curr.J3
484     J6.J4 = (J2.J4-J_Curr.J4)*Mr1P2+J_Curr.J4
485     J6.J5 = (J2.J5-J_Curr.J5)*Mr1P2+J_Curr.J5
486     J6.J6 = (J2.J6-J_Curr.J6)*Mr1P2+J_Curr.J6
487   Return '=======================
488   '
489   *P6xMr1P2:
490     GoSub *J6xMr1P2
491     P6=JtoP(J6)
492     P6.X = (P2.X-P_Curr.X)*r1+P_Curr.X
493     P6.Y = (P2.Y-P_Curr.Y)*r1+P_Curr.Y
494     P6.Z = (P2.Z-P_Curr.Z)*r1+P_Curr.Z
495   Return '=======================
496   '
497   *MOVEt2: ' bemenet a ( P2
498     J2 = PtoJ(P2)
499     'old If (Abs(J2.J4)<Rad(90)) And Abs(J2.J5)<Rad(75.0)  Then ' Soft J4
500     M4 = Abs(Deg(J2.J4)-MNMX(4,3)) 'resetnél ki van számolva a paraméterek közepe
501     M5 = Abs(Deg(J2.J5)-MNMX(5,3)) 'resetnél ki van számolva a paraméterek közepe
502     'ezt azért mert TB-n ropant nehéz olvasni a kilométeres sorokat
503     'egyszeru eltérést számol a középtol
504     If (M4<MNMX(4,4)) And (M5<MNMX(5,4)) Then ' Hard J4
505       ' nem ez Soft J4
506       Mj0 = J_Curr.J6-MPI2*Int(J_Curr.J6/MPI2)
507       Mj1 = J2.J6-MPI2*Int(J2.J6/MPI2)
508       If Mj0 = Mj1 Then
509         Mvs P2 Type 0,0
510       Else
511         Mj2 = Mj1
512         Mr2 = Mj1-Mj0
513         If Mr2 < 0 Then ' N <-+ minus +- P ==================================
514           If (Mj0>MLp45) And (Mj1<MLp45) Then
515             ' FLIP!
516             Mj1 = Mj1+M_PI
517             Mj2 = Mj1+M_PI
518             Mr2 = Mj1-Mj0
519             Mr1P2 = Mr1/Mr2 'N/N
520             GoSub *J6xMr1P2
521             J6.J6 = Mj1
522             Mov J6 Type 0,0
523             Mj0 = J_Curr.J6
524             Mr2 = Mj1-Mj0
525           EndIf
526         ElseIf Mr2 > 0 Then ' N -+ plus +-> P ==================================
527           If (Mj0<MLp45) And (Mj1>MLp45) Then
528             ' FLIP!
529             Mj1 = Mj1-M_PI
530             Mj2 = Mj1-M_PI
531             Mr2 = Mj1-Mj0
532             Mr1P2 = Mr1/Mr2 'N/N
533             GoSub *J6xMr1P2
534             J6.J6 = Mj1
535             Mov J6 Type 0,0
536             Mj0 = J_Curr.J6
537             Mr2 = Mj1-Mj0
538           EndIf
539         EndIf
540         If Mr2 < 0 Then ' N <-+ minus +- P ==================================
541           If (Mj0>MLm90) And (Mj1<MLm90) Then
542             Mr1 = MLm90-Mj0 'N
543             Mr1P2 = Mr1/Mr2 'N/N
544             GoSub *P6xMr1P2
545             Mvs P6 Type 0,0
546             Mj0 = J_Curr.J6
547             Mr2 = Mj1-Mj0
548           EndIf
549           ' -m1 <-0- +m0 -----------------------------------
550           If Mj0>0 And Mj1<0 Then
551             If Mj0>MLp1 Then
552               Mr1 = MLp1-Mj0 'P -> N
553               Mr1P2 = Mr1/Mr2 'N/N
554               GoSub *P6xMr1P2
555               Mvs P6 Type 0,0
556               Mj0 = J_Curr.J6
557               Mr2 = Mj1-Mj0
558             EndIf
559             If Mr2 < MLm1 Then
560               Mr1 = MLm1
561               Mr1P2 = Mr1/Mr2 'N/N
562             Else
563               Mr1 = Mr2
564               Mr1P2 = 1.0
565             EndIf
566             GoSub *J6xMr1P2
567             Mov J6 Type 0,0
568             Mj0 = J_Curr.J6
569             Mr2 = Mj1-Mj0
570           EndIf
571           ' -m1 <- -m0
572         ElseIf Mr2 > 0 Then ' N -+ plus +-> P ==================================
573           If Mj0<MLm90 And Mj1>MLm90 Then
574             Mr1 = MLm90-Mj0 'pozitive
575             Mr1P2 = Mr1/Mr2 'pozitive
576             GoSub *P6xMr1P2
577             Mvs P6 Type 0,0
578             Mj0 = J_Curr.J6
579             Mr2 = Mj1-Mj0
580           EndIf
581           ' -m1 -0-> +m0 -----------------------------------
582           If Mj0<0 And Mj1>0 Then
583             If Mj0<MLm1 Then
584               Mr1 = MLm1-Mj0 'N -> P
585               Mr1P2 = Mr1/Mr2 'P/P
586               GoSub *P6xMr1P2
587               Mvs P6 Type 0,0
588               Mj0 = J_Curr.J6
589               Mr2 = Mj1-Mj0
590             EndIf
591             If Mr2 > MLp1 Then
592               Mr1 = MLp1
593               Mr1P2 = Mr1/Mr2 'P/P
594             Else
595               Mr1 = Mr2
596               Mr1P2 = 1.0
597             EndIf
598             GoSub *J6xMr1P2
599             Mov J6 Type 0,0
600             Mj0 = J_Curr.J6
601             Mr2 = Mj1-Mj0
602           EndIf
603         EndIf
604         '
605         If Mr2 <> 0 Then
606           Mvs P2 Type 0,0
607         EndIf
608       EndIf
609     Else ' Hard J4
610       GoSub *MOVEj2
611     EndIf ' Hard J4
612   Return '=======================
613   '
614   *MOVEj2: ' bemenet a ( J2
615     J0=J_Curr
616     ' FLIP? 4 ax
617     If M4>MNMX(4,4) Then
618       If Deg(J2.J4)>MNMX(4,3) Then
619         J2.J4=J2.J4-M_PI
620       Else
621         J2.J4=J2.J4+M_PI
622       EndIf
623       ' FLIP 5 ax
624       J2.J5=J2.J5*-1.0
625       M5 = Abs(Deg(J2.J5)-MNMX(5,3)) 'lehet így mar jó?
626       ' FLIP 6 ax
627       J2.J6=J2.J6*-1.0
628     EndIf
629     If M5>MNMX(5,4) Then
630       MHibaszam=9115
631       If Deg(J2.J5)>MNMX(5,3) Then
632         J2.J5=M_PI*((MNMX(5,2)-Max5off)/180.0) 'maximum
633       Else
634         J2.J5=M_PI*((MNMX(5,1)+Max5off)/180.0) 'minimum
635       EndIf
636     EndIf
637     ' MIDLE J1
638     J1.J1 = (J2.J1-J_Curr.J1)*Mmid + J_Curr.J1
639     J1.J2 = (J2.J2-J_Curr.J2)*Mmid + J_Curr.J2
640     J1.J3 = (J2.J3-J_Curr.J3)*Mmid + J_Curr.J3
641     J1.J4 = (J2.J4-J_Curr.J4)*Mmid + J_Curr.J4
642     J1.J5 = (J2.J5-J_Curr.J5)*Mmid + J_Curr.J5
643     J1.J6 = (J2.J6-J_Curr.J6)*Mmid + J_Curr.J6
644     Mov J1 Type 0,0 'WthIf M_Ratio >= Mhs2rt, ioHS2=1
645   Return '=======================
646   '
647 '
648 '
649 'Kártya Inputok:
650 '0 - Gyárilag Foglalt
651 '1 -
652 '2 -
653 '3 -
654 '4 -
655 '5 -
656 '6 -
657 '7 -
658 '8 -
659 '9 -
660 '10 -
661 '11 -
662 '12 -
663 '13 -
664 '14 -
665 '15 -
666 '
667 'Kártya Outputok:
668 '0 -
669 '1 -
670 '2 -
671 '3 -
672 '4 -
673 '5 -
674 '6 -
675 '7 -
676 '8 -
677 '9 -
678 '10 -
679 '11 -
680 '12 -
681 '13 -
682 '14 -
683 '15 -
684 '
685 '
686 'Tool-ok:
687 '0=semmilyen tool
J0=(-77.560,-10.270,121.390,0.000,68.880,-304.560,0.000,0.000)
J1=(-4.090,-9.000,122.110,0.000,66.890,-231.090,0.000,0.000)
J2=(-4.090,-9.000,122.110,0.000,66.890,-231.090,0.000,0.000)
J6=(-6.510,-0.870,112.990,0.000,67.890,-90.000,0.000,0.000)
jAlaph=(2.600,-33.050,136.100,0.000,76.950,92.600)
jFelvetel=(2.600,-33.050,136.100,0.000,76.950,92.600)
jKamera=(2.600,-33.050,136.100,0.000,76.950,92.600)
jLerakas=(2.600,-33.050,136.100,0.000,76.950,92.600)
jMurites=(2.600,-33.050,136.100,0.000,76.950,92.600)
Jo1=(0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000)
jSzerviz=(2.600,-33.050,136.100,0.000,76.950,92.600)
jtemp0=(-7.340,-0.480,114.570,-1.490,74.210,-54.020)
jtemp1=(-3.050,-9.150,122.230,-8.210,66.400,-47.540)
jtemp2=(-4.090,-9.000,122.110,0.000,66.890,-52.090,0.000,0.000)
jtemp3=(0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000)
jtemp4=(0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000)
jtemp6=(92.300,17.440,98.380,0.000,64.180,-177.700)
P0=(150.020,-679.990,720.990,-180.000,0.000,47.000,0.000,0.000)(7,15728640)
P1=(700.000,-50.000,700.000,-180.000,0.000,47.000,0.000,0.000)(7,15728640)
P2=(700.000,-50.000,700.000,-180.000,0.000,47.000)(7,15728640)
P3=(700.000,-50.000,700.000,180.000,0.000,47.000,0.000,0.000)(7,15728640)
P4=(700.000,-50.000,700.000,180.000,0.000,47.000,0.000,0.000)(7,15728640)
P6=(800.000,-100.000,721.000,-180.000,0.000,-96.510,0.000,0.000)(7,0)
Po1=(0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000)
