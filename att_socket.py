#30/05/2017
#VELLEYEN Stephane

############################# Config socket
import socket
import time
import sys
ip="192.168.1.11"  #Attenuator IP adress [set by Stephane on LPNWS5222 with LanTronix DeviceInstaller]
port=10001  # Attenuator Port [set by Vincent]

instr=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
instr.connect((ip,port))

############################# Valeur par defaut & demande d'id

ch=1
att=10
instr.send('*IDN?\r')
name=instr.recv(2048)
print name
chancmd = "CHAN {0}\r".format(ch)
instr.send(chancmd) #set ask?id

#############################
choix=sys.argv[1]
att=sys.argv[2]
att=int(att)

if choix=="1": #Mode Attenuation fixe

	
	print "Mode Attenuation fixe"
	attcmd = "ATTN {0}\r".format(att)
	print "Setting:",attcmd
	instr.send(attcmd) #set attenuation


elif choix=="2": #Mode Incrementation

	print "Mode Incrementation"
	attcmd = "ATTN {0}\r".format(att)
	print "Setting:",attcmd
	instr.send(attcmd) #set attenuation
	 
	time.sleep(1)


	i=att
	while i < 80:

		incrcmd = 'INCR\r'
		instr.send(incrcmd) #set incrementation
		time.sleep(1)
		i=i+1
else:
	print "Erreur"


