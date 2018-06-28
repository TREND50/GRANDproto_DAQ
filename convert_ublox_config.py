#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import math

def convert_ublox_config(in_filename_ubx):
	ubx_header = [ '0XB5', '0X62' ];
		
	fd_ubx = open(in_filename_ubx, "r")
	
	nb_ubx_frame = 0
	for line in fd_ubx:
	##Parse in ubx frame
		list_element_in_line = line.split();
		comment = list_element_in_line[0]
		ubx_frame = list_element_in_line[2:];
	##Parse in ubx frame
		
		if (comment != "MON-VER"):
		##Compute CheckSum
			ubx_frame_checksumA = 0
			ubx_frame_checksumB = 0
			for index in range(len(ubx_frame)):
				ubx_frame_checksumA += int(ubx_frame[index], 16)
				ubx_frame_checksumA &= 0xFF
				ubx_frame_checksumB += int(ubx_frame_checksumA)
				ubx_frame_checksumB &= 0xFF
		##Compute CheckSum
					
			out_ubx_frame = ubx_header + ubx_frame + [hex(ubx_frame_checksumA).upper()] + [hex(ubx_frame_checksumB).upper()]
			
		#print output
			sys.stdout.write("###"+comment+"\n")
			sys.stdout.write("==BEGIN==\n")
			sys.stdout.write("TRENDGPS\n")
			sys.stdout.write("RWb\tSCALAR\t0\n")
			sys.stdout.write("Addr\tSCALAR\t0x42\n")
			wrd = int(math.ceil(len(out_ubx_frame)/2.0))
	#		sys.stderr.write(str(len(out_ubx_frame)) + " - " + str(len(out_ubx_frame)/2.0) + " - " + str(wrd) + "\n")	
			sys.stdout.write("wrd\tSCALAR\t"+str(wrd)+"\n")
			sys.stdout.write("data\tVECTOR\t")
			for index in range(len(out_ubx_frame)):
				sys.stdout.write(out_ubx_frame[index]+" ")
			sys.stdout.write("\n")
			sys.stdout.write("==END==\n")
			sys.stdout.write("###"+comment+"\n\n")
			nb_ubx_frame += 1
		#print output
	
	fd_ubx.close()
	sys.stderr.write("File " + in_filename_ubx + " contains " + str(nb_ubx_frame) + " ubx frames\n")

if __name__=='__main__':
	if(len(sys.argv) < 2):
		sys.stderr.write("Pas de fichier à convertir en argument\n")
		exit(1)
	in_filename_ubx = sys.argv[1];
	
	convert_ublox_config(in_filename_ubx)
	
	exit(0)