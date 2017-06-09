<?php
	require_once "db_utils.php";
	
	function submit_offer($user_id, $good_id, $num_to_sell, $min_price) {
	
		mysql_db_connect();
		
		if($num_to_sell <= 0) {
			return array("success"=>"false", "message"=>"You can only sell a positive amount.");
		}
		
		if($min_price <= 1) {
			$min_price = 1;
		}
		
		$captain_id;
		$city_id;
		$num_good_owned;
		
		$sql = "select id,city_id from captains where user_id=$user_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$captain_id = $row['id'];
			$city_id = $row['city_id'];
		} else {
			return array("success"=>"false", "message"=>"Cannot find captain.");
		}
		
		$sql = "select * from player_offers where user_id=$user_id and good_id=$good_id and city_id=$city_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			return array("success"=>"false", "message"=>"Previous offer must be claimed first.", "offer_id"=>$row['id']);
		}
		
		$sql = "select count from inventories where captain_id=$captain_id and good_id=$good_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$num_good_owned = $row['count'];
		} else {
			return array("success"=>"false", "message"=>"Insufficient stock.  You don't own any of that.","captain_id"=>$captain_id,"good_id"=>$good_id);
		}
		if($num_to_sell > $num_good_owned) {
			return array("success"=>"false", "message"=>"Insufficient stock.  You only own $num_good_owned, so you can't sell $num_to_sell.");
		}
		
		$sql = "insert into player_offers (user_id,good_id,city_id,to_sell,stock,min_price,balance) " . 
		"values($user_id,$good_id,$city_id,$num_to_sell,$num_to_sell,$min_price,0)";
		mysql_query($sql);
		$sql = "update inventories set count=count-$num_to_sell where good_id=$good_id and captain_id=$captain_id";
		mysql_query($sql);
		
		$sql = "select offers.id as offer_id,goods.id as good_id," . 
		"offers.city_id as city_id,goods.name as good_name,(offers.to_sell - offers.stock) as quantity_sold," . 
		"offers.to_sell as quantity_unsold,offers.balance as balance,offers.min_price as min_price " . 
		"from goods inner join player_offers as offers on goods.id=offers.good_id " . 
		"where offers.user_id=$user_id and offers.city_id=$city_id and offers.good_id=$good_id";
		$result = mysql_query($sql);
		$offer;
		if($result && ($row = mysql_fetch_assoc($result))) {
			$offer = $row;
		}
		
		return array("success"=>"true","offer"=>$offer);
	}
?>