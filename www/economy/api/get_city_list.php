<?php
	require_once "db_utils.php";

	function get_city_list ($user_id) {
	
		mysql_db_connect();
		
		$captain_id;
		$city_id;
		
		$sql = "select id,city_id from captains where user_id=$user_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$captain_id = $row['id'];
			$city_id = $row['city_id'];
		} else {
			return array("success"=>"false", "message"=>"Cannot find captain.");
		}
		
		$sql = "select id as city_id,name as city_name,mountain_score,plains_score,cultural_biome,day from cities where not id=$city_id";
		
		$cities = array();
		
		$result = mysql_query($sql);
		while($result && ($row = mysql_fetch_assoc($result))) {
			$cities[] = $row;
		}
		
		return array("success"=>"true","cities"=>$cities);
	}
?>