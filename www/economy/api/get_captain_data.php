<?php
	require_once "db_utils.php";

	function get_captain_data ($user_id) {
	
		mysql_db_connect();
		
		$first_name;
		$last_name;
		$money;
		$city_id;
		
		$sql = "select first_name,last_name,money from captains where user_id=$user_id";
		
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$first_name = $row['first_name'];
			$last_name = $row['last_name'];
			$money = $row['money'];
		} else {
			
			$max_first_names;
			$max_last_names;
			$max_cities;
			$city_id = 1;
			
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
			
			
			$sql = "select count(*) from cities";
			$result = mysql_query($sql);
			if($result && ($row = mysql_fetch_assoc($result))) {
				$max_cities = $row['count(*)'];
			}
			
			$sql = "select id from cities where id=".rand(1,$max_cities);
			$result = mysql_query($sql);
			if($result && ($row = mysql_fetch_assoc($result))) {
				$city_id = $row['id'];
			}
			
			$money = 10000;
			
			$sql = "insert into captains (user_id,first_name,last_name,ship_id,city_id,money) values('$user_id','$first_name','$last_name',1,'$city_id','$money')";
			$result = mysql_query($sql);
		}
		
		
		$sql = "select goods.name as name,goods.id as good_id,inventories.count as quantity,prices.price " . 
		"from captains inner join inventories on captains.id=inventories.captain_id " . 
		"inner join goods on inventories.good_id=goods.id " . 
		"inner join prices on prices.good_id=goods.id and prices.city_id=captains.city_id " . 
		"where captains.user_id=$user_id and inventories.count>0";
		
		$result = mysql_query($sql);
		$inventory = array();
		while($result && ($row = mysql_fetch_assoc($result))) {
			$inventory[] = $row;
		}
		
		
		return array("success"=>"true","first_name"=>$first_name,"last_name"=>$last_name,"money"=>intval($money),"inventory"=>$inventory);
	}
?>