#!/bin/bash
Number=24000
while $Number<25000
do 
	kill $Number
	$Number=$Number+1;	
done
