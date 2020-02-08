1   Servo Off
2   Meve= 3
3   Def Io HSr1=Bit,14640
4   Def Io oPOSx=DWord,12720
5   Def Io oPOSy=DWord,12752
6   Def Io oPOSz=DWord,12784
7   Def Io oDIRa=DWord,12976
8   Def Io oDIRb=DWord,13008
9   Def Io oDIRc=DWord,13040
10  Def Io oL1=DWord,13872
11  Def Io oL2=DWord,13904
12  Def Io oL3=DWord,13936
13  Def Io oL4=DWord,14000
14  Def Io oL5=DWord,14032
15  Def Io oL6=DWord,14064
16  Def Io HSr2=Bit,14672
17  While(Meve>0)
18   Select Meve
19    Case 1
20     GoSub *HS1
21     Break
22    Case 2
23     GoSub *HS2
24     Break
25    Case 3
26     GoSub *IDLE
27     Break
28   End Select
29   Dly 0.001
30  WEnd
31  End
32  *IDLE
33    If (HSr1=1) Then
34     Meve=1
35    Else
36     HSr1=0
37	EndIf
38    If (HSr2=1) Then
39     Meve=2
40    Else
41     HSr2=0
42	EndIf
43    Return
44  *HS1
45    GoSub *ACT1
46    Meve = 3
47    HSr1=1
48    Return
49  *HS2
50  GoSub *ACT1
51    Meve = 3
52    HSr2=1
53    Return
54  *ACT1
55    p1=P_Curr
56    oPOSx=p1.X*100.0
57    oPOSy=p1.Y*100.0
58    oPOSz=p1.Z*100.0
59    oDIRa=Deg(p1.A)*100.0
60    oDIRb=Deg(p1.B)*100.0
61    oDIRc=Deg(p1.C)*100.0
62    oL1 = Deg(p1.l1)*100.0
63    oL2 = Deg(p1.l2)*100.0
64    Return
p1=(+508.18,+23.12,+734.15,+180.00,+0.00,+90.00)(7,0)
