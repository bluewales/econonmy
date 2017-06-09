<?php
	require_once "db_utils.php";

	function travel ($user_id,$city_id) {
	
		mysql_db_connect();
		
		$captain_id;
		$city_id;
		
		
		
		$sql = "update captains set city_id=$city_id where user_id=$user_id";
		$result = mysql_query($sql);
		
		return array("success"=>"true");
	}
?>