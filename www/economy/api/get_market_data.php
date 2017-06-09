<?php
	require_once "db_utils.php";

	function get_market_data ($user_id) {
	
		mysql_db_connect();
		
		$city_id;
		$city_name;
		
		$sql = "select id,city_id from captains where user_id=$user_id";
		$result = mysql_query($sql);
		if($result && ($row = mysql_fetch_assoc($result))) {
			$captain_id = $row['id'];
			$city_id = $row['city_id'];
		} else {
			return array("success"=>"false", "message"=>"Cannot find captain.");
		}
		
		
		$sql = "select name,day from cities where id=$city_id";
		
		$result = mysql_query($sql);
		while($result && ($row = mysql_fetch_assoc($result))) {
			$city_day = $row['day'];
			$city_name = $row['name'];
		}
		
		$sql = "select goods.id as id,goods.name as name,prices.price as price " . 
		"from goods " . 
		"inner join prices on goods.id=prices.good_id " . 
		"inner join possessions on goods.id=possessions.good_id " . 
		"inner join citizens on citizens.id=possessions.citizen_id " .
		"and prices.city_id=citizens.city_id " .
		"where prices.city_id=$city_id and possessions.possessed>0 and not goods.name like '%coin%'" .
		"group by goods.id";
		
		$result = mysql_query($sql);
		$goods = array();
		while(($row = mysql_fetch_assoc($result))) {
			$goods[] = $row;
		}
		
		
		$sql = "select offers.id as offer_id,goods.id as good_id," . 
		"offers.city_id as city_id,goods.name as good_name,(offers.to_sell - offers.stock) as quantity_sold," . 
		"offers.stock as quantity_unsold,offers.balance as balance,offers.min_price as min_price " . 
		"from goods inner join player_offers as offers on goods.id=offers.good_id " . 
		"where offers.user_id=$user_id and offers.city_id=$city_id";
			
		$result = mysql_query($sql);
		$offers = array();
		while($result && ($row = mysql_fetch_assoc($result))) {
			$offers[] = $row;
		}
		
		$sql = "select bids.id as bid_id,goods.id as good_id," . 
		"bids.city_id as city_id,goods.name as good_name,(bids.to_buy - bids.stock) as quantity_unbought," . 
		"bids.stock as quantity_bought,bids.balance as balance,bids.max_price as max_price " . 
		"from goods inner join player_bids as bids on goods.id=bids.good_id " . 
		"where bids.user_id=$user_id and bids.city_id=$city_id";
		
		$result = mysql_query($sql);
		$bids = array();
		while($result && ($row = mysql_fetch_assoc($result))) {
			$bids[] = $row;
		}
		
		return array("success"=>"true","city_name"=>$city_name,"city_day"=>$city_day,
		"goods"=>$goods,"offers"=>$offers,"bids"=>$bids);
	}
?>