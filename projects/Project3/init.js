// javascript, chakra engine

var b = dd.select("JSRunButton");
var str = "Running";
b.set("text=" + str);

var txt = dd.select("TX1");
//txt.set("color=13421772"); // light gray

dd.select("s3");
var k1 = dd.create("type=listbox&x=200&y=200&w=100&nm=mylsbox");

k1.set("add&str=hoi1");
k1.set("add&str=hoi2");
k1.set("add&str=hoi3");
k1.set("select&no=2");

//var n = k1.get("select");


b.set("enable=0");
