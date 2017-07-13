#30/06/2017
#VELLEYEN Stephane
#############################

import os,sys 
import vxi11


instr =  vxi11.Instrument("192.168.1.10")
print(instr.ask("*IDN?"))


choix=sys.argv[1]

if choix =="1": #Choix des parametres 

	ch=sys.argv[2]
	func=sys.argv[3]
	freq=sys.argv[4]
	vcc=sys.argv[5]

	vcc=int(vcc)
	vmax=vpp/2
	vmin=vpp/-2

	if vcc>300: 
		print "Erreur vcc>300mv"
		vmax=150
		vmin=-150
		print "Defaut: ",vmax
		print "Defaut: ",vmin		
		
	else:
		print ""

	voffset=0
 	phase=0
	out="ON"
	symm=100
	width=500
	lead=10
	trail=10
	delay=20
	dcyc=50
	out="ON"

	

	if func=="RAMP": 

		#set ramp symmetry
		symmcmd = "SOUR{0}:FUNC:RAMP:SYMM {1}".format(ch,symm)
		print "Setting ramp symmetry:",symmcmd
		instr.write(symmcmd)

	elif func=="PULS": 
		

		#set pulse width
		widthcmd = "SOUR{0}:PULS:WIDT {1}ns".format(ch,width)
		print "Setting pulse width:",widthcmd
		instr.write(widthcmd)

		#set edges
		leadcmd = "SOUR{0}:PULS:TRAN:LEAD {1}ns".format(ch,lead)
		print "Setting pulse leading:",leadcmd
		instr.write(leadcmd)

		trailcmd = "SOUR{0}:PULS:TRAN:TRA {1}ns".format(ch,trail)
		print "Setting pulse trailing:",trailcmd
		instr.write(trailcmd)

		#set pulse delay
		delaycmd = "SOUR{0}:PULS:DEL {1}ms".format(ch,delay)
		print "Setting pulse delay:",delaycmd
		instr.write(delaycmd)


		#set pulse DCYC
		dcyccmd = "SOUR{0}:PULS:DCYC {1}".format(ch,dcyc)
		print "Setting pulse delay:",dcyccmd
		instr.write(dcyccmd)



	#set function
	funccmd = "SOUR{0}:FUNC {1}".format(ch,func)
	print "Setting function:",funccmd
	instr.write(funccmd)

	#Set frequency
	freqcmd = "SOUR{0}:FREQ:FIX {1}".format(ch,freq)
	print "Setting frequency:",freqcmd
	instr.write(freqcmd)

	#set high level
	vmaxcmd = "SOUR{0}:VOLTAGE:HIGH {1}mV".format(ch,vmax)
	print "Setting HIGHT Voltage:",vmaxcmd
	instr.write(vmaxcmd)

	#set low level
	vmincmd = "SOUR{0}:VOLTAGE:LOW {1}mV".format(ch,vmin)
	print "Setting Low Voltage:",vmincmd
	instr.write(vmincmd)

	#set offset
	voffcmd = "SOUR{0}:VOLTAGE:OFFS {1}V".format(ch,voffset)
	print "Setting offset Voltage:",voffcmd
	instr.write(voffcmd)

	#set phase
	phasecmd = "SOUR{0}:PHAS {1}DEG".format(ch,phase)
	print "Setting phase:",phasecmd
	instr.write(phasecmd)

	#set OUTPUT ON
	outcmd = "OUTP{0} {1}".format(ch,out)
	print "Setting outout:",outcmd
	instr.write(outcmd)

						
elif choix=="2":# Sinus 66MHz
	ch=2
	func="SIN"
	freq=66  #MHz
	voffset=0 #V
	vmax=50   #mV
	vmin=-50  #mV
	phase=0   #DEG
	out="ON"


	vcc=vmax+abs(vmin)
	if vcc>300: 
		print "Erreur vcc>300mv"
		vmax=150   #mV
		vmin=-150  #mV
		print "Defaut: ",vmax
		print "Defaut: ",vmin
	else:
		print ""

	#set function
	funccmd = "SOUR{0}:FUNC {1}".format(ch,func)
	print "Setting function:",funccmd
	instr.write(funccmd)

	#Set frequency
	freqcmd = "SOUR{0}:FREQ:FIX {1}MHz".format(ch,freq)
	print "Setting frequency(MHz):",freqcmd
	instr.write(freqcmd)

	#set high level
	vmaxcmd = "SOUR{0}:VOLTAGE:IMM:HIGH {1}mV".format(ch,vmax)
	print "Setting HIGHT Voltage(mV):",vmaxcmd
	instr.write(vmaxcmd)

	#set low level
	vmincmd = "SOUR{0}:VOLTAGE:IMM:LOW {1}mV".format(ch,vmin)
	print "Setting Low Voltage(mV):",vmincmd
	instr.write(vmincmd)



	#set offset
	voffcmd = "SOUR{0}:VOLTAGE:OFFS {1}V".format(ch,voffset)
	print "Setting offset Voltage(V):",voffcmd
	instr.write(voffcmd)

	#set phase
	phasecmd = "SOUR{0}:PHAS {1}DEG".format(ch,phase)
	print "Setting phase:",phasecmd
	instr.write(phasecmd)

	#set OUTPUT ON
	outcmd = "OUTP{0} {1}".format(ch,out)
	print "Setting outout:",outcmd
	instr.write(outcmd)


