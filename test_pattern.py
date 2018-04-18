
#/06/2017
#VELLEYEN Stephane
# 25/09/2017 OMH

import os
import time
import sys
import math
import numpy as np
import pylab as pl

boardid=sys.argv[1]
cas=sys.argv[2]
DATADIR=sys.argv[3]
siz = 2*90 # event size
off = 1  # Skip 1st sample for each channel
sizc = 4*(siz-(off+1))

def read():

  file = DATADIR+'/last_run.txt'
  rid = str(int(np.loadtxt(file)))
  fichier=DATADIR+'/P'+str(rid)+'_b'+boardid+'.data.txt' 
  print 'Reading pattern data file',fichier
  file = open(fichier,'r')
  evts = file.read().split('-----------------') # Definition d'un evenements
  nevts = len(evts)-1  # take last event
  file.close()


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
	  ievts2 = [int(i) for i in evts2[0:np.size(evts2)-1]]   #Skip first samples
	  index1st = [range(off+1),range(siz,siz+off+1),range(2*siz,2*siz+off+1),range(3*siz,3*siz+off+1)]
	  ievts2 = np.delete(ievts2,index1st)
	  test1 = np.mean(ievts2) 
	  print 'Mean value for test=',test1
       
          if np.size(ievts2) != sizc:
	      print '##########Error: event size !=',sizc,' ##########'
	      sys.exit(-1)
  
  return ievts2 #renvoie les donnees 	


def main():		
  tab=read()

  if cas=="0":
  	  a=0*np.ones(sizc) #On declare un tableau de 720 valeurs
  elif cas=="1":
  	  a=4095*np.ones(sizc) #On declare un tableau de 720 valeur
  elif cas=="2":
  	  a=1365*np.ones(sizc) #On declare un tableau de 720 valeur
  	  m = range(sizc)
  	  m2 = (np.asarray(m)%2 != 0)  # Start with 2730
  	  a[m2] = 2730 
  elif cas=="3":
  	  a=819*np.ones(sizc) #On declare un tableau de 720 valeur

  elif cas=="4":
  	  a=int(4032)*np.ones(sizc) #On declare un tableau de 720 valeur
  print 'Value expected=',np.mean(a)

  
  if np.array_equal(tab,a):  #On teste la moyenne et le nombre donnees
    print "Pattern ", cas, ": success."
    return 1 #Renvoie la valeur de la variable cas

  else:
    print "Pattern ", cas, ": failed."
    return 0  #renvoie 255

if __name__ == '__main__':
   main()


