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

setInterval(function() {
    update_clock();
}, 100);

function update_clock(){
    const formatter = new Intl.DateTimeFormat('us', { month: 'long' });
    var date = new Date();
    var day = date.getDay();
    var hr = date.getHours();
    var min = date.getMinutes();
    var ampm = "PM";
    console.log(date);

    switch (day){
        case 0:
            day = "Sunday";
            break;
        case 1:
            day = "Monday";
            break;
        case 2:
            day = "Tuesday";
            break;
        case 3:
            day = "Wednesday";
            break;
        case 4:
            day = "Thursday";
            break;
        case 5:
            day = "Friday";
            break;
        case 6:
            day = "Saturday";
            break;
    }
    if (hr < 12){
        ampm = "AM";
        if (hr == 0){
            hr = 12;
        }

        if (hr < 10){
            hr = "0" + hr;
        }
    }
    else if (hr > 12){
        hr -= 12;
        if (hr < 0){
            hr = hr * -1;
        }
    }

    clock_date_fld.innerHTML = day + " " + formatter.format(date) + " " + date.getDate() + ", " + date.getFullYear();
    if (min < 10){
        clock_time_fld.innerHTML = hr + ":0" + date.getMinutes() + ampm;
    } else {
        clock_time_fld.innerHTML = hr + ":" + date.getMinutes() + ampm;
    }

}

console.log('grabbing alarms');
$.getJSON("..//get_alarms",
    function(data){
        console.log(data);
        console.log('duh');
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
	console.log("Remove buttonn clicked for: " +alarm_name);
}
