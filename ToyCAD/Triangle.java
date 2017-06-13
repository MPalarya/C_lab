import java.util.ArrayList;

public class Triangle extends Shape {

	protected Triangle() {
		super();
	}

	public Triangle(ArrayList<String> arguments) {
		super(arguments);
	}
	
	@Override
	public double area() {
		return Triangle.triangleArea(points.get(0), points.get(1), points.get(2));
	}

	@Override
	public double circumference() {
		double d1 = points.get(0).distance(points.get(1));
		double d2 = points.get(0).distance(points.get(2));
		double d3 = points.get(1).distance(points.get(2));

		return d1 + d2 + d3;
	}

	@Override
	public boolean isInside(Point p) {
		Point a = points.get(0);
		Point b = points.get(1);
		Point c = points.get(2);

		double abc = Triangle.triangleArea(a, b, c);
		double abp = Triangle.triangleArea(a, b, p);
		double apc = Triangle.triangleArea(a, p, c);
		double pbc = Triangle.triangleArea(p, b, c);

		return abc == abp + apc + pbc;
	}

	static double triangleArea(Point p1, Point p2, Point p3) {
		return Math.abs(p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2;
	}

}
