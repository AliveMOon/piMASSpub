1    'F�vez�rl�, RV-13FRLM-R, K+F, BMR18905, RC1
2    'Nem �rok glob�lis v�ltoz�kat
3    'Ennek a programnak mindig futnia kell!
4    'A program csak is akkor tudja elind�tani, a kommunik�ci�s (2es) programot - abban az esetben, ha az le�llt -,
5    'ha a kulcs automat�ba van ford�tva
6    '
7    Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
8    Def Io ioHS1=Bit,10160
9    Def Io ioHS2=Bit,10161
10    Def Io ioCOM=DWord,10192
11    Def Io ioPIX=DWord,10224
12    Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
13    Def Io oMx=DWord,10256
14    Def Io oMy=DWord,10288
15    Def Io oMz=DWord,10320
16    Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
17    Def Io oMa=DWord,10352
18    Def Io oMb=DWord,10384
19    Def Io oMc=DWord,10416
20    Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
21    Def Io oMs12=DWord,10448
22    Def Io oMsMSTR=DWord,10480
23    Def Io oMsR2D=DWord,10512
24    oMsMSTR=0
25    M_Timer(1) = 0
26    'Helyi v�ltoz�k felsorol�s
27    Mtimeout=0
28    M99=0
29   Mprg1stop=0 '0 - nem siker�lt le�ll�tani / 1 - siker�lt le�ll�tani
30   Mprg1rst=0 '0 - nem siker�lt resetelni / 1 - siker�lt resetelni
31   Mprg1start=0 '0 - nem siker�lt elind�tani / 1 - siker�lt elind�tani
32   'Mprg2stop=0 '0 - nem siker�lt le�ll�tani / 1 - siker�lt le�ll�tani
33   'Mprg2rst=0 '0 - nem siker�lt resetelni / 1 - siker�lt resetelni
34   'Mprg2start=0 '0 - nem siker�lt elind�tani / 1 - siker�lt elind�tani
35   MhibaRst=0 '0 - nem siker�lt resetelni a hib�t / 1 - siker�lt resetelni a hib�t
36   MprogAllapot=0 '�llapotv�ltoz�
37   'Helyi v�ltoz�k felsorol�s v�ge--------
38   '-----------------------------------------
39   Mtimeout=0 'Az elej�n fontos, hogy megv�rjuk a 2-es program elindul�s�st, mert az XRun parancs, csak is
40   'Automata m�dban haszn�lhat�
41   'While (M_Run(2)<>1 And Mtimeout<50) 'Addig fut am�g 2-es a program el nem indul,
42    'vagy le nem j�r az id�z�t�
43    'Dly 0.1 'Itt fontos, hogy hiba eset�n ne v�rjon tov�bb, mert akkor �jb�l kell resetelni
44    'Mtimeout=Mtimeout+1
45   'WEnd
46   '-----------------------------------------
47   'V�gtelen ciklus
48   While (M99=0)
49   '
50    GoSub *FEEDBACK
51    If M_Psa(3)=1 Then
52      If M_Run(3)=1 Then
53        If M_Wai(3)=1 Then
54          XRun 3
55        EndIf
56      Else  '#####2
57        XRun 3,"R2D2",1
58      EndIf '#####2
59    Else  '#####1
60      If M_Run(3)=1 Then
61        If M_Wai(3)=1 Then
62             XRun 3
63        EndIf
64       Else
65          XRun 3
66      EndIf
67    EndIf '#####1
68    If MprogAllapot=0 Then
69     'Mrun2 = M_Run(2)
70     Mmode = M_Mode
71     Mrun1 = M_Run(1)
72     Merr = M_Err
73     Mpsa1 = M_Psa(1)
74     Mw1 = M_Wai(1)
75   '::::::::::::::::::::::::::::::::::::::::::::::::::
76     '::A glob�lis v�ltoz�k �rt�ket helyi v�ltoz�kba teszem, hogy a felt�telvizsg�lat k�zben nehogy egy
77     '::megv�ltozott �rt�ket vizsg�ljak
78     '::::::::::::::::::::::::::::::::::::::::::::::::::
79     If Merr=1 Then 'Van hiba? #####1
80      MprogAllapot=100 'Ha hiba van, akkor az els�dleges feladat az 1-es program le�ll�t�sa, �s a hiba resetel�se
81     Else '#####1
82      'If Mrun2=0 Then 'Nem fut a kommunik�ci�? #####2
83      ' MprogAllapot=500 'Nem fut a 2es program, akkor elind�t�s
84      'Else '#####2
85       If Mmode=1 Then 'Manualban van? #####3
86        If Mrun1=1 Then 'Fut az 1-es program?
87         MprogAllapot=1000 'Ha manualban fut az 1-es program, akkor le�ll�t�s
88        EndIf
89        If Mw1 = 1 Then  ' ne resetelje mert nem lehet debugolni
90        '
91        Else
92          If Mpsa1=0 Then 'Kiv�laszthat� az 1-es program?
93           MprogAllapot=1100 'Ha manualban nem kiv�laszthat� az 1-es program, akkor reset
94          EndIf
95        EndIf
96       Else 'Automat�ban van a kulcs #####3
97        If Mrun1=0 Then 'Nem fut az 1es program?
98         MprogAllapot=1500 'Ha automat�ban nem fut az 1-es program, akkor 1-es stop->reset->start
99        EndIf
100       EndIf '#####3
101      'EndIf '#####2
102     EndIf '#####1
103    EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
104   '
105   '
106   '
107   '
108    If MprogAllapot=100 Or M_In(10162)=1 Then 'Le�ll�t�sa hiba eset�n
109     GoSub *PROG1STOP
110     If Mprg1stop=0 Then
111      MprogAllapot=100 'Ha nem �llt le a program, akkor pr�ba �jra
112     Else
113      MprogAllapot=200 'Sikeres le�ll�s eset�n, j�het a hiba reset
114     EndIf
115    EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
116   '
117    If MprogAllapot=200 Then
118     GoSub *HIBARST
119     If MhibaRst=0 Then
120     MprogAllapot=200 'Ha megmaradt a hiba, akkor �jbol pr�b�lkozik a resetel�sre
121    Else
122     MprogAllapot=0 'Hiba sikeresen elt�ntetve, vissza az �llapotkeres�shez
123    EndIf
124   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
125  '
126  ' If MprogAllapot=500 Then '2es program stop
127  '  GoSub *PROG2STOP
128  '  If Mprg2stop=0 Then
129  '   MprogAllapot=500 'Ha nem �llt le a program, akkor pr�ba �jra
130  '  Else
131  '   MprogAllapot=600 'Sikeres le�ll�s eset�n, j�het a program reset
132  '  EndIf
133  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
134  '
135  ' If MprogAllapot=600 Then '2es program reset
136  '  GoSub *PROG2RST
137  '  If Mprg2rst=0 Then
138  '   MprogAllapot=600 'Ha nincs resetelve a program, akkor pr�ba �jra
139  '  Else
140  '   MprogAllapot=700 'Sikeres reset eset�n, j�het a program ind�t�s
141  '  EndIf
142  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
143  '
144  ' If MprogAllapot=700 Then '2es program ind�t�s
145  '  GoSub *PROG2START
146  '  If M_Err=1 Then
147  '   MprogAllapot=0 'Ha hiba t�rt�nt, akkor ugr�s vissza 0-ba
148  '  Else
149  '   If Mprg2start=0 Then 'nem indult el a program?
150  '    MprogAllapot=700 'Sikertelen ind�t�s eset�n �jrapr�b�lkoz�s
151  '   Else
152  '    MprogAllapot=0 'Sikeres ind�t�s eset�n, ugr�s vissza 0-ba
153  '   EndIf
154  '  EndIf
155  '  MprogAllapot=0
156  ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
157  '
158   If MprogAllapot=1000 Then '1es program stop
159    GoSub *PROG1STOP
160    If Mprg1stop=0 Then
161     MprogAllapot=1000 'Ha nem �llt le a program, akkor pr�ba �jra
162    Else
163     MprogAllapot=1100 'Sikeres le�ll�s eset�n, j�het a program reset
164    EndIf
165   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
166  '
167   If MprogAllapot=1100 Then '1es program reset
168    GoSub *PROG1RST
169    If Mprg1rst=0 Then
170     MprogAllapot=1100 'Ha nincs resetelve a program, akkor pr�ba �jra
171    Else
172     MprogAllapot=0 'Sikeres reset eset�n, ugr�s vissza 0-ba
173    EndIf
174   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
175  '
176   If MprogAllapot=1500 Then '1es program stop
177    GoSub *PROG1STOP
178    If Mprg1stop=0 Then
179     MprogAllapot=1500 'Ha nem �llt le a program, akkor pr�ba �jra
180    Else
181     MprogAllapot=1600 'Sikeres le�ll�s eset�n, j�het a program reset
182    EndIf
183   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
184  '
185   If MprogAllapot=1600 Then '1es program reset
186    GoSub *PROG1RST
187    If Mprg1rst=0 Then
188     MprogAllapot=1600 'Ha nincs resetelve a program, akkor pr�ba �jra
189    Else
190     MprogAllapot=1700 'Sikeres reset eset�n, j�het a program ind�t�s
191    EndIf
192   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
193  '
194   If MprogAllapot=1700 Then '1es program ind�t�s
195    GoSub *PROG1START
196    If M_Err=1 Then
197     MprogAllapot=0 'Ha hiba t�rt�nt, akkor ugr�s vissza 0-ba
198    Else
199     If M_Mode = 1 Then 'Ha manualban van, akkor ugr�s vissza 0-ba
200      MprogAllapot = 0
201     Else
202      If Mprg1start=0 Then 'nem indult el a program?
203       MprogAllapot=1700 'Sikertelen ind�t�s eset�n �jrapr�b�lkoz�s
204      Else
205       MprogAllapot=0 'Sikeres ind�t�s eset�n, ugr�s vissza 0-ba
206      EndIf
207     EndIf
208    EndIf
209   EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
210  '
211  Dly 0.001 '1 ms-es k�sleltet�s, hogy a ciklus ne fusson t�l gyorsan, ha �pp nem teljes�l semmilyen felt�tel
212  WEnd
213  End
214  '---------------------------------
215  'Szubrutinok k�vetkeznek-----
216  '---------------------------------
217  *FEEDBACK '=======================
218    Mms = M_Timer(1)
219    Ms=Mms/1000.0
220    oMsMSTR = Mms
221    oMx=P_Fbc.X*100
222    oMy=P_Fbc.Y*100
223    oMz=P_Fbc.Z*100
224    oMa=Deg(P_Fbc.A)*100
225    oMb=Deg(P_Fbc.B)*100
226    oMc=Deg(P_Fbc.C)*100
227   Return '=======================
228  '
229  *PROG1STOP 'Az 1es program le�ll�t�sa
230    XStp 1
231    Mtimeout=0
232    While (M_Run(1)<>0 And Mtimeout<30) 'Addig fut am�g a program le nem �ll
233     Dly 0.1
234     Mtimeout=Mtimeout+1
235    WEnd
236    If Mtimeout>=30 Then Mprg1stop=0 Else Mprg1stop=1 'Hiba eset�n jelz�s
237  Return
238  '
239  *PROG1RST 'Az 1es program resetel�se
240    XRst 1
241    Mtimeout=0
242    While (M_Psa(1)=0 And Mtimeout<30) 'Addig fut am�g a program nem a selectable �llapotban van
243     Dly 0.1
244     Mtimeout=Mtimeout+1
245    WEnd
246    If Mtimeout>=30 Then Mprg1rst=0 Else Mprg1rst=1 'Hiba eset�n jelz�s
247  Return
248  '
249  *PROG1START 'Az 1es program ind�t�sa
250    XRun 1,"SLAVE",1
251    Mtimeout=0
252    While (M_Run(1)<>1 And Mtimeout<60 And M_Err=0 And M_Mode<>1)
253     'Addig fut am�g a program el nem indul
254     Dly 0.1 'Itt fontos, hogy hiba eset�n ne v�rjon tov�bb, mert akkor �jb�l kell resetelni
255     Mtimeout=Mtimeout+1
256    WEnd
257    If (Mtimeout>=60 Or M_Err=1 Or M_Mode=1) Then Mprg1start=0 Else Mprg1start=1 'Hiba eset�n jelz�s
258  Return
259  '
260  '*PROG2STOP 'Az 2es program le�ll�t�sa
261  '  XStp 2
262  '  Mtimeout=0
263  '  While (M_Run(2)<>0 And Mtimeout<30) 'Addig fut am�g a program le nem �ll
264  '   Dly 0.1
265  '   Mtimeout=Mtimeout+1
266  '  WEnd
267  '  If Mtimeout>=30 Then Mprg2stop=0 Else Mprg2stop=1 'Hiba eset�n jelz�s
268  'Return
269  '
270  '*PROG2RST 'Az 2es program resetel�se
271  '  XRst 2
272  '  Mtimeout=0
273  '  While (M_Psa(2)=0 And Mtimeout<30) 'Addig fut am�g a program nem a selectable �llapotban van
274  '   Dly 0.1
275  '   Mtimeout=Mtimeout+1
276  '  WEnd
277  '  If Mtimeout>=30 Then Mprg2rst=0 Else Mprg2rst=1 'Hiba eset�n jelz�s
278  'Return
279  '
280  '*PROG2START 'Az 2es program ind�t�sa
281  '  XRun 2,"2"
282  '  Mtimeout=0
283  '  While (M_Run(2)<>1 And Mtimeout<60 And M_Err=0) 'Addig fut am�g a program el nem indul
284  '   Dly 0.1 'Itt fontos, hogy hiba eset�n ne v�rjon tov�bb, mert akkor �jb�l kell resetelni
285  '   Mtimeout=Mtimeout+1
286  '  WEnd
287  '  If (Mtimeout>=60 Or M_Err=1) Then Mprg2start=0 Else Mprg2start=1 'Hiba eset�n jelz�s
288  'Return
289  '
290  *HIBARST 'Ama el�j�v� ront�s tiszt��sa az �r neve �ltal
291    Dly 6
292    Reset Err
293    Mtimeout=0
294    While (Mtimeout<30 And M_Err=1) 'Addig a hiba el nem t�nik vagy timeout
295     Dly 0.1
296     Mtimeout=Mtimeout+1
297    WEnd
298    If (Mtimeout>=30) Then MhibaRst=0 Else MhibaRst=1 'Hiba eset�n jelz�s
299  Return
300  '
