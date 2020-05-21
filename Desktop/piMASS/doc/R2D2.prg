1     RelM Rem release mechanic
2     Def Act 1,M_Err=1 GoSub *SUB1
3     '
4     M_00#=0
5     '
6    Rem -  -- ---=--==-=== XYZ ===-==--=--- --  -
7    Def Io ioNM=DWord,10160
8    Def Io ioMx=DWord,10192
9    Def Io ioMy=DWord,10224
10    Def Io ioMz=DWord,10256
11    Rem -  -- ---=--==-=== ABC ===-==--=--- --  -
12    Def Io ioMa=DWord,10288
13    Def Io ioMb=DWord,10320
14    Def Io ioMc=DWord,10352
15    Rem -  -- ---=--==-=== TIME ===-==--=--- --  -
16    Def Io ioMs=DWord,10384
17    Rem -  -- ---=--==-=== CTRL ===-==--=--- --  -
18    Def Io ioHS1=Bit,10416
19    Def Io ioHS2=Bit,10417
20    Def Io ioHS3=Bit,10418
21    Def Io ioMajre=Bit,10419
22    Def Io ioPIX=DWord,10448
23    Def Io ioCOM=DWord,10480
24    Rem -  -- ---=--==-=== mSEC ===-==--=--- --  -
25    Def Io ioMsSLV=DWord,10512
26    Def Io ioMsMST=DWord,10544
27    Def Io ioMsR2D=DWord,10576
28     Rem -  -- ---=--==-=== ENViroment ===-==--=--- --  -
29     Def Io oEspd=DWord,11120
30     Def Io oNcnt=DWord,11248
31     Def Io oEkey=DWord,11280
32     Def Io oESvo=Bit,11312
33     ioMsR2D=0
34     Rem -  -- ---=--==-=== mXmYmZeS ===-==--=--- --  -
35     Rem -  -- ---=--==-=== mAmBmCnC ===-==--=--- --  -
36     ioHS1=0
37     ioHS2=0
38     ioCOM=0
39     oEkey=M_Mode
40     mcn=0
41     Meve=1
42     Mr = 0
43     Act 1=1
44      While(Meve>0)
45        Mms = M_Timer(1)
46        Ms=Mms/1000.0
47        ioMsR2D = Mms
48        If M_Svo=1 Then
49          oESvo=1
50        Else
51          oESvo=0
52        EndIf
53        Rem oEspd=M_RSpd
54        mcn=mcn+1
55        oEkey=M_Mode
56        p1=P_Curr
57        mpx=p1.X*100.0
58        mpy=p1.Y*100.0
59        mpz=p1.Z*100.0
60        ioMx=mpx
61        ioMy=mpy
62        ioMz=mpz
63        ioMa=Deg(p1.A)*100.0
64        ioMb=Deg(p1.B)*100.0
65        ioMc=Deg(p1.C)*100.0
66        mx=ioMx/100.0
67        my=ioMy/100.0
68        mz=ioMz/100.0
69        Dly 0.125
70        WEnd
71      End
72      '
73     *SUB1
74         Act 1=0
75         If M_00#=1 Then
76          ioCOM=M_ErrNo
77          ioHS2=1 'ReadyOut=1
78          Wait ioHS2=1 'wait ConfirmIn=1
79          ioCOM=0 'Hibaküldés kinullázása
80          ioHS2=0 'ReadyOut=0
81          Wait ioHS2=0 'wait ConfirmIn=0
82          M_00#=0
83          Mr=M_ErrNo
84         EndIf
85         Act 1=1
86     Return 0
p1=(500.000,500.000,500.000,180.000,0.000,-153.170)(7,0)
