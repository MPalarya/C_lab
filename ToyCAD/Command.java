import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.function.Function;

public class Command {

	public enum CommandType {
		NEW {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				try {
					String className = cmd.arguments.get(0).toLowerCase();
					className = className.substring(0, 1).toUpperCase() + className.substring(1);

					Class<?> class1 = Class.forName(className);
					Constructor<?> constructor = class1.getConstructor(ArrayList.class);
					Shape shape = (Shape) constructor.newInstance(cmd.arguments);
					shapes.add(shape);
					System.out.println(shape.getID());

				} catch (Exception e) {
					System.out.println("Error: New - bad arguments.");
				}
			}
		},
		DELETE {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				int id = Integer.parseInt(cmd.arguments.get(0));
				Shape s = Command.getShape(shapes, id);
				if (s != null) {
					shapes.remove(s);
				}
			}
		},
		MOVE {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				int id = Integer.parseInt(cmd.arguments.get(0));
				double xOffset = Double.parseDouble(cmd.arguments.get(1));
				double yOffset = Double.parseDouble(cmd.arguments.get(2));
				Shape s = Command.getShape(shapes, id);
				if (s != null) {
					s.move(xOffset, yOffset);
				}
			}
		},
		COPY {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				int id = Integer.parseInt(cmd.arguments.get(0));
				double xOffset = Double.parseDouble(cmd.arguments.get(1));
				double yOffset = Double.parseDouble(cmd.arguments.get(2));
				Shape s = Command.getShape(shapes, id);
				if (s == null) {
					return;
				}
				
				Shape copied = s.copy(s.getClass(), xOffset, yOffset);
				if (copied == null) {
					System.out.println("Error: Copy - failed.");
					return;
				}
				shapes.add(copied);
				System.out.println(String.format("%d", copied.getID()));
			}
		},
		AREA {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				Shape.Color c = Shape.Color.valueOf(cmd.arguments.get(0));
				Command.sumAllFunc(shapes, c, Shape::area);
			}
		},
		COLOR {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				Shape.Color c = Shape.Color.valueOf(cmd.arguments.get(0));
				int id = Integer.parseInt(cmd.arguments.get(1));
				Shape s = Command.getShape(shapes, id);
				s.setColor(c);
			}
		},
		CIRCUMFERENCE {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				Shape.Color c = Shape.Color.valueOf(cmd.arguments.get(0));
				Command.sumAllFunc(shapes, c, Shape::circumference);
			}
		},
		IS_INSIDE {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				int id = Integer.parseInt(cmd.arguments.get(0));
				double x = Double.parseDouble(cmd.arguments.get(1));
				double y = Double.parseDouble(cmd.arguments.get(2));
				int res = 0;
				Shape s = Command.getShape(shapes, id);
				if (s.isInside(new Point(x, y))) {
					res = 1;
				}
				System.out.println(String.format("%d", res));
			}
		},
		PRINT {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				int height = Integer.parseInt(cmd.arguments.get(0));
				int width = Integer.parseInt(cmd.arguments.get(1));
				String[][] drawing = new String[width][height];

				int xOffset = (int) Math.floor(width / 2);
				int yOffset = (int) Math.floor(height / 2);

				for (int y = height - 1; y >= 0; y--) {
					for (int x = 0; x < width; x++) {
						
						drawing[x][y] = x - xOffset == 0 && y - yOffset == 0 ? "+"
								: x - xOffset == 0 ? "|" : y - yOffset == 0 ? "-" : " ";
						
						for (Shape s : shapes) {
							if (s.isInside(new Point(x - xOffset, y - yOffset))) {
								drawing[x][y] = s.getColor().getCode() + "x" + Shape.Color.getResetCode();
								break;
							}
						}
						System.out.print(drawing[x][y] + " ");
					}
					System.out.print('\n');
				}
			}
		},
		EXIT {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				return;
			}
		},
		UNKNOWN {
			public void execute(ArrayList<Shape> shapes, Command cmd) {
				System.out.println("Error: Command not found!");
			}
		};

		public abstract void execute(ArrayList<Shape> shapes, Command cmd);

	}

	private CommandType commandType;
	private ArrayList<String> arguments;

	public Command() {
		commandType = CommandType.UNKNOWN;
		arguments = new ArrayList<String>();
	}

	public void execute(ArrayList<Shape> shapes) {
		try {
			commandType.execute(shapes, this);
		} catch (Exception e) {
			System.out.println("Error: bad arguments.");
		}
	}

	public CommandType parseCommand(String command) {
		arguments.clear();
		arguments.addAll(Arrays.asList(command.split("\\s+")));

		if (arguments.isEmpty()) {
			return CommandType.UNKNOWN;
		}

		commandType = getCommandType(arguments.remove(0));
		return commandType;
	}

	private static CommandType getCommandType(String str) {
		try {
			return CommandType.valueOf(str.toUpperCase());
		} catch (Exception IllegalArgumentException) {
			return CommandType.UNKNOWN;
		}
	}

	private static Shape getShape(ArrayList<Shape> shapes, int id) {
		for (Shape s : shapes) {
			if (s.getID() == id) {
				return s;
			}
		}
		return null;
	}

	private static void sumAllFunc(ArrayList<Shape> shapes, Shape.Color color, Function<Shape, Double> func) {
		double sum = 0;
		for (Shape s : shapes) {
			if (s.getColor().equals(color)) {
				sum += func.apply(s);
			}
		}
		System.out.println(String.format("%.2f", sum));
	}

}
