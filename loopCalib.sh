for i in {1..5000}; 
	do 
	echo $i
	#./send_msg 192.168.1.18 1234 norm.cfg; 
	./send_msg 192.168.1.18 1234 calib.cfg; 
	#./send_msg 192.168.1.18 1234 pattern.cfg; 
	
done
