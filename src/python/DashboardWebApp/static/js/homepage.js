document.addEventListener('DOMContentLoaded', function() {

	var elems = document.querySelectorAll('.dropdown-trigger');
	var instances = M.Dropdown.init(elems);
	
	elems = document.querySelectorAll('.sidenav');
	instances = M.Sidenav.init(elems);
	
	var add_btn_alarm = document.getElementById('add_btn_alarm');
	add_btn_alarm.addEventListener("click", handle_add_alarm);
	
	var remove_btns = document.querySelectorAll('#remove_btn_alarm');
	for (i=0;i<remove_btns.length;i++){
		remove_btns[i].addEventListener("click", handle_remove_alarm);
	}
	var edit_btns = document.querySelectorAll('#edit_btn_alarm');
	for (i=0;i<remove_btns.length;i++){
		edit_btns[i].addEventListener("click", handle_edit_alarm);
	}
	
	//var modal = document.querySelectorAll('modal');
	var jquerymodal = $('.modal');  //convert to jQuery Element
	$('.modal').modal();
	jquerymodal.modal();



  });

var clock_date_fld = document.getElementById("clock-date");
var clock_time_fld = document.getElementById("clock-time");

var light_state_ctrl = document.getElementById("");
var light_intensity_ctrl = document.getElementById("light-intensity-ctrl");
var light_color_ctrl = document.getElementById("light-color-ctrl");
var light_state_ctrl = document.getElementById("light-state-ctrl");
var lighting_options_tbl = document.getElementById("lighting-options-tbl");
var light_intensity_lbl = document.getElementById("light-intensity-lbl");
var led_state = 0;
var irgb = "c8ffffff";
var alarms = ["DrankinPatna", "HarryPotter"];
var last_alarm_datetime;
var last_alarm;
get_led_status();
get_alarms();

// update the clock, behavior points, & check for alarm triggers every 100ms
setInterval(function() {
    update_clock();
    handle_behavior_points(0);
    is_alarm_time();
}, 100);

// update the alarms list every 15min
setInterval(function() {
    get_alarms();
}, 900000);

function test_alarm(){
    console.log("testing arduino alarm...");
    alarm = alarms[1] 
    var path = "../trigger_alarm?melody=" +alarm["alarm_melody"] + "&msg=" +alarm["msg"] + "&irgb=" + alarm["color"];
    console.log(alarm);
    console.log(path);
    $.getJSON(path,
        function(data) {
            console.log(data);

        });
}

function get_alarms(){
    var path = "../get_alarms";
    $.getJSON(path,
        function(data) {
            console.log(data);
            alarms = data;
        });
}

function is_alarm_time(){
    for (var i in alarms){
        var current_date = new CustomDateObj(new Date());
        var alarm = alarms[i];

        // Check Days, Start Date, Repeat, & Time
        var hr = alarm["time"].split(":")[0];
        var min = alarm["time"].split(":")[1];
        hr = parseInt(hr);
        var ampm = min.substr(-2).toUpperCase();
        min = parseInt(min.substr(0,2));
        // Test to check if alarm triggers at correct time.
        if (hr == parseInt(current_date.hr) && min == parseInt(current_date.minute) && ampm == current_date.am_pm){
            // avoid resending the alarm trigger once already triggered.
            if (last_alarm != alarm && last_alarm_datetime != current_date){
                var path = "../trigger_alarm?melody=" +alarm["alarm_melody"] + "&msg=" +alarm["msg"] + "&irgb=" + alarm["color"];
                console.log(alarm["name"]);
                last_alarm = alarm;
                last_alarm_datetime = current_date;
                $.getJSON(path,
                    function(data) {
                        console.log(data);

                    });
            }
        }
        //console.log(current_date);
    }
    return false;
}

