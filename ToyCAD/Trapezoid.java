import java.util.Comparator;
import java.util.ArrayList;

public class Trapezoid extends Shape {

	private enum Vertices {
		ButtomLeft, ButtomRight, TopLeft, TopRight
	}

	protected Trapezoid() {
		super();
	}

	public Trapezoid(ArrayList<String> arguments) {
		super(arguments);
		sortVertices();
	}

	private void sortVertices() {
		points.sort(new Comparator<Point>() {
			public int compare(Point p1, Point p2) {
				double c1 = p1.y;
				double c2 = p2.y;

				if (c1 < c2) {
					return -1;
				} else if (c2 < c1) {
					return 1;
				}

				c1 = p1.x;
				c2 = p2.x;
				if (c1 < c2) {
					return -1;
				} else if (c2 < c1) {
					return 1;
				} else {
					return 0;
				}
			}
		});
	}
	
	@Override
	public double area() {
		Point bl = points.get(Vertices.ButtomLeft.ordinal());
		Point br = points.get(Vertices.ButtomRight.ordinal());
		Point tl = points.get(Vertices.TopLeft.ordinal());
		Point tr = points.get(Vertices.TopRight.ordinal());

		double h = tl.y - bl.y;
		double a = br.x - bl.x;
		double b = tr.x - tl.x;

		return (a + b) * h / 2;
	}

	@Override
	public double circumference() {
		Point bl = points.get(Vertices.ButtomLeft.ordinal());
		Point br = points.get(Vertices.ButtomRight.ordinal());
		Point tl = points.get(Vertices.TopLeft.ordinal());
		Point tr = points.get(Vertices.TopRight.ordinal());

		double a = br.x - bl.x;
		double b = tr.x - tl.x;
		double c = tl.distance(bl);
		double d = tr.distance(br);

		return a + b + c + d;
	}

	@Override
	public boolean isInside(Point p) {
		Point bl = points.get(Vertices.ButtomLeft.ordinal());
		Point br = points.get(Vertices.ButtomRight.ordinal());
		Point tl = points.get(Vertices.TopLeft.ordinal());
		Point tr = points.get(Vertices.TopRight.ordinal());

		double abcd = area();
		double bl_br_p = Triangle.triangleArea(bl, br, p);
		double bl_tl_p = Triangle.triangleArea(bl, tl, p);
		double br_tr_p = Triangle.triangleArea(br, tr, p);
		double tl_tr_p = Triangle.triangleArea(tl, tr, p);

		return abcd == bl_br_p + bl_tl_p + br_tr_p + tl_tr_p;
	}

}
