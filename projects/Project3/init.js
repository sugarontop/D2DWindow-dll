// javascript, chakra engine
// ABC
// DDD

function f2(obj,from_app_string)
{

	var k = dd.select("mytextbox");
	k.set("text=" + from_app_string );
}

function click(obj)
{

	var k = dd.select("mytextbox");
	k.set("text=success" );
}

function main1()
{
	var b = dd.select("JSRunButton"); //wor1.cpp, 231 line

	dd.select("s4");
	var k1 = dd.create("type=combobox&id=10001&x=150&y=90&w=100&nm=mylsbox");

	k1.set("add&str=hoi1");
	k1.set("add&str=hoi2");
	k1.set("add&str=hoi3");

	k1.OnChanged = f2;
	b.set("enable=0");
	
	
	var tx1 = dd.create("type=textbox&id=10002&x=150&y=130&w=200&nm=mytextbox");
	var btn1 = dd.create("type=button&id=10003&x=150&y=170&w=200&nm=mybtn");
	
	btn1.OnClick = click;
	
}


main1();
