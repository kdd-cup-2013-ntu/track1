import java.io.*;
import java.util.*;

public class Ensemble {

	private int numOfPrediction;
	private int numOfInstance;
	private ArrayList<ArrayList<Double>> feature;
	private ArrayList<Double> weight;

	public Ensemble(){

		feature = new ArrayList<ArrayList<Double>>();
		weight = new ArrayList<Double>();
		
		numOfPrediction = 0;
		numOfInstance = 0;
	}

	public void appendPrediction(String filename){
		try{
			BufferedReader reader = new BufferedReader(new FileReader(filename));
			
			feature.add(new ArrayList<Double>());
			weight.add(1.0);

			String line = null;

			while((line = reader.readLine()) != null){
				feature.get(numOfPrediction).add(Double.parseDouble(line));	
			}

			reader.close();

			if(numOfPrediction == 0){
				numOfInstance = feature.get(0).size();
			}else{
				if(numOfInstance != feature.get(numOfPrediction).size()){
					System.err.println("Dimension mismatched!");
					System.exit(-1);
				}
			}
			
			numOfPrediction++;

		}catch(Exception ex){
			ex.printStackTrace();
		}
	}

	public void assignWeight(int d, double w){
		weight.set(d, w);
	}

	public void output(String filename){
		try{
			
			PrintWriter writer = new PrintWriter(new FileWriter(filename));

			String line = null;

			for(int i = 0; i < numOfInstance; i++){
				double value = 0.0;
				for(int j = 0; j < feature.size(); j++){
					value += weight.get(j)*feature.get(j).get(i);
				}
				writer.println(value);
			}

			writer.close();

		}catch(Exception ex){
			ex.printStackTrace();
		}
	}

	public void scale(){
		
		double[] min = new double[numOfPrediction];
		double[] max = new double[numOfPrediction];
		
		for(int i = 0; i < feature.size(); i++){
			min[i] = feature.get(i).get(0);
			max[i] = feature.get(i).get(0);
		}

		for(int i = 1; i < numOfInstance; i++){
			for(int j = 0; j < feature.size(); j++){
				
				double value = feature.get(j).get(i);
				
				if(min[j] > value){
					min[j] = value;
				}
				
				if(max[j] < value){
					max[j] = value;
				}
			}
		}
		
		for(int i = 0; i < numOfInstance; i++){
			for(int j = 0; j < feature.size(); j++){
				double value = feature.get(j).get(i);
				feature.get(j).set(i, (value - min[j]) / (max[j] - min[j]));
			}
		}
	}

	public static void main(String[] args){
		if(args.length < 2){
			System.err.println("Usage: Ensemble [prediction]:[weight] ... [output] ");
			System.exit(-1);
		}

		Ensemble es = new Ensemble();

		for(int i = 0; i < args.length - 1; i++){
			String[] token = args[i].split(":");
			es.appendPrediction(token[0]);
			es.assignWeight(i, Double.parseDouble(token[1]));
		}

		es.scale();
		es.output(args[args.length-1]);
	}
}
