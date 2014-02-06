String temperture[];
String second[];
float count = 0.0;
float sec1 = 0.0;
int sec2 = 0;
int loop_count = 0;
color[] col = new color[1];
float old_ty = 0;


void setup() {
	size(650, 400);
	second = loadStrings("/home/takuma1/second.txt");
	sec1 = parseFloat(second[0]);
	sec2 = parseInt(second[0]);
	frameRate(50);
	initGraph();
}

void draw() {
	temperture = loadStrings("/home/takuma1/temperture.txt");
	float tx = map(50.0 + count, 0, width, 0, width);
	float ty = parseFloat(temperture[0]);
	if((ty != old_ty) && ((int)count % 50 == 0)) {
			fill(47);
			textSize(20);
			text(temperture[0],575,30);
			ellipse(tx, 300 - 6 * ty, 3, 3);
	}
	textSize(20);
	text(temperture[0],575,30);
	text("°C",600,30);
	fill(col[0]);
	if(count == 0.0)
		ellipse(tx, 300 - 6 * ty, 3, 3);
	else
		ellipse(tx, 300 - 6 * old_ty, 3, 3);
	if (tx == 550) {
		loop_count++;
		initGraph();
	}
	old_ty = ty;
	count = count + 1/sec1;
}

void initGraph() {
	background(47);
	count = 0;
	stroke(#ffffff);
	fill(#ffffff);
	strokeWeight(1);
	textSize(15);
	line(50,0,50,400);
	line(50,60,550,60);
	line(50,90,550,90);
	line(50,120,550,120);
	line(50,150,550,150);
	line(50,150,550,150);
	line(50,180,550,180);
	line(50,210,550,210);
	line(50,240,550,240);
	line(50,270,550,270);
	line(0,300,650,300);
	line(50,330,550,330);
	line(50,360,550,360);
	text(40,30,65);
	text(35,30,95);
	text(30,30,125);
	text(25,30,155);
	text(20,30,185);
	text(15,30,215);
	text(10,30,245);
	text(5,35,275);
	text(10 * loop_count,40,315);
	text(-5,30,335);
	text(-10,25,365);
	text("°C",30,20);
	text(sec2 + "s",625,315);
	text(10 * loop_count + 1,100,315);
	text(10 * loop_count + 2,150,315);
	text(10 * loop_count + 3,200,315);
	text(10 * loop_count + 4,250,315);
	text(10 * loop_count + 5,300,315);
	text(10 * loop_count + 6,350,315);
	text(10 * loop_count + 7,400,315);
	text(10 * loop_count + 8,450,315);
	text(10 * loop_count + 9,500,315);
	text(10 * loop_count + 10,550,315);
	noStroke();
	col[0] = color(255, 127, 31);
}
