<?php
	require_once "db_utils.php";

	function createuser ($username, $password1, $password2) {
	
		// Verify username
		global $a_username_min_length, $a_username_max_length, $a_password_min_length, $a_password_max_length,
				$db_address, $db_user, $db_password, $db_database, $db_table_prefix, $h_salt_length, $h_token_length;
				
		//$password_strength = file_get_contents("http://thedavidsons.co/test/password_checker.php?password=".urlencode ($password1)."&q=".rand());
		 
		if (strlen($username) < $a_username_min_length) {
			return array("message" =>  "Username must be at least " . $a_username_min_length . " characters.",
						"success" => "false", "error_code" => 114);
		}
		if ($a_username_max_length > 0 && strlen($username) > $a_username_max_length) {
			return array("message" => "Username cannot be longer than " . $a_username_max_length . " characters.",
						"success" => "false", "error_code" => 115);
		}
		
		// Verify password
		if ($password1 != $password2) {
			return array("success" => "false","message" => "Passwords do not match.");
		}
		if (strlen($password1) < $a_password_min_length) {
			return array("success" => "false","message" => "Password must be at lest " . $a_password_min_length . " characters.");
		}
		if ($a_password_max_length > 0 && strlen($password1) > $a_password_max_length) {
			return array("message" => "Password cannot be longer than " . $a_password_max_length . " characters.");
		}
		
		if (mysql_db_username_exists($username)) {
			return array("message" => "The username '".$username."' is already taken");
		}
		
		// Everything has been checked out, now we'll create the account
		
		// Generate password salt and hash
		$salt = randomString($h_salt_length);
		$hashed_password = hash_password($password1,$salt);
		
		$user_id = mysql_db_save_login($username, $hashed_password, $salt);
		if(!$user_id) {
			return array("message" =>  "Database error",
						"success" => "false");
		}
		
		// Create new login
		$auth_token = randomString($h_token_length);
		mysql_db_save_session_token($user_id, $auth_token);
		
		setcookie ( "auth_token", $auth_token );
		return array("success"=>"true","auth_token"=>$auth_token,"logged_in"=>"true");
	}
?>