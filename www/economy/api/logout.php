<?php
	function logout ($user_id) {
	
		global $h_token_length;
		
		echo "logging out";
		global $db_address, $db_user, $db_password, $db_database, $db_table_prefix;
	
		$auth_token = randomString($h_token_length+5);
		
		if (!mysql_db_save_session_token($user_id, $auth_token)) {
			return array("message" => "Not logged in.", "success" => "false", "error_code" => 104);
		}
		
		return array("success" => "true", "logged_in"=>"false");
	}
?>