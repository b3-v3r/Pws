var json_o = 0;
var json_text = 0;

var page = 0;

var all_projects = 0;
var all_windows = 0;

var windows_elems = 0;
var projects_elems = 0;

var notes_editor = 0;

var list_notes = 0;
var list_tasks = 0;

var last_task_id = 0; 

var hour = 0;


function FindWindow( name )
{
     for( var i = 0; i < windows_elems.length; i++ )
     {
          if( windows_elems[i].getElementsByClassName("window-name")[0].getElementsByTagName("LI")[0].textContent.trim().toLowerCase() 
                         == name.trim().toLowerCase() )
               return i
     }

     return -1;
}

//
// Functions for convert time 
//

function msToHoursMinAndSec(millisec) {
     
     if( millisec == 0 )
          return "0:0";

     var seconds = (millisec / 1000).toFixed(0);
     var minutes = Math.floor(seconds / 60);
     var hours = "";
     if (minutes > 59) {
          hours = Math.floor(minutes / 60);
          hours = (hours >= 10) ? hours : "0" + hours;
          minutes = minutes - (hours * 60);
          minutes = (minutes >= 10) ? minutes : "0" + minutes;
     }

     seconds = Math.floor(seconds % 60);
     seconds = (seconds >= 10) ? seconds : "0" + seconds;
     if (hours != "") {
          return hours + ":" + minutes + ":" + seconds;
     }
     return minutes + ":" + seconds;
}

function checkSec( sec )
{
     if( sec > 0 && sec <= 9 )
          return "0"+sec;
     return sec;
}

function getFormatTime( seconds )
{
     if (seconds > 60)
     {
          var min = Math.trunc(seconds / 60);
          var sec = seconds - ( min * 60 );
          return  min + ":" + checkSec(sec); 
     }
     return "00:"+checkSec(seconds);
}

// 
// Fuctions for add and remove windows
// 

function AddNewWindow( name, pid, CPM, CPS, all_press_keys, all_time, img_path )
{
     var num_window = FindWindow( name )

     if( num_window == -1 )
     {
          const new_w = document.createElement("DIV");
      
          new_w.setAttribute("class", "window");
          new_w.innerHTML = `

          <div class="window-name-border">
          <div class="window-name">
               <img src="`+img_path+`" alt="`+name+`">
               <li>`+name.charAt(0).toUpperCase()+name.substring(1)+`</li>
               <p>`+pid+`</p>
          </div>

          </div>

          <div class="stat_text"><p>Chars per minute</p></div>
          <div class="stat_num"><p>0</p></div>

          <div class="stat_text"><p>Chars per second</p></div>
          <div class="stat_num"><p>0</p></div>

          
          <div class="stat_text"><p>All num press keys</p></div>
          <div class="stat_num"><p>0</p></div>
     
          <div class="time_using_window">
               <center>
                    <p>Application usage time</p>
                    <h3>`+msToHoursMinAndSec(all_time)+`</h3>
               </center>
          </div>
          `;   

          var num_stats = new_w.getElementsByClassName("stat_num");
          
          if( all_press_keys != 0 )
          {
               num_stats[0].innerHTML = "<p>" + Math.round(CPM) + "</p>";
               num_stats[1].innerHTML = "<p>" + CPS.toFixed(1) + "</p>";
               num_stats[2].innerHTML = "<p>" + Math.round(all_press_keys) + "</p>";
          }

          all_windows.appendChild(new_w)

     }else {

          if( all_press_keys != 0 )
          {
               var num_stats = windows_elems[num_window].getElementsByClassName("stat_num");
               num_stats[0].innerHTML = "<p>" + Math.round(CPM) + "</p>";
               num_stats[1].innerHTML = "<p>" + CPS.toFixed(1) + "</p>";
               num_stats[2].innerHTML = "<p>" + Math.round(all_press_keys) + "</p>";


               var all_time_elem = windows_elems[num_window].getElementsByClassName("time_using_window");
               all_time_elem[0].innerHTML = `
               <center>
                    <p>Application usage time</p>
                    <h3>`+msToHoursMinAndSec(all_time)+`</h3>
               </center>`;

          }
     }
}

