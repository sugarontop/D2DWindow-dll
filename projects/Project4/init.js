// javascript, chakra engine


function main()
{
//	dd.select("s1");
//	var k3 = dd.create("type=grid&id=10105&x=650&y=50&cx=150&cy=450&nm=gd1");
//	k3.set("color=#FFFFA0&bkcolor=#111111");
//	k3.set("add&title=ABCDEFG1&val=000000");


	dd.select("s4");
	var k1 = dd.create("type=testobj1&id=10101&x=100&y=100&cx=50&cy=50&nm=vti");
	var k2 = dd.create("type=testobj1&id=10102&x=100&y=200&cx=50&cy=50&nm=vwo");
	var k3 = dd.create("type=testobj1&id=10103&x=150&y=210&cx=50&cy=50&nm=qqq");
	
//	var k1 = dd.create("type=listbox&id=10101&x=150&y=90&cx=250&cy=500&nm=vti");
	
//	k1.set("add&str=ABCDEFG2");
//	k1.set("add&str=ABCDEFG3");
//	k1.set("add&str=ABCDEFG4");
	
	var t1 = dd.select("TX1");
	t1.set("color=#FFFFA0");
	
	var t2 = dd.select("TX2");
	t2.set("text=double click vti");


}
function mv()
{
	dd.select("vti").move(500,200);
	dd.select("vwo").move(200,250);
	dd.select("qqq").move(300,100);
}


main();
mv();



	
