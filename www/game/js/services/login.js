game.factory('login', ['$http', function($http) { 
  return $http.get('http://tradingseas.com/economy/api/?method=login&city_id=1') 
            .success(function(data) { 
              return data; 
            }) 
            .error(function(err) { 
              return err; 
            }); 
}]);