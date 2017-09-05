function function1(button1, div1) 
{
	var div = document.getElementById(div1); 
	if (div.style.display !== 'none') 
	{
		div.style.display = 'none';
		document.getElementById(button1).value = "+";
	}
	else 
	{
		div.style.display = 'block';
		document.getElementById(button1).value = "-";
	}
};