<?php
	require_once "db_utils.php";
	
	function get_offer_status($user_id, $offer_id) {
	
		mysql_db_connect();
		
		$sql = "select offers.id as offer_id,goods.id as good_id," . 
		"offers.city_id as city_id,goods.name as good_name,(offers.to_sell - offers.stock) as quantity_sold," . 
		"offers.stock as quantity_unsold,offers.balance as balance,offers.min_price as min_price " . 
		"from goods inner join player_offers as offers on goods.id=offers.good_id " . 
		"where offers.user_id=$user_id and offers.id=$offer_id";
		$result = mysql_query($sql);
		$offer;
		if($result && ($row = mysql_fetch_assoc($result))) {
			$offer = $row;
		} else {
			return array("success"=>"false", "message"=>"Offer does not exist.");
		}
		
		return array("success"=>"true","offer"=>$offer);
	}
?>