1    Clr 1
2    'F�vez�rl�, RV-13FRLM-R, K+F, BMR18905, RC1
3    'Nem �rok glob�lis v�ltoz�kat
4    'Ennek a programnak mindig futnia kell!
5    'A program csak is akkor tudja elind�tani, a kommunik�ci�s (2es) programot - abban az esetben, ha az le�llt -,
6    'ha a kulcs automat�ba van ford�tva
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
30    'Helyi v�ltoz�k felsorol�s
31    ' zero
32    M_Timer(1)=0
33    ioMsMST=0
34    Mtimeout=0
35    Const Def Long BILL=&H4C4C4942
36    Const Def Long JOHN=&H4E484F4A
37    ioNM=JOHN&
38    'ioNM=BILL&
39   M99=0
40   Mprg1stop=0 '0 - nem siker�lt le�ll�tani / 1 - siker�lt le�ll�tani
41   Mprg1rst=0 '0 - nem siker�lt resetelni / 1 - siker�lt resetelni
42   Mprg1start=0 '0 - nem siker�lt elind�tani / 1 - siker�lt elind�tani
43   'Mprg2stop=0 '0 - nem siker�lt le�ll�tani / 1 - siker�lt le�ll�tani
44   'Mprg2rst=0 '0 - nem siker�lt resetelni / 1 - siker�lt resetelni
45   'Mprg2start=0 '0 - nem siker�lt elind�tani / 1 - siker�lt elind�tani
46   MhibaRst=0 '0 - nem siker�lt resetelni a hib�t / 1 - siker�lt resetelni a hib�t
47   MprogAllapot=0 '�llapotv�ltoz�
48   'Helyi v�ltoz�k felsorol�s v�ge--------
49   '-----------------------------------------
50   Mtimeout=0 'Az elej�n fontos, hogy megv�rjuk a 2-es program elindul�s�st, mert az XRun parancs, csak is
51   'Automata m�dban haszn�lhat�
52   'While (M_Run(2)<>1 And Mtimeout<50) 'Addig fut am�g 2-es a program el nem indul,
53    'vagy le nem j�r az id�z�t�
54    'Dly 0.1 'Itt fontos, hogy hiba eset�n ne v�rjon tov�bb, mert akkor �jb�l kell resetelni
55    'Mtimeout=Mtimeout+1
56   'WEnd
57   '-----------------------------------------
58   'V�gtelen ciklus
59   While (M99=0)
60   '
61    GoSub *FEEDBACK
62    If M_Psa(3)=1 Then
63      If M_Run(3)=1 Then
64        If M_Wai(3)=1 Then
65          XRun 3
66        EndIf
67      Else  '#####2
68        XRun 3,"R2D2",1
69      EndIf '#####2
70    Else  '#####1
71      If M_Run(3)=1 Then
72        If M_Wai(3)=1 Then
73             XRun 3
74        EndIf
75       Else
76          XRun 3
77      EndIf
78    EndIf '#####1
79    If MprogAllapot=0 Then
80     'Mrun2 = M_Run(2)
81     Mmode = M_Mode
82     Mrun1 = M_Run(1)
83     Merr = M_Err
84     Mpsa1 = M_Psa(1)
85     Mw1 = M_Wai(1)
86   '::::::::::::::::::::::::::::::::::::::::::::::::::
87     '::A glob�lis v�ltoz�k �rt�ket helyi v�ltoz�kba teszem, hogy a felt�telvizsg�lat k�zben nehogy egy
88     '::megv�ltozott �rt�ket vizsg�ljak
89     '::::::::::::::::::::::::::::::::::::::::::::::::::
90     If Merr=1 Then 'Van hiba? #####1
91      MprogAllapot=100 'Ha hiba van, akkor az els�dleges feladat az 1-es program le�ll�t�sa, �s a hiba resetel�se
92     Else '#####1
93      'If Mrun2=0 Then 'Nem fut a kommunik�ci�? #####2
94      ' MprogAllapot=500 'Nem fut a 2es program, akkor elind�t�s
95      'Else '#####2
96       If Mmode=1 Then 'Manualban van? #####3
97        If Mrun1=1 Then 'Fut az 1-es program?
98         MprogAllapot=1000 'Ha manualban fut az 1-es program, akkor le�ll�t�s
99        EndIf
100        If Mw1 = 1 Then  ' ne resetelje mert nem lehet debugolni
101        '
102        Else
103          If Mpsa1=0 Then 'Kiv�laszthat� az 1-es program?
104           MprogAllapot=1100 'Ha manualban nem kiv�laszthat� az 1-es program, akkor reset
105          EndIf
106        EndIf
107       Else 'Automat�ban van a kulcs #####3
108        If Mrun1=0 Then 'Nem fut az 1es program?
109         MprogAllapot=1500 'Ha automat�ban nem fut az 1-es program, akkor 1-es stop->reset->start
110        EndIf
111       EndIf '#####3
112      'EndIf '#####2
113     EndIf '#####1
114    EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
115   '
116   '
117   '
118   '
119    If MprogAllapot=100 Or M_In(10162)=1 Then 'Le�ll�t�sa hiba eset�n
120     GoSub *PROG1STOP
121     If Mprg1stop=0 Then
122      MprogAllapot=100 'Ha nem �llt le a program, akkor pr�ba �jra
123     Else
124      MprogAllapot=200 'Sikeres le�ll�s eset�n, j�het a hiba reset
125     EndIf
126    EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
127   '
128    If MprogAllapot=200 Then
129     GoSub *HIBARST
130     If MhibaRst=0 Then
131     MprogAllapot=200 'Ha megmaradt a hiba, akkor �jbol pr�b�lkozik a resetel�sre
132    Else
133     MprogAllapot=0 'Hiba sikeresen elt�ntetve, vissza az �llapotkeres�shez
134    EndIf
135   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
136  '
137  ' If MprogAllapot=500 Then '2es program stop
138  '  GoSub *PROG2STOP
139  '  If Mprg2stop=0 Then
140  '   MprogAllapot=500 'Ha nem �llt le a program, akkor pr�ba �jra
141  '  Else
142  '   MprogAllapot=600 'Sikeres le�ll�s eset�n, j�het a program reset
143  '  EndIf
144  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
145  '
146  ' If MprogAllapot=600 Then '2es program reset
147  '  GoSub *PROG2RST
148  '  If Mprg2rst=0 Then
149  '   MprogAllapot=600 'Ha nincs resetelve a program, akkor pr�ba �jra
150  '  Else
151  '   MprogAllapot=700 'Sikeres reset eset�n, j�het a program ind�t�s
152  '  EndIf
153  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
154  '
155  ' If MprogAllapot=700 Then '2es program ind�t�s
156  '  GoSub *PROG2START
157  '  If M_Err=1 Then
158  '   MprogAllapot=0 'Ha hiba t�rt�nt, akkor ugr�s vissza 0-ba
159  '  Else
160  '   If Mprg2start=0 Then 'nem indult el a program?
161  '    MprogAllapot=700 'Sikertelen ind�t�s eset�n �jrapr�b�lkoz�s
162  '   Else
163  '    MprogAllapot=0 'Sikeres ind�t�s eset�n, ugr�s vissza 0-ba
164  '   EndIf
165  '  EndIf
166  '  MprogAllapot=0
167  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
168  '
169   If MprogAllapot=1000 Then '1es program stop
170    GoSub *PROG1STOP
171    If Mprg1stop=0 Then
172     MprogAllapot=1000 'Ha nem �llt le a program, akkor pr�ba �jra
173    Else
174     MprogAllapot=1100 'Sikeres le�ll�s eset�n, j�het a program reset
175    EndIf
176   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
177  '
178   If MprogAllapot=1100 Then '1es program reset
179    GoSub *PROG1RST
180    If Mprg1rst=0 Then
181     MprogAllapot=1100 'Ha nincs resetelve a program, akkor pr�ba �jra
182    Else
183     MprogAllapot=0 'Sikeres reset eset�n, ugr�s vissza 0-ba
184    EndIf
185   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
186  '
187   If MprogAllapot=1500 Then '1es program stop
188    GoSub *PROG1STOP
189    If Mprg1stop=0 Then
190     MprogAllapot=1500 'Ha nem �llt le a program, akkor pr�ba �jra
191    Else
192     MprogAllapot=1600 'Sikeres le�ll�s eset�n, j�het a program reset
193    EndIf
194   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
195  '
196   If MprogAllapot=1600 Then '1es program reset
197    GoSub *PROG1RST
198    If Mprg1rst=0 Then
199     MprogAllapot=1600 'Ha nincs resetelve a program, akkor pr�ba �jra
200    Else
201     MprogAllapot=1700 'Sikeres reset eset�n, j�het a program ind�t�s
202    EndIf
203   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
204  '
205   If MprogAllapot=1700 Then '1es program ind�t�s
206    GoSub *PROG1START
207    If M_Err=1 Then
208     MprogAllapot=0 'Ha hiba t�rt�nt, akkor ugr�s vissza 0-ba
209    Else
210     If M_Mode = 1 Then 'Ha manualban van, akkor ugr�s vissza 0-ba
211      MprogAllapot = 0
212     Else
213      If Mprg1start=0 Then 'nem indult el a program?
214       MprogAllapot=1700 'Sikertelen ind�t�s eset�n �jrapr�b�lkoz�s
215      Else
216       MprogAllapot=0 'Sikeres ind�t�s eset�n, ugr�s vissza 0-ba
217      EndIf
218     EndIf
219    EndIf
220   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
221  '
222  Dly 0.001 '1 ms-es k�sleltet�s, hogy a ciklus ne fusson t�l gyorsan, ha �pp nem teljes�l semmilyen felt�tel
223  WEnd
224  End
225  '---------------------------------
226  'Szubrutinok k�vetkeznek-----
227  '---------------------------------
228  *FEEDBACK '=======================
229    Mms = M_Timer(1)
230    Ms=Mms/1000.0
231    ioMsMST = Mms
232    ioMx=P_Fbc.X*100
233    ioMy=P_Fbc.Y*100
234    ioMz=P_Fbc.Z*100
235    ioMa=Deg(P_Fbc.A)*100
236    ioMb=Deg(P_Fbc.B)*100
237    ioMc=Deg(P_Fbc.C)*100
238   Return '=======================
239  '
240  *PROG1STOP 'Az 1es program le�ll�t�sa
241    XStp 1
242    Mtimeout=0
243    While (M_Run(1)<>0 And Mtimeout<30) 'Addig fut am�g a program le nem �ll
244     Dly 0.1
245     Mtimeout=Mtimeout+1
246    WEnd
247    If Mtimeout>=30 Then Mprg1stop=0 Else Mprg1stop=1 'Hiba eset�n jelz�s
248  Return
249  '
250  *PROG1RST 'Az 1es program resetel�se
251    XRst 1
252    Mtimeout=0
253    While (M_Psa(1)=0 And Mtimeout<30) 'Addig fut am�g a program nem a selectable �llapotban van
254     Dly 0.1
255     Mtimeout=Mtimeout+1
256    WEnd
257    If Mtimeout>=30 Then Mprg1rst=0 Else Mprg1rst=1 'Hiba eset�n jelz�s
258  Return
259  '
260  *PROG1START 'Az 1es program ind�t�sa
261    XRun 1,"SLAVE",1
262    Mtimeout=0
263    While (M_Run(1)<>1 And Mtimeout<60 And M_Err=0 And M_Mode<>1)
264     'Addig fut am�g a program el nem indul
265     Dly 0.1 'Itt fontos, hogy hiba eset�n ne v�rjon tov�bb, mert akkor �jb�l kell resetelni
266     Mtimeout=Mtimeout+1
267    WEnd
268    If (Mtimeout>=60 Or M_Err=1 Or M_Mode=1) Then Mprg1start=0 Else Mprg1start=1 'Hiba eset�n jelz�s
269  Return
270  '
271  '*PROG2STOP 'Az 2es program le�ll�t�sa
272  '  XStp 2
273  '  Mtimeout=0
274  '  While (M_Run(2)<>0 And Mtimeout<30) 'Addig fut am�g a program le nem �ll
275  '   Dly 0.1
276  '   Mtimeout=Mtimeout+1
277  '  WEnd
278  '  If Mtimeout>=30 Then Mprg2stop=0 Else Mprg2stop=1 'Hiba eset�n jelz�s
279  'Return
280  '
281  '*PROG2RST 'Az 2es program resetel�se
282  '  XRst 2
283  '  Mtimeout=0
284  '  While (M_Psa(2)=0 And Mtimeout<30) 'Addig fut am�g a program nem a selectable �llapotban van
285  '   Dly 0.1
286  '   Mtimeout=Mtimeout+1
287  '  WEnd
288  '  If Mtimeout>=30 Then Mprg2rst=0 Else Mprg2rst=1 'Hiba eset�n jelz�s
289  'Return
290  '
291  '*PROG2START 'Az 2es program ind�t�sa
292  '  XRun 2,"2"
293  '  Mtimeout=0
294  '  While (M_Run(2)<>1 And Mtimeout<60 And M_Err=0) 'Addig fut am�g a program el nem indul
295  '   Dly 0.1 'Itt fontos, hogy hiba eset�n ne v�rjon tov�bb, mert akkor �jb�l kell resetelni
296  '   Mtimeout=Mtimeout+1
297  '  WEnd
298  '  If (Mtimeout>=60 Or M_Err=1) Then Mprg2start=0 Else Mprg2start=1 'Hiba eset�n jelz�s
299  'Return
300  '
301  *HIBARST 'Ama el�j�v� ront�s tiszt��sa az �r neve �ltal
302    Dly 6
303    Reset Err
304    Mtimeout=0
305    While (Mtimeout<30 And M_Err=1) 'Addig a hiba el nem t�nik vagy timeout
306     Dly 0.1
307     Mtimeout=Mtimeout+1
308    WEnd
309    If (Mtimeout>=30) Then MhibaRst=0 Else MhibaRst=1 'Hiba eset�n jelz�s
310  Return
311  '
