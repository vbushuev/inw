<?php
$dword = 123;
$bits = [];
for($i=0; $i<32; $i++){
	$bits[$i]=$dword & 1; 
	$dword>>=1;
}
print_r($bits);
?>