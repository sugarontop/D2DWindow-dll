// javascript, chakra engine


function main()
{

	dd.select("s4");
	var k1 = dd.create("type=testobj1&id=10101&x=150&y=90&cx=50&cy=50&nm=vti");
	var k2 = dd.create("type=testobj1&id=10102&x=150&y=150&cx=50&cy=50&nm=vwo");
	var k3 = dd.create("type=testobj1&id=10103&x=150&y=210&cx=50&cy=50&nm=qqq");
	
//	var k1 = dd.create("type=listbox&id=10101&x=150&y=90&cx=250&cy=500&nm=vti");
	
//	k1.set("add&str=ABCDEFG1");
//	k1.set("add&str=ABCDEFG2");
//	k1.set("add&str=ABCDEFG3");
//	k1.set("add&str=ABCDEFG4");
	
	
	
}
function mv()
{
	dd.select("vti").move(500,500);
	dd.select("vwo").move(200,50);
}

main();
mv();



var t1 = dd.select("TX1");
t1.set("color=#FFFFA0");
	
