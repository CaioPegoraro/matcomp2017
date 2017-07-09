import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;

class Node{

    int numberNeighbors;
    int[] flows;
    int[] capacities;

    Node(int neighbors){
        this.numberNeighbors = neighbors;
        flows = new int[neighbors];
        capacities = new int[neighbors];
    }
    void setNeighborFlow(int index, int flow){
        flows[index]=flow;
        capacities[index]=flow;
    }

    @Override
    public String toString() {
        String string = new String("");
        for (int i = 0; i<numberNeighbors;i++){
            String auxFlow = new String("");
            String auxCap = new String("");
            if (flows[i]>9) {
                auxFlow= "" + flows[i];
            } else {
                auxFlow = " "+flows[i];
            }
            if ( capacities[i] > 9 ){
                auxCap = "" + capacities[i];
            } else {
                auxCap = capacities[i]+" ";
            }
            string = string.concat(auxFlow + " ");
        }
        return string;
    }
}

/** first index is always the source
* * last index is always the sink
* *
* *
* *
* */
class Graph{

    int numberNodes;
    Node[] nodes;

    Graph(int numberNodes){
        this.numberNodes = numberNodes;
        this.nodes = new Node[numberNodes];
    }

    void setNode(int index,Node node){
        nodes[index]=node;
    }

    public void print() {
        for (int i = 0; i<numberNodes;i++){
            System.out.println(nodes[i]);
        }
    }

    public void findPath(int start, ArrayList<Integer> parents ) {
        ArrayList<Integer> path = new ArrayList<Integer>();
        if (start==numberNodes-1){
            parents.add(start);
        } else {
            for (int i = 0; i < numberNodes; i++) {
                int lFlow = nodes[start].flows[i];
                if (lFlow > 0 && !parents.contains(i)) {
                    parents.add(start);
                    int sizeBefore = parents.size();
                    findPath(i,parents);
                    if(sizeBefore==parents.size()){
                        parents.remove(parents.size()-1);
                    }
                }
            }
        }
    }

    public Integer getFlow(Integer source, Integer target) {
        return nodes[source].flows[target];
    }

    public void addFlow(Integer source, Integer target, Integer minFlow) {
        nodes[source].flows[target]= nodes[source].flows[target] + minFlow;
    }
}

public class Main {

    public static void main(String[] args) {

        Scanner in;
        if (args.length>0){
            try {
                in = new Scanner(new FileReader(args[0]));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                in = new Scanner(System.in);
            }
        } else {
            in = new Scanner(System.in);
        }
        System.out.print("Number of nodes: ");
        int numberNodes = in.nextInt();
        System.out.println(numberNodes);

        Graph residualGraph = new Graph(numberNodes);

        for (int i = 0; i<numberNodes;i++){
            Node residualNode = new Node(numberNodes);
            for (int j = 0; j<numberNodes;j++){
                int cost = in.nextInt();
                residualNode.setNeighborFlow(j,cost);
            }
            residualGraph.setNode(i,residualNode);
        }
        residualGraph.print();
        ArrayList<Integer> path = new ArrayList<Integer>();
        residualGraph.findPath(0, path);
        int totalFlow = 0;
        while (path.size()>0) {
            Integer minFlow = residualGraph.getFlow(path.get(0), path.get(1));
            for (int i = 1; i < path.size() - 1; i++) {
                minFlow = Math.min(minFlow, residualGraph.getFlow(path.get(i), path.get(i + 1)));
            }
            totalFlow+=minFlow;
            System.out.println("path:" + path + "  " + minFlow);

            for (int i = 0; i < path.size() - 1; i++) {
                residualGraph.addFlow(path.get(i), path.get(i+1),-minFlow);
                residualGraph.addFlow(path.get(i+1), path.get(i),minFlow);
            }

            residualGraph.print();
            path.clear();
            residualGraph.findPath(0, path);
        }

        System.out.println("Max Flow = " + totalFlow);
    }
}
