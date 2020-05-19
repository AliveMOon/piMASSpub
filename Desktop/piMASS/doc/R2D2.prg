1     RelM Rem release mechanic
2     Def Act 1,M_Err=1 GoSub *SUB1
3     '
4     M_00#=0
5     '
6     Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
7     Def Io ioHS1=Bit,10160
8     Def Io ioHS2=Bit,10161
9     Def Io ioCOM=DWord,10192
10     Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
11     Def Io oMx=DWord,10256
12     Def Io oMy=DWord,10288
13     Def Io oMz=DWord,10320
14     Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
15     Def Io oMa=DWord,10352
16     Def Io oMb=DWord,10384
17     Def Io oMc=DWord,10416
18     Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
19     Def Io oMs12=DWord,10448
20     Def Io oMs32=DWord,10480
21     Def Io oMsR2D=DWord,10512
22     Rem -  -- ---=--==-=== ENViroment ===-==--=--- --  -
23     Def Io oEspd=DWord,11120
24     Def Io oNcnt=DWord,11248
25     Def Io oEkey=DWord,11280
26     Def Io oESvo=Bit,11312
27     oMsR2D=0
28     Rem -  -- ---=--==-=== mXmYmZeS ===-==--=--- --  -
29     Rem -  -- ---=--==-=== mAmBmCnC ===-==--=--- --  -
30     ioHS1=0
31     ioHS2=0
32     ioCOM=0
33     oEkey=M_Mode
34     mcn=0
35     Meve=1
36     Mr = 0
37     Act 1=1
38      While(Meve>0)
39        Mms = M_Timer(1)
40        Ms=Mms/1000.0
41        oMsR2D = Mms
42        If M_Svo=1 Then
43          oESvo=1
44        Else
45          oESvo=0
46        EndIf
47        Rem oEspd=M_RSpd
48        mcn=mcn+1
49        oEkey=M_Mode
50        p1=P_Curr
51        mpx=p1.X*100.0
52        mpy=p1.Y*100.0
53        mpz=p1.Z*100.0
54        oMx=mpx
55        oMy=mpy
56        oMz=mpz
57        oMa=Deg(p1.A)*100.0
58        oMb=Deg(p1.B)*100.0
59        oMc=Deg(p1.C)*100.0
60        mx=oMx/100.0
61        my=oMy/100.0
62        mz=oMz/100.0
63        Dly 0.125
64        WEnd
65      End
66      '
67     *SUB1
68         Act 1=0
69         If M_00#=1 Then
70          ioCOM=M_ErrNo
71          ioHS2=1 'ReadyOut=1
72          Wait ioHS2=1 'wait ConfirmIn=1
73          ioCOM=0 'Hibaküldés kinullázása
74          ioHS2=0 'ReadyOut=0
75          Wait ioHS2=0 'wait ConfirmIn=0
76          M_00#=0
77          Mr=M_ErrNo
78         EndIf
79         Act 1=1
80     Return 0
p1=(47.140,441.720,-168.230,172.450,4.390,-173.860)(6,0)
