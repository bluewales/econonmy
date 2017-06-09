<?php
	require_once "config.php";
	
	$mysql_db_connection = false;
	
	function mysql_db_connect() {
		global $mysql_db_connection, $db_database, $db_address, $db_user, $db_password;
		if($mysql_db_connection)
			return $mysql_db_connection;

		$mysql_db_connection = mysql_connect($db_address,$db_user,$db_password);
		if (!$mysql_db_connection) die('Could not connect: ' . mysql_error());
		mysql_select_db($db_database, $mysql_db_connection);
		
		return $mysql_db_connection;
	}
	
	function mysql_db_username_exists($username) {
		global $db_table_prefix;
		$con = mysql_db_connect();
		$sql = "select * from " . $db_table_prefix . "_users where username='" . $username . "'";
		$result = mysql_query($sql);
		if($result) {
			$user_data = mysql_fetch_assoc($result);
		} else {
			return false;
		}
		if(!empty($user_data)) {
			return $user_data['id'];
		}
		return false;
	}
	
	function mysql_db_save_login($username, $hashed_password, $salt) {
		global $db_table_prefix;
		$con = mysql_db_connect();
		
		// Create user entry
		$sql = "insert into " . $db_table_prefix . "_users (username,password_hash,password_salt) " .
			"values('" . $username . "','" . $hashed_password . "', '" . $salt . "')";
		if(!mysql_query($sql)) {
			return false;
		}
		
		$sql="select * from " . $db_table_prefix . "_users where username='" . $username . "'";
		$result = mysql_query($sql);
		if(!$result) {
			return false;
		}
		
		$user_data = mysql_fetch_assoc($result);
		return $user_data['id'];
	}
	
	function mysql_db_valid_login($username, $password) {
		global $db_table_prefix;
		$con = mysql_db_connect();
		
		$sql="select * from " . $db_table_prefix . "_users where username='" . $username . "'";
		$result = mysql_query($sql);
		if(!$result) {
			return false;
		}
		$user_data = mysql_fetch_assoc($result);
		if(!$user_data || empty($user_data )) {
			return false;
		}
		$salt = $user_data['password_salt'];
		if(!$salt)
		{
			return false;
		}
		$hashedPassword = hash_password($password,$salt);
		if($hashedPassword != $user_data['password_hash']) {
			return false;
		}
		return $user_data['id'];
	}
	
	function mysql_db_save_session_token($user_id, $session_token) {
		global $db_table_prefix;
		$con = mysql_db_connect();
		
		$sql = "update " . $db_table_prefix . "_users set session_token='$session_token' where id='$user_id'";
		$result = mysql_query($sql);
		return (mysql_affected_rows() == 1);
	}
	
	function mysql_db_user_id_from_session_token($session_token) {
		global $db_table_prefix;
		$con = mysql_db_connect();
		
		$sql = "select * from " . $db_table_prefix . "_users where session_token='" . $session_token . "'";
		$user_data;
		$result = mysql_query($sql);
		if($result) {
			$user_data = mysql_fetch_assoc($result);
		} else {
			return false;
		}
		if(!empty($user_data)) {
			return $user_data['id'];
		}
		return false;
	}

	function mysql_db_create_user_table() {
		global $db_table_prefix,  $a_username_max_length, $h_salt_length, $h_token_length;
		$con = mysql_db_connect();

		$sql = "create table " . $db_table_prefix . "_users (" .
			"id int auto_increment," . 
			"username varchar(" . $a_username_max_length . ")," .
			"password_hash varchar(172)," .
			"password_salt varchar(" . $h_salt_length . ")," .
			"session_token varchar(" . $h_token_length . ")," .
			"created TIMESTAMP DEFAULT CURRENT_TIMESTAMP," .
			"primary key(id))";
		return mysql_query($sql);
	}
?>