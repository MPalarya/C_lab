import java.util.ArrayList;

public abstract class Shape {

	public enum Color {
		BLUE {
			@Override
			public String getCode() {
				return "\u001B[34m";
			}
		},
		GREEN {
			@Override
			public String getCode() {
				return "\u001B[32m";
			}
		},
		RED {
			@Override
			public String getCode() {
				return "\u001B[31m";
			}
		},
		YELLOW {
			@Override
			public String getCode() {
				return "\u001B[33m";
			}
		};

		public abstract String getCode();

		public static String getResetCode() {
			return "\u001B[0m";
		}
	}

	private static int idCounter = 0;

	private int id;
	private Color color;
	protected ArrayList<Point> points = new ArrayList<Point>();

	protected Shape() {
		setID(idCounter++);
	}

	public Shape(ArrayList<String> arguments) {
		this();
		setColor(Shape.Color.valueOf(arguments.get(1).toUpperCase()));
		
		double[] args = new double[arguments.size() - 2];
		for (int i = 0; i < arguments.size() - 2; i++) {
			args[i] = Double.parseDouble(arguments.get(i + 2));
		}
		fromArguments(args);
	}

	protected void fromArguments(double...args) {
		int loopEnd = args.length % 2 == 0 ? args.length : args.length - 1;

		for (int i = 0; i < loopEnd; i += 2) {
			points.add(new Point(args[i], args[i + 1]));
		}
	}
	
	public abstract double area();

	public abstract double circumference();

	public final void move(double xOffset, double yOffset) {
		for (Point p : points) {
			p.move(xOffset, yOffset);
		}
	}

	protected <T extends Shape> void copyFrom(T other) {
		this.setColor(other.getColor());
		this.points.clear();
		for (Point p : other.points) {
			this.points.add(new Point(p));
		}
	}

	public final <T extends Shape> T copy(Class<T> class1, double xOffset, double yOffset) {
		try {
			T newObj = (T) class1.newInstance();
			newObj.copyFrom(this);
			newObj.move(xOffset, yOffset);
			return newObj;
		} catch (InstantiationException | IllegalAccessException e) {
			return null;
		}
	}

	public abstract boolean isInside(Point p);

	public final int getID() {
		return id;
	}

	private final void setID(int id) {
		this.id = id;
	}

	public final Color getColor() {
		return color;
	}

	public final void setColor(Color color) {
		this.color = color;
	}

}