function GetProjectNameFromPath( path )
{
     var result = "";

     for( var i = path.length-1;i != 0; i-- )
     {
          if( path[i] == '/')
               break;
          
          result+=path[i];
     }

     return result.split("").reverse().join("");
}


function AddNewProject( project_path, files )
{
     for( var j=0;j < projects_elems.length;j++ )
     {
          if( GetProjectNameFromPath(project_path.trim()) == 
               projects_elems[j].getElementsByTagName("H2")[0].textContent.trim() )
               return;
     }

     const new_w = document.createElement("DIV");
     new_w.setAttribute("class", "project");


     new_w.innerHTML = `<h2>`+GetProjectNameFromPath(project_path)+`</h2>`;

     for( var i = 0; i < files.length; i++ )
     {
          const new_file_info = document.createElement("DIV");
          new_file_info.setAttribute("class", "file_info");

          new_file_info.innerHTML = `
          <p class="file_path">` + files[i].path + `</p>
          <p class="file_time">` + getFormatTime(files[i].last_change_time) + `  minutes ago</p>
          <p class="file_changes">` + files[i].num_changes+` changes</p>`;

          new_w.appendChild(new_file_info);
     }
     
     all_projects.appendChild(new_w)
}

//
// Load and write data to local storage 
//

function WriteUserDataToPage()
{

     if( localStorage.user_data == undefined )
          return;

     var obj = JSON.parse( localStorage.user_data );

     console.log( localStorage.user_data );

     obj.notes.reverse();
     for( var i = 0; i < obj.notes.length; i++ )
     {
          list_notes.innerHTML = '<div class="note" oncontextmenu="DelNote(this)" contenteditable="true">'+obj.notes[i]+'</div>'+ list_notes.innerHTML;
     }

     for( var i = 0; i < obj.tasks.length; i++ )
     {
          var html_task = `<div class="task" id="` +obj.tasks[i].id+ `">
               <input type="date" value="`+obj.tasks[i].date_deadline+`" onblur="SaveTaskValues()" >
               <i class="material-icons button_delete_task" onclick="DelTask(this)">delete</i>

               <center>
                    <textarea name="" id="" cols="30" onblur="SaveTaskValues()" rows="5" placeholder="Input task info">`+obj.tasks[i].text+`</textarea>
               </center>

               <p>Task execution time:</p>

               <div class="stopwatch">
               
                    <h2 onclick="StartTimerTask(this)">▶</h2>
                    <li class="clock_face">` + msToHoursMinAndSec( obj.tasks[i].time ) +`</li>
                    <time value="0">` + obj.tasks[i].time + `</time>
               </div>
          </div>`;

          list_tasks.innerHTML += html_task;

          last_task_id = parseInt(obj.tasks[i].id);
     }
}

function SaveNotesAndTasksToLocalStorage()
{
     var notes_list_val = list_notes.getElementsByClassName("note");
     var tasks_list_val = list_tasks.getElementsByClassName("task");

     var obj = JSON.parse( '{ "notes": [], "tasks": [] }' );

     for( var i =0; i < notes_list_val.length; i++ )
     {
          obj['notes'].push( notes_list_val[i].innerHTML );
     }

     for( var i =0; i < tasks_list_val.length; i++ )
     {
          var task = tasks_list_val[i]
          obj['tasks'].push(
          {
               id : task.id,
               
               text: task.getElementsByTagName("textarea")[0].value,
               date_deadline: task.getElementsByTagName("input")[0].value,

               time: task.getElementsByTagName("time")[0].innerHTML 
          });
     }

     localStorage.user_data = JSON.stringify( obj );
}

// 
// Fuctions for add and remove notes 
// 

function AddNewNote( elem )
{
     if( elem.value.length == 0 )
          return;


     list_notes.innerHTML = '<div class="note" oncontextmenu="DelNote(this)" contenteditable="true">'+elem.value+'</div>'+ list_notes.innerHTML;
     
     elem.value = ""
}

function DelNote( elem )
{
     if( confirm("Delete choice note ? ") )
     {
          elem.outerHTML = ' '
     }
}

