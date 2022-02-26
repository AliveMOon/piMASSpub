1 Servo Off
2 Mev= 3
3  Def Io HSr1=Bit,14640
4 Def Io oPOSx=DWord,12720
5 Def Io oPOSy=DWord,12752
6 Def Io oPOSz=DWord,12784
7 Def Io oDIRa=DWord,12976
8 Def Io oDIRb=DWord,13008
9 Def Io odirc=DWord,13040
10  Def Io HSr2=Bit,14672
11 While(Mev>0)
12  Select Mev
13   Case 1
14    GoSub *HS1
15    Break
16   Case 2
17    GoSub *HS2
18   Break
19   Case 3
20    GoSub *IDLE
21   Break
22  End Select
23  Dly 0.001
24 WEnd
25 End
26 *IDLE
27   If (HSr1=1) Then
28    Mev=1
29   Else
30     HSr1=0
31   EndIf
32   Return
33 *HS1
34   Mev = 3
35 p1=P_Curr
36 oPOSx=p1.X*100.0
37 oPOSy=p1.Y*100.0
38 oPOSz=p1.Z*100.0
39 oDIRa=p1.A*100.0
40 oDIRb=p1.B*100.0
41 odirc=p1.C*100.0
42   HSr1=1
43   Return
44 *HS2
45   Return
p1=(+508.18,+23.12,+734.15,+180.00,+0.00,+90.00)(7,0)
