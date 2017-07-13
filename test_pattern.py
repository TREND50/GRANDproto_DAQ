
#/06/2017
#VELLEYEN Stephane

import os
import time
import sys
import math
import numpy as np

boardid=sys.argv[1]
cas=sys.argv[2]
DATADIR=sys.argv[3]


def read():

  #DATADIR='/home-local/GRANDproto/GRANDproto/data/'
  #DATADIR='/home/trend/Documents/testbench/'
  print DATADIR
  file=open(DATADIR+'/last_run.txt','r') 
  data=file.read(1)  	#On recupere le numero du dernier fichier  de data modifie
  print 'RunID=',data
  file.close()


  fichier=DATADIR+'/P'+str(data)+'_b'+boardid+'.data' 
  # print fichier

  file = open(fichier,'r')

  evts = file.read().split('-----------------') # Definition d'un evenements

  nevts = len(evts)-1


  j = 0;  # Index of array filling (because date & data are "append")

  for i in range(1,nevts+1):  
   	if float(i)/100 == int(i/100):
	   print 'Event',i,'/',nevts

   	evt = evts[i]
   	evtsplit = evt.split('\n')
   	if np.size(evtsplit)>8:   # Event is of normal size

   	  # Data
   	  raw=evtsplit[9:][:]  #raw data
          evts2 = raw[0].split(' ') # Cut raw data list into samples
          ievts2 = [int(i) for i in evts2[0:np.size(evts2)-1]] 
          test1 = np.mean(ievts2) 

	  if np.size(ievts2) != 720:
	      print "##########Erreur##########"
	      sys.exit(-1)
  
  return ievts2 #renvoie les donnees 	



def affiche():

#  print 'Number of events:',nevts
#  print "Moyenne:",test1 
#  print "Nombre de donnees:",np.size(ievts2)
  print "sucess"

		

cmd = './pattern.sh ' + boardid + ' ' +cas
print cmd
os.system(cmd)   #On lance le test pattern

if cas=="0":

	tab=read()

  	a=0*np.ones(720) #On declare un tableau de 720 valeur	

	if np.array_equal(tab,a):  #On test la moyenne et le nombre donnees
		affiche()
		sys.exit(int(sys.argv[1])) #Renvoie la valeur de la variable cas 

	else:	
		sys.exit(-1)  #renvoie 255

elif cas=="1":
  	
	tab=read()

	b=np.ones(720) #On declare un tableau de 720 valeur
	
	if np.array_equal(tab,b): #On test la moyenne et le nombre donnees
		affiche()
		sys.exit(int(sys.argv[1])) #Renvoie la valeur de la variable cas

	else:	
		sys.exit(-1)  #renvoie 255


elif cas=="2":

	tab=read()

  	c=4032*np.ones(720) #On declare un tableau de 720 valeur
	
	if np.array_equal(tab,c):  #On test la moyenne et le nombre donnees
		affiche()
		sys.exit(int(sys.argv[1])) #Renvoie la valeur de la variable cas

	else:	
		sys.exit(-1)  #renvoie 255

elif cas=="3":

	tab=read()

  	d=819*np.ones(720) #On declare un tableau de 720 valeur


	if np.array_equal(tab,d): #On test la moyenne et le nombre donnees
		affiche()
		sys.exit(int(sys.argv[1])) #Renvoie la valeur de la variable cas
	else:	
		sys.exit(-1)  #renvoie 255


elif cas=="4":

	tab=read()

  	e=2730*np.ones(720) #On declare un tableau de 720 valeur

  	m =range(720)	
  	m2 = (m%2 != 0)  # Start with 2730
  	e[m2] = 1365

	if np.array_equal(tab,e):
		affiche()
		sys.exit(int(sys.argv[1])) #Renvoie la valeur de la variable cas
	else:	
		sys.exit(-1)  #renvoie 255

else:
	print "##########Erreur_pattern##########"
	sys.exit(-1)



file.close()


		
