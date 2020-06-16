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
			s.start();
		} catch (Exception e) {
			System.err.println("Internal error: " + e.getMessage());
		}
	}

}
