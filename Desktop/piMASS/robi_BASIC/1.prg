1 'Mozg�s vez�rl�, RV-13FRLM-R, K+F, BMR18905, RC1
2 'M_IP(4) -at olvasom
3 'M_QP(4) -ot �rom
4 'Glob�lis v�ltoz�k h�v�sa
5 Dim M_IP(4)
6 Dim M_QP(4)
7 'Dim M_Parancs
8 '
9 Servo Off
10 '
11 'Helyi v�ltoz�k felsorol�s-------------------
12 MSebesseg=100 'Robot sebess�g EZT NEM SZABAD M�SHOL V�LTOZTATNI alap: 100
13 MutazasiMagassag=740 'Utaz�si magass�g a fejnek EZT NEM SZABAD M�SHOL V�LTOZTATNI
14 Mgyors=100 'Robot gyorsul�s EZT NEM SZABAD M�SHOL V�LTOZTATNI alap: 80
15 Mlass=100 'Robot lassul�s EZT NEM SZABAD M�SHOL V�LTOZTATNI alap: 80
16 MSebPakolas=100 'Felv�teli, lerak�si sebess�g!!!
17 MGyorsPakolas=100 'Felv�teli, lerak�si gyorsul�s!!!
18 MLassPakolas=100 'Felv�teli, lerak�si lassul�s!!!
19 Mi1=0 'Minden For ciklusnak
20 Mi2=0 'Lefoglalva a Getparam fv-nek
21 Dim MIp(4) 'bemenetek helyi v�ltoz�ja
22 Dim MQp(4) 'kimenetek helyi v�ltoz�ja
23 '
24 Dim pKamera(4) 'Kamera poz�ci� a 4 t�pusra
25 Dim pFPozH(25) 'Felv�teli poz�ci� t�mb 25-�s rekesz
26 Dim pLPozH(6) 'Lerak�si poz�ci� t�mb 6-os rekesz
27 Dim pFPozT(35) 'Felv�teli poz�ci� t�mb 35-�s rekesz
28 Dim pLPozT(10) 'Lerak�si poz�ci� t�mb 10-es rekesz
29 Dim pFPozTH(70) 'Felv�teli poz�ci� t�mb 70-es rekesz
30 Dim pLPozTH(16) 'Lerak�si poz�ci� t�mb 16-os rekesz
31 Dim pFPozHN(88) 'Felv�teli poz�ci� t�mb 88-as rekesz
32 Dim pLPozHN(24) 'Lerak�si poz�ci� t�mb 24-es rekesz
33 '
34 'Tool (100,100,100,100)
35 Mdarab=0 'Megfog� darab �rz�kel�s t�rol�sa
36 MHibaszam=0 'Hibasz�m t�rol�sa
37 MpozIndex=0 'Poz�ci� index t�rol�sa
38 MFpozX=0
39 MFpozY=0
40 MLpozX=0
41 MLpozY=0
42 MForg=0
43 'MHibasz�m jelent�sei:
44 '900 - Rossz parancs k�ld�se a robotnak
45 '910 - Rossz mozg�s poz�ci� index param�ter (MpozIndex)
46 '920 - Servo OFF
47 '999 - Alaphelyzetfelv�teli hiba
48 '
49 MFel=0 'Ez a v�ltoz�, ha nulla, akkor b�rmilyen parancs j�het
50 MELOZOTERULET=0
51 MTERULET=0
52 'MTERULET jelent�sei (milyen poz�ci�ba k�pes menni):
53 '10 - alaphelyzet poz�ci� jog
54 '20 - szerviz poz�cio jog
55 '30 - amegfog� �r�t�se jog
56 '40 - bal oldali rasztung jog
57 '50 - jobb oldali rasztung jog
58 '
59 mXtemp=0 'alaphelyzet poz�ci� keres�shez
60 mYtemp=0 'alaphelyzet poz�ci� keres�shez
61 mZtemp=0 'alaphelyzet poz�ci� keres�shez
62 'ptemp0=0 poz�ci� seg�d alaphelyzethez
63 'ptemp1=0 'poz�ci� seg�d folyamathoz
64 'jtemp1=0 'poz�ci� seg�d forgat�shoz
65 MTavol=0 'alaphelyzetbe �rt�nk-e m�r?
66 '===================================================
67 '===================================================
68 '---------------------------------------------------
69 'Helyi v�ltoz�k felsorol�s v�ge------------------
70 '---------------------------------------------------
71 '
72 'Hiba eset�n megszak�t�s gener�l�dik
73 'M_QP(1)=0 'kimenet null�z�sa
74 'GoSub *GETPARAM
75 'M_QP(1)=0 'kimenet null�z�sa
76 '
77 '
78 '
79 While (M_Err=0)
80  Select M_In32(10208) 'Mozg�s v�grehajt�s parancs
81   Case 1 'Alaphelyzet felv�tel ES ut�n
82    GoSub *S1
83   Break
84   Case 2 'Alaphelyzet felv�tel CIKLUS
85    GoSub *S2
86   Break
87   Case 3 'Szerviz poz�ci� felv�tel
88    GoSub *S3
89   Break
90   Case 4 'Megfog� ki�r�t�se
91    GoSub *S4
92   Break
93     Case 10 'Poz�ci� f�l� �ll�s
94    GoSub *S10
95   Break
96     Case 12 'Poz�ci�ba �ll�s
97    GoSub *S12
98   Break
99     Case 14 'Fej kiemel�se
100    GoSub *S14
101   Break
102   Case 20 'Term�k felv�tele 25-�s rekeszb�l
103    GoSub *S20
104   Break
105   Case 21 'Term�k felv�tele 35-�s rekeszb�l
106    GoSub *S21
107   Break
108   Case 22 'Term�k felv�tele 70-es rekeszb�l
109    GoSub *S22
110   Break
111   Case 23 'Term�k felv�tele 88-as rekeszb�l
112    GoSub *S23
113   Break
114   Case 30 'Term�k kiemel�se a rekeszb�l
115    GoSub *S30
116   Break
117   Case 40 'Kamer�s ellen�rz�s
118    GoSub *S40
119   Break
120   Case 50 'Poz�ci�ba mozgat�s
121    GoSub *S50
122   Break
123   Case 70 'Term�k lerak�sa 6-os rekeszbe
124    GoSub *S70
125   Break
126   Case 71 'Term�k lerak�sa 10-es rekeszbe
127    GoSub *S71
128   Break
129   Case 72 'Term�k lerak�sa 16-os rekeszbe
130    GoSub *S72
131   Break
132   Case 73 'Term�k lerak�sa 24-es rekeszbe
133    GoSub *S73
134   Break
135   Case 80 'Fej kiemel�se az �l�kb�l
136    GoSub *S80
137   Break
138  End Select
139 'GoSub *GETPARAM
140  Dly 0.001
141 WEnd
142 End
143 '
144 '
145 '=================================================================
146 '---------------------------------
147 'Szubrutinok k�vetkeznek-----
148 '---------------------------------
149 '=================================================================
150 *S1 '=======================
151 'Alaphelyzet
152  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
153  'Inicializ�ci�s r�sz
154  Servo On 'szervo
155  Ovrd 5 'Sebess�g
156  Accel 5,5 'Gyorsul�s
157  M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
158 MHibaszam=0 'Hiba null�z�s
159 Mdarab=0
160 '=================================================================
161 '=================================================================
162 'Mozg�s: Kamer�s ellen�rz� poz�ci�t�l
163  mXtemp = Deg(J_Curr.J4)
164  mYtemp = Deg(J_Curr.J1)
165  mZtemp = Deg(J_Curr.J2)
166 If ((mXtemp>60 And mYtemp<15) Or (mXtemp>60 And mZtemp<-20)) Then
167 'Kamer�s poz�ci�ban �ll a robot!
168 Cnt 0
169 Mov jKamera
170   Dly 0.01
171 Mov jAlaph
172   Dly 0.01
173 EndIf
174 '=================================================================
175 '=================================================================
176 'Mozg�s: Lerak� ter�letr�l
177   ptemp0 = P_Curr 'Pillanatnyi poz�ci�
178 If (ptemp0.X > (250)) Then
179 'Lerak� poz�ci�ban �ll a fej!
180 Cnt 0
181 Dly 0.1
182   ptemp0.Z=520
183 Mvs ptemp0
184   Dly 0.01
185 Mov jLerakas
186   Dly 0.01
187 Mov jAlaph
188   Dly 0.01
189 EndIf
190 '=================================================================
191 '=================================================================
192 'Mozg�s: Felv�teli poz�ci�t�l
193   ptemp0 = P_Curr 'Pillanatnyi poz�ci�
194 If ((ptemp0.X) < (100)) Then
195 'Felv�teli poz�ci�ban �ll a fej!
196 Cnt 0
197   ptemp0.Z=520
198 Mvs ptemp0
199   Dly 0.01
200 Mov jFelvetel
201   Dly 0.01
202 Mov jAlaph
203   Dly 0.01
204 EndIf
205 '=================================================================
206 '=================================================================
207 ptemp0 = P_Curr 'Pillanatnyi poz�ci�
208 If ((ptemp0.X <= (250)) And  (ptemp0.X >= (100))) Then
209 Mov jAlaph
210 Else
211 MHibaszam=999
212 EndIf
213 '=================================================================
214 '=================================================================
215 'Lek�rdez�s, hogy Alaph-ben vagyunk
216  mXtemp = Deg(J_Curr.J1)
217  mYtemp = Deg(jAlaph.J1)
218  While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
219   Dly 0.1
220  WEnd
221  mXtemp = Deg(J_Curr.J2)
222  mYtemp = Deg(jAlaph.J2)
223  While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
224   Dly 0.1
225  WEnd
226  mXtemp = Deg(J_Curr.J3)
227  mYtemp = Deg(jAlaph.J3)
228  While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
229   Dly 0.1
230  WEnd
231  mXtemp = Deg(J_Curr.J4)
232  mYtemp = Deg(jAlaph.J4)
233  While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
234   Dly 0.1
235  WEnd
236  mXtemp = Deg(J_Curr.J5)
237  mYtemp = Deg(jAlaph.J5)
238  While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
239   Dly 0.1
240  WEnd
241  mXtemp = Deg(J_Curr.J6)
242  mYtemp = Deg(jAlaph.J6)
243  While ( mXtemp>mYtemp+20 Or mXtemp<mYtemp-20 )
244   Dly 0.1
245  WEnd
246 '
247 'Bels� v�ltoz�k be�ll�t�sa
248  MELOZOTERULET=10
249  MTERULET=10
250  MFel=0
251 '
252  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
253 Return '=======================
254 '=================================================================
255 '*************************************************************************
256 '*************************************************************************
257 '=================================================================
258 *S2 '=======================
259 If (MFel=0) Then
260 'Alaphelyzet
261  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
262  'Inicializ�ci�s r�sz
263   Ovrd MSebesseg 'Sebess�g
264  Accel Mgyors,Mlass 'Gyorsul�s
265 '
266  M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
267 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
268  MTERULET=10 'amelyik terulethez kell �ll�tani a tengelyeket
269  GoSub *TERULETVALTAS
270  Mov jAlaph
271 Dly 0.01
272 MHibaszam=0 'Hiba null�z�s
273 '
274  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
275 EndIf
276 Return '=======================
277 '=================================================================
278 '*************************************************************************
279 '*************************************************************************
280 '=================================================================
281 *S3 '=======================
282 If (MFel=0) Then
283 'Szerviz poz�ci� felv�tele
284  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
285  'Inicializ�ci�s r�sz
286  Ovrd 10 'Sebess�g
287  Accel 10,10 'Gyorsul�s
288 '
289  M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
290 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
291  MTERULET=20 'amelyik terulethez kell �ll�tani a tengelyeket
292  GoSub *TERULETVALTAS
293  Mov jSzerviz
294   Dly 0.01
295  MFel=0
296 '
297  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
298 EndIf
299 Return '=======================
300 '=================================================================
301 '*************************************************************************
302 '*************************************************************************
303 '=================================================================
304 *S4 '=======================
305 If (MFel=0) Then
306 'Megfog� ki�r�t�se
307  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
308  'Inicializ�ci�s r�sz
309   Ovrd MSebesseg 'Sebess�g
310  Accel Mgyors,Mlass 'Gyorsul�s
311 '
312  M_Tool=0 'Nincs m�g semmilyen tool haszn�lva, a poz�ci�k be�ll�t�s�hoz haszn�lok csak tool-t, elmenteni m�r tool0-val mentem
313 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
314  MTERULET=30 'amelyik terulethez kell �ll�tani a tengelyeket
315  GoSub *TERULETVALTAS
316  Mov jMurites
317   Dly 0.001
318 Cnt 0
319 Mvs pMurites
320   Dly 0.01
321 '
322  Mdarab=0
323  MFel=0
324 '
325  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
326 EndIf
327 Return '=======================
328 '=================================================================
329 '=================================================================
330 '=================================================================
331 '**********************POZ�CI� F�L� �LL�S*************************
332 '=================================================================
333 '=================================================================
334 '=================================================================
335 *S10 '=======================
336 '
337 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
338  'Term�k felv�tele
339  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
340 '
341  Ovrd MSebesseg 'Sebess�g
342  Accel Mgyors,Mlass 'Gyorsul�s
343 '
344 Cnt 1
345 '
346 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
347  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
348  GoSub *TERULETVALTAS
349 MHibaszam=0
350 '
351 Cnt 0
352 '
353 ptemp1=P_Curr 'Pillanatnyi poz�ci�
354 '
355 If (MpozIndex>0 And MpozIndex<26 And (MPosX>100 And MPosX<300 And MPosY>100 And MPosY<300 And MPosZ>100 And MPosZ<300) And (MPosA>100 And MPosA<300 And MPosB>100 And MPosB<300 And MPosC>100 And MPosC<300)) Then
356  ptemp1.X=(MPosX/100)
357  ptemp1.Y=(MPosY/100)
358  ptemp1.Z=((MPosZ/100)+300)
359  ptemp1.A=Rad(MPosA/100)
360  ptemp1.B=Rad(MPosB/100)
361  ptemp1.C=Rad(MPosC/100)
362  Mov ptemp1
363  Dly 0.01
364 '
365 MFel=12 'Csak a term�kre be�ll�s k�vetkezhet
366 '
367 Else
368 MHibaszam=910
369 EndIf
370 '
371  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
372 '
373 Else
374 MHibaszam=900
375 EndIf
376 Return '=======================
377 '=================================================================
378 '=================================================================
379 '=================================================================
380 '**************************POZ�CI�BA �LL�S************************
381 '=================================================================
382 '=================================================================
383 '=================================================================
384 *S12 '=======================
385 '
386 If ((MFel=12) Or (MFel=14)) Then 'Csak a term�kre be�ll�s k�vetkezhet
387  'Term�kre �ll�s
388  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
389 '
390  Ovrd MSebesseg 'Sebess�g
391  Accel Mgyors,Mlass 'Gyorsul�s
392 '
393 Cnt 1
394 '
395 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
396  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
397  GoSub *TERULETVALTAS
398 MHibaszam=0
399 '
400 Cnt 0
401 '
402 ptemp1=pFPozHFF
403 '
404 If (MpozIndex>0 And MpozIndex<26 And (MPosX>100 And MPosX<300 And MPosY>100 And MPosY<300 And MPosZ>100 And MPosZ<300) And (MPosA>100 And MPosA<300 And MPosB>100 And MPosB<300 And MPosC>100 And MPosC<300)) Then
405  ptemp1.X=(MPosX/100)
406  ptemp1.Y=(MPosY/100)
407  ptemp1.Z=(MPosZ/100)
408  ptemp1.A=Rad(MPosA)
409  ptemp1.B=Rad(MPosB)
410  ptemp1.C=Rad(MPosC)
411  Mov ptemp1
412  Dly 0.01
413 MFel=14 'Csak a term�k kiemel�se az �l�kb�l k�vetkezhet
414 '
415 Else
416 MHibaszam=910
417 EndIf
418 '
419  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
420 '
421 Else
422 MHibaszam=900
423 EndIf
424 Return '=======================
425 '=================================================================
426 '=================================================================
427 '=================================================================
428 '**************************ROBOTFEJ KIEMEL�SE*********************
429 '=================================================================
430 '=================================================================
431 '=================================================================
432 *S14 '=======================
433 '
434 If (MFel=14) Then 'Csak a term�k kiemel�se az �l�kb�l k�vetkezhet
435  'FEJ KIEMEL�SE
436  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
437 '
438  Ovrd MSebesseg 'Sebess�g
439  Accel Mgyors,Mlass 'Gyorsul�s
440 '
441 Cnt 1
442 '
443 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
444  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
445  GoSub *TERULETVALTAS
446 MHibaszam=0
447 '
448 Cnt 0
449 '
450 ptemp1=P_Curr 'Pillanatnyi poz�ci�
451 ptemp1.Z=P_Curr.Z+300
452 Mvs ptemp1
453 MFel=0 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
454 '
455  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
456 '
457 Else
458 MHibaszam=900
459 EndIf
460 Return '=======================
461 '=================================================================
462 '=================================================================
463 '=================================================================
464 '*******************************Felv�tel**************************
465 '=================================================================
466 '=================================================================
467 '=================================================================
468 *S20 '=======================
469 '
470 If (MFel=0 And Mdarab=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
471  'Term�k felv�tele
472  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
473 '
474  Ovrd MSebesseg 'Sebess�g
475  Accel Mgyors,Mlass 'Gyorsul�s
476 '
477 Cnt 1
478 '
479 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
480  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
481  GoSub *TERULETVALTAS
482 MHibaszam=0
483 '
484 Cnt 0
485 '
486 ptemp1=pFPozHFF
487 '
488 If (MpozIndex>0 And MpozIndex<26 And MFpozX>-5000 And MFpozX<35000& And MFpozY>-5000 And MFpozY<60000& And MFpozZ>-2000 And MFpozZ<6000) Then
489  ptemp1.X=pFPozHFF.X+(MFpozX/100)
490  ptemp1.Y=pFPozHFF.Y+(MFpozY/100)
491  ptemp1.Z=pFPozHFF.Z+((MFpozZ/100)+50)
492  Mov ptemp1
493  Dly 0.001
494  Ovrd MSebPakolas 'Sebess�g
495  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
496  ptemp1.X = pFPozHFF.X+(MFpozX/100)
497  ptemp1.Y = pFPozHFF.Y+(MFpozY/100)
498  ptemp1.Z=pFPozHFF.Z+(MFpozZ/100)
499  ptemp1.C=pFPozHFF.A+(MFpozA/100)
500  Mvs ptemp1
501  Dly 0.01
502 Mdarab=1
503 pFPozH(MpozIndex)=ptemp1
504  MHibaszam=0
505 '
506 MFel=30 'Csak a term�k kiemel�se az �l�kb�l k�vetkezhet
507 '
508 Else
509 MHibaszam=910
510 EndIf
511 '
512  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
513 '
514 Else
515 MHibaszam=900
516 EndIf
517 Return '=======================
518 '=================================================================
519 '**************************************************************************
520 '**************************************************************************
521 '=================================================================
522 *S21 '=========================
523 '
524 If (MFel=0 And Mdarab=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
525  'Term�k felv�tele
526  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
527 '
528  Ovrd MSebesseg 'Sebess�g
529  Accel Mgyors,Mlass 'Gyorsul�s
530 '
531 Cnt 1
532 '
533 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
534  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
535  GoSub *TERULETVALTAS
536 MHibaszam=0
537 '
538 Cnt 0
539 '
540 ptemp1=pFPozTFF
541 '
542 If (MpozIndex>0 And MpozIndex<36 And MFpozX>-5000 And MFpozX<35000& And MFpozY>-5000 And MFpozY<60000& And MFpozZ>-2000 And MFpozZ<6000) Then
543  ptemp1.X = pFPozTFF.X+(MFpozX/100)
544  ptemp1.Y = pFPozTFF.Y+(MFpozY/100)
545  ptemp1.Z=pFPozTFF.Z+((MFpozZ/100)+50)
546  Mov ptemp1
547  Dly 0.001
548  Ovrd MSebPakolas 'Sebess�g
549  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
550  ptemp1.X = pFPozTFF.X+(MFpozX/100)
551  ptemp1.Y = pFPozTFF.Y+(MFpozY/100)
552  ptemp1.Z=pFPozTFF.Z+(MFpozZ/100)
553  Mvs ptemp1
554  Dly 0.01
555 Mdarab=1
556 pFPozT(MpozIndex)=ptemp1
557  MHibaszam=0
558 '
559 MFel=30 'Csak a term�k kiemel�se az �l�kb�l k�vetkezhet
560 '
561 Else
562 MHibaszam=910
563 EndIf
564 '
565  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
566 '
567 Else
568 MHibaszam=900
569 EndIf
570 Return '=======================
571 '=================================================================
572 '*************************************************************************
573 '*************************************************************************
574 '=================================================================
575 *S22 '=======================
576 '
577 If (MFel=0 And Mdarab=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
578  'Term�k felv�tele
579  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
580 '
581  Ovrd MSebesseg 'Sebess�g
582  Accel Mgyors,Mlass 'Gyorsul�s
583 '
584 Cnt 1
585 '
586 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
587  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
588  GoSub *TERULETVALTAS
589 MHibaszam=0
590 '
591 Cnt 0
592 '
593 ptemp1=pFPozTHFF
594 '
595 If (MpozIndex>0 And MpozIndex<71 And MFpozX>-5000 And MFpozX<35000& And MFpozY>-5000 And MFpozY<60000& And MFpozZ>-2000 And MFpozZ<6000) Then
596  ptemp1.X = pFPozTHFF.X+(MFpozX/100)
597  ptemp1.Y = pFPozTHFF.Y+(MFpozY/100)
598  ptemp1.Z=pFPozTHFF.Z+((MFpozZ/100)+50)
599  Mov ptemp1
600  Dly 0.001
601  Ovrd MSebPakolas 'Sebess�g
602  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
603  ptemp1.X = pFPozTHFF.X+(MFpozX/100)
604  ptemp1.Y = pFPozTHFF.Y+(MFpozY/100)
605  ptemp1.Z=pFPozTHFF.Z+(MFpozZ/100)
606  Mvs ptemp1
607  Dly 0.01
608 Mdarab=1
609 pFPozTH(MpozIndex)=ptemp1
610  MHibaszam=0
611 '
612 MFel=30 'Csak a term�k kiemel�se az �l�kb�l k�vetkezhet
613 '
614 Else
615 MHibaszam=910
616 EndIf
617 '
618  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
619 '
620 Else
621 MHibaszam=900
622 EndIf
623 Return '=======================
624 '=================================================================
625 '*************************************************************************
626 '*************************************************************************
627 '=================================================================
628 *S23 '=======================
629 '
630 If (MFel=0 And Mdarab=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
631  'Term�k felv�tele
632  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
633 '
634  Ovrd MSebesseg 'Sebess�g
635  Accel Mgyors,Mlass 'Gyorsul�s
636 '
637 Cnt 1
638 '
639 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
640  MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
641  GoSub *TERULETVALTAS
642 MHibaszam=0
643 '
644 Cnt 0
645 '
646 ptemp1=pFPozHNFF
647 '
648 If (MpozIndex>0 And MpozIndex<89 And MFpozX>-5000 And MFpozX<35000& And MFpozY>-5000 And MFpozY<60000& And MFpozZ>-2000 And MFpozZ<6000) Then
649  ptemp1.X = pFPozHNFF.X+(MFpozX/100)
650  ptemp1.Y = pFPozHNFF.Y+(MFpozY/100)
651  ptemp1.Z=pFPozHNFF.Z+((MFpozZ/100)+50)
652  Mov ptemp1
653  Dly 0.001
654  Ovrd MSebPakolas 'Sebess�g
655  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
656  ptemp1.X = pFPozHNFF.X+(MFpozX/100)
657  ptemp1.Y = pFPozHNFF.Y+(MFpozY/100)
658  ptemp1.Z=pFPozHNFF.Z+(MFpozZ/100)
659  Mvs ptemp1
660  Dly 0.01
661 Mdarab=1
662 pFPozHN(MpozIndex)=ptemp1
663  MHibaszam=0
664 '
665 MFel=30 'Csak a term�k kiemel�se az �l�kb�l k�vetkezhet
666 '
667 Else
668 MHibaszam=910
669 EndIf
670 '
671  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
672 '
673 Else
674 MHibaszam=900
675 EndIf
676 Return '=======================
677 '=================================================================
678 '*************************************************************************
679 '*************************************************************************
680 '=================================================================
681 *S30 '=======================
682 '
683 If (MFel=30 Or MFel=0) Then 'a 20-as parancs kiad�sa ut�n csak ez v�gezhet� el
684 'Term�k kiemel�se az �l�kb�l
685 GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
686 '
687 Ovrd MSebesseg 'Sebess�g
688 Accel Mgyors,Mlass 'Gyorsul�s
689 '
690 Cnt 1
691 '
692 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
693 MTERULET=40 'amelyik terulethez kell �ll�tani a tengelyeket
694 GoSub *TERULETVALTAS
695 MHibaszam=0
696 '
697 Cnt 0
698 '
699 Ovrd MSebesseg 'Sebess�g
700 Accel Mgyors,Mlass 'Gyorsul�s
701 ptemp1=P_Curr 'Pillanatnyi poz�ci�
702 ptemp1.Z=P_Curr.Z+70
703 Mvs ptemp1
704  Dly 0.01
705 '
706 MHibaszam=0
707 '
708 MFel=0 'B�rmelyik parancs k�vetkezhet
709 '
710  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
711 '
712 Else
713 MHibaszam=900
714 EndIf
715 Return '=======================
716 '=================================================================
717 '*************************************************************************
718 '*************************************************************************
719 '=================================================================
720 *S40 '=======================
721 '
722 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
723  'Kamer�s ellen�rz�s
724  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
725 '
726 Ovrd MSebesseg 'Sebess�g
727 Accel Mgyors,Mlass 'Gyorsul�s
728 '
729 Cnt 1
730 '
731 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
732  MTERULET=50 'amelyik terulethez kell �ll�tani a tengelyeket
733  GoSub *TERULETVALTAS
734 '
735 Cnt 0
736 '
737 If (MpozIndex>0 And MpozIndex<5) Then
738 '
739 Ovrd MSebesseg 'Sebess�g
740 Accel Mgyors,Mlass 'Gyorsul�s
741 '
742  Mvs pKamera(MpozIndex)
743 Dly 0.01
744  MHibaszam=0
745 Else
746 MHibaszam=910
747 EndIf
748 '
749 MFel=0 'B�rmelyik parancs k�vetkezhet
750 '
751  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
752 '
753 Else
754 MHibaszam=900
755 EndIf
756 Return '=======================
757 '=================================================================
758 '*************************************************************************
759 '*************************************************************************
760 '=================================================================
761 *S50 '=======================
762 '
763 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
764 'Poz�ci�ba k�ld�s
765 GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
766 '
767 Ovrd MSebesseg 'Sebess�g
768 Accel Mgyors,Mlass 'Gyorsul�s
769 '
770 Cnt 1
771 '
772 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
773 MTERULET=50 'amelyik terulethez kell �ll�tani a tengelyeket
774 GoSub *TERULETVALTAS
775 '
776 Cnt 0
777 If (MpozIndex>0 And MpozIndex<5 And MForg>-18100 And MForg<18100) Then
778 '
779 Ovrd MSebesseg 'Sebess�g
780 Accel Mgyors,Mlass 'Gyorsul�s
781 '
782 'M_Tool=1
783 jtemp1=J_Curr
784 jtemp1.J6=jtemp1.J6+Rad(MForg/100)
785 'ptemp1=pKamera(MpozIndex)
786 'ptemp1.C=ptemp1.C+Rad(MForg/100)
787 'Mvs ptemp1
788 Mov jtemp1
789 Dly 0.01
790  MHibaszam=0
791 Else
792 MHibaszam=910
793 EndIf
794 '
795 MFel=0 'B�rmelyik parancs k�vetkezhet
796 '
797 GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
798 '
799 Else
800 MHibaszam=900
801 EndIf
802 Return '=======================
803 '=================================================================
804 '*************************************************************************
805 '*************************************************************************
806 '=================================================================
807 *S70 '=======================
808 '
809 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
810 'Term�k lerak�sa
811 GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
812 '
813 Ovrd MSebesseg 'Sebess�g
814 Accel Mgyors,Mlass 'Gyorsul�s
815 '
816 Cnt 1
817 '
818 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
819  MTERULET=60 'amelyik terulethez kell �ll�tani a tengelyeket
820  GoSub *TERULETVALTAS
821 MHibaszam=0
822 '
823 Cnt 0
824 '
825 ptemp1=pLPozHLL
826 '
827 If (MpozIndex>0 And MpozIndex<7 And MForg>-18100 And MForg<18100 And MLpozX>-5000 And MLpozX<35000& And MLpozY>-5000 And MLpozY<60000& And MLpozZ>-2000 And MLpozZ<6000) Then
828  '
829  ptemp1.X = pLPozHLL.X+(MLpozX/100)
830  ptemp1.Y = pLPozHLL.Y+(MLpozY/100)
831  ptemp1.Z=pLPozHLL.Z+((MLpozZ/100)+50)
832 '
833 jtemp1 = PtoJ(ptemp1)
834 jtemp1.J6=jtemp1.J6+Rad(MForg/100)
835 Mov jtemp1
836 '
837 'Mov ptemp1
838 Dly 0.001
839 '
840 'jtemp1=J_Curr
841 'jtemp1.J6=jtemp1.J6+Rad(MForg/100)
842 'Mov jtemp1
843 'Dly 0.01
844 '
845  Ovrd MSebPakolas 'Sebess�g
846  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
847  ptemp1=P_Curr
848  ptemp1.Z=P_Curr.Z-50
849  Mvs ptemp1
850  Dly 0.01
851 Mdarab=0
852 pLPozH(MpozIndex)=ptemp1
853 '
854  MHibaszam=0
855 '
856 MFel=80 '80-as parancs k�vetkezhet
857 '
858 Else
859 MHibaszam=910
860 EndIf
861 '
862  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
863 '
864 Else
865 MHibaszam=900
866 EndIf
867 Return '=======================
868 '=================================================================
869 '*************************************************************************
870 '*************************************************************************
871 '=================================================================
872 *S71 '=======================
873 '
874 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
875 'Term�k lerak�sa
876 GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
877 '
878 Ovrd MSebesseg 'Sebess�g
879 Accel Mgyors,Mlass 'Gyorsul�s
880 '
881 Cnt 1
882 '
883 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
884  MTERULET=60 'amelyik terulethez kell �ll�tani a tengelyeket
885  GoSub *TERULETVALTAS
886 MHibaszam=0
887 '
888 Cnt 0
889 '
890 ptemp1=pLPozTLL
891 '
892 If (MpozIndex>0 And MpozIndex<11 And MForg>-18100 And MForg<18100 And MLpozX>-5000 And MLpozX<35000& And MLpozY>-5000 And MLpozY<60000& And MLpozZ>-2000 And MLpozZ<6000) Then
893   '
894  ptemp1.X = pLPozTLL.X+(MLpozX/100)
895  ptemp1.Y = pLPozTLL.Y+(MLpozY/100)
896  ptemp1.Z=pLPozTLL.Z+((MLpozZ/100)+50)
897  Mov ptemp1
898 Dly 0.001
899 '
900 jtemp1=J_Curr
901 jtemp1.J6=jtemp1.J6+Rad(MForg/100)
902  Mov jtemp1
903  Dly 0.01
904 '
905  Ovrd MSebPakolas 'Sebess�g
906  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
907  ptemp1=P_Curr
908  ptemp1.Z=P_Curr.Z-50
909  Mvs ptemp1
910  Dly 0.01
911 Mdarab=0
912 pLPozT(MpozIndex)=ptemp1
913 '
914  MHibaszam=0
915 '
916 MFel=80 '80-as parancs k�vetkezhet
917 '
918 Else
919 MHibaszam=910
920 EndIf
921 '
922  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
923 '
924 Else
925 MHibaszam=900
926 EndIf
927 Return '=======================
928 '=================================================================
929 '*************************************************************************
930 '*************************************************************************
931 '=================================================================
932 *S72 '=======================
933 '
934 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
935 'Term�k lerak�sa
936 GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
937 '
938 Ovrd MSebesseg 'Sebess�g
939 Accel Mgyors,Mlass 'Gyorsul�s
940 '
941 Cnt 1
942 '
943 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
944  MTERULET=60 'amelyik terulethez kell �ll�tani a tengelyeket
945  GoSub *TERULETVALTAS
946 MHibaszam=0
947 '
948 Cnt 0
949 '
950 ptemp1=pLPozTHLL
951 '
952 If (MpozIndex>0 And MpozIndex<17 And MForg>-18100 And MForg<18100 And MLpozX>-5000 And MLpozX<35000& And MLpozY>-5000 And MLpozY<60000& And MLpozZ>-2000 And MLpozZ<6000) Then
953   '
954  ptemp1.X = pLPozTHLL.X+(MLpozX/100)
955  ptemp1.Y = pLPozTHLL.Y+(MLpozY/100)
956  ptemp1.Z=pLPozTHLL.Z+((MLpozZ/100)+50)
957  Mov ptemp1
958 Dly 0.001
959 '
960 jtemp1=J_Curr
961 jtemp1.J6=jtemp1.J6+Rad(MForg/100)
962  Mov jtemp1
963  Dly 0.01
964 '
965  Ovrd MSebPakolas 'Sebess�g
966  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
967  ptemp1=P_Curr
968  ptemp1.Z=P_Curr.Z-50
969  Mvs ptemp1
970  Dly 0.01
971 Mdarab=0
972 pLPozTH(MpozIndex)=ptemp1
973 '
974  MHibaszam=0
975 '
976 MFel=80 '80-as parancs k�vetkezhet
977 '
978 Else
979 MHibaszam=910
980 EndIf
981 '
982  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
983 '
984 Else
985 MHibaszam=900
986 EndIf
987 Return '=======================
988 '=================================================================
989 '*************************************************************************
990 '*************************************************************************
991 '=================================================================
992 *S73 '=======================
993 '
994 If (MFel=0) Then 'B�rmelyik mozg�s ut�n elv�gezhet� a parancs
995 'Term�k lerak�sa
996 GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
997 '
998 Ovrd MSebesseg 'Sebess�g
999 Accel Mgyors,Mlass 'Gyorsul�s
1000 '
1001 Cnt 1
1002 '
1003 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
1004  MTERULET=60 'amelyik terulethez kell �ll�tani a tengelyeket
1005  GoSub *TERULETVALTAS
1006 MHibaszam=0
1007 '
1008 Cnt 0
1009 '
1010 ptemp1=pLPozHNLL
1011 '
1012 If (MpozIndex>0 And MpozIndex<25 And MForg>-18100 And MForg<18100 And MLpozX>-5000 And MLpozX<35000& And MLpozY>-5000 And MLpozY<60000& And MLpozZ>-2000 And MLpozZ<6000) Then
1013  '
1014  ptemp1.X = pLPozHNLL.X+(MLpozX/100)
1015  ptemp1.Y = pLPozHNLL.Y+(MLpozY/100)
1016  ptemp1.Z=pLPozHNLL.Z+((MLpozZ/100)+50)
1017  Mov ptemp1
1018 Dly 0.001
1019 '
1020 jtemp1=J_Curr
1021 jtemp1.J6=jtemp1.J6+Rad(MForg/100)
1022  Mov jtemp1
1023  Dly 0.01
1024 '
1025  Ovrd MSebPakolas 'Sebess�g
1026  Accel MGyorsPakolas,MLassPakolas 'Gyorsul�s
1027  ptemp1=P_Curr
1028  ptemp1.Z=P_Curr.Z-50
1029  Mvs ptemp1
1030  Dly 0.01
1031 Mdarab=0
1032 pLPozHN(MpozIndex)=ptemp1
1033 '
1034  MHibaszam=0
1035 '
1036 MFel=80 '80-as parancs k�vetkezhet
1037 '
1038 Else
1039 MHibaszam=910
1040 EndIf
1041 '
1042  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
1043 '
1044 Else
1045 MHibaszam=900
1046 EndIf
1047 Return '=======================
1048 '=================================================================
1049 '*************************************************************************
1050 '*************************************************************************
1051 '=================================================================
1052 *S80 '=======================
1053 '
1054 If ((MFel=0 Or MFel=80)) Then 'a 70-es parancs kiad�sa ut�n csak ez v�gezhet� el
1055  'Term�k lerak�s ut�n fej kiemel�se az �l�kb�l
1056  GoSub *HANDSHAKE1 '�zenet meg�rkezett, parancs elkezdve nyugta
1057 '
1058  Ovrd MSebesseg 'Sebess�g
1059  Accel Mgyors,Mlass 'Gyorsul�s
1060 '
1061 Cnt 1
1062 '
1063 'Robot elhelyezked�s�nek ellen�rz�se, hogy a tengelyek megfelel�nek �lljanak
1064  MTERULET=60 'amelyik terulethez kell �ll�tani a tengelyeket
1065  GoSub *TERULETVALTAS
1066 MHibaszam=0
1067 '
1068 Cnt 0
1069 '
1070  Ovrd MSebesseg 'Sebess�g
1071  Accel Mgyors,Mlass 'Gyorsul�s
1072  ptemp1 = P_Curr 'Pillanatnyi poz�ci�
1073  ptemp1.Z=ptemp1.Z+40
1074  Mvs ptemp1
1075  Dly 0.01
1076 '
1077 MFel=0 'B�rmelyik parancs k�vetkezhet
1078 '
1079  GoSub *HANDSHAKE2 'v�ge a m�veletnek, v�rakoz�s a nyugt�z�sra
1080 '
1081 Else
1082 MHibaszam=900
1083 EndIf
1084 Return '=======================
1085 '=================================================================
1086 '=================================================================
1087 '=================================================================
1088 '*****************************************************************
1089 '=================================================================
1090 '=================================================================
1091 '=================================================================
1092 *GETPARAM '=======================
1093  For Mi2=1 To 4 'Param�terek �tv�tele az utols�t�l az els�ig
1094   MIp(5-Mi2)=M_IP(5-Mi2)
1095  Next
1096 M_Svo(1)=1
1097 Return '=======================
1098 '
1099 *HANDSHAKE1 '=======================
1100 Wait M_In(10160)=1 'wait RobotStartIn=1 --> U3E0\\G10010.0   U3E0\\G12058.0
1101 MpozIndex=M_In32(10176) 'poz�ci� index �tv�tele
1102 MPosJ6=M_In32(10240) 'Forgat�s param�ter �tv�tele
1103 MPosX=M_In32(10272) 'X poz�ci� �tv�tele
1104 MPosY=M_In32(10304) 'Y poz�ci� �tv�tele
1105 MPosZ=M_In32(10336) 'Z poz�ci� �tv�tele
1106 MPosA=M_In32(10368) 'A poz�ci� fok �tv�tele
1107 MPosB=M_In32(10400) 'B poz�ci� fok �tv�tele
1108 MPosC=M_In32(10432) 'C poz�ci� fok �tv�tele
1109 M_Out(10160)=1 'ConfirmOut=1 --> U3E1\\G10010.0   U3E2\\G10010.0
1110 Wait M_In(10160)=0 'wait RobotStartIn=0
1111 Wait M_In32(10176)=0 'poz�ci� index kinull�z�s�ra v�r
1112 Wait M_In32(10208)=0 'Mozg�s parancs null�z�s�ra v�r
1113 Wait M_In32(10240)=0 'Forgat�s parancs null�z�s�ra v�r
1114 Wait M_In32(10272)=0 'X poz�ci� �tv�tele parancs null�z�s�ra v�r
1115 Wait M_In32(10304)=0 'Y poz�ci� �tv�tele parancs null�z�s�ra v�r
1116 Wait M_In32(10336)=0 'Z poz�ci� �tv�tele parancs null�z�s�ra v�r
1117 Wait M_In32(10368)=0 'A poz�ci� fok �tv�tele parancs null�z�s�ra v�r
1118 Wait M_In32(10400)=0 'B poz�ci� fok �tv�tele parancs null�z�s�ra v�r
1119 Wait M_In32(10432)=0 'C poz�ci� fok �tv�tele parancs null�z�s�ra v�r
1120 M_Out(10160)=0 'ConfirmOut=0
1121 Return '=======================
1122 '
1123 *HANDSHAKE2 '=======================
1124 If (M_Svo(1)=1) Then
1125 '
1126 Else
1127 MHibaszam=920
1128 EndIf
1129 M_Out32(10176)=MHibaszam 'Hiba st�tusz k�ld�se
1130 M_Out(10161)=1 'ReadyOut=1
1131 Wait M_In(10161)=1 'wait ConfirmIn=1
1132 M_Out32(10176)=0 'Hibak�ld�s kinull�z�sa
1133 M_Out(10161)=0 'ReadyOut=0
1134 Wait M_In(10161)=0 'wait ConfirmIn=0
1135 Return '=======================
1136 '=================================================================
1137 '*************************************************************************
1138 '*************************************************************************
1139 '*************************************************************************
1140 '*************************************************************************
1141 '=================================================================
1142 *TERULETVALTAS '=======================
1143 If (MELOZOTERULET<>MTERULET) Then
1144 '................................
1145  If (MELOZOTERULET=10 And MTERULET=20) Then 'v�lt�s alaphelyzetb�l szervizpoz�ci�ra
1146   Mov jAlaph
1147   Mov jSzerviz
1148 '
1149   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1150  EndIf
1151 '................................
1152  If (MELOZOTERULET=30 And MTERULET=20) Then 'v�lt�s megfog� �r�t�sr�l szervizpoz�ci�ra
1153   Mov jMurites
1154   Mov jAlaph
1155   Mov jSzerviz
1156 '
1157   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1158  EndIf
1159 '................................
1160  If (MELOZOTERULET=40 And MTERULET=20) Then 'v�lt�s felv�telr�l szervizpoz�ci�ra
1161   Mov jFelvetel
1162   Mov jSzerviz
1163 '
1164   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1165  EndIf
1166 '................................
1167  If (MELOZOTERULET=50 And MTERULET=20) Then 'v�lt�s kamer�sr�l szervizpoz�ci�ra
1168   Mov jKamera
1169   Mov jAlaph
1170   Mov jSzerviz
1171 '
1172   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1173  EndIf
1174 '................................
1175  If (MELOZOTERULET=60 And MTERULET=20) Then 'v�lt�s lerak�sr�l szervizpoz�ci�ra
1176   Mov jLerakas
1177   Mov jAlaph
1178   Mov jSzerviz
1179 '
1180   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1181  EndIf
1182 '................................
1183  If (MELOZOTERULET=20 And MTERULET=10) Then 'v�lt�s szervizpoz�ci�r�l alaphelyzetre
1184   Mov jSzerviz
1185   Mov jAlaph
1186 '
1187   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1188  EndIf
1189 '................................
1190  If (MELOZOTERULET=30 And MTERULET=10) Then 'v�lt�s megfog� �r�t�sr�l alaphelyzetre
1191   Mov jMurites
1192   Mov jAlaph
1193 '
1194   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1195  EndIf
1196 '................................
1197  If (MELOZOTERULET=40 And MTERULET=10) Then 'v�lt�s felv�telr�l alaphelyzetre
1198   Mov jFelvetel
1199   Mov jAlaph
1200 '
1201   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1202  EndIf
1203 '................................
1204  If (MELOZOTERULET=50 And MTERULET=10) Then 'v�lt�s kamer�sr�l alaphelyzetre
1205   Mov jKamera
1206   Mov jAlaph
1207 '
1208   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1209  EndIf
1210 '................................
1211  If (MELOZOTERULET=60 And MTERULET=10) Then 'v�lt�s lerak�sr�l alaphelyzetre
1212   Mov jLerakas
1213   Mov jAlaph
1214 '
1215   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1216  EndIf
1217 '................................
1218  If (MELOZOTERULET=10 And MTERULET=30) Then 'v�lt�s alaphelyzetb�l megfog� �r�t�sre
1219   Mov jAlaph
1220   Mov jMurites
1221 '
1222   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1223  EndIf
1224 '................................
1225  If (MELOZOTERULET=20 And MTERULET=30) Then 'v�lt�s szervizpoz�ci�r�l megfog� �r�t�sre
1226  Mov jSzerviz
1227  Mov jAlaph
1228  Mov jMurites
1229 '
1230   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1231  EndIf
1232 '................................
1233  If (MELOZOTERULET=40 And MTERULET=30) Then 'v�lt�s felv�telr�l megfog� �r�t�sre
1234  Mov jFelvetel
1235  Mov jAlaph
1236  Mov jMurites
1237 '
1238   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1239  EndIf
1240 '................................
1241  If (MELOZOTERULET=50 And MTERULET=30) Then 'v�lt�s kamer�sr�l megfog� �r�t�sre
1242   Mov jKamera
1243   Mov jMurites
1244 '
1245   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1246  EndIf
1247 '................................
1248  If (MELOZOTERULET=60 And MTERULET=30) Then 'v�lt�s lerak�sr�l megfog� �r�t�sre
1249   Mov jLerakas
1250   Mov jMurites
1251 '
1252   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1253  EndIf
1254 '................................
1255  If (MELOZOTERULET=10 And MTERULET=40) Then 'v�lt�s alaphelyzetb�l felv�telre
1256   Mov jAlaph
1257   Mov jFelvetel
1258 '
1259   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1260  EndIf
1261 '................................
1262  If (MELOZOTERULET=20 And MTERULET=40) Then 'v�lt�s szervizpoz�ci�r�l felv�telre
1263   Mov jSzerviz
1264   Mov jFelvetel
1265 '
1266   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1267  EndIf
1268 '................................
1269  If (MELOZOTERULET=30 And MTERULET=40) Then 'v�lt�s megfog� �r�t�sr�l felv�telre
1270   Mov jMurites
1271   Mov jAlaph
1272   Mov jFelvetel
1273 '
1274   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1275  EndIf
1276 '................................
1277  If (MELOZOTERULET=50 And MTERULET=40) Then 'v�lt�s kamer�sr�l felv�telre
1278   Mov jKamera
1279   Mov jFelvetel
1280 '
1281   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1282  EndIf
1283 '................................
1284  If (MELOZOTERULET=60 And MTERULET=40) Then 'v�lt�s lerak�sr�l felv�telre
1285   Mov jLerakas
1286   Mov jAlaph
1287   Mov jFelvetel
1288 '
1289   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1290  EndIf
1291 '................................
1292  If (MELOZOTERULET=10 And MTERULET=50) Then 'v�lt�s alaphelyzetb�l kamer�sra
1293   Mov jAlaph
1294   Mov jKamera
1295 '
1296   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1297  EndIf
1298 '................................
1299  If (MELOZOTERULET=20 And MTERULET=50) Then 'v�lt�s szervizpoz�ci�r�l kamer�sra
1300   Mov jSzerviz
1301   Mov jAlaph
1302   Mov jKamera
1303 '
1304   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1305  EndIf
1306 '................................
1307  If (MELOZOTERULET=30 And MTERULET=50) Then 'v�lt�s megfog� �r�t�sr�l kamer�sra
1308   Mov jMurites
1309   Mov jKamera
1310 '
1311   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1312  EndIf
1313 '................................
1314  If (MELOZOTERULET=40 And MTERULET=50) Then 'v�lt�s felv�telr�l kamer�sra
1315   Mov jFelvetel
1316   Mov jAlaph
1317   Mov jKamera
1318 '
1319   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1320  EndIf
1321 '................................
1322  If (MELOZOTERULET=60 And MTERULET=50) Then 'v�lt�s lerak�sr�l kamer�sra
1323   Mov jLerakas
1324   Mov jKamera
1325 '
1326   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1327  EndIf
1328 '................................
1329  If (MELOZOTERULET=10 And MTERULET=60) Then 'v�lt�s alaphelyzetb�l lerak�sra
1330   Mov jAlaph
1331   Mov jLerakas
1332 '
1333   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1334  EndIf
1335 '................................
1336  If (MELOZOTERULET=20 And MTERULET=60) Then 'v�lt�s szervizpoz�ci�r�l lerak�sra
1337   Mov jSzerviz
1338   Mov jAlaph
1339   Mov jLerakas
1340 '
1341   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1342  EndIf
1343 '................................
1344  If (MELOZOTERULET=30 And MTERULET=60) Then 'v�lt�s megfog� �r�t�sr�l lerak�sra
1345   Mov jMurites
1346   Mov jLerakas
1347 '
1348   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1349  EndIf
1350 '................................
1351  If (MELOZOTERULET=40 And MTERULET=60) Then 'v�lt�s felv�telr�l lerak�sra
1352   Mov jFelvetel
1353   Mov jAlaph
1354   Mov jLerakas
1355 '
1356   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1357  EndIf
1358 '................................
1359  If (MELOZOTERULET=50 And MTERULET=60) Then 'v�lt�s kamer�sr�l lerak�sra
1360   Mov jKamera
1361   Mov jLerakas
1362 '
1363   MELOZOTERULET=MTERULET '�j tengely poz�ci�k felv�tele megt�rt�nt
1364  EndIf
1365 '................................
1366 EndIf
1367 Return '=======================
1368 '
1369 '
1370 'K�rtya Inputok:
1371 '0 - Gy�rilag Foglalt
1372 '1 -
1373 '2 -
1374 '3 -
1375 '4 -
1376 '5 -
1377 '6 -
1378 '7 -
1379 '8 -
1380 '9 -
1381 '10 -
1382 '11 -
1383 '12 -
1384 '13 -
1385 '14 -
1386 '15 -
1387 '
1388 'K�rtya Outputok:
1389 '0 -
1390 '1 -
1391 '2 -
1392 '3 -
1393 '4 -
1394 '5 -
1395 '6 -
1396 '7 -
1397 '8 -
1398 '9 -
1399 '10 -
1400 '11 -
1401 '12 -
1402 '13 -
1403 '14 -
1404 '15 -
1405 '
1406 '
1407 'Tool-ok:
1408 '0=semmilyen tool
1409 '
1410 '=================================================================
1411 '=================================================================
1412 '=================================================================
1413 '*******************************FELV�TEL TEST**************************
1414 '=================================================================
1415 '=================================================================
1416 '=================================================================
1417 '
1418 Cnt 0
1419 '
1420 ptemp1=pFPozHFF
1421 ptemp1.Z=pFPozHFF.Z+50
1422  Mov ptemp1
1423  Dly 0.01
1424  Mvs pFPozHFF
1425 Dly 0.01
1426 '=================================================================
1427 '=================================================================
1428 ptemp1=pFPozTFF
1429 ptemp1.Z=pFPozTFF.Z+50
1430  Mov ptemp1
1431  Dly 0.01
1432  Mvs pFPozTFF
1433 Dly 0.01
1434 '=================================================================
1435 '=================================================================
1436 ptemp1=pFPozTHFF
1437 ptemp1.Z=pFPozTHFF.Z+50
1438  Mov ptemp1
1439  Dly 0.01
1440  Mvs pFPozTHFF
1441 Dly 0.01
1442 '=================================================================
1443 '=================================================================
1444 ptemp1=pFPozHNFF
1445 ptemp1.Z=pFPozHNFF.Z+50
1446  Mov ptemp1
1447  Dly 0.01
1448  Mvs pFPozHNFF
1449 Dly 0.01
1450 '=================================================================
1451 '=================================================================
1452 '=================================================================
1453 '=================================================================
1454 '=================================================================
1455 '**************************LERAK�S TEST***************************
1456 '=================================================================
1457 '=================================================================
1458 '=================================================================
1459 '
1460 Cnt 0
1461 '
1462 ptemp1=pLPozHLL
1463 ptemp1.Z=pLPozHLL.Z+50
1464  Mov ptemp1
1465  Dly 0.01
1466  Mvs pLPozHLL
1467 Dly 0.01
1468 '=================================================================
1469 '=================================================================
1470 ptemp1=pLPozTLL
1471 ptemp1.Z=pLPozTLL.Z+50
1472  Mov ptemp1
1473  Dly 0.01
1474  Mvs pLPozTLL
1475 Dly 0.01
1476 '=================================================================
1477 '=================================================================
1478 ptemp1=pLPozTHLL
1479 ptemp1.Z=pLPozTHLL.Z+50
1480  Mov ptemp1
1481  Dly 0.01
1482  Mvs pLPozTHLL
1483 Dly 0.01
1484 '=================================================================
1485 '=================================================================
1486 ptemp1=pLPozHNLL
1487 ptemp1.Z=pLPozHNLL.Z+50
1488  Mov ptemp1
1489  Dly 0.01
1490  Mvs pLPozHNLL
1491 Dly 0.01
1492 '=================================================================
1493 '=================================================================
pKamera(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pKamera(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pKamera(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pKamera(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(11)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(12)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(13)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(14)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(15)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(16)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(17)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(18)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(19)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(20)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(21)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(22)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(23)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(24)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozH(25)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozH(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozH(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozH(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozH(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozH(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozH(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(11)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(12)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(13)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(14)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(15)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(16)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(17)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(18)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(19)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(20)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(21)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(22)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(23)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(24)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(25)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(26)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(27)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(28)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(29)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(30)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(31)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(32)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(33)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(34)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozT(35)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozT(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(11)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(12)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(13)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(14)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(15)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(16)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(17)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(18)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(19)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(20)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(21)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(22)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(23)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(24)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(25)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(26)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(27)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(28)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(29)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(30)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(31)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(32)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(33)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(34)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(35)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(36)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(37)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(38)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(39)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(40)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(41)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(42)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(43)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(44)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(45)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(46)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(47)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(48)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(49)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(50)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(51)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(52)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(53)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(54)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(55)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(56)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(57)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(58)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(59)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(60)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(61)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(62)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(63)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(64)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(65)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(66)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(67)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(68)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(69)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTH(70)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(11)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(12)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(13)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(14)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(15)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTH(16)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(11)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(12)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(13)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(14)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(15)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(16)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(17)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(18)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(19)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(20)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(21)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(22)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(23)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(24)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(25)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(26)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(27)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(28)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(29)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(30)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(31)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(32)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(33)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(34)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(35)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(36)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(37)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(38)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(39)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(40)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(41)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(42)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(43)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(44)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(45)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(46)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(47)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(48)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(49)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(50)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(51)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(52)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(53)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(54)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(55)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(56)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(57)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(58)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(59)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(60)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(61)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(62)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(63)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(64)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(65)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(66)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(67)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(68)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(69)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(70)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(71)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(72)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(73)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(74)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(75)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(76)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(77)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(78)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(79)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(80)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(81)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(82)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(83)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(84)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(85)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(86)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(87)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHN(88)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(1)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(2)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(3)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(4)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(5)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(6)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(7)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(8)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(9)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(10)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(11)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(12)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(13)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(14)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(15)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(16)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(17)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(18)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(19)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(20)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(21)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(22)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(23)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHN(24)=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
ptemp0=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pMurites=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
ptemp1=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHFF=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTFF=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozTHFF=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pFPozHNFF=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHLL=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTLL=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozTHLL=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
pLPozHNLL=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)(,)
jKamera=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
jAlaph=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
jLerakas=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
jFelvetel=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
jSzerviz=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
jMurites=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
jtemp1=(0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00)
