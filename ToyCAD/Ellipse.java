import java.util.ArrayList;

public class Ellipse extends Shape {

	private double d;

	protected Ellipse() {
		super();
	}
	
	public Ellipse(ArrayList<String> arguments) {
		super(arguments);
		
		double d = Double.parseDouble(arguments.get(arguments.size()-1));
		this.d = Math.abs(d);
	}

	private Point getAxises() {
		// returns p where p.x represents the long axis (a),
		// and p.y represents the short axis (b).
		Point p1 = points.get(0);
		Point p2 = points.get(1);
		Point center = new Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

		double center_to_p1 = p1.distance(center);
		double p1_to_edge = (this.d - 2 * center_to_p1) / 2;

		double a = center_to_p1 + p1_to_edge;
		double b = Math.sqrt(Math.pow(this.d / 2, 2) - Math.pow(center_to_p1, 2));

		return new Point(a, b);
	}

	@Override
	public double area() {
		Point axises = getAxises();
		double a = axises.x;
		double b = axises.y;

		return a * b * Math.PI;
	}

	@Override
	public double circumference() {
		Point axises = getAxises();
		double a = axises.x;
		double b = axises.y;

		// Ramanujan's approximation for circumference of an ellipse:
		double h = Math.pow(a - b, 2) / Math.pow(a + b, 2);
		double c = Math.PI * (a + b) * (1 + (3 * h / (10 + Math.sqrt(4 - 3 * h))));
		return c;
	}

	@Override
	public boolean isInside(Point p) {
		return points.get(0).distance(p) + points.get(1).distance(p) <= d;
	}

	@Override
	protected <T extends Shape> void copyFrom(T other) {
		super.copyFrom(other);
		this.d = ((Ellipse) other).d;
	}

}
