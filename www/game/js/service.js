function Economy(engine) {
	this.http_engine = engine;
	this.request = function(method, params) {
		var url = "http://tradingseas.com/economy/api/";
		var query = "?method=" + method;
		
		for(var param in params) {
			query += "&" + param + "=" + params[param];
		}
		console.log(url+query)
		return this.http_engine.get(url + query)
	}
}