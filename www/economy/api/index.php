<?php
	require_once "config.php";
	require_once "password_utils.php";
	require_once "db_utils.php";
	
	error_reporting(E_ALL);
	ini_set('display_errors', '1');
	
	function isAssoc($arr) {
		return array_keys($arr) !== range(0, count($arr) - 1);
	}
	
	function JSON_from_array($array) {
	
		header('Content-type: application/json');
	
		$json = json_encode($array);
		if(isset($_GET['pretty'])) {
			require_once "json_utils.php";
			$json = indent($json);
		}
		return $json;
	}
	
	function plist_from_array($array, $indent="") {
		$assoc = isAssoc($array);
		
		$next_indent = isset($_GET['pretty'])?"  ".$indent:"";
		$new_line = isset($_GET['pretty'])?"\n":"";
		
		$result = $indent.($assoc?("<dict >"):("<array >")).$new_line;
		foreach ($array as $i => $value) {
			if( is_array($value)) {
				$result .= ($assoc?("  <key >".$i."</key >".$new_line.plist_from_array($value,$next_indent)):(plist_from_array($value,$next_indent))).$new_line;
			} else if(is_int($value)) {
				$result .= $next_indent.($assoc?("<key >".$i."</key >"."<integer >".$value."</integer >"):("<integer >".$value."</integer >")).$new_line;
			} else {
				$result .= $next_indent.($assoc?("<key >".$i."</key >"."<string >".$value."</string >"):("<string >".$value."</string >")).$new_line;
			}
		}
		$result .= $indent.($assoc?("</dict >"):("</array >"));
		return $result;
	}
	
	function fgetline ($file) {
		$result = "";
		$c = " ";
		do {
			$c = fread($file,1);
			if(($c || $c == '0') && $c != "\r" && $c != "\n")
				$result .= $c;
		} while ($c == '0' || $c != "\r" && $c != "\n" && $c);
		if($result == "") return false;
		return $result;
	}
	
	function authenticate_admin($auth_token) {
		$authenticated = authenticate_user($auth_token);
		return $authenticated['admin']?$authenticated:false;
	}
	
	function authenticate_user($auth_token) {
		global $a_skeleton_key, $db_address, $db_user, $db_password, $db_table_prefix, $db_database ;
		$authenticated = false;
		if (strlen($a_skeleton_key) > 0 && $auth_token == $a_skeleton_key) {
			$authenticated = array("user_id" => "0","admin" => true);
			$authenticated['username'] = "superuser";
			$authenticated['telephone'] = "(555) 555-5555";
			$authenticated['auth_token'] = $auth_token;
		} else {
			$con = mysql_connect($db_address, $db_user, $db_password);
			if (!$con) die('Could not connect: ' . mysql_error());
			mysql_select_db($db_database, $con);
			
			$sql = "select * from ".$db_table_prefix."_sessions where auth_token='$auth_token'";
			$result = mysql_query($sql);
			
			if($result && $row = mysql_fetch_assoc ($result)) {
				$user_id = $row['user_id'];
			} else {
				return false;
			}
			
			$sql = "select * from ".$db_table_prefix."_users where id='$user_id'";
			$result = mysql_query($sql);
			
			if($result && $row = mysql_fetch_assoc ($result)) {
				$authenticated = array();
				$authenticated['user_id'] = $row['id'];
				$authenticated['admin'] = false;
				$authenticated['username'] = $row['username'];
				$authenticated['telephone'] = $row['telephone'];
				$authenticated['auth_token'] = $auth_token;
				$authenticated['admin'] = $row['director'];
			}
		}
		return $authenticated;
	}
	
	function done($result) {
		
		if(isset($_REQUEST['debug'])) {
			$result['debug'] =  array(
			"headers" => apache_request_headers(),
			"request" =>  $_REQUEST,
			"get" =>  $_GET,
			"post" =>  $_POST,
			"cookie" =>  $_COOKIE,
			);
		}
		
		if(isset($_REQUEST['plist'])) {
			echo plist_from_array($result);
		} else {
			echo JSON_from_array($result);
		}
		exit(0);
	}
	
	$targets = array (
		"login" => array (
			"function" => "login",
			"file" => "login.php",
			"required_params" => array (
				"username",
				"password"
			),
			"optional_params" => array ()
		),
		"logout" => array (
			"function" => "logout",
			"file" => "logout.php",
			"required_params" => array (
				"auth_token"
			),
			"optional_params" => array ()
		),
		"createuser" => array (
			"function" => "createuser",
			"file" => "createuser.php",
			"required_params" => array (
				"username",
				"password1",
				"password2"
			),
			"optional_params" => array (
			)
		),
		"get_market_data" => array (
			"function" => "get_market_data",
			"file" => "get_market_data.php",
			"required_params" => array (
				"auth_token"
				
			),
			"optional_params" => array ()
		),
		"get_captain_data" => array (
			"function" => "get_captain_data",
			"file" => "get_captain_data.php",
			"required_params" => array (
				"auth_token"
				
			),
			"optional_params" => array ()
		),
		"submit_offer" => array (
			"function" => "submit_offer",
			"file" => "submit_offer.php",
			"required_params" => array (
				"auth_token",
				"good_id",
				"num_to_sell",
				"min_price"
			),
			"optional_params" => array ()
		),
		"submit_bid" => array (
			"function" => "submit_bid",
			"file" => "submit_bid.php",
			"required_params" => array (
				"auth_token",
				"good_id",
				"num_to_buy",
				"max_price",
				"money"
			),
			"optional_params" => array ()
		),
		"get_offer_status" => array (
			"function" => "get_offer_status",
			"file" => "get_offer_status.php",
			"required_params" => array (
				"auth_token",
				"offer_id"
			),
			"optional_params" => array ()
		),
		"get_bid_status" => array (
			"function" => "get_bid_status",
			"file" => "get_bid_status.php",
			"required_params" => array (
				"auth_token",
				"bid_id"
			),
			"optional_params" => array ()
		),
		"claim_offer" => array (
			"function" => "claim_offer",
			"file" => "claim_offer.php",
			"required_params" => array (
				"auth_token",
				"offer_id"
			),
			"optional_params" => array ()
		),
		"claim_bid" => array (
			"function" => "claim_bid",
			"file" => "claim_bid.php",
			"required_params" => array (
				"auth_token",
				"bid_id"
			),
			"optional_params" => array ()
		),
		"get_city_list" => array (
			"function" => "get_city_list",
			"file" => "get_city_list.php",
			"required_params" => array (
				"auth_token"
			),
			"optional_params" => array ()
		),
		"travel" => array (
			"function" => "travel",
			"file" => "travel.php",
			"required_params" => array (
				"auth_token",
				"city_id"
			),
			"optional_params" => array ()
		)
	);
	
	
	$result = array();
	$result['method'] = "none";
	$result['success'] = "false";
	$result['logged_in'] = "false";
	$failed = false;
	
	
	
	$postdata = file_get_contents("php://input");
	$json_data = json_decode ($postdata);
	if($json_data) {
		$json_data = (array) $json_data;
	} else {
		$json_data = array();
	}
	
	$_REQUEST = array_merge($_REQUEST, $json_data);
	$_REQUEST = array_merge($_REQUEST, $_COOKIE);
	
	if (isset($_REQUEST['method']) && array_key_exists ($_REQUEST['method'], $targets)) {
		$target = $targets[$_REQUEST['method']];
		require_once($target['file']);
		$result['method'] = $_REQUEST['method'];
		
		$function = $targets[$_REQUEST['method']]['function'];
		
		$params = array ();
		foreach ($target['required_params'] as $param) {
			if (!isset($_REQUEST[$param])) {
				$result['success'] = "false";
				$result['message'] = "Required parameter not found";
				$result['error_code'] = 83;
				$result['missing_param'] = $param;
				done($result);
			} else {
				if($param == "auth_token") {
					$user_id = mysql_db_user_id_from_session_token($_REQUEST[$param]);
					if(!$user_id) {
						$result['success'] = "false";
						$result['message'] = "Authorization Failed. Invalid auth_token.";
						$result['error_code'] = 54;
						done($result);
					}
					$params["user_id"] = $user_id;
					$result["logged_in"] = "true";
				} else {
					$params[$param] = $_REQUEST[$param];
				}
			}
		}
		foreach ($target['optional_params'] as $param) {
			if (!isset($_REQUEST[$param])) {
				$params[$param] = "";
			} else {
				$params[$param] = $_REQUEST[$param];
			}
		}
		$partial_result = call_user_func_array($function,$params);
		$result = array_merge($result, $partial_result);
	} else {
		$result['success'] = "false";
		$result['error_code'] = 84;
		$result['message'] = "No method named.";
	}
	
	done($result);
?>
