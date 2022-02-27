// javascript, chakra engine

var gk1;
function f2(a)
{

	var k = dd.select("JSRunButton");
	k.set("text=" + a );
}
function f1( t )
{
	var b = dd.select("JSRunButton");
	var str = t; //"Running100";
	b.set("text=" + str);


}

function main1()
{
	var b = dd.select("JSRunButton");
	//var str = "Running";
	//b.set("text=" + str);
	
	var txt = dd.select("TX1");
	//txt.set("color=13421772"); // light gray

	dd.select("s3");
	var k1 = dd.create("type=listbox&x=200&y=200&w=100&nm=mylsbox");

	k1.set("add&str=hoi1");
	k1.set("add&str=hoi2");
	k1.set("add&str=hoi3");
	//k1.set("select&no=2");

	//var n = k1.get("select");
	k1.OnChanged = f2;
	gk1 = k1;
	//k1.OnChanged("test");
	//b.set("enable=0");
}

main1();