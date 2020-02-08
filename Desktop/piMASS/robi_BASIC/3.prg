1 'Fõvezérlõ, RV-13FRLM-R, K+F, BMR18905, RC1
2 'Nem írok globális változókat
3 'Ennek a programnak mindig futnia kell!
4 'A program csak is akkor tudja elindítani, a kommunikációs (2es) programot - abban az esetben, ha az leállt -,
5 'ha a kulcs automatába van fordítva
6 '
7 'Helyi változók felsorolás
8 Mtimeout=0
9 M99=0
10 Mprg1stop=0 '0 - nem sikerült leállítani / 1 - sikerült leállítani
11 Mprg1rst=0 '0 - nem sikerült resetelni / 1 - sikerült resetelni
12 Mprg1start=0 '0 - nem sikerült elindítani / 1 - sikerült elindítani
13 'Mprg2stop=0 '0 - nem sikerült leállítani / 1 - sikerült leállítani
14 'Mprg2rst=0 '0 - nem sikerült resetelni / 1 - sikerült resetelni
15 'Mprg2start=0 '0 - nem sikerült elindítani / 1 - sikerült elindítani
16 MhibaRst=0 '0 - nem sikerült resetelni a hibát / 1 - sikerült resetelni a hibát
17 MprogAllapot=0 'Állapotváltozó
18 'Helyi változók felsorolás vége--------
19 '-----------------------------------------
20 Mtimeout=0 'Az elején fontos, hogy megvárjuk a 2-es program elindulásást, mert az XRun parancs, csak is
21 'Automata módban használható
22 'While (M_Run(2)<>1 And Mtimeout<50) 'Addig fut amíg 2-es a program el nem indul,
23  'vagy le nem jár az idõzítõ
24  'Dly 0.1 'Itt fontos, hogy hiba esetén ne várjon tovább, mert akkor újból kell resetelni
25  'Mtimeout=Mtimeout+1
26 'WEnd
27 '-----------------------------------------
28 'Végtelen ciklus
29 While (M99=0)
30 '
31  If MprogAllapot=0 Then
32   'Mrun2 = M_Run(2)
33   Mmode = M_Mode
34   Mrun1 = M_Run(1)
35   Merr = M_Err
36   Mpsa1 = M_Psa(1)
37   '::::::::::::::::::::::::::::::::::::::::::::::::::
38   '::A globális változók értéket helyi változókba teszem, hogy a feltételvizsgálat közben nehogy egy
39   '::megváltozott értéket vizsgáljak
40   '::::::::::::::::::::::::::::::::::::::::::::::::::
41   If Merr=1 Then 'Van hiba? #####1
42    MprogAllapot=100 'Ha hiba van, akkor az elsõdleges feladat az 1-es program leállítása, és a hiba resetelése
43   Else '#####1
44    'If Mrun2=0 Then 'Nem fut a kommunikáció? #####2
45    ' MprogAllapot=500 'Nem fut a 2es program, akkor elindítás
46    'Else '#####2
47     If Mmode=1 Then 'Manualban van? #####3
48      If Mrun1=1 Then 'Fut az 1-es program?
49       MprogAllapot=1000 'Ha manualban fut az 1-es program, akkor leállítás
50      EndIf
51      If Mpsa1=0 Then 'Kiválasztható az 1-es program?
52       MprogAllapot=1100 'Ha manualban nem kiválasztható az 1-es program, akkor reset
53      EndIf
54     Else 'Automatában van a kulcs #####3
55      If Mrun1=0 Then 'Nem fut az 1es program?
56       MprogAllapot=1500 'Ha automatában nem fut az 1-es program, akkor 1-es stop->reset->start
57      EndIf
58     EndIf '#####3
59    'EndIf '#####2
60   EndIf '#####1
61  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
62 '
63 '
64 '
65 '
66  If MprogAllapot=100 Or M_In(10162)=1 Then 'Leállítása hiba esetén
67   GoSub *PROG1STOP
68   If Mprg1stop=0 Then
69    MprogAllapot=100 'Ha nem állt le a program, akkor próba újra
70   Else
71    MprogAllapot=200 'Sikeres leállás esetén, jöhet a hiba reset
72   EndIf
73  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
74 '
75  If MprogAllapot=200 Then
76   GoSub *HIBARST
77   If MhibaRst=0 Then
78    MprogAllapot=200 'Ha megmaradt a hiba, akkor újbol próbálkozik a resetelésre
79   Else
80    MprogAllapot=0 'Hiba sikeresen eltüntetve, vissza az állapotkereséshez
81   EndIf
82  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
83 '
84 ' If MprogAllapot=500 Then '2es program stop
85 '  GoSub *PROG2STOP
86 '  If Mprg2stop=0 Then
87 '   MprogAllapot=500 'Ha nem állt le a program, akkor próba újra
88 '  Else
89 '   MprogAllapot=600 'Sikeres leállás esetén, jöhet a program reset
90 '  EndIf
91 ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
92 '
93 ' If MprogAllapot=600 Then '2es program reset
94 '  GoSub *PROG2RST
95 '  If Mprg2rst=0 Then
96 '   MprogAllapot=600 'Ha nincs resetelve a program, akkor próba újra
97 '  Else
98 '   MprogAllapot=700 'Sikeres reset esetén, jöhet a program indítás
99 '  EndIf
100 ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
101 '
102 ' If MprogAllapot=700 Then '2es program indítás
103 '  GoSub *PROG2START
104 '  If M_Err=1 Then
105 '   MprogAllapot=0 'Ha hiba történt, akkor ugrás vissza 0-ba
106 '  Else
107 '   If Mprg2start=0 Then 'nem indult el a program?
108 '    MprogAllapot=700 'Sikertelen indítás esetén újrapróbálkozás
109 '   Else
110 '    MprogAllapot=0 'Sikeres indítás esetén, ugrás vissza 0-ba
111 '   EndIf
112 '  EndIf
113 '  MprogAllapot=0
114 ' EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
115 '
116  If MprogAllapot=1000 Then '1es program stop
117   GoSub *PROG1STOP
118   If Mprg1stop=0 Then
119    MprogAllapot=1000 'Ha nem állt le a program, akkor próba újra
120   Else
121    MprogAllapot=1100 'Sikeres leállás esetén, jöhet a program reset
122   EndIf
123  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
124 '
125  If MprogAllapot=1100 Then '1es program reset
126   GoSub *PROG1RST
127   If Mprg1rst=0 Then
128    MprogAllapot=1100 'Ha nincs resetelve a program, akkor próba újra
129   Else
130    MprogAllapot=0 'Sikeres reset esetén, ugrás vissza 0-ba
131   EndIf
132  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
133 '
134  If MprogAllapot=1500 Then '1es program stop
135   GoSub *PROG1STOP
136   If Mprg1stop=0 Then
137    MprogAllapot=1500 'Ha nem állt le a program, akkor próba újra
138   Else
139    MprogAllapot=1600 'Sikeres leállás esetén, jöhet a program reset
140   EndIf
141  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
142 '
143  If MprogAllapot=1600 Then '1es program reset
144   GoSub *PROG1RST
145   If Mprg1rst=0 Then
146    MprogAllapot=1600 'Ha nincs resetelve a program, akkor próba újra
147   Else
148    MprogAllapot=1700 'Sikeres reset esetén, jöhet a program indítás
149   EndIf
150  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
151 '
152  If MprogAllapot=1700 Then '1es program indítás
153   GoSub *PROG1START
154   If M_Err=1 Then
155    MprogAllapot=0 'Ha hiba történt, akkor ugrás vissza 0-ba
156   Else
157    If M_Mode = 1 Then 'Ha manualban van, akkor ugrás vissza 0-ba
158     MprogAllapot = 0
159    Else
160     If Mprg1start=0 Then 'nem indult el a program?
161      MprogAllapot=1700 'Sikertelen indítás esetén újrapróbálkozás
162     Else
163      MprogAllapot=0 'Sikeres indítás esetén, ugrás vissza 0-ba
164     EndIf
165    EndIf
166   EndIf
167  EndIf '~~~~~~~~~~~~~~~~~~~~~~~~
168 '
169 Dly 0.001 '1 ms-es késleltetés, hogy a ciklus ne fusson túl gyorsan, ha épp nem teljesül semmilyen feltétel
170 WEnd
171 End
172 '---------------------------------
173 'Szubrutinok következnek-----
174 '---------------------------------
175 *PROG1STOP 'Az 1es program leállítása
176   XStp 1
177   Mtimeout=0
178   While (M_Run(1)<>0 And Mtimeout<30) 'Addig fut amíg a program le nem áll
179    Dly 0.1
180    Mtimeout=Mtimeout+1
181   WEnd
182   If Mtimeout>=30 Then Mprg1stop=0 Else Mprg1stop=1 'Hiba esetén jelzés
183 Return
184 '
185 *PROG1RST 'Az 1es program resetelése
186   XRst 1
187   Mtimeout=0
188   While (M_Psa(1)=0 And Mtimeout<30) 'Addig fut amíg a program nem a selectable állapotban van
189    Dly 0.1
190    Mtimeout=Mtimeout+1
191   WEnd
192   If Mtimeout>=30 Then Mprg1rst=0 Else Mprg1rst=1 'Hiba esetén jelzés
193 Return
194 '
195 *PROG1START 'Az 1es program indítása
196   XRun 1,"1",1
197   Mtimeout=0
198   While (M_Run(1)<>1 And Mtimeout<60 And M_Err=0 And M_Mode<>1)
199    'Addig fut amíg a program el nem indul
200    Dly 0.1 'Itt fontos, hogy hiba esetén ne várjon tovább, mert akkor újból kell resetelni
201    Mtimeout=Mtimeout+1
202   WEnd
203   If (Mtimeout>=60 Or M_Err=1 Or M_Mode=1) Then Mprg1start=0 Else Mprg1start=1 'Hiba esetén jelzés
204 Return
205 '
206 '*PROG2STOP 'Az 2es program leállítása
207 '  XStp 2
208 '  Mtimeout=0
209 '  While (M_Run(2)<>0 And Mtimeout<30) 'Addig fut amíg a program le nem áll
210 '   Dly 0.1
211 '   Mtimeout=Mtimeout+1
212 '  WEnd
213 '  If Mtimeout>=30 Then Mprg2stop=0 Else Mprg2stop=1 'Hiba esetén jelzés
214 'Return
215 '
216 '*PROG2RST 'Az 2es program resetelése
217 '  XRst 2
218 '  Mtimeout=0
219 '  While (M_Psa(2)=0 And Mtimeout<30) 'Addig fut amíg a program nem a selectable állapotban van
220 '   Dly 0.1
221 '   Mtimeout=Mtimeout+1
222 '  WEnd
223 '  If Mtimeout>=30 Then Mprg2rst=0 Else Mprg2rst=1 'Hiba esetén jelzés
224 'Return
225 '
226 '*PROG2START 'Az 2es program indítása
227 '  XRun 2,"2"
228 '  Mtimeout=0
229 '  While (M_Run(2)<>1 And Mtimeout<60 And M_Err=0) 'Addig fut amíg a program el nem indul
230 '   Dly 0.1 'Itt fontos, hogy hiba esetén ne várjon tovább, mert akkor újból kell resetelni
231 '   Mtimeout=Mtimeout+1
232 '  WEnd
233 '  If (Mtimeout>=60 Or M_Err=1) Then Mprg2start=0 Else Mprg2start=1 'Hiba esetén jelzés
234 'Return
235 '
236 *HIBARST 'Ama elõjövõ rontás tisztíása az Úr neve által
237   Dly 1
238   Reset Err
239   Mtimeout=0
240   While (Mtimeout<30 And M_Err=1) 'Addig a hiba el nem tûnik vagy timeout
241    Dly 0.1
242    Mtimeout=Mtimeout+1
243   WEnd
244   If (Mtimeout>=30) Then MhibaRst=0 Else MhibaRst=1 'Hiba esetén jelzés
245 Return
246 '
