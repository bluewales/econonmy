<?php
	// if these setting are changed, the users table should be dropped and allowed to be rebuilt
	//  (this means you shouldn't change these if there exist users who cannot be lost)

	// database settings
	$db_address = "localhost";
	$db_database = "economy";
	$db_user = "econouser";
	$db_password = "econopass";
	$db_table_prefix = "economy";
	
	// account settings
	$a_username_max_length = 30;  // must be greater than 0
	$a_username_min_length = 0;
	$a_password_max_length = 0;  // 0 indicates no restriction
	$a_password_min_length = 0;
	$a_skeleton_key = "";  // if the the user sets their auth_token to this value, they do not need to authenticate to do many things, set as empty string to disable
	
	// hash settings
	$h_salt_length = 128;  // must be greater than 0
	$h_token_length = 32; // must be greater than 0
	
	$g_time_zone = "America/Denver";
	
	
	// init -- DO NOT TOUCH
	date_default_timezone_set ($g_time_zone);
?>