function DelAllNotes()
{
     if( confirm("DELETE ALL NOTES ?") )
     {
          list_notes.innerHTML = "";
     }
}
// 
// Fuctions for add and remove tasks 
// 
function AddNewTask()
{
     last_task_id += 1;

     var html_task = `<div class="task" id="` +last_task_id+  `">
               <input type="date" value="" onblur="SaveTaskValues()" >
               <i class="material-icons button_delete_task" onclick="DelTask(this)">delete</i>

               <center>
                    <textarea name="" id="" cols="30" onblur="SaveTaskValues()" rows="5" placeholder="Input task info"></textarea>
               </center>

               <p>Task execution time:</p>

               <div class="stopwatch">
               
                    <h2 onclick="StartTimerTask(this)">▶</h2>
                    <li class="clock_face">0:0:0</li>
                    <time value="0">0</time>
               </div>
          </div>
     `;
     
     list_tasks.innerHTML += html_task;
}

function SaveTaskValues()
{
     // For save input text to html
     var all_task_values = document.getElementsByTagName("textarea");
     var all_inputs_values = list_tasks.getElementsByTagName("input");

     for( var i = 0; i <  all_task_values.length; i++ )
     {
          all_task_values[i].innerHTML = all_task_values[i].value;
          all_inputs_values[i].innerHTML = all_inputs_values[i].value;
     }
}

function DelTask( elem )
{
     var choice = confirm("Delete choice task ? ")

     if( choice )
     {
          elem.parentNode.outerHTML = ' '
     }
}

//
// Function stopwatches 
//

function StopTimerTask( elem )
{
     elem.outerHTML  = '<h2 onclick="StartTimerTask(this)">▶</h2>';
}

function StartTimerTask( elem )
{

     var task_id   = elem.parentNode.parentNode.id;
     var prev_time = parseInt(elem.parentNode.getElementsByTagName("TIME")[0].innerHTML);

     var timer_id = setTimeout( function tick( task_id, prev_time ) { 
          
          var timer_value = document.getElementById(task_id).getElementsByClassName("clock_face")[0].innerHTML; 

          if( timer_value == null ||  
               document.getElementById(task_id).getElementsByTagName("H2")[0].innerHTML != "▐▐")
               return;

          document.getElementById(task_id).getElementsByClassName("clock_face")[0].innerHTML = msToHoursMinAndSec(prev_time+1000);
          document.getElementById(task_id).getElementsByTagName("TIME")[0].innerHTML = prev_time;
          timer_id = setTimeout( tick, 1000, task_id, prev_time+1000);

     }, 1000, task_id, prev_time);
     

     elem.outerHTML = '<h2 onclick="StopTimerTask(this)" style="font-size: 1em; margin-top: 2%;">▐▐</h2>';
}

// 
// Fuctions for make diagram most development projects  
// 

function GetStatData( data )
{
     for( var i =0; i < window.json_o.projects.length; i++ )
     {
          var proj_path = window.json_o.projects[i].project_path;
          var num_changes = 0;

          for( var j=0; j < json_o.projects[i].files.length; j++)
               num_changes += json_o.projects[i].files[j].num_changes;
          
          var arr = [ GetProjectNameFromPath(proj_path), num_changes ];
          data.addRow( [GetProjectNameFromPath(proj_path), num_changes] );
     }
     
     return data;
}

function BuildStats()
{
     google.charts.load("current", {packages:["corechart"]});
     google.charts.setOnLoadCallback(drawChart);
 

     function drawChart() {

          var data_user = new google.visualization.DataTable();
          data_user.addColumn('string', 'Project name');
          data_user.addColumn('number', 'Num changes');

          var mod_data = GetStatData(data_user);

          var options = {
               title: '',
               pieHole: 0.6,
               slices: {
                    0: { color: '#00cec9' },
                    1: { color: '#ff6b81' },
                    2: { color: '#bdd3ff'}
               },
               backgroundColor: 'transparent',
               legend         : 'none',
               chartArea      : {left: 10, top: 10, width: '95%', height: '95%'},
               
               
               pieSliceTextStyle: {
                    color: 'black',
               }
          };

          var chart = new google.visualization.PieChart(document.getElementById('ProjectStat'));
          chart.draw(mod_data, options);          
     }
}

// 
// Fuctions switch hours  
// 

