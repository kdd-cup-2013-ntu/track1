import java.io.*;
import java.util.*;

public class RowMerger {
	
	private ArrayList<Integer> trainLabelList;
	private ArrayList<Integer> validLabelList;
	
	private ArrayList<String> trainFeatureList;
	private ArrayList<String> validFeatureList;

	public RowMerger(String pathToTrainSVM, String pathToValidSVM, String pathToValidCSV, String pathToAnsCSV){
		
		readSVMFiles(pathToTrainSVM, pathToValidSVM);
		readCSVFiles(pathToValidCSV, pathToAnsCSV);
	}

	public void readSVMFiles(String pathToTrainSVM, String pathToValidSVM){
		
		trainLabelList = new ArrayList<Integer>();
		validLabelList = new ArrayList<Integer>();
		
		trainFeatureList = new ArrayList<String>();
		validFeatureList = new ArrayList<String>();
		
		BufferedReader reader = null;
		
		try{
			reader = new BufferedReader(new FileReader(pathToTrainSVM));

			String line = null;

			while((line = reader.readLine()) != null){
				int state = 0;

				for(int i = 0; i < line.length(); i++){
					
					char ch = line.charAt(i);

					switch(state){
						case 0:
							if(ch == '\t' || ch == ' '){
								trainLabelList.add(Integer.parseInt(line.substring(0,i)));
								state = 1;
							}
							break;
						case 1:
							if(ch != '\t' && ch != ' '){
								trainFeatureList.add(line.substring(i,line.length()));
								state = 2;
							}
							break;
						default:
							break;
					}

					if(state == 2){
						break;
					}
				}
			}
			
			reader.close();
			reader = new BufferedReader(new FileReader(pathToValidSVM));

			while((line = reader.readLine()) != null){
				int state = 0;

				for(int i = 0; i < line.length(); i++){
					
					char ch = line.charAt(i);

					switch(state){
						case 0:
							if(ch == '\t' || ch == ' '){
								validLabelList.add(Integer.parseInt(line.substring(0,i)));
								state = 1;
							}
							break;
						case 1:
							if(ch != '\t' && ch != ' '){
								validFeatureList.add(line.substring(i,line.length()));
								state = 2;
							}
							break;
						default:
							break;
					}

					if(state == 2){
						break;
					}
				}
			}

			reader.close();

		}catch(Exception ex){
			ex.printStackTrace();
		}
	}

	public void readCSVFiles(String pathToValidCSV, String pathToAnsCSV){
			
		BufferedReader readerV = null;
		BufferedReader readerA = null;
		
		try{
			readerV = new BufferedReader(new FileReader(pathToValidCSV));
			readerA = new BufferedReader(new FileReader(pathToAnsCSV));
			
			String line = null;
			String[] token = null;
			String[] author = null;
			String[] confirm = null;

			readerV.readLine();
			readerA.readLine();

			int ptr = 0;

			while((line = readerV.readLine()) != null){
				
				token = line.split(",");
				author = token[1].split("[ \t]");
				
				line = readerA.readLine();
				token = line.split(",");
				confirm = token[1].split("[ \t]");

				HashSet<String> confirmSet = new HashSet<String>();
				
				for(String c: confirm){
					if(c == ""){
						continue;
					}
					confirmSet.add(c);
				}

				for(String a: author){
					if(a == ""){
						continue;
					}

					if(confirmSet.contains(a)){
						validLabelList.set(ptr, 1);
					}else{
						validLabelList.set(ptr, 0);
					}

					ptr++;
				}
			}

			if(ptr != validLabelList.size()){
				System.err.println("Error: dimension mismatched.");
				System.exit(-1);
			}

			readerA.close();
			readerV.close();

		}catch(Exception ex){
			ex.printStackTrace();
		}
	}

	public void merge(String pathToOutput){
		
		PrintWriter writer = null;

		try{
			writer = new PrintWriter(new FileWriter(pathToOutput));
			

			for(int i = 0; i < trainLabelList.size(); i++){
				
				if(trainLabelList.get(i) > 0){
					writer.println(1 + " " + trainFeatureList.get(i));
				}else{
					writer.println(0 + " " + trainFeatureList.get(i));
				}
			}
			
			for(int i = 0; i < validLabelList.size(); i++){
				
				if(validLabelList.get(i) > 0){
					writer.println(1 + " " + validFeatureList.get(i));
				}else{
					writer.println(0 + " " + validFeatureList.get(i));
				}
			}

			writer.close();

		}catch(Exception ex){
			ex.printStackTrace();
		}
	}
	
	public static void main(String[] args){

		if(args.length != 5){
			System.err.println("Usage: java RowMerger [Train.svm] [Valid.svm] [Valid.csv] [ValidSolution.csv] [output]");
			System.exit(-1);
		}
		
		RowMerger rm = new RowMerger(args[0], args[1], args[2], args[3]);
		rm.merge(args[4]);
		return;
	}
}
