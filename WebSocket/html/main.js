//document.getElementById("datetime").innerHTML = "WebSocket is not connected";

<!-- Global Stuff -->
var websocket = new WebSocket('ws://'+location.hostname+'/');
var radioNumber = 0;
var segment = new SevenSegmentDisplay("SVGSSD");


function sendId(name) {
	console.log('sendId name=[%s]', name);
	var data = {};
	data["id"] = name;
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

function sendIdValue(name, text) {
	console.log('sendIdValue name=[%s] text=[%s]', name, text);
	var data = {};
	data["id"] = name;
	data["value"] = text;
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

function stereoMode(mode) {
	if (mode == 0) {
		document.getElementById("speaker2").style.visibility ="hidden";
	} else if (mode == 1) {
		document.getElementById("speaker2").style.visibility ="visible";
	}
}

/*
function changeColor() {
	var color = parseInt(document.getElementById("color").value);
	console.log("changeColor color=", color);
	segment.ColorScheme = color;
	sendIdValue('color-request', String(color));
}
*/

function showPreset(text, defaultMode) {
	console.log("showPreset text=", text);
	if (text.indexOf('.') == -1) {
		// 87 --> 87.0
		text = text + ".0";
		console.log("text(2)=", text);
	}
	console.log("showPreset defaultMode=", defaultMode);

	// Remove Event Listener
	var radioList = document.querySelectorAll("input[type='radio']");
	console.log("radioList[b]=", radioList.length);
	for (var i = 0; i < radioList.length; i++) {
		radioList[i].removeEventListener('change', radioEvent);
	}

	// Set ID
	//var presetId = "radio" + String(radioNumber);
	var presetId = "preset" + String(radioNumber);
	var defaultId = "default" + String(radioNumber);
	var checkValue = text;

	// Add preset radio
	var check = document.createElement('input');
	//check.setAttribute('type', 'checkbox');
	check.setAttribute('type', 'radio');
	console.log("presetId=", presetId);
	//check.setAttribute('name', "presetGroup");
	check.setAttribute('name', "jump-request");
	//check.setAttribute('value', presetId);
	check.setAttribute('value', checkValue);
	check.setAttribute('id', presetId);
	if (defaultMode == 1) {
		check.setAttribute('checked', 'true');
	}
	document.getElementById('preset-panel').appendChild(check);

	// Add preset label
	var checkLabel = document.createElement('label');
	checkLabel.setAttribute('for',presetId); // this corresponds to the checkbox id
	checkLabel.innerText = 'Goto ' + text + 'MHz';
	document.getElementById('preset-panel').appendChild(checkLabel);

	// Add system default radio
	var check = document.createElement('input');
	//check.setAttribute('type', 'checkbox');
	check.setAttribute('type', 'radio');
	console.log("defaultId=", defaultId);
	//check.setAttribute('name', "defaultGroup");
	check.setAttribute('name', "write-request");
	//check.setAttribute('value', defaultId);
	check.setAttribute('value', checkValue);
	check.setAttribute('id', defaultId);
	if (defaultMode == 1) {
		check.setAttribute('checked', 'true');
	}
	document.getElementById('preset-panel').appendChild(check);

	// Add default label
	var checkLabel = document.createElement('label');
	checkLabel.setAttribute('for',defaultId); // this corresponds to the checkbox id
	checkLabel.innerText = 'As system default';
	document.getElementById('preset-panel').appendChild(checkLabel);

	// Add br
	var br = document.createElement("br");
	document.getElementById('preset-panel').appendChild(br);

	radioNumber = radioNumber + 1;

	// Add all checkboxes to addEventListener
	// https://mebee.info/2020/09/04/post-18164/
	//var chk = document.querySelectorAll("input[type='checkbox']");
	var radioList = document.querySelectorAll("input[type='radio']");
	console.log("radioList[a]=", radioList.length);
/*
	for (let i = 0; i < radioList.length; i++) {
		radioList[i].addEventListener('change', (event) => {
			console.log("EventListener i=", i);
			if (radioList[i].checked) {
				console.log("value=", radioList[i].value);
			}
		});
	}
*/
	for (var i = 0; i < radioList.length; i++) {
		radioList[i].addEventListener('change', radioEvent);
	}
}

// Event handler for radio
function radioEvent() {
	var radioList = document.querySelectorAll("input[type='radio']");
	console.log("radioEvent radioList.length=", radioList.length);
	console.log("radioEvent radioList=", radioList);
	for (var i = 0; i < radioList.length; i++) {
		console.log("radioEvent i=", i);
		if (radioList[i].checked) {
			console.log("radioEvent id=", radioList[i].id);
			console.log("radioEvent name=", radioList[i].name);
			console.log("radioEvent typeof(name)", typeof(radioList[i].name));
			console.log("radioEvent value=", radioList[i].value);
			console.log("radioEvent typeof(value)", typeof(radioList[i].value));
			sendIdValue(radioList[i].name, radioList[i].value)

			// Change number of segments
			if (radioList[i].name == "jump-request") {
				var freq = parseFloat(radioList[i].value, 10);
				console.log("freq=", freq);
				var digits = 3;
				if (freq >= 100.0) digits = 4;
				console.log("digits=", digits);
				segment.NumberOfDigits = digits;
			}
		}
	}
}


// Draw signal level
function signalLevel (level, size) {
	var element = document.getElementById( "signal-canvas" ) ;
	var context = element.getContext( "2d" ) ;


	var xpos = (size*2);
	var ypos = 100;
	for(var i=0;i<15;i++) {
		context.beginPath () ;
		context.arc( xpos, ypos, size, 0 * Math.PI / 180, 360 * Math.PI / 180, false ) ;
		context.fillStyle = "white";
		context.fill() ;

		if (i < level) {
			context.beginPath () ;
			context.arc( xpos, ypos, size, 0 * Math.PI / 180, 360 * Math.PI / 180, false ) ;
			//context.fillStyle = "rgba(0,255,0,0.8)" ;
			if (i < 7) {
				context.fillStyle = "red";
			} else {
				context.fillStyle = "blue";
			}
			context.fill() ;
		} else {
			context.beginPath () ;
			context.arc( xpos, ypos, size, 0 * Math.PI / 180, 360 * Math.PI / 180, false ) ;
			//context.fillStyle = "rgba(255,0,0,0.8)" ;
			//context.fill() ;
			//context.strokeStyle = "rgba(0,255,0,0.8)" ;
			context.strokeStyle = "cyan";
			context.stroke();
		}
	xpos = xpos + (size*2);
	}
}

// Set panel position
function panel_init() {
	var canvas = document.getElementById("signal-panel");
	canvas.style.position = "absolute";
	canvas.style.left = "0px";
	canvas.style.top = "120px";

	var canvas = document.getElementById("speaker-panel");
	canvas.style.position = "absolute";
	canvas.style.left = "10px";
	canvas.style.top = "240px";

	var canvas = document.getElementById("button-panel");
	canvas.style.position = "absolute";
	canvas.style.left = "10px";
	canvas.style.top = "280px";

	var canvas = document.getElementById("preset-panel");
	canvas.style.position = "absolute";
	canvas.style.left = "10px";
	canvas.style.top = "340px";
}


window.onload = function() {
	console.log("onload");
	panel_init();

	// ColorScheme 1-6
	segment.ColorScheme = 2;
	segment.DecimalPointType = 2;
	segment.NumberOfDecimalPlaces = 2;
	segment.NumberOfDigits = 3;
};


websocket.onopen = function(evt) {
	console.log('WebSocket connection opened');
	var data = {};
	data["id"] = "init";
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
	//document.getElementById("datetime").innerHTML = "WebSocket is connected!";
}

websocket.onmessage = function(evt) {
	var msg = evt.data;
	//console.log("msg=" + msg);
	var values = msg.split('\4'); // \4 is EOT
	//console.log("values=" + values);
	switch(values[0]) {
		case 'HEAD':
			console.log("HEAD values[1]=" + values[1]);
			var h1 = document.getElementById( 'header' );
			h1.textContent = values[1];
			break;

		case 'STATUS':
			//console.log("STATUS values[1]=" + values[1]);
			//console.log("STATUS values[2]=" + values[2]);
			//console.log("STATUS values[3]=" + values[3]);
			segment.Value = parseFloat(values[1]);
			var stereo_mode = parseInt(values[2], 10);
			stereoMode(stereo_mode)
			var signal_level = parseInt(values[3], 10);
			signalLevel(signal_level, 8);
			break;

		case 'PRESET':
			console.log("PRESET values[1]=" + values[1]);
			showPreset(values[1], 1);
			var value = parseFloat(values[1]);
			console.log("PRESET value=" + value);
			break;

		case 'PRESET_LIST':
			console.log("PRESET_LIST values[1]=" + values[1]);
			console.log("PRESET_LIST values[2]=" + values[2]);
			var value = parseInt(values[1]);
			var mode = parseInt(values[2]);
			value = value/10.0;
			console.log("PRESET_LIST value=" + value);
			var text = String(value);
			console.log("PRESET_LIST text=" + text);
			showPreset(text, mode);

			// Change number of segments
			if (mode == 1) {
				var digits = 3;
				if (value >= 100.0) digits = 4;
				console.log("PRESET_LIST digits=" + digits);
				segment.NumberOfDigits = digits;
			}
			break;

		case 'COLOR':
			console.log("COLOR values[1]=" + values[1]);
			var color = parseInt(values[1]);
			segment.ColorScheme = color;
			break;

		default:
			break;
	}
}

websocket.onclose = function(evt) {
	console.log('Websocket connection closed');
	//document.getElementById("datetime").innerHTML = "WebSocket closed";
}

websocket.onerror = function(evt) {
	console.log('Websocket error: ' + evt);
	//document.getElementById("datetime").innerHTML = "WebSocket error????!!!1!!";
}
