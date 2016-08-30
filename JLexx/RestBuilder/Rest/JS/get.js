
//-------GENERATED FILE-------
var express = require('express');
var router = express.Router();

router.get("/queryWithProjection/:username/:pass/:arrString",function(req,res){
	//BEGIN PARAMS 
	var username     = req.params.username;
	var pass         = req.params.pass;
	var arrString    = req.params.arrString;
	//END PARAMS 

	var success = false;                       // set this to true or false on completion
	var toReturn = "Not yet implemented :("; // set this

	// TODO: Implement Body!

	if(success){
		res.send({"Success":toReturn});
	}else{
		res.send({"Error":toReturn});
	}

});

router.get("/helloWorld",function(req,res){
	//BEGIN PARAMS 
	//END PARAMS 

	var success = false;                       // set this to true or false on completion
	var toReturn = "Not yet implemented :("; // set this

	// TODO: Implement Body!

	if(success){
		res.send({"Success":toReturn});
	}else{
		res.send({"Error":toReturn});
	}

});

router.get("/helloWorldComplex/:username/andPass/:pass",function(req,res){
	//BEGIN PARAMS 
	var username     = req.params.username;
	var pass         = req.params.pass;
	//END PARAMS 

	var success = false;                       // set this to true or false on completion
	var toReturn = "Not yet implemented :("; // set this

	// TODO: Implement Body!

	if(success){
		res.send({"Success":toReturn});
	}else{
		res.send({"Error":toReturn});
	}

});

module.exports = router;