function LoadStatNextHour()
{
     if( hour+1 == 1 )
          return;

     hour += 1;
     all_windows.innerHTML = " ";
 
     if( hour == 0 )
          document.getElementsByClassName("hour_holder")[0].getElementsByTagName("p")[0].innerHTML = "Current hour"; 
     else
          document.getElementsByClassName("hour_holder")[0].getElementsByTagName("p")[0].innerHTML = Math.abs(hour) + " hours ago"; 
     
     WriteToPage();
}

function LoadStatPrevHour()
{
     hour -= 1;
     all_windows.innerHTML = " ";
     document.getElementsByClassName("hour_holder")[0].getElementsByTagName("p")[0].innerHTML = Math.abs(hour) + " hours ago"; 

     WriteToPage();
}

// 
// Main function drawing datas 
// 

function WriteToPage()
{

     window.json_o = JSON.parse( window.json_text );

     //
     // Write windows info 
     //
     
     for ( var i = 0; i < json_o.windows.length; i++ ) {

          var wname = json_o.windows[i].name;
          var wpid  = json_o.windows[i].pid;
          
          var wAll_used_time = json_o.windows[i].all_time;

          var count_hours = 0;
          var j = json_o.windows[i].stat_per_hours.length - 1 + hour;

          if( j < 0 || j > json_o.windows[i].stat_per_hours.length-1 )
               continue;

          var current_CMP = json_o.windows[i].stat_per_hours[j].CPM;
          var current_CMS = json_o.windows[i].stat_per_hours[j].CPS;
          var current_num_press_keys  = json_o.windows[i].stat_per_hours[j].num_pressed_keys;
          var img_path = json_o.windows[i].path_icon;

          AddNewWindow( wname, wpid, current_CMP, current_CMS, current_num_press_keys, wAll_used_time , img_path)
     }

     //
     // Write projects info 
     //
     for( var i =0; i < json_o.projects.length; i++ )
     {
          var project_path = json_o.projects[i].project_path;
          AddNewProject( project_path, json_o.projects[i].files);
     }

     //
     // Build stats
     //
     BuildStats();
}



function showProjects()
{
     document.getElementsByClassName("projects")[0].style.display = "block";
     document.getElementsByClassName("containerProjectStats")[0].style.display = "block";

     document.getElementsByClassName("windows")[0].style.display = "none";
     document.getElementsByClassName("notes")[0].style.display = "none";

}

function showWindows()
{
     document.getElementsByClassName("windows")[0].style.display = "block";
     document.getElementsByClassName("projects")[0].style.display = "none";
     document.getElementsByClassName("notes")[0].style.display = "none";
     document.getElementsByClassName("containerProjectStats")[0].style.display = "none";
}

function showNotes()
{
     document.getElementsByClassName("notes")[0].style.display = "block";
     document.getElementsByClassName("projects")[0].style.display = "none";
     document.getElementsByClassName("windows")[0].style.display = "none";
     document.getElementsByClassName("containerProjectStats")[0].style.display = "none";

}

function GetDataFromClient()
{
     var xmlHttp = new XMLHttpRequest();
     xmlHttp.open( "GET", "/get_json", false );
     xmlHttp.send( null );
     
     window.json_text = xmlHttp.responseText;
     WriteToPage();
}


let main_timer_id = setTimeout( function tick() {
     
     GetDataFromClient();
     main_timer_id = setTimeout( tick, 10000)
}, 10000);




window.onload = function(){

     page          = document.getElementsByClassName("page")[0];

     all_projects  = page.getElementsByClassName("projects")[0];
     all_windows   = page.getElementsByClassName("windows")[0];

     windows_elems = all_windows.getElementsByClassName("window");
     projects_elems = all_projects.getElementsByClassName("project");

     notes_editor = document.getElementsByClassName("notes")[0].getElementsByClassName("editor_notes")[0];
     
     list_notes = notes_editor.getElementsByClassName("list-notes")[0];
     list_tasks = document.getElementsByClassName("list-tasks")[0];
     
     last_task_id = 0; 

     hour = 0;


     GetDataFromClient();
     WriteUserDataToPage();


     // 
     // Save to localStorage all notes and tasks 
     //
     window.onbeforeunload = function () {
          SaveNotesAndTasksToLocalStorage();
     };

}