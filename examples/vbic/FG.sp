VBIC Gummel Test
V1 Q1_E 0 5.0
VC Q1_C 0 0.0 
VB Q1_B 0 0.0
Q1 Q1_C Q1_B Q1_E P1
.DC V1 0.2 1.2 10m
.OPTIONS GMIN=1e-13

.control
run
plot abs(i(vc)) abs(i(vb)) ylimit 1e-15 0.1 ylog
plot abs(i(vc))/abs(i(vb)) vs abs(-i(vc)) xlimit 1e-09 0.1 xlog
.endc

.MODEL P1 PNP LEVEL=4 
+ IS=1e-16 IBEI=1e-18 IBEN=5e-15 IBCI=2e-17 IBCN=5e-15 ISP=1e-15 RCX=10
+ RCI=60 RBX=10 RBI=40 RE=2 RS=20 RBP=40 VEF=10 VER=4 IKF=2e-3 ITF=8e-2
+ XTF=20 IKR=2e-4 IKP=2e-4 CJE=1e-13 CJC=2e-14 CJEP=1e-13 CJCP=4e-13 VO=2
+ GAMM=2e-11 HRCF=2 QCO=1e-12 AVC1=2 AVC2=15 TF=10e-12 TR=100e-12 TD=2e-11 RTH=300

.END
