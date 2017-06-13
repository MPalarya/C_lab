import java.util.ArrayList;

public class Rectangle extends Shape {

	protected Rectangle() {
		super();
	}
	
	public Rectangle(ArrayList<String> arguments) {
		super(arguments);
	}
	
	@Override
	public double area() {
		return Math.abs((points.get(0).x - points.get(1).x) * (points.get(0).y - points.get(1).y));
	}

	@Override
	public double circumference() {
		return 2 * (Math.abs(points.get(0).x - points.get(1).x) + Math.abs(points.get(0).y - points.get(1).y));
	}

	@Override
	public boolean isInside(Point p) {
		return p.x <= Math.max(points.get(0).x, points.get(1).x)
				&& p.x >= Math.min(points.get(0).x, points.get(1).x)
				&& p.y <= Math.max(points.get(0).y, points.get(1).y)
				&& p.y >= Math.min(points.get(0).y, points.get(1).y);
	}

}
