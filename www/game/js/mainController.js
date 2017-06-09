game.controller('appController', ['http_engine', '$location', function(http_engine, $location) {
	var view_model = this;
	view_model.http_engine = http_engine;
	view_model.economy = new Economy(view_model.http_engine);
	
	function data_loaded(data) {
		//console.log(JSON.stringify(data));
		if(data.logged_in == 'false') {
			$location.url('/login');
		} else {
			if($location.path() == '/login') {
				$location.url('/');
			}
		}
	}
	
	view_model.login = function(username,password) {
		view_model.economy.request("login", {"username":username,"password":password})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				view_model.login_error = data.message;
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}
	
	view_model.passwordsMissmatch = function(password1,password2) {
		
	
		if(!password1 || !password2) {
			return true;
		}
		if(password1 != password2) {
			view_model.createuser_error = "Passwords don't match";
			return true;
		}
		return false;
	}
	

	view_model.createuser = function(username,password1,password2) {
		if(password1 != password2) {
			return;
		}
		
		view_model.economy.request("createuser", {"username":username,"password1":password1,"password2":password2})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				view_model.createuser_error = data.message;
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}
	
	
	view_model.buy = function(good) {
		console.log("Wanting to buy " + good.quantity_to_buy + " " + good.name + " for at least " + good.max_price_to_buy);
		view_model.economy.request("submit_bid", {"good_id":good.id,"num_to_buy":good.quantity_to_buy,"max_price":good.max_price_to_buy,"money":good.max_price_to_buy*good.quantity_to_buy})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				good.message = data.message;
			} else {
				view_model.captain.money -= good.max_price_to_buy*good.quantity_to_buy;
				good.message = "An order has been placed to buy " + good.quantity_to_buy + " " + good.name + " for no more than "  + good.max_price_to_buy + " each";
				
				good.max_price_to_buy = 0;
				good.quantity_to_buy = 0;
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}
	
	view_model.sell = function(good) {
		console.log("Wanting to sell " + good.quantity_to_sell + " " + good.name + " for at least " + good.min_price_to_sell);
		view_model.economy.request("submit_offer", {"good_id":good.good_id,"num_to_sell":good.quantity_to_sell,"min_price":good.min_price_to_sell})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				good.message = data.message;
			} else {
				
				good.quantity -= good.quantity_to_sell;
				good.message = "An order has been placed to sell " + good.quantity_to_sell + " " + good.name + " for at least "  + good.min_price_to_sell + " each";
				good.quantity_to_sell = 0;
				good.min_price_to_sell = good.price;
				
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}
	
	view_model.claim_offer = function(offer) {
		view_model.economy.request("claim_offer", {"offer_id":offer.offer_id})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				offer.message = data.message;
			} else {
				view_model.load_market_data();
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}
	
	view_model.claim_bid = function(bid) {
		view_model.economy.request("claim_bid", {"bid_id":bid.bid_id})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				offer.message = data.message;
			} else {
				view_model.load_market_data();
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}
	
	view_model.travel = function(city) {
		view_model.economy.request("travel", {"city_id":city.city_id})
		.success(function(data) {
			data_loaded(data);
			if(data.success == "false") {
				offer.message = data.message;
				console.log(data.message);
			} else {
				view_model.load_market_data();
				view_model.load_cities();
			}
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	}

	view_model.load_market_data = function() {
		view_model.economy.request("get_market_data", {"city_id":"1"})
		.success(function(data) {
			data_loaded(data);
			view_model.market_data = data;
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	};
	view_model.load_market_data();
	
	view_model.load_captain_data = function() {
		view_model.economy.request("get_captain_data", {})
		.success(function(data) {
			data_loaded(data);
			view_model.captain = data;
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	};
	view_model.load_captain_data();
	
	view_model.load_cities = function() {
		view_model.economy.request("get_city_list", {})
		.success(function(data) {
			data_loaded(data);
			view_model.cities = data.cities;
		}) 
		.error(function(err) { 
			console.log("err " + err);
		});
	};
	view_model.load_cities();
	
}]);