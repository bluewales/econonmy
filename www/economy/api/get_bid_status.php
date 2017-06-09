<?php
	require_once "db_utils.php";
	
	function get_bid_status($user_id, $bid_id) {
	
		mysql_db_connect();
		
		$sql = "select bids.id as bid_id,goods.id as good_id," . 
		"bids.city_id as city_id,goods.name as good_name,(bids.to_buy - bids.stock) as quantity_unbought," . 
		"bids.stock as quantity_bought,bids.balance as balance,bids.max_price as max_price " . 
		"from goods inner join player_bids as bids on goods.id=bids.good_id " . 
		"where bids.user_id=$user_id and bids.id=$bid_id";
		$result = mysql_query($sql);
		$bid;
		if($result && ($row = mysql_fetch_assoc($result))) {
			$bid = $row;
		} else {
			return array("success"=>"false", "message"=>"Bid does not exist.");
		}
		
		return array("success"=>"true","bid"=>$bid);
	}
?>