elif choix =="3":# Square
	ch=2
	func="SQU"
	freq=100  #Hz
	voffset=0 #V
	vmax=50	  #mV
	vmin=-50  #mV
	phase=0   #DEG
	out="ON"

	vcc=vmax+abs(vmin)
	if vcc>300: 
		print "Erreur vcc>300mv"
		vmax=150   #mV
		vmin=-150  #mV
		print "Defaut: ",vmax
		print "Defaut: ",vmin
	else:
		print ""

	#set function
	funccmd = "SOUR{0}:FUNC {1}".format(ch,func)
	print "Setting function:",funccmd
	instr.write(funccmd)

	#Set frequency
	freqcmd = "SOUR{0}:FREQ:FIX {1}Hz".format(ch,freq)
	print "Setting frequency:",freqcmd
	instr.write(freqcmd)

	#set high level
	vmaxcmd = "SOUR{0}:VOLTAGE:HIGH {1}mV".format(ch,vmax)
	print "Setting HIGHT Voltage:",vmaxcmd
	instr.write(vmaxcmd)

	#set low level
	vmincmd = "SOUR{0}:VOLTAGE:LOW {1}mV".format(ch,vmin)
	print "Setting Low Voltage:",vmincmd
	instr.write(vmincmd)

	#set offset
	voffcmd = "SOUR{0}:VOLTAGE:OFFS {1}V".format(ch,voffset)
	print "Setting offset Voltage:",voffcmd
	instr.write(voffcmd)

	#set phase
	phasecmd = "SOUR{0}:PHAS {1}DEG".format(ch,phase)
	print "Setting phase:",phasecmd
	instr.write(phasecmd)

	#set OUTPUT ON
	outcmd = "OUTP{0} {1}".format(ch,out)
	print "Setting outout:",outcmd
	instr.write(outcmd)


else:# Defaut
	ch=2
	func="SIN"
	freq=66   #MHz
	vmax =50  #mV
	vmin =-50 #mV
	voffset=0 #V
	phase=0	  #DEG
	symm=100
	width=500
	lead=10
	trail=10
	delay=20
	dcyc=50
	out="ON"

	if func=="RAMP": 

		#set ramp symmetry
		symmcmd = "SOUR{0}:FUNC:RAMP:SYMM {1}".format(ch,symm)
		print "Setting ramp symmetry:",symmcmd
		instr.write(symmcmd)

	elif func=="PULS": 
		

		#set pulse width
		widthcmd = "SOUR{0}:PULS:WIDT {1}ns".format(ch,width)
		print "Setting pulse width:",widthcmd
		instr.write(widthcmd)

		#set edges
		leadcmd = "SOUR{0}:PULS:TRAN:LEAD {1}ns".format(ch,lead)
		print "Setting pulse leading:",leadcmd
		instr.write(leadcmd)

		trailcmd = "SOUR{0}:PULS:TRAN:TRA {1}ns".format(ch,trail)
		print "Setting pulse trailing:",trailcmd
		instr.write(trailcmd)

		#set pulse delay
		delaycmd = "SOUR{0}:PULS:DEL {1}ms".format(ch,delay)
		print "Setting pulse delay:",delaycmd
		instr.write(delaycmd)


		#set pulse DCYC
		dcyccmd = "SOUR{0}:PULS:DCYC {1}".format(ch,dcyc)
		print "Setting pulse delay:",dcyccmd
		instr.write(dcyccmd)

	
	vcc=vmax+abs(vmin)
	if vcc>300: 
		print "Erreur vcc>300mv"
		vmax=150   #mV
		vmin=-150  #mV
		print "Defaut: ",vmax
		print "Defaut: ",vmin
	else:
		print ""

	#set function
	funccmd = "SOUR{0}:FUNC {1}".format(ch,func)
	print "Setting function:",funccmd
	instr.write(funccmd)

	#Set frequency
	freqcmd = "SOUR{0}:FREQ:FIX {1}MHz".format(ch,freq)
	print "Setting frequency:",freqcmd
	instr.write(freqcmd)

	#set high level
	vmaxcmd = "SOUR{0}:VOLTAGE:HIGH {1}mV".format(ch,vmax)
	print "Setting HIGHT Voltage:",vmaxcmd
	instr.write(vmaxcmd)

	#set low level
	vmincmd = "SOUR{0}:VOLTAGE:LOW {1}mV".format(ch,vmin)
	print "Setting Low Voltage:",vmincmd
	instr.write(vmincmd)

	#set offset
	voffcmd = "SOUR{0}:VOLTAGE:OFFS {1}V".format(ch,voffset)
	print "Setting offset Voltage:",voffcmd
	instr.write(voffcmd)

	#set phase
	phasecmd = "SOUR{0}:PHAS {1}DEG".format(ch,phase)
	print "Setting phase:",phasecmd
	instr.write(phasecmd)

	#set OUTPUT ON
	outcmd = "OUTP{0} {1}".format(ch,out)
	print "Setting outout:",outcmd
	instr.write(outcmd)

