import java.util.ArrayList;
import java.util.Scanner;

public class ToyCAD {

	public static void main(String[] args) {
		ArrayList<Shape> shapes = new ArrayList<Shape>();
		Command cmd = new Command();
		Scanner scan = new Scanner(System.in);
		String line;
		
		while (true) {
			line = scan.nextLine().toUpperCase();
			if (cmd.parseCommand(line) == Command.CommandType.EXIT) {
				break;
			}
			cmd.execute(shapes);
		}
		scan.close();
	}

}
