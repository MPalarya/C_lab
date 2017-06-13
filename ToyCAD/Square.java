import java.util.ArrayList;

public class Square extends Rectangle {

	protected Square() {
		super();
	}

	public Square(ArrayList<String> arguments) {
		super(arguments);
		
		double side = Double.parseDouble(arguments.get(arguments.size()-1));
	
		Point p1 = this.points.get(0);
		p1.move(-side/2, -side/2);
		
		Point p2 = new Point(p1.x + side, p1.y + side);
		points.add(p2);
	}

}
