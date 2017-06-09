<?php
	require_once "config.php";
	require_once "password_utils.php";
	require_once "db_utils.php";
	
	error_reporting(E_ALL);
	ini_set('display_errors', '1');
	
	mysql_db_connect();
	
	$max_first_names;
	$max_last_names;
	$first_name;
	$last_name;
	
	
	
	$first_name_table = (rand(0,1) == 1)?("dutch_boy_first_names"):("dutch_girl_first_names");
			
	$sql = "select count(*) from $first_name_table";
	$result = mysql_query($sql);
	if($result && ($row = mysql_fetch_assoc($result))) {
		$max_first_names = $row['count(*)'];
	}
	
	$sql = "select count(*) from dutch_last_names";
	$result = mysql_query($sql);
	if($result && ($row = mysql_fetch_assoc($result))) {
		$max_last_names = $row['count(*)'];
	}
	
	$sql = "select name from $first_name_table where id=".rand(1,$max_first_names);
	$result = mysql_query($sql);
	if($result && ($row = mysql_fetch_assoc($result))) {
		$first_name = $row['name'];
	}
	
	$sql = "select name from dutch_last_names where id=".rand(1,$max_last_names);
	$result = mysql_query($sql);
	if($result && ($row = mysql_fetch_assoc($result))) {
		$last_name = $row['name'];
	}
	
	echo $first_name . " " . $last_name;
	
	//$sql = "insert into captains (user_id,first_name,last_name,ship_id,city_id,money) values from captains where user_id=$user_id";
?>
