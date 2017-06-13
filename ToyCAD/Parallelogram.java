import java.util.ArrayList;

public class Parallelogram extends Triangle {

	protected Parallelogram() {
		super();
	}

	public Parallelogram(ArrayList<String> arguments) {
		super(arguments);
	}
	
	@Override
	public double area() {
		return super.area() * 2;
	}

	@Override
	public double circumference() {
		double a = points.get(0).distance(points.get(1));
		double b = points.get(1).distance(points.get(2));
		return 2 * (a + b);
	}

	@Override
	public boolean isInside(Point p) {
		Point a = points.get(0);
		Point b = points.get(1);
		Point c = points.get(2);
		Point d = new Point(c.x + a.x - b.x, c.y + a.y - b.y);

		if (super.isInside(p)) {
			return true;
		}

		// check the completing triangle of the parallelogram:
		double deltax = d.x - b.x;
		double deltay = d.y - b.y;
		b.move(deltax, deltay);

		boolean res = false;
		if (super.isInside(p)) {
			res = true;
		}
		b.move(-deltax, -deltay);
		return res;
	}

}
