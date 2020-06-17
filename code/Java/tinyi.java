import interpreter.command.BlocksCommand;
import lexical.LexicalAnalysis;
import syntatic.SyntaticAnalysis;

public class tinyi {
	public static void main(String args[]) {
		if (args.length != 1) {
			System.out.println("Usage: java tinyi [source file]");
			return;
		}

		try (LexicalAnalysis l = new LexicalAnalysis(args[0])) {
			SyntaticAnalysis s = new SyntaticAnalysis(l);

			BlocksCommand program = s.start();
			program.execute();
		} catch (Exception e) {
			System.err.println("Internal error: " + e.getMessage());
		}
	}

}
