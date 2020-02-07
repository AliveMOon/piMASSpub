1 Servo Off
2 Mev= 3
3 Def Io HSr1=Bit,14706
4 Def Io HSr2=Bit,14708
5 While(Mev>0)
6  Select Mev
7   Case 1
8    GoSub *HS1
9    Break
10   Case 2
11    GoSub *HS2
12   Break
13   Case 3
14    GoSub *IDLE
15   Break
16  End Select
17  Dly 0.01
18 WEnd
19 End
20 *IDLE
21   If (HSr1=1) Then
22   Mev=1
23   EndIf
24   Return
25 *HS1
26  HSr2=1
27    Mev = 3
28  Return
29 *HS2
30  Return
