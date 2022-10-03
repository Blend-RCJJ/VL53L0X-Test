import processing.serial.*;

Serial serial;

int deg;
int dist;

float x, y;
float _x, _y;

float scale = 0.7;

boolean isFirst = true;

void setup() {
    size(1200, 700);
    serial = new Serial(this, "/dev/tty.usbmodem142101", 115200);
    reset();
}

void draw() {
    colorMode(HSB, 2000);
    translate(width / 2, height / 2);

    if (serial.available() >= 5) {
        if (serial.read() == 'F') {
            deg  = serial.read() * 256 + serial.read();
            dist = serial.read() * 256 + serial.read();

            serial.clear();

            println(dist);

            if (dist == 0 || dist >= 2000) {
                return;
            }

            x = dist * sin(radians(-deg - 180)) * scale;
            y = dist * cos(radians(-deg - 180)) * scale;

            if (!isFirst) {
                stroke(constrain(dist * 3 - 100, 0, 1300), 1000, 2000, 1000);
                line(x, y, _x, _y);
            } else {
                isFirst = false;
            }

            _x = x;
            _y = y;
        }
    }
}

void reset() {
    translate(width / 2, height / 2);
    background(30);

    noFill();

    stroke(#FFFFFF, 100);
    strokeWeight(2);

    for (int i = 1; i <= 4; i++) {
        ellipse(0, 0, 500 * i * scale, 500 * i * scale);
    }

    strokeWeight(20);
}
