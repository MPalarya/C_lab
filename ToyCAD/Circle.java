import java.util.ArrayList;

public class Circle extends Shape {

	private double r;

	protected Circle() {
		super();
	}

	public Circle(ArrayList<String> arguments) {
		super(arguments);
		
		double rad = Double.parseDouble(arguments.get(arguments.size()-1));
		this.r = Math.abs(rad);
	}

	@Override
	public double area() {
		return Math.PI * Math.pow(r, 2);
	}

	@Override
	public double circumference() {
		return 2 * Math.PI * r;
	}

	@Override
	public boolean isInside(Point p) {
		return points.get(0).distance(p) <= r;
	}

	@Override
	protected <T extends Shape> void copyFrom(T other) {
		super.copyFrom(other);
		this.r = ((Circle) other).r;
	}

}
