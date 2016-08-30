
//-------GENERATED FILE-------
var express = require('express');
var router = express.Router();
var upload = require('multer');

router.post("/helloPost/:username/:pass",
	upload.fields([
		{name:'cool'},
		{name:'pic'},
		{name:'fileName'},
		{name:'theDict'},
		{name:'theArray'}
	]) ,function(req,res){
	//BEGIN PARAMS 
	var username     = req.params.username;
	var pass         = req.params.pass;
	//END PARAMS 

	//BEGIN POST PARAMS
	var cool         = JSON.parse(req.body.cool);
	var pic          = JSON.parse(req.body.pic);
	var fileName     = JSON.parse(req.body.fileName);
	var theDict      = JSON.parse(req.body.theDict);
	var theArray     = JSON.parse(req.body.theArray);

	//END   POST PARAMS

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