<html>
<head>

</head>
<body>
<script>
switch ({{day}}) {
  
  case 7:
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
document.getElementById("demo").innerHTML = "Today is " + day;
	}
</script>
<h1> Day selected {{ day }} </h1>
<h1> Time {{time}} </h1>
<h1>Predicted value is {{predict}}</h1>
</body>
</html>