function get_led_status(){
    var path = "../led_status";
    $.getJSON(path,
        function(data) {
            console.log(data);
            console.log(typeof data);
            console.log("returned led state:" + data['state']);
            console.log("returned led irgb:" + data['irgb'].substr(2));
            console.log("returned led i:" + data["irgb"].substr(0,2));
            console.log("returned led r:" + data["irgb"].substr(2,2));
            console.log("returned led g:" + data["irgb"].substr(4,2));
            console.log("returned led b:" + data["irgb"].substr(-2));
            if (data["state"] == "0"){
                led_state = 0;
                lighting_options_tbl.style.display = "none";
                light_state_ctrl.checked = false;
            } else {
                led_state = 1;
                lighting_options_tbl.style.display = "block";
                light_state_ctrl.checked = true;
            }
            irgb =  data['irgb'].substr(2);
            light_intensity_ctrl.value = parse_string_to_byte(data["irgb"].substr(0,2));
            light_intensity_lbl.innerHTML = light_intensity_ctrl.value + "%";
            light_color_ctrl.value = "#" + data["irgb"].substr(2,2) + data["irgb"].substr(4,2) + data["irgb"].substr(-2);
        });
}

function parse_string_to_byte(byte_string){
    var final_val = 0;
    if (byte_string.charCodeAt(0) > 60 && byte_string.charCodeAt(1) > 60){
        final_val = (byte_string.charCodeAt(0) - 55) + ((byte_string.charCodeAt(1) - 55) * 16);
    } else if (byte_string.charCodeAt(0) > 60 && byte_string.charCodeAt(1) < 60){
        final_val = (byte_string.charCodeAt(0) - 48) + ((byte_string.charCodeAt(1) - 55) * 16);
    } else if (byte_string.charCodeAt(0) < 60 && byte_string.charCodeAt(1) > 60){
        final_val = (byte_string.charCodeAt(0) - 55) + ((byte_string.charCodeAt(1) - 48) * 16);
    } else if (byte_string.charCodeAt(0) < 60 && byte_string.charCodeAt(1) < 60){
        final_val = (byte_string.charCodeAt(0) - 48) + ((byte_string.charCodeAt(1) - 48) * 16);
    }
    return final_val;
}

function handle_light_state(state){
    console.log("testing light state, value: " + state);

    if (state) {
        lighting_options_tbl.style.display = "block";
        led_state = 1;
    } else {
        lighting_options_tbl.style.display = "none";
        led_state = 0;
    }
    set_led_status();
}

function set_led_status(){
    test_alarm();
    var path = "../led_adjust/" +led_state +" "+ irgb;
    console.log("full path: " + path);
    $.getJSON(path,
        function(data) {
            console.log(data);

        });
}

function handle_light_intensity(intensity){
    console.log("testing light intensity, value: " + intensity);
    console.log("intensity conversion " + light_intensity_ctrl.value + "=>" + parseInt(light_intensity_ctrl.value).toString(16));
    light_intensity_lbl.innerHTML = intensity + "%";
    irgb = parseInt(light_intensity_ctrl.value).toString(16) + light_color_ctrl.value.substr(1).toString(16);

    set_led_status();
}

function handle_light_color(color){
    console.log("testing light color, value: " + color);
    console.log("intensity conversion " + light_intensity_ctrl.value + "=>" + parseInt(light_intensity_ctrl.value).toString(16));
    irgb = parseInt(light_intensity_ctrl.value).toString(16) + light_color_ctrl.value.substr(1).toString(16);

    set_led_status();
}

function handle_behavior_points(btn_id){
    //console.log(btn_id);
    var arg = -1;
    switch (btn_id){
        case "happy-marks-up-btn":
            arg = 0;
            break;
        case "happy-marks-down-btn":
            arg = 1;
            break;
        case "sad-marks-up-btn":
            arg = 2;
            break;
        case "sad-marks-down-btn":
            arg = 3;
            break;
        case "bonus-marks-up-btn":
            arg = 4;
            break;
        case "bonus-marks-down-btn":
            arg = 5;
            break;
    }

    //console.log(arg);
    var path = "..//update_behavior_points/" + arg;
    $.getJSON(path,
        function(data) {
            //console.log(data);
            update_behavior_graph(data["behavior_record"]["happy_point_count"], data["behavior_record"]["sad_point_count"], data["behavior_record"]["bonus_point_count"])
        });
}

function update_clock(){
    const formatter = new Intl.DateTimeFormat('us', { month: 'long' });
    var date = new CustomDateObj(new Date());

    clock_date_fld.innerHTML = date.day_text + " " + formatter.format(date.date) + " " + date.day_date + ", " + date.year;
    clock_time_fld.innerHTML = date.hr + ":" + date.minute + date.am_pm

}


