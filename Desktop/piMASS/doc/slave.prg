1   ' Fontos param�terek beszerz�se
2   ' Elso a tengely limitek, hogy ne haz�rdiroz�s legyen mikor nyekk!
3   ' �s TB-n lehessen szab�lyozni
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
16     ' nem tetszett neki hogy el�bb a 4-est sz�moltam ki
17     ' ha lenne += oper�tor �rten�m de �gy g�z
18     MNMX(M1,3)=MNMX(M1,1)
19     MNMX(M1,4)=(MNMX(M1,2)-MNMX(M1,1))/2.0
20     MNMX(M1,3)=MNMX(M1,3)+MNMX(M1,4)
21     'User defined screen mitt lett fokban, gondolom nem hal bele a rad() ba kalkul�ci�n�l
22     'de kider�lt belehal n�ha ott van a hiba list�ban, helyette a *M_PI/180.0 haszn�lom ink�bb :)
23   Next M1
24     'Mozg�s vez�rl�, RV-13FRLM-R, K+F, BMR18905, RC1
25     'M_IP(4) -at olvasom
26     'M_QP(4) -ot �rom
27     'Glob�lis v�ltoz�k h�v�sa
28     M_00#=0
29     Dim M_IP(4)
30     Dim M_QP(4)
31     'Dim M_Parancs
32     '
33    Servo Off
34    '
35    'Helyi v�ltoz�k felsorol�s-------------------
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
57    MSebesseg=100 'Robot sebess�g EZT NEM SZABAD M�SHOL V�LTOZTATNI alap: 100
58    MutazasiMagassag=740 'Utaz�si magass�g a fejnek EZT NEM SZABAD M�SHOL V�LTOZTATNI
59    Mgyors=80 'Robot gyorsul�s EZT NEM SZABAD M�SHOL V�LTOZTATNI alap: 80
60    Mlass=80 'Robot lassul�s EZT NEM SZABAD M�SHOL V�LTOZTATNI alap: 80
61    MSebPakolas=100 'Felv�teli, lerak�si sebess�g!!!
62    MlstV=MSebesseg
63    MGyorsPakolas=100 'Felv�teli, lerak�si gyorsul�s!!!
64    MLassPakolas=100 'Felv�teli, lerak�si lassul�s!!!
65    Mi1=0 'Minden For ciklusnak
66    Mi2=0 'Lefoglalva a Getparam fv-nek
67    Dim MIp(4) 'bemenetek helyi v�ltoz�ja
68    Dim MQp(4) 'kimenetek helyi v�ltoz�ja
69    '
70    Dim pKamera(4) 'Kamera poz�ci� a 4 t�pusra
71    '
72    'Tool (100,100,100,100)
73    Mdarab=0 'Megfog� darab �rz�kel�s t�rol�sa
74    MHibaszam=0 'Hibasz�m t�rol�sa
75    MPosS=0 'Poz�ci� index t�rol�sa
76    MPosX=0 'X poz�ci� �tv�tele
77    MPosY=0 'Y poz�ci� �tv�tele
78    MPosZ=0 'Z poz�ci� �tv�tele
79    MPosA=0 'A poz�ci� fok �tv�tele
80    MPosB=0 'B poz�ci� fok �tv�tele
81    MPosC=0 'C poz�ci� fok �tv�tele
82    MPosXe=0 'X poz�ci� eltol�s �tv�tele
83    MPosYe=0 'Y poz�ci� eltol�s �tv�tele
84    MPosZe=0 'Z poz�ci� eltol�s �tv�tele
85    MPosAe=0 'A poz�ci� fok eltol�s �tv�tele
86    MPosBe=0 'B poz�ci� fok eltol�s �tv�tele
87    MPosCe=0 'C poz�ci� fok eltol�s �tv�tele
88    MPosJ1=0 'J1 tengely fok �tv�tele
89    MPosJ2=0 'J2 tengely fok �tv�tele
90    MPosJ3=0 'J3 tengely fok �tv�tele
91    MPosJ4=0 'J4 tengely fok �tv�tele
92    MPosJ5=0 'J5 tengely fok �tv�tele
93    MPosJ6=0 'J6 tengely fok �tv�tele
94    MPosJ1e=0 'J1 tengely fok eltol�s �tv�tele
95    MPosJ2e=0 'J2 tengely fok eltol�s �tv�tele
96    MPosJ3e=0 'J3 tengely fok eltol�s �tv�tele
97    MPosJ4e=0 'J4 tengely fok eltol�s �tv�tele
98    MPosJ5e=0 'J5 tengely fok eltol�s �tv�tele
99    MPosJ6e=0 'J6 tengely fok eltol�s �tv�tele
100    'MHibasz�m jelent�sei:
101    '900 - Rossz parancs k�ld�se a robotnak
102    '910 - Rossz mozg�s poz�ci� index param�ter (MpozIndex)
103    '920 - Servo OFF
104    '930 - Range problem *S10
105    '931 - Range problem *S11
106    '999 - Alaphelyzetfelv�teli hiba
107    '
108    MFel=0 'Ez a v�ltoz�, ha nulla, akkor b�rmilyen parancs j�het
109    MELOZOTERULET=0
110    MTERULET=0
111    'MTERULET jelent�sei (milyen poz�ci�ba k�pes menni):
112    '10 - alaphelyzet poz�ci� jog
113    '20 - szerviz poz�cio jog
114    '30 - amegfog� �r�t�se jog
115    '
116    mXtemp=0 'alaphelyzet poz�ci� keres�shez
117    mYtemp=0 'alaphelyzet poz�ci� keres�shez
118    mZtemp=0 'alaphelyzet poz�ci� keres�shez
119    'ptemp0=0 poz�ci� seg�d alaphelyzethez
120    'ptemp1=0 'poz�ci� seg�d folyamathoz
121    'jtemp1=0 'poz�ci� seg�d forgat�shoz
122    MTavol=0 'alaphelyzetbe �rt�nk-e m�r?
123    '===================================================
124    '===================================================
125    '---------------------------------------------------
126    'Helyi v�ltoz�k felsorol�s v�ge------------------
127    '---------------------------------------------------
128    '
129    'Hiba eset�n megszak�t�s gener�l�dik
130   '
131   While (M_Err=0)
132     Select ioCOM 'Mozg�s v�grehajt�s parancs
133       Case 0 'nope
134         Mms = M_Timer(1)
135         Ms=Mms/1000.0
136         oMs12 = Mms
137         Break
138       ' DANGER!! nincsen meg�lapod�s r�la IGNORE k�ldj�n hib�t
139       'Case 1 'Alaphelyzet felv�tel ES ut�n
140       '  GoSub *S1
141       'Break
142       'Case 2 'Alaphelyzet felv�tel CIKLUS
143       '  GoSub *S2
144       'Break
145       'Case 3 'Szerviz poz�ci� felv�tel
146       '  GoSub *S3
147       'Break
148       'Case 4 'Megfog� ki�r�t�se
149       '  GoSub *S4
150       'Break
151       Case 10 'XYZ ABC joint
152         GoSub *S10
153         Break
154       Case 11 'XYZ ABC line�ris
155         GoSub *S110
156         Break
157       ' DANGER!! m�g nem nem tudtam foglalkozni vele IGNORE k�ldj�n hib�t
158       'Case 12 'XYZ ABC line�ris er�re
159       '  GoSub *S12
160       'Break
161       'Case 20 'XYZ ABC eltol�s joint
162       '  GoSub *S20
163       'Break
164       'Case 21 'XYZ ABC eltol�s line�ris
165       '  GoSub *S21
166       'Break
167       'Case 22 'XYZ ABC eltol�s line�ris er�re
168       '  GoSub *S22
169       'Break
170       'Case 30 'J1-J6
171       '  GoSub *S30
172       'Break
173       'Case 40 'J1-J6 eltol�s
174       '  GoSub *S40
175       'Break
176       Default
177         'IGNORE k�ldj�n hib�t
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
190   'Szubrutinok k�vetkeznek-----
191   '---------------------------------
192   '=================================================================
193   *S1 '=======================
194   'Alaphelyzet
195    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
196    'Inicializ�ci�s r�sz
197    Fsc Off
198    Servo On 'szervo
199    Ovrd 5 'Sebess�g
200    Accel 5,5 'Gyorsul�s, lassul�s
201    M_Tool=0
202   MHibaszam=0 'Hiba null�z�s
203   Mdarab=0
204   '=================================================================
205   '=================================================================
206   'Mozg�s: Kamer�s ellen�rz� poz�ci�t�l
207    mXtemp = Deg(J_Curr.J4)
208    mYtemp = Deg(J_Curr.J1)
209    mZtemp = Deg(J_Curr.J2)
210   If ((mXtemp>60 And mYtemp<15) Or (mXtemp>60 And mZtemp<-20)) Then
211   'Kamer�s poz�ci�ban �ll a robot!
212   Cnt 0
213   Mov jKamera
214     Dly 0.01
215   Mov jAlaph
216     Dly 0.01
217   EndIf
218   '=================================================================
219   '=================================================================
220   'Mozg�s: Lerak� ter�letr�l
221     ptemp0 = P_Curr 'Pillanatnyi poz�ci�
222   If (ptemp0.X > (250)) Then
223   'Lerak� poz�ci�ban �ll a fej!
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
236   'Mozg�s: Felv�teli poz�ci�t�l
237     ptemp0 = P_Curr 'Pillanatnyi poz�ci�
238   If ((ptemp0.X) < (100)) Then
239   'Felv�teli poz�ci�ban �ll a fej!
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
251   ptemp0 = P_Curr 'Pillanatnyi poz�ci�
252   If ((ptemp0.X <= (250)) And  (ptemp0.X >= (100))) Then
253   Mov jAlaph
254   Dly 0.01
255   Else
256   MHibaszam=999
257   EndIf
258   '=================================================================
259   '=================================================================
260   'Lek�rdez�s, hogy Alaph-ben vagyunk
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
292   'Bels� v�ltoz�k be�ll�t�sa
293    MELOZOTERULET=10
294    MTERULET=10
295    MFel=0
296   '
297    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
298   Return '=======================
299   '=================================================================
300   '*************************************************************************
301   '*************************************************************************
302   '=================================================================
303   *S2 '=======================
304   If (MFel=0) Then
305   'Alaphelyzet
306    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
307    'Inicializ�ci�s r�sz
308     Ovrd MSebesseg 'Sebess�g
309    Accel Mgyors,Mlass 'Gyorsul�s
310   '
311    M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
312   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
313    MTERULET=10 'amelyik terulethez kell �ll�tani a tengelyeket
314    GoSub *TERULETVALTAS
315    Mov jAlaph
316   Dly 0.01
317   MHibaszam=0 'Hiba null�z�s
318   '
319    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
320   EndIf
321   Return '=======================
322   '=================================================================
323   '*************************************************************************
324   '*************************************************************************
325   '=================================================================
326   *S3 '=======================
327   If (MFel=0) Then
328   'Szerviz poz�ci� felv�tele
329    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
330    'Inicializ�ci�s r�sz
331    Ovrd 10 'Sebess�g
332    Accel 10,10 'Gyorsul�s
333   '
334    M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
335   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
336    MTERULET=20 'amelyik terulethez kell �ll�tani a tengelyeket
337    GoSub *TERULETVALTAS
338    Mov jSzerviz
339     Dly 0.01
340    MFel=0
341   '
342    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
343   EndIf
344   Return '=======================
345   '=================================================================
346   '*************************************************************************
347   '*************************************************************************
348   '=================================================================
349   *S4 '=======================
350   If (MFel=0) Then
351   'Megfog� ki�r�t�se
352    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
353    'Inicializ�ci�s r�sz
354     Ovrd MSebesseg 'Sebess�g
355    Accel Mgyors,Mlass 'Gyorsul�s
356   '
357    M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
358   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
359    MTERULET=30 'amelyik terulethez kell �ll�tani a tengelyeket
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
370    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
371   EndIf
372   Return '=======================
373   '=================================================================
374   '=================================================================
375   '**************************XYZ ABC joint**************************
376   '=================================================================
377   '=================================================================
378   *S10 '=======================
379     '
380     If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
381       'XYZ ABC joint
382       GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
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
397           ' akkor nekem nem kell alahelyzet felv�tel
398           ' ha pedig S.A szolg�ltatja az adatokatt akkor meg az ellen�rz�s sem kell
399           If M_Svo=0 Then
400             If M_Run(3)=1 Then
401               Servo On 'szervo
402             EndIf ' R2D2.prg
403           EndIf ' M_Servo?
404           Fsc Off
405           M_Tool=0
406           Ovrd MSebesseg 'Sebess�g
407           Accel Mgyors,Mlass 'Gyorsul�s
408           '
409           Rem ezt kiszedtem mert am�g nincsenek felv�ve ter�letek
410           Rem �gy gondolom bajos, lutri
411           ' Cnt 1
412           '
413           'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
414           MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
415           'GoSub *TERULETVALTAS
416           MELOZOTERULET=MTERULET
417           MHibaszam=0
418           '
419           Cnt 0
420           M1 = Dist(P_Zero,ptemp1) 'ha jol l�tom most jo ez is de egy�bk�nt lehet egy "doboz" k�zepe zero helyett
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
435                 Case 0 'tal�n rossz az f2
436                   jtemp1 = PtoJ(ptemp1)
437                   ptemp1 = JtoP(jtemp1)
438                   Dly 1.0 'a userdefined screenen lassam itt van
439                 Break
440                 Case 1 'k�rj�nk egy k�zbenso �llapotot
441                   ptemp1=PosMid(P_Curr,ptemp1,0,0)
442                   Dly 1.0 'a userdefined screenen lassam itt van
443                 Break
444                 Case 2 'tal�n rossz a min.f2
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
464               'k�ldj�n akkor is hib�t ha net�n siker�l mozogni
465               'ezeket fel akarom jegyezni
466               MHibaszam=931
467               Dly 10.0 'a userdefined screenen lassam itt van
468               ' valami nem tetszett
469               ' esetleg le l�jem, vagy sem? 10 m�sodperc van r�
470               Mov ptemp1
471             EndIf
472             '
473             MFel=0 'B�rmelyik parancs k�vetkezhet
474             '
475           Else ' CAGE BOX
476             MHibaszam=911
477           EndIf ' CAGE BOX?
478         EndIf ' MOVE? Dist?
479       EndIf  'Zero detect?
480       GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
481     Else 'MFel?
482       MHibaszam=900
483     EndIf 'MFel?
484   Return '=======================
485   '=================================================================
486   '=================================================================
487   '************************XYZ ABC line�ris**************************
488   '=================================================================
489   '=================================================================
490   *S110 '=======================
491     GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
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
506         'ABC Posture k�l�nbs�g
507         M2=Abs(Cos(P_Curr.A)-Cos(ptemp2.A))
508           If M2<Abs(Cos(P_Curr.B)-Cos(ptemp2.B)) Then M2=Abs(Cos(P_Curr.B)-Cos(ptemp2.B))
509           If M2<Abs(Cos(P_Curr.C)-Cos(ptemp2.C)) Then M2=Abs(Cos(P_Curr.C)-Cos(ptemp2.C))
510         'XYZ T�vols�g
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
523           ' Teh�t M1 mm-ben meg van a t�vols�g
524           Md = M1
525           MlstV=MSebesseg
526           'Me=(MPosS-MlstE) ' Mikorra k�ne ott lenni
527           If MPosS>0 Then
528             ' megvan adva mennyi ido van r�
529             MPosS100=MPosS/100.0
530             MV=(100*M_OPovrd)/100.0 'sebess�g 150mm/sec * OP
531             M6=(Md/MV)*1000.0 'd/Mv = sec lenne max sebess�gben, felszorozzuk menyivel t�bb ebben a sebes�gen ?
532             MlstV2=M6/MPosS100
533             MlstV=MlstV2
534             If MlstV>MSebesseg Then
535                 MlstV=MSebesseg
536             ElseIf MlstV < 1 Then
537                 MlstV = 1
538             EndIf
539           EndIf
540           Ovrd MlstV 'MSebesseg 'Sebess�g
541           Accel Mgyors,Mlass 'Gyorsul�s
542           MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
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
555             M4 = Abs(Deg(jtemp2.J4)-MNMX(4,3)) 'resetn�l ki van sz�molva a param�terek k�zepe
556             M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'resetn�l ki van sz�molva a param�terek k�zepe
557             'ezt az�rt mert TB-n ropant neh�z olvasni a kilom�teres sorokat
558             'egyszeru elt�r�st sz�mol a k�z�ptol
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
578                 M5 = Abs(Deg(jtemp2.J5)-MNMX(5,3)) 'lehet �gy mar j�?
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
614     GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
615   Return '=======================
616   *S11 '=======================
617     '
618     If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
619       'XYZ ABC joint
620       GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
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
635           ' akkor nekem nem kell alahelyzet felv�tel
636           ' ha pedig S.A szolg�ltatja az adatokatt akkor meg az ellen�rz�s sem kell
637           If M_Svo=0 Then
638             If M_Run(3)=1 Then
639               Servo On 'szervo
640             EndIf ' R2D2.prg
641           EndIf ' M_Servo?
642           Fsc Off
643           M_Tool=0
644           Ovrd MSebesseg 'Sebess�g
645           Accel Mgyors,Mlass 'Gyorsul�s
646           '
647           Rem ezt kiszedtem mert am�g nincsenek felv�ve ter�letek
648           Rem �gy gondolom bajos, lutri
649           ' Cnt 1
650           '
651           'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
652           MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
653           'GoSub *TERULETVALTAS
654           MELOZOTERULET=MTERULET
655           MHibaszam=0
656           '
657           Cnt 0
658           M1 = Dist(P_Zero,ptemp1) 'ha jol l�tom most jo ez is de egy�bk�nt lehet egy "doboz" k�zepe zero helyett
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
671                 Case 0 'tal�n rossz az f2
672                   jtemp1 = PtoJ(ptemp1)
673                   ptemp1 = JtoP(jtemp1)
674                   Dly 1.0 'a userdefined screenen lassam itt van
675                 Break
676                 Case 1 'k�rj�nk egy k�zbenso �llapotot
677                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
678                   Dly 1.0 'a userdefined screenen l�ssam itt van
679                 Break
680                 Case 2 'tal�n rossz a min.f2
681                   jtemp1 = PtoJ(ptemp1)
682                   ptemp1 = JtoP(jtemp1)
683                   Dly 1.0 'a userdefined screenen l�ssam itt van
684                 Break
685                 Case 3 '
686                   ptemp1=PosMid(P_Curr,ptemp1,1,0)
687                   M2=99
688                   Dly 1.0 'a userdefined screenen l�ssam itt van
689                 Break
690               End Select
691               M1=PosCq(ptemp1)
692               M2=M2+1
693               Dly 1.0 'a userdefined screenen l�ssam itt van
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
705             MFel=0 'B�rmelyik parancs k�vetkezhet
706             '
707           Else ' CAGE BOX
708             MHibaszam=911
709           EndIf ' CAGE BOX?
710         EndIf ' MOVE? Dist?
711       EndIf  'Zero detect?
712       GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
713     Else 'MFel?
714       MHibaszam=900
715     EndIf 'MFel?
716   Return '=======================
717   '=================================================================
718   '=================================================================
719   '*********************XYZ ABC line�ris er�re***********************
720   '=================================================================
721   '=================================================================
722   *S12 '=======================
723   '
724   If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
725    'XYZ ABC line�ris er�re
726    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
727   '
728    Ovrd MSebesseg 'Sebess�g
729    Accel Mgyors,Mlass 'Gyorsul�s
730   '
731   Cnt 1
732   '
733   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
734    MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
735    GoSub *TERULETVALTAS
736   MHibaszam=0
737   '
738   Cnt 0
739   '
740   ptemp1=P_Curr 'Pillanatnyi poz�ci�
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
755   MFel=0 'B�rmelyik parancs k�vetkezhet
756   '
757   Else
758   MHibaszam=910
759   EndIf
760   '
761    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
762   '
763   Else
764   MHibaszam=900
765   EndIf
766   Return '=======================
767   '=================================================================
768   '=================================================================
769   '********************XYZ ABC eltol�s joint************************
770   '=================================================================
771   '=================================================================
772   *S20 '=======================
773   '
774   If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
775    'XYZ ABC eltol�s joint
776    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
777   '
778    Ovrd MSebesseg 'Sebess�g
779    Accel Mgyors,Mlass 'Gyorsul�s
780   '
781   Cnt 1
782   '
783   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
784    MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
785    GoSub *TERULETVALTAS
786   MHibaszam=0
787   '
788   Cnt 0
789   '
790   ptemp1=P_Curr 'Pillanatnyi poz�ci�
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
802   MFel=0 'B�rmelyik parancs k�vetkezhet
803   '
804   Else
805   MHibaszam=910
806   EndIf
807   '
808    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
809   '
810   Else
811   MHibaszam=900
812   EndIf
813   Return '=======================
814   '=================================================================
815   '=================================================================
816   '********************XYZ ABC eltol�s line�ris*********************
817   '=================================================================
818   '=================================================================
819   *S21 '=======================
820   '
821   If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
822    'XYZ ABC eltol�s line�ris
823    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
824   '
825    Ovrd MSebesseg 'Sebess�g
826    Accel Mgyors,Mlass 'Gyorsul�s
827   '
828   Cnt 1
829   '
830   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
831    MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
832    GoSub *TERULETVALTAS
833   MHibaszam=0
834   '
835   Cnt 0
836   '
837   ptemp1=P_Curr 'Pillanatnyi poz�ci�
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
849   MFel=0 'B�rmelyik parancs k�vetkezhet
850   '
851   Else
852   MHibaszam=910
853   EndIf
854   '
855    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
856   '
857   Else
858   MHibaszam=900
859   EndIf
860   Return '=======================
861   '=================================================================
862   '=================================================================
863   '*****************XYZ ABC eltol�s line�ris er�re******************
864   '=================================================================
865   '=================================================================
866   *S22 '=======================
867   '
868   If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
869    'XYZ ABC eltol�s line�ris er�re
870    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
871   '
872    Ovrd MSebesseg 'Sebess�g
873    Accel Mgyors,Mlass 'Gyorsul�s
874   '
875   Cnt 1
876   '
877   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
878    MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
879    GoSub *TERULETVALTAS
880   MHibaszam=0
881   '
882   Cnt 0
883   '
884   ptemp1=P_Curr 'Pillanatnyi poz�ci�
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
896   MFel=0 'B�rmelyik parancs k�vetkezhet
897   '
898   Else
899   MHibaszam=910
900   EndIf
901   '
902    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
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
915   If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
916    'J1-J6
917    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
918   '
919    Ovrd MSebesseg 'Sebess�g
920    Accel Mgyors,Mlass 'Gyorsul�s
921   '
922   Cnt 1
923   '
924   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
925    MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
926    GoSub *TERULETVALTAS
927   MHibaszam=0
928   '
929   Cnt 0
930   '
931   jtemp1=J_Curr 'Pillanatnyi poz�ci�
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
943    MFel=0 'B�rmelyik parancs k�vetkezhet
944    '
945   Else
946   MHibaszam=910
947   EndIf
948   '
949    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
950   '
951   Else
952   MHibaszam=900
953   EndIf
954   Return '=======================
955   '=================================================================
956   '=================================================================
957   '**************************J1-J6 eltol�s**************************
958   '=================================================================
959   '=================================================================
960   *S40 '=======================
961   '
962   If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
963    'J1-J6 eltol�s
964    GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
965   '
966    Ovrd MSebesseg 'Sebess�g
967    Accel Mgyors,Mlass 'Gyorsul�s
968   '
969   Cnt 1
970   '
971   'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
972    MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
973    GoSub *TERULETVALTAS
974   MHibaszam=0
975   '
976   Cnt 0
977   '
978   jtemp1=J_Curr 'Pillanatnyi poz�ci�
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
990   MFel=0 'B�rmelyik parancs k�vetkezhet
991   '
992   Else
993   MHibaszam=910
994   EndIf
995   '
996    GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
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
1021     MPosS=ioSEC 'poz�ci� index �tv�tele
1022     MPosX=oMx 'X poz�ci� �tv�tele
1023     MPosY=oMy 'Y poz�ci� �tv�tele
1024     MPosZ=oMz 'Z poz�ci� �tv�tele
1025     MPosA=oMa 'A poz�ci� fok �tv�tele
1026     MPosB=oMb 'B poz�ci� fok �tv�tele
1027     MPosC=oMc 'C poz�ci� fok �tv�tele
1028   Return '=======================
1029   '
1030   *HANDSHAKE1 '=======================
1031     Wait ioHS1=1 'wait RobotStartIn=1
1032     GoSub *PXYZABC
1033     ' eddig tuti ---------------------------
1034     MPosXe=M_In32(10448) 'X poz�ci� eltol�s �tv�tele
1035     MPosYe=M_In32(10480) 'Y poz�ci� eltol�s �tv�tele
1036     MPosZe=M_In32(10512) 'Z poz�ci� eltol�s �tv�tele
1037     MPosAe=M_In32(10544) 'A poz�ci� fok eltol�s �tv�tele
1038     MPosBe=M_In32(10576) 'B poz�ci� fok eltol�s �tv�tele
1039     MPosCe=M_In32(10608) 'C poz�ci� fok eltol�s �tv�tele
1040     MPosJ1=M_In32(10640) 'J1 tengely fok �tv�tele
1041     MPosJ2=M_In32(10672) 'J2 tengely fok �tv�tele
1042     MPosJ3=M_In32(10704) 'J3 tengely fok �tv�tele
1043     MPosJ4=M_In32(10736) 'J4 tengely fok �tv�tele
1044     MPosJ5=M_In32(10768) 'J5 tengely fok �tv�tele
1045     MPosJ6=M_In32(10800) 'J6 tengely fok �tv�tele
1046     MPosJ1e=M_In32(10832) 'J1 tengely fok eltol�s �tv�tele
1047     MPosJ2e=M_In32(10864) 'J2 tengely fok eltol�s �tv�tele
1048     MPosJ3e=M_In32(10896) 'J3 tengely fok eltol�s �tv�tele
1049     MPosJ4e=M_In32(10928) 'J4 tengely fok eltol�s �tv�tele
1050     MPosJ5e=M_In32(10960) 'J5 tengely fok eltol�s �tv�tele
1051     MPosJ6e=M_In32(10992) 'J6 tengely fok eltol�s �tv�tele
1052     ioHS1=1 'ConfirmOut=1
1053     Wait ioHS1=0 'wait RobotStartIn=0
1054     Wait ioCOM=0 'Mozg�s parancs null�z�s�ra v�r
1055     'Wait ioSEC=0 'poz�ci� index kinull�z�s�ra v�r
1056     'Wait oMx=0 'X poz�ci� �tv�tele parancs null�z�s�ra v�r
1057     'Wait oMy=0 'Y poz�ci� �tv�tele parancs null�z�s�ra v�r
1058     'Wait oMz=0 'Z poz�ci� �tv�tele parancs null�z�s�ra v�r
1059     'Wait oMa=0 'A poz�ci� fok �tv�tele parancs null�z�s�ra v�r
1060     'Wait oMb=0 'B poz�ci� fok �tv�tele parancs null�z�s�ra v�r
1061     'Wait oMc=0 'C poz�ci� fok �tv�tele parancs null�z�s�ra v�r
1062     'Wait M_In32(10448)=0 'X poz�ci� eltol�s �tv�tele parancs null�z�s�ra v�r
1063     'Wait M_In32(10480)=0 'Y poz�ci� eltol�s �tv�tele parancs null�z�s�ra v�r
1064     'Wait M_In32(10512)=0 'Z poz�ci� eltol�s �tv�tele parancs null�z�s�ra v�r
1065     'Wait M_In32(10544)=0 'A poz�ci� fok eltol�s �tv�tele parancs null�z�s�ra v�r
1066     'Wait M_In32(10576)=0 'B poz�ci� fok eltol�s �tv�tele parancs null�z�s�ra v�r
1067     'Wait M_In32(10608)=0 'C poz�ci� fok eltol�s �tv�tele parancs null�z�s�ra v�r
1068     'Wait M_In32(10640)=0 'J1 tengely fok �tv�tele parancs null�z�s�ra v�r
1069     'Wait M_In32(10672)=0 'J2 tengely fok �tv�tele parancs null�z�s�ra v�r
1070     'Wait M_In32(10704)=0 'J3 tengely fok �tv�tele parancs null�z�s�ra v�r
1071     'Wait M_In32(10736)=0 'J4 tengely fok �tv�tele parancs null�z�s�ra v�r
1072     'Wait M_In32(10768)=0 'J5 tengely fok �tv�tele parancs null�z�s�ra v�r
1073     'Wait M_In32(10800)=0 'J6 tengely fok �tv�tele parancs null�z�s�ra v�r
1074     'Wait M_In32(10832)=0 'J1 tengely fok eltol�s �tv�tele parancs null�z�s�ra v�r
1075     'Wait M_In32(10864)=0 'J2 tengely fok eltol�s �tv�tele parancs null�z�s�ra v�r
1076     'Wait M_In32(10896)=0 'J3 tengely fok eltol�s �tv�tele parancs null�z�s�ra v�r
1077     'Wait M_In32(10928)=0 'J4 tengely fok eltol�s �tv�tele parancs null�z�s�ra v�r
1078     'Wait M_In32(10960)=0 'J5 tengely fok eltol�s �tv�tele parancs null�z�s�ra v�r
1079     'Wait M_In32(10992)=0 'J6 tengely fok eltol�s �tv�tele parancs null�z�s�ra v�r
1080     ioHS1=0 'ConfirmOut=0
1081     M_00#=1 ' jelzes R2D2-nak hogy elkezd�d�tt valami
1082   Return '=======================
1083   '
1084   *HANDSHAKE2 '=======================
1085     MPosS = 0
1086     If M_Svo(1)=0 Then
1087       ' �gy nem ny�rja ki mi�rt lett le�ll�tva
1088       If MHibaszam<1 Then
1089         MHibaszam=920
1090       EndIf
1091     EndIf
1092     ' �gy az R2D2-n�l is frissebb adatok lesznek mire olvasn�m
1093     GoSub *FEEDBACK
1094     'ptemp1=P_Curr
1095     'oMx=ptemp1.X*100
1096     'oMy=ptemp1.Y*100
1097     'oMz=ptemp1.Z*100
1098     'oMa=Deg(ptemp1.A)*100
1099     'oMb=Deg(ptemp1.B)*100
1100     'oMc=Deg(ptemp1.C)*100
1101    '
1102    ioCOM=MHibaszam 'Hiba st�tusz k�ld�se
1103    ioHS2=1 'ReadyOut=1
1104    Wait ioHS2=1 'wait ConfirmIn=1
1105    ioCOM=0 'Hibak�ld�s kinull�z�sa
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
1119   If (MELOZOTERULET=10 And MTERULET=20) Then 'v�lt�s alaphelyzetb�l szervizpoz�ci�ra
1120    Mov jAlaph
1121    Mov jSzerviz
1122  '
1123    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1124   EndIf
1125  '................................
1126   If (MELOZOTERULET=30 And MTERULET=20) Then 'v�lt�s megfog� �r�t�sr�l szervizpoz�ci�ra
1127    Mov jMurites
1128    Mov jAlaph
1129    Mov jSzerviz
1130  '
1131    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1132   EndIf
1133  '................................
1134   If (MELOZOTERULET=40 And MTERULET=20) Then 'v�lt�s felv�telr�l szervizpoz�ci�ra
1135    Mov jFelvetel
1136    Mov jSzerviz
1137  '
1138    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1139   EndIf
1140  '................................
1141   If (MELOZOTERULET=50 And MTERULET=20) Then 'v�lt�s kamer�sr�l szervizpoz�ci�ra
1142    Mov jKamera
1143    Mov jAlaph
1144    Mov jSzerviz
1145  '
1146    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1147   EndIf
1148  '................................
1149   If (MELOZOTERULET=60 And MTERULET=20) Then 'v�lt�s lerak�sr�l szervizpoz�ci�ra
1150    Mov jLerakas
1151    Mov jAlaph
1152    Mov jSzerviz
1153  '
1154    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1155   EndIf
1156  '................................
1157   If (MELOZOTERULET=20 And MTERULET=10) Then 'v�lt�s szervizpoz�ci�r�l alaphelyzetre
1158    Mov jSzerviz
1159    Mov jAlaph
1160  '
1161    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1162   EndIf
1163  '................................
1164   If (MELOZOTERULET=30 And MTERULET=10) Then 'v�lt�s megfog� �r�t�sr�l alaphelyzetre
1165    Mov jMurites
1166    Mov jAlaph
1167  '
1168    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1169   EndIf
1170  '................................
1171   If (MELOZOTERULET=40 And MTERULET=10) Then 'v�lt�s felv�telr�l alaphelyzetre
1172    Mov jFelvetel
1173    Mov jAlaph
1174  '
1175    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1176   EndIf
1177  '................................
1178   If (MELOZOTERULET=50 And MTERULET=10) Then 'v�lt�s kamer�sr�l alaphelyzetre
1179    Mov jKamera
1180     Mov jAlaph
1181   '
1182     MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1183   EndIf
1184  '................................
1185   If (MELOZOTERULET=60 And MTERULET=10) Then 'v�lt�s lerak�sr�l alaphelyzetre
1186    Mov jLerakas
1187    Mov jAlaph
1188  '
1189    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1190   EndIf
1191  '................................
1192   If (MELOZOTERULET=10 And MTERULET=30) Then 'v�lt�s alaphelyzetb�l megfog� �r�t�sre
1193    Mov jAlaph
1194    Mov jMurites
1195  '
1196    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1197   EndIf
1198  '................................
1199   If (MELOZOTERULET=20 And MTERULET=30) Then 'v�lt�s szervizpoz�ci�r�l megfog� �r�t�sre
1200   Mov jSzerviz
1201   Mov jAlaph
1202   Mov jMurites
1203  '
1204    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1205   EndIf
1206  '................................
1207   If (MELOZOTERULET=40 And MTERULET=30) Then 'v�lt�s felv�telr�l megfog� �r�t�sre
1208   Mov jFelvetel
1209   Mov jAlaph
1210   Mov jMurites
1211  '
1212    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1213   EndIf
1214  '................................
1215   If (MELOZOTERULET=50 And MTERULET=30) Then 'v�lt�s kamer�sr�l megfog� �r�t�sre
1216    Mov jKamera
1217    Mov jMurites
1218  '
1219    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1220   EndIf
1221  '................................
1222   If (MELOZOTERULET=60 And MTERULET=30) Then 'v�lt�s lerak�sr�l megfog� �r�t�sre
1223    Mov jLerakas
1224    Mov jMurites
1225  '
1226    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1227   EndIf
1228  '................................
1229   If (MELOZOTERULET=10 And MTERULET=40) Then 'v�lt�s alaphelyzetb�l felv�telre
1230    Mov jAlaph
1231    Mov jFelvetel
1232  '
1233    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1234   EndIf
1235  '................................
1236   If (MELOZOTERULET=20 And MTERULET=40) Then 'v�lt�s szervizpoz�ci�r�l felv�telre
1237    Mov jSzerviz
1238    Mov jFelvetel
1239  '
1240    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1241   EndIf
1242  '................................
1243   If (MELOZOTERULET=30 And MTERULET=40) Then 'v�lt�s megfog� �r�t�sr�l felv�telre
1244    Mov jMurites
1245    Mov jAlaph
1246    Mov jFelvetel
1247  '
1248    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1249   EndIf
1250  '................................
1251   If (MELOZOTERULET=50 And MTERULET=40) Then 'v�lt�s kamer�sr�l felv�telre
1252    Mov jKamera
1253    Mov jFelvetel
1254  '
1255    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1256   EndIf
1257  '................................
1258   If (MELOZOTERULET=60 And MTERULET=40) Then 'v�lt�s lerak�sr�l felv�telre
1259    Mov jLerakas
1260    Mov jAlaph
1261    Mov jFelvetel
1262  '
1263    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1264   EndIf
1265  '................................
1266   If (MELOZOTERULET=10 And MTERULET=50) Then 'v�lt�s alaphelyzetb�l kamer�sra
1267    Mov jAlaph
1268    Mov jKamera
1269  '
1270    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1271   EndIf
1272  '................................
1273   If (MELOZOTERULET=20 And MTERULET=50) Then 'v�lt�s szervizpoz�ci�r�l kamer�sra
1274    Mov jSzerviz
1275    Mov jAlaph
1276    Mov jKamera
1277  '
1278    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1279   EndIf
1280  '................................
1281   If (MELOZOTERULET=30 And MTERULET=50) Then 'v�lt�s megfog� �r�t�sr�l kamer�sra
1282    Mov jMurites
1283     Mov jKamera
1284   '
1285     MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1286   EndIf
1287  '................................
1288   If (MELOZOTERULET=40 And MTERULET=50) Then 'v�lt�s felv�telr�l kamer�sra
1289    Mov jFelvetel
1290    Mov jAlaph
1291    Mov jKamera
1292  '
1293    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1294   EndIf
1295  '................................
1296   If (MELOZOTERULET=60 And MTERULET=50) Then 'v�lt�s lerak�sr�l kamer�sra
1297    Mov jLerakas
1298    Mov jKamera
1299  '
1300    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1301   EndIf
1302  '................................
1303   If (MELOZOTERULET=10 And MTERULET=60) Then 'v�lt�s alaphelyzetb�l lerak�sra
1304    Mov jAlaph
1305    Mov jLerakas
1306  '
1307    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1308   EndIf
1309 '................................
1310   If (MELOZOTERULET=20 And MTERULET=60) Then 'v�lt�s szervizpoz�ci�r�l lerak�sra
1311    Mov jSzerviz
1312    Mov jAlaph
1313    Mov jLerakas
1314  '
1315    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1316   EndIf
1317  '................................
1318   If (MELOZOTERULET=30 And MTERULET=60) Then 'v�lt�s megfog� �r�t�sr�l lerak�sra
1319    Mov jMurites
1320    Mov jLerakas
1321  '
1322    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1323   EndIf
1324  '................................
1325   If (MELOZOTERULET=40 And MTERULET=60) Then 'v�lt�s felv�telr�l lerak�sra
1326    Mov jFelvetel
1327    Mov jAlaph
1328    Mov jLerakas
1329  '
1330    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1331   EndIf
1332  '................................
1333   If (MELOZOTERULET=50 And MTERULET=60) Then 'v�lt�s kamer�sr�l lerak�sra
1334    Mov jKamera
1335    Mov jLerakas
1336  '
1337    MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1338   EndIf
1339  '................................
1340  EndIf
1341  Return '=======================
1342 '
1343 '
1344 'K�rtya Inputok:
1345 '0 - Gy�rilag Foglalt
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
1362 'K�rtya Outputok:
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
