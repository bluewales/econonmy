var game = angular.module('game', ['ngRoute']);

game.config(['$routeProvider', function($routeProvider) {
	$routeProvider
	.when('/', {
		controller: 'appController as view_model', 
		templateUrl: 'js/views/about.html'
	})
	.when('/login', { 
		controller: 'appController as view_model', 
		templateUrl: 'js/views/login.html' 
	})
	.when('/sell', { 
		controller: 'appController as view_model', 
		templateUrl: 'js/views/inventory.html' 
	})
	.when('/buy', {
		controller: 'appController as view_model', 
		templateUrl: 'js/views/market.html' 
	})
	.when('/orders', {
		controller: 'appController as view_model', 
		templateUrl: 'js/views/trades.html' 
	})
	.when('/travel', {
		controller: 'appController as view_model', 
		templateUrl: 'js/views/travel.html' 
	})
	.when('/about', {
		controller: 'appController as view_model', 
		templateUrl: 'js/views/about.html' 
	})
	.otherwise({ 
		redirectTo: '/about' 
	}); 
}]); 