class CustomDateObj {
    constructor(date){
        this.date = date;
        this.day_date = date.getDate();
        this.day_number = date.getDay();
        this.day_text = Intl.DateTimeFormat('en', { weekday: 'long' }).format(date);
        this.am_pm = "PM";
        this.hr = date.getHours();
        this.minute = ('0' + date.getMinutes()).slice(-2);
        this.year = date.getUTCFullYear();
        this.month_number = date.getMonth();
        this.month_text = Intl.DateTimeFormat('en', { month: 'long' }).format(this.month_number);
        this.get_adjusted_time();
    }

    get_adjusted_time(){
        if (this.hr < 12){
            this.am_pm = "AM";
            if (this.hr == 0){
                this.hr = 12;
            }
        }
        else if (this.hr > 12){
            this.hr = this.hr - 12;
            if (this.hr < 0){
                this.hr = this.hr * -1;
            }
        }

        this.hr = ('0' + this.hr).slice(-2);
    }

}

//console.log('grabbing alarms');
$.getJSON("..//get_alarms",
    function(data){
        //console.log(data);
    }
);

var ctx_behavior = document.getElementById('BehaviorChart').getContext('2d');
var myChart_behavior = new Chart(ctx_behavior, {
	type: 'bar',
	data: {
		labels: ['Marks Count'],
		datasets: [
			{
				label : 'Happy Marks',
				data : [12],
				backgroundColor : 'rgba(75, 192, 192, 0.2)',
				borderColor : 'rgba(75, 192, 192, 1)',
				borderWidth : 1 
			},
			{
				label : 'Sad Marks',
				data : [9],
				backgroundColor : 'rgba(255, 99, 132, 0.2)',
				borderColor : 'rgba(255, 99, 132, 1)',
				borderWidth : 1 
			},
			{
				label : 'Bonus Marks',
				data : [3],
				backgroundColor : 'rgba(255, 206, 86, 0.2)',
				borderColor : 'rgba(255, 206, 86, 1)',
				borderWidth : 1 
			}
		]},
	options: {
		responsive: true,
		plugins: {
			legend: {
				display: true,
				position: 'right'
			}
		},
		indexAxis: 'y',
		scales: {
			y: {
				beginAtZero: true
			}
		}
	 }
});


var ctx_school = document.getElementById('SchoolCoverageChart').getContext('2d');
var myChart_school = new Chart(ctx_school, {
	type: 'polarArea',
	data: {
		labels: ['Math', 'Science', 'Reading', 'History', 'Writing', 'Art'],
		datasets: [{
			label: '# of Votes',
			data: [12, 19, 3, 5, 2, 3],
			backgroundColor: [
				'rgba(255, 99, 132, 0.2)',
				'rgba(54, 162, 235, 0.2)',
				'rgba(255, 206, 86, 0.2)',
				'rgba(75, 192, 192, 0.2)',
				'rgba(153, 102, 255, 0.2)',
				'rgba(255, 159, 64, 0.2)'
			],
			borderColor: [
				'rgba(255, 99, 132, 1)',
				'rgba(54, 162, 235, 1)',
				'rgba(255, 206, 86, 1)',
				'rgba(75, 192, 192, 1)',
				'rgba(153, 102, 255, 1)',
				'rgba(255, 159, 64, 1)'
			],
			borderWidth: 1
		}]
	}
});




function handle_edit_alarm(){
	var alarm_name = this.parentElement.parentElement.getElementsByTagName("a")[0].getElementsByTagName("h5")[0].innerHTML;
	console.log("Edit buttonn clicked for: " +alarm_name);
}

function handle_add_alarm(){
	console.log("Add button clicked.");
}

function handle_remove_alarm(){
	var alarm_name = this.parentElement.parentElement.getElementsByTagName("a")[0].getElementsByTagName("h5")[0].innerHTML;
	console.log("Remove button clicked for: " +alarm_name);
}

function update_behavior_graph(happy_pts, sad_pts, bonus_pts){
    //console.log(myChart_behavior.data.datasets[0].data);
    myChart_behavior.data.datasets[0].data = [happy_pts];
    myChart_behavior.data.datasets[1].data = [sad_pts];
    myChart_behavior.data.datasets[2].data = [bonus_pts];
    myChart_behavior.update();
}