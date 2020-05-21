1    Clr 1
2    'Fõvezérlõ, RV-13FRLM-R, K+F, BMR18905, RC1
3    'Nem írok globális változókat
4    'Ennek a programnak mindig futnia kell!
5    'A program csak is akkor tudja elindítani, a kommunikációs (2es) programot - abban az esetben, ha az leállt -,
6    'ha a kulcs automatába van fordítva
7    '
8    Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
9    Def Io ioNM=DWord,10160
10    Def Io ioMx=DWord,10192
11    Def Io ioMy=DWord,10224
12    Def Io ioMz=DWord,10256
13    Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
14    Def Io ioMa=DWord,10288
15    Def Io ioMb=DWord,10320
16    Def Io ioMc=DWord,10352
17    Rem -  -- ---=--==-=== TIME ===-==--=--- --  -
18    Def Io ioMs=DWord,10384
19    Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
20    Def Io ioHS1=Bit,10416
21    Def Io ioHS2=Bit,10417
22    Def Io ioHS3=Bit,10418
23    Def Io ioMajre=Bit,10419
24    Def Io ioPIX=DWord,10448
25    Def Io ioCOM=DWord,10480
26    Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
27    Def Io ioMsSLV=DWord,10512
28    Def Io ioMsMST=DWord,10544
29    Def Io ioMsR2D=DWord,10576
30    'Helyi változók felsorolás
31    ' zero
32    M_Timer(1)=ioMsMST=Mtimeout=0
33    Const Def Long BILL=&H4C4C4942
34    Const Def Long JOHN=&H4E484F4A
35    ioNM=JOHN&
36   M99=0
37   Mprg1stop=0 '0 - nem sikerült leállítani / 1 - sikerült leállítani
38   Mprg1rst=0 '0 - nem sikerült resetelni / 1 - sikerült resetelni
39   Mprg1start=0 '0 - nem sikerült elindítani / 1 - sikerült elindítani
40   'Mprg2stop=0 '0 - nem sikerült leállítani / 1 - sikerült leállítani
41   'Mprg2rst=0 '0 - nem sikerült resetelni / 1 - sikerült resetelni
42   'Mprg2start=0 '0 - nem sikerült elindítani / 1 - sikerült elindítani
43   MhibaRst=0 '0 - nem sikerült resetelni a hibát / 1 - sikerült resetelni a hibát
44   MprogAllapot=0 'Állapotváltozó
45   'Helyi változók felsorolás vége--------
46   '-----------------------------------------
47   Mtimeout=0 'Az elején fontos, hogy megvárjuk a 2-es program elindulásást, mert az XRun parancs, csak is
48   'Automata módban használható
49   'While (M_Run(2)<>1 And Mtimeout<50) 'Addig fut amíg 2-es a program el nem indul,
50    'vagy le nem jár az idõzítõ
51    'Dly 0.1 'Itt fontos, hogy hiba esetén ne várjon tovább, mert akkor újból kell resetelni
52    'Mtimeout=Mtimeout+1
53   'WEnd
54   '-----------------------------------------
55   'Végtelen ciklus
56   While (M99=0)
57   '
58    GoSub *FEEDBACK
59    If M_Psa(3)=1 Then
60      If M_Run(3)=1 Then
61        If M_Wai(3)=1 Then
62          XRun 3
63        EndIf
64      Else  '#####2
65        XRun 3,"R2D2",1
66      EndIf '#####2
67    Else  '#####1
68      If M_Run(3)=1 Then
69        If M_Wai(3)=1 Then
70             XRun 3
71        EndIf
72       Else
73          XRun 3
74      EndIf
75    EndIf '#####1
76    If MprogAllapot=0 Then
77     'Mrun2 = M_Run(2)
78     Mmode = M_Mode
79     Mrun1 = M_Run(1)
80     Merr = M_Err
81     Mpsa1 = M_Psa(1)
82     Mw1 = M_Wai(1)
83   '::::::::::::::::::::::::::::::::::::::::::::::::::
84     '::A globális változók értéket helyi változókba teszem, hogy a feltételvizsgálat közben nehogy egy
85     '::megváltozott értéket vizsgáljak
86     '::::::::::::::::::::::::::::::::::::::::::::::::::
87     If Merr=1 Then 'Van hiba? #####1
88      MprogAllapot=100 'Ha hiba van, akkor az elsõdleges feladat az 1-es program leállítása, és a hiba resetelése
89     Else '#####1
90      'If Mrun2=0 Then 'Nem fut a kommunikáció? #####2
91      ' MprogAllapot=500 'Nem fut a 2es program, akkor elindítás
92      'Else '#####2
93       If Mmode=1 Then 'Manualban van? #####3
94        If Mrun1=1 Then 'Fut az 1-es program?
95         MprogAllapot=1000 'Ha manualban fut az 1-es program, akkor leállítás
96        EndIf
97        If Mw1 = 1 Then  ' ne resetelje mert nem lehet debugolni
98        '
99        Else
100          If Mpsa1=0 Then 'Kiválasztható az 1-es program?
101           MprogAllapot=1100 'Ha manualban nem kiválasztható az 1-es program, akkor reset
102          EndIf
103        EndIf
104       Else 'Automatában van a kulcs #####3
105        If Mrun1=0 Then 'Nem fut az 1es program?
106         MprogAllapot=1500 'Ha automatában nem fut az 1-es program, akkor 1-es stop->reset->start
107        EndIf
108       EndIf '#####3
109      'EndIf '#####2
110     EndIf '#####1
111    EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
112   '
113   '
114   '
115   '
116    If MprogAllapot=100 Or M_In(10162)=1 Then 'Leállítása hiba esetén
117     GoSub *PROG1STOP
118     If Mprg1stop=0 Then
119      MprogAllapot=100 'Ha nem állt le a program, akkor próba újra
120     Else
121      MprogAllapot=200 'Sikeres leállás esetén, jöhet a hiba reset
122     EndIf
123    EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
124   '
125    If MprogAllapot=200 Then
126     GoSub *HIBARST
127     If MhibaRst=0 Then
128     MprogAllapot=200 'Ha megmaradt a hiba, akkor újbol próbálkozik a resetelésre
129    Else
130     MprogAllapot=0 'Hiba sikeresen eltüntetve, vissza az állapotkereséshez
131    EndIf
132   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
133  '
134  ' If MprogAllapot=500 Then '2es program stop
135  '  GoSub *PROG2STOP
136  '  If Mprg2stop=0 Then
137  '   MprogAllapot=500 'Ha nem állt le a program, akkor próba újra
138  '  Else
139  '   MprogAllapot=600 'Sikeres leállás esetén, jöhet a program reset
140  '  EndIf
141  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
142  '
143  ' If MprogAllapot=600 Then '2es program reset
144  '  GoSub *PROG2RST
145  '  If Mprg2rst=0 Then
146  '   MprogAllapot=600 'Ha nincs resetelve a program, akkor próba újra
147  '  Else
148  '   MprogAllapot=700 'Sikeres reset esetén, jöhet a program indítás
149  '  EndIf
150  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
151  '
152  ' If MprogAllapot=700 Then '2es program indítás
153  '  GoSub *PROG2START
154  '  If M_Err=1 Then
155  '   MprogAllapot=0 'Ha hiba történt, akkor ugrás vissza 0-ba
156  '  Else
157  '   If Mprg2start=0 Then 'nem indult el a program?
158  '    MprogAllapot=700 'Sikertelen indítás esetén újrapróbálkozás
159  '   Else
160  '    MprogAllapot=0 'Sikeres indítás esetén, ugrás vissza 0-ba
161  '   EndIf
162  '  EndIf
163  '  MprogAllapot=0
164  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
165  '
166   If MprogAllapot=1000 Then '1es program stop
167    GoSub *PROG1STOP
168    If Mprg1stop=0 Then
169     MprogAllapot=1000 'Ha nem állt le a program, akkor próba újra
170    Else
171     MprogAllapot=1100 'Sikeres leállás esetén, jöhet a program reset
172    EndIf
173   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
174  '
175   If MprogAllapot=1100 Then '1es program reset
176    GoSub *PROG1RST
177    If Mprg1rst=0 Then
178     MprogAllapot=1100 'Ha nincs resetelve a program, akkor próba újra
179    Else
180     MprogAllapot=0 'Sikeres reset esetén, ugrás vissza 0-ba
181    EndIf
182   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
183  '
184   If MprogAllapot=1500 Then '1es program stop
185    GoSub *PROG1STOP
186    If Mprg1stop=0 Then
187     MprogAllapot=1500 'Ha nem állt le a program, akkor próba újra
188    Else
189     MprogAllapot=1600 'Sikeres leállás esetén, jöhet a program reset
190    EndIf
191   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
192  '
193   If MprogAllapot=1600 Then '1es program reset
194    GoSub *PROG1RST
195    If Mprg1rst=0 Then
196     MprogAllapot=1600 'Ha nincs resetelve a program, akkor próba újra
197    Else
198     MprogAllapot=1700 'Sikeres reset esetén, jöhet a program indítás
199    EndIf
200   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
201  '
202   If MprogAllapot=1700 Then '1es program indítás
203    GoSub *PROG1START
204    If M_Err=1 Then
205     MprogAllapot=0 'Ha hiba történt, akkor ugrás vissza 0-ba
206    Else
207     If M_Mode = 1 Then 'Ha manualban van, akkor ugrás vissza 0-ba
208      MprogAllapot = 0
209     Else
210      If Mprg1start=0 Then 'nem indult el a program?
211       MprogAllapot=1700 'Sikertelen indítás esetén újrapróbálkozás
212      Else
213       MprogAllapot=0 'Sikeres indítás esetén, ugrás vissza 0-ba
214      EndIf
215     EndIf
216    EndIf
217   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
218  '
219  Dly 0.001 '1 ms-es késleltetés, hogy a ciklus ne fusson túl gyorsan, ha épp nem teljesül semmilyen feltétel
220  WEnd
221  End
222  '---------------------------------
223  'Szubrutinok következnek-----
224  '---------------------------------
225  *FEEDBACK '=======================
226    Mms = M_Timer(1)
227    Ms=Mms/1000.0
228    ioMsMST = Mms
229    ioMx=P_Fbc.X*100
230    ioMy=P_Fbc.Y*100
231    ioMz=P_Fbc.Z*100
232    ioMa=Deg(P_Fbc.A)*100
233    ioMb=Deg(P_Fbc.B)*100
234    ioMc=Deg(P_Fbc.C)*100
235   Return '=======================
236  '
237  *PROG1STOP 'Az 1es program leállítása
238    XStp 1
239    Mtimeout=0
240    While (M_Run(1)<>0 And Mtimeout<30) 'Addig fut amíg a program le nem áll
241     Dly 0.1
242     Mtimeout=Mtimeout+1
243    WEnd
244    If Mtimeout>=30 Then Mprg1stop=0 Else Mprg1stop=1 'Hiba esetén jelzés
245  Return
246  '
247  *PROG1RST 'Az 1es program resetelése
248    XRst 1
249    Mtimeout=0
250    While (M_Psa(1)=0 And Mtimeout<30) 'Addig fut amíg a program nem a selectable állapotban van
251     Dly 0.1
252     Mtimeout=Mtimeout+1
253    WEnd
254    If Mtimeout>=30 Then Mprg1rst=0 Else Mprg1rst=1 'Hiba esetén jelzés
255  Return
256  '
257  *PROG1START 'Az 1es program indítása
258    XRun 1,"SLAVE",1
259    Mtimeout=0
260    While (M_Run(1)<>1 And Mtimeout<60 And M_Err=0 And M_Mode<>1)
261     'Addig fut amíg a program el nem indul
262     Dly 0.1 'Itt fontos, hogy hiba esetén ne várjon tovább, mert akkor újból kell resetelni
263     Mtimeout=Mtimeout+1
264    WEnd
265    If (Mtimeout>=60 Or M_Err=1 Or M_Mode=1) Then Mprg1start=0 Else Mprg1start=1 'Hiba esetén jelzés
266  Return
267  '
268  '*PROG2STOP 'Az 2es program leállítása
269  '  XStp 2
270  '  Mtimeout=0
271  '  While (M_Run(2)<>0 And Mtimeout<30) 'Addig fut amíg a program le nem áll
272  '   Dly 0.1
273  '   Mtimeout=Mtimeout+1
274  '  WEnd
275  '  If Mtimeout>=30 Then Mprg2stop=0 Else Mprg2stop=1 'Hiba esetén jelzés
276  'Return
277  '
278  '*PROG2RST 'Az 2es program resetelése
279  '  XRst 2
280  '  Mtimeout=0
281  '  While (M_Psa(2)=0 And Mtimeout<30) 'Addig fut amíg a program nem a selectable állapotban van
282  '   Dly 0.1
283  '   Mtimeout=Mtimeout+1
284  '  WEnd
285  '  If Mtimeout>=30 Then Mprg2rst=0 Else Mprg2rst=1 'Hiba esetén jelzés
286  'Return
287  '
288  '*PROG2START 'Az 2es program indítása
289  '  XRun 2,"2"
290  '  Mtimeout=0
291  '  While (M_Run(2)<>1 And Mtimeout<60 And M_Err=0) 'Addig fut amíg a program el nem indul
292  '   Dly 0.1 'Itt fontos, hogy hiba esetén ne várjon tovább, mert akkor újból kell resetelni
293  '   Mtimeout=Mtimeout+1
294  '  WEnd
295  '  If (Mtimeout>=60 Or M_Err=1) Then Mprg2start=0 Else Mprg2start=1 'Hiba esetén jelzés
296  'Return
297  '
298  *HIBARST 'Ama elõjövõ rontás tisztíása az Úr neve által
299    Dly 6
300    Reset Err
301    Mtimeout=0
302    While (Mtimeout<30 And M_Err=1) 'Addig a hiba el nem tûnik vagy timeout
303     Dly 0.1
304     Mtimeout=Mtimeout+1
305    WEnd
306    If (Mtimeout>=30) Then MhibaRst=0 Else MhibaRst=1 'Hiba esetén jelzés
307  Return
308  '
