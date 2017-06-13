
public class Point {

	double x, y;

	Point(double x, double y) {
		this.x = x;
		this.y = y;
	}

	Point(Point other) {
		this.x = other.x;
		this.y = other.y;
	}

	final double distance(Point other) {
		return Math.sqrt(Math.pow(this.x - other.x, 2) + Math.pow(this.y - other.y, 2));
	}

	final void move(double xOffset, double yOffset) {
		x += xOffset;
		y += yOffset;
	}

	public String toString() {
		return String.format("(%.2f,%.2f)", this.x, this.y);
	}
}
