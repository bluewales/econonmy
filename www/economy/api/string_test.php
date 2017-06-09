<?php
	require_once "config.php";
	require_once "password_utils.php";
	require_once "db_utils.php";
	
	error_reporting(E_ALL);
	ini_set('display_errors', '1');
	
	$length = 128;
	
	$strong = false;
	$string = openssl_random_pseudo_bytes ( $length, $strong);
	
	echo "length = " . strlen($string) . "\n";
	
	$string = base64_encode($string);
	
	echo $string . "\n";
	echo "length = " . strlen($string) . "\n";
	
	$string = substr($string,0,$length);
	
	echo $string . "\n";
	echo "length = " . strlen($string) . "\n";
	
	echo $strong;
	
?>
